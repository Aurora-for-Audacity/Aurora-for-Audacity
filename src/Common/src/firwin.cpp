#include <aurora.h>

void SCFirWin::Dump(const int id, const char* tag)
{
    FILE* f;
    int k;
    
    char str[32];
    if(m_adbTaps != 0)
    {
        sprintf(str, "/tmp/%s_%d", tag, id);
        std::cout << "Dumping filter " << id << " to " << str << std::endl;
        if((f = fopen(str, "w")) != 0)
        {
            k = 0;
            
            while(k < m_nNtaps)
            {
                fprintf(f, "%.18e\n", m_adbTaps[k++]);
            }
            fclose(f);
        }
    }
}

void SCFirWin::TestConv(int M, int N) // REMOVE BEFORE FLIGHT
{
    std::vector<double> a(M+N, 0.0);
    std::vector<double> b(M+N, 0.0);
    std::vector<double> c(M+N-1, 0.0);
    
    int k,i;
    
    for(k = 0; k < M; k++) a[k] = 1.0;
    for(k = 0; k < N; k++) b[k] = double(k);
    
    for(k = 0; k < (M+N)-1; k++)
    {
        for(i = 0; i < k+1; i++)
        {
            c[k] += a[i] * b[k-i];
        }
    }
    
    FILE* f;
    std::cout << "Dumping conv_test" << std::endl;
    if((f = fopen("/tmp/conv_test", "w")) != 0)
    {
        k = 0;
        
        while(k < M+N)
        {
            fprintf(f, "%.18e\n", c[k++]);
        }
        fclose(f);
    }
}


// ----------------------------------------------------------------------------
void SCFirWin::Pad(const int M, const bool center)
{
    if(!m_adbTaps)
    {
    	return;
    }

    if(M < m_nNtaps)
    {
    	return;
    }
    Aurora::Sample* yt = new Aurora::Sample [M];
    memset(yt, 0, M * sizeof(Aurora::Sample));

    int ofs = (center) ? (M-m_nNtaps)/2 : 0;
    memcpy(yt + ofs, m_adbTaps, m_nNtaps*sizeof(Aurora::Sample));

    delete [] m_adbTaps;

    m_adbTaps = yt;
    m_nNtaps  = M;
}

void SCFirWin::Sinc(std::vector<double>& x)
{
    std::vector<double>::iterator it;
    double zero = 1.0e-18;
    
    for(it = x.begin(); it != x.end(); it++)
    {
        if(*it != 0.0)
            *it = sin(*it * M_PI) / (*it * M_PI);
        else
            *it = sin(zero * M_PI) / (zero * M_PI);
    }
}

bool SCFirWin::Process()
{
    // Taken 'as-is' from fir_filter_design.py (Scipy)
    
    if(!IsOk())
    {
        return false;
    }
    
    if(m_bPassZero)
    {
        std::vector<double>::iterator it;
        it = m_vdbCutoffs.begin();    
        it = m_vdbCutoffs.insert(it, 0.0);
    }
    
    if(m_bPassNyquist)
    {
        m_vdbCutoffs.push_back(1.0);
    }
    
    int i,k;
    double alpha = 0.5 * (m_nNtaps - 1);
    
    std::vector<double> m(m_nNtaps, 0.0);
    std::vector<double> rsinc(m_nNtaps, 0.0);
    std::vector<double> lsinc(m_nNtaps, 0.0);
    
    for(i = 0; i < m_nNtaps; i++)
    {
        m[i] = double(i) - alpha;
    }
    memset(m_adbTaps, 0, sizeof(Aurora::Sample)*m_nNtaps);
    
    for(i = 0; i < (int)m_vdbCutoffs.size(); i +=2 )
    {        
        for(k = 0; k < m_nNtaps; k++)
        {
            lsinc[k] = m[k] * m_vdbCutoffs[i];
            rsinc[k] = m[k] * m_vdbCutoffs[i+1];
        }
        Sinc(lsinc);
        Sinc(rsinc);
        
        for(k = 0; k < m_nNtaps; k++)
        {
            m_adbTaps[k] -= Aurora::Sample(lsinc[k] * m_vdbCutoffs[i]);
            m_adbTaps[k] += Aurora::Sample(rsinc[k] * m_vdbCutoffs[i+1]);
        }

    }
    
    Aurora::Sample* pWin = m_Window.Get();
    
    for(k = 0; k < m_nNtaps; k++)
    {
        m_adbTaps[k] *= pWin[k];
    }

    
    if(m_bScale)
    {
        double scaleFrequency;    
        if(m_vdbCutoffs[0] == 0.0)
        {
            scaleFrequency = 0.0;
        }
        else if(m_vdbCutoffs[1] == 1.0)
        {
            scaleFrequency = 1.0;
        }
        else
        {
            scaleFrequency = 0.5 * (m_vdbCutoffs[0] + m_vdbCutoffs[1]);
        }
        
        double c = 0.0;
        double s = 0.0;
                
        for(k = 0; k < m_nNtaps; k++)
        {
            c = cos(M_PI * m[k] * scaleFrequency);
            s += double(m_adbTaps[k]) * c;
        }

        for(k = 0; k < m_nNtaps; k++)
        {
            m_adbTaps[k] /= Aurora::Sample(s);
        }
    }
    return true;
}

bool SCFirWin::Create(const int    N, 
                      double       cutoff, 
                      const int    window,
                      const double width,
                      const bool   passZero,
                      const bool   scale,
                      const double fNyq)
{ return Create(N, &cutoff, 1, window, width, passZero, scale, fNyq); }

bool SCFirWin::Create(const int    N, 
                      double*      cutoffs, 
                      const int    cutoffsCount, 
                      const int    window,
                      const double width,
                      const bool   passZero,
                      const bool   scale,
                      const double fNyq)
{
    // Taken 'as-is' from fir_filter_design.py (Scipy)
 
    m_nNtaps = N;
    
    if(m_nNtaps <= 0)
    {
        m_nErr = ERR_BAD_N;
        return false;
    }
    
    if((!cutoffs) || (cutoffsCount <= 0))
    {
        m_nErr = ERR_BAD_CUTOFF_VEC;
        return false;
    }

    m_dbFnyq = fNyq;
        
    for(int i = 0; i < cutoffsCount; i++)
    {
        m_vdbCutoffs.push_back(cutoffs[i] / m_dbFnyq);

        if((m_vdbCutoffs[i] <= 0.0) || (m_vdbCutoffs[i] >= 1.0))
        {
            m_vdbCutoffs.clear();
            m_nErr = ERR_BAD_CUTOFF_VALUES;
            return false;
        }
        if((i > 0) && (m_vdbCutoffs[i] <= m_vdbCutoffs[i-1]))
        {
            m_vdbCutoffs.clear();
            m_nErr = ERR_BAD_CUTOFF_PROG;
            return false;
        }
    }

    m_dbWidth     = width;
    m_nWindowType = window;
    
    double wp1 = 0.0, wp2 = 0.0; // window parameters
    if(m_dbWidth > 0.0)
    {
        m_nWindowType = SCSignalWindows::WT_KAISER;
        double a = m_Window.KaiserAtten(m_nNtaps, m_dbWidth / m_dbFnyq);
        wp1      = m_Window.KaiserBeta(a);
    }
    m_Window.Create((unsigned int)m_nNtaps, m_nWindowType, wp1, wp2);
    
    m_bPassZero = passZero;
    m_bPassNyquist = bool(cutoffsCount & 1) ^ m_bPassZero;
    
    if((m_bPassNyquist) && ((m_nNtaps % 2) == 0))
    {
        m_vdbCutoffs.clear();
        m_nErr = ERR_PASS_NYQUIST;
        return false;
    }
    
    m_bScale = scale;
    
    Destroy();
    m_adbTaps = new Aurora::Sample [m_nNtaps];
    
    return Process();
}

void SCFirWin::Destroy()
{
    if(m_adbTaps)
    {
        delete [] m_adbTaps;
        m_adbTaps = 0;
    }
    
}

SCFirWin::SCFirWin()
: m_nNtaps(0),
  m_vdbCutoffs(),
  m_dbWidth(0.0),
  m_nWindowType(10),
  m_bPassZero(true),
  m_bPassNyquist(true),
  m_bScale(true),
  m_dbFnyq(1.0),
  m_nErr(ERR_OK),
  m_Window(),
  m_adbTaps(0)
{}

SCFirWin::SCFirWin(const int    N, 
                   double       cutoff, 
                   const int    window,
                   const double width,
                   const bool   passZero,
                   const bool   scale,
                   const double fNyq)
 : m_nNtaps(0),
   m_vdbCutoffs(),
   m_dbWidth(0.0),
   m_nWindowType(10),
   m_bPassZero(true),
   m_bPassNyquist(true),
   m_bScale(true),
   m_dbFnyq(1.0),
   m_nErr(ERR_OK),
   m_Window(),
   m_adbTaps(0)
{ Create(N, cutoff, window, width, passZero, scale, fNyq); }

SCFirWin::SCFirWin(const int    N, 
                   double*      cutoffs, 
                   const int    cutoffsCount, 
                   const int    window,
                   const double width,
                   const bool   passZero,
                   const bool   scale,
                   const double fNyq)
 : m_nNtaps(0),
   m_vdbCutoffs(),
   m_dbWidth(0.0),
   m_nWindowType(10),
   m_bPassZero(true),
   m_bPassNyquist(true),
   m_bScale(true),
   m_dbFnyq(1.0),
   m_nErr(ERR_OK),
   m_Window(),
   m_adbTaps(0)
{ Create(N, cutoffs, cutoffsCount, window, width, passZero, scale, fNyq); }

SCFirWin::~SCFirWin()
{ Destroy(); }

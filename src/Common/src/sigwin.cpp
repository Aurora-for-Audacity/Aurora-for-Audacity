
#include <aurora.h>

double arcosh(const double x)
{
    double x2 = x*x;
    if(x2 <= 1.0) return 0.0; //  ...NOT SURE...
    return log(x + sqrt(x2 - 1));
}


double SCSignalWindows::KaiserBeta(const double atten)
{
    // taken 'as-is' from scipy fir_filter_design.py
    double a = atten;
    double beta = 0.0;
    
    if(a > 50.0)
    {
        beta = 0.1102 * (a - 8.7);
    }
    else if(a > 21.0)
    {
        beta = 0.5842 * pow(a - 21.0, 0.4) + 0.07886 * (a - 21.0);
    }    
    return beta;
}

double SCSignalWindows::KaiserAtten(const int N, const double width)
{
    // taken 'as-is' from scipy fir_filter_design.py
    double a = 2.285 * double(N - 1) * M_PI * width + 7.95;
    return a;
}

double SCSignalWindows::KaiserOrd(int* N, const double ripple, const double width)
{
    // taken 'as-is' from scipy fir_filter_design.py
    double A = fabs(ripple);
    if(A < 8)
    {
        return -1;
    }
    double beta = KaiserBeta(A);
    double ntaps = (A - 7.95) / 2.285 / (M_PI * width) + 1.0;
    
    *N = int(ceil(ntaps));
    
    return beta;
}

    
void SCSignalWindows::BartlettHann()
{
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = 0.62 - 
                         0.48 * fabs(n / Aurora::Sample(m_unLength - 1) - 0.5) -
                         0.38 * cos(2.0 * M_PI * n / Aurora::Sample(m_unLength-1));
        n++;
    }
}

void SCSignalWindows::Bartlett()
{
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = 1.0 - fabs( (Aurora::Sample(n) - (m_unLength - 1.0)/2.0 ) /
                                                 ((m_unLength - 1.0)/2.0) );        
        n++;
    }
}

void SCSignalWindows::Blackman(const double a0, const double a1,
                               const double a2, const double a3,
                               const double a4)
{
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(a0 -
                                  a1 * cos( (2.0 * M_PI * n) / double(m_unLength - 1) ) +
                                  a2 * cos( (4.0 * M_PI * n) / double(m_unLength - 1) ) -
                                  a3 * cos( (6.0 * M_PI * n) / double(m_unLength - 1) ) +
                                  a4 * cos( (8.0 * M_PI * n) / double(m_unLength - 1) ));
        n++;
    }
}

void SCSignalWindows::Bohman()
{
    // TODO: cosine convolved by itself....
    
}


void SCSignalWindows::Boxcar()
{
    // AKA rectangular
    
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(1.0);
        n++;
    }
}

void SCSignalWindows::DolphChebyshev(const double alpha)
{
    m_nErr = ERR_NOT_IMPL;
    return;
    
    double* W0 = new double [m_unLength];
    
    double N    = double(m_unLength);
    double beta = cosh( (1/double(m_unLength)) * arcosh(pow(10.0, alpha)) );
    double acosh_beta = arcosh(beta);
    
    unsigned int n = 0;
    while(n < m_unLength)
    {
        W0[n] = cos( N * acos(beta * cos((M_PI * n)/N)) ) /
                cosh( N * acosh_beta );
        n++;
    }

    // TODO: to be continued...    
    
    delete [] W0;
}

void SCSignalWindows::Gaussian(const double sigma, const double p)
{
    double halfN = (double(m_unLength) - 1.0) / 2.0;
    
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(exp( -0.5 *
                                  pow(fabs((n - halfN)/(sigma*halfN)), (2.0*p)) ));
        n++;
    }
}

void SCSignalWindows::Hamming(const double alpha)
{
    double beta = 1.0 - alpha;
    
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(alpha - beta * cos((2.0*M_PI*n) /
                                          double(m_unLength - 1)));
        n++;
    }    
}

void SCSignalWindows::Hann()
{    
    unsigned int n = 0;
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(0.5 * (1.0 - cos((2.0*M_PI*n) /
                                         double(m_unLength - 1)) ));
        n++;
    }    
}

double SCSignalWindows::MBesselZeroOrder(const double n)
{
    // source: https://ccrma.stanford.edu/~jos/sasp/Kaiser_Window.html
    double I0   = 0.0,
           fact = 0.0;
    double k = 0.0,
           j = 0.0;
           
    while(k < 100.0)
    {
        I0   = pow(n/2.0, k);
        
        fact = 1.0;
        j    = 1.0;
        
        while(j < k)
        {
            fact *= (j + 1.0);
            j    += 1.0;
        }
        
        I0 = pow(I0/fact, 2.0);
        
        k += 1.0;
    }
    
    return I0;
}

void SCSignalWindows::Kaiser(const double beta)
{
    unsigned int n = 0;
    double I0 = 0.0;
    double N  = double(m_unLength);
    
    while(n < m_unLength)
    {
        I0 = MBesselZeroOrder(((2.0*n)/(N - 1)) - 1.0);        
        m_smpWindow[n] = Aurora::Sample((I0 * beta * sqrt(1.0 - pow(((2.0*n)/
                                                      (N - 1)) - 1.0, 2.0))) / 
                                  (I0 * beta));
        n++;
    }
}

void SCSignalWindows::Parzen()
{
    // TODO
    m_nErr = ERR_NOT_IMPL;
}

void SCSignalWindows::Slepian()
{
    // TODO
    m_nErr = ERR_NOT_IMPL;
}

void SCSignalWindows::Triang()
{
    unsigned int n = 0;
    
    while(n < m_unLength)
    {
        m_smpWindow[n] = Aurora::Sample(1.0 - fabs( (double(n) - (m_unLength - 1.0)/2.0 ) /
                                                           ((m_unLength + 1.0)/2.0) ));
        n++;
    }
}
      
      
void SCSignalWindows::Create(unsigned int M, const int type, double p1, double p2)
{
    if(M != m_unLength)
    {
        Destroy();
        m_unLength = M;
        m_smpWindow = new Aurora::Sample [m_unLength];
    }
    
    m_nType = type;
    
    switch(m_nType)
    {
        case WT_BARTHANN:        BartlettHann();     break;
        case WT_BARTLETT:        Bartlett();         break;
        case WT_BLACKMAN:        Blackman(0.42659, 
                                          0.49656, 
                                          0.076849); break;
        case WT_BLACKMANHARRIS:  Blackman(0.35875,
                                          0.48829,
                                          0.14128,
                                          0.01168);  break;
        case WT_BOHMAN:          Bohman();           break;
        case WT_BOXCAR:          Boxcar();           break;
        case WT_CHEBWIN:         DolphChebyshev(p1); break;
        case WT_FLATTOP:         Blackman(1.0,
                                          1.93,
                                          1.29,
                                          0.388,
                                          0.028);    break;
        case WT_GAUSSIAN:        Gaussian(p1);       break;
        case WT_GENERALGAUSSIAN: Gaussian(p1, p2);   break;
        case WT_HAMMING:         Hamming(0.54);      break;
        case WT_HANN:            Hann();             break;
        case WT_KAISER:          Kaiser(p1);         break;
        case WT_NUTTALL:         Blackman(0.355768,
                                          0.487396,
                                          0.144232,
                                          0.012604); break;
        case WT_PARZEN:          Parzen();           break;
        case WT_SLEPIAN:         Slepian();          break;
        case WT_TRIANG:          Triang();           break;
    }
    
}

void SCSignalWindows::Destroy()
{
    if(m_smpWindow)
    {
        delete [] m_smpWindow;
        m_smpWindow = 0;
    }
}
        
SCSignalWindows::SCSignalWindows()
 : m_smpWindow(0),
   m_unLength(0),
   m_nType(0),
   m_nErr(0)
{}

SCSignalWindows::SCSignalWindows(unsigned int M, const int type, double p1, double p2)
 : m_smpWindow(0),
   m_unLength(0),
   m_nType(0),
   m_nErr(0)
{ Create(M, type, p1, p2); }

SCSignalWindows::~SCSignalWindows()
{ Destroy(); }

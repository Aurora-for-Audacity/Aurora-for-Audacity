/******************************
 *
 *  Third Octave Band Filter
 *
 ******************************/

#include <aurora.h>

#include "fir-taps.cpp"
#include "iir-taps.cpp"

#define SCIPY_FILTER
//#define GP_IIR 

#ifdef DUMP_TAPS
void SCFilter::Dump(const int id, const char* tag)
{
    FILE* f;
    int k;
    
    char str[32];
    if(! m_B.empty())
    {
        sprintf(str, "/tmp/%s_%d", tag, id);
        std::cout << "Dumping FIR taps " << id << " to " << str << std::endl;
        if((f = fopen(str, "w")) != 0)
        {
            k = 0;
            
            while(k < m_nOrder)
            {
                fprintf(f, "%.18e\n", m_B[k++]);
            }
            fclose(f);
        }
    }
}
#endif

#ifdef GP_IIR

void SCFilter::IIR(Aurora::Sample* yt, Aurora::Sample* xt, const Aurora::SampleCount length)
{    
    Aurora::Sample* y = yt;
    Aurora::Sample* x = xt;
    Aurora::Sample* x_ptr;
    Aurora::Sample* y_ptr;
    
    int i ,k;
    
    for(i = 0; i < m_nOrder; i++)
    {
        x_ptr = x;
        y_ptr = y;
        
        *y = m_adbB[0] * *x;
        
        for(k = 1; k <= i; k++)
        {
            x_ptr--, y_ptr--;
            *y += m_adbB[k] * *x_ptr - m_adbA[k] * *y_ptr;
        }
        x++;
        y++;
    }
    
    for(i = 0; i < int(length - m_nOrder); i++)
    {
        x_ptr = x;
        y_ptr = y;
        
        *y = m_adbB[0] * *x;
   
        for(k = 1; k < m_nOrder; k++)
        {
            x_ptr--; y_ptr--;
            *y += m_adbB[k] * *x_ptr - m_adbA[k] * *y_ptr;     // x[i-k]
        }
        x++;
        y++;
    } 
}

#elif defined SCIPY_FILTER
void SCFilter::IIR(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length)
{   
    // Direct Form II
    const Aurora::Sample* ptr_x = xt;
    Aurora::Sample* ptr_y = yt;
    const Aurora::Sample a0 = m_A[0];

    std::vector<Aurora::Sample> Z(m_nOrder, 0.0f);
    //Aurora::Sample* Z = new Aurora::Sample [m_nOrder];
    //memset(Z, 0, sizeof(Aurora::Sample)*m_nOrder);
    
    for (Aurora::SampleCount k = 0; k < length; k++) 
    {
        auto b = m_B.data();   /* Reset a and b pointers */
        auto a = m_A.data();
        auto xn = ptr_x;
        auto yn = ptr_y;

        if (m_nOrder > 1) 
        {
            auto z = Z.data();
            *yn = *z + *b / a0 * *xn;   /* Calculate first delay (output) */
            b++;
            a++;
            /* Fill in middle delays */
            for (int n = 0; n < m_nOrder - 2; n++) 
            {
                *z = Z[1] + *xn * (*b / a0) - *yn * (*a / a0);
                b++;
                a++;
                z++;
            }
            /* Calculate last delay */
            *z = *xn * (*b / a0) - *yn * (*a / a0);
        } 
        else 
        {
            *yn = *xn * (*b / a0);
        }

        ptr_y += 1;      /* Move to next input/output point */
        ptr_x += 1;
    } 
}

#endif

#if defined SCIPY_FILTER
void SCFilter::FIR(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length)
{    
    // Direct Form II
    const Aurora::Sample* ptr_x = xt;
    Aurora::Sample* ptr_y = yt;

    std::vector<Aurora::Sample> Z(m_nOrder, 0.0f);
    
    for (Aurora::SampleCount k = 0; k < length; k++)
    {
        auto b = m_B.data();   /* Reset a and b pointers */
        auto xn = ptr_x;
        auto yn = ptr_y;

        if (m_nOrder > 1) 
        {
            auto z = Z.data();
            *yn = *z + *b * *xn;   /* Calculate first delay (output) */
            b++;
            /* Fill in middle delays */
            for (int n = 0; n < m_nOrder - 2; n++) 
            {
                *z = Z[1] + *xn * (*b);
                b++;
                z++;
            }
            /* Calculate last delay */
            *z = *xn * (*b);
        } 
        else 
        {
            *yn = *xn * (*b);
        }

        ptr_y += 1;      /* Move to next input/output point */
        ptr_x += 1;
    }    
}

#else

void SCFilter::FIR(Aurora::Sample* yt, Aurora::Sample* xt, const Aurora::SampleCount length)
{    
    // Generic fir calculation routine...not really optimized yet.
    
    Aurora::Sample* y = yt;
    Aurora::Sample* x = xt;
    Aurora::Sample* x_ptr;

    int i,k;    
    
    for(i = 0; i < m_nOrder; i++)
    {
        x_ptr = x;
        *y    = 0.0;
        
        for(k = 0; k <= i; k++, x_ptr--)
        {
            *y += m_adbB[k] * *x_ptr;
        }
        x++;
        y++;
    }
    
    for(i = 0; i < int(length - m_nOrder); i++)
    {
        x_ptr = x;
        *y    = 0.0;
           
        for(k = 0; k < m_nOrder; k++, x_ptr--)
        {
            *y += m_adbB[k] * *x_ptr;     // x[i-k]
        }
        x++;
        y++;
    } 
}
#endif

bool SCFilter::Apply(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length)
{
    if (int(length) < m_nOrder)
    {
        return false;
    }
        
    if(m_A.empty())
    {
        FIR(yt, xt, length);    
    }
    else
    {
        IIR(yt, xt, length);        
    }
  
    return true;    
}  

bool SCFilter::Apply(const Aurora::TSamplesVector& in, Aurora::TSamplesVector& out)
{
    assert(out.Length() >= in.Length());

    return Apply(out.Samples(), in.CSamples(), in.Length());
}

void SCFilter::init(const Aurora::Sample* b, const Aurora::Sample* a)
{
    if(b != 0)
    {
        m_B.resize(m_nOrder);
        memcpy(m_B.data(), b, m_nOrder*sizeof(Aurora::Sample));
        
        if(a != 0)
        {
            m_A.resize(m_nOrder);
            memcpy(m_A.data(), a, m_nOrder*sizeof(Aurora::Sample));
        }
    }   
    
}

SCFilter::SCFilter(const int    N,
                   double       cutoff,
                   const int    window,
                   const double width,
                   const bool   passZero,
                   const bool   scale,
                   const double fnyq)
: m_dbRate(2.0 * fnyq),
  m_dbFnyq(fnyq),
  m_nOrder(N)
{
    SCFirWin fw(N, cutoff, window, width, passZero, scale, fnyq);
    init(fw.Get());
}

SCFilter::SCFilter(const int N,
                   Aurora::Sample* b,
                   Aurora::Sample* a,
                   const bool   isFir,
                   const double rate)
  : m_dbRate(rate),
    m_dbFnyq(rate/2.0),
    m_nOrder(N),
    m_bIsFir(isFir)
{
    init(b, a);
}

// ------------------------------------------------------------------
SCOctaveBandFilterBase::SCOctaveBandFilterBase()
: SCFilter((int)0,(Aurora::Sample*)0, (Aurora::Sample*)0, true, 0.0),
  m_dbFmExp(0.0)
{}

SCOctaveBandFilterBase::SCOctaveBandFilterBase(const double rate,
                                               const double fmExp,
                                               const bool   isFir)
  : SCFilter((int)0,(Aurora::Sample*)0, (Aurora::Sample*)0, (bool)isFir, (double)rate),
    m_dbFmExp(fmExp)
{
   Create(rate, fmExp, isFir);
}

// ------------------------------------------------------------------
void SCOctaveBandFilter::Create(const double rate,
                                const double fmExp,
                                const bool   isFir)
{
	m_dbRate  = rate;
	m_dbFmExp = fmExp;
	m_bIsFir  = isFir;

    SCOctaveBandFirPreset param(m_dbFmExp, m_dbRate, 0, 0);

    if(m_bIsFir)
    {
        if(m_dbRate == 96000.0)
        {
            if(m_dbFmExp == 12.0)
            {
                param.LpN =  65;    param.LpDelta =  0.01;
                param.HpN = 161;    param.HpDelta = 0.005;
            }
            else if (m_dbFmExp == 9.0)
            {
                param.LpN = 115;    param.LpDelta = 0.005;
                param.HpN = 161;    param.HpDelta = 0.005;
            }
            else
            {
                m_nErr = ERR_FM;
                return;
            }
        }
        else if(m_dbRate == 48000.0)
        {
            if(m_dbFmExp == 12.0)
            {
                param.LpN = 69;    param.LpDelta = 0.01;
                param.HpN = 89;    param.HpDelta = 0.01;
            }
            else if (m_dbFmExp == 9.0)
            {
                param.LpN = 69;    param.LpDelta = 0.01;
                param.HpN = 89;    param.HpDelta = 0.01;
            }
            else
            {
                m_nErr = ERR_FM;
                return;
            }
        }
        else if(m_dbRate == 44100.0)
        {
            if(m_dbFmExp == 12.0)
            {
                param.LpN = 55;    param.LpDelta = -0.03;
                param.HpN = 89;    param.HpDelta =  0.01;
            }
            else if (m_dbFmExp == 9.0)
            {
                param.LpN = 55;    param.LpDelta = 0.01;
                param.HpN = 89;    param.HpDelta = 0.01;
            }
            else
            {
                m_nErr = ERR_FM;
                return;
            }
        }
        else
        {
            m_nErr = ERR_RATE;
            return;
        }
    }
    else
    {
		m_nErr = ERR_NOT_IMPL;
		return;

	    const Aurora::Sample* a_ptr = 0;
	    const Aurora::Sample* b_ptr = 0;
	    Aurora::SampleCount L = 0;

	    m_nOrder = int(L) / sizeof(Aurora::Sample);
	    SCFilter::init(b_ptr, a_ptr);
    }

    SCOctaveBandFir obf(param);
    if(!obf.IsOk())
    {
        m_nErr = ERR_CREATING_FILTER;
        return;
    }
    m_nOrder = obf.GetOrder();
    SCFilter::init(obf.Get());
}

SCOctaveBandFilter::SCOctaveBandFilter()
: SCOctaveBandFilterBase()
{}

SCOctaveBandFilter::SCOctaveBandFilter(const double rate,
                                       const double fmExp,
                                       const bool   isFir)
: SCOctaveBandFilterBase(rate, fmExp, isFir)
{}

// ------------------------------------------------------------------
void SCThirdOctaveBandFilter::Create(const double rate,
                                     const double fmExp,
                                     const bool   isFir)
{
	m_dbRate  = rate;
	m_dbFmExp = fmExp;
	m_bIsFir  = isFir;

    SCOctaveBandFirPreset param(m_dbFmExp, m_dbRate, 0, 0);
    param.OctaveFraction = 3.0;

    if(m_bIsFir)
    {
        if(m_dbRate == 96000.0)
        {
            if(m_dbFmExp == 13.0)
            {
                param.LpN = 111;    param.LpDelta = 0.01;
                param.HpN = 131;    param.HpDelta = 0.007;
            }
            else // 12.0, 11.0, 10.0
            {
                param.LpN = 171;    param.LpDelta = 0.006;
                param.HpN = 201;    param.HpDelta = 0.005;
            }
        }
        else if(m_dbRate == 48000.0)
        {
            if(m_dbFmExp == 13.0)
            {
                param.LpN =  71;    param.LpDelta = 0.01;
                param.HpN = 111;    param.HpDelta = 0.01;
            }
            else // 12.0, 11.0, 10.0
            {
                param.LpN = 121;    param.LpDelta = 0.005;
                param.HpN = 177;    param.HpDelta = 0.005;
            }
        }
        else if(m_dbRate == 44100.0)
        {
            if(m_dbFmExp == 13.0)
            {
                param.LpN =  81;    param.LpDelta = -0.02;
                param.HpN = 121;    param.HpDelta =  0.005;
            }
            else // 12.0, 11.0, 10.0
            {
                param.LpN = 105;    param.LpDelta = 0.01;
                param.HpN = 167;    param.HpDelta = 0.005;
            }
        }
        else
        {
            m_nErr = ERR_RATE;
            return;
        }
    }
    else
    {
        m_nErr = ERR_NOT_IMPL;
        return;

        const Aurora::Sample* a_ptr = 0;
        const Aurora::Sample* b_ptr = 0;
        Aurora::SampleCount L = 0;

        m_nOrder = int(L) / sizeof(Aurora::Sample);
        SCFilter::init(b_ptr, a_ptr);
    }

    SCOctaveBandFir obf(param);

    if(!obf.IsOk())
    {
        m_nErr = ERR_CREATING_FILTER;
        return;
    }
    m_nOrder = obf.GetOrder();
    SCFilter::init(obf.Get());
}

SCThirdOctaveBandFilter::SCThirdOctaveBandFilter()
: SCOctaveBandFilterBase()
{}

SCThirdOctaveBandFilter::SCThirdOctaveBandFilter(const double rate, 
                                                 const double fmExp,
                                                 const bool   isFir)
  : SCOctaveBandFilterBase(rate, fmExp, isFir)
{}


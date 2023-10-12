/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsData.cpp

  Simone Campanini

*******************************************************************//**

\class XFunctionsSpectralDataSerie
\brief The result window plot

*//*******************************************************************/

#include <aurora.h>

#include "correlator.h"
#include "XFunctionsData.h"

/*
    Logarithmic axis formulae:
    
    fk= pow(fend/fstart, k/L) * fstart
    k = L*log10(fk/fstart)/log10(fend/fstart)
*/

void Aurora::XFunctionsSpectralDataSerie::SetHandlerId(const Aurora::XFunctionsPlotType type)
{
    wxASSERT_MSG(type >= 0 && type <= Aurora::XFunctionsPlotType::Coherence,
                 wxT("Aurora::XFunctionsSpectralDataSerie::SetHandler: Id out of range."));

    switch(type)
    {
        case Aurora::XFunctionsPlotType::AutoCorrelationLeft:  m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Rll; break;
        case Aurora::XFunctionsPlotType::AutoCorrelationRight: m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Rrr; break;

        case Aurora::XFunctionsPlotType::Magnitude:  m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Magn;      break;
        case Aurora::XFunctionsPlotType::Phase:      m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Phaze;     break;
        
        case Aurora::XFunctionsPlotType::Real:       m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::SqrReal;   break;
        case Aurora::XFunctionsPlotType::Imaginary:  m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::SqrImag;   break;
        
        case Aurora::XFunctionsPlotType::Alpha:      m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Alpha;     break;
        case Aurora::XFunctionsPlotType::Coherence:  m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Coherence; break;
            
        default:
            assert(1);
            m_fpAverage = &Aurora::XFunctionsSpectralDataSerie::Rll;
    }
    m_nHandlerType = (int)type;
}

double Aurora::XFunctionsSpectralDataSerie::Rll(const int f0, const int f1)
{
    assert(m_correlator != nullptr);
    return m_correlator->Rll(f0, f1);
}

double Aurora::XFunctionsSpectralDataSerie::Rrr(const int f0, const int f1)
{
    assert(m_correlator != nullptr);
    return m_correlator->Rrr(f0, f1);
}

double Aurora::XFunctionsSpectralDataSerie::Magn(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return m_spectrum.Magn(f0);
    }
    double sum = 0.0;
    
    for(int i = f0; i < f1 && i < (int)m_spectrum.Length(); i++)
    {
        sum += m_spectrum.Magn(i);
    }
    return dB(sum/(f1 - f0));
}

double Aurora::XFunctionsSpectralDataSerie::Phaze(const int f0, const int f1)
{
    // Phase in degrees
    if(f0 == f1)
    {
        return m_spectrum.Phas(f0);
    }
    double sum = 0.0;
    
    for(int i = f0; i < f1 && i < (int)m_spectrum.Length(); i++)
    {
        sum += 180.0 * m_spectrum.Phas(i) / M_PI;
    }
    return sum/(f1 - f0);
}

double Aurora::XFunctionsSpectralDataSerie::SqrReal(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return dB(m_spectrum.Real(f0) * m_spectrum.Real(f0));
    }
    double sum = 0.0;
    
    for(int i = f0; i < f1 && i < (int)m_spectrum.Length(); i++)
    {
        sum += m_spectrum.Real(i) * m_spectrum.Real(i);
    }
    return dB(sum/(f1 - f0));
}

double Aurora::XFunctionsSpectralDataSerie::SqrImag(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return dB(m_spectrum.Imag(f0) * m_spectrum.Imag(f0));
    }
    double sum = 0.0;
    for(int i = f0; i < f1 && i < (int)m_spectrum.Length(); i++)
    {
        sum += m_spectrum.Imag(i) * m_spectrum.Imag(i);
    }
    return dB(sum/(f1 - f0));
}

double Aurora::XFunctionsSpectralDataSerie::Alpha(const int f0, const int f1)
{
    assert(m_correlator != nullptr);
    return m_correlator->Alpha(f0, f1);
}

double Aurora::XFunctionsSpectralDataSerie::Coherence(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return m_spectrum.Real(f0);
    }    
    double sum = 0.0;
    
    for(int i = f0; i < f1 && i < (int)m_spectrum.Length(); i++)
    {
        sum += m_spectrum.Real(i);
    }
    return sum/(f1 - f0);
}

void Aurora::XFunctionsSpectralDataSerie::CalculateDataSerie(const size_t unWindowLength, 
                                                             const double lo,
                                                             const double hi,
                                                             const bool bIsLog)
{
    if(m_spectrum.IsNull())
    {
        printf("No data");
        return;
    }

    // No calculation is needed: use existing datas.
    if(size() != unWindowLength)
    {
        resize(unWindowLength);
    }
    const double bw = (double)m_spectrum.GetFrequencyBinsWidth();
    const double wl = (double)unWindowLength;
    
    int k, nf0, nf1;
    
    if(bIsLog)
    {
        double f0, f1;
        const double fLo = (lo > 0) ? lo : 1.0;
        
        for(k = 0; k < int(unWindowLength-1); k++)
        {
            f0 = std::pow(hi/fLo,     k/wl) * fLo;
            f1 = std::pow(hi/fLo, (k+1)/wl) * fLo;

            nf0 = iround(f0 * bw);
            nf1 = iround(f1 * bw);

            this->at(k) = (this->*m_fpAverage)(nf0, nf1);
        }
    }
    else
    {
        // k = Fk * (2.0*L/Fs)
        const int nStepWidth = (int) floor((hi - lo) * bw / wl);

        for(k = 0; k < int(unWindowLength); k++)
        {
            nf0 = k * nStepWidth + lo * bw;
            nf1 = nf0 + nStepWidth;

            this->at(k) = (this->*m_fpAverage)(nf0, nf1);
        }
    }
}

void Aurora::XFunctionsSpectralDataSerie::FindMaxMin(double& max, double& min)
{
    max = -1e12;
    min = 1e12;
    
    for (int k = 0; k < (int)m_spectrum.Length() - 1; ++k)
    {
        const double s = (this->*m_fpAverage)(k, k + 1);
        
        if (s > max)
        {
            max = s;
        }
        else if (s < min)
        {
            min = s;
        }
    }
}

bool Aurora::XFunctionsSpectralDataSerie::IsOk(const double lo, const double hi)
{
    if(lo >= hi)
    {
        // We cannot reverse time or frequencies!
        return false;
    }
    
    if(lo < 0)
    {
        // Frequencies cannot be negative
        // In this case we have a frequency horizontal axis
        return false;
    }
    
    const double fhop = 1.0 / m_spectrum.GetFrequencyBinsWidth();
    
    if(hi-lo < fhop)
    {
        // Frequency intervals smaller than
        // minimum frequency hop are not accepted
        return false;                      
    }
    
    if(hi > m_spectrum.GetNyquistFrequency())
    {
         // Nyquist frequency  is the maximum accepted.
        return false;
    }
    return true;
}

void Aurora::XFunctionsSpectralDataSerie::CopySpectralData(const Aurora::ComplexSpectrum& src)
{
    m_spectrum.Copy(src);
}

Aurora::XFunctionsSpectralDataSerie&
Aurora::XFunctionsSpectralDataSerie::operator=(Aurora::XFunctionsSpectralDataSerie&& sds)
{
    if(this == &sds) 
    {
        return *this;
    }
    TSpectralDataSerie::operator=(std::move(sds));

    SetLineColour(sds.GetLineColour());
    SetFillColour(sds.GetFillColour());

    Show(sds.IsShown());

    return *this;
}

Aurora::XFunctionsSpectralDataSerie& 
Aurora::XFunctionsSpectralDataSerie::operator=(const Aurora::XFunctionsSpectralDataSerie& sds)
{
    if(this == &sds) 
    {
        return *this;
    }
    TSpectralDataSerie::operator=(sds);

    SetLineColour(sds.GetLineColour());
    SetFillColour(sds.GetFillColour());

    Show(sds.IsShown());

    return *this;
}


Aurora::XFunctionsSpectralDataSerie::XFunctionsSpectralDataSerie(Aurora::Correlator* correlator, 
                                                                 const Aurora::XFunctionsPlotType nHandlerType)
 : TSpectralDataSerie(double(correlator->GetSamplerate()), (int)nHandlerType),
   m_correlator(correlator)
{ 
    SetHandlerId(nHandlerType); 
    m_spectrum.Resize(m_correlator->GetFunctionSpectrum().Length());
}

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiTrack.cpp

  Angelo Farina - Simone Campanini

***********************************************************************/
#include <aurora.h>

#include "sti.h"
#include "StiTrack.h"

bool Aurora::STIAudioTrack::FindFirstArrivalTime(const double threshold)
{
    const Aurora::Sample max = Aurora::SamplesVector::GetAbsMax();    
    const Aurora::Sample relativeThreshold = threshold * max;
    
    m_fat = 0;
    Aurora::Sample* samples = Samples();
    
    while( (m_fat < GetLength()) && 
           (std::fabs(samples[m_fat]) < relativeThreshold) )
    {
        m_fat++;
    }
    
    Aurora::SampleCount one_ms = Aurora::SampleCount(0.001 * GetRate());
    
    if(m_fat > one_ms) 
    {
        m_fat -= one_ms;
    }
    else
    {
        m_fat = 0;
    }
    
    return (m_fat != Length());
}

void Aurora::STIAudioTrack::STIOctaveFilter(const float fcb)
{
    m_filteredTrack.SetRate(GetRate());

    // A.Farina
    // Slightly modified (coefficients) octave filter for STI
    // calculations (but I don't know really)
    
    double f0, fQ;
    double fc = (double)fcb;
    // Band filtering with six-poles IIR filter
    // first filter (2 poles)
    f0 = 0.75f * fc;
    fQ = 8.0f;
    Filter(BPF, f0, fQ); 
    // second filter (2 poles)
    f0 = fc;
    fQ = 4.0f;
    Filter(BPF, f0, fQ); 
    // third filter (2 poles)
    f0 = 1.3f * fc;
    fQ = 8.0f;
    Filter(BPF, f0, fQ); 
}

void Aurora::STIAudioTrack::CalculateMTF(const float fcb,
                                         Aurora::STI::ModulationTransferFunctionsTable& tMTF)
{
    double sum = 0.0; // TODO: this should be a class attribute
    
    double OM[] = { 0.63, 0.8, 1.0, 1.25, 1.6,  2.0,  2.5, 
                    3.15, 4.0, 5.0, 6.3,  8.0, 10.0, 12.5 };
        
    Aurora::ScalarVector<double> trackSqm(GetLength());
    Aurora::SampleCount n;
    
    for(n = 0; n < GetLength(); n++)
    {
        trackSqm[n] = GetFilteredSquaredSample(n);
        sum += trackSqm[n];
    }
    
    size_t k = 0;
    auto& modFrequencies = tMTF.GetFrequencies();
    
    assert(modFrequencies.size() == 14);
    
    for (auto frq :modFrequencies)
    {
        double cosTot = 0.0;
        double sinTot = 0.0;
        
        for(n = m_fat; n < GetLength(); n++)
        {
            const double arg = 2.0 * M_PI * OM[k] * (n - m_fat)/GetRate();
            cosTot += trackSqm[n] * std::cos(arg);
            sinTot += trackSqm[n] * std::sin(arg);
        }
        tMTF.SetValue(frq, fcb, std::sqrt(cosTot * cosTot + sinTot* sinTot) / sum);
        ++k;
    }
}

double Aurora::STIAudioTrack::CalculateMTF_STItel(const float fcb) 
{
    double sum = 0.0;
   
    const auto& OMSTItel = Aurora::STISpectrum::OMSTItelProfile();
        
    Aurora::ScalarVector<double> trackSqm(GetLength());
    Aurora::SampleCount n;
    
    for(n = 0; n < GetLength(); n++)
    {
        trackSqm[n] = GetFilteredSquaredSample(n);
        sum += trackSqm[n];
    }
    
    double cosTot = 0.0;
    double sinTot = 0.0;
    
    for(n = m_fat; n < GetLength(); n++)
    {
        const double arg = 2.0 * M_PI * OMSTItel[fcb] * (n - m_fat)/GetRate();
        cosTot += trackSqm[n] * std::cos(arg);
        sinTot += trackSqm[n] * std::sin(arg);
    }
    // aMTF_STItel[nBand-2] =    
    return std::sqrt(cosTot * cosTot + sinTot* sinTot) / sum;
}

void Aurora::STIAudioTrack::CalculateMTF_RaSTI(Aurora::STISpectrum& aMTF_RaSTI)
{
    double sum = 0.0;
    
    const auto& OMRaSTI = Aurora::STISpectrum::OMRaSTIProfile();
        
    Aurora::ScalarVector<double> trackSqm(GetLength());
    Aurora::SampleCount n;
    
    for(n = 0; n < GetLength(); n++)
    {
        trackSqm[n] = GetFilteredSquaredSample(n);
        sum += trackSqm[n];
    }
    
    aMTF_RaSTI.ForEach(false,
                       [&](const size_t i, const float fcb, double& value)
    {
        double cosTot = 0.0;
        double sinTot = 0.0;
        
        for(n = m_fat; n < GetLength(); n++)
        {
            const double arg = 2.0 * M_PI * OMRaSTI[fcb] * (n - m_fat)/GetRate();
            cosTot += trackSqm[n] * std::cos(arg);
            sinTot += trackSqm[n] * std::sin(arg);
        }
        //aMTF_RaSTI[k] 
        value = std::sqrt(cosTot * cosTot + sinTot* sinTot) / sum;
        
    });    
}

Aurora::STIAudioTrack& Aurora::STIAudioTrack::operator=(STIAudioTrack&& at)
{
    Aurora::AudioTrack::operator=(std::move(at));
    m_fat = at.m_fat;
    return *this;
}

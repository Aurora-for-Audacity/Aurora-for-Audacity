/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  ThAnalyzerData.cpp

  Simone Campanini

*******************************************************************//**

\class AFTimeDataSerie
\brief The time data storage class.

*//*******************************************************************/

#include <aurora.h>

#include "TimeHistoryAnalyzer.h"
#include "ThAnalyzerTrack.h"
#include "ThAnalyzerData.h"

//----------------------------------------------------------------------------
// AFTimeDataSerie implementation
//----------------------------------------------------------------------------

void Aurora::ThAnalyzerTimeDataSerie::Decimate(const size_t unWindowLength, 
                                               double& tLo,
                                               double& tHi)
{
    //Samples that come in a single pixel
    Aurora::SampleCount nSamples = (Aurora::SampleCount)std::floor((tHi - tLo) * m_dbRate);
    Aurora::SampleCount iMax = nSamples / unWindowLength;
    Aurora::SampleCount k = 0, i = 0;

    if (iMax > 0)
    {
        for(k = 0; k < unWindowLength; k++)
        {
            i = k * iMax;
            this->at(k) = (i > m_timeSamples.size()) ? -200.0 // -infty
                                                     : m_timeSamples[i] + m_dbFullScale;
        }
    }
    else
    {
        // happens when there are too few samples, so nothing to decimate here
        Aurora::SampleCount step = unWindowLength / nSamples;
        
        while (k < unWindowLength)
        {
            Aurora::SampleCount j = 0;
            
            while (j < step && k < unWindowLength)
            {
                this->at(k) = m_timeSamples[i] + m_dbFullScale;
                ++k;
                ++j;
            }
            ++i;
            
            if (i == m_timeSamples.size())
            {
                i = m_timeSamples.size() - 1;
            }
        }
    }
}


void Aurora::ThAnalyzerTimeDataSerie::CalculateDataSerie(const size_t unWindowLength, 
                                                         double& tLo, 
                                                         double& tHi)
{
    if(m_timeSamples.empty())
    {
        printf("Uninitialized data pointer\n");
        return;
    }

    // No calculation is needed: use existing datas.
    if(size() != unWindowLength)
    {
        resize(unWindowLength);
    }

    switch(m_nHandlerType)
    {
        case HT_RMS:       Rms(unWindowLength, tLo, tHi);      break;
        case HT_DECIMATE : Decimate(unWindowLength, tLo, tHi); break;
        default:           Decimate(unWindowLength, tLo, tHi); break;
    }

}

bool Aurora::ThAnalyzerTimeDataSerie::IsOk(const double tLo,
                                           const double tHi)
{
    if((tLo < 0) || (tHi < 0))    // Time cannot be negative
    {
        return false;
    }
    
    if(tLo >= tHi)                // We cannot reverse time!
    {
        return false;
    }

    if((tHi - tLo) < 1.0/m_dbRate)  // Time interval smaller than
    {
        return false;                 // sampling time are not accepted
    }
    return true;
}

Aurora::ThAnalyzerTimeDataSerie::ThAnalyzerTimeDataSerie(Aurora::ThAnalyzerAudioTrack& track,
                                                         const int handlerType)
    : TTimeDataSerie<double>(track.GetRate(), handlerType)
{
    m_timeSamples.resize(track.Length());
    const Aurora::Sample* data = track.GetData();

    for (Aurora::SampleCount i = 0; i < track.Length(); ++i)
    {
        m_timeSamples[i] = (double)data[i];
    }
    m_dbFullScale = track.GetFullScale();
}

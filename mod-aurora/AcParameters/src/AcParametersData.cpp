/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  data.cpp

  Simone Campanini

*******************************************************************//**

\class DecayPlot
\brief The decay graphic

*//*******************************************************************/
#include <aurora.h>

#include "AcParametersBase.h"
#include "AcParametersTrack.h"
#include "AcParametersData.h"

//----------------------------------------------------------------------------
// Aurora::AcParametersTimeDataSerie implementation
//----------------------------------------------------------------------------

void Aurora::AcParametersTimeDataSerie::Rms(const size_t unWindowLength,
                                            double& tlo,
                                            double& thi)
{
    //Samples that come in a single pixel
    Aurora::SampleCount iMax = 
        (Aurora::SampleCount) std::floor( ((thi - tlo) * m_dbRate) 
                                           /  unWindowLength );
    Aurora::SampleCount k, i, t0, t1;

    for(k = 0; k < unWindowLength; k++)
    {
        // RMS on 1 ms calculation
        double rms  = 0.0;
        t0 = k*iMax + tlo * m_dbRate;
        t1 = t0 + iMax;

        for(i = t0; i < t1; i++)
        {
            if(i < m_timeSamples.size())
            {
                rms += m_timeSamples[i] * m_timeSamples[i];              
            }
        }
        rms /= (t1 - t0);

        this->at(k) = dB(rms) + m_fullscale;
    }
}

void Aurora::AcParametersTimeDataSerie::Decimate(const size_t unWindowLength,
                                                 double& tlo,
                                                 double& thi)
{
    //Samples that come in a single pixel
    Aurora::SampleCount iMax = (Aurora::SampleCount) floor( ((thi - tlo) * m_dbRate) /
    		                                    unWindowLength );
    const double dbCorrection = 10.0 * std::log10(m_dbRate/100.0);

    Aurora::SampleCount k, i;

    for(k = 0; k < unWindowLength; k++)
    {
        i = k*iMax;        
        this->at(k) = (i > m_timeSamples.size() ? 0 // -200
                                                : dB(m_timeSamples[i]) - dbCorrection
                                                                       + m_fullscale);
    }
}


void Aurora::AcParametersTimeDataSerie::CalculateDataSerie(const size_t unWindowLength,
                                                           double& tlo,
                                                           double& thi)
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
        case HT_RMS:       Rms(unWindowLength, tlo, thi);      break;
        case HT_DECIMATE : Decimate(unWindowLength, tlo, thi); break;
        default:           Decimate(unWindowLength, tlo, thi); break;
    }
}

bool Aurora::AcParametersTimeDataSerie::IsOk(const double tlo, const double thi)
{
    if((tlo < 0) || (thi < 0))
    {
        // Time cannot be negative
        return false;
    }
    
    if(tlo >= thi)
    {
        // We cannot reverse time!
        return false;        
    }

    if((thi-tlo) < 1.0/m_dbRate)
    {
        // Time interval smaller than sampling time are not accepted
        return false;
    }

    return true;
}

Aurora::AcParametersTimeDataSerie::AcParametersTimeDataSerie(Aurora::AcParametersAudioTrack& track, 
                                                             const int nHandlerType)
 : Aurora::TTimeDataSerie<Aurora::Sample>((size_t)track.GetFilteredSamples().Length(),
		                                  track.GetFilteredSamples().GetRate(),
		                                  nHandlerType)
{ }

Aurora::AcParametersTimeDataSerie::AcParametersTimeDataSerie(Aurora::TSchroederDecay& decay,
                                                             const int nHandlerType)
  : Aurora::TTimeDataSerie<Aurora::Sample>((size_t)decay.GetLength(),
	                                        decay.GetRate(),
		                                    nHandlerType)
{ }

//
//  AcTSchroederDecay.cpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#include "AcTSchroederDecay.h"

//----------------------------------------------------------------------------
// TSchroederDecay implementation
//----------------------------------------------------------------------------

void Aurora::TSchroederDecay::SetData(Aurora::Sample* src,
                                      const Aurora::SampleCount length)
{
    memcpy(data(), src, length * sizeof(Aurora::Sample));
}

void Aurora::TSchroederDecay::FindFirstArrivalTime()
{
    Aurora::Sample last = front(); // should be the max too...
    Aurora::Sample threshold = last * 1.0e-4;
    Aurora::SampleCount decayStart = 1;

    Aurora::Sample* s = data();

    while(decayStart < size())
    {
        if ((last - s[decayStart]) > threshold)
        {
            break;
        }
        last = s[decayStart];
        decayStart++;
    }

    printf("Decay start: %d\n", int(decayStart));

    m_fat = decayStart;
}

void Aurora::TSchroederDecay::DoBackwardIntegration(Aurora::AcParametersAudioTrack& track)
{
    // Calculate Schroeder Integral.
    // The resulting decay will be stored in m_apsmpDecay vector.
    // It returns the short factor t*p^2, useful for TS determination.
    // TO DO: negative returns on error.
    m_dbRate = track.GetFilteredSamples().GetRate();
    
    int length = (int)track.GetFilteredSamples().Length();
    int i2     = (int)(length - track.GetFirstArrivalTime() *m_dbRate);
    Aurora::Sample* samples = track.Filtered().Samples();
        
    const double nc = track.GetNoiseCorrFactor();
    double asq = 0;
    double sum = 0.0;
    
    if(length != size())
    {
        Init(length);
    }

    m_dbTP2 = 0.0;

    for(int i = length-1; i >= 0; i--)
    {
        // Amplitude square. If noise correction is deactivated,
        //  the correction factor is set to 0.0.
        asq = double(samples[i] * samples[i]) - nc;

        // if FAT is not reached, update i2 and dbTP2
        if(i2)
        {
            m_dbTP2 += asq * (i2--);
        }
        // update squares' total
        sum += asq;

        // store actual integral value.
        operator[](i) = Aurora::Sample(sum);
    }

    // samples to seconds conversion
    m_dbTP2 /= track.GetFilteredSamples().GetRate();

    // Find decay real start
    FindFirstArrivalTime();
}

void Aurora::TSchroederDecay::Init(Aurora::AcParametersAudioTrack& track)
{
    Init(track.GetFilteredSamples().Length());
    DoBackwardIntegration(track);
}

void Aurora::TSchroederDecay::Init(const Aurora::SampleCount length)
{
    if (length > 0)
    {
        resize(length);
    }
    
    memset(data(), 0, size() * sizeof(Aurora::Sample));
}

// *** 'ctors

Aurora::TSchroederDecay::TSchroederDecay(const Aurora::SampleCount length)
{ Init(length); }

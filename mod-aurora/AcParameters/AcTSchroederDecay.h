//
//  AcTSchroederDecay.hpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#ifndef AcTSchroederDecay_hpp
#define AcTSchroederDecay_hpp

#include <aurora.h>
#include "AcParametersTrack.h"

namespace Aurora
{
//----------------------------------------------------------------------------
// TSchroederDecay
//
/// \brief The Schroeder decay holder
//
/// This code calculates the Schroeder decay from an impulse response and
/// holds it.

class TSchroederDecay : public std::vector<Aurora::Sample>
{
private:
    Aurora::SampleCount m_fat = 0;         // First Arrival Time
    double m_dbTP2 = 0.0;
    double m_dbRate = 0.0;
    
public:
    void Dump(const int idx = -1);
    
    Aurora::SampleCount GetLength() const { return size(); }
    double GetTP2  () const { return m_dbTP2; }
    double GetRate () const { return m_dbRate; }
    
    void FindFirstArrivalTime();
    Aurora::SampleCount GetFirstArrivalTimeInSamples() const { return m_fat; }
    
    void SetData(Aurora::Sample* src, const Aurora::SampleCount length);
    
    void DoBackwardIntegration(AcParametersAudioTrack& track);
    void Init(AcParametersAudioTrack& track);
    void Init(const Aurora::SampleCount length);
    
    TSchroederDecay() { }
    TSchroederDecay(const Aurora::SampleCount length);
};
}

#endif /* AcTSchroederDecay_hpp */

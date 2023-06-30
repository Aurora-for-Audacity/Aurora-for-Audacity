/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Acoustical Parameters

  AcParametersTrack.cpp

  Angelo Farina - Simone Campanini

************************************************************************/
#include <aurora.h>

#include "AcParametersBase.h"
#include "AcParametersTrack.h"

//-------------------------------------------------------------------------------------
// Aurora::AcParametersAudioTrack implementation
//-------------------------------------------------------------------------------------
bool Aurora::AcParametersAudioTrack::FindFirstArrivalTime(const double dbDirectSoundTrigdB)
{
    assert(dbDirectSoundTrigdB <= 0.0);

    // WARNING: this method is duplicated! See afaudio.cpp
    const Aurora::Sample max = TSamplesVector::GetAbsMax();
    const Aurora::Sample relativeThreshold = std::pow(10.0f, 
                                                      dbDirectSoundTrigdB/20.0f) * max;
    Aurora::SampleCount fat = 0;
    Aurora::Sample* samples = Samples();

    while(    (fat < GetLength()) 
           && (std::fabs(samples[fat]) < relativeThreshold))
                //(fabs(m_apsmpTrack[CH_LEFT][fat]) < dbRelativeThreshold) )
    {
        fat++;
    }
    // FAT not found:  the signal is not an Impulse Response!!!
    if(fat == GetLength()) 
    {
        return false;
    }
    // Safer to start 4 samples before FAT
    if (fat > 4)
    {
        fat -= 4;
    }
    else
    {
        fat = 0;
    }
    m_dbFAT = fat / GetRate();

    return true;
}

Aurora::SampleCount Aurora::AcParametersAudioTrack::GetLengthFromFAT() const
{
    return Length() - Aurora::SampleCount(m_dbFAT*GetRate());
}

Aurora::SampleCount Aurora::AcParametersAudioTrack::GetSamplesFromFAT() const
{
    return Length() - Aurora::SampleCount(m_dbFAT*GetRate());
}

Aurora::SampleCount Aurora::AcParametersAudioTrack::GetFirstArrivalTimeInSamples(const bool useFilteredTrackRate)
{
    const double fs = (useFilteredTrackRate ? GetRate() : Filtered().GetRate());
    assert(fs > 0.0);
    return (Aurora::SampleCount)(m_dbFAT * fs);    
}

void Aurora::AcParametersAudioTrack::ApplyFilter(const float fcb)
{
    wxString label = "IR_";

    ResetFilteredTrack();
    Aurora::AcParametersSpectrum::Band b(fcb);

    if (b.IsAweighted())
    {
        GaindB(0.0);
        AFilter();
        label << "A_filtered";

    }
    else if (b.IsLinear())
    {
        GaindB(0.0);
        LFilter();
        label << "not_filtered";        
    }
    else
    {
        GaindB(22.2); // dB valore da verificare..... diventa 12.8825;
        OctaveFilter((double)fcb);
        label << b.GetShortLabel(true);
    }
    
    // Removing mean value
    RemoveMean();

    //SetName(label);
}

Aurora::AcParametersAudioTrack& Aurora::AcParametersAudioTrack::operator=(Aurora::AcParametersAudioTrack&& at)
{
    Aurora::AudioTrack::operator=(std::move(at));
    m_name = at.m_name;
    m_dbFAT = at.m_dbFAT;
    return *this;
}


Aurora::AcParametersAudioTrack::AcParametersAudioTrack(Aurora::AcParametersAudioTrack&& at)
    : Aurora::AudioTrack(std::move(at)),
      m_name(at.m_name),
      m_dbFAT(at.m_dbFAT)
{ }

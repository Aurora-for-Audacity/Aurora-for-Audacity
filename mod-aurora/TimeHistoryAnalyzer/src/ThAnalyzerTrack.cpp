/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Time History Analyzer

  track.cpp

  Angelo Farina - Simone Campanini

**********************************************************************/
#include <aurora.h>
#include "ThAnalyzerTrack.h"

//----------------------------------------------------------------------------
// THASignalAudioTrack implementation
//----------------------------------------------------------------------------
void Aurora::ThAnalyzerAudioTrack::Filter()
{
   ResetFilteredTrack();

   switch(m_nSelectedFilter)
   {
       case FLT_LIN: LFilter();            break;
       case FLT_ITU: ITU_P56_Filter();     break;
       case FLT_IEC: AFilter();            break;
       case FLT_ISO: ISO_8041_Filter();    break;
       case FLT_UNI: UNI_9916_Filter();    break;
   }
}

void Aurora::ThAnalyzerAudioTrack::SetFilterFromString(const wxString& s)
{
    int fid = Aurora::AudioTrack::FLT_LIN;
    
    if(s.Find("ITU") != wxNOT_FOUND) { fid = Aurora::AudioTrack::FLT_ITU; } else
    if(s.Find("IEC") != wxNOT_FOUND) { fid = Aurora::AudioTrack::FLT_IEC; } else
    if(s.Find("ISO") != wxNOT_FOUND) { fid = Aurora::AudioTrack::FLT_ISO; } else
    if(s.Find("UNI") != wxNOT_FOUND) { fid = Aurora::AudioTrack::FLT_UNI; }

    SetFilter(fid);
}

Aurora::ThAnalyzerAudioTrack& Aurora::ThAnalyzerAudioTrack::operator=(ThAnalyzerAudioTrack&& at)
{
    Aurora::AudioTrack::operator=(std::move(at));
    m_name = at.m_name;
    m_nIdx = at.m_nIdx;
    m_dbReference = at.m_dbReference;
    return *this;
}
        
Aurora::ThAnalyzerAudioTrack::ThAnalyzerAudioTrack(ThAnalyzerAudioTrack&& at)
: Aurora::AudioTrack(std::move(at)),  
  m_nIdx(at.m_nIdx),
  m_dbReference(at.m_dbReference),
  m_name(at.m_name)
{ }

Aurora::ThAnalyzerAudioTrack::ThAnalyzerAudioTrack(const SampleCount length,
                                                   const double dbRate,
                                                   const int    nIdx,
                                                   const double dbFullscale,
                                                   const int    nFilterId)
 : Aurora::AudioTrack(length, dbRate), 
   m_nIdx(nIdx)
{ 
    SetFullscale(dbFullscale);
    SetFilter(nFilterId);                  
}

Aurora::ThAnalyzerAudioTrack::ThAnalyzerAudioTrack(const SampleCount length,
                                                   const double dbRate, 
                                                   const double dbReference, 
                                                   const int nIdx)
: Aurora::AudioTrack(length, dbRate), 
  m_nIdx(nIdx), 
  m_dbReference(dbReference) 
{}

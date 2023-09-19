/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Time History Analyzer

  track.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_ITUP56_TRACK_H__
#define __AURORA_ITUP56_TRACK_H__

namespace Aurora
{
    //----------------------------------------------------------------------------
    // THAAudioTrack
    //----------------------------------------------------------------------------
    class ThAnalyzerAudioTrack : public Aurora::AudioTrack
    {
    private:
        int      m_nIdx        = 0;
        double   m_dbReference = 94.0;
        wxString m_name        = "TrackName" ;

    public:
        void Filter();

        int GetIndex() const { return m_nIdx; }        
        wxString& GetName()  { return m_name; }
        
        double GetReferenceLevel() const { return m_dbReference; }
        void SetReferenceLevel(const double rv) { m_dbReference = rv; }

        void SetFilterFromString(const wxString& s);
        
        void SetIndex(const int nIdx)        { m_nIdx = nIdx; }
        void SetName (const wxChar*   name)  { m_name = name; }
        void SetName (const wxString& name)  { m_name = name; }

        ThAnalyzerAudioTrack& operator=(ThAnalyzerAudioTrack&& at);
        ThAnalyzerAudioTrack& operator=(ThAnalyzerAudioTrack& at) = delete; 
        
        ThAnalyzerAudioTrack() { }
        ThAnalyzerAudioTrack(ThAnalyzerAudioTrack&& at);
        ThAnalyzerAudioTrack(const ThAnalyzerAudioTrack&  at) = delete;
        
        ThAnalyzerAudioTrack(const SampleCount length,
                             const double dbRate,
                             const int    nIdx        = 0,
                             const double dbFullscale = 120.0,
                             const int    nFilterId   = FLT_LIN);
        
        ThAnalyzerAudioTrack(const SampleCount length,
                             const double dbRate,
                             const double dbReference = 94.0,
                             const int nIdx = 0);

    };
    
typedef std::vector<Aurora::ThAnalyzerAudioTrack> TArrayOfThAnalyzerAudioTracks;

} // namespace Aurora

#endif //__AURORA_ITUP56_TRACK_H__


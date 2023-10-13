/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  module.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_ITUP56_MODULE_H__
#define __AURORA_ITUP56_MODULE_H__

// from Audacity
#include <aurora.h>

#include <WaveTrack.h>
#include <effects/EffectManager.h>
#include <effects/StatefulPerTrackEffect.h>

#include "TimeHistoryAnalyzer.h"

namespace Aurora
{
//----------------------------------------------------------------------------
// EffectTimeHistoryAnalyzer
//----------------------------------------------------------------------------


    class TimeHistoryAnalyzerEffect: public StatefulPerTrackEffect,
                                     public Aurora::TimeHistoryAnalyzer
    {
      private:

        TArrayOfThAnalyzerAudioTracks m_aCalibrationTracks;

        int m_nProgMax = 0;
        std::unique_ptr<GTProgressIndicator> m_pProgressDlg;

    public:
        int  GetTrackIndexFromName(wxString& name);
        void   SetTracksNamesArray(wxArrayString& trackNames);

        TArrayOfThAnalyzerAudioTracks& GetCalibrationTracks() { return  m_aCalibrationTracks; }
        
        bool LoadTrackData(const int index, Aurora::AudioTrack* dest);
        void AppendTracksToWorkspace();

        bool DoFullscaleCalibration(std::vector<double>& fullScales);
        bool DoAnalysis();


    // ---------------- Standard Audacity Effects' methods ----------------
//        bool Init() override;
//        
//        bool ShowInterface(wxWindow& parent,
//                           const EffectDialogFactory& factory,
//                           bool forceModal = false) override;

      protected:
//        bool Process() override;
//        void End() override;

        ComponentInterfaceSymbol GetSymbol() const override;
        TranslatableString GetDescription() const override;
        PluginPath GetPath() const override;
        EffectType GetType() const override;

      public:
        TimeHistoryAnalyzerEffect() { }
        
    private:
        wxWindow* m_parent = nullptr;

    };

} // namespace Aurora

#endif // __AURORA_ITUP56_MODULE_H__

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283




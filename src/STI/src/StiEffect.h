/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  module.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_STI_MODULE_H__
#define __AURORA_STI_MODULE_H__

#include <aurora.h>

#include "sti.h"

class WaveTrack;

namespace Aurora
{

    class STIEffect: public Aurora::EffectBase,
                     public Aurora::STI
    {
    private:
        bool  m_bIsStereo    = false;
        bool  m_bShowResults = false;

        wxArrayString  m_trackNames;

        int m_anCalibrationTracksIdx [2];
        int m_anSignalTracksIdx      [2];
        int m_anNoiseTracksIdx       [2];
        int m_anIrTracksIdx          [2];

    public:
        const wxString& GetTrackName(const int nIdx) const { return m_trackNames.Item(nIdx); }
        wxArrayString* GetTrackNamesArray() { return &m_trackNames; }

        bool IsCalibrationTrackSet(const int nChnl) const { return (m_anCalibrationTracksIdx [nChnl] != 0); }
        bool IsSignalTrackSet(const int nChnl)      const { return (m_anSignalTracksIdx [nChnl] != 0); }
        bool IsNoiseTrackSet(const int nChnl)       const { return (m_anNoiseTracksIdx [nChnl] != 0); }
        bool IsIrTrackSet(const int nChnl)          const { return (m_anIrTracksIdx [nChnl] != 0); }
        
        bool AreCalibrationTracksSet() const;
        bool AreIrTracksSet()          const;
        bool AreSignalAndNoiseSet(const int nChnl) const;
        
        bool IsStereo() const { return AreIrTracksSet(); } 
        
        int GetCalibrationTrackIdx(const int nChnl) const { return m_anCalibrationTracksIdx [nChnl]; }
        int GetSignalTrackIdx     (const int nChnl)      const { return m_anSignalTracksIdx [nChnl]; }
        int GetNoiseTrackIdx      (const int nChnl)       const { return m_anNoiseTracksIdx [nChnl]; }
        int GetIrTrackIdx         (const int nChnl)          const { return m_anIrTracksIdx [nChnl]; }
        
        void SetCalibrationTrackIdx(const int nChnl, const int nValue) { m_anCalibrationTracksIdx [nChnl] = nValue; }
        void SetSignalTrackIdx     (const int nChnl, const int nValue)      { m_anSignalTracksIdx [nChnl] = nValue; }
        void SetNoiseTrackIdx      (const int nChnl, const int nValue)       { m_anNoiseTracksIdx [nChnl] = nValue; }
        void SetIrTrackIdx         (const int nChnl, const int nValue)          { m_anIrTracksIdx [nChnl] = nValue; }
        
    public:
        void SetProgressMeterMessage(const wxString& msg);
        bool UpdateProgressMeter(const int nStep, const int nTotal);

    private:
        bool LoadTrackData(Aurora::STIAudioTrack& track, 
                           const int nCh, 
                           const int nTrackIdx);

    public:
        bool DoFullscaleCalibration(const bool bIsStereo = false);    
        bool DoSNRatioCalculation(const bool bIsStereo = false);
        bool DoSTICalculation();
    
    // ---------------- Standard Audacity Effects' methods ----------------
        ComponentInterfaceSymbol GetSymbol() override;
        
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
        EffectType GetType() override;
        
        bool Init() override;
        
#if AUDACITY_HEX_VERSION < 0x020400
        bool PromptUser(wxWindow* parent) override;
#else
        bool ShowInterface(wxWindow& parent,
                           const EffectDialogFactory& factory,
                           bool forceModal = false) override;
#endif

    protected:
        bool Process() override;
        void End() override;
        
    public:     
        STIEffect();
        
    private:
        wxWindow* m_parent = nullptr;
    };

} // namespace Aurora

#endif // __AURORA_STI_MODULE_H__

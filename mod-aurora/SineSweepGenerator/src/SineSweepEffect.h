/**********************************************************************
 
 Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 plugin collection
 
 Sine Sweep Signal Sweep Generator
 
 module.h
 
 Simone Campanini
 
 **********************************************************************/

#ifndef __AURORA_SSWGEN_MODULE_H__
#define __AURORA_SSWGEN_MODULE_H__

#include <aurora.h>

#include <effects/EffectManager.h>
#include "TimeWarper.h"
#include <Project.h>
#include <Prefs.h>
#include <ViewInfo.h>
#include <WaveTrack.h>

#include "effects/Generator.h"
#include "SyncLock.h"
#include "SineSweepGenerator.h"
#include "SineSweepDialogs.h"
#include "SineSweepUi.h"

#include "effects/EffectEditor.h"
#include "LoadEffects.h"
#include "ShuttleGui.h"
#include "WaveTrack.h"
#include "widgets/valnum.h"

namespace Aurora
{
/*!
 \class EffectSineSweepGenerator
 \brief The Audacity module front-end for Aurora's SineSweepGenerator class.
 
 This class is an Audacity module (plug-in) that loads on run time.
 Its core is the instantiation of a TimeHistoryAnalyzer singleton that
 does all calculations.
 */
class SineSweepGeneratorEffect : public SineSweepGenerator,
public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    SineSweepGeneratorEffect();
    virtual ~SineSweepGeneratorEffect();
    
    std::unique_ptr<EffectEditor> PopulateOrExchange(
                                                     ShuttleGui & S, EffectInstance &instance,
                                                     EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
    bool TransferDataToWindow(const EffectSettings &settings) override;
    bool TransferDataFromWindow(EffectSettings &settings) override;
    
    
protected:
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString  GetDescription() const override;
    ManualPageID ManualPage() const override;
    EffectType GetType() const override;
        
    void BeforeGenerate() override;
    
    void Failure() override;
    void Success() override;

    // Generator implementation
    bool GenerateTrack(EffectSettings &settings,
                       WaveTrack *tmp, const WaveTrack &track, int ntrack) override;
    
private:
    /// do whatever additional setuip is required to initialise both the sine sweep and inverse filter tracks.
    /// Adds one more track as the EffectBase class already generates a track if the effect type is a generator.
    /// @see lib-effect/EffectBase::DoEffect method
    void setupTracks() const;
private:
    
    double mDuration;
    
    wxWeakRef<wxWindow> mUIParent{};
    
    wxBitmap auroraLogo;
    // GUI Elements
    wxButton *mCloseButton;
    
    wxTextCtrl *   m_pTextCtrl_FromFrequency;
    wxTextCtrl *   m_pTextCtrl_ToFrequency;
    wxTextCtrl *   m_pTextCtrl_Duration;
    wxTextCtrl *   m_pTextCtrl_Amplitude;
    wxTextCtrl *   m_pTextCtrl_Channels;
    
    wxChoice   *   m_pChoice_FadeInChoice;
    wxTextCtrl *   m_pTextCtrl_FadeInDuration;
    wxChoice   *   m_pChoice_FadeOutChoice;
    wxTextCtrl *   m_pTextCtrl_FadeOutDuration;
    
    wxTextCtrl *   m_pTextCtrl_SilenceDuration;
    wxTextCtrl *   m_pTextCtrl_Cycles;
    wxTextCtrl *   m_pTextCtrl_dBVariation;
    
    wxRadioButton *m_Radio_LinearSweep;
    wxRadioButton *m_Radio_ExpSweep;
    wxRadioButton *m_Radio_PinkSweep;
    
    wxCheckBox *   m_Check_ControlPulses;
    
    double m_FromFrequency =    22.0;
    double m_ToFrequency   = 22000.0;
    double m_Duration      =    15.0;
    double m_Amplitude     =     0.707;
    int m_Channels         =     1;
    double    mFadeInDuration  = 0.5;
    FadeTypes mFadeInChoice    = FT_HANN;
    double    mFadeOutDuration = 0.1;
    FadeTypes mFadeOutChoice   = FT_HANN;
    SweepTypes m_SweepType;
    double m_SilenceDuration  = 5.0;
    int m_Cycles  = 1;
    double m_dBVariation  = 0.0;
    bool mControlPulse = false;
    
};
}

#endif // __AURORA_SSWGEN_MODULE_H__

#pragma once

#include <aurora.h>

#include <ShuttleGui.h>
#include <LoadEffects.h>
#include <effects/Generator.h>
#include <widgets/valnum.h>

/// Dummy Class to help understand the inrastructure of writing an audacity generator plugin
class DummySineSweepGenerator : public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    DummySineSweepGenerator();
    virtual ~DummySineSweepGenerator();
    
    
    //------------------------------------------------------------------------
    // From ComponentInterface
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString  GetDescription() const override;
    //------------------------------------------------------------------------
    // from EffectDefinitionInterface
    ManualPageID ManualPage() const override;
    EffectType GetType() const override;
    //------------------------------------------------------------------------
    std::unique_ptr<EffectEditor> PopulateOrExchange(
                                                     ShuttleGui & S, EffectInstance &instance,
                                                     EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
    bool TransferDataToWindow(const EffectSettings &settings) override;
    bool TransferDataFromWindow(EffectSettings &settings) override;
    
protected:
    
    bool GenerateTrack(EffectSettings &settings,
                       WaveTrack *tmp, const WaveTrack &track, int ntrack) override;
    
private:
    /// do whatever additional setuip is required to initialise both the sine sweep and inverse filter tracks.
    /// Adds one more track as the EffectBase class already generates a track if the effect type is a generator.
    /// @see lib-effect/EffectBase::DoEffect method
    //void setupTracks() const;
private:
    enum FadeTypes
    {
        FT_RECT,
        FT_HAMMING,
        FT_HANN,
        FT_QUARTER_SINE,
        FT_LINEAR,
        FT_GAUSSIAN,
        FT_BLACKMANN,
        FT_BLACK_HARRIS,
        FT_LOG
    };
        
    // GUI Elements
    wxStaticBitmap *auroraLogo;
    wxStaticBitmap *ssg_logo;

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
    double m_Duration      = 15.0;
    double m_Amplitude = 0.707;
    int m_Channels  = 1;
    
    double    mFadeInDuration;
    FadeTypes mFadeInChoice;
    double    mFadeOutDuration;
    FadeTypes mFadeOutChoice;
    
    double m_SilenceDuration  = 0.0;
    
    int m_Cycles  = 1;
    double m_dBVariation  = 0.0;
    
    bool mControlPulses;
    
};

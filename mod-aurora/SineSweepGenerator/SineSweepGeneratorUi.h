/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 Silence.h
 
 Dominic Mazzoni
 
 An effect to add silence.
 
 **********************************************************************/
#pragma once

#include "SilenceBase.h"
#include "effects/StatefulEffectUIServices.h"
#include <wx/wx.h>
#include <wx/valgen.h>
#include <Aurora/aurora.h>

class NumericTextCtrl;

class SineSweepGeneratorUi final : public StatefulEffect, public StatefulEffectUIServices
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    SineSweepGeneratorUi();
    virtual ~SineSweepGeneratorUi();
    
    std::unique_ptr<EffectEditor> PopulateOrExchange(
                                                     ShuttleGui & S, EffectInstance &instance,
                                                     EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
    bool TransferDataToWindow(const EffectSettings &settings) override;
    bool TransferDataFromWindow(EffectSettings &settings) override;
    
    
    // ComponentInterface implementation
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString GetDescription() const override;
    ManualPageID ManualPage() const override;
    
    // EffectDefinitionInterface implementation
    EffectType GetType() const override;

protected:
    /// From StatefulEffect
    bool Process(EffectInstance &instance, EffectSettings &settings) override;
    //   bool GenerateTrack(const EffectSettings& settings, WaveTrack& tmp) override;
    
private:
    NumericTextCtrl *mDurationT;
    
    
    // UI Elememnts
    wxBitmap mAuroraLogo;
    wxBitmap mSineSweepLogo;
    
    wxButton *mCloseButton;
    
    wxTextCtrl *   m_pTextCtrl_FromFrequency;
    wxTextCtrl *   m_pTextCtrl_ToFrequency;
    wxTextCtrl *   m_pTextCtrl_Amplitude;
    wxTextCtrl *   m_pTextCtrl_Channels;
    
    wxChoice   *   m_pChoice_FadeInChoice;
    wxChoice   *   m_pChoice_FadeOutChoice;
    
    wxTextCtrl *   m_pTextCtrl_Duration;
    wxTextCtrl *   m_pTextCtrl_FadeInDuration;
    wxTextCtrl *   m_pTextCtrl_FadeOutDuration;
    
    wxTextCtrl *   m_pTextCtrl_SilenceDuration;
    wxTextCtrl *   m_pTextCtrl_Cycles;
    wxTextCtrl *   m_pTextCtrl_dBVariation;
    
    wxRadioButton *m_Radio_LinearSweep;
    wxRadioButton *m_Radio_ExpSweep;
    wxRadioButton *m_Radio_PinkSweep;
        
    wxCheckBox *   m_Check_ControlPulses;
    
    Aurora::SineSweepGenerator mSineSweepGenerator{};
    
    // Settings Variables
    double m_FromFrequency   =    22.0;
    double m_ToFrequency     = 22000.0;
    double m_Duration        =    15.0;
    double m_Amplitude       =     0.707;
    int    m_Channels        =     1;
    double mFadeInDuration   =     0.1;
    int    mFadeInChoice     =     Aurora::SineSweepGenerator::FadeTypes::FT_HANN;
    double mFadeOutDuration  =     0.1;
    int    mFadeOutChoice    =     Aurora::SineSweepGenerator::FadeTypes::FT_HANN;
    int    m_SweepType       =     Aurora::SineSweepGenerator::SweepTypes::ST_LOG;
    double m_SilenceDuration =     5.0;
    int    m_Cycles          =     1;
    double m_dBVariation     =     0.0;
    bool   mControlPulse     =     false;
};

#pragma once

#include <aurora.h>

#include <ShuttleGui.h>
#include <LoadEffects.h>
#include <effects/Generator.h>
#include <widgets/valnum.h>

/// Dummy Class to help understand the inrastructure of writing an audacity generator plugin
class DummyKirkeby : public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    DummyKirkeby();
    virtual ~DummyKirkeby();
    
    
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
    
    // UI Elements
    wxTextCtrl * textCrtlNumIRsToInvert;
    wxTextCtrl * textCrtlInverseFilterLength;
    wxTextCtrl * textCrtlInBandReg;
    wxTextCtrl * textCrtlOutBandReg;
    wxTextCtrl * textCrtlLowCutFreq;
    wxTextCtrl * textCrtlHighCutFreq;
    wxTextCtrl * textCrtlTransitionWidth;
    wxTextCtrl * textCrtlFadeInSize;
    wxTextCtrl * textCrtlFadeOutSize;
    wxTextCtrl * textCrtlAveragingWidth;
    wxTextCtrl * textCrtlGain;
    
    // UI Parameters
    double paramNumIRsToInvert = 0.0;
    double paramInverseFilterLength = 0.0;
    double paramInBandReg = 0.0;
    double paramOutBandReg = 0.0;
    double paramLowCutFreq = 0.0;
    double paramHighCutFreq = 0.0;
    double paramTransitionWidth = 0.0;
    double paramFadeInSize = 0.0;
    double paramFadeOutSize = 0.0;
    double paramAveragingWidth = 0.0;
    double paramGain = 0.0;
};

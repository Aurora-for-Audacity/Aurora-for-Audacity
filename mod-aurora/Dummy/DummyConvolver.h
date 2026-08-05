#pragma once

#include <aurora.h>

#include <ShuttleGui.h>
#include <LoadEffects.h>
#include <effects/Generator.h>
#include <widgets/valnum.h>

/// Dummy Class to help understand the inrastructure of writing an audacity generator plugin
class DummyConvolver : public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    DummyConvolver();
    virtual ~DummyConvolver();
    
    
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
    void PopulateStep1(ShuttleGui &);
    void PopulateStep2(ShuttleGui &);
    
    /// do whatever additional setuip is required to initialise both the sine sweep and inverse filter tracks.
    /// Adds one more track as the EffectBase class already generates a track if the effect type is a generator.
    /// @see lib-effect/EffectBase::DoEffect method
    //void setupTracks() const;
    
private:
    
    // UI Elements
    wxTextCtrl * textCrtlGain;
    
    // UI Parameters
    double paramGain = -58.14;
};

/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 Silence.h
 
 Dominic Mazzoni
 
 An effect to add silence.
 
 **********************************************************************/

#ifndef __AUDACITY_EFFECT_SILENCE__
#define __AUDACITY_EFFECT_SILENCE__

#include "SilenceBase.h"
#include "effects/StatefulEffectUIServices.h"

class NumericTextCtrl;

class BUILTIN_EFFECTS_API EffectDummy final : public Generator, public StatefulEffectUIServices 
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    EffectDummy();
    virtual ~EffectDummy();
    
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
   // Generator implementation

   bool GenerateTrack(const EffectSettings& settings, WaveTrack& tmp) override;
    
private:
    
};

#endif

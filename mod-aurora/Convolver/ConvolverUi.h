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
#include "../AuroraPlot/AuroraPlot.h"

class BUILTIN_EFFECTS_API ConvolverUi final : public Generator, public StatefulEffectUIServices
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    ConvolverUi();
    virtual ~ConvolverUi();
    
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
    
    // UI Elememnts
    wxBitmap mAuroraLogo;
    wxBitmap mConvolverLogo;
    
    wxButton *mCloseButton;
    
    AuroraPlot *mPlot;
    
    Aurora::ConvolverController mConvolver;

};

#pragma once

#include <wx/setup.h> // for wxUSE_* macros
#include <wx/brush.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/font.h>
#include <wx/file.h>
#include <wx/scrolbar.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "wxPanelWrapper.h" // to inherit
#include "ShuttleGui.h"
#include <effects/Generator.h>
#include "ProjectWindow.h"
#include "CommonCommandFlags.h"
#include "widgets/VetoDialogHook.h"

#include "widgets/valnum.h"
#include <effects/Generator.h>

#include "effects/EffectEditor.h"
#include "LoadEffects.h"
#include "WaveTrack.h"
#include "widgets/NumericTextCtrl.h"

/// Dummy Class to help understand the inrastructure of writing an audacity generator plugin
class DummyGenerator : public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    DummyGenerator();
    virtual ~DummyGenerator();
    
    
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
};

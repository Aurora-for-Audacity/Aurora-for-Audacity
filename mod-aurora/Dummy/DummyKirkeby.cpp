#include "DummyKirkeby.h"

const ComponentInterfaceSymbol DummyKirkeby::Symbol {XC("Dummy", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyKirkeby > reg; }

DummyKirkeby::DummyKirkeby()
{
}

DummyKirkeby::~DummyKirkeby()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyKirkeby::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyKirkeby::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyKirkeby::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyKirkeby::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyKirkeby::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyKirkeby::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    return nullptr;
}

bool DummyKirkeby::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyKirkeby::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

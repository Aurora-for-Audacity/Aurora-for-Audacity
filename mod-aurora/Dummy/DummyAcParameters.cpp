#include "DummyAcParameters.h"

const ComponentInterfaceSymbol DummyAcParameters::Symbol {XC("Dummy", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyAcParameters > reg; }

DummyAcParameters::DummyAcParameters()
{
}

DummyAcParameters::~DummyAcParameters()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyAcParameters::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyAcParameters::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyAcParameters::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyAcParameters::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyAcParameters::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyAcParameters::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    return nullptr;
}

bool DummyAcParameters::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyAcParameters::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

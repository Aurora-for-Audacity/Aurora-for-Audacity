#include "DummyGenerator.h"

const ComponentInterfaceSymbol DummyGenerator::Symbol {XC("Dummy", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyGenerator > reg; }

DummyGenerator::DummyGenerator()
{
}

DummyGenerator::~DummyGenerator()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyGenerator::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyGenerator::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyGenerator::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyGenerator::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyGenerator::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyGenerator::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    return nullptr;
}

bool DummyGenerator::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyGenerator::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

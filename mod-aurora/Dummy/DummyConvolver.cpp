#include "DummyConvolver.h"

const ComponentInterfaceSymbol DummyConvolver::Symbol {XC("Dummy", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyConvolver > reg; }

DummyConvolver::DummyConvolver()
{
}

DummyConvolver::~DummyConvolver()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyConvolver::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyConvolver::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyConvolver::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyConvolver::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyConvolver::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyConvolver::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    return nullptr;
}

bool DummyConvolver::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyConvolver::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

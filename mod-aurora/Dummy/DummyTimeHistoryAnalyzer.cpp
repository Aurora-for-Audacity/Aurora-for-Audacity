#include "DummyTimeHistoryAnalyzer.h"

const ComponentInterfaceSymbol DummyTimeHistoryAnalyzer::Symbol {XC("Dummy", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyTimeHistoryAnalyzer > reg; }

DummyTimeHistoryAnalyzer::DummyTimeHistoryAnalyzer()
{
}

DummyTimeHistoryAnalyzer::~DummyTimeHistoryAnalyzer()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyTimeHistoryAnalyzer::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyTimeHistoryAnalyzer::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyTimeHistoryAnalyzer::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyTimeHistoryAnalyzer::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyTimeHistoryAnalyzer::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyTimeHistoryAnalyzer::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    return nullptr;
}

bool DummyTimeHistoryAnalyzer::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyTimeHistoryAnalyzer::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

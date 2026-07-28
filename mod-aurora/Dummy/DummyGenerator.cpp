/**********************************************************************

  Audacity: A Digital Audio Editor

  Silence.cpp

  Dominic Mazzoni

*******************************************************************//**

\class EffectDummy
\brief An effect to add silence.

*//*******************************************************************/
#include "DummyGenerator.h"
#include "effects/EffectEditor.h"
#include "LoadEffects.h"

#include "ShuttleGui.h"
#include "widgets/NumericTextCtrl.h"
#include "WaveTrack.h"

// Effect implementation
const ComponentInterfaceSymbol EffectDummy::Symbol
/* i18n-hint: noun */
{ XC("Dummy Gen", "generator") };

EffectDummy::EffectDummy()
{
   SetLinearEffectFlag(true);
}

EffectDummy::~EffectDummy()
{
}

// ComponentInterface implementation

ComponentInterfaceSymbol EffectDummy::GetSymbol() const
{
   return Symbol;
}

TranslatableString EffectDummy::GetDescription() const
{
   return XO("Creates audio of zero amplitude");
}

ManualPageID EffectDummy::ManualPage() const
{
   return L"DummyGen";
}


// EffectDefinitionInterface implementation

EffectType EffectDummy::GetType() const
{
   return EffectTypeGenerate;
}


bool EffectDummy::GenerateTrack(const EffectSettings& settings, WaveTrack& tmp)
{
   tmp.InsertSilence(0.0, settings.extra.GetDuration());
   return true;
}

namespace{ BuiltinEffectsModule::Registration< EffectDummy > reg; }


std::unique_ptr<EffectEditor> EffectDummy::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &, EffectSettingsAccess &access,
   const EffectOutputs *)
{
   S.StartVerticalLay();
   {
      S.StartHorizontalLay();
      {
//         S.AddPrompt(XXO("&Duration:"));
//         auto &extra = access.Get().extra;
//         mDurationT = safenew
//            NumericTextCtrl(FormatterContext::SampleRateContext(mProjectRate),
//                              S.GetParent(), wxID_ANY,
//                              NumericConverterType_TIME(),
//                              extra.GetDurationFormat(),
//                              extra.GetDuration(),
//                               NumericTextCtrl::Options{}
//                                  .AutoPos(true));
//         S.Name(XO("Duration"))
//            .Position(wxALIGN_CENTER | wxALL)
//            .AddWindow(mDurationT);
      }
      S.EndHorizontalLay();
   }
   S.EndVerticalLay();

   return nullptr;
}

bool EffectDummy::TransferDataToWindow(const EffectSettings &settings)
{
//   mDurationT->SetValue(settings.extra.GetDuration());

   return true;
}

bool EffectDummy::TransferDataFromWindow(EffectSettings &settings)
{
//   settings.extra.SetDuration(mDurationT->GetValue());

   return true;
}

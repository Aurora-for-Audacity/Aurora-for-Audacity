#include "DummyKirkeby.h"

const ComponentInterfaceSymbol DummyKirkeby::Symbol {XC("DummyKirkeby", "generator")};

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
    
    S.StartVerticalLay(0);
    {
        
        S.StartStatic(XO("Impulse Response Info"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.AddFixedText(XXO("44100.0 / 1 / 2646000"));
            }
            S.EndHorizontalLay();
            
            S.StartMultiColumn(1, wxALIGN_CENTER);
            {
                S.AddRadioButton(XO("Mono (no CrossTalk Cancellation)"));
                S.AddRadioButtonToGroup(XO("Stereo (with CrossTalk Cancellation)"));
                S.AddRadioButtonToGroup(XO("Dipole (2x2, with CrossTalk Cancellation)"));
            }
            S.EndMultiColumn();
            
            S.StartHorizontalLay(wxALIGN_LEFT);
            {
                S.AddCheckBox(XO("CrossTalk cancel only"), false);
            }
            S.EndHorizontalLay();
            
            S.StartMultiColumn(4, wxEXPAND);
            {
                S.AddCheckBox(XO("Multiple IRs"), false);
                textCrtlNumIRsToInvert = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramInverseFilterLength,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Number of IRs to invert:"), wxT(""), 25);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
    
        S.StartStatic(XO("Filter Calculation Parameters"));
        {            
            S.StartMultiColumn(2, wxEXPAND);
            {
                textCrtlInverseFilterLength = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramInverseFilterLength,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Inverse Filter Length:"), wxT(""), 22);
               
                
                textCrtlInBandReg = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramInBandReg,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("IN-band Regularisation Parameter:"), wxT(""), 33);
               
                
                textCrtlOutBandReg = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramOutBandReg,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("OUT-band Regularisation Parameter:"), wxT(""), 35);
               
                
                textCrtlLowCutFreq = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramLowCutFreq,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Lower cut frequency:"), wxT(""), 21);
               
                
                textCrtlHighCutFreq = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramHighCutFreq,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Higher cut frequency:"), wxT(""), 22);
                
                
                textCrtlTransitionWidth = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramTransitionWidth,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Transition width:"), wxT(""), 18);
                
                
                textCrtlFadeInSize = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramFadeInSize,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Fade-in:"), wxT(""), 9);
                
                
                textCrtlFadeOutSize = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramFadeOutSize,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Fade-out:"), wxT(""), 10);
               
            }
            S.EndMultiColumn();
            
        }
        S.EndStatic();
        
        S.StartStatic(XO("Averaging Mode"));
        {
            S.StartMultiColumn(3, wxEXPAND);
            {
                S.AddChoice(XXO(" "), {}, {});
                S.AddFixedText(XXO(" "));
                S.AddChoice(XXO("Type:"), {}, {});
                S.AddFixedText(XXO(" "));
                textCrtlAveragingWidth = S
                    .Validator<FloatingPointValidator<double>>(3,
                                                               &paramFadeOutSize,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Width"), wxT(""), 10);
                S.AddFixedText(XXO("Spectral Points"));
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Gain Settings"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.AddCheckBox(XXO("Autorange"), false);
                textCrtlGain = S
                    .Validator<FloatingPointValidator<double>>(
                                                               3,
                                                               &paramGain,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0)
                    .AddTextBox(XXO("Manual Gain:"), wxT(""), 12);
                S.AddFixedText(XXO("dB"), wxT(""), 2);
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
    }
    S.EndVerticalLay();
    
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

#include "DummySineSweepGenerator.h"

#define DummySineSweepGeneratorTitle XO("Dummy Sine Sweep Generator")

enum
{
    ID_Channels = 10000,
    ID_Amplitude,
    ID_Duration,
    ID_FromFrequency,
    ID_ToFrequency,
    ID_Radio_Linear,
    ID_Radio_Exp,
    ID_Radio_Pink,
    ID_FadeInType,
    ID_FadeInDuration,
    ID_FadeOutType,
    ID_FadeOutDuration,
    ID_Cycles,
    ID_SilenceDuration,
    ID_dBVariation,
    ID_ControlPulses
};

const ComponentInterfaceSymbol DummySineSweepGenerator::Symbol {XC("DummySineSweepGenerator", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummySineSweepGenerator > reg; }

DummySineSweepGenerator::DummySineSweepGenerator()
{
}

DummySineSweepGenerator::~DummySineSweepGenerator()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummySineSweepGenerator::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummySineSweepGenerator::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummySineSweepGenerator::ManualPage() const
{
    return L"A Dummy Effect";
}
EffectType DummySineSweepGenerator::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummySineSweepGenerator::GenerateTrack(EffectSettings &settings,
                                            WaveTrack *tmp, const WaveTrack &track, int ntrack)
{
    return true;
}

std::unique_ptr<EffectEditor> DummySineSweepGenerator::PopulateOrExchange(
                                                                          ShuttleGui & S, EffectInstance &instance,
                                                                          EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    
    Aurora::InitArtProvider();
    
    wxBitmap auroraBitmap = Aurora::ArtProvider::GetBitmap("Aurora_logo");
    wxBitmap ssgBitmap = Aurora::ArtProvider::GetBitmap("ssg_logo");
    
    auroraLogo = safenew wxStaticBitmap(S.GetParent(), -1, auroraBitmap);
    ssg_logo   = safenew wxStaticBitmap(S.GetParent(), -1, ssgBitmap);
    
    S.StartVerticalLay(0);
    {
        S.StartHorizontalLay();
        {
            
            S.Prop(0).AddWindow(auroraLogo);
            S.Prop(0).AddWindow(ssg_logo);
        }
        S.EndHorizontalLay();
        
        S.StartStatic(XO("Sweep"));
        {
            S.StartMultiColumn(5, wxALIGN_CENTER);
            {
                m_pTextCtrl_FromFrequency = S.Id(ID_FromFrequency)
                    .Name(XO("from (Hz)"))
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_FromFrequency,
                                                               NumValidatorStyle::THREE_TRAILING_ZEROES,
                                                               0.0)
                    .AddTextBox(XXO("f&rom"), wxT(""), 12);
                
                m_pTextCtrl_ToFrequency = S.Id(ID_ToFrequency)
                    .Name(XO("to (Hz)"))
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_ToFrequency,
                                                               NumValidatorStyle::THREE_TRAILING_ZEROES,
                                                               0.0)
                    .AddTextBox(XXO("t&o"), wxT(""), 12);
                
                S.AddUnits(XO("Hz"));
            }
            S.EndMultiColumn();
            
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_Duration = S.Id(ID_Duration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_Duration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Percent Change (samples)"), L"", 12);
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_Amplitude = S.Id(ID_Amplitude)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_Amplitude,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 1.0)
                    .AddTextBox(XXO("Amplitude (0.0 - 1.0)"), L"", 12);
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_Channels =   S.Id(ID_Channels)
                    .Validator<IntegerValidator<int>>(
                                                      &m_Channels, NumValidatorStyle::DEFAULT,
                                                      1, 64 )
                    .AddTextBox(XXO("&Threshold (lower is more sensitive):"),
                                wxT(""),
                                10);
            }
            S.EndHorizontalLay();
            
            S.StartMultiColumn(5, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddPrompt(XXO("Sweep Type"));
                m_Radio_LinearSweep = S.Id(ID_Radio_Linear)
                    .AddRadioButton(XXO("Li&near"));
                
                m_Radio_ExpSweep = S.Id(ID_Radio_Exp)
                    .AddRadioButtonToGroup(XXO("&Exponential"));
                
                m_Radio_PinkSweep = S.Id(ID_Radio_Pink)
                    .AddRadioButtonToGroup(XXO("Pin&k"));
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        TranslatableStrings windowChoices{
            XO("Rectangular"),
            XO("Hamming"),
            XO("Hann"),
            XO("1/4 Sine"),
            XO("Linear"),
            XO("Gaussian"),
            XO("Blackmann"),
            XO("Black/Harris"),
            XO("Log")
        };
        S.StartStatic(XO("Fade-in / Fade-Out"));
        {
            
            S.StartMultiColumn(5, wxALIGN_CENTER_HORIZONTAL);
            {
                m_pTextCtrl_Duration = S.Id(ID_Duration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &mFadeInDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Fade-Out (seconds)"), L"", 12);
                
                m_pChoice_FadeInChoice = S.Id(ID_FadeInType).Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Window:"), windowChoices, mFadeInChoice);
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(5, wxALIGN_CENTER_HORIZONTAL);
            {
                m_pTextCtrl_Duration = S.Id(ID_Duration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &mFadeOutDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Fade-Out (seconds)"), L"", 12);
                
                m_pChoice_FadeOutChoice = S.Id(ID_FadeInType).Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Window:"), windowChoices, mFadeOutChoice);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Silence"));
        {
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_SilenceDuration = S.Id(ID_SilenceDuration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_SilenceDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Duration (seconds)"), L"", 12);
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Repetitions"));
        {
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_Cycles = S.Id(ID_Cycles)
                    .Validator<IntegerValidator<int>>(
                                                      &m_Cycles, NumValidatorStyle::DEFAULT,
                                                      1, 64 )
                    .AddTextBox(XXO("Number of Cycles"), L"", 12);
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_RIGHT);
            {
                m_pTextCtrl_dBVariation = S.Id(ID_dBVariation)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_dBVariation,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("dB Variation"), L"", 12);
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
        S.StartHorizontalLay(wxCENTER);
        {
            mControlPulses = S.Id(ID_ControlPulses)
                .AddCheckBox(XXO("Add a Control Pulses Track"), false);
        }
        S.EndHorizontalLay();
    }
    S.EndVerticalLay();
    return nullptr;
}

bool DummySineSweepGenerator::TransferDataToWindow(const EffectSettings &)
{
    return true;
}

bool DummySineSweepGenerator::TransferDataFromWindow(EffectSettings &)
{
    return true;
}

#include "DummySpeechTransmissionIndex.h"

const ComponentInterfaceSymbol DummySpeechTransmissionIndex::Symbol {XC("DummySpeechTransmissionIndex", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummySpeechTransmissionIndex > reg; }

DummySpeechTransmissionIndex::DummySpeechTransmissionIndex()
{
}

DummySpeechTransmissionIndex::~DummySpeechTransmissionIndex()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummySpeechTransmissionIndex::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummySpeechTransmissionIndex::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummySpeechTransmissionIndex::ManualPage() const
{
    return L"A Dummy Effect";
}
EffectType DummySpeechTransmissionIndex::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummySpeechTransmissionIndex::GenerateTrack(EffectSettings &settings,
                                                 WaveTrack *tmp, const WaveTrack &track, int ntrack)
{
    return true;
}

std::unique_ptr<EffectEditor> DummySpeechTransmissionIndex::PopulateOrExchange(
                                                                               ShuttleGui & S, EffectInstance &instance,
                                                                               EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    Aurora::InitArtProvider();
        
    wxBitmap bm1 = Aurora::ArtProvider::GetBitmap("Aurora_logo");
    wxBitmap bm2 = Aurora::ArtProvider::GetBitmap("sti_logo");
    
    
//    
//    auroraLogo = safenew wxStaticBitmap(S.GetParent(),
//                                        -1,
//                                        bm1);
//    stiLogo = safenew wxStaticBitmap(S.GetParent(),
//                                     -1,
//                                     bm2);
    
    S.StartVerticalLay(0);
    {
        S.StartHorizontalLay(wxALIGN_CENTER);
        {
//            S.Prop(0).AddW
//            S.Prop(1).AddWindow(stiLogo);
        }
        S.EndHorizontalLay();
                
            S.StartNotebook();
            {
                PopulateStart(S);
                PopulateCalibration(S);
                PopulateSNRCalculation(S);
                PopulateImpulseResponse(S);
            }
            S.EndNotebook();
    }
    S.EndVerticalLay();
    
    return nullptr;
}

bool DummySpeechTransmissionIndex::TransferDataToWindow(const EffectSettings &)
{
    return true;
}

bool DummySpeechTransmissionIndex::TransferDataFromWindow(EffectSettings &)
{
    return true;
}

void DummySpeechTransmissionIndex::PopulateStart(ShuttleGui & S)
{
    S.StartNotebookPage( XO("Start"));
    S.StartVerticalLay(2);
    {
        S.AddFixedText(XO(
                          "Welcome to Aurora's Speech Transmission Index Evaluator\n"
                          "\n"
                          "In order to proceed, you have to provide the following recordings:\n"
                          "\n"
                          "\t1) the calibration signal (94 dB)\n"
                          "\t2) the room's background noise\n"
                          "\t3) the test signal (pink noise, 62 dB @ 1m)\n"
                          "\t4) the room's Impulse Response.\n"
                          "\n"
                          "If you have already calibrated the fullscale, the last value should be saved and loaded as default;\n"
                          "same thing for the S/N calculation: if you have previously saved a set of values, you can restore them.\n"
                          "\n"
                          "Otherwise, all the signals 1-4 must be present\n"
                          "in the workspace!\n"
                          "\n"
                          "The recordings can be mono or stereo, but...be coherent!\n"
                          )
                       );
    }
    S.EndVerticalLay();
    S.EndNotebookPage();
}
void DummySpeechTransmissionIndex::PopulateCalibration(ShuttleGui & S)
{
    S.StartNotebookPage( XO("Fullscale Calibration"));
    S.StartVerticalLay(2);
    {
        S.StartStatic(XO("Current Fullscale"));
        {
            S.StartMultiColumn(5, wxALIGN_CENTER);
            {
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &leftdB,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 120.0)
                .AddTextBox(XXO("Left: dB"), L"", 12);
                
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &rightdB,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 120.0)
                .AddTextBox(XXO("Right: dB"), L"", 12);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Choose calibration tracks from lists"));
        {
            S.StartMultiColumn(5, wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Left:"), {}, {});
                
                S.Name(XO("Level:"))
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &leftCalibrationdB,
                                                               NumValidatorStyle::THREE_TRAILING_ZEROES,
                                                               0.0)
                    .AddTextBox(XXO("dB"), wxT(""), 2);
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(5, wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Left:"), {}, {});
                
                S.Name(XO("Level:"))
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &rightCalibrationdB,
                                                               NumValidatorStyle::THREE_TRAILING_ZEROES,
                                                               0.0)
                    .AddTextBox(XXO("dB"), wxT(""), 2);
            }
            S.EndMultiColumn();
            
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.AddButton(XO("Cablirate!"));
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
    }
    S.EndVerticalLay();
    S.EndNotebookPage();
}
void DummySpeechTransmissionIndex::PopulateSNRCalculation(ShuttleGui & S)
{
    S.StartNotebookPage( XO("SNR Calculation"));
    S.StartVerticalLay(2);
    {
        S.StartStatic(XO("Choose background noise tracks from lists:"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Left:"), {}, {});
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Right:"), {}, {});
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Choose test signal tracks from lists:"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Left:"), {}, {});
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Right:"), {}, {});
            }
            S.EndHorizontalLay();

            S.StartMultiColumn(3, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddRadioButton(XXO("Signal"));
                S.AddRadioButtonToGroup(XXO("Signal + Noise"));
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Levels:"));
        {
            S.StartMultiColumn(5, wxALIGN_LEFT);
            {
                S.AddButton(XO("Load SPL Values"));
                S.AddButton(XO("Save SPL Values"));
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
    }
    S.EndVerticalLay();
    S.EndNotebookPage();
}
void DummySpeechTransmissionIndex::PopulateImpulseResponse(ShuttleGui & S)
{
    S.StartNotebookPage( XO("Room Impulse Response"));
    S.StartVerticalLay(2);
    {
        S.StartStatic(XO("Choose Impulse Responses from track lists:"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Left:"), {}, {});
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("&Right:"), {}, {});
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &leftdB,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 120.0)
                .AddTextBox(XO("First Arrival Threshold: "), L"", 26);
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
    }
    S.EndVerticalLay();
    S.EndNotebookPage();
}

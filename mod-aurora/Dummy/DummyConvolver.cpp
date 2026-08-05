#include "DummyConvolver.h"

const ComponentInterfaceSymbol DummyConvolver::Symbol {XC("DummyConvolver", "generator")};

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
    S.StartVerticalLay(0);
    {
        S.StartNotebook();
        {
            PopulateStep1(S);
            PopulateStep2(S);
        }
        S.EndNotebook();
    }
    S.EndVerticalLay();
    
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


void DummyConvolver::PopulateStep1(ShuttleGui & S)
{
    S.StartNotebookPage(XO("Step 1: Select Tracks"));
    {
        S.StartVerticalLay(2);
        {
            S.StartHorizontalLay(wxEXPAND, 0);
            {
                S.AddListControlReportMode({ XO("Tracks") } );
            }
            S.EndHorizontalLay();
            
            S.StartMultiColumn(2, wxEXPAND);
            {
                S.AddButton(XXO("To Audio Data"));
                S.AddButton(XXO("To Filters"));
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(3, wxEXPAND);
            {
                S.AddListControlReportMode({ XO("Audio Data:") } );
                S.AddButton(XXO("Remove"));
                S.AddListControlReportMode({ XO("Filters (IRs):") } );
            }
            S.EndMultiColumn();
            
            S.StartStatic(XO("Ouput"));
            {
                S.AddFixedText(XO("No output yet, please choose data and filter"));
            }
            S.EndStatic();
            
            S.StartHorizontalLay(wxEXPAND, 0);
            {
                S.AddButton(XXO("Next"));
            }
            S.EndHorizontalLay();
        }
        S.EndVerticalLay();
        
    }
    S.EndNotebookPage();
    
}
void DummyConvolver::PopulateStep2(ShuttleGui & S)
{
    S.StartNotebookPage(XO("Step 2: Setup Convolution"));
    {
        S.StartVerticalLay(2);
        {
            S.StartStatic(XO("Impulse Responses(s) RMS"));
            {
                
            }
            S.EndStatic();
            
            S.StartMultiColumn(2, wxEXPAND);
            {
                
                S.StartStatic(XO("Impulse Responses(s) RMS"));
                {
                    S.StartMultiColumn(1);
                    {
                        S.AddCheckBox(XO("Full AutoRange - Remove DC (2 Passes)"), true);
                        S.AddCheckBox(XO("First Block Autorange"), true);
                        
                        textCrtlGain = S.Validator<FloatingPointValidator<double>>(
                                                                                   3, &paramGain,
                                                                                   NumValidatorStyle::TWO_TRAILING_ZEROES,
                                                                                   0.0, 100.0)
                        .AddTextBox(XXO("Gain (dB):"), L"", 12);
                    }
                    S.EndMultiColumn();
                }
                S.EndStatic();
                
                S.StartStatic(XO("Impulse Responses(s) RMS"));
                {
                    
                    S.AddCheckBox(XO("Time Reversed IR"), true);
                    S.AddCheckBox(XO("Cross Talk Cancel Mode"), true);
                    S.AddCheckBox(XO("Preserve Length"), true);
                }
                S.EndStatic();
            }
            S.EndMultiColumn();
            
            S.StartStatic(XO("Info"));
            {
                S.StartMultiColumn(2, wxEXPAND);
                {
                    S.AddFixedText(XO("Audio Data: "));
                    S.AddFixedText(XO("No Data"));
                    S.AddFixedText(XO("IRL:"));
                    S.AddFixedText(XO("No Data"));
                    S.AddFixedText(XO("FFT Size:"));
                    S.AddFixedText(XO("No Data"));
                    S.AddFixedText(XO("Mode:"));
                    S.AddFixedText(XO("No Data"));
                }
                S.EndMultiColumn();
            }
            S.EndStatic();
        }
        S.EndVerticalLay();
    }
    S.EndNotebookPage();
}

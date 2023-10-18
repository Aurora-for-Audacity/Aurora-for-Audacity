#include "DummyXFunctions.h"

const ComponentInterfaceSymbol DummyXFunctions::Symbol {XC("DummyXFunctions", "generator")};

namespace{ BuiltinEffectsModule::Registration< DummyXFunctions > reg; }

DummyXFunctions::DummyXFunctions()
{
}

DummyXFunctions::~DummyXFunctions()
{
}

//------------------------------------------------------------------------
// From ComponentInterface
ComponentInterfaceSymbol DummyXFunctions::GetSymbol() const
{
    return Symbol;
}

TranslatableString DummyXFunctions::GetDescription() const
{
    return XO("A Dummy Effect that does nothing");
}
//------------------------------------------------------------------------
// from EffectDefinitionInterface
ManualPageID DummyXFunctions::ManualPage() const
{
     return L"A Dummy Effect";
}
EffectType DummyXFunctions::GetType() const
{
    return EffectTypeGenerate; // or EffectTypeAnalyze
}

bool DummyXFunctions::GenerateTrack(EffectSettings &settings,
                                   WaveTrack *tmp, const WaveTrack &track, int ntrack)
{    
    return true;
}

std::unique_ptr<EffectEditor> DummyXFunctions::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    S.StartVerticalLay(0);
    {
        S.StartStatic(XO("FFT Setup"));
        {
            S.StartMultiColumn(5, wxALIGN_CENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Length:"), {}, {});
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Window:"), {}, {});
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Processing Function"));
        {
            S.StartHorizontalLay(wxCENTER);
            {
                S.Focus()
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO(""), {}, {});
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxCENTER);
            {
            }
            S.EndHorizontalLay();
            
            S.StartHorizontalLay(wxCENTER);
            {
                S.AddFixedText(XO("Lag: 0.000"));
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Computation of FIR filters for PU Probe calibration"));
        {
            S.StartMultiColumn(4, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddCheckBox(XXO("Rigid termination in a SW tube"), false);
                S.AddCheckBox(XXO("Free Field"), false);
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(6, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddFixedText(XO("c [m/s]"));
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &c,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 100.0);
                S.AddFixedText(XO("d [mm]"));
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &d,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 100.0);
                S.AddFixedText(XO("fmax [Hz]"));
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &fmax,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 100.0);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Following Filter (sine sweep measurements)"));
        {
            S.StartMultiColumn(3, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddCheckBox(XXO("Enable"), false);
                S.AddFixedText(XXO("Bandwidth (Octave)"));
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(5, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddRadioButton(XO("Follow Left"));
                S.AddRadioButtonToGroup(XO("Follow Right"));
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &bandwidthOctave,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 100.0);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Output Results"));
        {
            S.StartMultiColumn(4, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddCheckBox(XXO("Normalize"), false);
                S.AddCheckBox(XXO("Squared Hilbert Transform"), false);
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(4, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddCheckBox(XXO("Shift to Half Window"), false);
                S.AddCheckBox(XXO("Time Reversal"), false);
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(4, wxALIGN_CENTER_HORIZONTAL);
            {
                S.AddCheckBox(XXO("Coherence Weighting"), false);
                S.AddCheckBox(XXO("Dirac Pulse"), false);
            }
            S.EndMultiColumn();
            
            S.StartHorizontalLay(wxALIGN_LEFT);
            {
                S.Validator<FloatingPointValidator<double>>(
                                                            3, &bandwidthOctave,
                                                            NumValidatorStyle::ONE_TRAILING_ZERO,
                                                            0.0, 100.0);
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
    }
    S.EndVerticalLay();
    
    return nullptr;
}

bool DummyXFunctions::TransferDataToWindow(const EffectSettings &)
{
   return true;
}

bool DummyXFunctions::TransferDataFromWindow(EffectSettings &)
{
   return true;
}

#include "DummyTimeHistoryAnalyzer.h"

const ComponentInterfaceSymbol DummyTimeHistoryAnalyzer::Symbol {XC("Dummy Time History Analyser", "generator")};

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
    S.StartVerticalLay(0);
    {
        S.StartNotebook();
        {
            S.StartNotebookPage(XO("Setup"));
            {
                S.StartVerticalLay(2);
                {
                    S.StartStatic(XO("Track(s) to Analyse"));
                    {
                        S.StartHorizontalLay();
                        {
                            
                        }
                        S.EndHorizontalLay();
                        
                        S.StartHorizontalLay();
                        {
                            S.AddButton(XO("Analyse"));
                            S.AddCheckBox(XO("remove DC Component"), false);
                        }
                        S.EndHorizontalLay();
                    }
                    S.EndStatic();
                }
                S.EndVerticalLay();
            }
            S.EndNotebookPage();
            
            S.StartNotebookPage(XO("Calibration"));
            {
                S.StartVerticalLay(2);
                {
                    S.StartStatic(XO("Select Track(s) containing Calibration Signal"));
                    {
                        S.StartHorizontalLay();
                        {

                        }
                        S.EndHorizontalLay();
                        
                        S.StartHorizontalLay();
                        {
                            S.AddButton(XO("Calibrate"));
                            S.AddCheckBox(XO("Apply Ch 1 Calibration to All Channels"), false);
                        }
                        S.EndHorizontalLay();
                    }
                    S.EndStatic();
                }
                S.EndVerticalLay();
            }
            S.EndNotebookPage();
        }
        S.EndNotebook();
        
        S.StartMultiColumn(2, wxEXPAND);
        {
            S.StartStatic(XO("Input Data Graph"));
            {
                S.StartMultiColumn(1);
                {
                                    RulerPanel(
                                       S.GetParent(), wxID_ANY, wxVERTICAL,
                                       wxSize{ 100, 100 }, // Ruler can't handle small sizes
                                       RulerPanel::Range{ 0.0, -120 },
                                       LinearDBFormat::Instance(),
                                       XO("dB"),
                                       RulerPanel::Options{}
                                          .LabelEdges(true)
                                          .TickColour( theTheme.Colour( clrGraphLabels ) )
                                    );
                }
                S.EndMultiColumn();
            }
            S.EndStatic();
            
            S.StartStatic(XO("Calculated Parameters Table"));
            {  
                S.StartMultiColumn(1);
                {

                }
                S.EndMultiColumn();
            }
            S.EndStatic();
        }
        S.EndMultiColumn();
        
    }
    S.EndVerticalLay();
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

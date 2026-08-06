/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 Silence.cpp
 
 Dominic Mazzoni
 
 *******************************************************************/

/**
 
 \class ConvolverUi
 \brief Mulei-Channel Convolution
 
 *//*******************************************************************/
#include "ConvolverUi.h"
#include "effects/EffectEditor.h"
#include "LoadEffects.h"

#include "ShuttleGui.h"
#include "widgets/NumericTextCtrl.h"
#include "WaveTrack.h"

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/valnum.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include <widgets/valnum.h>

#include "../res/aurora_logos.h"
#include "../mod_aurora.h"

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

// Effect implementation
const ComponentInterfaceSymbol ConvolverUi::Symbol
/* i18n-hint: noun */
{ XC("Aurora Convolver", "generator") };

ConvolverUi::ConvolverUi()
{
    SetLinearEffectFlag(true);
}

ConvolverUi::~ConvolverUi()
{
    
}

// ComponentInterface implementation

ComponentInterfaceSymbol ConvolverUi::GetSymbol() const
{
    return Symbol;
}

TranslatableString ConvolverUi::GetDescription() const
{
    return XO("Convolves Multi-channel signal");
}

ManualPageID ConvolverUi::ManualPage() const
{
    return L"AuroraConvolver";
}


// EffectDefinitionInterface implementation

EffectType ConvolverUi::GetType() const
{
    return EffectTypeGenerate;
}


bool ConvolverUi::GenerateTrack(const EffectSettings& settings, WaveTrack& tmp)
{
    //    convolver.Reset();
    //    convolver.SetSamplerate(ssweep.GetSamplerate()); // ???
    //    convolver.CheckSamplerate(ssweep.GetSamplerate());
    //    convolver.SetFilterMatrixDimensions(1,1);
    //    convolver.ResizeFilterTrack(0,numSamples);
    //    convolver.ResizeInputTrack(0, numSamples);
    //
    //    auto& convolutionFilters = convolver.GetFilters();
    //    auto& input = convolver.GetInputTrack(0);
    //
    //    std::copy_n(filter.get(), numSamples, convolutionFilters[0].Samples());
    //    std::copy_n(audio.get(),  numSamples, input.Samples());
    //
    //    convolver.DoConvolution();
    //    auto& conv = convolver.GetOutputTrack(0);
    //    conv.Samples();
    
    tmp.InsertSilence(0.0, settings.extra.GetDuration());
    return true;
}

namespace{ BuiltinEffectsModule::Registration< ConvolverUi > reg; }


std::unique_ptr<EffectEditor> ConvolverUi::PopulateOrExchange(
                                                              ShuttleGui & S, EffectInstance &, EffectSettingsAccess &access,
                                                              const EffectOutputs *)
{
    mAuroraLogo = LoadPngBitmap(
                                Aurora_logo_png,
                                sizeof(Aurora_logo_png)
                                );
    
    mConvolverLogo = LoadPngBitmap(
                                   conv_logo_png,
                                   sizeof(ssg_logo_png)
                                   );
//    S.StartVerticalLay(0);
//    {
//        S.StartHorizontalLay(wxALIGN_CENTER);
//        {
//            S.AddWindow(
//                        safenew wxStaticBitmap(
//                                               S.GetParent(),
//                                               wxID_ANY,
//                                               mAuroraLogo)
//                        );
//            
//            S.GetSizer()->AddStretchSpacer();
//            
//            S.AddWindow(
//                        safenew wxStaticBitmap(
//                                               S.GetParent(),
//                                               wxID_ANY,
//                                               mConvolverLogo)
//                        );
//        }
//        S.EndHorizontalLay();
//        
//        S.StartNotebook();
//        {
//            S.StartNotebookPage(XO("Select Tracks"));
//            {
//                S.StartStatic(XO("Selected Tracks"));
//                {
//                    S.AddListControlReportMode({{ XO("Selected Tracks"), wxLIST_FORMAT_RIGHT }});
//                }
//                S.EndStatic();
//                
//                S.StartHorizontalLay(wxALIGN_CENTER);
//                {
//                    S.AddButton(XO("To Audio"));
//                    S.GetSizer()->AddStretchSpacer();
//                    S.AddButton(XO("To Filters"));
//                    // To Audio Data Button
//                    // To Filters  Button
//                }
//                S.EndHorizontalLay();
//                
//                S.StartHorizontalLay(wxALIGN_CENTER);
//                {
//                    S.AddFixedText(XO("Audio Data"));
//                    S.GetSizer()->AddStretchSpacer();
//                    S.AddFixedText(XO("Filters (IRs)"));
//                    // Audio Data Static Text
//                    // Filters (IRs) Static Text
//                }
//                S.EndHorizontalLay();
//                
//                S.StartHorizontalLay(wxALIGN_CENTER);
//                {
//                    S.StartVerticalLay(wxALIGN_CENTER);
//                    {
//                        S.AddListControlReportMode({{ XO("Audio Data"), wxLIST_FORMAT_LEFT }});
//                    }
//                    S.EndVerticalLay();
//                    
//                    S.StartVerticalLay(wxALIGN_CENTER);
//                    {
//                        S.AddButton(XO("up"));
//                        
//                        S.StartHorizontalLay(wxALIGN_CENTER);
//                        {
//                            S.AddButton(XO("left"));
//                            S.AddButton(XO("right"));
//                        }
//                        S.EndHorizontalLay();
//                        
//                        S.AddButton(XO("down"));
//                        S.AddButton(XO("remove"));
//                    }
//                    S.EndVerticalLay();
//                    
//                    S.StartVerticalLay(wxALIGN_CENTER);
//                    {
//                        S.AddListControlReportMode({{ XO("Filters (IRs)"), wxLIST_FORMAT_LEFT }});
//                    }
//                    S.EndVerticalLay();
//                }
//                S.EndHorizontalLay();
//                
//                
//                S.StartHorizontalLay(wxALIGN_CENTER);
//                {
//                    S.AddCheckBox(XXO("Matrix Mode"), false);
//                }
//                S.EndHorizontalLay();
//                
//                S.StartStatic(XO("Output:"));
//                {
//                    S.AddFixedText(XO("No output yet. Please choose data and filter"));
//                }
//                S.EndStatic();
//                
//            }
//            S.EndNotebookPage();
//            
//            S.StartNotebookPage(XO("Setup"));
//            {
//                
//            }
//            S.EndNotebookPage();
//        }
//        S.EndNotebook();
//    }
//    S.EndVerticalLay();
    
    
    return nullptr;
}

bool ConvolverUi::TransferDataToWindow(const EffectSettings &settings)
{
    //   mDurationT->SetValue(settings.extra.GetDuration());
    
    return true;
}

bool ConvolverUi::TransferDataFromWindow(EffectSettings &settings)
{
    //   settings.extra.SetDuration(mDurationT->GetValue());
    
    return true;
}

// A different approach would be a Simple Book instead of a single Notebook
//S.StartSimplebook();
//{
//    S.StartNotebookPage(XO("Step 1"));
//    {
//        S.AddTitle(XO("Configure Sweep"));
//
//        S.Id(ID_FromFrequency)
//            .AddTextBox(XXO("Start frequency"), wxT(""), 12);
//
//        S.Id(ID_ToFrequency)
//            .AddTextBox(XXO("End frequency"), wxT(""), 12);
//    }
//    S.EndNotebookPage();
//
//
//    S.StartNotebookPage(XO("Step 2"));
//    {
//        S.AddTitle(XO("Configure Envelope"));
//
//        S.Id(ID_FadeInDuration)
//            .AddTextBox(XXO("Fade-in"), wxT(""), 12);
//
//        S.Id(ID_FadeOutDuration)
//            .AddTextBox(XXO("Fade-out"), wxT(""), 12);
//    }
//    S.EndNotebookPage();
//
//
//    S.StartNotebookPage(XO("Step 3"));
//    {
//        S.AddTitle(XO("Review"));
//
//        // summary controls
//    }
//    S.EndNotebookPage();
//}
//S.EndSimplebook();
// Use:
// wxSimplebook *mBook
//mNextButton = S.AddButton(XO("&Continue"));
//mBackButton = S.AddButton(XO("&Back"));
// mBook->ChangeSelection(1); // go to next page or
//mBook->ChangeSelection(
//mBook->GetSelection() + 1
//);

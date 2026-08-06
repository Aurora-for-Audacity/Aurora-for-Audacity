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

std::vector<double> convTimes =
{
    0.0, 0.01, 0.02, 0.034,
};

std::vector<double> convLevels =
{
    -0.0, -6.0, -12.0, -24.2,
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

//    FindProject()
//    mFactory
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
    mSelectedTracks.clear();

    for (auto track : mTracks->Selected<WaveTrack>())
    {
        mSelectedTracks.push_back(track);
        trackNames.Add(track->GetName());
    }
    
    
    mAuroraLogo = LoadPngBitmap(
                                Aurora_logo_png,
                                sizeof(Aurora_logo_png)
                                );
    
    mConvolverLogo = LoadPngBitmap(
                                   conv_logo_png,
                                   sizeof(ssg_logo_png)
                                   );
    
    S.StartVerticalLay(0);
    {
        S.StartHorizontalLay(wxEXPAND,0);
        {
            S.AddWindow(
                        safenew wxStaticBitmap(
                                               S.GetParent(),
                                               wxID_ANY,
                                               mAuroraLogo)
                        );
            
            S.GetSizer()->AddStretchSpacer();
            
            S.AddWindow(
                        safenew wxStaticBitmap(
                                               S.GetParent(),
                                               wxID_ANY,
                                               mConvolverLogo)
                        );
        }
        S.EndHorizontalLay();
        
        S.AddSpace(5);
        
    }
    S.EndVerticalLay();
    
    S.StartNotebook();
    {
        S.StartNotebookPage(XO("Select Tracks"));
        {
            S.StartStatic(XO("Selected Tracks"));
            {
                mTrackListCtrl = safenew wxListBox(
                    S.GetParent(),
                    wxID_ANY,
                    wxDefaultPosition,
                    wxDefaultSize,
                    trackNames,
                    wxLB_SINGLE
                );

                S.GetSizer()->Add(
                    mTrackListCtrl,
                    1,
                    wxEXPAND | wxALL,
                    5
                );
            }
            S.EndStatic();
                        
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                // Left column
                S.StartVerticalLay(0);
                {
                    S.AddButton(XO("To Audio"));
                    S.AddFixedText(XO("Audio Data"));
                    S.AddListControl();
                }
                S.EndVerticalLay();

                // Middle column
                S.StartVerticalLay(wxALIGN_CENTER_VERTICAL);
                {
                    S.GetSizer()->AddStretchSpacer();

                    S.AddButton(XO("u"));

                    S.StartHorizontalLay(0);
                    {
                        S.AddButton(XO("l"));
                        S.AddButton(XO("r"));
                    }
                    S.EndHorizontalLay();

                    S.AddButton(XO("d"));
                    S.AddButton(XO("Remove"));

//                    S.GetSizer()->AddStretchSpacer();
                }
                S.EndVerticalLay();

                // Right column
                S.StartVerticalLay(0);
                {
                    S.AddButton(XO("To Filters"));
                    S.AddFixedText(XO("Filters (IRs)"));
                    S.AddListControl();
                }
                S.EndVerticalLay();
            }
            S.EndHorizontalLay();
            
            S.AddSpace(5);
            
            S.StartVerticalLay(0);
            {
                S.StartHorizontalLay(wxALIGN_CENTER);
                {
                    mMatrixModeCheckBox = S.AddCheckBox(XO("Matrix Mode"), false);
                }
                S.EndHorizontalLay();
            }
            S.EndVerticalLay();
            
            S.StartStatic(XO("Output:"));
            {
                S.AddFixedText(XO("No output yet. Please choose data and filter"));
            }
            S.EndStatic();
            
        }
        S.EndNotebookPage();
        
        S.StartNotebookPage(XO("Setup"));
        {
            S.StartStatic(XO("Impulse Response(s) RMS:"));
            {
                mPlot = safenew AuroraPlot(S.GetParent(), wxID_ANY);
                S.Prop(1).Position(wxEXPAND).MinSize({ wxDefaultCoord, 250 }).AddWindow(mPlot);
                mPlot->SetData(convTimes, convLevels);
            }
            S.EndStatic();
            S.StartMultiColumn(2, wxEXPAND);
            {
                S.StartStatic(XO("Autorange"));
                {
                    mFullAutorangeCheckBox = S.AddCheckBox(XO("Full Autorange - Remove DC (2 passes)"), true);
                    mFirstBlockAutorangeCheckBox = S.AddCheckBox(XO("First Block autorange"), false);
                    S.StartHorizontalLay(wxALIGN_LEFT);
                    {
                        mGainCtrl = S.AddTextBox(XXO("Gain:"), wxT("-3dB"), 12);
                        S.AddUnits(XO("dB"));
                    }
                    S.EndHorizontalLay();
                }
                S.EndStatic();
                S.StartStatic(XO("Advanced"));
                {
                    mTimeRevCheckBox = S.AddCheckBox(XO("TimeReversed Imp. Resp."), false);
                    mCrossTalkCheckBox = S.AddCheckBox(XO("Crosstalk Cancel"), false);
                    mPreserveLengthCheckBox = S.AddCheckBox(XO("Preserve Length"), false);
                }
                S.EndStatic();
            }
            S.EndMultiColumn();
            S.StartStatic(XO("Advanced"));
            {
                S.StartMultiColumn(2, wxEXPAND);
                {
                    S.AddFixedText(XO("Audio Data:"));
                    S.AddFixedText(XO("Lorem Ipsum dolor sit amet consectetur"));
                    S.AddFixedText(XO("Impulse Resp.:"));
                    S.AddFixedText(XO("Lorem Ipsum dolor sit amet consectetur"));
                    S.AddFixedText(XO("FFT Size:"));
                    S.AddFixedText(XO("Lorem Ipsum dolor sit amet consectetur"));
                    S.AddFixedText(XO("Mode:"));
                    S.AddFixedText(XO("Lorem Ipsum dolor sit amet consectetur"));
                }
                S.EndMultiColumn();
            }
            S.EndStatic();
        }
        S.EndNotebookPage();
    }
    S.EndNotebook();
    
    //
    // Configure UI Elements
    //
    
    mCrossTalkCheckBox->Enable(false);
    mGainCtrl->Enable(false);
    mMatrixModeCheckBox->Enable(false);
    
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

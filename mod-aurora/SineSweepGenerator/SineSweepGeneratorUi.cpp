/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 Silence.cpp
 
 Dominic Mazzoni
 
 *******************************************************************/

/**
 
 \class SineSweepGeneratorUi
 \brief An genertor to create sine sweeps for impulse responses
 
 *//*******************************************************************/
#include "SineSweepGeneratorUi.h"
#include "effects/EffectEditor.h"
#include "LoadEffects.h"
#include "ProjectRate.h"

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
const ComponentInterfaceSymbol SineSweepGeneratorUi::Symbol
/* i18n-hint: noun */
{ XC("Aurora Sine Sweep", "generator") };

SineSweepGeneratorUi::SineSweepGeneratorUi()
{
    SetLinearEffectFlag(true);
}

SineSweepGeneratorUi::~SineSweepGeneratorUi()
{
    
}

// ComponentInterface implementation

ComponentInterfaceSymbol SineSweepGeneratorUi::GetSymbol() const
{
    return Symbol;
}

TranslatableString SineSweepGeneratorUi::GetDescription() const
{
    return XO("Creates a sine sweep");
}

ManualPageID SineSweepGeneratorUi::ManualPage() const
{
    return L"SineSweepGenerator";
}


// EffectDefinitionInterface implementation

EffectType SineSweepGeneratorUi::GetType() const
{
    return EffectTypeGenerate;
}

bool SineSweepGeneratorUi::Process(EffectInstance &instance, EffectSettings &settings)
{
    std::cout << __func__ << '\n';
    
    bool bGoodResult = true;
    
    
    ssweep.SetSamplerate(mProjectRate);
    
    ssweep.SetStartFrequency(m_FromFrequency);
    ssweep.SetEndFrequency(m_ToFrequency);
    ssweep.SetSweepDuration(m_Duration);
    ssweep.SetAmplitude(m_Amplitude);
    ssweep.SetSweepChnlsNumber(m_Channels);
    ssweep.SetFadeInDuration(mFadeInDuration);
    ssweep.SetFadeInType(mFadeInChoice);
    ssweep.SetFadeOutDuration(mFadeOutDuration);
    ssweep.SetFadeOutType(mFadeOutChoice);    
    
    if(m_Radio_LinearSweep->GetValue())
        ssweep.SetSweepType(Aurora::SineSweepGenerator::SweepTypes::ST_LINEAR);
    if(m_Radio_ExpSweep->GetValue())
        ssweep.SetSweepType(Aurora::SineSweepGenerator::SweepTypes::ST_LOG);
    if(m_Radio_PinkSweep->GetValue())
        ssweep.SetSweepType(Aurora::SineSweepGenerator::SweepTypes::ST_PINK);
        
    ssweep.SetSilenceDuration(m_SilenceDuration);
    ssweep.SetRepetitionsNumber(m_Cycles);
    ssweep.SetDeltaL(m_dBVariation);
    ssweep.SetControlPulses(mControlPulse);
    
    ssweep.Generate();
    
    auto refTrack   = mFactory->Create(sampleFormat::floatSample, mProjectRate);      // mono
    
    WaveTrack *newTrack{};
    
    refTrack->SetName(mTracks->MakeUniqueTrackName(WaveTrack::GetDefaultAudioTrackNamePreference()));
    newTrack = mTracks->Add(refTrack);
    newTrack->SetSelected(false);
    
    
    size_t numSamples  = ssweep.GetBuffersLength();
    auto filter  = std::make_unique<float[]>(numSamples);
    auto audio   = std::make_unique<float[]>(numSamples);
    
    ssweep.FillBlock(audio.get() ,  numSamples, 0, 0); // Sweep  == Channel_1
    ssweep.FillBlock(filter.get(),  numSamples, 0, 1); // Filter == Channel_2

    auto tracks = mTracks->Any<WaveTrack>();
    size_t index = 0;
    
    for (auto track : tracks)
    {
        std::cout << "index: " << index << " " << track->GetName() << '\n';
                
        if (index == 0)
        {
            track->Append(0,
                          reinterpret_cast<constSamplePtr>(audio.get()),
                          floatSample,
                          numSamples);
            track->SetName("Sweep");
        }
        else if (index == 1)
        {
            track->Append(0,
                          reinterpret_cast<constSamplePtr>(filter.get()),
                          floatSample,
                          numSamples);
            track->SetName("Filter");
        }

        ++index;
    }
    
    return bGoodResult;
}

namespace{ BuiltinEffectsModule::Registration< SineSweepGeneratorUi > reg; }


std::unique_ptr<EffectEditor> SineSweepGeneratorUi::PopulateOrExchange(
                                                                       ShuttleGui & S, EffectInstance &, EffectSettingsAccess &access,
                                                                       const EffectOutputs *)
{
    mAuroraLogo = LoadPngBitmap(Aurora_logo_png,
                                sizeof(Aurora_logo_png));
    
    mSineSweepLogo = LoadPngBitmap(
                                   ssg_logo_png,
                                   sizeof(ssg_logo_png)
                                   );
    
    //    S.AddTitle(XO("Aurora for Audacity - Sine Sweep Gen. - (v.0.0.1)"));
    S.StartVerticalLay(0);
    {
        S.StartHorizontalLay(wxALIGN_CENTER);
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
                                               mSineSweepLogo)
                        );
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
                    .AddTextBox(XXO("from"), wxT(""), 12);
                
                m_pTextCtrl_ToFrequency = S.Id(ID_ToFrequency)
                    .Name(XO("to (Hz)"))
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_ToFrequency,
                                                               NumValidatorStyle::THREE_TRAILING_ZEROES,
                                                               0.0)
                    .AddTextBox(XXO("to"), wxT(""), 12);
                
                S.AddUnits(XO("Hz"));
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(3, wxALIGN_CENTER);
            {
                m_pTextCtrl_Duration = S.Id(ID_Duration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_Duration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Duration"), L"", 12);
                
                S.AddUnits(XO("Seconds"));
                
                m_pTextCtrl_Amplitude = S.Id(ID_Amplitude)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_Amplitude,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 1.0)
                    .AddTextBox(XXO("Amplitude"), L"", 12);
                S.AddUnits(XO("0.0 - 1.0"));
                
                m_pTextCtrl_Channels =   S.Id(ID_Channels)
                    .Validator<IntegerValidator<int>>(
                                                      &m_Channels, NumValidatorStyle::DEFAULT,
                                                      1, 64 )
                    .AddTextBox(XXO("Channels"),
                                wxT(""),
                                10);
                S.AddUnits(XO(" "));
            }
            S.EndMultiColumn();
            
            S.StartMultiColumn(3, wxALIGN_CENTER);
            {
                m_Radio_LinearSweep = S.Id(ID_Radio_Linear)
                    .AddRadioButton(XXO("Linear"));
                
                
                m_Radio_ExpSweep = S.Id(ID_Radio_Exp)
                    .AddRadioButtonToGroup(XXO("Exponential"));
                m_Radio_ExpSweep->SetValue(true);
                
                m_Radio_PinkSweep = S.Id(ID_Radio_Pink)
                    .AddRadioButtonToGroup(XXO("Pink"));
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
            
            S.StartMultiColumn(4, wxALIGN_CENTER);
            {
                m_pTextCtrl_FadeInDuration = S.Id(ID_FadeInDuration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &mFadeInDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Fade-In (seconds)"), L"", 12);
                
                m_pChoice_FadeInChoice = S.Id(ID_FadeInType)
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Window:"), windowChoices, mFadeInChoice);
                
                m_pTextCtrl_FadeOutDuration = S.Id(ID_FadeOutDuration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &mFadeOutDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Fade-Out (seconds)"), L"", 12);
                
                m_pChoice_FadeOutChoice = S.Id(ID_FadeOutType)
                    .MinSize( { wxDefaultCoord, wxDefaultCoord } )
                    .AddChoice(XXO("Window:"), windowChoices, mFadeOutChoice);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Silence"));
        {
            S.StartHorizontalLay(wxALIGN_CENTER);
            {
                m_pTextCtrl_SilenceDuration = S.Id(ID_SilenceDuration)
                    .Validator<FloatingPointValidator<double>>(
                                                               3, &m_SilenceDuration,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0, 100.0)
                    .AddTextBox(XXO("Duration"), L"", 12);
                S.AddUnits(XXO("(seconds)"));
            }
            S.EndHorizontalLay();
        }
        S.EndStatic();
        
        S.StartStatic(XO("Repetitions"));
        {
            S.StartMultiColumn(2, wxALIGN_CENTER);
            {
                m_pTextCtrl_Cycles = S.Id(ID_Cycles)
                    .Validator<IntegerValidator<int>>(
                                                      &m_Cycles,
                                                      NumValidatorStyle::DEFAULT,
                                                      1,
                                                      64)
                    .AddTextBox(XXO("Number of Cycles"), L"", 12);
                
                m_pTextCtrl_dBVariation = S.Id(ID_dBVariation)
                    .Validator<FloatingPointValidator<double>>(
                                                               3,
                                                               &m_dBVariation,
                                                               NumValidatorStyle::ONE_TRAILING_ZERO,
                                                               0.0,
                                                               100.0)
                    .AddTextBox(XXO("dB Variation"), L"", 12);
            }
            S.EndMultiColumn();
        }
        S.EndStatic();
        
        S.StartHorizontalLay(wxALIGN_CENTER);
        {
            m_Check_ControlPulses = S.Validator<wxGenericValidator>(&mControlPulse).AddCheckBox(XXO("Add a Control Pulses Track"), false);
        }
        S.EndHorizontalLay();
    }
    S.EndVerticalLay();
    
    return nullptr;
}

bool SineSweepGeneratorUi::TransferDataToWindow(const EffectSettings &settings)
{
    //   mDurationT->SetValue(settings.extra.GetDuration());
    
    return true;
}

bool SineSweepGeneratorUi::TransferDataFromWindow(EffectSettings &settings)
{
    //   settings.extra.SetDuration(mDurationT->GetValue());
    
    return true;
}

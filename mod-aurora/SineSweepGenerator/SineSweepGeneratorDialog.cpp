/**********************************************************************
 
 Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 plugin collection
 
 Sine Sweep Signal Generator
 
 module.cpp
 
 Angelo Farina - Simone Campanini
 
 *******************************************************************/



#include "SineSweepGeneratorDialog.h"
#include "LoadEffects.h"
#include <wx/wx.h>
#include <wx/valgen.h>
//
//----------------------------------------------------------------------------
// Aurora::ConvolverEffect implementation
//----------------------------------------------------------------------------

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

const ComponentInterfaceSymbol Aurora::SineSweepGeneratorDialog::Symbol
/* i18n-hint: noun */
{ XC("SineSweep", "generator") };

namespace{ BuiltinEffectsModule::Registration< Aurora::SineSweepGeneratorDialog > reg; }

Aurora::SineSweepGeneratorDialog::SineSweepGeneratorDialog()
{
    SetLinearEffectFlag(true);
}

Aurora::SineSweepGeneratorDialog::~SineSweepGeneratorDialog()
{
}

std::unique_ptr<EffectEditor> Aurora::SineSweepGeneratorDialog::PopulateOrExchange(
   ShuttleGui & S, EffectInstance &instance,
   EffectSettingsAccess &access, const EffectOutputs *pOutputs)
{
    mUIParent = S.GetParent();
    setupTracks();
    
    S.AddTitle(XO("Aurora for Audacity - Sine Sweep Gen. - (v." + Aurora::ModuleVersionString() + ")"));
    
    S.StartVerticalLay(0);
    {
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
                    .AddTextBox(XXO("Duration (seconds)"), L"", 12);
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
                    .AddTextBox(XXO("&Channels:"),
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
            m_Check_ControlPulses = S.Validator<wxGenericValidator>(&mControlPulse).AddCheckBox(XXO("Add a Control Pulses Track"), false);
        }
        S.EndHorizontalLay();
    }
    S.EndVerticalLay();
        
    return nullptr;
}

bool Aurora::SineSweepGeneratorDialog::TransferDataToWindow(const EffectSettings &)
{
    if (!mUIParent->TransferDataToWindow())
    {
       return false;
    }
    
    m_Radio_ExpSweep->SetValue(true);
    
    
   return true;
}

bool Aurora::SineSweepGeneratorDialog::TransferDataFromWindow(EffectSettings &)
{

    
    if (!mUIParent->Validate() || !mUIParent->TransferDataFromWindow())
    {
       return false;
    }
    
    if(m_Radio_LinearSweep->GetValue())
        m_SweepType = ST_LINEAR;
    else if(m_Radio_ExpSweep->GetValue())
        m_SweepType = ST_LOG;
    else if(m_Radio_PinkSweep->GetValue())
        m_SweepType = ST_PINK;
            
    mFadeInChoice  = static_cast<FadeTypes>(m_pChoice_FadeInChoice->GetSelection());
    mFadeOutChoice = static_cast<FadeTypes>(m_pChoice_FadeOutChoice->GetSelection());
    
    SetSamplerate(mProjectRate);
    SetStartFrequency(m_FromFrequency);
    SetEndFrequency(m_ToFrequency);
    SetSweepDuration(m_Duration);
    SetAmplitude(m_Amplitude);
    SetSweepChnlsNumber(m_Channels);
    SetFadeInDuration(mFadeInDuration);
    SetFadeInType(mFadeInChoice);
    SetFadeOutDuration(mFadeOutDuration);
    SetFadeOutType(mFadeOutChoice);
    SetSweepType(m_SweepType);
    SetSilenceDuration(m_SilenceDuration);
    SetRepetitionsNumber(m_Cycles);
    SetDeltaL(m_dBVariation);
    SetControlPulses(mControlPulse);
  
    if( ErrorCheck() != Aurora::SineSweepGenerator::ERR_OK)
    {
        wxString msg;
        GetErrorMessage(msg);
        MessageBox(msg, Aurora::MessageType::Error);
        return false;
    }
    
    if(!Generate() )
    {
        MessageBox("Something strange occurred.\nCannot generate Sine Sweep Signal.",
                   Aurora::MessageType::Error);
        Destroy();
        return false;
    }
    
   return true;
}


ComponentInterfaceSymbol Aurora::SineSweepGeneratorDialog::GetSymbol() const
{
    return Symbol;
}

TranslatableString Aurora::SineSweepGeneratorDialog::GetDescription() const
{
    return XO("Generate exponential sine sweep signals");
}

ManualPageID Aurora::SineSweepGeneratorDialog::ManualPage() const
{
    return L"Exponential Sine Sweep";
}

EffectType Aurora::SineSweepGeneratorDialog::GetType() const
{
    return EffectTypeGenerate;
}

void Aurora::SineSweepGeneratorDialog::BeforeGenerate()
{
    mDuration = GetTotalDuration();
}

void Aurora::SineSweepGeneratorDialog::Failure()
{
    Aurora::SineSweepGenerator::Destroy();
    Aurora::MessageBox("Cannot create signal tracks.",
                       Aurora::MessageType::Error);
}

void Aurora::SineSweepGeneratorDialog::Success()
{
    Aurora::SineSweepGenerator::Destroy();
}

bool Aurora::SineSweepGeneratorDialog::GenerateTrack(EffectSettings &settings,
                                                     WaveTrack *tmp, const WaveTrack &track, int ntrack)
{
    bool bGoodResult = true;
  
    sampleCount numSamples;
    
    // Filter has different length
    numSamples = (ntrack == GetFilterChannel()) ? sampleCount(GetFilterLength()) : sampleCount(GetBuffersLength());

    decltype(numSamples) i = 0;
    Floats data{ tmp->GetMaxBlockSize() };
    
    while ((i < numSamples) && bGoodResult)
    {
        const auto block =
        limitSampleBufferSize(tmp->GetBestBlockSize(i), numSamples - i);
                
        FillBlock(data.get(), block, i.as_size_t(), ntrack);
        
        // Add the generated data to the temporary track
        tmp->Append((samplePtr)data.get(), floatSample, block);
        i += block;
        
        // Update the progress meter
        if (TrackProgress(ntrack, i.as_double() / numSamples.as_double()))
        {
            bGoodResult = false;
        }
    }
    return bGoodResult;
}

void Aurora::SineSweepGeneratorDialog::setupTracks() const
{
   // Add one more track as the EffectBase class already generates a track if the effect type is a generator. see lib-effect/EffectBase::DoEffect method
    
    WaveTrack *newTrack{};
    auto track = mFactory->Create();
    
    track->SetName(mTracks->MakeUniqueTrackName(WaveTrack::GetDefaultAudioTrackNamePreference()));
    newTrack = mTracks->Add(track);
    newTrack->SetSelected(true);
    
    int i = 1;
    int neededTracks = 2;
    for (auto it = mTracks->begin(); it != mTracks->end(); ++it)
    {
        Track* t = *it;
        wxString name;
        
        if(i == 1)
            name.Printf(_("Sweep Channel %d"), i);
        else
            name = _("Inverse Filter");
        
        t->SetName(name);
        i++;
    }
}

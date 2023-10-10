#include "DummyAnalyse.h"
#include "widgets/valnum.h"


#define DummyAnalysisTitle XO("Dummy Analysis")

//------------------------------------------------------------------------------------
// Hook up event handles
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

BEGIN_EVENT_TABLE(DummyAnalyse, wxDialogWrapper)
    EVT_RADIOBUTTON(ID_Radio_Linear, DummyAnalyse::OnNoiseReductionChoice)
    EVT_RADIOBUTTON(ID_Radio_Exp,    DummyAnalyse::OnNoiseReductionChoice)
    EVT_RADIOBUTTON(ID_Radio_Pink,   DummyAnalyse::OnNoiseReductionChoice)
    EVT_CLOSE(DummyAnalyse::OnCloseWindow)
    EVT_BUTTON(wxID_CANCEL, DummyAnalyse::OnCloseButton)
END_EVENT_TABLE()

//------------------------------------------------------------------------------------

DummyAnalyse::DummyAnalyse(wxWindow *parent, wxWindowID id,
                           AudacityProject &project,
                           const TranslatableString &title,
                           const wxPoint &pos)
    : wxDialogWrapper(parent, id, title, pos, wxDefaultSize,
                      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX),
        mProject{&project},
        m_Radio_LinearSweep(NULL),
        m_Radio_ExpSweep(NULL),
        m_Radio_PinkSweep(NULL)
{
    std::cout << __func__ << '\n';
    SetName();

    Populate();
}

DummyAnalyse::~DummyAnalyse()
{
    std::cout << __func__ << '\n';
}

bool DummyAnalyse::Show(bool show)
{
    std::cout << __func__ << '\n';
    bool res = wxDialogWrapper::Show(show);

    return res;
}

//------------------------------------------------------------------------------------

void DummyAnalyse::Populate()
{
    std::cout << __func__ << '\n';
    SetTitle(DummyAnalysisTitle);

    ShuttleGui S(this, eIsCreating);
    //===================================================================
    // Add Logo
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
    //===================================================================
    // Add Close Button
    S.AddStandardButtons(eCloseButton);
    mCloseButton = static_cast<wxButton *>(FindWindowById(wxID_CANCEL));
    mCloseButton->SetDefault();
    //===================================================================

    Layout();
    Fit();
    Center(); // Bug 1607:

    SetMinSize(GetSize());
}

void DummyAnalyse::UpdatePrefs()
{
    std::cout << __func__ << '\n';
}

// This handles the whole radio group
void DummyAnalyse::OnNoiseReductionChoice( wxCommandEvent & WXUNUSED(event))
{
    if (m_Radio_ExpSweep->GetValue());
    if (m_Radio_PinkSweep->GetValue());
    if (m_Radio_LinearSweep->GetValue());
}

void DummyAnalyse::OnCloseWindow(wxCloseEvent &WXUNUSED(event))
{
    std::cout << __func__ << '\n';
    Show(false);
}

void DummyAnalyse::OnCloseButton(wxCommandEvent &event)
{
    std::cout << __func__ << '\n';
    Show(false);
}

//------------------------------------------------------------------------------------
// Bumf to hook-in to Audacity

#include "commands/CommandContext.h"
#include "commands/CommandManager.h"
#include "ProjectWindows.h"

namespace
{
    AttachedWindows::RegisteredFactory sDummyAnalyseWindowKey{
        [](AudacityProject &parent) -> wxWeakRef<wxWindow>
        {
            auto &window = ProjectWindow::Get(parent);
            return safenew DummyAnalyse(
                &window, -1, parent, DummyAnalysisTitle,
                wxPoint{150, 150});
        }};

    void OnOpenWindow(const CommandContext &context)
    {
        std::cout << __func__ << '\n';
        auto &project = context.project;
        CommandManager::Get(project).RegisterLastAnalyzer(context);
        auto newWindow = &GetAttachedWindows(project)
                              .Get<DummyAnalyse>(sDummyAnalyseWindowKey);

        if (VetoDialogHook::Call(newWindow))
            return;
        newWindow->Show(true);
        newWindow->Raise();
        newWindow->SetFocus();
    }

    using namespace MenuTable;
    AttachedItem sAttachment{wxT("Analyze/Analyzers/Windows"),
                             Command(wxT("DummyAnalyse"), XXO("Dummy Analyse..."),
                                     OnOpenWindow,
                                     AudioIONotBusyFlag() | WaveTracksSelectedFlag() | TimeSelectedFlag())};

}
//------------------------------------------------------------------------------------

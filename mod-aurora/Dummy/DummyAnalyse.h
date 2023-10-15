#include <iostream>

#include <aurora.h>

#include <wx/setup.h> // for wxUSE_* macros
#include <wx/brush.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/font.h>
#include <wx/file.h>
#include <wx/scrolbar.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>


#include "wxPanelWrapper.h" // to inherit
#include "ShuttleGui.h"
#include "ProjectWindow.h"
#include "CommonCommandFlags.h"
#include "widgets/VetoDialogHook.h"

#include "widgets/valnum.h"

class AudacityProject;

class DummyAnalyse final : public wxDialogWrapper,
public PrefsListener
{
public:
    DummyAnalyse(wxWindow *parent, wxWindowID id,
                 AudacityProject &project,
                 const TranslatableString & title, const wxPoint & pos);
    virtual ~DummyAnalyse();
    
    bool Show( bool show = true ) override;
    
    
private:
    
    void Populate();
    // PrefsListener implementation
    void UpdatePrefs() override;
    
    void OnNoiseReductionChoice(wxCommandEvent & WXUNUSED(event));
    void OnCloseWindow(wxCloseEvent & event);
    void OnCloseButton(wxCommandEvent & event);
    
private:
    enum FadeTypes
    {
        FT_RECT,
        FT_HAMMING,
        FT_HANN,
        FT_QUARTER_SINE,
        FT_LINEAR,
        FT_GAUSSIAN,
        FT_BLACKMANN,
        FT_BLACK_HARRIS,
        FT_LOG
    };
    
    AudacityProject *mProject;
    DECLARE_EVENT_TABLE()
    wxBitmap auroraLogo;
    // GUI Elements
    wxButton *mCloseButton;
    
    wxTextCtrl *   m_pTextCtrl_FromFrequency;
    wxTextCtrl *   m_pTextCtrl_ToFrequency;
    wxTextCtrl *   m_pTextCtrl_Duration;
    wxTextCtrl *   m_pTextCtrl_Amplitude;
    wxTextCtrl *   m_pTextCtrl_Channels;
    
    wxChoice   *   m_pChoice_FadeInChoice;
    wxTextCtrl *   m_pTextCtrl_FadeInDuration;
    wxChoice   *   m_pChoice_FadeOutChoice;
    wxTextCtrl *   m_pTextCtrl_FadeOutDuration;
    
    wxTextCtrl *   m_pTextCtrl_SilenceDuration;
    wxTextCtrl *   m_pTextCtrl_Cycles;
    wxTextCtrl *   m_pTextCtrl_dBVariation;
    
    wxRadioButton *m_Radio_LinearSweep;
    wxRadioButton *m_Radio_ExpSweep;
    wxRadioButton *m_Radio_PinkSweep;
    
    wxCheckBox *   m_Check_ControlPulses;
    
    double m_FromFrequency =    22.0;
    double m_ToFrequency   = 22000.0;
    double m_Duration      = 15.0;
    double m_Amplitude = 0.707;
    int m_Channels  = 1;
    
    double    mFadeInDuration;
    FadeTypes mFadeInChoice;
    double    mFadeOutDuration;
    FadeTypes mFadeOutChoice;
    
    double m_SilenceDuration  = 0.0;
    
    int m_Cycles  = 1;
    double m_dBVariation  = 0.0;
    
    bool mControlPulses;
};

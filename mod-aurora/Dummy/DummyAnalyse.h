#include <iostream>

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
    
    void OnCloseWindow(wxCloseEvent & event);
    void OnCloseButton(wxCommandEvent & event);
    
private:
    AudacityProject *mProject;
    DECLARE_EVENT_TABLE()
    
    // GUI Elements
    wxButton *mCloseButton;
};

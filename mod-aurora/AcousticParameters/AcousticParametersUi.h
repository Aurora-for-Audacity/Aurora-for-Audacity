/*
 
 */

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
#include <wx/grid.h>
#include <wx/scrolbar.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <wxPanelWrapper.h>
#include <ShuttleGui.h>
#include <ProjectWindow.h>
#include <CommonCommandFlags.h>
#include <widgets/valnum.h>
#include <widgets/RulerPanel.h>
#include <widgets/FreqGauge.h>
#include <widgets/LinearDBFormat.h>
#include <widgets/LinearUpdater.h>
#include <widgets/LogarithmicUpdater.h>
#include <widgets/RealFormat.h>
#include <widgets/RulerPanel.h>


#include "../AuroraPlot/AuroraPlot.h"

#include <Aurora/AcousticalParameters.h>

class AudacityProject;

class AcousticParametersUi final : public wxDialogWrapper,
public PrefsListener
{
public:
    AcousticParametersUi(wxWindow *parent, wxWindowID id,
                         AudacityProject &project,
                         const TranslatableString & title, const wxPoint & pos);
    virtual ~AcousticParametersUi();
    
    bool Show( bool show = true ) override;
    
    
private:
    
    void Populate();
    // PrefsListener implementation
    void UpdatePrefs() override;
    
    void OnSize(wxSizeEvent & event);
    void OnCloseWindow(wxCloseEvent & event);
    void OnCloseButton(wxCommandEvent & event);
    
    void OnNoiseReductionChoice(wxCommandEvent & WXUNUSED(event));
    void OnSetup(wxCommandEvent  & WXUNUSED(event));
    void OnSaveR(wxCommandEvent  & WXUNUSED(event));
    void OnCopyR(wxCommandEvent  & WXUNUSED(event));
    void OnStoreG(wxCommandEvent & WXUNUSED(event));
    void OnFilter(wxCommandEvent & WXUNUSED(event));
    
    
    
    std::vector<float> RMS(std::vector<float> audioVector,
             const size_t unWindowWidth,
             double lo,
             double hi);
    std::vector<float> Decimate(std::vector<float> audioVector,
                  const size_t unWindowLength,
                  double& tlo,
                  double& thi);
    
    
private:
    
    AudacityProject *mProject;
    
    wxBitmap mAuroraLogo;
    wxBitmap mAcousticParametersLogo;
    
    // GUI Elements
    
    AuroraPlot *mPlot;
    
    wxButton* mSetupButton;
    wxButton* mSaveRButton;
    wxButton* mCopyRButton;
    wxButton* mStoreGButton;
    
    wxStaticText *mMicTypeText;
    wxListCtrl* mChannelList;
    wxStaticText *mTuserLimitsText;
    
    wxGrid*   mResultsGrid;
    
    wxButton* mFilterButton;
    wxButton* mCloseButton;
    
    RulerPanel *vRuler;
    RulerPanel *hRuler;
    
    Aurora::AcousticalParameters mAcousticalParameters;
    
    DECLARE_EVENT_TABLE()
};

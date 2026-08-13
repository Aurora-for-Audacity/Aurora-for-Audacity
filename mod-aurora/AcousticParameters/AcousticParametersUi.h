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
    
    void GridToCSV();
//    for (int row = 0; row < grid->GetNumberRows(); ++row)
//    {
//        for (int pos = 0; pos < grid->GetNumberCols(); ++pos)
//        {
//            int col = grid->GetColAt(pos);
//
//            if (pos > 0)
//                file.Write(",");
//
//            file.Write(CsvEscape(grid->GetCellValue(row, col)));
//        }
//
//        file.Write("\r\n");
//    }
    
    /// Get RMS data of an audio vector for a plot with a given window width.
    /// This is to help provide a smaller dataset for plotting for resolutions that could not show the full trace.
    /// - Parameters:
    ///   - audioVector: input audio vector
    ///   - unWindowWidth: width of plot in pixels
    ///   - lo: start time in seconds
    ///   - hi: end time in seconds
    AuroraPlot::PlotData RMS(std::vector<float> audioVector,
             const size_t unWindowWidth,
             double lo,
             double hi);
    
    /// Get A decimated version of an audio vector for a plot with a given window width.
    /// This is to help provide a smaller dataset for plotting for resolutions that could not show the full trace.
    /// - Parameters:
    ///   - audioVector: input audio vector
    ///   - unWindowWidth: width of plot in pixels
    ///   - lo: start time in seconds
    ///   - hi: end time in seconds
    AuroraPlot::PlotData Decimate(std::vector<float> audioVector,
                  const size_t unWindowLength,
                  double tlo,
                  double thi);
    
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

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  gui.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_STI_GUI_H__
#define __AURORA_STI_GUI_H__

namespace Aurora
{
    //----------------------------------------------------------------------------
    // STISetupDialog
    //----------------------------------------------------------------------------
    #define ID_SPL_COPY_SELECTION_MENU 10020

    class STISetupDialog : public STISetupDlg
    {
    private:
        STIEffect*  m_pSti = nullptr;   
        STISpectrumDialog* m_pSpectrumDlg = nullptr;
        
        std::unique_ptr<wxMenu> m_splTableContextMenu;
        
        void InitTrackChoice(wxChoice* pwxChoice);
        void FillSigNoiseGrid();
        
    protected:    
        // Handlers for STISetupDlg events.
        void OnCloseDialog( wxCloseEvent& event ) { EndModal(false); }
        void OnLeftFullscaleText( wxCommandEvent& event );
        void OnRightFullscaleText( wxCommandEvent& event );
        void OnCalibLeftChoice( wxCommandEvent& event );
        void OnCalibRefLeftText( wxCommandEvent& event );
        void OnCalibRightChoice( wxCommandEvent& event );
        void OnCalibRefRightText( wxCommandEvent& event );
        void OnDoFullscaleCalibration( wxCommandEvent& event );
        void OnNoiseLeftChoice( wxCommandEvent& event );
        void OnNoiseRightChoice( wxCommandEvent& event );
        void OnSignalLeftChoice( wxCommandEvent& event );
        void OnSignalRightChoice( wxCommandEvent& event );
        void OnSignalRadio( wxCommandEvent& event );
        void OnSignalPlusNoiseRadio( wxCommandEvent& event );
        void OnSplTableRightClick( wxGridEvent& event ); //***
        void OnLoadSplValues( wxCommandEvent& event );
        void OnSaveSplValues( wxCommandEvent& event );
        void OnComputeSNRatio( wxCommandEvent& event );
        void OnLeftIRChoice( wxCommandEvent& event );
        void OnRightIRChoice( wxCommandEvent& event );
        void OnFatText( wxCommandEvent& event );
        void OnHelp( wxCommandEvent& event );
        void OnCancel( wxCommandEvent& event )  { EndModal(false); }
        void OnComputeSTI( wxCommandEvent& event )  { EndModal(true); }
        
        void OnSplTableKeyPress( wxKeyEvent& event );
        void OnSplTableCopySelection(wxCommandEvent & event);

    public:
        /** Constructor */
        STISetupDialog( wxWindow* parent, 
                        Aurora::STIEffect* sti);
        ~STISetupDialog();
    };

    //----------------------------------------------------------------------------
    // STIShowDialog
    //----------------------------------------------------------------------------
    #define ID_PLOT   10010
    #define ID_MTF_COPY_SELECTION_MENU 10020
    #define ID_STI_COPY_SELECTION_MENU 10021

    class STIShowDialog : public STIShowDlg
    {
    private:
        wxPanel*         m_plotPage = nullptr;
        Aurora::STIPlot* m_pPlot = nullptr;
        
        Aurora::STIEffect* m_pSti = nullptr;
        bool m_bIsStereo = false;
        
        int m_nCurrentChannel = CH_LEFT;

        std::unique_ptr<wxMenu>  m_mtfTableContextMenu;
        std::unique_ptr<wxMenu>  m_stiTableContextMenu;

        void UpdateTables();
        void UpdatePlot();
        
    protected:
        // Handlers for STIShowDlg events.
        void OnCloseDialog( wxCloseEvent& event ) { EndModal(false); }
        void OnMtfTableRigtClick( wxGridEvent& event );
        void OnMtfTableKeyPress( wxKeyEvent& event );
        void OnSTITableRightClick( wxGridEvent& event );
        void OnSTITableKeyPress( wxKeyEvent& event );
        void OnLeftRadio( wxCommandEvent& event );
        void OnRightRadio( wxCommandEvent& event );
        void OnSnCorrectionCheck( wxCommandEvent& event );
        void OnMaskCorrectionCheck( wxCommandEvent& event );
        void OnSave( wxCommandEvent& event );
        void OnHelp( wxCommandEvent& event );
        void OnOk( wxCommandEvent& event );

        // Context menu handlers
        void OnMtfTableCopySelection(wxCommandEvent & event);
        void OnSTITableCopySelection(wxCommandEvent & event);   
        
    public:
        /** Constructor */
        STIShowDialog( wxWindow* parent, 
                       Aurora::STIEffect* sti, 
                       const bool bIsStereo = false);
        ~STIShowDialog();
    };
} // namespace Aurora

#endif // __AURORA_STI_GUI__

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283

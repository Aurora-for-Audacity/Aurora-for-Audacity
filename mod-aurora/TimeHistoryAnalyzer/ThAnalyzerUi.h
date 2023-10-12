/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  gui.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ITUP56_GUI_H__
#define __AURORA_ITUP56_GUI_H__

#include <wxcmn.h>

#include <wx/clipbrd.h>
#include <wx/statline.h>
#include <wx/textfile.h>

namespace Aurora
{

#ifndef __ITUP56_SINGLE_FRAME__
    //----------------------------------------------------------------------------
    // TimeHistoryAnalyzerDialog  TBR (deprecated)
    //----------------------------------------------------------------------------
    class TimeHistoryAnalyzerDialog: public THADlg
    {
       private:
          Aurora::TimeHistoryAnalyzerEffect* m_pEffect = nullptr;

          AFConfig*   m_pCfg;

          wxArrayString  m_awxszTrackNames;
          wxArrayString  m_awxszCalibrationColsLabels;
         
       protected:
          // handlers
          void OnFullscaleGridContentChanged( wxGridEvent& event );
    	  void OnRemoveDCCheck( wxCommandEvent& event );
    	  void OnCalibrationGridContentChanged( wxGridEvent& event );  // NOT NEEDED
          void OnApplyToAllCheck( wxCommandEvent& event );
          void OnCalibrate( wxCommandEvent& event );
          
          void OnOk( wxCommandEvent& event );
          void OnCancel( wxCommandEvent& event );
          void OnHelp( wxCommandEvent& event );
          
          // -- Utilities ---

    public:      
          // constructors and destructors 
          TimeHistoryAnalyzerDialog(wxWindow *pParent,
        		                    TimeHistoryAnalyzer* pThan,
                                    EffectTimeHistoryAnalyzer* pEThan,
                                    AFConfig* pCfg);

    };


    //----------------------------------------------------------------------------
    // TimeHistoryAnalyzerShow  TBR (deprecated)
    //----------------------------------------------------------------------------
    class TimeHistoryAnalyzerShow : public THAShowDlg
    {
        public:
            enum { ID_COPY_SELECTION_MENU = 0X2000,
                   ID_COPY_TABLE_MENU,
                   ID_EXPORT_TABLE_MENU };
    //		enum { PAR_FSL = TimeHistoryAnalyzer::THA_FSL };

        private:
           TArrayOfTHASignalAudioTrack*   m_paSignalTracks;
           TimeHistoryAnalyzer::TResults* m_pResults;

           THAExports  mExportData;
           
           int m_nCurrentPlotType,
               m_nCurrentChannel;

           wxMenu *mContextMenu;
      

           // --- Useful methods ---
           void ResetColumn(const int nCh, const int nChnls);
           void FillColumn(const int nCh, const int nChnls);
       
        protected:
            // Handlers for THAShow events.
            
            void OnCloseDlg( wxCloseEvent& event );
            void OnSelectDisplay( wxCommandEvent& event );
            void OnExport( wxCommandEvent& event );
            void OnCopyTableToClipboard( wxCommandEvent& event );
            void OnCopySelection( wxCommandEvent& event );
            void OnTogglePlot( wxGridEvent& event );
            void OnGridCellRightClick( wxGridEvent& event );
    	    	void OnOk( wxCommandEvent& event );
            void OnAddTracksBtn( wxCommandEvent& event );
    	    	void OnHelp( wxCommandEvent& event );
            
        public:
    		/** Constructor */
            TimeHistoryAnalyzerShow(wxWindow* parent,
                                    TArrayOfTHASignalAudioTrack*   paSignalTracks,
                                    TimeHistoryAnalyzer::TResults* pResults);
            ~TimeHistoryAnalyzerShow();
    };

#else
    //----------------------------------------------------------------------------
    // TimeHistoryAnalyzerFrame
    //----------------------------------------------------------------------------
    // This class merges the previous two and tries to realize a sort of
    // unified workspace for time history analysis
    //----------------------------------------------------------------------------
    class TimeHistoryAnalyzerFrame : public THADialog
    {
      private:

        enum 
        { 
            ID_COPY_SELECTION_MENU = 0X2000,
            ID_COPY_TABLE_MENU,
            ID_EXPORT_TABLE_MENU 
        };

        TimeHistoryAnalyzerEffect* m_pEffect = nullptr;

        wxArrayString  m_aTrackNames;
        wxArrayString  m_aCalibrationColsLabels;

        Aurora::ThAnalyzerExports  m_exporter;

        int m_nCurrentPlotType = 0;
        int m_nCurrentChannel  = 0;

        std::unique_ptr<wxMenu> m_pContextMenu;

        wxColour m_panelsBckgndColor;

        // --- Useful methods ---
        void UpdateResults();

        void InitColumns(const int nStartCol = 0, const int nEndCol = -1);
        void FillColumn(const int nCh, const int nChnls, wxString& label);

        void StoreGeometry();
        void RestoreGeometry();

      protected:
        void OnCloseDialog( wxCloseEvent& event );
        void OnFullscaleGridContentChanged( wxGridEvent& event );
        void OnAnalyze ( wxCommandEvent& event );
        void OnRemoveDCCheck( wxCommandEvent& event );
        void OnCalibrationGridContentChanged( wxGridEvent& event );
        void OnCalibrate( wxCommandEvent& event );
        void OnApplyToAllCheck( wxCommandEvent& event );
        void OnSelectDisplay( wxCommandEvent& event );
        void OnGridCellRightClick( wxGridEvent& event );
        void OnTogglePlot( wxGridEvent& event );

        void OnHelp( wxCommandEvent& event );
        void OnAddTracksBtn( wxCommandEvent& event );
        void OnOk( wxCommandEvent& event );

        void OnExport( wxCommandEvent& event );
        void OnCopyTableToClipboard( wxCommandEvent& event );
        void OnCopySelection( wxCommandEvent& event );

      public:
    	/** Constructor */
        TimeHistoryAnalyzerFrame(wxWindow* pParent,
                                 TimeHistoryAnalyzerEffect* effect);

        ~TimeHistoryAnalyzerFrame();

    };

#endif

} // namespace Aurora

#endif // __AURORA_ITUP56_GUI__

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


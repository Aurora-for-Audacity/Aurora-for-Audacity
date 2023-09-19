/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiSpectrum.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_STI_SPECTRUM_H__
#define __AURORA_STI_SPECTRUM_H__

#define ID_RIGHT_PLOT  10002
#define ID_LEVEL_COPY_SELECTION_MENU 10022

namespace Aurora
{

    ///////////////////////////////////////////////////////////////////////////////
    /// Class STISpectrumDialog
    ///////////////////////////////////////////////////////////////////////////////
    class STISpectrumDialog : public STISpectrumDlg
    {
    private:
        Aurora::STIEffect* m_pSti = nullptr;
        bool  m_bIsStereo    = false;
        int   m_nCurrentChnl = 0;

        wxPanel* m_rightPage;
        wxMenu*  m_levelContextMenu;

        wxArrayString m_bandLabels;
            
    // private methods
        void FillTable();
            
        void OnCloseDialog( wxCloseEvent& event );
        void OnChannelSelect( wxCommandEvent& event );
    //    void OnPageChange( wxNotebookEvent& event );
        void OnLevelTableLabelClick( wxGridEvent& event );
        void OnLevelTableRightClick( wxGridEvent& event );

        void OnLevelTableKeyPress( wxKeyEvent& event );
        void OnLevelTableCopySelection(wxCommandEvent & event);

        void StoreGeometry();
        void RestoreGeometry();

    public:
            
        STISpectrumDialog(wxWindow* parent, 
                          Aurora::STIEffect* sti,
                          const bool bIsStereo);
        ~STISpectrumDialog();
    };
} // namespace Aurora

#endif //__AURORA_STI_SPECTRUM_H__

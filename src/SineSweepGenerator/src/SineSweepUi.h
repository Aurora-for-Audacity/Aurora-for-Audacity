/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Generator

  SineSweepUi.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_SineSweepGEN_GUI_H__
#define __AURORA_SineSweepGEN_GUI_H__

namespace Aurora
{
    //----------------------------------------------------------------------------
    // SineSweepGeneratorDialog
    //----------------------------------------------------------------------------
    class SineSweepGeneratorDialog: public SsgDlg 
    { 
       private:
          SineSweepGenerator* m_pSsg;
                   
       protected:
          // handlers
          // Handlers for SsgDlg events.
          void OnLinearSweepRadio( wxCommandEvent& event ) override;
          void OnLogSweepRadio( wxCommandEvent& event ) override;
          void OnPinkSweepRadio( wxCommandEvent& event ) override;
          void OnFadeInChoice( wxCommandEvent& event ) override;
          void OnFadeOutChoice( wxCommandEvent& event ) override;
          void OnOk( wxCommandEvent& event ) override;
          void OnCancel( wxCommandEvent& event ) override;
          void OnHelp( wxCommandEvent& event ) override;
        
          // -- Utilities ---

    public:      
          // constructors and destructors 
          SineSweepGeneratorDialog(SineSweepGenerator* pSsg, wxWindow *pParent);
    };
}

#endif // __AURORA_SineSweepGEN_GUI__

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

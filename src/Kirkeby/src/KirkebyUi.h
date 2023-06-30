/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  gui.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_KIRKEBY_GUI_H__
#define __AURORA_KIRKEBY_GUI_H__

#include "KirkebyDialogs.h"

namespace Aurora
{
    //----------------------------------------------------------------------------
    // KirkebyDialog
    //----------------------------------------------------------------------------
    class KirkebyDialog : public KirkebyDlg
    {
    private:
        KirkebyEffect* m_pKirk;

        //void BuildFilterLengthList();

    protected:
        // Handlers for KirkebyDlg events.
        void OnIrTypeChoice(wxCommandEvent& event);
        void OnMonoFilteringRadio(wxCommandEvent& event);
        void OnStereoFilteringRadio(wxCommandEvent& event);
        void OnDipoleFilteringRadio(wxCommandEvent& event);

        void OnCrossTalkCheck(wxCommandEvent& event);
        void OnMultiIRCheck(wxCommandEvent& event);
        void OnMultiIrText(wxCommandEvent& event);
        //void OnFilterLengthCombo( wxCommandEvent& event );
        //void OnFilterLengthText( wxCommandEvent& event );
        void OnIRPText(wxCommandEvent& event);
        void OnORPText(wxCommandEvent& event);
        void OnLowerCutFrqText(wxCommandEvent& event);
        void OnHigherCutFrqText(wxCommandEvent& event);
        void OnTransitionWidthText(wxCommandEvent& event);
        void OnFadeInText(wxCommandEvent& event);
        void OnFadeOutText(wxCommandEvent& event);
        void OnAverageModeChoice(wxCommandEvent& event);
        void OnAverageTypeChoice(wxCommandEvent& event);
        void OnAverageWidthText(wxCommandEvent& event);
        void OnAutorangeCheck(wxCommandEvent& event);
        void OnGainText(wxCommandEvent& event);
        void OnHelp(wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void OnOk(wxCommandEvent& event);

    public:
        /** Constructor */
        KirkebyDialog(wxWindow* pParent, KirkebyEffect* pKirk, const bool bIsStereo = false);
    };

}  // namespace

#endif // __AURORA_KIRKEBY_GUI_H__

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

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_LOGO_PANEL 1000
#define ID_IR_INFO_STEXT 1001
#define ID_MONO_FILTERING_RADIO 1002
#define ID_STEREO_FILTERING_RADIO 1003
#define ID_DIPOLE_FILTERING_RADIO 1004
#define ID_CROSSTALK_CHECK 1005
#define ID_MULTI_IR_CHECK 1006
#define ID_IR_NUMBER_LABEL_STEXT 1007
#define ID_MULTI_IR_TEXT 1008
#define ID_FILTER_LENGTH_TEXT 1009
#define ID_IRP_TEXT 1010
#define ID_ORP_TEXT 1011
#define ID_LOWER_CUT_FRQ_TEXT 1012
#define ID_HIGHER_CUT_FRQ_TEXT 1013
#define ID_TRANSITION_WIDTH_TEXT 1014
#define ID_FADE_IN_TEXT 1015
#define ID_FADE_OUT_TEXT 1016
#define ID_AVERAGE_MODE_CHOICE 1017
#define ID_AVERAGE_TYPE_LABEL_STEXT 1018
#define ID_AVERAGE_WIDTH_LABEL_STEXT 1019
#define ID_AVERAGE_WIDTH_TEXT 1020
#define ID_AVG_WIDTH_MU_STEXT 1021
#define ID_AUTORANGE_CHECK 1022
#define ID_GAIN_LABEL_STEXT 1023
#define ID_GAIN_MU_STEXT 1024

///////////////////////////////////////////////////////////////////////////////
/// Class KirkebyDlg
///////////////////////////////////////////////////////////////////////////////
class KirkebyDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_irInfoPanel;
		wxStaticText* m_irInfoLabel;
		wxRadioButton* m_monoFilteringButton;
		wxRadioButton* m_stereoFilteringButton;
		wxRadioButton* m_dipoleFilteringButton;
		wxCheckBox* m_crossTalkCheck;
		wxCheckBox* m_multiIRsCheck;
		wxStaticText* m_multiIRNumberLabel;
		wxTextCtrl* m_multiIRText;
		wxPanel* m_filterSettingsPanel;
		wxStaticText* m_flLabel;
		wxTextCtrl* m_filterLengthText;
		wxStaticText* m_flUnitsLabel;
		wxStaticText* m_irpLabel;
		wxTextCtrl* m_irpText;
		wxStaticText* m_irpEmptyLabel;
		wxStaticText* m_orpLabel;
		wxTextCtrl* m_orpText;
		wxStaticText* m_orpEmptyLabel;
		wxStaticText* m_lcfLabel;
		wxTextCtrl* m_lowerCutFrqText;
		wxStaticText* m_lcfUnitsLabel;
		wxStaticText* m_hcfLabel;
		wxTextCtrl* m_higherCutFrqText;
		wxStaticText* m_hcfUnitsLabel;
		wxStaticText* m_twLabel;
		wxTextCtrl* m_transWidthText;
		wxStaticText* m_twUnitsLabel;
		wxStaticText* m_fiLabel;
		wxTextCtrl* m_fadeInText;
		wxStaticText* m_fiUnitsLabel;
		wxStaticText* m_foLabel;
		wxTextCtrl* m_fadeOutText;
		wxStaticText* m_foUnitsLabel;
		wxPanel* m_averagingModePanel;
		wxChoice* m_averageModeChoice;
		wxStaticText* m_atLabel;
		wxChoice* m_averageTypeChoice;
		wxStaticText* m_atEmptyLabel;
		wxStaticText* m_awLabel;
		wxTextCtrl* m_averageWidthText;
		wxStaticText* m_awUnitsLabel;
		wxPanel* m_gainSettingsPanel;
		wxCheckBox* m_autorangeCheck;
		wxStaticText* m_gainLabel;
		wxTextCtrl* m_gainText;
		wxStaticText* m_gainUnitsLabel;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* mHelpBtn;
		wxButton* mCancelBtn;
		wxButton* mOkBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnMonoFilteringRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStereoFilteringRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDipoleFilteringRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCrossTalkCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMultiIRCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMultiIrText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnIRPText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnORPText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLowerCutFrqText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHigherCutFrqText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTransitionWidthText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeInText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeOutText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAverageModeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAverageTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAverageWidthText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAutorangeCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGainText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		KirkebyDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Aurora Invert Kirkeby"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~KirkebyDlg();

};


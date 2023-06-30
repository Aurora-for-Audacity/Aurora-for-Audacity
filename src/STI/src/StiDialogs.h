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
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

#define ID_LOGO_PANEL 1000
#define ID_START_PNL 1001
#define ID_FS_CALIBRATION_PNL 1002
#define ID_LEFT_FS_TC 1003
#define ID_RIGHT_FS_TC 1004
#define ID_LEFT_CAL_CHOICE 1005
#define ID_LEFT_CAL_TC 1006
#define ID_RIGHT_CAL_CHOICE 1007
#define ID_RIGHT_CAL_TC 1008
#define ID_CALIBRATE_BTN 1009
#define ID_SN_PNL 1010
#define ID_LEFT_BCK_NOISE_CHOICE 1011
#define ID_RIGHT_BCK_NOISE_CHOICE 1012
#define ID_LEFT_SIGNAL_CHOICE 1013
#define ID_RIGHT_SIGNAL_CHOICE 1014
#define ID_SIGNAL_RB 1015
#define ID_SIGNOISE_RB 1016
#define ID_SPL_GRID 1017
#define ID_LOAD_SPL_VALUES_BTN 1018
#define ID_SAVE_SPL_VALUES_BTN 1019
#define ID_IR_PNL 1020
#define ID_LEFT_IR_CHOICE 1021
#define ID_RIGHT_IR_CHOICE 1022
#define ID_FAT_TC 1023
#define ID_GRID_PNL 1024
#define ID_MTF_VALUES_GRID 1025
#define ID_BAND_STI_GRID 1026
#define ID_LEFT_RB 1027
#define ID_RIGHT_RB 1028
#define ID_SNCORR_CB 1029
#define ID_MASK_CB 1030
#define ID_STI_MALE_ST 1031
#define ID_STI_FEMALE_ST 1032
#define ID_RASTI_ST 1033
#define ID_SAVE_BTN 1034

///////////////////////////////////////////////////////////////////////////////
/// Class STISetupDlg
///////////////////////////////////////////////////////////////////////////////
class STISetupDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxNotebook* m_notebook;
		wxPanel* m_startPage;
		wxStaticText* m_welcomeMsg;
		wxPanel* m_calibrationPage;
		wxStaticText* m_fsLeftLabel;
		wxTextCtrl* m_fsLeftText;
		wxStaticText* m_fsRightLabel;
		wxTextCtrl* m_fsRightText;
		wxStaticText* m_fsRightUnitsLabel;
		wxStaticText* m_leftCalLabel;
		wxChoice* m_leftCalChoice;
		wxStaticText* m_leftCalLevelLabel;
		wxTextCtrl* m_leftCalLevelText;
		wxStaticText* m_leftCalLevelUnits;
		wxStaticText* m_rightCalLabel;
		wxChoice* m_rightCalChoice;
		wxStaticText* m_rightCalLevelLabel;
		wxTextCtrl* m_rightCalLevelText;
		wxStaticText* m_rightCalLevelUnits;
		wxButton* m_calibrateButton;
		wxPanel* m_snrCalculationPage;
		wxStaticText* m_leftNoiseLabel;
		wxChoice* m_leftNoiseChoice;
		wxStaticText* m_rightNoiseLabel;
		wxChoice* m_rightNoiseChoice;
		wxStaticText* m_leftSignalLabel;
		wxChoice* m_leftSignalChoice;
		wxStaticText* m_rightSignalLabel;
		wxChoice* m_rightSignalChoice;
		wxRadioButton* m_signalRadioButton;
		wxRadioButton* m_sigNoiseRadioButton;
		wxGrid* m_splTable;
		wxButton* m_loadSplValuesButton;
		wxButton* m_saveSplValuesButton;
		wxButton* m_calculateSPLButton;
		wxPanel* m_roomIrPage;
		wxStaticText* m_leftIrLabel;
		wxChoice* m_leftIrChoice;
		wxStaticText* m_rightIrLabel;
		wxChoice* m_rightIrChoice;
		wxStaticText* m_fatLabel;
		wxTextCtrl* m_fatText;
		wxStaticText* m_fatUnitsLabel;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpButton;
		wxButton* m_cancelButton;
		wxButton* m_okButton;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLeftFullscaleText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightFullscaleText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibLeftChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibRefLeftText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibRightChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibRefRightText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDoFullscaleCalibration( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNoiseLeftChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNoiseRightChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSignalLeftChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSignalRightChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSignalRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSignalPlusNoiseRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadSplValues( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveSplValues( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComputeSNRatio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeftIRChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightIRChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFatText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComputeSTI( wxCommandEvent& event ) { event.Skip(); }


	public:

		STISetupDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Aurora STI & Octave Band Analysis"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~STISetupDlg();

};

///////////////////////////////////////////////////////////////////////////////
/// Class STIShowDlg
///////////////////////////////////////////////////////////////////////////////
class STIShowDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_bodyPanel;
		wxNotebook* m_notebook;
		wxPanel* m_tablePage;
		wxGrid* m_mtfTable;
		wxGrid* m_stiTable;
		wxPanel* m_controlsPanel;
		wxRadioButton* m_leftRadioButton;
		wxRadioButton* m_rightRadioButton;
		wxCheckBox* m_snCorrectionCheck;
		wxCheckBox* m_maskCorrectionCheck;
		wxStaticText* m_stiMaleLabel;
		wxStaticText* m_stiMaleValue;
		wxStaticText* m_stiFemaleLabel;
		wxStaticText* m_stiFemaleValue;
		wxStaticText* m_rastiLabel;
		wxStaticText* m_rastiValue;
		wxStaticText* m_stitelLabel;
		wxStaticText* m_stitelValue;
		wxStaticText* m_stipaLabel;
		wxStaticText* m_stipaValue;
		wxButton* m_saveSettingsButton;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpButton;
		wxButton* m_okButton;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnLeftRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSnCorrectionCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMaskCorrectionCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		STIShowDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("STI (according to IEC-EN 60268-16:2003)"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~STIShowDlg();

};


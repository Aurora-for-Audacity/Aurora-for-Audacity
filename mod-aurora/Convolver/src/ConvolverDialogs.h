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
#include <wx/listctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
#include "ConvolverPlot.h"
#include <wx/textctrl.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class AFTrackSelectorDlg
///////////////////////////////////////////////////////////////////////////////
class AFTrackSelectorDlg : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_LOGO_PANEL = 1000,
			ID_AVAIL_TRACKS_LIST,
			ID_TO_AUDIO_DATA_BTN,
			ID_TO_IRs_BTN,
			ID_AUDIO_DATA_LIST,
			ID_MOVE_UP_BTN,
			ID_TO_LEFT_BTN,
			ID_TO_RIGHT_BTN,
			ID_MOVE_DOWN_BTN,
			ID_REMOVE_BTN,
			ID_IR_LIST,
			ID_MATRIX_MODE_CHECK
		};

		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_stepOnePanel;
		wxStaticText* m_stepOneLabel;
		wxStaticLine* m_stepOneLine;
		wxPanel* m_trackListPanel;
		wxStaticText* m_selectedTracksLabel;
		wxListCtrl* mAvailableList;
		wxButton* m_toAudioDataButton;
		wxButton* m_toFiltersButton;
		wxPanel* m_splittedListsPanel;
		wxStaticText* m_audioDataLabel;
		wxListCtrl* mAudioDataList;
		wxBitmapButton* m_upButton;
		wxBitmapButton* m_leftButton;
		wxBitmapButton* m_rightButton;
		wxBitmapButton* m_bottomButton;
		wxButton* m_removeButton;
		wxStaticText* m_filtersLabel;
		wxListCtrl* mFilterList;
		wxPanel* m_checkBoxPanel;
		wxCheckBox* mMatrixModeCheck;
		wxPanel* m_outputDescriptionPanel;
		wxStaticText* m_outputDescriptionLabel;
		wxStaticLine* m_bottomLine;
		wxPanel* m_bottomPanel;
		wxPanel* m_buttonsPanel;
		wxButton* mHelpBtn;
		wxButton* mCancelBtn;
		wxButton* mOkBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnAvailableDragInit( wxListEvent& event ) { event.Skip(); }
		virtual void ToAudioDataBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToIRsBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAudioDataDragInit( wxListEvent& event ) { event.Skip(); }
		virtual void OnMoveUpBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToLeftBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnToRightBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveDownBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFilterDragInit( wxListEvent& event ) { event.Skip(); }
		virtual void OnMatrixModeCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }


	public:

		AFTrackSelectorDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Aurora Convolver"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~AFTrackSelectorDlg();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AFConvolverDlg
///////////////////////////////////////////////////////////////////////////////
class AFConvolverDlg : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_LOGO_PANEL = 1000,
			ID_PLOT,
			ID_FULL_AUTORANGE_CHECK,
			ID_FB_AUTORANGE,
			ID_GAIN_LABEL_ST,
			ID_GAIN_TEXT,
			ID_TREV_IR_CHECK,
			ID_CROSSTALK_CHECK,
			ID_PRES_LENGTH_CHECK,
			ID_AUDIO_DATA_STEXT,
			ID_IR_DATA_STEXT,
			ID_FFT_SIZE_STEXT,
			ID_MODE_STEXT
		};

		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_stepTwoPanel;
		wxStaticText* m_stepTwoLabel;
		wxStaticLine* m_stepTwoLine;
		wxPanel* m_plotPanel;
		Aurora::ConvolverPlot* mPlot;
		wxPanel* m_optionsPanel;
		wxCheckBox* mFullAutorangeCheck;
		wxCheckBox* mFBAutorangeCheck;
		wxStaticText* mGainStext;
		wxTextCtrl* mGainText;
		wxStaticText* mdBStext;
		wxCheckBox* mTimeReversedCheck;
		wxCheckBox* mCrossTalkCheck;
		wxCheckBox* mPreserveLengthCheck;
		wxPanel* m_infoPanel;
		wxStaticText* m_audioDataLabel;
		wxStaticText* mAudioDataStext;
		wxStaticText* m_irLabel;
		wxStaticText* mFilterStext;
		wxStaticText* m_fftSizeLabel;
		wxStaticText* mFftSizeStext;
		wxStaticText* m_modeLabel;
		wxStaticText* mModeStext;
		wxStaticLine* m_bottomLine;
		wxPanel* m_bottomPanel;
		wxButton* mHelpBtn;
		wxButton* mCancelBtn;
		wxButton* mOkBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnFullAutorangeCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFirstBlockAutorangeCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGainText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimeReverseIrCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCrosstalkCancelCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPreserveLengthCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		AFConvolverDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Aurora Convolver"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~AFConvolverDlg();

};


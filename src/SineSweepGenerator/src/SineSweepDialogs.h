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
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/radiobut.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class SsgDlg
///////////////////////////////////////////////////////////////////////////////
class SsgDlg : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_LOGO_PANEL = 1000,
			ID_START_FREQ_TEXT,
			ID_END_FREQ_TEXT,
			ID_DURATION_TEXT,
			ID_AMPLITUDE_TEXT,
			ID_CHANNELS_TEXT,
			ID_LINEAR_SWEEP_RADIO,
			ID_LOG_SWEEP_RADIO,
			ID_PINK_SWEEP_RADIO,
			ID_FADE_IN_TEXT,
			ID_FADE_IN_CHOICE,
			ID_FADE_OUT_TEXT,
			ID_FADE_OUT_CHOICE,
			ID_SILENCE_TEXT,
			ID_REPETITIONS_TEXT,
			ID_DELTA_L_TEXT,
			ID_CTRL_PULSES_CHECK
		};

		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_topPanel;
		wxStaticText* m_fromLabel;
		wxTextCtrl* m_fromText;
		wxStaticText* m_hzToLabel;
		wxTextCtrl* m_toText;
		wxStaticText* m_hzLabel;
		wxStaticText* m_durationLabel;
		wxTextCtrl* m_durationText;
		wxStaticText* m_sOrSamplesLabel;
		wxStaticText* m_amplidtudeLabel;
		wxTextCtrl* m_amplitudeText;
		wxStaticText* m_amplitudeRangeLabel;
		wxStaticText* m_channelsLabel;
		wxTextCtrl* m_chnlsCountText;
		wxStaticText* m_emptyLabel;
		wxRadioButton* m_linearSweepBtn;
		wxRadioButton* m_expSweepBtn;
		wxRadioButton* m_pinkSweepBtn;
		wxStaticText* m_fadeInLabel;
		wxStaticText* m_fadeInSamplesLabel;
		wxTextCtrl* m_fadeInText;
		wxChoice* m_fadeInChoice;
		wxStaticText* m_fadeOutLabel;
		wxStaticText* m_fadeOutSampleLabel;
		wxTextCtrl* m_pwxtcFadeOut;
		wxChoice* m_fadeOutChoice;
		wxPanel* m_midPanel;
		wxStaticText* m_silenceDurationLabel;
		wxTextCtrl* m_silenceDurationText;
		wxStaticText* m_ncyclesLabel;
		wxTextCtrl* m_cyclesCountText;
		wxStaticText* m_levelVarLabel;
		wxTextCtrl* m_deltaLText;
		wxCheckBox* m_pulsesCheck;
		wxStaticLine* m_bottomLine;
		wxPanel* m_bottomPanel;
		wxButton* m_helpBtn;
		wxButton* m_cancelBtn;
		wxButton* m_generateBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnStartFrequencyText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEndFrequencyText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSweepDurationText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAmplitudeText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLinearSweepRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLogSweepRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPinkSweepRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeInText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeInChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeOutText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFadeOutChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSilenceDurationText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRepetitionsText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeltaLText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnControlPulsesCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		SsgDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Generate Sine Sweep"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~SsgDlg();

};


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
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include "XFunctionsPlot.h"
#include <wx/scrolwin.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////

#define ID_FFT_SIZE_CHOICE 1000
#define ID_WINDOW_CHOICE 1001
#define ID_XFUNCTION_CHOICE 1002
#define ID_EQUATION_SBMP 1003
#define ID_LAG_LABEL_STEXT 1004
#define ID_LAG_STEXT 1005
#define ID_SW_TUBE_CHECK 1006
#define ID_FREE_FIELD_CHECK 1007
#define ID_SOUNDSPEED_TEXT 1008
#define ID_PROBE_DIST_TEXT 1009
#define ID_FMAX_TEXT 1010
#define ID_ENABLE_FFILTER_CHECK 1011
#define ID_FOLLOW_LEFT_RADIO 1012
#define ID_FOLLOW_RIGHT_RADIO 1013
#define ID_BANDWIDTH_TEXT 1014
#define ID_NORMALIZE_LEFT_CHECK 1015
#define ID_HILBERT_CHECK 1016
#define ID_SHIFT_TO_HALF_CHECK 1017
#define ID_TIME_REV_CHECK 1018
#define ID_COHERENCE_CHECK 1019
#define ID_DIRAC_CHECK 1020
#define ID_TRIGGER_TEXT 1021
#define ID_FFT_SIZE_STEXT 1022
#define ID_CHANNELS_STEXT 1023
#define ID_RATE_STEXT 1024
#define ID_NBLOCKS_STEXT 1025
#define ID_TRANSFORM_TYPE_STEXT 1026
#define ID_RMS_LEFT_LABEL_STEXT 1027
#define ID_RMS_LEFT_STEXT 1028
#define ID_RMS_LEFT_dB_STEXT 1029
#define ID_RMS_RIGHT_LABEL_STEXT 1030
#define ID_RMS_RIGHT_STEXT 1031
#define ID_RMS_RIGHT_dB_STEXT 1032
#define ID_RMS_CROSS_LABEL_STEXT 1033
#define ID_RMS_CROSS_STEXT 1034
#define ID_RMS_CROSS_dB_STEXT 1035
#define ID_TIME_OF_FLIGHT_STEXT 1036
#define ID_TRIGGER_TIME_LABEL_STEXT 1037
#define ID_TRIGGER_TIME_STEXT 1038
#define ID_TRIGGER_TIME_AT_STEXT 1039
#define ID_TRIGGER_DELAY_STEXT 1040
#define ID_NCC_MAX_LABEL_STEXT 1041
#define ID_NCC_MAX_STEXT 1042
#define ID_NCC_MAX_AT_STEXT 1043
#define ID_NCC_MAX_DELAY_STEXT 1044
#define ID_SAVE_TO_TIMEDELAY_CHECK 1045
#define ID_EXPORT_ALL_CHECK 1046
#define ID_SAVE_SPECTRUM_BTN 1047
#define ID_COPY_SPECTRUM_BTN 1048
#define ID_EXPORT_WAVEFORMS_CHECK 1049
#define ID_RSIDE_SPLITTER 1050
#define ID_PLOT_SPLITTER 1051
#define ID_UPPER_PLOT 1052
#define ID_LOWER_PLOT 1053
#define ID_FIRST_PLOT_STEXT 1054
#define ID_FIRST_PLOT_CHOICE 1055
#define ID_SECOND_PLOT_STEXT 1056
#define ID_SECOND_PLOT_CHOICE 1057
#define ID_FREQUENCY_RANGE_CHOICE 1058
#define ID_LINEAR_SCALE_CHECK 1059

///////////////////////////////////////////////////////////////////////////////
/// Class XFunctionsDlg
///////////////////////////////////////////////////////////////////////////////
class XFunctionsDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_fftPanel;
		wxStaticText* m_fftSizeLabel;
		wxChoice* m_fftSizeChoice;
		wxStaticText* m_fftWindowLabel;
		wxChoice* m_windowChoice;
		wxPanel* m_functionSetupPanel;
		wxChoice* m_xfunctionChoice;
		wxPanel* m_equationPanel;
		wxStaticBitmap* m_equationBmp;
		wxStaticText* m_lagLabel;
		wxStaticText* m_lagValueLabel;
		wxPanel* m_puProbeSetupPanel;
		wxCheckBox* m_swTubeCheck;
		wxCheckBox* m_freeFieldCheck;
		wxStaticText* m_soundSpeedLabel;
		wxTextCtrl* m_soundSpeedText;
		wxStaticText* m_probeDistLabel;
		wxTextCtrl* m_probeDistText;
		wxStaticText* m_fmaxLabel;
		wxTextCtrl* m_fmaxText;
		wxPanel* m_ffilterPanel;
		wxCheckBox* m_enableFFilterCheck;
		wxStaticText* m_emptyLabel;
		wxStaticText* m_bwLabel;
		wxRadioButton* m_followLeftRadio;
		wxRadioButton* m_followRightRadio;
		wxTextCtrl* m_bandwidthText;
		wxPanel* m_outputSetupPanel;
		wxCheckBox* m_normalizeLeftCheck;
		wxCheckBox* m_hilbertCheck;
		wxCheckBox* m_shiftToHalfCheck;
		wxCheckBox* m_timeReversalCheck;
		wxCheckBox* m_coherenceCheck;
		wxCheckBox* m_diracCheck;
		wxStaticText* m_triggerLevelLabel;
		wxTextCtrl* m_triggerText;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpBtn;
		wxButton* m_cancelBtn;
		wxButton* m_okBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnFftSizeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWindowChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnXFunctionChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSWTubeCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFreeFieldCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSoundSpeedText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnProbeDistanceText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFmaxText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnableFollowingFilterCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFollowLeftRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFollowRightRadio( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBandwidthText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNormalizeLeftChannelCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHilbertCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShiftToHalfWindowCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTimeReversalCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCoherenceCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDiracPulseCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTriggerLevelText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		XFunctionsDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Cross Functions"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~XFunctionsDlg();

};

///////////////////////////////////////////////////////////////////////////////
/// Class XFunctionsShowDlg
///////////////////////////////////////////////////////////////////////////////
class XFunctionsShowDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_upperPanel;
		wxPanel* m_inputInfoPanel;
		wxStaticText* m_fftSizeLabel;
		wxStaticText* m_fftSizeValueLabel;
		wxStaticText* m_fftSizeUnitLabel;
		wxStaticText* m_channelsLabel;
		wxStaticText* m_channelsValueLabel;
		wxStaticText* m_channelsUnitLabel;
		wxStaticText* m_rateLabel;
		wxStaticText* m_rateValueLabel;
		wxStaticText* m_rateUnitLabel;
		wxStaticText* m_nblocksLabel;
		wxStaticText* m_nblocksValueLabel;
		wxStaticText* m_nblocksUnitLabel;
		wxPanel* m_ttypePanel;
		wxStaticText* m_transformTypeLabel;
		wxPanel* m_statsPanel;
		wxStaticText* m_rmsLeftLabel;
		wxStaticText* m_rmsLeftValueLabel;
		wxStaticText* m_rmsLeftUnitLabel;
		wxStaticText* m_rmsRightLabel;
		wxStaticText* m_rmsRightValueLabel;
		wxStaticText* m_rmsRightUnitLabel;
		wxStaticText* m_rmsCrossLabel;
		wxStaticText* m_rmsCrossValueLabel;
		wxStaticText* m_rmsCrossUnitLabel;
		wxStaticLine* m_statsLine;
		wxStaticText* m_timeOfFlightValueLabel;
		wxStaticText* m_triggerTimeLabel;
		wxStaticText* m_triggerTimeValueLabel;
		wxStaticText* m_triggerTimeAtLabel;
		wxStaticText* m_triggerDelayValueLabel;
		wxStaticText* m_nccMaxLabel;
		wxStaticText* m_nccMaxValueLabel;
		wxStaticText* m_nccMaxAtLabel;
		wxStaticText* m_nccMaxDelayValueLabel;
		wxCheckBox* m_saveToTimeDelayCheck;
		wxPanel* m_exportPanel;
		wxCheckBox* m_exportAllCheck;
		wxButton* m_saveBtn;
		wxButton* m_copyBtn;
		wxCheckBox* m_exportWaveformsCheck;
		wxSplitterWindow* m_rightSideSplitter;
		wxPanel* m_rightUpperPanel;
		wxSplitterWindow* m_plotSplitter;
		wxPanel* m_upperSplitterPanel;
		wxScrolledWindow* m_upperPlotScroller;
		Aurora::XFunctionsPlot *m_upperPlot;
		wxPanel* m_lowerSplitterPanel;
		wxScrolledWindow* m_lowerPlotScroller;
		Aurora::XFunctionsPlot *m_lowerPlot;
		wxPanel* m_rightLowerPanel;
		wxStaticText* m_upperPlotLabel;
		wxChoice* m_upperPlotChoice;
		wxStaticText* m_lowerPlotLabel;
		wxChoice* m_lowerPlotChoice;
		wxStaticText* m_frangeLabel;
		wxChoice* m_frequencyRangeChoice;
		wxCheckBox* m_linearScaleCheck;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpBtn;
		wxButton* m_closeBtn;

		// Virtual event handlers, overide them in your derived class
		virtual void OnSaveToTimeDelayCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportAllSpectrumsCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveSpectrumBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCopySpectrumBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExportWaveformsCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFirstPlotChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSecondPlotChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFrequencyRangeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLinearScaleCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCommandEvent& event ) { event.Skip(); }


	public:

		XFunctionsShowDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Cross Functions"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
		~XFunctionsShowDlg();

		void m_rightSideSplitterOnIdle( wxIdleEvent& )
		{
		m_rightSideSplitter->SetSashPosition( 420 );
		m_rightSideSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( XFunctionsShowDlg::m_rightSideSplitterOnIdle ), NULL, this );
		}

		void m_plotSplitterOnIdle( wxIdleEvent& )
		{
		m_plotSplitter->SetSashPosition( 0 );
		m_plotSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( XFunctionsShowDlg::m_plotSplitterOnIdle ), NULL, this );
		}

};


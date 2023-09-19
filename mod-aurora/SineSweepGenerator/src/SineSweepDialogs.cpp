///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../src/SineSweepDialogs.h"

///////////////////////////////////////////////////////////////////////////

SsgDlg::SsgDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( 380,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_topPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* topPanelSizer;
	topPanelSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sweepParametersSizer;
	sweepParametersSizer = new wxStaticBoxSizer( new wxStaticBox( m_topPanel, wxID_ANY, wxT("Sweep") ), wxVERTICAL );

	wxBoxSizer* freqSizer;
	freqSizer = new wxBoxSizer( wxHORIZONTAL );

	m_fromLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("From"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fromLabel->Wrap( -1 );
	freqSizer->Add( m_fromLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fromText = new wxTextCtrl( sweepParametersSizer->GetStaticBox(), ID_START_FREQ_TEXT, wxT("22.0"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	freqSizer->Add( m_fromText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_hzToLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("Hz  to"), wxDefaultPosition, wxDefaultSize, 0 );
	m_hzToLabel->Wrap( -1 );
	freqSizer->Add( m_hzToLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_toText = new wxTextCtrl( sweepParametersSizer->GetStaticBox(), ID_END_FREQ_TEXT, wxT("22000.0"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	freqSizer->Add( m_toText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_hzLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_hzLabel->Wrap( -1 );
	freqSizer->Add( m_hzLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	sweepParametersSizer->Add( freqSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_durationLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("Duration"), wxDefaultPosition, wxDefaultSize, 0 );
	m_durationLabel->Wrap( -1 );
	fgSizer->Add( m_durationLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_durationText = new wxTextCtrl( sweepParametersSizer->GetStaticBox(), ID_DURATION_TEXT, wxT("15.0"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	m_durationText->SetToolTip( wxT("Duration of the sweep; if the number is < 100 is taken as seconds, otherwise it's taken as samples count.") );

	fgSizer->Add( m_durationText, 0, wxALL, 5 );

	m_sOrSamplesLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("s or samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sOrSamplesLabel->Wrap( -1 );
	fgSizer->Add( m_sOrSamplesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_amplidtudeLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("Amplitude"), wxDefaultPosition, wxDefaultSize, 0 );
	m_amplidtudeLabel->Wrap( -1 );
	fgSizer->Add( m_amplidtudeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_amplitudeText = new wxTextCtrl( sweepParametersSizer->GetStaticBox(), ID_AMPLITUDE_TEXT, wxT("0.5"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	fgSizer->Add( m_amplitudeText, 0, wxALL, 5 );

	m_amplitudeRangeLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("(0 - 1.0)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_amplitudeRangeLabel->Wrap( -1 );
	fgSizer->Add( m_amplitudeRangeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_channelsLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxT("Channels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_channelsLabel->Wrap( -1 );
	fgSizer->Add( m_channelsLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_chnlsCountText = new wxTextCtrl( sweepParametersSizer->GetStaticBox(), ID_CHANNELS_TEXT, wxT("1"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	fgSizer->Add( m_chnlsCountText, 0, wxALL, 5 );

	m_emptyLabel = new wxStaticText( sweepParametersSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_emptyLabel->Wrap( -1 );
	fgSizer->Add( m_emptyLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	sweepParametersSizer->Add( fgSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxBoxSizer* sweepTypeSizer;
	sweepTypeSizer = new wxBoxSizer( wxHORIZONTAL );

	m_linearSweepBtn = new wxRadioButton( sweepParametersSizer->GetStaticBox(), ID_LINEAR_SWEEP_RADIO, wxT("Linear Sweep"), wxDefaultPosition, wxDefaultSize, 0 );
	sweepTypeSizer->Add( m_linearSweepBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_expSweepBtn = new wxRadioButton( sweepParametersSizer->GetStaticBox(), ID_LOG_SWEEP_RADIO, wxT("Exp Sweep"), wxDefaultPosition, wxDefaultSize, 0 );
	m_expSweepBtn->SetValue( true );
	sweepTypeSizer->Add( m_expSweepBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_pinkSweepBtn = new wxRadioButton( sweepParametersSizer->GetStaticBox(), ID_PINK_SWEEP_RADIO, wxT("Pink Sweep"), wxDefaultPosition, wxDefaultSize, 0 );
	sweepTypeSizer->Add( m_pinkSweepBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	sweepParametersSizer->Add( sweepTypeSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	topPanelSizer->Add( sweepParametersSizer, 0, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* fadeSizer;
	fadeSizer = new wxStaticBoxSizer( new wxStaticBox( m_topPanel, wxID_ANY, wxT("Fade-in and Fade-out duration") ), wxVERTICAL );

	wxFlexGridSizer* fadeFgSizer;
	fadeFgSizer = new wxFlexGridSizer( 2, 4, 0, 0 );
	fadeFgSizer->SetFlexibleDirection( wxBOTH );
	fadeFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_fadeInLabel = new wxStaticText( fadeSizer->GetStaticBox(), wxID_ANY, wxT("Fade-in"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fadeInLabel->Wrap( -1 );
	fadeFgSizer->Add( m_fadeInLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fadeInSamplesLabel = new wxStaticText( fadeSizer->GetStaticBox(), wxID_ANY, wxT("(s or samples)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fadeInSamplesLabel->Wrap( -1 );
	fadeFgSizer->Add( m_fadeInSamplesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_fadeInText = new wxTextCtrl( fadeSizer->GetStaticBox(), ID_FADE_IN_TEXT, wxT("0.5"), wxDefaultPosition, wxSize( 60,25 ), wxTE_RIGHT );
	fadeFgSizer->Add( m_fadeInText, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxString m_fadeInChoiceChoices[] = { wxT("Rectangular"), wxT("Hamming"), wxT("Hann"), wxT("1/4 Sine"), wxT("Linear"), wxT("Gaussian"), wxT("Blackmann"), wxT("Black/Harris"), wxT("Log") };
	int m_fadeInChoiceNChoices = sizeof( m_fadeInChoiceChoices ) / sizeof( wxString );
	m_fadeInChoice = new wxChoice( fadeSizer->GetStaticBox(), ID_FADE_IN_CHOICE, wxDefaultPosition, wxSize( -1,25 ), m_fadeInChoiceNChoices, m_fadeInChoiceChoices, 0 );
	m_fadeInChoice->SetSelection( 2 );
	fadeFgSizer->Add( m_fadeInChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fadeOutLabel = new wxStaticText( fadeSizer->GetStaticBox(), wxID_ANY, wxT("Fade-out"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fadeOutLabel->Wrap( -1 );
	fadeFgSizer->Add( m_fadeOutLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_fadeOutSampleLabel = new wxStaticText( fadeSizer->GetStaticBox(), wxID_ANY, wxT("(s or samples)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fadeOutSampleLabel->Wrap( -1 );
	fadeFgSizer->Add( m_fadeOutSampleLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_pwxtcFadeOut = new wxTextCtrl( fadeSizer->GetStaticBox(), ID_FADE_OUT_TEXT, wxT("0.1"), wxDefaultPosition, wxSize( 60,25 ), wxTE_RIGHT );
	fadeFgSizer->Add( m_pwxtcFadeOut, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxString m_fadeOutChoiceChoices[] = { wxT("Rectangular"), wxT("Hamming"), wxT("Hann"), wxT("1/4 Sine"), wxT("Linear"), wxT("Gaussian"), wxT("Blackmann"), wxT("Black/Harris"), wxT("Log") };
	int m_fadeOutChoiceNChoices = sizeof( m_fadeOutChoiceChoices ) / sizeof( wxString );
	m_fadeOutChoice = new wxChoice( fadeSizer->GetStaticBox(), ID_FADE_OUT_CHOICE, wxDefaultPosition, wxSize( -1,25 ), m_fadeOutChoiceNChoices, m_fadeOutChoiceChoices, 0 );
	m_fadeOutChoice->SetSelection( 2 );
	fadeFgSizer->Add( m_fadeOutChoice, 0, wxALL, 5 );


	fadeSizer->Add( fadeFgSizer, 1, wxEXPAND, 5 );


	topPanelSizer->Add( fadeSizer, 1, wxEXPAND|wxALL, 5 );


	m_topPanel->SetSizer( topPanelSizer );
	m_topPanel->Layout();
	topPanelSizer->Fit( m_topPanel );
	mainSizer->Add( m_topPanel, 0, wxALL|wxEXPAND, 5 );

	m_midPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* midPanelSizer;
	midPanelSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* silenceSizer;
	silenceSizer = new wxStaticBoxSizer( new wxStaticBox( m_midPanel, wxID_ANY, wxT("Silence") ), wxHORIZONTAL );

	m_silenceDurationLabel = new wxStaticText( silenceSizer->GetStaticBox(), wxID_ANY, wxT("Duration (s or samples)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_silenceDurationLabel->Wrap( -1 );
	silenceSizer->Add( m_silenceDurationLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_silenceDurationText = new wxTextCtrl( silenceSizer->GetStaticBox(), ID_SILENCE_TEXT, wxT("5.0"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	silenceSizer->Add( m_silenceDurationText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	midPanelSizer->Add( silenceSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	wxStaticBoxSizer* repetitionsSizer;
	repetitionsSizer = new wxStaticBoxSizer( new wxStaticBox( m_midPanel, wxID_ANY, wxT("Repetitions") ), wxHORIZONTAL );

	wxFlexGridSizer* repetitionsSbSizer;
	repetitionsSbSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	repetitionsSbSizer->SetFlexibleDirection( wxBOTH );
	repetitionsSbSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_ncyclesLabel = new wxStaticText( repetitionsSizer->GetStaticBox(), wxID_ANY, wxT("Number of cycles"), wxDefaultPosition, wxDefaultSize, 0 );
	m_ncyclesLabel->Wrap( -1 );
	repetitionsSbSizer->Add( m_ncyclesLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_cyclesCountText = new wxTextCtrl( repetitionsSizer->GetStaticBox(), ID_REPETITIONS_TEXT, wxT("1"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	repetitionsSbSizer->Add( m_cyclesCountText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_levelVarLabel = new wxStaticText( repetitionsSizer->GetStaticBox(), wxID_ANY, wxT("Level variation (dB/rep)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_levelVarLabel->Wrap( -1 );
	repetitionsSbSizer->Add( m_levelVarLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_deltaLText = new wxTextCtrl( repetitionsSizer->GetStaticBox(), ID_DELTA_L_TEXT, wxT("0.0"), wxDefaultPosition, wxSize( -1,25 ), wxTE_RIGHT );
	repetitionsSbSizer->Add( m_deltaLText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	repetitionsSizer->Add( repetitionsSbSizer, 1, wxEXPAND|wxALL, 5 );


	midPanelSizer->Add( repetitionsSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_pulsesCheck = new wxCheckBox( m_midPanel, ID_CTRL_PULSES_CHECK, wxT("Add a control pulses track"), wxDefaultPosition, wxDefaultSize, 0 );
	midPanelSizer->Add( m_pulsesCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_midPanel->SetSizer( midPanelSizer );
	m_midPanel->Layout();
	midPanelSizer->Fit( m_midPanel );
	mainSizer->Add( m_midPanel, 1, wxALL|wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bottomSizer;
	bottomSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpBtn = new wxButton( m_bottomPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	bottomSizer->Add( m_helpBtn, 0, wxALL, 5 );


	bottomSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_cancelBtn = new wxButton( m_bottomPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bottomSizer->Add( m_cancelBtn, 0, wxALL, 5 );

	m_generateBtn = new wxButton( m_bottomPanel, wxID_OK, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );

	m_generateBtn->SetDefault();
	bottomSizer->Add( m_generateBtn, 0, wxALL, 5 );


	m_bottomPanel->SetSizer( bottomSizer );
	m_bottomPanel->Layout();
	bottomSizer->Fit( m_bottomPanel );
	mainSizer->Add( m_bottomPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SsgDlg::OnCloseDialog ) );
	m_fromText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnStartFrequencyText ), NULL, this );
	m_toText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnEndFrequencyText ), NULL, this );
	m_durationText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnSweepDurationText ), NULL, this );
	m_amplitudeText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnAmplitudeText ), NULL, this );
	m_linearSweepBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnLinearSweepRadio ), NULL, this );
	m_expSweepBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnLogSweepRadio ), NULL, this );
	m_pinkSweepBtn->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnPinkSweepRadio ), NULL, this );
	m_fadeInText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnFadeInText ), NULL, this );
	m_fadeInChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SsgDlg::OnFadeInChoice ), NULL, this );
	m_pwxtcFadeOut->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnFadeOutText ), NULL, this );
	m_fadeOutChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SsgDlg::OnFadeOutChoice ), NULL, this );
	m_silenceDurationText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnSilenceDurationText ), NULL, this );
	m_cyclesCountText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnRepetitionsText ), NULL, this );
	m_deltaLText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnDeltaLText ), NULL, this );
	m_pulsesCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SsgDlg::OnControlPulsesCheck ), NULL, this );
	m_helpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnHelp ), NULL, this );
	m_cancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnCancel ), NULL, this );
	m_generateBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnOk ), NULL, this );
}

SsgDlg::~SsgDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( SsgDlg::OnCloseDialog ) );
	m_fromText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnStartFrequencyText ), NULL, this );
	m_toText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnEndFrequencyText ), NULL, this );
	m_durationText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnSweepDurationText ), NULL, this );
	m_amplitudeText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnAmplitudeText ), NULL, this );
	m_linearSweepBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnLinearSweepRadio ), NULL, this );
	m_expSweepBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnLogSweepRadio ), NULL, this );
	m_pinkSweepBtn->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( SsgDlg::OnPinkSweepRadio ), NULL, this );
	m_fadeInText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnFadeInText ), NULL, this );
	m_fadeInChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SsgDlg::OnFadeInChoice ), NULL, this );
	m_pwxtcFadeOut->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnFadeOutText ), NULL, this );
	m_fadeOutChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SsgDlg::OnFadeOutChoice ), NULL, this );
	m_silenceDurationText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnSilenceDurationText ), NULL, this );
	m_cyclesCountText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnRepetitionsText ), NULL, this );
	m_deltaLText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SsgDlg::OnDeltaLText ), NULL, this );
	m_pulsesCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SsgDlg::OnControlPulsesCheck ), NULL, this );
	m_helpBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnHelp ), NULL, this );
	m_cancelBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnCancel ), NULL, this );
	m_generateBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SsgDlg::OnOk ), NULL, this );

}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "XFunctionsDialogs.h"

#include "../res/equations/null.xpm"

///////////////////////////////////////////////////////////////////////////

XFunctionsDlg::XFunctionsDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 1, wxEXPAND | wxALL, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_fftPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* fftSetupBoxSizer;
	fftSetupBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_fftPanel, wxID_ANY, wxT("FFT Setup") ), wxVERTICAL );

	wxBoxSizer* fftSetupSizer;
	fftSetupSizer = new wxBoxSizer( wxHORIZONTAL );

	m_fftSizeLabel = new wxStaticText( fftSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("Length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftSizeLabel->Wrap( -1 );
	fftSetupSizer->Add( m_fftSizeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_fftSizeChoiceChoices[] = { wxT("512"), wxT("1024"), wxT("2048"), wxT("4096"), wxT("8192"), wxT("16384"), wxT("32768"), wxT("65536"), wxT("131072"), wxT("262144"), wxT("524288"), wxT("1048576") };
	int m_fftSizeChoiceNChoices = sizeof( m_fftSizeChoiceChoices ) / sizeof( wxString );
	m_fftSizeChoice = new wxChoice( fftSetupBoxSizer->GetStaticBox(), ID_FFT_SIZE_CHOICE, wxDefaultPosition, wxDefaultSize, m_fftSizeChoiceNChoices, m_fftSizeChoiceChoices, 0 );
	m_fftSizeChoice->SetSelection( 5 );
	fftSetupSizer->Add( m_fftSizeChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	fftSetupSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_fftWindowLabel = new wxStaticText( fftSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("Window:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftWindowLabel->Wrap( -1 );
	fftSetupSizer->Add( m_fftWindowLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_windowChoiceChoices[] = { wxT("Rectangular"), wxT("Hanning"), wxT("Blackman-Harris"), wxT("Flat-Top"), wxT("Triangular"), wxT("Half-Sine") };
	int m_windowChoiceNChoices = sizeof( m_windowChoiceChoices ) / sizeof( wxString );
	m_windowChoice = new wxChoice( fftSetupBoxSizer->GetStaticBox(), ID_WINDOW_CHOICE, wxDefaultPosition, wxDefaultSize, m_windowChoiceNChoices, m_windowChoiceChoices, 0 );
	m_windowChoice->SetSelection( 1 );
	fftSetupSizer->Add( m_windowChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	fftSetupBoxSizer->Add( fftSetupSizer, 1, wxEXPAND, 5 );


	m_fftPanel->SetSizer( fftSetupBoxSizer );
	m_fftPanel->Layout();
	fftSetupBoxSizer->Fit( m_fftPanel );
	mainSizer->Add( m_fftPanel, 0, wxALL|wxEXPAND, 5 );

	m_functionSetupPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* functionSetupBoxSizer;
	functionSetupBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_functionSetupPanel, wxID_ANY, wxT("Processing Function:") ), wxVERTICAL );

	wxString m_xfunctionChoiceChoices[] = { wxT("Auto-Correlation"), wxT("Cross-Correlation"), wxT("White Cross-Correlation"), wxT("Cross-Power Spectrum Phase"), wxT("Transfer Function H1"), wxT("Transfer Function H2"), wxT("Transfer Function H3"), wxT("Z & Absorption Coefficient Alpha (PU probe)"), wxT("Z & Absorption Coefficient Alpha (PP probe)") };
	int m_xfunctionChoiceNChoices = sizeof( m_xfunctionChoiceChoices ) / sizeof( wxString );
	m_xfunctionChoice = new wxChoice( functionSetupBoxSizer->GetStaticBox(), ID_XFUNCTION_CHOICE, wxDefaultPosition, wxDefaultSize, m_xfunctionChoiceNChoices, m_xfunctionChoiceChoices, 0 );
	m_xfunctionChoice->SetSelection( 1 );
	functionSetupBoxSizer->Add( m_xfunctionChoice, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_equationPanel = new wxPanel( functionSetupBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize( 310,60 ), wxTAB_TRAVERSAL );
	wxBoxSizer* equationSizer;
	equationSizer = new wxBoxSizer( wxVERTICAL );

	m_equationBmp = new wxStaticBitmap( m_equationPanel, ID_EQUATION_SBMP, wxBitmap( null_xpm ), wxDefaultPosition, wxDefaultSize, 0 );
	equationSizer->Add( m_equationBmp, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_equationPanel->SetSizer( equationSizer );
	m_equationPanel->Layout();
	functionSetupBoxSizer->Add( m_equationPanel, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* lagSizer;
	lagSizer = new wxBoxSizer( wxHORIZONTAL );

	m_lagLabel = new wxStaticText( functionSetupBoxSizer->GetStaticBox(), ID_LAG_LABEL_STEXT, wxT("Lag:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lagLabel->Wrap( -1 );
	m_lagLabel->Enable( false );

	lagSizer->Add( m_lagLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lagValueLabel = new wxStaticText( functionSetupBoxSizer->GetStaticBox(), ID_LAG_STEXT, wxT("9.9 s"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lagValueLabel->Wrap( -1 );
	m_lagValueLabel->Enable( false );

	lagSizer->Add( m_lagValueLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	functionSetupBoxSizer->Add( lagSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_functionSetupPanel->SetSizer( functionSetupBoxSizer );
	m_functionSetupPanel->Layout();
	functionSetupBoxSizer->Fit( m_functionSetupPanel );
	mainSizer->Add( m_functionSetupPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_puProbeSetupPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* puProbeSetupBoxSizer;
	puProbeSetupBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_puProbeSetupPanel, wxID_ANY, wxT("Computation of FIR filters for PU Probe calibration") ), wxVERTICAL );

	wxBoxSizer* puProbeSetupTopSizer;
	puProbeSetupTopSizer = new wxBoxSizer( wxHORIZONTAL );

	m_swTubeCheck = new wxCheckBox( puProbeSetupBoxSizer->GetStaticBox(), ID_SW_TUBE_CHECK, wxT("Rigid termination in a SW tube"), wxDefaultPosition, wxDefaultSize, 0 );
	puProbeSetupTopSizer->Add( m_swTubeCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	puProbeSetupTopSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_freeFieldCheck = new wxCheckBox( puProbeSetupBoxSizer->GetStaticBox(), ID_FREE_FIELD_CHECK, wxT("Free Field"), wxDefaultPosition, wxDefaultSize, 0 );
	puProbeSetupTopSizer->Add( m_freeFieldCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	puProbeSetupBoxSizer->Add( puProbeSetupTopSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* puProbeSetupBottomSizer;
	puProbeSetupBottomSizer = new wxBoxSizer( wxHORIZONTAL );

	m_soundSpeedLabel = new wxStaticText( puProbeSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("c [m/s]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_soundSpeedLabel->Wrap( -1 );
	puProbeSetupBottomSizer->Add( m_soundSpeedLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_soundSpeedText = new wxTextCtrl( puProbeSetupBoxSizer->GetStaticBox(), ID_SOUNDSPEED_TEXT, wxT("343.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_soundSpeedText->SetMinSize( wxSize( 50,-1 ) );

	puProbeSetupBottomSizer->Add( m_soundSpeedText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_probeDistLabel = new wxStaticText( puProbeSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("d [mm]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_probeDistLabel->Wrap( -1 );
	puProbeSetupBottomSizer->Add( m_probeDistLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_probeDistText = new wxTextCtrl( puProbeSetupBoxSizer->GetStaticBox(), ID_PROBE_DIST_TEXT, wxT("40.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_probeDistText->SetMinSize( wxSize( 50,-1 ) );

	puProbeSetupBottomSizer->Add( m_probeDistText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fmaxLabel = new wxStaticText( puProbeSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("fmax [Hz]:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fmaxLabel->Wrap( -1 );
	puProbeSetupBottomSizer->Add( m_fmaxLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM|wxLEFT, 5 );

	m_fmaxText = new wxTextCtrl( puProbeSetupBoxSizer->GetStaticBox(), ID_FMAX_TEXT, wxT("1900.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_fmaxText->SetMinSize( wxSize( 65,-1 ) );

	puProbeSetupBottomSizer->Add( m_fmaxText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	puProbeSetupBoxSizer->Add( puProbeSetupBottomSizer, 1, wxEXPAND, 5 );


	m_puProbeSetupPanel->SetSizer( puProbeSetupBoxSizer );
	m_puProbeSetupPanel->Layout();
	puProbeSetupBoxSizer->Fit( m_puProbeSetupPanel );
	mainSizer->Add( m_puProbeSetupPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_ffilterPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* ffilterBoxSizer;
	ffilterBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_ffilterPanel, wxID_ANY, wxT("Following Filter (sine sweep measurements)") ), wxVERTICAL );

	wxFlexGridSizer* ffilterFgSizer;
	ffilterFgSizer = new wxFlexGridSizer( 2, 3, 0, 0 );
	ffilterFgSizer->SetFlexibleDirection( wxBOTH );
	ffilterFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_enableFFilterCheck = new wxCheckBox( ffilterBoxSizer->GetStaticBox(), ID_ENABLE_FFILTER_CHECK, wxT("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	ffilterFgSizer->Add( m_enableFFilterCheck, 0, wxALL, 5 );

	m_emptyLabel = new wxStaticText( ffilterBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_emptyLabel->Wrap( -1 );
	ffilterFgSizer->Add( m_emptyLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_bwLabel = new wxStaticText( ffilterBoxSizer->GetStaticBox(), wxID_ANY, wxT("Bandwidth (Octave)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_bwLabel->Wrap( -1 );
	ffilterFgSizer->Add( m_bwLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_followLeftRadio = new wxRadioButton( ffilterBoxSizer->GetStaticBox(), ID_FOLLOW_LEFT_RADIO, wxT("Follow Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_followLeftRadio->SetValue( true );
	m_followLeftRadio->Enable( false );

	ffilterFgSizer->Add( m_followLeftRadio, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_followRightRadio = new wxRadioButton( ffilterBoxSizer->GetStaticBox(), ID_FOLLOW_RIGHT_RADIO, wxT("Follow Right"), wxDefaultPosition, wxDefaultSize, 0 );
	m_followRightRadio->Enable( false );

	ffilterFgSizer->Add( m_followRightRadio, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_bandwidthText = new wxTextCtrl( ffilterBoxSizer->GetStaticBox(), ID_BANDWIDTH_TEXT, wxT("0.33"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_bandwidthText->Enable( false );
	m_bandwidthText->SetMinSize( wxSize( 120,-1 ) );

	ffilterFgSizer->Add( m_bandwidthText, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	ffilterBoxSizer->Add( ffilterFgSizer, 1, wxEXPAND, 5 );


	m_ffilterPanel->SetSizer( ffilterBoxSizer );
	m_ffilterPanel->Layout();
	ffilterBoxSizer->Fit( m_ffilterPanel );
	mainSizer->Add( m_ffilterPanel, 0, wxALL|wxEXPAND, 5 );

	m_outputSetupPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* outputSetupBoxSizer;
	outputSetupBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_outputSetupPanel, wxID_ANY, wxT("Output Results") ), wxVERTICAL );

	wxFlexGridSizer* outputSetupFgSizer;
	outputSetupFgSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
	outputSetupFgSizer->SetFlexibleDirection( wxBOTH );
	outputSetupFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_normalizeLeftCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_NORMALIZE_LEFT_CHECK, wxT("Normalize"), wxDefaultPosition, wxDefaultSize, 0 );
	m_normalizeLeftCheck->SetValue(true);
	outputSetupFgSizer->Add( m_normalizeLeftCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_hilbertCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_HILBERT_CHECK, wxT("Squared Hilbert Transform"), wxDefaultPosition, wxDefaultSize, 0 );
	outputSetupFgSizer->Add( m_hilbertCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_shiftToHalfCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_SHIFT_TO_HALF_CHECK, wxT("Shift to half window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_shiftToHalfCheck->SetValue(true);
	outputSetupFgSizer->Add( m_shiftToHalfCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_timeReversalCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_TIME_REV_CHECK, wxT("Time reversal"), wxDefaultPosition, wxDefaultSize, 0 );
	outputSetupFgSizer->Add( m_timeReversalCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_coherenceCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_COHERENCE_CHECK, wxT("Coherence weighting"), wxDefaultPosition, wxDefaultSize, 0 );
	outputSetupFgSizer->Add( m_coherenceCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_diracCheck = new wxCheckBox( outputSetupBoxSizer->GetStaticBox(), ID_DIRAC_CHECK, wxT("Dirac Pulse"), wxDefaultPosition, wxDefaultSize, 0 );
	outputSetupFgSizer->Add( m_diracCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* triggerSizer;
	triggerSizer = new wxBoxSizer( wxHORIZONTAL );

	m_triggerLevelLabel = new wxStaticText( outputSetupBoxSizer->GetStaticBox(), wxID_ANY, wxT("Trigger Level [%]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_triggerLevelLabel->Wrap( -1 );
	triggerSizer->Add( m_triggerLevelLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_triggerText = new wxTextCtrl( outputSetupBoxSizer->GetStaticBox(), ID_TRIGGER_TEXT, wxT("50"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_triggerText->SetMinSize( wxSize( 50,-1 ) );

	triggerSizer->Add( m_triggerText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	outputSetupFgSizer->Add( triggerSizer, 1, wxEXPAND, 5 );


	outputSetupBoxSizer->Add( outputSetupFgSizer, 1, wxEXPAND, 5 );


	m_outputSetupPanel->SetSizer( outputSetupBoxSizer );
	m_outputSetupPanel->Layout();
	outputSetupBoxSizer->Fit( m_outputSetupPanel );
	mainSizer->Add( m_outputSetupPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpBtn = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpBtn, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_cancelBtn = new wxButton( m_buttonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_cancelBtn, 0, wxALL, 5 );

	m_okBtn = new wxButton( m_buttonsPanel, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );

	m_okBtn->SetDefault();
	buttonsSizer->Add( m_okBtn, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	m_fftSizeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFftSizeChoice ), NULL, this );
	m_windowChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnWindowChoice ), NULL, this );
	m_xfunctionChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnXFunctionChoice ), NULL, this );
	m_swTubeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnSWTubeCheck ), NULL, this );
	m_freeFieldCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnFreeFieldCheck ), NULL, this );
	m_soundSpeedText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnSoundSpeedText ), NULL, this );
	m_probeDistText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnProbeDistanceText ), NULL, this );
	m_fmaxText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnFmaxText ), NULL, this );
	m_enableFFilterCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnEnableFollowingFilterCheck ), NULL, this );
	m_followLeftRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFollowLeftRadio ), NULL, this );
	m_followRightRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFollowRightRadio ), NULL, this );
	m_bandwidthText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnBandwidthText ), NULL, this );
	m_normalizeLeftCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnNormalizeLeftChannelCheck ), NULL, this );
	m_hilbertCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnHilbertCheck ), NULL, this );
	m_shiftToHalfCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnShiftToHalfWindowCheck ), NULL, this );
	m_timeReversalCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnTimeReversalCheck ), NULL, this );
	m_coherenceCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnCoherenceCheck ), NULL, this );
	m_diracCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnDiracPulseCheck ), NULL, this );
	m_triggerText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnTriggerLevelText ), NULL, this );
	m_helpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnHelp ), NULL, this );
	m_cancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnCancel ), NULL, this );
	m_okBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnOk ), NULL, this );
}

XFunctionsDlg::~XFunctionsDlg()
{
	// Disconnect Events
	m_fftSizeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFftSizeChoice ), NULL, this );
	m_windowChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnWindowChoice ), NULL, this );
	m_xfunctionChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnXFunctionChoice ), NULL, this );
	m_swTubeCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnSWTubeCheck ), NULL, this );
	m_freeFieldCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnFreeFieldCheck ), NULL, this );
	m_soundSpeedText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnSoundSpeedText ), NULL, this );
	m_probeDistText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnProbeDistanceText ), NULL, this );
	m_fmaxText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnFmaxText ), NULL, this );
	m_enableFFilterCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnEnableFollowingFilterCheck ), NULL, this );
	m_followLeftRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFollowLeftRadio ), NULL, this );
	m_followRightRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( XFunctionsDlg::OnFollowRightRadio ), NULL, this );
	m_bandwidthText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnBandwidthText ), NULL, this );
	m_normalizeLeftCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnNormalizeLeftChannelCheck ), NULL, this );
	m_hilbertCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnHilbertCheck ), NULL, this );
	m_shiftToHalfCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnShiftToHalfWindowCheck ), NULL, this );
	m_timeReversalCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnTimeReversalCheck ), NULL, this );
	m_coherenceCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnCoherenceCheck ), NULL, this );
	m_diracCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnDiracPulseCheck ), NULL, this );
	m_triggerText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( XFunctionsDlg::OnTriggerLevelText ), NULL, this );
	m_helpBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnHelp ), NULL, this );
	m_cancelBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnCancel ), NULL, this );
	m_okBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsDlg::OnOk ), NULL, this );

}

XFunctionsShowDlg::XFunctionsShowDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND | wxALL, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_upperPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* upperPanelSizer;
	upperPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* leftSizer;
	leftSizer = new wxBoxSizer( wxVERTICAL );

	m_inputInfoPanel = new wxPanel( m_upperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* inputInfoBoxSizer;
	inputInfoBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_inputInfoPanel, wxID_ANY, wxT("Input Waveform Info") ), wxVERTICAL );

	wxFlexGridSizer* inputInfoFgSizer;
	inputInfoFgSizer = new wxFlexGridSizer( 4, 3, 0, 0 );
	inputInfoFgSizer->SetFlexibleDirection( wxBOTH );
	inputInfoFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_fftSizeLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("FFT Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftSizeLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_fftSizeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fftSizeValueLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), ID_FFT_SIZE_STEXT, wxT("1048576"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftSizeValueLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_fftSizeValueLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_fftSizeUnitLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftSizeUnitLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_fftSizeUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_channelsLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("Channels:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_channelsLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_channelsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_channelsValueLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), ID_CHANNELS_STEXT, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_channelsValueLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_channelsValueLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_channelsUnitLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_channelsUnitLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_channelsUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rateLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("Sample Rate:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rateLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_rateLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rateValueLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), ID_RATE_STEXT, wxT("44100"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rateValueLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_rateValueLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_rateUnitLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rateUnitLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_rateUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_nblocksLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxT("N. of blocks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nblocksLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_nblocksLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_nblocksValueLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), ID_NBLOCKS_STEXT, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nblocksValueLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_nblocksValueLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_nblocksUnitLabel = new wxStaticText( inputInfoBoxSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_nblocksUnitLabel->Wrap( -1 );
	inputInfoFgSizer->Add( m_nblocksUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	inputInfoBoxSizer->Add( inputInfoFgSizer, 1, wxEXPAND, 5 );


	m_inputInfoPanel->SetSizer( inputInfoBoxSizer );
	m_inputInfoPanel->Layout();
	inputInfoBoxSizer->Fit( m_inputInfoPanel );
	leftSizer->Add( m_inputInfoPanel, 0, wxALL|wxEXPAND, 5 );

	m_ttypePanel = new wxPanel( m_upperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* ttypeBoxSizer;
	ttypeBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_ttypePanel, wxID_ANY, wxT("Transform type") ), wxVERTICAL );

	m_transformTypeLabel = new wxStaticText( ttypeBoxSizer->GetStaticBox(), ID_TRANSFORM_TYPE_STEXT, wxT("Very long transform type name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_transformTypeLabel->Wrap( -1 );
	ttypeBoxSizer->Add( m_transformTypeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_ttypePanel->SetSizer( ttypeBoxSizer );
	m_ttypePanel->Layout();
	ttypeBoxSizer->Fit( m_ttypePanel );
	leftSizer->Add( m_ttypePanel, 0, wxEXPAND | wxALL, 5 );

	m_statsPanel = new wxPanel( m_upperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* statsBoxSizer;
	statsBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_statsPanel, wxID_ANY, wxT("Statistics") ), wxVERTICAL );

	wxFlexGridSizer* rmsFgSizer;
	rmsFgSizer = new wxFlexGridSizer( 3, 3, 0, 0 );
	rmsFgSizer->SetFlexibleDirection( wxBOTH );
	rmsFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_rmsLeftLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_LEFT_LABEL_STEXT, wxT("RMS left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsLeftLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsLeftLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsLeftValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_LEFT_STEXT, wxT("-120.00"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_rmsLeftValueLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsLeftValueLabel, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_rmsLeftUnitLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_LEFT_dB_STEXT, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsLeftUnitLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsLeftUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsRightLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_RIGHT_LABEL_STEXT, wxT("RMS right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsRightLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsRightLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsRightValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_RIGHT_STEXT, wxT("-120.00"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_rmsRightValueLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsRightValueLabel, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsRightUnitLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_RIGHT_dB_STEXT, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsRightUnitLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsRightUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsCrossLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_CROSS_LABEL_STEXT, wxT("RMS cross:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsCrossLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsCrossLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rmsCrossValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_CROSS_STEXT, wxT("-120.00"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_rmsCrossValueLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsCrossValueLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_rmsCrossUnitLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_RMS_CROSS_dB_STEXT, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rmsCrossUnitLabel->Wrap( -1 );
	rmsFgSizer->Add( m_rmsCrossUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	statsBoxSizer->Add( rmsFgSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_statsLine = new wxStaticLine( statsBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	statsBoxSizer->Add( m_statsLine, 0, wxEXPAND | wxALL, 5 );

	m_timeOfFlightValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_TIME_OF_FLIGHT_STEXT, wxT("Time of flight:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_timeOfFlightValueLabel->Wrap( -1 );
	m_timeOfFlightValueLabel->Enable( false );

	statsBoxSizer->Add( m_timeOfFlightValueLabel, 0, wxALL, 5 );

	wxFlexGridSizer* tofFgSizer;
	tofFgSizer = new wxFlexGridSizer( 2, 4, 0, 0 );
	tofFgSizer->SetFlexibleDirection( wxBOTH );
	tofFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_triggerTimeLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_TRIGGER_TIME_LABEL_STEXT, wxT("Trigger Time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_triggerTimeLabel->Wrap( -1 );
	m_triggerTimeLabel->Enable( false );

	tofFgSizer->Add( m_triggerTimeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_triggerTimeValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_TRIGGER_TIME_STEXT, wxT("50.0%"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_triggerTimeValueLabel->Wrap( -1 );
	m_triggerTimeValueLabel->Enable( false );

	tofFgSizer->Add( m_triggerTimeValueLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_triggerTimeAtLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_TRIGGER_TIME_AT_STEXT, wxT("at"), wxDefaultPosition, wxDefaultSize, 0 );
	m_triggerTimeAtLabel->Wrap( -1 );
	m_triggerTimeAtLabel->Enable( false );

	tofFgSizer->Add( m_triggerTimeAtLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_triggerDelayValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_TRIGGER_DELAY_STEXT, wxT(" 0.000 s"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_triggerDelayValueLabel->Wrap( -1 );
	m_triggerDelayValueLabel->Enable( false );

	tofFgSizer->Add( m_triggerDelayValueLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_nccMaxLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_NCC_MAX_LABEL_STEXT, wxT("NCC Max.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nccMaxLabel->Wrap( -1 );
	m_nccMaxLabel->Enable( false );

	tofFgSizer->Add( m_nccMaxLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_nccMaxValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_NCC_MAX_STEXT, wxT("0.9999"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_nccMaxValueLabel->Wrap( -1 );
	m_nccMaxValueLabel->Enable( false );

	tofFgSizer->Add( m_nccMaxValueLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_nccMaxAtLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_NCC_MAX_AT_STEXT, wxT("at"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nccMaxAtLabel->Wrap( -1 );
	m_nccMaxAtLabel->Enable( false );

	tofFgSizer->Add( m_nccMaxAtLabel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	m_nccMaxDelayValueLabel = new wxStaticText( statsBoxSizer->GetStaticBox(), ID_NCC_MAX_DELAY_STEXT, wxT("0.000 s"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_nccMaxDelayValueLabel->Wrap( -1 );
	m_nccMaxDelayValueLabel->Enable( false );

	tofFgSizer->Add( m_nccMaxDelayValueLabel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	statsBoxSizer->Add( tofFgSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_saveToTimeDelayCheck = new wxCheckBox( statsBoxSizer->GetStaticBox(), ID_SAVE_TO_TIMEDELAY_CHECK, wxT("Save to timedelay.txt"), wxDefaultPosition, wxDefaultSize, 0 );
	m_saveToTimeDelayCheck->SetValue(true);
	m_saveToTimeDelayCheck->Enable( false );

	statsBoxSizer->Add( m_saveToTimeDelayCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_statsPanel->SetSizer( statsBoxSizer );
	m_statsPanel->Layout();
	statsBoxSizer->Fit( m_statsPanel );
	leftSizer->Add( m_statsPanel, 1, wxALL|wxEXPAND, 5 );

	m_exportPanel = new wxPanel( m_upperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* exportSizer;
	exportSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* exportBoxSizer;
	exportBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_exportPanel, wxID_ANY, wxT("Export spectral data") ), wxVERTICAL );

	m_exportAllCheck = new wxCheckBox( exportBoxSizer->GetStaticBox(), ID_EXPORT_ALL_CHECK, wxT("Dump all spectrums"), wxDefaultPosition, wxDefaultSize, 0 );
	exportBoxSizer->Add( m_exportAllCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* exportButtonsSizer;
	exportButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_saveBtn = new wxButton( exportBoxSizer->GetStaticBox(), ID_SAVE_SPECTRUM_BTN, wxT("Save as..."), wxDefaultPosition, wxDefaultSize, 0 );
	exportButtonsSizer->Add( m_saveBtn, 0, wxALL, 5 );

	m_copyBtn = new wxButton( exportBoxSizer->GetStaticBox(), ID_COPY_SPECTRUM_BTN, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	exportButtonsSizer->Add( m_copyBtn, 0, wxALL, 5 );


	exportBoxSizer->Add( exportButtonsSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	exportSizer->Add( exportBoxSizer, 1, wxEXPAND, 5 );

	m_exportWaveformsCheck = new wxCheckBox( m_exportPanel, ID_EXPORT_WAVEFORMS_CHECK, wxT("Export waveforms to workspace"), wxDefaultPosition, wxDefaultSize, 0 );
	m_exportWaveformsCheck->SetValue(true);
	exportSizer->Add( m_exportWaveformsCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_exportPanel->SetSizer( exportSizer );
	m_exportPanel->Layout();
	exportSizer->Fit( m_exportPanel );
	leftSizer->Add( m_exportPanel, 0, wxALL|wxEXPAND, 5 );


	upperPanelSizer->Add( leftSizer, 0, 0, 5 );

	wxBoxSizer* rightSizer;
	rightSizer = new wxBoxSizer( wxVERTICAL );

	m_rightSideSplitter = new wxSplitterWindow( m_upperPanel, ID_RSIDE_SPLITTER, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_NO_XP_THEME );
	m_rightSideSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( XFunctionsShowDlg::m_rightSideSplitterOnIdle ), NULL, this );

	m_rightUpperPanel = new wxPanel( m_rightSideSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* rightUpperSizer;
	rightUpperSizer = new wxBoxSizer( wxVERTICAL );

	m_plotSplitter = new wxSplitterWindow( m_rightUpperPanel, ID_PLOT_SPLITTER, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_NO_XP_THEME );
	m_plotSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( XFunctionsShowDlg::m_plotSplitterOnIdle ), NULL, this );

	m_plotSplitter->SetMinSize( wxSize( 400,-1 ) );

	m_upperSplitterPanel = new wxPanel( m_plotSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* upperSplitterSizer;
	upperSplitterSizer = new wxBoxSizer( wxVERTICAL );

	m_upperPlotScroller = new wxScrolledWindow( m_upperSplitterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_upperPlotScroller->SetScrollRate( 5, 5 );
	wxBoxSizer* upperPlotSizer;
	upperPlotSizer = new wxBoxSizer( wxVERTICAL );

	m_upperPlot = new Aurora::XFunctionsPlot(m_upperPlotScroller, ID_UPPER_PLOT);
	upperPlotSizer->Add( m_upperPlot, 1, wxALL|wxEXPAND, 5 );


	m_upperPlotScroller->SetSizer( upperPlotSizer );
	m_upperPlotScroller->Layout();
	upperPlotSizer->Fit( m_upperPlotScroller );
	upperSplitterSizer->Add( m_upperPlotScroller, 1, wxALL|wxEXPAND, 5 );


	m_upperSplitterPanel->SetSizer( upperSplitterSizer );
	m_upperSplitterPanel->Layout();
	upperSplitterSizer->Fit( m_upperSplitterPanel );
	m_lowerSplitterPanel = new wxPanel( m_plotSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* lowerSplitterSizer;
	lowerSplitterSizer = new wxBoxSizer( wxVERTICAL );

	m_lowerPlotScroller = new wxScrolledWindow( m_lowerSplitterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_lowerPlotScroller->SetScrollRate( 5, 5 );
	wxBoxSizer* lowerPlotSizer;
	lowerPlotSizer = new wxBoxSizer( wxVERTICAL );

	m_lowerPlot = new Aurora::XFunctionsPlot(m_lowerPlotScroller,  ID_LOWER_PLOT);
	lowerPlotSizer->Add( m_lowerPlot, 1, wxALL|wxEXPAND, 5 );


	m_lowerPlotScroller->SetSizer( lowerPlotSizer );
	m_lowerPlotScroller->Layout();
	lowerPlotSizer->Fit( m_lowerPlotScroller );
	lowerSplitterSizer->Add( m_lowerPlotScroller, 1, wxALL|wxEXPAND, 5 );


	m_lowerSplitterPanel->SetSizer( lowerSplitterSizer );
	m_lowerSplitterPanel->Layout();
	lowerSplitterSizer->Fit( m_lowerSplitterPanel );
	m_plotSplitter->SplitHorizontally( m_upperSplitterPanel, m_lowerSplitterPanel, 0 );
	rightUpperSizer->Add( m_plotSplitter, 1, wxEXPAND, 5 );


	m_rightUpperPanel->SetSizer( rightUpperSizer );
	m_rightUpperPanel->Layout();
	rightUpperSizer->Fit( m_rightUpperPanel );
	m_rightLowerPanel = new wxPanel( m_rightSideSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* plotSettingsBoxSizer;
	plotSettingsBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_rightLowerPanel, wxID_ANY, wxT("Plot settings") ), wxVERTICAL );

	wxBoxSizer* plotSettinsUpperSizer;
	plotSettinsUpperSizer = new wxBoxSizer( wxHORIZONTAL );

	m_upperPlotLabel = new wxStaticText( plotSettingsBoxSizer->GetStaticBox(), ID_FIRST_PLOT_STEXT, wxT("Upper plot:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_upperPlotLabel->Wrap( -1 );
	plotSettinsUpperSizer->Add( m_upperPlotLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_upperPlotChoiceChoices[] = { wxT("Autocorrelation Left"), wxT("Autocorrelation Right"), wxT("Cross-Power Spect. Magn."), wxT("Cross-Power Spect. Phase") };
	int m_upperPlotChoiceNChoices = sizeof( m_upperPlotChoiceChoices ) / sizeof( wxString );
	m_upperPlotChoice = new wxChoice( plotSettingsBoxSizer->GetStaticBox(), ID_FIRST_PLOT_CHOICE, wxDefaultPosition, wxDefaultSize, m_upperPlotChoiceNChoices, m_upperPlotChoiceChoices, 0 );
	m_upperPlotChoice->SetSelection( 0 );
	plotSettinsUpperSizer->Add( m_upperPlotChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lowerPlotLabel = new wxStaticText( plotSettingsBoxSizer->GetStaticBox(), ID_SECOND_PLOT_STEXT, wxT("Lower plot:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lowerPlotLabel->Wrap( -1 );
	plotSettinsUpperSizer->Add( m_lowerPlotLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_lowerPlotChoiceChoices[] = { wxT("Autocorrelation Left"), wxT("Autocorrelation Right"), wxT("Cross-Power Spect. Magn."), wxT("Cross-Power Spect. Phase") };
	int m_lowerPlotChoiceNChoices = sizeof( m_lowerPlotChoiceChoices ) / sizeof( wxString );
	m_lowerPlotChoice = new wxChoice( plotSettingsBoxSizer->GetStaticBox(), ID_SECOND_PLOT_CHOICE, wxDefaultPosition, wxDefaultSize, m_lowerPlotChoiceNChoices, m_lowerPlotChoiceChoices, 0 );
	m_lowerPlotChoice->SetSelection( 1 );
	plotSettinsUpperSizer->Add( m_lowerPlotChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	plotSettingsBoxSizer->Add( plotSettinsUpperSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* plotSettingsLowerSizer;
	plotSettingsLowerSizer = new wxBoxSizer( wxHORIZONTAL );

	m_frangeLabel = new wxStaticText( plotSettingsBoxSizer->GetStaticBox(), wxID_ANY, wxT("Frequency range:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_frangeLabel->Wrap( -1 );
	plotSettingsLowerSizer->Add( m_frangeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_frequencyRangeChoiceChoices[] = { wxT("0 Hz -> 2 kHZ"), wxT("0 Hz -> Nyquist Freq.") };
	int m_frequencyRangeChoiceNChoices = sizeof( m_frequencyRangeChoiceChoices ) / sizeof( wxString );
	m_frequencyRangeChoice = new wxChoice( plotSettingsBoxSizer->GetStaticBox(), ID_FREQUENCY_RANGE_CHOICE, wxDefaultPosition, wxDefaultSize, m_frequencyRangeChoiceNChoices, m_frequencyRangeChoiceChoices, 0 );
	m_frequencyRangeChoice->SetSelection( 1 );
	plotSettingsLowerSizer->Add( m_frequencyRangeChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_linearScaleCheck = new wxCheckBox( plotSettingsBoxSizer->GetStaticBox(), ID_LINEAR_SCALE_CHECK, wxT("Linear Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	plotSettingsLowerSizer->Add( m_linearScaleCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	plotSettingsBoxSizer->Add( plotSettingsLowerSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_rightLowerPanel->SetSizer( plotSettingsBoxSizer );
	m_rightLowerPanel->Layout();
	plotSettingsBoxSizer->Fit( m_rightLowerPanel );
	m_rightSideSplitter->SplitHorizontally( m_rightUpperPanel, m_rightLowerPanel, 420 );
	rightSizer->Add( m_rightSideSplitter, 1, wxEXPAND, 5 );


	upperPanelSizer->Add( rightSizer, 1, wxEXPAND, 5 );


	m_upperPanel->SetSizer( upperPanelSizer );
	m_upperPanel->Layout();
	upperPanelSizer->Fit( m_upperPanel );
	mainSizer->Add( m_upperPanel, 1, wxALL|wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpBtn = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpBtn, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_closeBtn = new wxButton( m_buttonsPanel, wxID_CLOSE, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );

	m_closeBtn->SetDefault();
	buttonsSizer->Add( m_closeBtn, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	m_saveToTimeDelayCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnSaveToTimeDelayCheck ), NULL, this );
	m_exportAllCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnExportAllSpectrumsCheck ), NULL, this );
	m_saveBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnSaveSpectrumBtn ), NULL, this );
	m_copyBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnCopySpectrumBtn ), NULL, this );
	m_exportWaveformsCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnExportWaveformsCheck ), NULL, this );
	m_upperPlotChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnFirstPlotChoice ), NULL, this );
	m_lowerPlotChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnSecondPlotChoice ), NULL, this );
	m_frequencyRangeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnFrequencyRangeChoice ), NULL, this );
	m_linearScaleCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnLinearScaleCheck ), NULL, this );
	m_helpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnHelp ), NULL, this );
	m_closeBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnClose ), NULL, this );
}

XFunctionsShowDlg::~XFunctionsShowDlg()
{
	// Disconnect Events
	m_saveToTimeDelayCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnSaveToTimeDelayCheck ), NULL, this );
	m_exportAllCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnExportAllSpectrumsCheck ), NULL, this );
	m_saveBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnSaveSpectrumBtn ), NULL, this );
	m_copyBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnCopySpectrumBtn ), NULL, this );
	m_exportWaveformsCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnExportWaveformsCheck ), NULL, this );
	m_upperPlotChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnFirstPlotChoice ), NULL, this );
	m_lowerPlotChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnSecondPlotChoice ), NULL, this );
	m_frequencyRangeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( XFunctionsShowDlg::OnFrequencyRangeChoice ), NULL, this );
	m_linearScaleCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnLinearScaleCheck ), NULL, this );
	m_helpBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnHelp ), NULL, this );
	m_closeBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( XFunctionsShowDlg::OnClose ), NULL, this );

}

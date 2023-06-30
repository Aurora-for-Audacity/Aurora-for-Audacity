///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "../src/KirkebyDialogs.h"

///////////////////////////////////////////////////////////////////////////

KirkebyDlg::KirkebyDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_irInfoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* irInfoSizer;
	irInfoSizer = new wxStaticBoxSizer( new wxStaticBox( m_irInfoPanel, wxID_ANY, wxT("Impulse Response info") ), wxVERTICAL );

	m_irInfoLabel = new wxStaticText( irInfoSizer->GetStaticBox(), ID_IR_INFO_STEXT, wxT("48000 Hz / 2 channels / 1000000000 Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_irInfoLabel->Wrap( -1 );
	irInfoSizer->Add( m_irInfoLabel, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxLEFT|wxTOP, 5 );

	wxBoxSizer* rbuttonsSizer;
	rbuttonsSizer = new wxBoxSizer( wxVERTICAL );

	m_monoFilteringButton = new wxRadioButton( irInfoSizer->GetStaticBox(), ID_MONO_FILTERING_RADIO, wxT("Mono (no CrossTalk cancellation)"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_monoFilteringButton->SetValue( true );
	rbuttonsSizer->Add( m_monoFilteringButton, 0, wxALL, 5 );

	m_stereoFilteringButton = new wxRadioButton( irInfoSizer->GetStaticBox(), ID_STEREO_FILTERING_RADIO, wxT("Stereo (with CrossTalk cancellation)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stereoFilteringButton->Enable( false );

	rbuttonsSizer->Add( m_stereoFilteringButton, 0, wxALL, 5 );

	m_dipoleFilteringButton = new wxRadioButton( irInfoSizer->GetStaticBox(), ID_DIPOLE_FILTERING_RADIO, wxT("Dipole (2x2, with CrossTalk cancellation)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_dipoleFilteringButton->Enable( false );

	rbuttonsSizer->Add( m_dipoleFilteringButton, 0, wxALL, 5 );


	irInfoSizer->Add( rbuttonsSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_crossTalkCheck = new wxCheckBox( irInfoSizer->GetStaticBox(), ID_CROSSTALK_CHECK, wxT("CrossTalk cancel only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_crossTalkCheck->Enable( false );

	irInfoSizer->Add( m_crossTalkCheck, 0, wxALL, 5 );

	wxBoxSizer* multiIrSizer;
	multiIrSizer = new wxBoxSizer( wxHORIZONTAL );

	m_multiIRsCheck = new wxCheckBox( irInfoSizer->GetStaticBox(), ID_MULTI_IR_CHECK, wxT("Multiple IRs"), wxDefaultPosition, wxDefaultSize, 0 );
	multiIrSizer->Add( m_multiIRsCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	multiIrSizer->Add( 25, 0, 0, 0, 5 );

	m_multiIRNumberLabel = new wxStaticText( irInfoSizer->GetStaticBox(), ID_IR_NUMBER_LABEL_STEXT, wxT("Number of IRs to invert:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_multiIRNumberLabel->Wrap( -1 );
	m_multiIRNumberLabel->Enable( false );

	multiIrSizer->Add( m_multiIRNumberLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_multiIRText = new wxTextCtrl( irInfoSizer->GetStaticBox(), ID_MULTI_IR_TEXT, wxT("1"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	m_multiIRText->Enable( false );

	multiIrSizer->Add( m_multiIRText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	irInfoSizer->Add( multiIrSizer, 1, wxEXPAND, 5 );


	m_irInfoPanel->SetSizer( irInfoSizer );
	m_irInfoPanel->Layout();
	irInfoSizer->Fit( m_irInfoPanel );
	mainSizer->Add( m_irInfoPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_filterSettingsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* filterSettingsSizer;
	filterSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( m_filterSettingsPanel, wxID_ANY, wxT("Filter Calculation Parameters") ), wxVERTICAL );

	wxFlexGridSizer* settingsTableSizer;
	settingsTableSizer = new wxFlexGridSizer( 8, 3, 0, 0 );
	settingsTableSizer->SetFlexibleDirection( wxBOTH );
	settingsTableSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_flLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Inverse filter length:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_flLabel->Wrap( -1 );
	settingsTableSizer->Add( m_flLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_filterLengthText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_FILTER_LENGTH_TEXT, wxT("100000000"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_filterLengthText, 0, wxALL, 5 );

	m_flUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_flUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_flUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_irpLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("IN-band Regularisation Parameter:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_irpLabel->Wrap( -1 );
	settingsTableSizer->Add( m_irpLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_irpText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_IRP_TEXT, wxT("0.001"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_irpText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_irpEmptyLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_irpEmptyLabel->Wrap( -1 );
	settingsTableSizer->Add( m_irpEmptyLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_orpLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("OUT-band Regularisation Parameter:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_orpLabel->Wrap( -1 );
	settingsTableSizer->Add( m_orpLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_orpText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_ORP_TEXT, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_orpText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_orpEmptyLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_orpEmptyLabel->Wrap( -1 );
	settingsTableSizer->Add( m_orpEmptyLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lcfLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Lower cut frequency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lcfLabel->Wrap( -1 );
	settingsTableSizer->Add( m_lcfLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lowerCutFrqText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_LOWER_CUT_FRQ_TEXT, wxT("80"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_lowerCutFrqText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_lcfUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lcfUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_lcfUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_hcfLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Higher cut frequency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_hcfLabel->Wrap( -1 );
	settingsTableSizer->Add( m_hcfLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_higherCutFrqText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_HIGHER_CUT_FRQ_TEXT, wxT("16000"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_higherCutFrqText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_hcfUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Hz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_hcfUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_hcfUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_twLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Transition width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_twLabel->Wrap( -1 );
	settingsTableSizer->Add( m_twLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_transWidthText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_TRANSITION_WIDTH_TEXT, wxT("0.333"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_transWidthText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_twUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Octaves"), wxDefaultPosition, wxDefaultSize, 0 );
	m_twUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_twUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fiLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Fade-in:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fiLabel->Wrap( -1 );
	settingsTableSizer->Add( m_fiLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fadeInText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_FADE_IN_TEXT, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_fadeInText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fiUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fiUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_fiUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_foLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Fade-out:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_foLabel->Wrap( -1 );
	settingsTableSizer->Add( m_foLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fadeOutText = new wxTextCtrl( filterSettingsSizer->GetStaticBox(), ID_FADE_OUT_TEXT, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	settingsTableSizer->Add( m_fadeOutText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_foUnitsLabel = new wxStaticText( filterSettingsSizer->GetStaticBox(), wxID_ANY, wxT("Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	m_foUnitsLabel->Wrap( -1 );
	settingsTableSizer->Add( m_foUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	filterSettingsSizer->Add( settingsTableSizer, 1, wxEXPAND|wxALL, 5 );


	m_filterSettingsPanel->SetSizer( filterSettingsSizer );
	m_filterSettingsPanel->Layout();
	filterSettingsSizer->Fit( m_filterSettingsPanel );
	mainSizer->Add( m_filterSettingsPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_averagingModePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* averagingModeSizer;
	averagingModeSizer = new wxStaticBoxSizer( new wxStaticBox( m_averagingModePanel, wxID_ANY, wxT("Averaging Mode") ), wxVERTICAL );

	wxString m_averageModeChoiceChoices[] = { wxT("No Average"), wxT("Real, Imaginary"), wxT("Magnitude, Phase"), wxT("Cepstrum") };
	int m_averageModeChoiceNChoices = sizeof( m_averageModeChoiceChoices ) / sizeof( wxString );
	m_averageModeChoice = new wxChoice( averagingModeSizer->GetStaticBox(), ID_AVERAGE_MODE_CHOICE, wxDefaultPosition, wxDefaultSize, m_averageModeChoiceNChoices, m_averageModeChoiceChoices, 0 );
	m_averageModeChoice->SetSelection( 0 );
	averagingModeSizer->Add( m_averageModeChoice, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxFlexGridSizer* averagingModeTable;
	averagingModeTable = new wxFlexGridSizer( 3, 3, 0, 0 );
	averagingModeTable->SetFlexibleDirection( wxBOTH );
	averagingModeTable->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_atLabel = new wxStaticText( averagingModeSizer->GetStaticBox(), ID_AVERAGE_TYPE_LABEL_STEXT, wxT("Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_atLabel->Wrap( -1 );
	m_atLabel->Enable( false );

	averagingModeTable->Add( m_atLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_averageTypeChoiceChoices[] = { wxT("Linear"), wxT("Octaves") };
	int m_averageTypeChoiceNChoices = sizeof( m_averageTypeChoiceChoices ) / sizeof( wxString );
	m_averageTypeChoice = new wxChoice( averagingModeSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_averageTypeChoiceNChoices, m_averageTypeChoiceChoices, 0 );
	m_averageTypeChoice->SetSelection( 0 );
	m_averageTypeChoice->Enable( false );

	averagingModeTable->Add( m_averageTypeChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_atEmptyLabel = new wxStaticText( averagingModeSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_atEmptyLabel->Wrap( -1 );
	averagingModeTable->Add( m_atEmptyLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_awLabel = new wxStaticText( averagingModeSizer->GetStaticBox(), ID_AVERAGE_WIDTH_LABEL_STEXT, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_awLabel->Wrap( -1 );
	m_awLabel->Enable( false );

	averagingModeTable->Add( m_awLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_averageWidthText = new wxTextCtrl( averagingModeSizer->GetStaticBox(), ID_AVERAGE_WIDTH_TEXT, wxT("5.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_averageWidthText->Enable( false );

	averagingModeTable->Add( m_averageWidthText, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_awUnitsLabel = new wxStaticText( averagingModeSizer->GetStaticBox(), ID_AVG_WIDTH_MU_STEXT, wxT("Spectral Points"), wxDefaultPosition, wxDefaultSize, 0 );
	m_awUnitsLabel->Wrap( -1 );
	m_awUnitsLabel->Enable( false );

	averagingModeTable->Add( m_awUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	averagingModeSizer->Add( averagingModeTable, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_averagingModePanel->SetSizer( averagingModeSizer );
	m_averagingModePanel->Layout();
	averagingModeSizer->Fit( m_averagingModePanel );
	mainSizer->Add( m_averagingModePanel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_gainSettingsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* gainSettingsSizer;
	gainSettingsSizer = new wxStaticBoxSizer( new wxStaticBox( m_gainSettingsPanel, wxID_ANY, wxT("Gain Settings") ), wxHORIZONTAL );

	m_autorangeCheck = new wxCheckBox( gainSettingsSizer->GetStaticBox(), ID_AUTORANGE_CHECK, wxT("Autorange"), wxDefaultPosition, wxDefaultSize, 0 );
	m_autorangeCheck->SetValue(true);
	gainSettingsSizer->Add( m_autorangeCheck, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	gainSettingsSizer->Add( 25, 0, 0, 0, 5 );

	m_gainLabel = new wxStaticText( gainSettingsSizer->GetStaticBox(), ID_GAIN_LABEL_STEXT, wxT("Manual Gain:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_gainLabel->Wrap( -1 );
	m_gainLabel->Enable( false );

	gainSettingsSizer->Add( m_gainLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_gainText = new wxTextCtrl( gainSettingsSizer->GetStaticBox(), wxID_ANY, wxT("100.0"), wxDefaultPosition, wxSize( 60,-1 ), wxTE_RIGHT );
	m_gainText->Enable( false );

	gainSettingsSizer->Add( m_gainText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_gainUnitsLabel = new wxStaticText( gainSettingsSizer->GetStaticBox(), ID_GAIN_MU_STEXT, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_gainUnitsLabel->Wrap( -1 );
	m_gainUnitsLabel->Enable( false );

	gainSettingsSizer->Add( m_gainUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_gainSettingsPanel->SetSizer( gainSettingsSizer );
	m_gainSettingsPanel->Layout();
	gainSettingsSizer->Fit( m_gainSettingsPanel );
	mainSizer->Add( m_gainSettingsPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	mHelpBtn = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( mHelpBtn, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	mCancelBtn = new wxButton( m_buttonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( mCancelBtn, 0, wxALL, 5 );

	mOkBtn = new wxButton( m_buttonsPanel, wxID_OK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );

	mOkBtn->SetDefault();
	buttonsSizer->Add( mOkBtn, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxEXPAND | wxALL, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	m_monoFilteringButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnMonoFilteringRadio ), NULL, this );
	m_stereoFilteringButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnStereoFilteringRadio ), NULL, this );
	m_dipoleFilteringButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnDipoleFilteringRadio ), NULL, this );
	m_crossTalkCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnCrossTalkCheck ), NULL, this );
	m_multiIRsCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnMultiIRCheck ), NULL, this );
	m_multiIRText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnMultiIrText ), NULL, this );
	m_irpText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnIRPText ), NULL, this );
	m_orpText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnORPText ), NULL, this );
	m_lowerCutFrqText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnLowerCutFrqText ), NULL, this );
	m_higherCutFrqText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnHigherCutFrqText ), NULL, this );
	m_transWidthText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnTransitionWidthText ), NULL, this );
	m_fadeInText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnFadeInText ), NULL, this );
	m_fadeOutText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnFadeOutText ), NULL, this );
	m_averageModeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( KirkebyDlg::OnAverageModeChoice ), NULL, this );
	m_averageTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( KirkebyDlg::OnAverageTypeChoice ), NULL, this );
	m_averageWidthText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnAverageWidthText ), NULL, this );
	m_autorangeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnAutorangeCheck ), NULL, this );
	m_gainText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnGainText ), NULL, this );
	mHelpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnHelp ), NULL, this );
	mCancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnCancel ), NULL, this );
	mOkBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnOk ), NULL, this );
}

KirkebyDlg::~KirkebyDlg()
{
	// Disconnect Events
	m_monoFilteringButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnMonoFilteringRadio ), NULL, this );
	m_stereoFilteringButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnStereoFilteringRadio ), NULL, this );
	m_dipoleFilteringButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( KirkebyDlg::OnDipoleFilteringRadio ), NULL, this );
	m_crossTalkCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnCrossTalkCheck ), NULL, this );
	m_multiIRsCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnMultiIRCheck ), NULL, this );
	m_multiIRText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnMultiIrText ), NULL, this );
	m_irpText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnIRPText ), NULL, this );
	m_orpText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnORPText ), NULL, this );
	m_lowerCutFrqText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnLowerCutFrqText ), NULL, this );
	m_higherCutFrqText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnHigherCutFrqText ), NULL, this );
	m_transWidthText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnTransitionWidthText ), NULL, this );
	m_fadeInText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnFadeInText ), NULL, this );
	m_fadeOutText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnFadeOutText ), NULL, this );
	m_averageModeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( KirkebyDlg::OnAverageModeChoice ), NULL, this );
	m_averageTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( KirkebyDlg::OnAverageTypeChoice ), NULL, this );
	m_averageWidthText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnAverageWidthText ), NULL, this );
	m_autorangeCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( KirkebyDlg::OnAutorangeCheck ), NULL, this );
	m_gainText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( KirkebyDlg::OnGainText ), NULL, this );
	mHelpBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnHelp ), NULL, this );
	mCancelBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnCancel ), NULL, this );
	mOkBtn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( KirkebyDlg::OnOk ), NULL, this );

}

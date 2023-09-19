///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "AcParametersDialogs.h"

///////////////////////////////////////////////////////////////////////////

AcParDlg::AcParDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,500 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_pLogoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_pLogoPanel, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_pTopStaticline = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_pTopStaticline, 0, wxEXPAND | wxALL, 5 );

	wxBoxSizer* midHSizer;
	midHSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* leftVSizer;
	leftVSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* optionsBSizer;
	optionsBSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Common options") ), wxVERTICAL );

	m_pAnalysisPanel = new wxPanel( optionsBSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* analysisBSizer;
	analysisBSizer = new wxStaticBoxSizer( new wxStaticBox( m_pAnalysisPanel, wxID_ANY, wxT("Analysis") ), wxHORIZONTAL );

	m_pOctaveRbutton = new wxRadioButton( analysisBSizer->GetStaticBox(), ID_OCTAVE_RADIO, wxT("Octave Bands"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_pOctaveRbutton->SetValue( true );
	analysisBSizer->Add( m_pOctaveRbutton, 0, wxALL, 5 );

	m_pThirdOctaveRbutton = new wxRadioButton( analysisBSizer->GetStaticBox(), ID_ONE_THIRD_OCTAVE_RADIO, wxT("One-Third Octave Bands"), wxDefaultPosition, wxDefaultSize, 0 );
	analysisBSizer->Add( m_pThirdOctaveRbutton, 0, wxALL, 5 );


	m_pAnalysisPanel->SetSizer( analysisBSizer );
	m_pAnalysisPanel->Layout();
	analysisBSizer->Fit( m_pAnalysisPanel );
	optionsBSizer->Add( m_pAnalysisPanel, 0, wxALL|wxEXPAND, 5 );

	m_pExtrPanel = new wxPanel( optionsBSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* extremesBSizer;
	extremesBSizer = new wxStaticBoxSizer( new wxStaticBox( m_pExtrPanel, wxID_ANY, wxT("User Defined Reverberation Time Extremes") ), wxVERTICAL );

	extremesBSizer->SetMinSize( wxSize( 290,-1 ) );
	wxBoxSizer* extremesHSizer;
	extremesHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pLeftParenthesisLabel = new wxStaticText( extremesBSizer->GetStaticBox(), wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLeftParenthesisLabel->Wrap( -1 );
	extremesHSizer->Add( m_pLeftParenthesisLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pUserMinLevelText = new wxTextCtrl( extremesBSizer->GetStaticBox(), ID_RTE_1_TEXT, wxT("-5.0"), wxDefaultPosition, wxSize( 60,-1 ), wxTE_RIGHT );
	extremesHSizer->Add( m_pUserMinLevelText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMaxUnitLabel = new wxStaticText( extremesBSizer->GetStaticBox(), wxID_ANY, wxT("dB, "), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMaxUnitLabel->Wrap( -1 );
	extremesHSizer->Add( m_pMaxUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pUserMaxLevelText = new wxTextCtrl( extremesBSizer->GetStaticBox(), ID_RTE_2_TEXT, wxT("-15.0"), wxDefaultPosition, wxSize( 60,-1 ), wxTE_RIGHT );
	extremesHSizer->Add( m_pUserMaxLevelText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMinUnitLabel = new wxStaticText( extremesBSizer->GetStaticBox(), wxID_ANY, wxT("dB )"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMinUnitLabel->Wrap( -1 );
	extremesHSizer->Add( m_pMinUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	extremesBSizer->Add( extremesHSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_pExtrPanel->SetSizer( extremesBSizer );
	m_pExtrPanel->Layout();
	extremesBSizer->Fit( m_pExtrPanel );
	optionsBSizer->Add( m_pExtrPanel, 0, wxALL|wxEXPAND, 5 );

	m_pChecksPanel = new wxPanel( optionsBSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* checkBoxesVSizer;
	checkBoxesVSizer = new wxBoxSizer( wxVERTICAL );

	m_pNoiseCorrectionCheck = new wxCheckBox( m_pChecksPanel, ID_ENC_CHECKBOX, wxT("Enable Noise Correction"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	checkBoxesVSizer->Add( m_pNoiseCorrectionCheck, 0, wxALL, 5 );

	m_pEDTCheck = new wxCheckBox( m_pChecksPanel, ID_EDT_CHECKBOX, wxT("EDT without linear regression"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	checkBoxesVSizer->Add( m_pEDTCheck, 0, wxALL, 5 );

	m_pStageCheck = new wxCheckBox( m_pChecksPanel, ID_ST_CHECKBOX, wxT("Compute Stage Parameters (ST)"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	checkBoxesVSizer->Add( m_pStageCheck, 0, wxALL, 5 );

	m_pAvgModeCheck = new wxCheckBox( m_pChecksPanel, ID_AVG_CHECKBOX, wxT("Average Mode instead of A-weight"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	checkBoxesVSizer->Add( m_pAvgModeCheck, 0, wxALL, 5 );


	m_pChecksPanel->SetSizer( checkBoxesVSizer );
	m_pChecksPanel->Layout();
	checkBoxesVSizer->Fit( m_pChecksPanel );
	optionsBSizer->Add( m_pChecksPanel, 0, wxALL|wxEXPAND, 5 );

	m_pLimitsPanel = new wxPanel( optionsBSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* limitsFgSizer;
	limitsFgSizer = new wxFlexGridSizer( 2, 3, 0, 0 );
	limitsFgSizer->SetFlexibleDirection( wxBOTH );
	limitsFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_pDstLabel = new wxStaticText( m_pLimitsPanel, wxID_ANY, wxT("Direct Sound Trigger (below FS):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pDstLabel->Wrap( -1 );
	limitsFgSizer->Add( m_pDstLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pDstText = new wxTextCtrl( m_pLimitsPanel, ID_DST_TEXT, wxT("20.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	limitsFgSizer->Add( m_pDstText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pDstUnitLabel = new wxStaticText( m_pLimitsPanel, wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pDstUnitLabel->Wrap( -1 );
	limitsFgSizer->Add( m_pDstUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pFullScaleLabel = new wxStaticText( m_pLimitsPanel, wxID_ANY, wxT("Peak SPL value (FullScale):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pFullScaleLabel->Wrap( -1 );
	limitsFgSizer->Add( m_pFullScaleLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pFullScaleText = new wxTextCtrl( m_pLimitsPanel, ID_PEAK_SPL_TEXT, wxT("120.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	limitsFgSizer->Add( m_pFullScaleText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pFullScaleUnitLabel = new wxStaticText( m_pLimitsPanel, wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pFullScaleUnitLabel->Wrap( -1 );
	limitsFgSizer->Add( m_pFullScaleUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_pLimitsPanel->SetSizer( limitsFgSizer );
	m_pLimitsPanel->Layout();
	limitsFgSizer->Fit( m_pLimitsPanel );
	optionsBSizer->Add( m_pLimitsPanel, 1, wxEXPAND | wxALL, 5 );


	leftVSizer->Add( optionsBSizer, 1, wxEXPAND|wxALL, 5 );


	midHSizer->Add( leftVSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* rightVSizer;
	rightVSizer = new wxBoxSizer( wxVERTICAL );

	m_pTwoChnlsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* twoChannelBSizer;
	twoChannelBSizer = new wxStaticBoxSizer( new wxStaticBox( m_pTwoChnlsPanel, wxID_ANY, wxT("Two Channels Mode") ), wxVERTICAL );

	m_pTwoOmniRadio = new wxRadioButton( twoChannelBSizer->GetStaticBox(), ID_2CH_MODE_OMNI_RADIO, wxT("2 Omidirectional Microphones"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	m_pTwoOmniRadio->SetValue( true );
	m_pTwoOmniRadio->Enable( false );

	twoChannelBSizer->Add( m_pTwoOmniRadio, 0, wxALL, 5 );

	m_pAmbisonicsWYRbutton = new wxRadioButton( twoChannelBSizer->GetStaticBox(), ID_2CH_MODE_SOUNDFIELD_RADIO, wxT("Ambisonics Microphone (WY only)"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	m_pAmbisonicsWYRbutton->Enable( false );

	twoChannelBSizer->Add( m_pAmbisonicsWYRbutton, 0, wxALL, 5 );

	m_pPUProbeRbutton = new wxRadioButton( twoChannelBSizer->GetStaticBox(), ID_2CH_MODE_PU_PROBE_RADIO, wxT("Omni/Eight microphone - PU probe"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	m_pPUProbeRbutton->Enable( false );

	twoChannelBSizer->Add( m_pPUProbeRbutton, 0, wxALL, 5 );

	m_pIntensityProbeRbutton = new wxRadioButton( twoChannelBSizer->GetStaticBox(), ID_2CH_MODE_INTENSITY_PROBE_RADIO, wxT("P-P Sound Intensity Probe"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	m_pIntensityProbeRbutton->Enable( false );

	twoChannelBSizer->Add( m_pIntensityProbeRbutton, 0, wxALL, 5 );

	wxBoxSizer* intensityProbeHSizer;
	intensityProbeHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pProbeMicsDistLabel = new wxStaticText( twoChannelBSizer->GetStaticBox(), ID_PROBE_MICS_DIST_STEXT, wxT("d:"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pProbeMicsDistLabel->Wrap( -1 );
	m_pProbeMicsDistLabel->Enable( false );

	intensityProbeHSizer->Add( m_pProbeMicsDistLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pProbeMicsDistText = new wxTextCtrl( twoChannelBSizer->GetStaticBox(), ID_PP_DIST_TEXT, wxT("12.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	m_pProbeMicsDistText->Enable( false );
	m_pProbeMicsDistText->SetToolTip( wxT("Distance in millimetres between Sound Intensity Probe microphones.") );

	intensityProbeHSizer->Add( m_pProbeMicsDistText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pProbeDistUnitLabel = new wxStaticText( twoChannelBSizer->GetStaticBox(), wxID_ANY, wxT("mm,"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pProbeDistUnitLabel->Wrap( -1 );
	m_pProbeDistUnitLabel->Enable( false );

	intensityProbeHSizer->Add( m_pProbeDistUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pSoundSpeedLabel = new wxStaticText( twoChannelBSizer->GetStaticBox(), ID_SOUND_SPEED_STEXT, wxT("c:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSoundSpeedLabel->Wrap( -1 );
	m_pSoundSpeedLabel->Enable( false );

	intensityProbeHSizer->Add( m_pSoundSpeedLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pSoundSpeedText = new wxTextCtrl( twoChannelBSizer->GetStaticBox(), ID_C_TEXT, wxT("340.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	m_pSoundSpeedText->Enable( false );
	m_pSoundSpeedText->SetToolTip( wxT("Sound speed in m/s.") );

	intensityProbeHSizer->Add( m_pSoundSpeedText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pSoundSpeedUnitLabel = new wxStaticText( twoChannelBSizer->GetStaticBox(), wxID_ANY, wxT("m/s."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pSoundSpeedUnitLabel->Wrap( -1 );
	m_pSoundSpeedUnitLabel->Enable( false );

	intensityProbeHSizer->Add( m_pSoundSpeedUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	twoChannelBSizer->Add( intensityProbeHSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_pBinauralRbutton = new wxRadioButton( twoChannelBSizer->GetStaticBox(), ID_2CH_MODE_BINAURAL_RADIO, wxT("Binaural Dummy Head"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBinauralRbutton->Enable( false );

	twoChannelBSizer->Add( m_pBinauralRbutton, 0, wxALL, 5 );

	wxBoxSizer* iaccHSizer;
	iaccHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pIaccLabel = new wxStaticText( twoChannelBSizer->GetStaticBox(), ID_IACC_STEXT, wxT("IACC integration"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pIaccLabel->Wrap( -1 );
	m_pIaccLabel->Enable( false );

	iaccHSizer->Add( m_pIaccLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_pIaccChoiceChoices[] = { wxT("0-80 ms (Early)"), wxT("80-oo ms (Late)"), wxT("0-oo ms (All)") };
	int m_pIaccChoiceNChoices = sizeof( m_pIaccChoiceChoices ) / sizeof( wxString );
	m_pIaccChoice = new wxChoice( twoChannelBSizer->GetStaticBox(), ID_IACC_CHOICE, wxDefaultPosition, wxDefaultSize, m_pIaccChoiceNChoices, m_pIaccChoiceChoices, 0 );
	m_pIaccChoice->SetSelection( 2 );
	m_pIaccChoice->Enable( false );

	iaccHSizer->Add( m_pIaccChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	twoChannelBSizer->Add( iaccHSizer, 0, 0, 5 );


	m_pTwoChnlsPanel->SetSizer( twoChannelBSizer );
	m_pTwoChnlsPanel->Layout();
	twoChannelBSizer->Fit( m_pTwoChnlsPanel );
	rightVSizer->Add( m_pTwoChnlsPanel, 0, wxALL|wxEXPAND, 5 );

	m_pFourChnlsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* fourChnlsBSizer;
	fourChnlsBSizer = new wxStaticBoxSizer( new wxStaticBox( m_pFourChnlsPanel, wxID_ANY, wxT("Four Channels Mode") ), wxVERTICAL );

	m_pFourOmniRbutton = new wxRadioButton( fourChnlsBSizer->GetStaticBox(), ID_4CH_MODE_OMNI_RADIO, wxT("4 Omnidirectional Microphones"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_pFourOmniRbutton->SetValue( true );
	m_pFourOmniRbutton->Enable( false );

	fourChnlsBSizer->Add( m_pFourOmniRbutton, 0, wxALL, 5 );

	m_pAmbisonicsLabel = new wxStaticText( fourChnlsBSizer->GetStaticBox(), ID_4CH_SOUNDFIELD_STEXT, wxT("Ambisonics Microphone:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pAmbisonicsLabel->Wrap( -1 );
	m_pAmbisonicsLabel->Enable( false );

	fourChnlsBSizer->Add( m_pAmbisonicsLabel, 0, wxALL, 5 );

	wxBoxSizer* ambisonicsHSizer;
	ambisonicsHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pFuMaRbutton = new wxRadioButton( fourChnlsBSizer->GetStaticBox(), ID_4CH_MODE_SOUNDFIELD_XWYZ_RADIO, wxT("FuMa - WX YZ"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pFuMaRbutton->Enable( false );

	ambisonicsHSizer->Add( m_pFuMaRbutton, 0, wxALL, 5 );

	m_pAmbixRbutton = new wxRadioButton( fourChnlsBSizer->GetStaticBox(), ID_4CH_MODE_SOUNDFIELD_WYXZ_RADIO, wxT("Ambix - WY ZX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pAmbixRbutton->Enable( false );

	ambisonicsHSizer->Add( m_pAmbixRbutton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	fourChnlsBSizer->Add( ambisonicsHSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_pFourChnlsPanel->SetSizer( fourChnlsBSizer );
	m_pFourChnlsPanel->Layout();
	fourChnlsBSizer->Fit( m_pFourChnlsPanel );
	rightVSizer->Add( m_pFourChnlsPanel, 0, wxALL|wxEXPAND, 5 );


	midHSizer->Add( rightVSizer, 1, wxEXPAND, 5 );


	mainSizer->Add( midHSizer, 1, wxEXPAND, 5 );

	m_pAppendPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* appendVSizer;
	appendVSizer = new wxBoxSizer( wxVERTICAL );

	m_pAppendResultsCheck = new wxCheckBox( m_pAppendPanel, ID_APP_RES_CHECKBOX, wxT("Append Results to File:"), wxDefaultPosition, wxSize( -1,20 ), 0 );
	appendVSizer->Add( m_pAppendResultsCheck, 0, wxALL, 5 );

	wxBoxSizer* filenameHSizer;
	filenameHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pFileNameText = new wxTextCtrl( m_pAppendPanel, ID_FILE_NAME_TEXT, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_pFileNameText->Enable( false );

	filenameHSizer->Add( m_pFileNameText, 1, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pOpenFileButton = new wxButton( m_pAppendPanel, ID_OPEN_FILE_BTN, wxT("..."), wxDefaultPosition, wxSize( 30,25 ), 0 );
	m_pOpenFileButton->Enable( false );

	filenameHSizer->Add( m_pOpenFileButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	appendVSizer->Add( filenameHSizer, 1, wxEXPAND, 5 );


	m_pAppendPanel->SetSizer( appendVSizer );
	m_pAppendPanel->Layout();
	appendVSizer->Fit( m_pAppendPanel );
	mainSizer->Add( m_pAppendPanel, 0, wxALL|wxEXPAND, 5 );

	m_pBottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_pBottomLine, 0, wxEXPAND | wxALL, 5 );

	m_pButtonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsHSizer;
	buttonsHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pHelpButton = new wxButton( m_pButtonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsHSizer->Add( m_pHelpButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	buttonsHSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_pCancelButton = new wxButton( m_pButtonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	buttonsHSizer->Add( m_pCancelButton, 0, wxALL, 5 );

	m_pOkButton = new wxButton( m_pButtonsPanel, wxID_OK, wxT("Calculate!"), wxDefaultPosition, wxSize( -1,-1 ), 0 );

	m_pOkButton->SetDefault();
	buttonsHSizer->Add( m_pOkButton, 0, wxALL, 5 );


	m_pButtonsPanel->SetSizer( buttonsHSizer );
	m_pButtonsPanel->Layout();
	buttonsHSizer->Fit( m_pButtonsPanel );
	mainSizer->Add( m_pButtonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AcParDlg::OnCloseDialog ) );
	m_pOctaveRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnOctaveBandsRadio ), NULL, this );
	m_pThirdOctaveRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnThirdOctaveBandsRadio ), NULL, this );
	m_pUserMinLevelText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditRevTimeMinExtreme ), NULL, this );
	m_pUserMaxLevelText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditRevTimeMaxExtreme ), NULL, this );
	m_pNoiseCorrectionCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnNoiseCorrCheck ), NULL, this );
	m_pEDTCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnEDTCheck ), NULL, this );
	m_pStageCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnComputeStageParamCheck ), NULL, this );
	m_pAvgModeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnAvgModeCheck ), NULL, this );
	m_pDstText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditDirSoundTrigValue ), NULL, this );
	m_pFullScaleText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditPeakSPLValue ), NULL, this );
	m_pTwoOmniRadio->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnOmniMicClick ), NULL, this );
	m_pAmbisonicsWYRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnSoundfieldClick ), NULL, this );
	m_pPUProbeRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnPUProbeClick ), NULL, this );
	m_pIntensityProbeRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnIntensityProbeClick ), NULL, this );
	m_pProbeMicsDistText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditProbeMicDist ), NULL, this );
	m_pSoundSpeedText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditCValue ), NULL, this );
	m_pBinauralRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnBinauralClick ), NULL, this );
	m_pIaccChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AcParDlg::OnIACCChoice ), NULL, this );
	m_pFourOmniRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeOmniRadio ), NULL, this );
	m_pFuMaRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeSoundfieldWXYZRadio ), NULL, this );
	m_pAmbixRbutton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeSoundfieldWYXZRadio ), NULL, this );
	m_pAppendResultsCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnAppendResultsToFile ), NULL, this );
	m_pFileNameText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditAppendFileName ), NULL, this );
	m_pOpenFileButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnOpenFile ), NULL, this );
	m_pHelpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnHelp ), NULL, this );
	m_pCancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnCancel ), NULL, this );
	m_pOkButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnOk ), NULL, this );
}

AcParDlg::~AcParDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AcParDlg::OnCloseDialog ) );
	m_pOctaveRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnOctaveBandsRadio ), NULL, this );
	m_pThirdOctaveRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnThirdOctaveBandsRadio ), NULL, this );
	m_pUserMinLevelText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditRevTimeMinExtreme ), NULL, this );
	m_pUserMaxLevelText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditRevTimeMaxExtreme ), NULL, this );
	m_pNoiseCorrectionCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnNoiseCorrCheck ), NULL, this );
	m_pEDTCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnEDTCheck ), NULL, this );
	m_pStageCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnComputeStageParamCheck ), NULL, this );
	m_pAvgModeCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnAvgModeCheck ), NULL, this );
	m_pDstText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditDirSoundTrigValue ), NULL, this );
	m_pFullScaleText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditPeakSPLValue ), NULL, this );
	m_pTwoOmniRadio->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnOmniMicClick ), NULL, this );
	m_pAmbisonicsWYRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnSoundfieldClick ), NULL, this );
	m_pPUProbeRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnPUProbeClick ), NULL, this );
	m_pIntensityProbeRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnIntensityProbeClick ), NULL, this );
	m_pProbeMicsDistText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditProbeMicDist ), NULL, this );
	m_pSoundSpeedText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditCValue ), NULL, this );
	m_pBinauralRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::OnBinauralClick ), NULL, this );
	m_pIaccChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AcParDlg::OnIACCChoice ), NULL, this );
	m_pFourOmniRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeOmniRadio ), NULL, this );
	m_pFuMaRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeSoundfieldWXYZRadio ), NULL, this );
	m_pAmbixRbutton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( AcParDlg::On4ChModeSoundfieldWYXZRadio ), NULL, this );
	m_pAppendResultsCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AcParDlg::OnAppendResultsToFile ), NULL, this );
	m_pFileNameText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AcParDlg::OnEditAppendFileName ), NULL, this );
	m_pOpenFileButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnOpenFile ), NULL, this );
	m_pHelpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnHelp ), NULL, this );
	m_pCancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnCancel ), NULL, this );
	m_pOkButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParDlg::OnOk ), NULL, this );

}

GStoreDlg::GStoreDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_pUpperPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* upperVSizer;
	upperVSizer = new wxBoxSizer( wxVERTICAL );

	m_pTopExplicationLabel = new wxStaticText( m_pUpperPanel, wxID_ANY, wxT("You have requested to store the actual Signal values as reference for the strenGth computation. This means that this impulse response is perfectly anechoic, and it was measured at a known distance from the sound source (assumed omnidirectional)"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_pTopExplicationLabel->Wrap( 400 );
	upperVSizer->Add( m_pTopExplicationLabel, 0, wxALL, 5 );

	m_pBottomExplicationLabel = new wxStaticText( m_pUpperPanel, wxID_ANY, wxT("If you want to proceed, please insert the exact distance (in m) and press Store. If you press Cancel, the previous reference values will be left unchanged."), wxDefaultPosition, wxDefaultSize, 0 );
	m_pBottomExplicationLabel->Wrap( 400 );
	upperVSizer->Add( m_pBottomExplicationLabel, 0, wxALL, 5 );


	m_pUpperPanel->SetSizer( upperVSizer );
	m_pUpperPanel->Layout();
	upperVSizer->Fit( m_pUpperPanel );
	mainSizer->Add( m_pUpperPanel, 0, wxALL, 5 );

	m_pMidPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* midHSizer;
	midHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pDistanceLabel = new wxStaticText( m_pMidPanel, wxID_ANY, wxT("Distance from the sound source (m):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pDistanceLabel->Wrap( -1 );
	midHSizer->Add( m_pDistanceLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pDistanceText = new wxTextCtrl( m_pMidPanel, ID_G_DIST_TEXT, wxEmptyString, wxDefaultPosition, wxSize( -1,20 ), 0 );
	midHSizer->Add( m_pDistanceText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_pMidPanel->SetSizer( midHSizer );
	m_pMidPanel->Layout();
	midHSizer->Fit( m_pMidPanel );
	mainSizer->Add( m_pMidPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_pButtonsLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_pButtonsLine, 0, wxEXPAND | wxALL, 5 );

	m_pButtonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsHSizer;
	buttonsHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pRestoreButton = new wxButton( m_pButtonsPanel, ID_RESTORE_BTN, wxT("Restore defaults"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsHSizer->Add( m_pRestoreButton, 0, wxALL, 5 );


	buttonsHSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_pCancelButton = new wxButton( m_pButtonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsHSizer->Add( m_pCancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_pStoreButton = new wxButton( m_pButtonsPanel, wxID_OK, wxT("Store"), wxDefaultPosition, wxDefaultSize, 0 );

	m_pStoreButton->SetDefault();
	buttonsHSizer->Add( m_pStoreButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_pButtonsPanel->SetSizer( buttonsHSizer );
	m_pButtonsPanel->Layout();
	buttonsHSizer->Fit( m_pButtonsPanel );
	mainSizer->Add( m_pButtonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GStoreDlg::OnCloseDialog ) );
	m_pDistanceText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GStoreDlg::OnEditDistanceField ), NULL, this );
	m_pRestoreButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnRestoreDefaultValues ), NULL, this );
	m_pCancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnCancel ), NULL, this );
	m_pStoreButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnOK ), NULL, this );
}

GStoreDlg::~GStoreDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GStoreDlg::OnCloseDialog ) );
	m_pDistanceText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( GStoreDlg::OnEditDistanceField ), NULL, this );
	m_pRestoreButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnRestoreDefaultValues ), NULL, this );
	m_pCancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnCancel ), NULL, this );
	m_pStoreButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GStoreDlg::OnOK ), NULL, this );

}

AcParShowDlg::AcParShowDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_pLogoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_pLogoPanel, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_pTopLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_pTopLine, 0, wxEXPAND | wxALL, 5 );

	m_pHorzSplitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_pHorzSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( AcParShowDlg::m_pHorzSplitterOnIdle ), NULL, this );

	m_pUpperPanel = new wxScrolledWindow( m_pHorzSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_pUpperPanel->SetScrollRate( 5, 5 );
	wxBoxSizer* upperHSizer;
	upperHSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* plotBSizer;
	plotBSizer = new wxStaticBoxSizer( new wxStaticBox( m_pUpperPanel, ID_PLOT_SBOX, wxT("Unfiltered Impulse Response") ), wxVERTICAL );

	m_pPlotPanel = new wxPanel( plotBSizer->GetStaticBox(), ID_PLOT_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* plotVSizer;
	plotVSizer = new wxBoxSizer( wxVERTICAL );

	m_pPlot = new Aurora::AcParametersDecayPlot(m_pPlotPanel, ID_PLOT, wxDefaultPosition, wxDefaultSize);
	m_pPlot->SetMinSize( wxSize( -1,250 ) );

	plotVSizer->Add( m_pPlot, 1, wxALL|wxEXPAND, 5 );


	m_pPlotPanel->SetSizer( plotVSizer );
	m_pPlotPanel->Layout();
	plotVSizer->Fit( m_pPlotPanel );
	plotBSizer->Add( m_pPlotPanel, 1, wxEXPAND | wxALL, 5 );


	upperHSizer->Add( plotBSizer, 1, wxEXPAND|wxALL, 5 );

	m_topRightPanel = new wxPanel( m_pUpperPanel, ID_BUTTONS_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* controlsVSizer;
	controlsVSizer = new wxBoxSizer( wxVERTICAL );

	m_pSetupButton = new wxButton( m_topRightPanel, ID_SETUP_BTN, wxT("Setup"), wxDefaultPosition, wxDefaultSize, 0 );

	m_pSetupButton->SetDefault();
	controlsVSizer->Add( m_pSetupButton, 0, wxALL|wxEXPAND, 5 );

	m_pSaveButton = new wxButton( m_topRightPanel, wxID_SAVE, wxT("Save Results to File..."), wxDefaultPosition, wxDefaultSize, 0 );
	controlsVSizer->Add( m_pSaveButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_pCopyButton = new wxButton( m_topRightPanel, wxID_COPY, wxT("Copy Results to Clipboard"), wxDefaultPosition, wxDefaultSize, 0 );
	controlsVSizer->Add( m_pCopyButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_pStoreGButton = new wxButton( m_topRightPanel, ID_STORE_G_BTN, wxT("Store G Reference Signal"), wxDefaultPosition, wxDefaultSize, 0 );
	controlsVSizer->Add( m_pStoreGButton, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* micTypeBSizer;
	micTypeBSizer = new wxStaticBoxSizer( new wxStaticBox( m_topRightPanel, wxID_ANY, wxT("Mic/probe type:") ), wxVERTICAL );

	m_pSelectedMicLabel = new wxStaticText( micTypeBSizer->GetStaticBox(), ID_MIC_TYPE_ST, wxT("Omnidirectional mic(s) "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_pSelectedMicLabel->Wrap( -1 );
	micTypeBSizer->Add( m_pSelectedMicLabel, 0, wxALL, 5 );


	controlsVSizer->Add( micTypeBSizer, 0, wxEXPAND|wxALL, 5 );

	wxStaticBoxSizer* trackListBSizer;
	trackListBSizer = new wxStaticBoxSizer( new wxStaticBox( m_topRightPanel, wxID_ANY, wxT("Channels:") ), wxVERTICAL );

	m_pTracksList = new wxListBox( trackListBSizer->GetStaticBox(), ID_CHANNELS_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_pTracksList->SetMinSize( wxSize( -1,90 ) );

	trackListBSizer->Add( m_pTracksList, 1, wxALL|wxEXPAND, 5 );


	controlsVSizer->Add( trackListBSizer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* tuserBSizer;
	tuserBSizer = new wxStaticBoxSizer( new wxStaticBox( m_topRightPanel, wxID_ANY, wxT("Tuser limits:") ), wxVERTICAL );

	wxBoxSizer* tuserHSizer;
	tuserHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pLeftParenthesisLabel = new wxStaticText( tuserBSizer->GetStaticBox(), wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0 );
	m_pLeftParenthesisLabel->Wrap( -1 );
	tuserHSizer->Add( m_pLeftParenthesisLabel, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMaxUserLabel = new wxStaticText( tuserBSizer->GetStaticBox(), ID_UP_TUSER_LIMIT_STEXT, wxT("-999.9"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMaxUserLabel->Wrap( -1 );
	tuserHSizer->Add( m_pMaxUserLabel, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMaxUnitLabel = new wxStaticText( tuserBSizer->GetStaticBox(), wxID_ANY, wxT(" dB, "), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMaxUnitLabel->Wrap( -1 );
	tuserHSizer->Add( m_pMaxUnitLabel, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMinUserLabel = new wxStaticText( tuserBSizer->GetStaticBox(), ID_LOW_TUSER_LIMIT_STEXT, wxT("-999.9"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMinUserLabel->Wrap( -1 );
	tuserHSizer->Add( m_pMinUserLabel, 0, wxTOP|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 5 );

	m_pMinUnitLabel = new wxStaticText( tuserBSizer->GetStaticBox(), wxID_ANY, wxT(" dB)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_pMinUnitLabel->Wrap( -1 );
	tuserHSizer->Add( m_pMinUnitLabel, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5 );


	tuserBSizer->Add( tuserHSizer, 0, wxEXPAND, 5 );


	controlsVSizer->Add( tuserBSizer, 0, wxALL|wxEXPAND, 5 );


	m_topRightPanel->SetSizer( controlsVSizer );
	m_topRightPanel->Layout();
	controlsVSizer->Fit( m_topRightPanel );
	upperHSizer->Add( m_topRightPanel, 0, wxALL|wxEXPAND, 5 );


	m_pUpperPanel->SetSizer( upperHSizer );
	m_pUpperPanel->Layout();
	upperHSizer->Fit( m_pUpperPanel );
	m_pLowerPanel = new wxPanel( m_pHorzSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* gridVSizer;
	gridVSizer = new wxBoxSizer( wxVERTICAL );

	m_pGrid = new wxGrid( m_pLowerPanel, ID_PARAM_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_pGrid->CreateGrid( 14, 12 );
	m_pGrid->EnableEditing( false );
	m_pGrid->EnableGridLines( true );
	m_pGrid->EnableDragGridSize( false );
	m_pGrid->SetMargins( 0, 0 );

	// Columns
	m_pGrid->SetColSize( 0, 80 );
	m_pGrid->EnableDragColMove( false );
	m_pGrid->EnableDragColSize( true );
	m_pGrid->SetColLabelSize( 25 );
	m_pGrid->SetColLabelValue( 0, wxT("31.5") );
	m_pGrid->SetColLabelValue( 1, wxT("63") );
	m_pGrid->SetColLabelValue( 2, wxT("125") );
	m_pGrid->SetColLabelValue( 3, wxT("250") );
	m_pGrid->SetColLabelValue( 4, wxT("500") );
	m_pGrid->SetColLabelValue( 5, wxT("1k") );
	m_pGrid->SetColLabelValue( 6, wxT("2k") );
	m_pGrid->SetColLabelValue( 7, wxT("4k") );
	m_pGrid->SetColLabelValue( 8, wxT("8k") );
	m_pGrid->SetColLabelValue( 9, wxT("16k") );
	m_pGrid->SetColLabelValue( 10, wxT("A") );
	m_pGrid->SetColLabelValue( 11, wxT("Lin") );
	m_pGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_pGrid->AutoSizeRows();
	m_pGrid->EnableDragRowSize( true );
	m_pGrid->SetRowLabelSize( 120 );
	m_pGrid->SetRowLabelValue( 0, wxT("Signal [dB]") );
	m_pGrid->SetRowLabelValue( 1, wxT("Noise [dB]") );
	m_pGrid->SetRowLabelValue( 2, wxT("strenGth [dB]") );
	m_pGrid->SetRowLabelValue( 3, wxT("C50 [dB]") );
	m_pGrid->SetRowLabelValue( 4, wxT("C80 [dB]") );
	m_pGrid->SetRowLabelValue( 5, wxT("D50 [%]") );
	m_pGrid->SetRowLabelValue( 6, wxT("Ts [ms]") );
	m_pGrid->SetRowLabelValue( 7, wxT("EDT [s]") );
	m_pGrid->SetRowLabelValue( 8, wxT("Tuser [s]") );
	m_pGrid->SetRowLabelValue( 9, wxT("T20 [s]") );
	m_pGrid->SetRowLabelValue( 10, wxT("T30 [s]") );
	m_pGrid->SetRowLabelValue( 11, wxT("Peakiness [dB]") );
	m_pGrid->SetRowLabelValue( 12, wxT("Millisecs [dB]") );
	m_pGrid->SetRowLabelValue( 13, wxT("Impulsivs [dB]") );
	m_pGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance
	m_pGrid->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	// Cell Defaults
	m_pGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	gridVSizer->Add( m_pGrid, 1, wxALL|wxEXPAND, 5 );


	m_pLowerPanel->SetSizer( gridVSizer );
	m_pLowerPanel->Layout();
	gridVSizer->Fit( m_pLowerPanel );
	m_pHorzSplitter->SplitHorizontally( m_pUpperPanel, m_pLowerPanel, 0 );
	mainSizer->Add( m_pHorzSplitter, 1, wxEXPAND, 5 );

	m_pBottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_pBottomLine, 0, wxEXPAND | wxALL, 5 );

	m_pButtonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsHSizer;
	buttonsHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_pHelpButton = new wxButton( m_pButtonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsHSizer->Add( m_pHelpButton, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	buttonsHSizer->Add( 0, 0, 1, wxALIGN_CENTER_VERTICAL, 5 );

	m_pCloseButton = new wxButton( m_pButtonsPanel, wxID_CLOSE, wxT("Close"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	buttonsHSizer->Add( m_pCloseButton, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );

	m_pOkButton = new wxButton( m_pButtonsPanel, wxID_OK, wxT("Filter"), wxDefaultPosition, wxDefaultSize, 0 );

	m_pOkButton->SetDefault();
	m_pOkButton->SetToolTip( wxT("Add to workspace filtered track(s).") );

	buttonsHSizer->Add( m_pOkButton, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );


	m_pButtonsPanel->SetSizer( buttonsHSizer );
	m_pButtonsPanel->Layout();
	buttonsHSizer->Fit( m_pButtonsPanel );
	mainSizer->Add( m_pButtonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AcParShowDlg::OnCloseDialog ) );
	m_pSetupButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnSetupButton ), NULL, this );
	m_pSaveButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnSaveResults ), NULL, this );
	m_pCopyButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnCopyResults ), NULL, this );
	m_pStoreGButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnStoreGRefSig ), NULL, this );
	m_pTracksList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( AcParShowDlg::OnChannelListSelect ), NULL, this );
	m_pGrid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( AcParShowDlg::OnGridCellRightClick ), NULL, this );
	m_pGrid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( AcParShowDlg::OnGraphChange ), NULL, this );
	m_pGrid->Connect( wxEVT_GRID_LABEL_LEFT_DCLICK, wxGridEventHandler( AcParShowDlg::OnGridLabelDoubleClick ), NULL, this );
	m_pHelpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnHelp ), NULL, this );
	m_pCloseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnClose ), NULL, this );
	m_pOkButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnOk ), NULL, this );
}

AcParShowDlg::~AcParShowDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AcParShowDlg::OnCloseDialog ) );
	m_pSetupButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnSetupButton ), NULL, this );
	m_pSaveButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnSaveResults ), NULL, this );
	m_pCopyButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnCopyResults ), NULL, this );
	m_pStoreGButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnStoreGRefSig ), NULL, this );
	m_pTracksList->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( AcParShowDlg::OnChannelListSelect ), NULL, this );
	m_pGrid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( AcParShowDlg::OnGridCellRightClick ), NULL, this );
	m_pGrid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( AcParShowDlg::OnGraphChange ), NULL, this );
	m_pGrid->Disconnect( wxEVT_GRID_LABEL_LEFT_DCLICK, wxGridEventHandler( AcParShowDlg::OnGridLabelDoubleClick ), NULL, this );
	m_pHelpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnHelp ), NULL, this );
	m_pCloseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnClose ), NULL, this );
	m_pOkButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AcParShowDlg::OnOk ), NULL, this );

}

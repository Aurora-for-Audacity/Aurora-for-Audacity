///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "StiDialogs.h"

///////////////////////////////////////////////////////////////////////////

STISetupDlg::STISetupDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_startPage = new wxPanel( m_notebook, ID_START_PNL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* startPageSizer;
	startPageSizer = new wxBoxSizer( wxVERTICAL );


	startPageSizer->Add( 0, 30, 0, wxEXPAND, 5 );

	m_welcomeMsg = new wxStaticText( m_startPage, wxID_ANY, wxT("Welcome to Aurora's Speech Transmission Index Evaluator\n\nIn order to proceed, you have to provide the following recordings:\n\n  1) the calibration signal (94 dB)\n  2) the room's background noise\n  3) the test signal (pink noise, 62 dB @ 1m)\n  4) the room's Impulse Response.\n\nIf you have already calibrated the fullscale, the\nlast value should be saved and loaded as\ndefault; same thing for the S/N calculation:\nif you have previously saved a set of values,\nyou can restore them. \n\nOtherwise, all the signals 1-4 must be present\nin the workspace!\n\nThe recordings can be mono or stereo, but...be coherent!"), wxDefaultPosition, wxDefaultSize, 0 );
	m_welcomeMsg->Wrap( -1 );
	startPageSizer->Add( m_welcomeMsg, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_startPage->SetSizer( startPageSizer );
	m_startPage->Layout();
	startPageSizer->Fit( m_startPage );
	m_notebook->AddPage( m_startPage, wxT("Start"), true );
	m_calibrationPage = new wxPanel( m_notebook, ID_FS_CALIBRATION_PNL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* calibrationPageSizer;
	calibrationPageSizer = new wxBoxSizer( wxVERTICAL );


	calibrationPageSizer->Add( 0, 60, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* fullscaleBoxSizer;
	fullscaleBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_calibrationPage, wxID_ANY, wxT("Current Fullscale") ), wxHORIZONTAL );

	m_fsLeftLabel = new wxStaticText( fullscaleBoxSizer->GetStaticBox(), wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fsLeftLabel->Wrap( -1 );
	fullscaleBoxSizer->Add( m_fsLeftLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fsLeftText = new wxTextCtrl( fullscaleBoxSizer->GetStaticBox(), ID_LEFT_FS_TC, wxT("100.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	fullscaleBoxSizer->Add( m_fsLeftText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fsRightLabel = new wxStaticText( fullscaleBoxSizer->GetStaticBox(), wxID_ANY, wxT("dB        Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fsRightLabel->Wrap( -1 );
	fullscaleBoxSizer->Add( m_fsRightLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fsRightText = new wxTextCtrl( fullscaleBoxSizer->GetStaticBox(), ID_RIGHT_FS_TC, wxT("100.0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_RIGHT );
	fullscaleBoxSizer->Add( m_fsRightText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fsRightUnitsLabel = new wxStaticText( fullscaleBoxSizer->GetStaticBox(), wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fsRightUnitsLabel->Wrap( -1 );
	fullscaleBoxSizer->Add( m_fsRightUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	calibrationPageSizer->Add( fullscaleBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	calibrationPageSizer->Add( 0, 35, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* calibrationTracksBoxSizer;
	calibrationTracksBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_calibrationPage, wxID_ANY, wxT("Choose calibration tracks from lists:") ), wxVERTICAL );

	wxFlexGridSizer* calibrationTracksFgSizer;
	calibrationTracksFgSizer = new wxFlexGridSizer( 2, 5, 0, 0 );
	calibrationTracksFgSizer->SetFlexibleDirection( wxBOTH );
	calibrationTracksFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_leftCalLabel = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftCalLabel->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_leftCalLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_leftCalChoiceChoices[] = { wxT("None") };
	int m_leftCalChoiceNChoices = sizeof( m_leftCalChoiceChoices ) / sizeof( wxString );
	m_leftCalChoice = new wxChoice( calibrationTracksBoxSizer->GetStaticBox(), ID_LEFT_CAL_CHOICE, wxDefaultPosition, wxDefaultSize, m_leftCalChoiceNChoices, m_leftCalChoiceChoices, 0 );
	m_leftCalChoice->SetSelection( 0 );
	m_leftCalChoice->SetMinSize( wxSize( 250,-1 ) );

	calibrationTracksFgSizer->Add( m_leftCalChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_leftCalLevelLabel = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Level:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftCalLevelLabel->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_leftCalLevelLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_leftCalLevelText = new wxTextCtrl( calibrationTracksBoxSizer->GetStaticBox(), ID_LEFT_CAL_TC, wxT("94.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_leftCalLevelText->SetMinSize( wxSize( 60,-1 ) );

	calibrationTracksFgSizer->Add( m_leftCalLevelText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_leftCalLevelUnits = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftCalLevelUnits->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_leftCalLevelUnits, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightCalLabel = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightCalLabel->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_rightCalLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_rightCalChoiceChoices[] = { wxT("None") };
	int m_rightCalChoiceNChoices = sizeof( m_rightCalChoiceChoices ) / sizeof( wxString );
	m_rightCalChoice = new wxChoice( calibrationTracksBoxSizer->GetStaticBox(), ID_RIGHT_CAL_CHOICE, wxDefaultPosition, wxDefaultSize, m_rightCalChoiceNChoices, m_rightCalChoiceChoices, 0 );
	m_rightCalChoice->SetSelection( 0 );
	m_rightCalChoice->Enable( false );
	m_rightCalChoice->SetMinSize( wxSize( 250,-1 ) );

	calibrationTracksFgSizer->Add( m_rightCalChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightCalLevelLabel = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Level:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightCalLevelLabel->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_rightCalLevelLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightCalLevelText = new wxTextCtrl( calibrationTracksBoxSizer->GetStaticBox(), ID_RIGHT_CAL_TC, wxT("94.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_rightCalLevelText->Enable( false );
	m_rightCalLevelText->SetMinSize( wxSize( 60,-1 ) );

	calibrationTracksFgSizer->Add( m_rightCalLevelText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightCalLevelUnits = new wxStaticText( calibrationTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightCalLevelUnits->Wrap( -1 );
	calibrationTracksFgSizer->Add( m_rightCalLevelUnits, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	calibrationTracksBoxSizer->Add( calibrationTracksFgSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_calibrateButton = new wxButton( calibrationTracksBoxSizer->GetStaticBox(), ID_CALIBRATE_BTN, wxT("Calibrate!"), wxDefaultPosition, wxDefaultSize, 0 );
	m_calibrateButton->Enable( false );

	calibrationTracksBoxSizer->Add( m_calibrateButton, 0, wxALL|wxEXPAND, 5 );


	calibrationPageSizer->Add( calibrationTracksBoxSizer, 0, wxEXPAND, 5 );


	m_calibrationPage->SetSizer( calibrationPageSizer );
	m_calibrationPage->Layout();
	calibrationPageSizer->Fit( m_calibrationPage );
	m_notebook->AddPage( m_calibrationPage, wxT("Fullscale Calibration"), false );
	m_snrCalculationPage = new wxPanel( m_notebook, ID_SN_PNL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* snrCalculationSizer;
	snrCalculationSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* noiseBoxSizer;
	noiseBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_snrCalculationPage, wxID_ANY, wxT("Choose background noise tracks from lists:") ), wxVERTICAL );

	wxFlexGridSizer* noiseFgSizer;
	noiseFgSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	noiseFgSizer->SetFlexibleDirection( wxBOTH );
	noiseFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_leftNoiseLabel = new wxStaticText( noiseBoxSizer->GetStaticBox(), wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftNoiseLabel->Wrap( -1 );
	noiseFgSizer->Add( m_leftNoiseLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_leftNoiseChoiceChoices[] = { wxT("None") };
	int m_leftNoiseChoiceNChoices = sizeof( m_leftNoiseChoiceChoices ) / sizeof( wxString );
	m_leftNoiseChoice = new wxChoice( noiseBoxSizer->GetStaticBox(), ID_LEFT_BCK_NOISE_CHOICE, wxDefaultPosition, wxDefaultSize, m_leftNoiseChoiceNChoices, m_leftNoiseChoiceChoices, 0 );
	m_leftNoiseChoice->SetSelection( 0 );
	m_leftNoiseChoice->SetMinSize( wxSize( 250,-1 ) );

	noiseFgSizer->Add( m_leftNoiseChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightNoiseLabel = new wxStaticText( noiseBoxSizer->GetStaticBox(), wxID_ANY, wxT("Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightNoiseLabel->Wrap( -1 );
	noiseFgSizer->Add( m_rightNoiseLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_rightNoiseChoiceChoices[] = { wxT("None") };
	int m_rightNoiseChoiceNChoices = sizeof( m_rightNoiseChoiceChoices ) / sizeof( wxString );
	m_rightNoiseChoice = new wxChoice( noiseBoxSizer->GetStaticBox(), ID_RIGHT_BCK_NOISE_CHOICE, wxDefaultPosition, wxDefaultSize, m_rightNoiseChoiceNChoices, m_rightNoiseChoiceChoices, 0 );
	m_rightNoiseChoice->SetSelection( 0 );
	m_rightNoiseChoice->Enable( false );
	m_rightNoiseChoice->SetMinSize( wxSize( 250,-1 ) );

	noiseFgSizer->Add( m_rightNoiseChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	noiseBoxSizer->Add( noiseFgSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	snrCalculationSizer->Add( noiseBoxSizer, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* signalBoxSizer;
	signalBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_snrCalculationPage, wxID_ANY, wxT("Choose test signal tracks from lists:") ), wxVERTICAL );

	wxFlexGridSizer* signalFgSizer;
	signalFgSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	signalFgSizer->SetFlexibleDirection( wxBOTH );
	signalFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_leftSignalLabel = new wxStaticText( signalBoxSizer->GetStaticBox(), wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftSignalLabel->Wrap( -1 );
	signalFgSizer->Add( m_leftSignalLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_leftSignalChoiceChoices[] = { wxT("None") };
	int m_leftSignalChoiceNChoices = sizeof( m_leftSignalChoiceChoices ) / sizeof( wxString );
	m_leftSignalChoice = new wxChoice( signalBoxSizer->GetStaticBox(), ID_LEFT_SIGNAL_CHOICE, wxDefaultPosition, wxDefaultSize, m_leftSignalChoiceNChoices, m_leftSignalChoiceChoices, 0 );
	m_leftSignalChoice->SetSelection( 0 );
	m_leftSignalChoice->SetMinSize( wxSize( 250,-1 ) );

	signalFgSizer->Add( m_leftSignalChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightSignalLabel = new wxStaticText( signalBoxSizer->GetStaticBox(), wxID_ANY, wxT("Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightSignalLabel->Wrap( -1 );
	signalFgSizer->Add( m_rightSignalLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_rightSignalChoiceChoices[] = { wxT("None") };
	int m_rightSignalChoiceNChoices = sizeof( m_rightSignalChoiceChoices ) / sizeof( wxString );
	m_rightSignalChoice = new wxChoice( signalBoxSizer->GetStaticBox(), ID_RIGHT_SIGNAL_CHOICE, wxDefaultPosition, wxDefaultSize, m_rightSignalChoiceNChoices, m_rightSignalChoiceChoices, 0 );
	m_rightSignalChoice->SetSelection( 0 );
	m_rightSignalChoice->Enable( false );
	m_rightSignalChoice->SetMinSize( wxSize( 250,-1 ) );

	signalFgSizer->Add( m_rightSignalChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	signalBoxSizer->Add( signalFgSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* rbuttonsSizer;
	rbuttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_signalRadioButton = new wxRadioButton( signalBoxSizer->GetStaticBox(), ID_SIGNAL_RB, wxT("Signal"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	rbuttonsSizer->Add( m_signalRadioButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_sigNoiseRadioButton = new wxRadioButton( signalBoxSizer->GetStaticBox(), ID_SIGNOISE_RB, wxT("Signal + Noise"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sigNoiseRadioButton->SetValue( true );
	rbuttonsSizer->Add( m_sigNoiseRadioButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	signalBoxSizer->Add( rbuttonsSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	snrCalculationSizer->Add( signalBoxSizer, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* levelsTableBoxSizer;
	levelsTableBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_snrCalculationPage, wxID_ANY, wxT("Levels:") ), wxVERTICAL );

	m_splTable = new wxGrid( levelsTableBoxSizer->GetStaticBox(), ID_SPL_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_splTable->CreateGrid( 7, 6 );
	m_splTable->EnableEditing( false );
	m_splTable->EnableGridLines( true );
	m_splTable->EnableDragGridSize( false );
	m_splTable->SetMargins( 0, 0 );

	// Columns
	m_splTable->EnableDragColMove( false );
	m_splTable->EnableDragColSize( true );
	m_splTable->SetColLabelSize( 30 );
	m_splTable->SetColLabelValue( 0, wxT("BckN Left") );
	m_splTable->SetColLabelValue( 1, wxT("BckN Right") );
	m_splTable->SetColLabelValue( 2, wxT("Sig Left") );
	m_splTable->SetColLabelValue( 3, wxT("Sig Right") );
	m_splTable->SetColLabelValue( 4, wxT("S+N Left") );
	m_splTable->SetColLabelValue( 5, wxT("S+N Right") );
	m_splTable->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_splTable->SetRowSize( 0, 23 );
	m_splTable->SetRowSize( 1, 23 );
	m_splTable->SetRowSize( 2, 23 );
	m_splTable->SetRowSize( 3, 23 );
	m_splTable->SetRowSize( 4, 23 );
	m_splTable->SetRowSize( 5, 23 );
	m_splTable->SetRowSize( 6, 23 );
	m_splTable->EnableDragRowSize( true );
	m_splTable->SetRowLabelSize( 60 );
	m_splTable->SetRowLabelValue( 0, wxT("125") );
	m_splTable->SetRowLabelValue( 1, wxT("250") );
	m_splTable->SetRowLabelValue( 2, wxT("500") );
	m_splTable->SetRowLabelValue( 3, wxT("1k") );
	m_splTable->SetRowLabelValue( 4, wxT("2k") );
	m_splTable->SetRowLabelValue( 5, wxT("4k") );
	m_splTable->SetRowLabelValue( 6, wxT("8k") );
	m_splTable->SetRowLabelAlignment( wxALIGN_RIGHT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_splTable->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );
	m_splTable->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	levelsTableBoxSizer->Add( m_splTable, 0, wxALL|wxEXPAND, 5 );


	snrCalculationSizer->Add( levelsTableBoxSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* saveLoadButtonsSizer;
	saveLoadButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_loadSplValuesButton = new wxButton( m_snrCalculationPage, ID_LOAD_SPL_VALUES_BTN, wxT("Load SPL Values"), wxDefaultPosition, wxDefaultSize, 0 );
	saveLoadButtonsSizer->Add( m_loadSplValuesButton, 0, wxALL, 5 );

	m_saveSplValuesButton = new wxButton( m_snrCalculationPage, ID_SAVE_SPL_VALUES_BTN, wxT("Save SPL Values"), wxDefaultPosition, wxDefaultSize, 0 );
	saveLoadButtonsSizer->Add( m_saveSplValuesButton, 0, wxALL, 5 );


	saveLoadButtonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_calculateSPLButton = new wxButton( m_snrCalculationPage, wxID_ANY, wxT("Compute Levels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_calculateSPLButton->Enable( false );

	saveLoadButtonsSizer->Add( m_calculateSPLButton, 0, wxALL, 5 );


	snrCalculationSizer->Add( saveLoadButtonsSizer, 0, wxEXPAND, 5 );


	m_snrCalculationPage->SetSizer( snrCalculationSizer );
	m_snrCalculationPage->Layout();
	snrCalculationSizer->Fit( m_snrCalculationPage );
	m_notebook->AddPage( m_snrCalculationPage, wxT("SNR Calculation"), false );
	m_roomIrPage = new wxPanel( m_notebook, ID_IR_PNL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* roomIrPageSizer;
	roomIrPageSizer = new wxBoxSizer( wxVERTICAL );


	roomIrPageSizer->Add( 0, 80, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* roomIrTracksBoxSizer;
	roomIrTracksBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_roomIrPage, wxID_ANY, wxT("Choose Impulse Responses from track lists:") ), wxVERTICAL );

	wxFlexGridSizer* roomIrTracksFgSizer;
	roomIrTracksFgSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	roomIrTracksFgSizer->SetFlexibleDirection( wxBOTH );
	roomIrTracksFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_leftIrLabel = new wxStaticText( roomIrTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_leftIrLabel->Wrap( -1 );
	roomIrTracksFgSizer->Add( m_leftIrLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_leftIrChoiceChoices[] = { wxT("None") };
	int m_leftIrChoiceNChoices = sizeof( m_leftIrChoiceChoices ) / sizeof( wxString );
	m_leftIrChoice = new wxChoice( roomIrTracksBoxSizer->GetStaticBox(), ID_LEFT_IR_CHOICE, wxDefaultPosition, wxDefaultSize, m_leftIrChoiceNChoices, m_leftIrChoiceChoices, 0 );
	m_leftIrChoice->SetSelection( 0 );
	m_leftIrChoice->SetMinSize( wxSize( 250,-1 ) );

	roomIrTracksFgSizer->Add( m_leftIrChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rightIrLabel = new wxStaticText( roomIrTracksBoxSizer->GetStaticBox(), wxID_ANY, wxT("Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rightIrLabel->Wrap( -1 );
	roomIrTracksFgSizer->Add( m_rightIrLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_rightIrChoiceChoices[] = { wxT("None") };
	int m_rightIrChoiceNChoices = sizeof( m_rightIrChoiceChoices ) / sizeof( wxString );
	m_rightIrChoice = new wxChoice( roomIrTracksBoxSizer->GetStaticBox(), ID_RIGHT_IR_CHOICE, wxDefaultPosition, wxDefaultSize, m_rightIrChoiceNChoices, m_rightIrChoiceChoices, 0 );
	m_rightIrChoice->SetSelection( 0 );
	m_rightIrChoice->Enable( false );
	m_rightIrChoice->SetMinSize( wxSize( 250,-1 ) );

	roomIrTracksFgSizer->Add( m_rightIrChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	roomIrTracksBoxSizer->Add( roomIrTracksFgSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	roomIrPageSizer->Add( roomIrTracksBoxSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* fatSizer;
	fatSizer = new wxBoxSizer( wxHORIZONTAL );

	m_fatLabel = new wxStaticText( m_roomIrPage, wxID_ANY, wxT("First Arrival Threshold:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fatLabel->Wrap( -1 );
	fatSizer->Add( m_fatLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fatText = new wxTextCtrl( m_roomIrPage, ID_FAT_TC, wxT("20.0"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_fatText->SetMinSize( wxSize( 60,-1 ) );

	fatSizer->Add( m_fatText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_fatUnitsLabel = new wxStaticText( m_roomIrPage, wxID_ANY, wxT("% of fullscale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fatUnitsLabel->Wrap( -1 );
	fatSizer->Add( m_fatUnitsLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	roomIrPageSizer->Add( fatSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );


	m_roomIrPage->SetSizer( roomIrPageSizer );
	m_roomIrPage->Layout();
	roomIrPageSizer->Fit( m_roomIrPage );
	m_notebook->AddPage( m_roomIrPage, wxT("Room Impulse Response"), false );

	mainSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpButton = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_cancelButton = new wxButton( m_buttonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );

	m_cancelButton->SetDefault();
	buttonsSizer->Add( m_cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_okButton = new wxButton( m_buttonsPanel, wxID_OK, wxT("Compute STI"), wxDefaultPosition, wxDefaultSize, 0 );
	m_okButton->Enable( false );

	buttonsSizer->Add( m_okButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( STISetupDlg::OnCloseDialog ) );
	m_fsLeftText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnLeftFullscaleText ), NULL, this );
	m_fsRightText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnRightFullscaleText ), NULL, this );
	m_leftCalChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnCalibLeftChoice ), NULL, this );
	m_leftCalLevelText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnCalibRefLeftText ), NULL, this );
	m_rightCalChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnCalibRightChoice ), NULL, this );
	m_rightCalLevelText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnCalibRefRightText ), NULL, this );
	m_calibrateButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnDoFullscaleCalibration ), NULL, this );
	m_leftNoiseChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnNoiseLeftChoice ), NULL, this );
	m_rightNoiseChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnNoiseRightChoice ), NULL, this );
	m_leftSignalChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalLeftChoice ), NULL, this );
	m_rightSignalChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalRightChoice ), NULL, this );
	m_signalRadioButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalRadio ), NULL, this );
	m_sigNoiseRadioButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalPlusNoiseRadio ), NULL, this );
	m_loadSplValuesButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnLoadSplValues ), NULL, this );
	m_saveSplValuesButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnSaveSplValues ), NULL, this );
	m_calculateSPLButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnComputeSNRatio ), NULL, this );
	m_leftIrChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnLeftIRChoice ), NULL, this );
	m_rightIrChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnRightIRChoice ), NULL, this );
	m_fatText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnFatText ), NULL, this );
	m_helpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnHelp ), NULL, this );
	m_cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnCancel ), NULL, this );
	m_okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnComputeSTI ), NULL, this );
}

STISetupDlg::~STISetupDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( STISetupDlg::OnCloseDialog ) );
	m_fsLeftText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnLeftFullscaleText ), NULL, this );
	m_fsRightText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnRightFullscaleText ), NULL, this );
	m_leftCalChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnCalibLeftChoice ), NULL, this );
	m_leftCalLevelText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnCalibRefLeftText ), NULL, this );
	m_rightCalChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnCalibRightChoice ), NULL, this );
	m_rightCalLevelText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnCalibRefRightText ), NULL, this );
	m_calibrateButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnDoFullscaleCalibration ), NULL, this );
	m_leftNoiseChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnNoiseLeftChoice ), NULL, this );
	m_rightNoiseChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnNoiseRightChoice ), NULL, this );
	m_leftSignalChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalLeftChoice ), NULL, this );
	m_rightSignalChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalRightChoice ), NULL, this );
	m_signalRadioButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalRadio ), NULL, this );
	m_sigNoiseRadioButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STISetupDlg::OnSignalPlusNoiseRadio ), NULL, this );
	m_loadSplValuesButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnLoadSplValues ), NULL, this );
	m_saveSplValuesButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnSaveSplValues ), NULL, this );
	m_calculateSPLButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnComputeSNRatio ), NULL, this );
	m_leftIrChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnLeftIRChoice ), NULL, this );
	m_rightIrChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISetupDlg::OnRightIRChoice ), NULL, this );
	m_fatText->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( STISetupDlg::OnFatText ), NULL, this );
	m_helpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnHelp ), NULL, this );
	m_cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnCancel ), NULL, this );
	m_okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STISetupDlg::OnComputeSTI ), NULL, this );

}

STIShowDlg::STIShowDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxALL|wxEXPAND, 5 );

	m_bodyPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bodySizer;
	bodySizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* tableNotebookSizer;
	tableNotebookSizer = new wxBoxSizer( wxVERTICAL );

	m_notebook = new wxNotebook( m_bodyPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_tablePage = new wxPanel( m_notebook, ID_GRID_PNL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* tableSizer;
	tableSizer = new wxBoxSizer( wxVERTICAL );

	m_mtfTable = new wxGrid( m_tablePage, ID_MTF_VALUES_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_mtfTable->CreateGrid( 14, 7 );
	m_mtfTable->EnableEditing( false );
	m_mtfTable->EnableGridLines( true );
	m_mtfTable->EnableDragGridSize( false );
	m_mtfTable->SetMargins( 0, 0 );

	// Columns
	m_mtfTable->EnableDragColMove( false );
	m_mtfTable->EnableDragColSize( true );
	m_mtfTable->SetColLabelSize( 30 );
	m_mtfTable->SetColLabelValue( 0, wxT("125 Hz") );
	m_mtfTable->SetColLabelValue( 1, wxT("250 Hz") );
	m_mtfTable->SetColLabelValue( 2, wxT("500 Hz") );
	m_mtfTable->SetColLabelValue( 3, wxT("1 kHz") );
	m_mtfTable->SetColLabelValue( 4, wxT("2 kHz") );
	m_mtfTable->SetColLabelValue( 5, wxT("4 kHz") );
	m_mtfTable->SetColLabelValue( 6, wxT("8 kHz") );
	m_mtfTable->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_mtfTable->EnableDragRowSize( true );
	m_mtfTable->SetRowLabelSize( 60 );
	m_mtfTable->SetRowLabelValue( 0, wxT("0.63") );
	m_mtfTable->SetRowLabelValue( 1, wxT("0.8") );
	m_mtfTable->SetRowLabelValue( 2, wxT("1") );
	m_mtfTable->SetRowLabelValue( 3, wxT("1.25") );
	m_mtfTable->SetRowLabelValue( 4, wxT("1.6") );
	m_mtfTable->SetRowLabelValue( 5, wxT("2") );
	m_mtfTable->SetRowLabelValue( 6, wxT("2.5") );
	m_mtfTable->SetRowLabelValue( 7, wxT("3.15") );
	m_mtfTable->SetRowLabelValue( 8, wxT("4") );
	m_mtfTable->SetRowLabelValue( 9, wxT("5") );
	m_mtfTable->SetRowLabelValue( 10, wxT("6.3") );
	m_mtfTable->SetRowLabelValue( 11, wxT("8") );
	m_mtfTable->SetRowLabelValue( 12, wxT("10") );
	m_mtfTable->SetRowLabelValue( 13, wxT("12.5") );
	m_mtfTable->SetRowLabelAlignment( wxALIGN_RIGHT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_mtfTable->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );
	m_mtfTable->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_TOP );
	tableSizer->Add( m_mtfTable, 0, wxALL|wxEXPAND, 5 );


	m_tablePage->SetSizer( tableSizer );
	m_tablePage->Layout();
	tableSizer->Fit( m_tablePage );
	m_notebook->AddPage( m_tablePage, wxT("MTF Values table"), false );

	tableNotebookSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );

	wxStaticBoxSizer* stiTableSizer;
	stiTableSizer = new wxStaticBoxSizer( new wxStaticBox( m_bodyPanel, wxID_ANY, wxT("Band STI values") ), wxVERTICAL );

	m_stiTable = new wxGrid( stiTableSizer->GetStaticBox(), ID_BAND_STI_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_stiTable->CreateGrid( 1, 7 );
	m_stiTable->EnableEditing( false );
	m_stiTable->EnableGridLines( true );
	m_stiTable->EnableDragGridSize( false );
	m_stiTable->SetMargins( 0, 0 );

	// Columns
	m_stiTable->EnableDragColMove( false );
	m_stiTable->EnableDragColSize( true );
	m_stiTable->SetColLabelSize( 30 );
	m_stiTable->SetColLabelValue( 0, wxT("125 Hz") );
	m_stiTable->SetColLabelValue( 1, wxT("250 Hz") );
	m_stiTable->SetColLabelValue( 2, wxT("500 Hz") );
	m_stiTable->SetColLabelValue( 3, wxT("1 kHz") );
	m_stiTable->SetColLabelValue( 4, wxT("2 kHz") );
	m_stiTable->SetColLabelValue( 5, wxT("4 kHz") );
	m_stiTable->SetColLabelValue( 6, wxT("8 kHz") );
	m_stiTable->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_stiTable->EnableDragRowSize( true );
	m_stiTable->SetRowLabelSize( 80 );
	m_stiTable->SetRowLabelValue( 0, wxT("Band STI") );
	m_stiTable->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_stiTable->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );
	m_stiTable->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_TOP );
	stiTableSizer->Add( m_stiTable, 0, wxALL, 5 );


	tableNotebookSizer->Add( stiTableSizer, 0, wxEXPAND, 5 );


	bodySizer->Add( tableNotebookSizer, 1, wxEXPAND, 5 );

	m_controlsPanel = new wxPanel( m_bodyPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* controlsPanelSizer;
	controlsPanelSizer = new wxBoxSizer( wxVERTICAL );


	controlsPanelSizer->Add( 0, 25, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* channelsBoxSizer;
	channelsBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_controlsPanel, wxID_ANY, wxT("Channel Selection") ), wxHORIZONTAL );

	m_leftRadioButton = new wxRadioButton( channelsBoxSizer->GetStaticBox(), ID_LEFT_RB, wxT("Left"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_leftRadioButton->SetValue( true );
	channelsBoxSizer->Add( m_leftRadioButton, 0, wxALL, 5 );


	channelsBoxSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_rightRadioButton = new wxRadioButton( channelsBoxSizer->GetStaticBox(), ID_RIGHT_RB, wxT("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	channelsBoxSizer->Add( m_rightRadioButton, 0, wxALL, 5 );


	controlsPanelSizer->Add( channelsBoxSizer, 0, wxEXPAND, 5 );


	controlsPanelSizer->Add( 0, 25, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* settingsSizer;
	settingsSizer = new wxStaticBoxSizer( new wxStaticBox( m_controlsPanel, wxID_ANY, wxT("MTF Settings") ), wxVERTICAL );

	m_snCorrectionCheck = new wxCheckBox( settingsSizer->GetStaticBox(), ID_SNCORR_CB, wxT("S/N Correction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_snCorrectionCheck->SetValue(true);
	settingsSizer->Add( m_snCorrectionCheck, 0, wxALL, 5 );

	m_maskCorrectionCheck = new wxCheckBox( settingsSizer->GetStaticBox(), ID_MASK_CB, wxT("Masking Correction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_maskCorrectionCheck->SetValue(true);
	settingsSizer->Add( m_maskCorrectionCheck, 0, wxALL, 5 );


	controlsPanelSizer->Add( settingsSizer, 0, wxEXPAND, 5 );


	controlsPanelSizer->Add( 0, 25, 0, wxEXPAND, 5 );

	wxStaticBoxSizer* stiWeightsBoxSizer;
	stiWeightsBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_controlsPanel, wxID_ANY, wxT("Weighted STI") ), wxVERTICAL );

	wxFlexGridSizer* stiWeightFgSizer;
	stiWeightFgSizer = new wxFlexGridSizer( 5, 2, 0, 0 );
	stiWeightFgSizer->SetFlexibleDirection( wxBOTH );
	stiWeightFgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_stiMaleLabel = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("STI male:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stiMaleLabel->Wrap( -1 );
	stiWeightFgSizer->Add( m_stiMaleLabel, 0, wxALL, 5 );

	m_stiMaleValue = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), ID_STI_MALE_ST, wxT("0.000"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_stiMaleValue->Wrap( -1 );
	stiWeightFgSizer->Add( m_stiMaleValue, 0, wxALL, 5 );

	m_stiFemaleLabel = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("STI female:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stiFemaleLabel->Wrap( -1 );
	stiWeightFgSizer->Add( m_stiFemaleLabel, 0, wxALL, 5 );

	m_stiFemaleValue = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), ID_STI_FEMALE_ST, wxT("0.000"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_stiFemaleValue->Wrap( -1 );
	stiWeightFgSizer->Add( m_stiFemaleValue, 1, wxALL, 5 );

	m_rastiLabel = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("RaSTI:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rastiLabel->Wrap( -1 );
	stiWeightFgSizer->Add( m_rastiLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_rastiValue = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), ID_RASTI_ST, wxT("0.000"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_rastiValue->Wrap( -1 );
	stiWeightFgSizer->Add( m_rastiValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_stitelLabel = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("STItel:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stitelLabel->Wrap( -1 );
	stiWeightFgSizer->Add( m_stitelLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_stitelValue = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("0.000"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_stitelValue->Wrap( -1 );
	stiWeightFgSizer->Add( m_stitelValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_stipaLabel = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("STIpa:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stipaLabel->Wrap( -1 );
	stiWeightFgSizer->Add( m_stipaLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_stipaValue = new wxStaticText( stiWeightsBoxSizer->GetStaticBox(), wxID_ANY, wxT("0.000"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_stipaValue->Wrap( -1 );
	stiWeightFgSizer->Add( m_stipaValue, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	stiWeightsBoxSizer->Add( stiWeightFgSizer, 1, wxEXPAND, 5 );


	controlsPanelSizer->Add( stiWeightsBoxSizer, 0, wxEXPAND, 5 );


	controlsPanelSizer->Add( 0, 25, 0, wxEXPAND, 5 );

	m_saveSettingsButton = new wxButton( m_controlsPanel, ID_SAVE_BTN, wxT("Save to File..."), wxDefaultPosition, wxDefaultSize, 0 );
	controlsPanelSizer->Add( m_saveSettingsButton, 0, wxALL|wxEXPAND, 5 );


	m_controlsPanel->SetSizer( controlsPanelSizer );
	m_controlsPanel->Layout();
	controlsPanelSizer->Fit( m_controlsPanel );
	bodySizer->Add( m_controlsPanel, 0, wxEXPAND | wxALL, 5 );


	m_bodyPanel->SetSizer( bodySizer );
	m_bodyPanel->Layout();
	bodySizer->Fit( m_bodyPanel );
	mainSizer->Add( m_bodyPanel, 1, wxEXPAND | wxALL, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpButton = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpButton, 0, wxALL, 5 );

	m_okButton = new wxButton( m_buttonsPanel, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_okButton, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxALIGN_RIGHT, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( STIShowDlg::OnCloseDialog ) );
	m_leftRadioButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STIShowDlg::OnLeftRadio ), NULL, this );
	m_rightRadioButton->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STIShowDlg::OnRightRadio ), NULL, this );
	m_snCorrectionCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( STIShowDlg::OnSnCorrectionCheck ), NULL, this );
	m_maskCorrectionCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( STIShowDlg::OnMaskCorrectionCheck ), NULL, this );
	m_saveSettingsButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnSave ), NULL, this );
	m_helpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnHelp ), NULL, this );
	m_okButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnOk ), NULL, this );
}

STIShowDlg::~STIShowDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( STIShowDlg::OnCloseDialog ) );
	m_leftRadioButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STIShowDlg::OnLeftRadio ), NULL, this );
	m_rightRadioButton->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( STIShowDlg::OnRightRadio ), NULL, this );
	m_snCorrectionCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( STIShowDlg::OnSnCorrectionCheck ), NULL, this );
	m_maskCorrectionCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( STIShowDlg::OnMaskCorrectionCheck ), NULL, this );
	m_saveSettingsButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnSave ), NULL, this );
	m_helpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnHelp ), NULL, this );
	m_okButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( STIShowDlg::OnOk ), NULL, this );

}

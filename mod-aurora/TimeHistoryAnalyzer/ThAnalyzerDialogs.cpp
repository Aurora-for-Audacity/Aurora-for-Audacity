///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ThAnalyzerNbook.h"

#include "ThAnalyzerDialogs.h"

///////////////////////////////////////////////////////////////////////////

THADlg::THADlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxSize( -1,-1 ) );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_pNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_setupPanel = new wxPanel( m_pNotebook, ID_SETUP_PAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* setupSizer;
	setupSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* setupBoxSizer;
	setupBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_setupPanel, wxID_ANY, wxT("Track(s) to analyze:") ), wxVERTICAL );

	m_setupGrid = new wxGrid( setupBoxSizer->GetStaticBox(), ID_FULLSCALE_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_setupGrid->CreateGrid( 3, 4 );
	m_setupGrid->EnableEditing( true );
	m_setupGrid->EnableGridLines( true );
	m_setupGrid->EnableDragGridSize( false );
	m_setupGrid->SetMargins( 0, 0 );

	// Columns
	m_setupGrid->SetColSize( 0, 90 );
	m_setupGrid->SetColSize( 1, 90 );
	m_setupGrid->SetColSize( 2, 90 );
	m_setupGrid->SetColSize( 3, 90 );
	m_setupGrid->AutoSizeColumns();
	m_setupGrid->EnableDragColMove( false );
	m_setupGrid->EnableDragColSize( true );
	m_setupGrid->SetColLabelSize( 30 );
	m_setupGrid->SetColLabelValue( 0, wxT("Ch 1") );
	m_setupGrid->SetColLabelValue( 1, wxT("Ch 2") );
	m_setupGrid->SetColLabelValue( 2, wxT("Ch 3") );
	m_setupGrid->SetColLabelValue( 3, wxT("Ch 4") );
	m_setupGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_setupGrid->EnableDragRowSize( true );
	m_setupGrid->SetRowLabelSize( 140 );
	m_setupGrid->SetRowLabelValue( 0, wxT("Select track:") );
	m_setupGrid->SetRowLabelValue( 1, wxT("Select filter:") );
	m_setupGrid->SetRowLabelValue( 2, wxT("Fullscale level [dB]") );
	m_setupGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_setupGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	m_setupGrid->SetMinSize( wxSize( 400,110 ) );

	setupBoxSizer->Add( m_setupGrid, 1, wxALL|wxEXPAND, 5 );


	setupSizer->Add( setupBoxSizer, 1, wxALL|wxEXPAND, 5 );

	m_removeDcCheck = new wxCheckBox( m_setupPanel, ID_RDC_CHECK, wxT("Remove DC component"), wxDefaultPosition, wxDefaultSize, 0 );
	m_removeDcCheck->SetValue(true);
	setupSizer->Add( m_removeDcCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_setupPanel->SetSizer( setupSizer );
	m_setupPanel->Layout();
	setupSizer->Fit( m_setupPanel );
	m_pNotebook->AddPage( m_setupPanel, wxT("Setup"), true );
	m_calibrationPanel = new wxPanel( m_pNotebook, ID_CALIBRATION_PAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* calibrationSizer;
	calibrationSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* calibrationBoxSizer;
	calibrationBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_calibrationPanel, wxID_ANY, wxT("Select track(s) containing calibration signal:") ), wxVERTICAL );

	m_calibrationGrid = new wxGrid( calibrationBoxSizer->GetStaticBox(), ID_CALIBRATION_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_calibrationGrid->CreateGrid( 2, 4 );
	m_calibrationGrid->EnableEditing( true );
	m_calibrationGrid->EnableGridLines( true );
	m_calibrationGrid->EnableDragGridSize( false );
	m_calibrationGrid->SetMargins( 0, 0 );

	// Columns
	m_calibrationGrid->SetColSize( 0, 90 );
	m_calibrationGrid->SetColSize( 1, 90 );
	m_calibrationGrid->SetColSize( 2, 90 );
	m_calibrationGrid->SetColSize( 3, 90 );
	m_calibrationGrid->AutoSizeColumns();
	m_calibrationGrid->EnableDragColMove( true );
	m_calibrationGrid->EnableDragColSize( true );
	m_calibrationGrid->SetColLabelSize( 30 );
	m_calibrationGrid->SetColLabelValue( 0, wxT("Ch 1") );
	m_calibrationGrid->SetColLabelValue( 1, wxT("Ch 2") );
	m_calibrationGrid->SetColLabelValue( 2, wxT("Ch 3") );
	m_calibrationGrid->SetColLabelValue( 3, wxT("Ch 4") );
	m_calibrationGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_calibrationGrid->EnableDragRowSize( true );
	m_calibrationGrid->SetRowLabelSize( 160 );
	m_calibrationGrid->SetRowLabelValue( 0, wxT("Calibration signal:") );
	m_calibrationGrid->SetRowLabelValue( 1, wxT("Reference level [dB]") );
	m_calibrationGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_calibrationGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	m_calibrationGrid->SetMinSize( wxSize( 400,-1 ) );

	calibrationBoxSizer->Add( m_calibrationGrid, 1, wxALL|wxEXPAND, 5 );

	m_ch1ForAllCheck = new wxCheckBox( calibrationBoxSizer->GetStaticBox(), ID_CALIB_APPLY_TO_ALL, wxT("Apply Ch 1 calibration to all channels"), wxDefaultPosition, wxDefaultSize, 0 );
	calibrationBoxSizer->Add( m_ch1ForAllCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	calibrationSizer->Add( calibrationBoxSizer, 1, wxEXPAND, 5 );

	m_calibrateButton = new wxButton( m_calibrationPanel, ID_CALIBRATE_BTN, wxT("Calibrate"), wxDefaultPosition, wxDefaultSize, 0 );
	calibrationSizer->Add( m_calibrateButton, 0, wxALL|wxEXPAND, 5 );


	m_calibrationPanel->SetSizer( calibrationSizer );
	m_calibrationPanel->Layout();
	calibrationSizer->Fit( m_calibrationPanel );
	m_pNotebook->AddPage( m_calibrationPanel, wxT("Calibration"), false );

	mainSizer->Add( m_pNotebook, 1, wxALL|wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpButton = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	buttonsSizer->Add( m_helpButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_cancelButton = new wxButton( m_buttonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	buttonsSizer->Add( m_cancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_analyzeButton = new wxButton( m_buttonsPanel, wxID_OK, wxT("Analyze"), wxDefaultPosition, wxSize( -1,-1 ), 0 );

	m_analyzeButton->SetDefault();
	buttonsSizer->Add( m_analyzeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	m_setupGrid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADlg::OnFullscaleGridContentChanged ), NULL, this );
	m_removeDcCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADlg::OnRemoveDCCheck ), NULL, this );
	m_calibrationGrid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADlg::OnCalibrationGridContentChanged ), NULL, this );
	m_ch1ForAllCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADlg::OnApplyToAllCheck ), NULL, this );
	m_calibrateButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnCalibrate ), NULL, this );
	m_helpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnHelp ), NULL, this );
	m_cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnCancel ), NULL, this );
	m_analyzeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnOk ), NULL, this );
}

THADlg::~THADlg()
{
	// Disconnect Events
	m_setupGrid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADlg::OnFullscaleGridContentChanged ), NULL, this );
	m_removeDcCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADlg::OnRemoveDCCheck ), NULL, this );
	m_calibrationGrid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADlg::OnCalibrationGridContentChanged ), NULL, this );
	m_ch1ForAllCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADlg::OnApplyToAllCheck ), NULL, this );
	m_calibrateButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnCalibrate ), NULL, this );
	m_helpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnHelp ), NULL, this );
	m_cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnCancel ), NULL, this );
	m_analyzeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADlg::OnOk ), NULL, this );

}

THAShowDlg::THAShowDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_BORDER|wxSP_NO_XP_THEME );
	m_splitter->Connect( wxEVT_IDLE, wxIdleEventHandler( THAShowDlg::m_splitterOnIdle ), NULL, this );

	m_leftPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	wxBoxSizer* leftSizer;
	leftSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* leftBoxSizer;
	leftBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_leftPanel, wxID_ANY, wxT("Input data graph") ), wxVERTICAL );

	wxBoxSizer* leftTopHSizer;
	leftTopHSizer = new wxBoxSizer( wxHORIZONTAL );

	m_plotTypeLabel = new wxStaticText( leftBoxSizer->GetStaticBox(), wxID_ANY, wxT("What's plotting:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotTypeLabel->Wrap( -1 );
	leftTopHSizer->Add( m_plotTypeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_plotTypeChoiceChoices[] = { wxT("Peak (1 sample)"), wxT("RMS (1 ms)"), wxT("ITU (30 ms)"), wxT("Impulse (35 ms up, 1.5 s down)"), wxT("Fast (125 ms)"), wxT("Slow (1 s)") };
	int m_plotTypeChoiceNChoices = sizeof( m_plotTypeChoiceChoices ) / sizeof( wxString );
	m_plotTypeChoice = new wxChoice( leftBoxSizer->GetStaticBox(), ID_DISPLAY_CHOICE, wxDefaultPosition, wxDefaultSize, m_plotTypeChoiceNChoices, m_plotTypeChoiceChoices, 0 );
	m_plotTypeChoice->SetSelection( 1 );
	leftTopHSizer->Add( m_plotTypeChoice, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	leftBoxSizer->Add( leftTopHSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_plotPanel = new wxPanel( leftBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* plotSizer;
	plotSizer = new wxBoxSizer( wxVERTICAL );

	m_plotScroller = new wxScrolledWindow( m_plotPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_plotScroller->SetScrollRate( 5, 5 );
	wxBoxSizer* scrollerSizer;
	scrollerSizer = new wxBoxSizer( wxVERTICAL );

	m_pPlot = new Aurora::ThAnalyzerPlot(m_plotPanel, ID_PLOT);
	m_pPlot->SetMinSize( wxSize( 200,50 ) );

	scrollerSizer->Add( m_pPlot, 1, wxALL|wxEXPAND, 5 );


	m_plotScroller->SetSizer( scrollerSizer );
	m_plotScroller->Layout();
	scrollerSizer->Fit( m_plotScroller );
	plotSizer->Add( m_plotScroller, 1, wxEXPAND | wxALL, 5 );


	m_plotPanel->SetSizer( plotSizer );
	m_plotPanel->Layout();
	plotSizer->Fit( m_plotPanel );
	leftBoxSizer->Add( m_plotPanel, 1, wxEXPAND | wxALL, 5 );


	leftSizer->Add( leftBoxSizer, 1, wxALL|wxEXPAND, 5 );


	m_leftPanel->SetSizer( leftSizer );
	m_leftPanel->Layout();
	leftSizer->Fit( m_leftPanel );
	m_rightPanel = new wxPanel( m_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* rightSizer;
	rightSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* rightBoxSizer;
	rightBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_rightPanel, wxID_ANY, wxT("Calculated parameters table") ), wxVERTICAL );

	m_gridPanel = new wxPanel( rightBoxSizer->GetStaticBox(), ID_GRID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* gridPanelSizer;
	gridPanelSizer = new wxBoxSizer( wxVERTICAL );

	m_resultsGrid = new wxGrid( m_gridPanel, ID_PARAM_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_resultsGrid->CreateGrid( 15, 1 );
	m_resultsGrid->EnableEditing( false );
	m_resultsGrid->EnableGridLines( true );
	m_resultsGrid->EnableDragGridSize( false );
	m_resultsGrid->SetMargins( 0, 0 );

	// Columns
	m_resultsGrid->EnableDragColMove( false );
	m_resultsGrid->EnableDragColSize( true );
	m_resultsGrid->SetColLabelSize( 30 );
	m_resultsGrid->SetColLabelValue( 0, wxT("Left") );
	m_resultsGrid->SetColLabelValue( 1, wxT("Right") );
	m_resultsGrid->SetColLabelValue( 2, wxT("Avg") );
	m_resultsGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_resultsGrid->AutoSizeRows();
	m_resultsGrid->EnableDragRowSize( true );
	m_resultsGrid->SetRowLabelSize( 220 );
	m_resultsGrid->SetRowLabelValue( 0, wxT("Plot Color") );
	m_resultsGrid->SetRowLabelValue( 1, wxT("Filter Type") );
	m_resultsGrid->SetRowLabelValue( 2, wxT("Full Scale Level [dB Peak]") );
	m_resultsGrid->SetRowLabelValue( 3, wxT("Average Level [Leq - dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 4, wxT("Single Event Level [SEL - dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 5, wxT("Total Duration [s]") );
	m_resultsGrid->SetRowLabelValue( 6, wxT("Active Speech Level [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 7, wxT("Threshold Level [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 8, wxT("Activity Factor [%]") );
	m_resultsGrid->SetRowLabelValue( 9, wxT("Max Peak Level [dB Peak]") );
	m_resultsGrid->SetRowLabelValue( 10, wxT("Max SPL Impulse [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 11, wxT("Max SPL Fast [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 12, wxT("Max SPL Slow [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 13, wxT("Duration of the impulsive event [s]") );
	m_resultsGrid->SetRowLabelValue( 14, wxT("Impulsive Event?") );
	m_resultsGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance
	m_resultsGrid->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	// Cell Defaults
	m_resultsGrid->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );
	m_resultsGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	gridPanelSizer->Add( m_resultsGrid, 1, wxALL|wxEXPAND, 5 );


	m_gridPanel->SetSizer( gridPanelSizer );
	m_gridPanel->Layout();
	gridPanelSizer->Fit( m_gridPanel );
	rightBoxSizer->Add( m_gridPanel, 1, wxEXPAND | wxALL, 5 );


	rightSizer->Add( rightBoxSizer, 1, wxALL|wxEXPAND, 5 );


	m_rightPanel->SetSizer( rightSizer );
	m_rightPanel->Layout();
	rightSizer->Fit( m_rightPanel );
	m_splitter->SplitVertically( m_leftPanel, m_rightPanel, 0 );
	mainSizer->Add( m_splitter, 1, wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpButton = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpButton, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_addTracksButton = new wxButton( m_buttonsPanel, ID_ADD_TRACKS_BTN, wxT("Close && Add tracks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_addTracksButton->SetToolTip( wxT("Close dialog then add filtered tracks to Audacity workspace") );

	buttonsSizer->Add( m_addTracksButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_closeButton = new wxButton( m_buttonsPanel, wxID_OK, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );

	m_closeButton->SetDefault();
	m_closeButton->SetToolTip( wxT("Close dialog") );

	buttonsSizer->Add( m_closeButton, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( THAShowDlg::OnCloseDlg ) );
	m_plotTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( THAShowDlg::OnSelectDisplay ), NULL, this );
	m_resultsGrid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( THAShowDlg::OnGridCellRightClick ), NULL, this );
	m_resultsGrid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( THAShowDlg::OnTogglePlot ), NULL, this );
	m_helpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnHelp ), NULL, this );
	m_addTracksButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnAddTracksBtn ), NULL, this );
	m_closeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnOk ), NULL, this );
}

THAShowDlg::~THAShowDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( THAShowDlg::OnCloseDlg ) );
	m_plotTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( THAShowDlg::OnSelectDisplay ), NULL, this );
	m_resultsGrid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( THAShowDlg::OnGridCellRightClick ), NULL, this );
	m_resultsGrid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( THAShowDlg::OnTogglePlot ), NULL, this );
	m_helpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnHelp ), NULL, this );
	m_addTracksButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnAddTracksBtn ), NULL, this );
	m_closeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THAShowDlg::OnOk ), NULL, this );

}

THADialog::THADialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_pHorzSplitter = new wxSplitterWindow( this, ID_HORZ_SPLITTER, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_pHorzSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( THADialog::m_pHorzSplitterOnIdle ), NULL, this );

	m_upperSplitterPanel = new wxPanel( m_pHorzSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* upperPanelSizer;
	upperPanelSizer = new wxBoxSizer( wxVERTICAL );

	m_pNotebook = new Aurora::ThAnalyzerNotebook( m_upperSplitterPanel, ID_SETUP_NOTEBOOK, wxDefaultPosition, wxDefaultSize, 0 );
	m_setupPage = new wxPanel( m_pNotebook, ID_SETUP_PAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* setupPageSizer;
	setupPageSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* setupGridSizer;
	setupGridSizer = new wxStaticBoxSizer( new wxStaticBox( m_setupPage, wxID_ANY, wxT("Track(s) to analyze:") ), wxVERTICAL );

	m_fullscalesGrid = new wxGrid( setupGridSizer->GetStaticBox(), ID_FULLSCALE_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_fullscalesGrid->CreateGrid( 3, 8 );
	m_fullscalesGrid->EnableEditing( true );
	m_fullscalesGrid->EnableGridLines( true );
	m_fullscalesGrid->EnableDragGridSize( false );
	m_fullscalesGrid->SetMargins( 0, 0 );

	// Columns
	m_fullscalesGrid->SetColSize( 0, 90 );
	m_fullscalesGrid->SetColSize( 1, 90 );
	m_fullscalesGrid->SetColSize( 2, 90 );
	m_fullscalesGrid->SetColSize( 3, 90 );
	m_fullscalesGrid->SetColSize( 4, 90 );
	m_fullscalesGrid->SetColSize( 5, 90 );
	m_fullscalesGrid->SetColSize( 6, 90 );
	m_fullscalesGrid->SetColSize( 7, 90 );
	m_fullscalesGrid->AutoSizeColumns();
	m_fullscalesGrid->EnableDragColMove( false );
	m_fullscalesGrid->EnableDragColSize( true );
	m_fullscalesGrid->SetColLabelSize( 30 );
	m_fullscalesGrid->SetColLabelValue( 0, wxT("Ch 1") );
	m_fullscalesGrid->SetColLabelValue( 1, wxT("Ch 2") );
	m_fullscalesGrid->SetColLabelValue( 2, wxT("Ch 3") );
	m_fullscalesGrid->SetColLabelValue( 3, wxT("Ch 4") );
	m_fullscalesGrid->SetColLabelValue( 4, wxT("Ch 5") );
	m_fullscalesGrid->SetColLabelValue( 5, wxT("Ch 6") );
	m_fullscalesGrid->SetColLabelValue( 6, wxT("Ch 7") );
	m_fullscalesGrid->SetColLabelValue( 7, wxT("Ch 8") );
	m_fullscalesGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_fullscalesGrid->EnableDragRowSize( true );
	m_fullscalesGrid->SetRowLabelSize( 140 );
	m_fullscalesGrid->SetRowLabelValue( 0, wxT("Select track:") );
	m_fullscalesGrid->SetRowLabelValue( 1, wxT("Select filter:") );
	m_fullscalesGrid->SetRowLabelValue( 2, wxT("Fullscale level [dB]") );
	m_fullscalesGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_fullscalesGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	m_fullscalesGrid->SetMinSize( wxSize( 400,-1 ) );

	setupGridSizer->Add( m_fullscalesGrid, 1, wxALL|wxEXPAND, 5 );


	setupPageSizer->Add( setupGridSizer, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* setupControlsSizer;
	setupControlsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_analyzeButton = new wxButton( m_setupPage, ID_ANALYZE_BTN, wxT("Analyze"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	setupControlsSizer->Add( m_analyzeButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	setupControlsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_removeDcCheck = new wxCheckBox( m_setupPage, ID_RDC_CHECK, wxT("Remove DC component"), wxDefaultPosition, wxDefaultSize, 0 );
	m_removeDcCheck->SetValue(true);
	setupControlsSizer->Add( m_removeDcCheck, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	setupPageSizer->Add( setupControlsSizer, 0, wxEXPAND, 5 );


	m_setupPage->SetSizer( setupPageSizer );
	m_setupPage->Layout();
	setupPageSizer->Fit( m_setupPage );
	m_pNotebook->AddPage( m_setupPage, wxT("Setup"), true );
	m_calibrationPage = new wxPanel( m_pNotebook, ID_CALIBRATION_PAGE, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* calibrationPageSizer;
	calibrationPageSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* calibrationGridSizer;
	calibrationGridSizer = new wxStaticBoxSizer( new wxStaticBox( m_calibrationPage, wxID_ANY, wxT("Select track(s) containing calibration signal:") ), wxVERTICAL );

	m_calibrationGrid = new wxGrid( calibrationGridSizer->GetStaticBox(), ID_CALIBRATION_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_calibrationGrid->CreateGrid( 2, 8 );
	m_calibrationGrid->EnableEditing( true );
	m_calibrationGrid->EnableGridLines( true );
	m_calibrationGrid->EnableDragGridSize( false );
	m_calibrationGrid->SetMargins( 0, 0 );

	// Columns
	m_calibrationGrid->SetColSize( 0, 90 );
	m_calibrationGrid->SetColSize( 1, 90 );
	m_calibrationGrid->SetColSize( 2, 90 );
	m_calibrationGrid->SetColSize( 3, 90 );
	m_calibrationGrid->SetColSize( 4, 90 );
	m_calibrationGrid->SetColSize( 5, 90 );
	m_calibrationGrid->SetColSize( 6, 90 );
	m_calibrationGrid->SetColSize( 7, 90 );
	m_calibrationGrid->AutoSizeColumns();
	m_calibrationGrid->EnableDragColMove( true );
	m_calibrationGrid->EnableDragColSize( true );
	m_calibrationGrid->SetColLabelSize( 30 );
	m_calibrationGrid->SetColLabelValue( 0, wxT("Ch 1") );
	m_calibrationGrid->SetColLabelValue( 1, wxT("Ch 2") );
	m_calibrationGrid->SetColLabelValue( 2, wxT("Ch 3") );
	m_calibrationGrid->SetColLabelValue( 3, wxT("Ch 4") );
	m_calibrationGrid->SetColLabelValue( 4, wxT("Ch 5") );
	m_calibrationGrid->SetColLabelValue( 5, wxT("Ch 6") );
	m_calibrationGrid->SetColLabelValue( 6, wxT("Ch 7") );
	m_calibrationGrid->SetColLabelValue( 7, wxT("Ch 8") );
	m_calibrationGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_calibrationGrid->EnableDragRowSize( true );
	m_calibrationGrid->SetRowLabelSize( 160 );
	m_calibrationGrid->SetRowLabelValue( 0, wxT("Calibration signal:") );
	m_calibrationGrid->SetRowLabelValue( 1, wxT("Reference level [dB]") );
	m_calibrationGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_calibrationGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_TOP );
	m_calibrationGrid->SetMinSize( wxSize( 400,-1 ) );

	calibrationGridSizer->Add( m_calibrationGrid, 1, wxALL|wxEXPAND, 5 );


	calibrationPageSizer->Add( calibrationGridSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* calibrationControlsSizer;
	calibrationControlsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_calibrateButton = new wxButton( m_calibrationPage, ID_CALIBRATE_BTN, wxT("Calibrate"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	calibrationControlsSizer->Add( m_calibrateButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	calibrationControlsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_ch1ForAllCheck = new wxCheckBox( m_calibrationPage, ID_CALIB_APPLY_TO_ALL, wxT("Apply Ch 1 calibration to all channels"), wxDefaultPosition, wxDefaultSize, 0 );
	calibrationControlsSizer->Add( m_ch1ForAllCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	calibrationPageSizer->Add( calibrationControlsSizer, 0, wxEXPAND, 5 );


	m_calibrationPage->SetSizer( calibrationPageSizer );
	m_calibrationPage->Layout();
	calibrationPageSizer->Fit( m_calibrationPage );
	m_pNotebook->AddPage( m_calibrationPage, wxT("Calibration"), false );

	upperPanelSizer->Add( m_pNotebook, 1, wxEXPAND | wxALL, 5 );


	m_upperSplitterPanel->SetSizer( upperPanelSizer );
	m_upperSplitterPanel->Layout();
	upperPanelSizer->Fit( m_upperSplitterPanel );
	m_lowerSplitterPanel = new wxPanel( m_pHorzSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* lowerPanelSizer;
	lowerPanelSizer = new wxBoxSizer( wxVERTICAL );

	m_pVertSplitter = new wxSplitterWindow( m_lowerSplitterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D|wxSP_BORDER|wxSP_NO_XP_THEME );
	m_pVertSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( THADialog::m_pVertSplitterOnIdle ), NULL, this );

	m_leftSplitterPanel = new wxPanel( m_pVertSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	wxBoxSizer* leftPanelSizer;
	leftPanelSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* leftPanelBoxSizer;
	leftPanelBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_leftSplitterPanel, wxID_ANY, wxT("Input data graph") ), wxVERTICAL );

	m_plotScroller = new wxScrolledWindow( leftPanelBoxSizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_plotScroller->SetScrollRate( 5, 5 );
	wxBoxSizer* plotScrollerSizer;
	plotScrollerSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* plotControlsSizer;
	plotControlsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_plotTypeLabel = new wxStaticText( m_plotScroller, wxID_ANY, wxT("What's plotted:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_plotTypeLabel->Wrap( -1 );
	plotControlsSizer->Add( m_plotTypeLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_plotTypeChoiceChoices[] = { wxT("Peak (1 sample)"), wxT("RMS (1 ms)"), wxT("ITU (30 ms)"), wxT("Impulse (35 ms up, 1.5 s down)"), wxT("Fast (125 ms)"), wxT("Slow (1 s)") };
	int m_plotTypeChoiceNChoices = sizeof( m_plotTypeChoiceChoices ) / sizeof( wxString );
	m_plotTypeChoice = new wxChoice( m_plotScroller, ID_DISPLAY_CHOICE, wxDefaultPosition, wxDefaultSize, m_plotTypeChoiceNChoices, m_plotTypeChoiceChoices, 0 );
	m_plotTypeChoice->SetSelection( 1 );
	m_plotTypeChoice->Enable( false );

	plotControlsSizer->Add( m_plotTypeChoice, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	plotScrollerSizer->Add( plotControlsSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	m_plotPanel = new wxPanel( m_plotScroller, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* plotSizer;
	plotSizer = new wxBoxSizer( wxVERTICAL );

	m_pPlot = new Aurora::ThAnalyzerPlot(m_plotPanel, ID_PLOT, wxDefaultPosition, wxSize(400,300));
	m_pPlot->SetMinSize( wxSize( 200,50 ) );

	plotSizer->Add( m_pPlot, 1, wxALL|wxEXPAND, 5 );


	m_plotPanel->SetSizer( plotSizer );
	m_plotPanel->Layout();
	plotSizer->Fit( m_plotPanel );
	plotScrollerSizer->Add( m_plotPanel, 1, wxEXPAND | wxALL, 5 );


	m_plotScroller->SetSizer( plotScrollerSizer );
	m_plotScroller->Layout();
	plotScrollerSizer->Fit( m_plotScroller );
	leftPanelBoxSizer->Add( m_plotScroller, 1, wxEXPAND | wxALL, 5 );


	leftPanelSizer->Add( leftPanelBoxSizer, 1, wxALL|wxEXPAND, 5 );


	m_leftSplitterPanel->SetSizer( leftPanelSizer );
	m_leftSplitterPanel->Layout();
	leftPanelSizer->Fit( m_leftSplitterPanel );
	m_rightSplitterPanel = new wxPanel( m_pVertSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* rightPanelSizer;
	rightPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* rightPanelBoxSizer;
	rightPanelBoxSizer = new wxStaticBoxSizer( new wxStaticBox( m_rightSplitterPanel, wxID_ANY, wxT("Calculated parameters table") ), wxVERTICAL );

	m_resultsGridPanel = new wxPanel( rightPanelBoxSizer->GetStaticBox(), ID_GRID_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* resultsGridSizer;
	resultsGridSizer = new wxBoxSizer( wxVERTICAL );

	m_resultsGrid = new wxGrid( m_resultsGridPanel, ID_PARAM_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_resultsGrid->CreateGrid( 15, 1 );
	m_resultsGrid->EnableEditing( false );
	m_resultsGrid->EnableGridLines( true );
	m_resultsGrid->EnableDragGridSize( false );
	m_resultsGrid->SetMargins( 0, 0 );

	// Columns
	m_resultsGrid->EnableDragColMove( false );
	m_resultsGrid->EnableDragColSize( true );
	m_resultsGrid->SetColLabelSize( 30 );
	m_resultsGrid->SetColLabelValue( 0, wxT("Left") );
	m_resultsGrid->SetColLabelValue( 1, wxT("Right") );
	m_resultsGrid->SetColLabelValue( 2, wxT("Avg") );
	m_resultsGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_resultsGrid->AutoSizeRows();
	m_resultsGrid->EnableDragRowSize( true );
	m_resultsGrid->SetRowLabelSize( 220 );
	m_resultsGrid->SetRowLabelValue( 0, wxT("Plot Color") );
	m_resultsGrid->SetRowLabelValue( 1, wxT("Filter Type") );
	m_resultsGrid->SetRowLabelValue( 2, wxT("Full Scale Level [dB Peak]") );
	m_resultsGrid->SetRowLabelValue( 3, wxT("Average Level [Leq - dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 4, wxT("Single Event Level [SEL - dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 5, wxT("Total Duration [s]") );
	m_resultsGrid->SetRowLabelValue( 6, wxT("Active Speech Level [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 7, wxT("Threshold Level [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 8, wxT("Activity Factor [%]") );
	m_resultsGrid->SetRowLabelValue( 9, wxT("Max Peak Level [dB Peak]") );
	m_resultsGrid->SetRowLabelValue( 10, wxT("Max SPL Impulse [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 11, wxT("Max SPL Fast [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 12, wxT("Max SPL Slow [dB RMS]") );
	m_resultsGrid->SetRowLabelValue( 13, wxT("Duration of the impulsive event [s]") );
	m_resultsGrid->SetRowLabelValue( 14, wxT("Impulsive Event?") );
	m_resultsGrid->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance
	m_resultsGrid->SetLabelFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	// Cell Defaults
	m_resultsGrid->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWFRAME ) );
	m_resultsGrid->SetDefaultCellAlignment( wxALIGN_CENTER, wxALIGN_CENTER );
	resultsGridSizer->Add( m_resultsGrid, 1, wxALL|wxEXPAND, 5 );


	m_resultsGridPanel->SetSizer( resultsGridSizer );
	m_resultsGridPanel->Layout();
	resultsGridSizer->Fit( m_resultsGridPanel );
	rightPanelBoxSizer->Add( m_resultsGridPanel, 1, wxEXPAND | wxALL, 5 );


	rightPanelSizer->Add( rightPanelBoxSizer, 1, wxALL|wxEXPAND, 5 );


	m_rightSplitterPanel->SetSizer( rightPanelSizer );
	m_rightSplitterPanel->Layout();
	rightPanelSizer->Fit( m_rightSplitterPanel );
	m_pVertSplitter->SplitVertically( m_leftSplitterPanel, m_rightSplitterPanel, 0 );
	lowerPanelSizer->Add( m_pVertSplitter, 1, wxEXPAND, 5 );


	m_lowerSplitterPanel->SetSizer( lowerPanelSizer );
	m_lowerSplitterPanel->Layout();
	lowerPanelSizer->Fit( m_lowerSplitterPanel );
	m_pHorzSplitter->SplitHorizontally( m_upperSplitterPanel, m_lowerSplitterPanel, 200 );
	mainSizer->Add( m_pHorzSplitter, 1, wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_buttonsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_helpButton = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( m_helpButton, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_addTracksButton = new wxButton( m_buttonsPanel, ID_ADD_TRACKS_BTN, wxT("Close && Add tracks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_addTracksButton->SetToolTip( wxT("Close dialog then add filtered tracks to Audacity workspace") );

	buttonsSizer->Add( m_addTracksButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_closeButton = new wxButton( m_buttonsPanel, wxID_OK, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );

	m_closeButton->SetDefault();
	m_closeButton->SetToolTip( wxT("Close dialog") );

	buttonsSizer->Add( m_closeButton, 0, wxALL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	mainSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( THADialog::OnCloseDialog ) );
	m_fullscalesGrid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADialog::OnFullscaleGridContentChanged ), NULL, this );
	m_analyzeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnAnalyze ), NULL, this );
	m_removeDcCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADialog::OnRemoveDCCheck ), NULL, this );
	m_calibrationGrid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADialog::OnCalibrationGridContentChanged ), NULL, this );
	m_calibrateButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnCalibrate ), NULL, this );
	m_ch1ForAllCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADialog::OnApplyToAllCheck ), NULL, this );
	m_plotTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( THADialog::OnSelectDisplay ), NULL, this );
	m_resultsGrid->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( THADialog::OnGridCellRightClick ), NULL, this );
	m_resultsGrid->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( THADialog::OnTogglePlot ), NULL, this );
	m_helpButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnHelp ), NULL, this );
	m_addTracksButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnAddTracksBtn ), NULL, this );
	m_closeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnOk ), NULL, this );
}

THADialog::~THADialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( THADialog::OnCloseDialog ) );
	m_fullscalesGrid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADialog::OnFullscaleGridContentChanged ), NULL, this );
	m_analyzeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnAnalyze ), NULL, this );
	m_removeDcCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADialog::OnRemoveDCCheck ), NULL, this );
	m_calibrationGrid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( THADialog::OnCalibrationGridContentChanged ), NULL, this );
	m_calibrateButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnCalibrate ), NULL, this );
	m_ch1ForAllCheck->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( THADialog::OnApplyToAllCheck ), NULL, this );
	m_plotTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( THADialog::OnSelectDisplay ), NULL, this );
	m_resultsGrid->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( THADialog::OnGridCellRightClick ), NULL, this );
	m_resultsGrid->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( THADialog::OnTogglePlot ), NULL, this );
	m_helpButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnHelp ), NULL, this );
	m_addTracksButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnAddTracksBtn ), NULL, this );
	m_closeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( THADialog::OnOk ), NULL, this );

}

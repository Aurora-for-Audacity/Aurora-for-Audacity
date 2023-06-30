///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "StiSpectrumDialog.h"

///////////////////////////////////////////////////////////////////////////

STISpectrumDlg::STISpectrumDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,500 ), wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxALL|wxEXPAND, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_topPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* topPanelSizer;
	topPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	m_channelSelectLabel = new wxStaticText( m_topPanel, wxID_ANY, wxT("Select channel:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_channelSelectLabel->Wrap( -1 );
	topPanelSizer->Add( m_channelSelectLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_channelSelectChoiceChoices[] = { wxT("Left"), wxT("Right") };
	int m_channelSelectChoiceNChoices = sizeof( m_channelSelectChoiceChoices ) / sizeof( wxString );
	m_channelSelectChoice = new wxChoice( m_topPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_channelSelectChoiceNChoices, m_channelSelectChoiceChoices, 0 );
	m_channelSelectChoice->SetSelection( 0 );
	topPanelSizer->Add( m_channelSelectChoice, 0, wxALL, 5 );


	m_topPanel->SetSizer( topPanelSizer );
	m_topPanel->Layout();
	topPanelSizer->Fit( m_topPanel );
	mainSizer->Add( m_topPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_verticalSplitter = new wxSplitterWindow( this, ID_MAIN_SPLITTER, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_verticalSplitter->Connect( wxEVT_IDLE, wxIdleEventHandler( STISpectrumDlg::m_verticalSplitterOnIdle ), NULL, this );

	m_upperPanel = new wxPanel( m_verticalSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* plotSizer;
	plotSizer = new wxBoxSizer( wxVERTICAL );

	m_spectrumPlot = new Aurora::OctaveBandsPlot(m_upperPanel, ID_LEFT_PLOT);
	plotSizer->Add( m_spectrumPlot, 1, wxALL|wxEXPAND, 5 );


	m_upperPanel->SetSizer( plotSizer );
	m_upperPanel->Layout();
	plotSizer->Fit( m_upperPanel );
	m_lowerPanel = new wxPanel( m_verticalSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* lowerPanelSizer;
	lowerPanelSizer = new wxBoxSizer( wxVERTICAL );

	m_tablePanel = new wxPanel( m_lowerPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* tableSizer;
	tableSizer = new wxBoxSizer( wxVERTICAL );

	m_spectrumTable = new wxGrid( m_tablePanel, ID_SPECTRUM_GRID, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_spectrumTable->CreateGrid( 3, 13 );
	m_spectrumTable->EnableEditing( true );
	m_spectrumTable->EnableGridLines( true );
	m_spectrumTable->EnableDragGridSize( false );
	m_spectrumTable->SetMargins( 0, 0 );

	// Columns
	m_spectrumTable->SetColSize( 0, 50 );
	m_spectrumTable->SetColSize( 1, 50 );
	m_spectrumTable->SetColSize( 2, 50 );
	m_spectrumTable->SetColSize( 3, 50 );
	m_spectrumTable->SetColSize( 4, 50 );
	m_spectrumTable->SetColSize( 5, 50 );
	m_spectrumTable->SetColSize( 6, 50 );
	m_spectrumTable->SetColSize( 7, 50 );
	m_spectrumTable->SetColSize( 8, 50 );
	m_spectrumTable->SetColSize( 9, 50 );
	m_spectrumTable->SetColSize( 10, 50 );
	m_spectrumTable->SetColSize( 11, 50 );
	m_spectrumTable->SetColSize( 12, 50 );
	m_spectrumTable->EnableDragColMove( false );
	m_spectrumTable->EnableDragColSize( true );
	m_spectrumTable->SetColLabelSize( 30 );
	m_spectrumTable->SetColLabelValue( 0, wxT(" ") );
	m_spectrumTable->SetColLabelValue( 1, wxT("31.5") );
	m_spectrumTable->SetColLabelValue( 2, wxT("63") );
	m_spectrumTable->SetColLabelValue( 3, wxT("125") );
	m_spectrumTable->SetColLabelValue( 4, wxT("250") );
	m_spectrumTable->SetColLabelValue( 5, wxT("500") );
	m_spectrumTable->SetColLabelValue( 6, wxT("1k") );
	m_spectrumTable->SetColLabelValue( 7, wxT("2k") );
	m_spectrumTable->SetColLabelValue( 8, wxT("4k") );
	m_spectrumTable->SetColLabelValue( 9, wxT("8k") );
	m_spectrumTable->SetColLabelValue( 10, wxT("16k") );
	m_spectrumTable->SetColLabelValue( 11, wxT("A") );
	m_spectrumTable->SetColLabelValue( 12, wxT("Lin") );
	m_spectrumTable->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_spectrumTable->EnableDragRowSize( true );
	m_spectrumTable->SetRowLabelSize( 120 );
	m_spectrumTable->SetRowLabelValue( 0, wxT("Background Noise") );
	m_spectrumTable->SetRowLabelValue( 1, wxT("Signal") );
	m_spectrumTable->SetRowLabelValue( 2, wxT("Signal + Noise") );
	m_spectrumTable->SetRowLabelAlignment( wxALIGN_LEFT, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_spectrumTable->SetDefaultCellBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	m_spectrumTable->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_TOP );
	tableSizer->Add( m_spectrumTable, 1, wxALL|wxEXPAND, 5 );


	m_tablePanel->SetSizer( tableSizer );
	m_tablePanel->Layout();
	tableSizer->Fit( m_tablePanel );
	lowerPanelSizer->Add( m_tablePanel, 1, wxALL|wxEXPAND, 5 );


	m_lowerPanel->SetSizer( lowerPanelSizer );
	m_lowerPanel->Layout();
	lowerPanelSizer->Fit( m_lowerPanel );
	m_verticalSplitter->SplitHorizontally( m_upperPanel, m_lowerPanel, 560 );
	mainSizer->Add( m_verticalSplitter, 1, wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();

	// Connect Events
	m_channelSelectChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISpectrumDlg::OnChannelSelect ), NULL, this );
	m_spectrumTable->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( STISpectrumDlg::OnLevelTableLabelClick ), NULL, this );
}

STISpectrumDlg::~STISpectrumDlg()
{
	// Disconnect Events
	m_channelSelectChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( STISpectrumDlg::OnChannelSelect ), NULL, this );
	m_spectrumTable->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( STISpectrumDlg::OnLevelTableLabelClick ), NULL, this );

}

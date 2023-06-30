///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <aurora.h>

#include "ConvolverDialogs.h"

#include "../res/down.xpm"
#include "../res/left.xpm"
#include "../res/right.xpm"
#include "../res/up.xpm"

///////////////////////////////////////////////////////////////////////////

AFTrackSelectorDlg::AFTrackSelectorDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND | wxALL, 5 );

	m_stepOnePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* stepOneSizer;
	stepOneSizer = new wxBoxSizer( wxHORIZONTAL );

	m_stepOneLabel = new wxStaticText( m_stepOnePanel, wxID_ANY, wxT("Step 1: Select Tracks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stepOneLabel->Wrap( -1 );
	stepOneSizer->Add( m_stepOneLabel, 0, wxALL, 5 );


	m_stepOnePanel->SetSizer( stepOneSizer );
	m_stepOnePanel->Layout();
	stepOneSizer->Fit( m_stepOnePanel );
	mainSizer->Add( m_stepOnePanel, 0, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_stepOneLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_stepOneLine, 0, wxEXPAND | wxALL, 5 );

	m_trackListPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* trackListSizer;
	trackListSizer = new wxBoxSizer( wxVERTICAL );

	m_selectedTracksLabel = new wxStaticText( m_trackListPanel, wxID_ANY, wxT("Selected tracks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_selectedTracksLabel->Wrap( -1 );
	trackListSizer->Add( m_selectedTracksLabel, 0, wxALL, 5 );

	mAvailableList = new wxListCtrl( m_trackListPanel, ID_AVAIL_TRACKS_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxBORDER_RAISED );
	mAvailableList->SetMinSize( wxSize( -1,150 ) );

	trackListSizer->Add( mAvailableList, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* trackListButtonsSizer;
	trackListButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_toAudioDataButton = new wxButton( m_trackListPanel, ID_TO_AUDIO_DATA_BTN, wxT("To Audio Data"), wxDefaultPosition, wxSize( 130,-1 ), 0 );
	m_toAudioDataButton->SetToolTip( wxT("Move selected track(s) to Audio Data list.") );

	trackListButtonsSizer->Add( m_toAudioDataButton, 0, wxALL, 5 );


	trackListButtonsSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_toFiltersButton = new wxButton( m_trackListPanel, ID_TO_IRs_BTN, wxT("To Filters"), wxDefaultPosition, wxSize( 130,-1 ), 0 );
	m_toFiltersButton->SetToolTip( wxT("Move selected track(s) to Filters list.") );

	trackListButtonsSizer->Add( m_toFiltersButton, 0, wxALL, 5 );


	trackListSizer->Add( trackListButtonsSizer, 0, wxEXPAND, 5 );


	m_trackListPanel->SetSizer( trackListSizer );
	m_trackListPanel->Layout();
	trackListSizer->Fit( m_trackListPanel );
	mainSizer->Add( m_trackListPanel, 1, wxALL|wxEXPAND, 5 );

	m_splittedListsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* splittedListsSizer;
	splittedListsSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* audioDataSizer;
	audioDataSizer = new wxBoxSizer( wxVERTICAL );

	m_audioDataLabel = new wxStaticText( m_splittedListsPanel, wxID_ANY, wxT("Audio Datas:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_audioDataLabel->Wrap( -1 );
	audioDataSizer->Add( m_audioDataLabel, 0, wxALL, 5 );

	mAudioDataList = new wxListCtrl( m_splittedListsPanel, ID_AUDIO_DATA_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxBORDER_RAISED );
	mAudioDataList->SetMinSize( wxSize( 120,150 ) );

	audioDataSizer->Add( mAudioDataList, 0, wxALL, 5 );


	splittedListsSizer->Add( audioDataSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* crossButtonsSizer;
	crossButtonsSizer = new wxBoxSizer( wxVERTICAL );

	m_upButton = new wxBitmapButton( m_splittedListsPanel, ID_MOVE_UP_BTN, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), wxBU_AUTODRAW|0 );

	m_upButton->SetBitmap( wxBitmap( up_xpm ) );
	m_upButton->SetToolTip( wxT("Move selected track up in the list.") );

	crossButtonsSizer->Add( m_upButton, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	wxBoxSizer* leftRightButtonsSizer;
	leftRightButtonsSizer = new wxBoxSizer( wxHORIZONTAL );

	m_leftButton = new wxBitmapButton( m_splittedListsPanel, ID_TO_LEFT_BTN, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), wxBU_AUTODRAW|0 );

	m_leftButton->SetBitmap( wxBitmap( left_xpm ) );
	m_leftButton->SetToolTip( wxT("Move selected track(s) from Filters to Audio Datas list.") );

	leftRightButtonsSizer->Add( m_leftButton, 0, wxLEFT, 5 );


	leftRightButtonsSizer->Add( 30, 0, 0, 0, 5 );

	m_rightButton = new wxBitmapButton( m_splittedListsPanel, ID_TO_RIGHT_BTN, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), wxBU_AUTODRAW|0 );

	m_rightButton->SetBitmap( wxBitmap( right_xpm ) );
	m_rightButton->SetToolTip( wxT("Move selected track(s) from Audio Datas to Filters list.") );

	leftRightButtonsSizer->Add( m_rightButton, 0, wxRIGHT, 5 );


	crossButtonsSizer->Add( leftRightButtonsSizer, 0, 0, 5 );

	m_bottomButton = new wxBitmapButton( m_splittedListsPanel, ID_MOVE_DOWN_BTN, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), wxBU_AUTODRAW|0 );

	m_bottomButton->SetBitmap( wxBitmap( down_xpm ) );
	m_bottomButton->SetToolTip( wxT("Move selected track down in the list.") );

	crossButtonsSizer->Add( m_bottomButton, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	crossButtonsSizer->Add( 0, 10, 0, 0, 5 );

	m_removeButton = new wxButton( m_splittedListsPanel, ID_REMOVE_BTN, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0 );
	m_removeButton->SetToolTip( wxT("Remove selected track(s) from list.") );

	crossButtonsSizer->Add( m_removeButton, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	splittedListsSizer->Add( crossButtonsSizer, 0, wxALIGN_BOTTOM, 5 );

	wxBoxSizer* filtersSizer;
	filtersSizer = new wxBoxSizer( wxVERTICAL );

	m_filtersLabel = new wxStaticText( m_splittedListsPanel, wxID_ANY, wxT("Filters (IRs):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_filtersLabel->Wrap( -1 );
	filtersSizer->Add( m_filtersLabel, 0, wxALL, 5 );

	mFilterList = new wxListCtrl( m_splittedListsPanel, ID_IR_LIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST|wxBORDER_RAISED );
	mFilterList->SetMinSize( wxSize( 120,150 ) );

	filtersSizer->Add( mFilterList, 0, wxALL, 5 );


	splittedListsSizer->Add( filtersSizer, 1, wxEXPAND, 5 );


	m_splittedListsPanel->SetSizer( splittedListsSizer );
	m_splittedListsPanel->Layout();
	splittedListsSizer->Fit( m_splittedListsPanel );
	mainSizer->Add( m_splittedListsPanel, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_checkBoxPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* checkBoxSizer;
	checkBoxSizer = new wxBoxSizer( wxVERTICAL );

	mMatrixModeCheck = new wxCheckBox( m_checkBoxPanel, ID_MATRIX_MODE_CHECK, wxT("Matrix mode"), wxDefaultPosition, wxDefaultSize, 0 );
	mMatrixModeCheck->Enable( false );
	mMatrixModeCheck->SetToolTip( wxT("When there are M audio data vectors and N filters, with M != N, the matrix convolution product is automatically selected. \nOtherwise, when M = N, a \"row to row\" convolution product is performed, with N output tracks. If this control is checked, then the matrix convolution product is forced. Pay attention: filters tracks must be in \"matrix\" form, so they will be splitted in M \"columns\".") );

	checkBoxSizer->Add( mMatrixModeCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );


	m_checkBoxPanel->SetSizer( checkBoxSizer );
	m_checkBoxPanel->Layout();
	checkBoxSizer->Fit( m_checkBoxPanel );
	mainSizer->Add( m_checkBoxPanel, 0, wxALL|wxEXPAND, 5 );

	m_outputDescriptionPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* outputBSizer;
	outputBSizer = new wxStaticBoxSizer( new wxStaticBox( m_outputDescriptionPanel, wxID_ANY, wxT("Output:") ), wxVERTICAL );

	m_outputDescriptionLabel = new wxStaticText( outputBSizer->GetStaticBox(), wxID_ANY, wxT("M one to one convolution products"), wxDefaultPosition, wxDefaultSize, 0 );
	m_outputDescriptionLabel->Wrap( -1 );
	m_outputDescriptionLabel->SetMinSize( wxSize( -1,60 ) );

	outputBSizer->Add( m_outputDescriptionLabel, 0, wxALL|wxEXPAND, 5 );


	m_outputDescriptionPanel->SetSizer( outputBSizer );
	m_outputDescriptionPanel->Layout();
	outputBSizer->Fit( m_outputDescriptionPanel );
	mainSizer->Add( m_outputDescriptionPanel, 0, wxALL|wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bottomSizer;
	bottomSizer = new wxBoxSizer( wxVERTICAL );

	m_buttonsPanel = new wxPanel( m_bottomPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* buttonsSizer;
	buttonsSizer = new wxBoxSizer( wxHORIZONTAL );

	mHelpBtn = new wxButton( m_buttonsPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( mHelpBtn, 0, wxALL, 5 );


	buttonsSizer->Add( 0, 0, 1, 0, 5 );

	mCancelBtn = new wxButton( m_buttonsPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonsSizer->Add( mCancelBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mOkBtn = new wxButton( m_buttonsPanel, wxID_OK, wxT("Continue"), wxDefaultPosition, wxDefaultSize, 0 );

	mOkBtn->SetDefault();
	mOkBtn->SetToolTip( wxT("Continue to convolution setup dialog.") );

	buttonsSizer->Add( mOkBtn, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_buttonsPanel->SetSizer( buttonsSizer );
	m_buttonsPanel->Layout();
	buttonsSizer->Fit( m_buttonsPanel );
	bottomSizer->Add( m_buttonsPanel, 0, wxALL|wxEXPAND, 5 );


	m_bottomPanel->SetSizer( bottomSizer );
	m_bottomPanel->Layout();
	bottomSizer->Fit( m_bottomPanel );
	mainSizer->Add( m_bottomPanel, 0, wxEXPAND, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	mAvailableList->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( AFTrackSelectorDlg::OnAvailableDragInit ), NULL, this );
	m_toAudioDataButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::ToAudioDataBtn ), NULL, this );
	m_toFiltersButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnToIRsBtn ), NULL, this );
	mAudioDataList->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( AFTrackSelectorDlg::OnAudioDataDragInit ), NULL, this );
	m_upButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnMoveUpBtn ), NULL, this );
	m_leftButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnToLeftBtn ), NULL, this );
	m_rightButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnToRightBtn ), NULL, this );
	m_bottomButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnMoveDownBtn ), NULL, this );
	m_removeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnRemoveBtn ), NULL, this );
	mFilterList->Connect( wxEVT_COMMAND_LIST_BEGIN_DRAG, wxListEventHandler( AFTrackSelectorDlg::OnFilterDragInit ), NULL, this );
	mMatrixModeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnMatrixModeCheck ), NULL, this );
	mHelpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnHelp ), NULL, this );
	mCancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnCancel ), NULL, this );
	mOkBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFTrackSelectorDlg::OnOK ), NULL, this );
}

AFTrackSelectorDlg::~AFTrackSelectorDlg()
{
}

AFConvolverDlg::AFConvolverDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	m_logoPanel = new wxPanel( this, ID_LOGO_PANEL, wxDefaultPosition, wxSize( -1,54 ), wxTAB_TRAVERSAL );
	mainSizer->Add( m_logoPanel, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	m_topLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_topLine, 0, wxEXPAND|wxALL, 5 );

	m_stepTwoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* stepTwoSizer;
	stepTwoSizer = new wxBoxSizer( wxHORIZONTAL );

	m_stepTwoLabel = new wxStaticText( m_stepTwoPanel, wxID_ANY, wxT("Step 2: setup convolution"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stepTwoLabel->Wrap( -1 );
	stepTwoSizer->Add( m_stepTwoLabel, 0, wxALL, 5 );


	m_stepTwoPanel->SetSizer( stepTwoSizer );
	m_stepTwoPanel->Layout();
	stepTwoSizer->Fit( m_stepTwoPanel );
	mainSizer->Add( m_stepTwoPanel, 1, wxEXPAND|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_stepTwoLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_stepTwoLine, 0, wxALL|wxEXPAND, 5 );

	m_plotPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* plotSizer;
	plotSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* plotStaticSizer;
	plotStaticSizer = new wxStaticBoxSizer( new wxStaticBox( m_plotPanel, wxID_ANY, wxT("Impulse Response(s) RMS") ), wxVERTICAL );

	mPlot = new Aurora::ConvolverPlot((wxWindow *)m_plotPanel, (wxWindowID)ID_PLOT);
	mPlot->SetMinSize( wxSize( 450,200 ) );

	plotStaticSizer->Add( mPlot, 1, wxALL|wxEXPAND, 5 );


	plotSizer->Add( plotStaticSizer, 1, wxALL|wxEXPAND, 5 );


	m_plotPanel->SetSizer( plotSizer );
	m_plotPanel->Layout();
	plotSizer->Fit( m_plotPanel );
	mainSizer->Add( m_plotPanel, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	m_optionsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* optionsSizer;
	optionsSizer = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* autorangeSizer;
	autorangeSizer = new wxStaticBoxSizer( new wxStaticBox( m_optionsPanel, wxID_ANY, wxT("Autorange") ), wxVERTICAL );

	mFullAutorangeCheck = new wxCheckBox( autorangeSizer->GetStaticBox(), ID_FULL_AUTORANGE_CHECK, wxT("Full Autorange - Remove DC (2 passes)"), wxDefaultPosition, wxDefaultSize, 0 );
	mFullAutorangeCheck->SetValue(true);
	autorangeSizer->Add( mFullAutorangeCheck, 0, wxALL, 5 );

	mFBAutorangeCheck = new wxCheckBox( autorangeSizer->GetStaticBox(), ID_FB_AUTORANGE, wxT("First Block autorange"), wxDefaultPosition, wxDefaultSize, 0 );
	autorangeSizer->Add( mFBAutorangeCheck, 0, wxALL, 5 );

	wxBoxSizer* gainSizer;
	gainSizer = new wxBoxSizer( wxHORIZONTAL );


	gainSizer->Add( 5, 0, 0, 0, 5 );

	mGainStext = new wxStaticText( autorangeSizer->GetStaticBox(), ID_GAIN_LABEL_ST, wxT("Gain:"), wxDefaultPosition, wxDefaultSize, 0 );
	mGainStext->Wrap( -1 );
	gainSizer->Add( mGainStext, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mGainText = new wxTextCtrl( autorangeSizer->GetStaticBox(), ID_GAIN_TEXT, wxT("0.00"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	mGainText->SetMinSize( wxSize( 80,25 ) );

	gainSizer->Add( mGainText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	mdBStext = new wxStaticText( autorangeSizer->GetStaticBox(), wxID_ANY, wxT("dB"), wxDefaultPosition, wxDefaultSize, 0 );
	mdBStext->Wrap( -1 );
	gainSizer->Add( mdBStext, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	autorangeSizer->Add( gainSizer, 1, wxEXPAND, 5 );


	optionsSizer->Add( autorangeSizer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* advancedSizer;
	advancedSizer = new wxStaticBoxSizer( new wxStaticBox( m_optionsPanel, wxID_ANY, wxT("Advanced") ), wxVERTICAL );

	mTimeReversedCheck = new wxCheckBox( advancedSizer->GetStaticBox(), ID_TREV_IR_CHECK, wxT("TimeReversed Imp. Resp."), wxDefaultPosition, wxDefaultSize, 0 );
	advancedSizer->Add( mTimeReversedCheck, 0, wxALL, 5 );

	mCrossTalkCheck = new wxCheckBox( advancedSizer->GetStaticBox(), ID_CROSSTALK_CHECK, wxT("Crosstalk Cancel Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	advancedSizer->Add( mCrossTalkCheck, 0, wxALL, 5 );

	mPreserveLengthCheck = new wxCheckBox( advancedSizer->GetStaticBox(), ID_PRES_LENGTH_CHECK, wxT("Preserve Length"), wxDefaultPosition, wxDefaultSize, 0 );
	advancedSizer->Add( mPreserveLengthCheck, 0, wxALL, 5 );


	optionsSizer->Add( advancedSizer, 0, wxALL|wxEXPAND, 5 );


	m_optionsPanel->SetSizer( optionsSizer );
	m_optionsPanel->Layout();
	optionsSizer->Fit( m_optionsPanel );
	mainSizer->Add( m_optionsPanel, 0, wxALL|wxEXPAND, 5 );

	m_infoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* infoSizer;
	infoSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* infoStaticSizer;
	infoStaticSizer = new wxStaticBoxSizer( new wxStaticBox( m_infoPanel, wxID_ANY, wxT("Info") ), wxVERTICAL );

	wxFlexGridSizer* infoFlexGridSizer;
	infoFlexGridSizer = new wxFlexGridSizer( 4, 2, 0, 0 );
	infoFlexGridSizer->SetFlexibleDirection( wxBOTH );
	infoFlexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_audioDataLabel = new wxStaticText( infoStaticSizer->GetStaticBox(), wxID_ANY, wxT("Audio Data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_audioDataLabel->Wrap( -1 );
	infoFlexGridSizer->Add( m_audioDataLabel, 0, wxALL, 5 );

	mAudioDataStext = new wxStaticText( infoStaticSizer->GetStaticBox(), ID_AUDIO_DATA_STEXT, wxT("44100 Hz/16 bits/Mono/100000 Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	mAudioDataStext->Wrap( -1 );
	infoFlexGridSizer->Add( mAudioDataStext, 0, wxALL, 5 );

	m_irLabel = new wxStaticText( infoStaticSizer->GetStaticBox(), wxID_ANY, wxT("Impulse Resp.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_irLabel->Wrap( -1 );
	infoFlexGridSizer->Add( m_irLabel, 0, wxALL, 5 );

	mFilterStext = new wxStaticText( infoStaticSizer->GetStaticBox(), ID_IR_DATA_STEXT, wxT("44100 Hz/16 bits/Mono/100000 Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	mFilterStext->Wrap( -1 );
	infoFlexGridSizer->Add( mFilterStext, 0, wxALL, 5 );

	m_fftSizeLabel = new wxStaticText( infoStaticSizer->GetStaticBox(), wxID_ANY, wxT("FFT Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_fftSizeLabel->Wrap( -1 );
	infoFlexGridSizer->Add( m_fftSizeLabel, 0, wxALL, 5 );

	mFftSizeStext = new wxStaticText( infoStaticSizer->GetStaticBox(), ID_FFT_SIZE_STEXT, wxT("65535 Samples"), wxDefaultPosition, wxDefaultSize, 0 );
	mFftSizeStext->Wrap( -1 );
	infoFlexGridSizer->Add( mFftSizeStext, 0, wxALL, 5 );

	m_modeLabel = new wxStaticText( infoStaticSizer->GetStaticBox(), wxID_ANY, wxT("Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_modeLabel->Wrap( -1 );
	infoFlexGridSizer->Add( m_modeLabel, 0, wxALL, 5 );

	mModeStext = new wxStaticText( infoStaticSizer->GetStaticBox(), ID_MODE_STEXT, wxT("Matrix Convolution Product"), wxDefaultPosition, wxDefaultSize, 0 );
	mModeStext->Wrap( -1 );
	infoFlexGridSizer->Add( mModeStext, 0, wxALL, 5 );


	infoStaticSizer->Add( infoFlexGridSizer, 1, wxEXPAND, 5 );


	infoSizer->Add( infoStaticSizer, 0, wxALL|wxEXPAND, 5 );


	m_infoPanel->SetSizer( infoSizer );
	m_infoPanel->Layout();
	infoSizer->Fit( m_infoPanel );
	mainSizer->Add( m_infoPanel, 0, wxALL|wxEXPAND, 5 );

	m_bottomLine = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainSizer->Add( m_bottomLine, 0, wxEXPAND | wxALL, 5 );

	m_bottomPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bottomSizer;
	bottomSizer = new wxBoxSizer( wxHORIZONTAL );

	mHelpBtn = new wxButton( m_bottomPanel, wxID_HELP, wxT("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	bottomSizer->Add( mHelpBtn, 0, wxALL, 5 );


	bottomSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	mCancelBtn = new wxButton( m_bottomPanel, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bottomSizer->Add( mCancelBtn, 0, wxALL, 5 );

	mOkBtn = new wxButton( m_bottomPanel, wxID_OK, wxT("Calculate!"), wxDefaultPosition, wxDefaultSize, 0 );

	mOkBtn->SetDefault();
	bottomSizer->Add( mOkBtn, 0, wxALL, 5 );


	m_bottomPanel->SetSizer( bottomSizer );
	m_bottomPanel->Layout();
	bottomSizer->Fit( m_bottomPanel );
	mainSizer->Add( m_bottomPanel, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );

	// Connect Events
	mFullAutorangeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnFullAutorangeCheck ), NULL, this );
	mFBAutorangeCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnFirstBlockAutorangeCheck ), NULL, this );
	mGainText->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( AFConvolverDlg::OnGainText ), NULL, this );
	mTimeReversedCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnTimeReverseIrCheck ), NULL, this );
	mCrossTalkCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnCrosstalkCancelCheck ), NULL, this );
	mPreserveLengthCheck->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnPreserveLengthCheck ), NULL, this );
	mHelpBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnHelp ), NULL, this );
	mCancelBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnCancel ), NULL, this );
	mOkBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AFConvolverDlg::OnOk ), NULL, this );
}

AFConvolverDlg::~AFConvolverDlg()
{
}

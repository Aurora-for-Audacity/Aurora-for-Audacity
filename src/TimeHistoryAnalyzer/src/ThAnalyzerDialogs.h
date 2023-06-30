///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.9.0 Jan 22 2020)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
namespace Aurora{ class ThAnalyzerNotebook; }

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include "ThAnalyzerPlot.h"
#include <wx/scrolwin.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class THADlg
///////////////////////////////////////////////////////////////////////////////
class THADlg : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_SETUP_PAGE = 1000,
			ID_FULLSCALE_GRID,
			ID_RDC_CHECK,
			ID_CALIBRATION_PAGE,
			ID_CALIBRATION_GRID,
			ID_CALIB_APPLY_TO_ALL,
			ID_CALIBRATE_BTN
		};

		wxNotebook* m_pNotebook;
		wxPanel* m_setupPanel;
		wxGrid* m_setupGrid;
		wxCheckBox* m_removeDcCheck;
		wxPanel* m_calibrationPanel;
		wxGrid* m_calibrationGrid;
		wxCheckBox* m_ch1ForAllCheck;
		wxButton* m_calibrateButton;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpButton;
		wxButton* m_cancelButton;
		wxButton* m_analyzeButton;

		// Virtual event handlers, overide them in your derived class
		virtual void OnFullscaleGridContentChanged( wxGridEvent& event ) { event.Skip(); }
		virtual void OnRemoveDCCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibrationGridContentChanged( wxGridEvent& event ) { event.Skip(); }
		virtual void OnApplyToAllCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibrate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		THADlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Time History Analysis"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~THADlg();

};

///////////////////////////////////////////////////////////////////////////////
/// Class THAShowDlg
///////////////////////////////////////////////////////////////////////////////
class THAShowDlg : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_DISPLAY_CHOICE = 1000,
			ID_PLOT,
			ID_GRID_PANEL,
			ID_PARAM_GRID,
			ID_ADD_TRACKS_BTN
		};

		wxSplitterWindow* m_splitter;
		wxPanel* m_leftPanel;
		wxStaticText* m_plotTypeLabel;
		wxChoice* m_plotTypeChoice;
		wxPanel* m_plotPanel;
		wxScrolledWindow* m_plotScroller;
		Aurora::ThAnalyzerPlot *m_pPlot;
		wxPanel* m_rightPanel;
		wxPanel* m_gridPanel;
		wxGrid* m_resultsGrid;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpButton;
		wxButton* m_addTracksButton;
		wxButton* m_closeButton;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDlg( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSelectDisplay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGridCellRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnTogglePlot( wxGridEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddTracksBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		THAShowDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Time History Analyzer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxFULL_REPAINT_ON_RESIZE );
		~THAShowDlg();

		void m_splitterOnIdle( wxIdleEvent& )
		{
		m_splitter->SetSashPosition( 0 );
		m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( THAShowDlg::m_splitterOnIdle ), NULL, this );
		}

};

///////////////////////////////////////////////////////////////////////////////
/// Class THADialog
///////////////////////////////////////////////////////////////////////////////
class THADialog : public wxDialog
{
	private:

	protected:
		enum
		{
			ID_LOGO_PANEL = 1000,
			ID_HORZ_SPLITTER,
			ID_SETUP_NOTEBOOK,
			ID_SETUP_PAGE,
			ID_FULLSCALE_GRID,
			ID_ANALYZE_BTN,
			ID_RDC_CHECK,
			ID_CALIBRATION_PAGE,
			ID_CALIBRATION_GRID,
			ID_CALIBRATE_BTN,
			ID_CALIB_APPLY_TO_ALL,
			ID_DISPLAY_CHOICE,
			ID_PLOT,
			ID_GRID_PANEL,
			ID_PARAM_GRID,
			ID_ADD_TRACKS_BTN
		};

		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxSplitterWindow* m_pHorzSplitter;
		wxPanel* m_upperSplitterPanel;
		Aurora::ThAnalyzerNotebook* m_pNotebook;
		wxPanel* m_setupPage;
		wxGrid* m_fullscalesGrid;
		wxButton* m_analyzeButton;
		wxCheckBox* m_removeDcCheck;
		wxPanel* m_calibrationPage;
		wxGrid* m_calibrationGrid;
		wxButton* m_calibrateButton;
		wxCheckBox* m_ch1ForAllCheck;
		wxPanel* m_lowerSplitterPanel;
		wxSplitterWindow* m_pVertSplitter;
		wxPanel* m_leftSplitterPanel;
		wxScrolledWindow* m_plotScroller;
		wxStaticText* m_plotTypeLabel;
		wxChoice* m_plotTypeChoice;
		wxPanel* m_plotPanel;
		Aurora::ThAnalyzerPlot *m_pPlot;
		wxPanel* m_rightSplitterPanel;
		wxPanel* m_resultsGridPanel;
		wxGrid* m_resultsGrid;
		wxStaticLine* m_bottomLine;
		wxPanel* m_buttonsPanel;
		wxButton* m_helpButton;
		wxButton* m_addTracksButton;
		wxButton* m_closeButton;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseDialog( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnFullscaleGridContentChanged( wxGridEvent& event ) { event.Skip(); }
		virtual void OnAnalyze( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveDCCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCalibrationGridContentChanged( wxGridEvent& event ) { event.Skip(); }
		virtual void OnCalibrate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnApplyToAllCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSelectDisplay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGridCellRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnTogglePlot( wxGridEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddTracksBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }


	public:

		THADialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Time History Analyzer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~THADialog();

		void m_pHorzSplitterOnIdle( wxIdleEvent& )
		{
		m_pHorzSplitter->SetSashPosition( 200 );
		m_pHorzSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( THADialog::m_pHorzSplitterOnIdle ), NULL, this );
		}

		void m_pVertSplitterOnIdle( wxIdleEvent& )
		{
		m_pVertSplitter->SetSashPosition( 0 );
		m_pVertSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( THADialog::m_pVertSplitterOnIdle ), NULL, this );
		}

};


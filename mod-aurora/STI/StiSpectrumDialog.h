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
#include "StiPlot.h"
#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_SPECTRUM_DLG 1000
#define ID_MAIN_SPLITTER 1001
#define ID_LEFT_PLOT 1002
#define ID_SPECTRUM_GRID 1003

///////////////////////////////////////////////////////////////////////////////
/// Class STISpectrumDlg
///////////////////////////////////////////////////////////////////////////////
class STISpectrumDlg : public wxDialog
{
	private:

	protected:
		wxPanel* m_logoPanel;
		wxStaticLine* m_topLine;
		wxPanel* m_topPanel;
		wxStaticText* m_channelSelectLabel;
		wxChoice* m_channelSelectChoice;
		wxSplitterWindow* m_verticalSplitter;
		wxPanel* m_upperPanel;
		Aurora::OctaveBandsPlot* m_spectrumPlot;
		wxPanel* m_lowerPanel;
		wxPanel* m_tablePanel;
		wxGrid* m_spectrumTable;

		// Virtual event handlers, overide them in your derived class
		virtual void OnChannelSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLevelTableLabelClick( wxGridEvent& event ) { event.Skip(); }


	public:

		STISpectrumDlg( wxWindow* parent, wxWindowID id = ID_SPECTRUM_DLG, const wxString& title = wxT("Octave Band Analysis"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,800 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~STISpectrumDlg();

		void m_verticalSplitterOnIdle( wxIdleEvent& )
		{
		m_verticalSplitter->SetSashPosition( 560 );
		m_verticalSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( STISpectrumDlg::m_verticalSplitterOnIdle ), NULL, this );
		}

};


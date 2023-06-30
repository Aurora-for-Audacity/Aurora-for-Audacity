/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Generator

  SineSweepUi.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::SineSweepGeneratorDialog 
\brief Dialog used with EffectAurora::SineSweepGenerator.

*//*******************************************************************/
#include <aurora.h>

#include "SineSweepEffect.h"
#include "SineSweepDialogs.h"

#include "SineSweepUi.h"

//----------------------------------------------------------------------------
// Aurora::SineSweepGeneratorDialog
//----------------------------------------------------------------------------

void Aurora::SineSweepGeneratorDialog::OnLinearSweepRadio( wxCommandEvent& event )
{
   m_pSsg->SetSweepType(Aurora::SineSweepGenerator::ST_LINEAR);
}

void Aurora::SineSweepGeneratorDialog::OnLogSweepRadio( wxCommandEvent& event )
{
   m_pSsg->SetSweepType(Aurora::SineSweepGenerator::ST_LOG);
}

void Aurora::SineSweepGeneratorDialog::OnPinkSweepRadio( wxCommandEvent& event )
{
   m_pSsg->SetSweepType(Aurora::SineSweepGenerator::ST_PINK);
}

void Aurora::SineSweepGeneratorDialog::OnFadeInChoice( wxCommandEvent& event )
{
    m_pSsg->SetFadeInType(event.GetSelection());
}

void Aurora::SineSweepGeneratorDialog::OnFadeOutChoice( wxCommandEvent& event )
{
   m_pSsg->SetFadeOutType(event.GetSelection());
}

void Aurora::SineSweepGeneratorDialog::OnOk( wxCommandEvent& event )
{
	wxWindow*  pWnd = FindWindowById(ID_START_FREQ_TEXT, this);
	wxTextCtrl* pTc = wxDynamicCast(pWnd, wxTextCtrl);
	double  dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetStartFrequency(dbValue);

	pWnd = FindWindowById(ID_END_FREQ_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetEndFrequency(dbValue);

	pWnd = FindWindowById(ID_DURATION_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetSweepDuration(dbValue);

	pWnd = FindWindowById(ID_AMPLITUDE_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetAmplitude(dbValue);

	pWnd = FindWindowById(ID_CHANNELS_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	long lValue = ::TextPtrToLong(pTc);
	m_pSsg->SetSweepChnlsNumber((int)lValue);

	pWnd = FindWindowById(ID_FADE_IN_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetFadeInDuration(dbValue);

	pWnd = FindWindowById(ID_FADE_OUT_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetFadeOutDuration(dbValue);

	pWnd = FindWindowById(ID_SILENCE_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetSilenceDuration(dbValue);

	pWnd = FindWindowById(ID_REPETITIONS_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	lValue = ::TextPtrToLong(pTc);
	m_pSsg->SetRepetitionsNumber((int)lValue);

	pWnd = FindWindowById(ID_DELTA_L_TEXT, this);
	pTc  = wxDynamicCast(pWnd, wxTextCtrl);
	dbValue = ::TextPtrToDouble(pTc);
	m_pSsg->SetDeltaL(dbValue);

	m_pSsg->SetControlPulses(m_pulsesCheck->IsChecked());

    if( m_pSsg->ErrorCheck() != Aurora::SineSweepGenerator::ERR_OK)
    {
    	wxString msg;
    	m_pSsg->GetErrorMessage(msg);
    	Aurora::SineSweepGeneratorEffect::MessageBox(msg, Aurora::MessageType::Error);
		return;
	}

    EndModal(true);
}

void Aurora::SineSweepGeneratorDialog::OnCancel( wxCommandEvent& event )
{
    EndModal(false);
}

void Aurora::SineSweepGeneratorDialog::OnHelp( wxCommandEvent& event )
{
    // TODO: Implement OnHelp
}

Aurora::SineSweepGeneratorDialog::SineSweepGeneratorDialog(Aurora::SineSweepGenerator* pSsg, 
	                                                       wxWindow* pParent)
  : SsgDlg( pParent ),
    m_pSsg(pSsg)
{    
	// Set caption
	wxString title("Aurora for Audacity - Sine Sweep Gen. - (v.");
	title << Aurora::ModuleVersionString() << ")";
    SetTitle(title);

	// Draw logo
	wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

	pLogoSizer->Add( new wxStaticBitmap( m_logoPanel, wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 0 ),
                    0, wxALL, 5 );

	pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	pLogoSizer->Add( new wxStaticBitmap( m_logoPanel, wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("ssg_logo"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 0 ),
                    0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();
}

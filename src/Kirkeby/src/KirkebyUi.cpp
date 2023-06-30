/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  gui.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::KirkebyDialog 
\brief Dialog used with EffectKirkeby.

*//*******************************************************************/

#include <aurora.h>

// From Audacity source tree
#include <ModuleManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>
#include <widgets/ProgressDialog.h>

#include <wx/filename.h>

#include "KirkebyBase.h"
#include "KirkebyDialogs.h"
#include "KirkebyEffect.h"

#include "KirkebyUi.h"

//----------------------------------------------------------------------------
// Aurora::KirkebyDialog implementation
//----------------------------------------------------------------------------

/*void Aurora::KirkebyDialog::BuildFilterLengthList()
{
    wxString wxszValue;
    int nLength = int(m_pKirk->GetInverseFilterLength());
    int nExp = ::AuroraNearTwoPowerExp(double(nLength));
    m_pwxcmbFilterLength->Clear();
    while( nExp < 20)
    {
        wxszValue.Printf(wxT("%d"), int(pow(2.0, double(nExp))));
//        printf("%s\n", (const char*)wxszValue.ToAscii()); // DEBUG
        m_pwxcmbFilterLength->Append(wxszValue);
        nExp++;
    }
    m_pwxcmbFilterLength->SetSelection(0); //The best should be the first...
}
*/
// -------- Events manager
void Aurora::KirkebyDialog::OnMonoFilteringRadio( wxCommandEvent& event )
{
    m_pKirk->SetFilterType(KirkebyEffect::FilterType::Mono);
    m_crossTalkCheck->Disable();
    m_multiIRsCheck->Enable();
    int nCols = 1;

    if(m_multiIRsCheck->IsChecked())
    {
        if (!(nCols = ::TextPtrToInt(m_multiIRText)))
        {
            nCols = 1;
        }
    }
    m_pKirk->SetCols(nCols);    
    
    m_filterLengthText->SetValue(wxString::Format("%d",
                                  (int)m_pKirk->GetColumnLength()));
//    BuildFilterLengthList();
}

void Aurora::KirkebyDialog::OnStereoFilteringRadio( wxCommandEvent& event )
{
    m_pKirk->SetFilterType(KirkebyEffect::FilterType::Stereo);
    m_crossTalkCheck->Enable();
    m_multiIRsCheck->Disable();
    m_pKirk->SetCols(2);        // TODO: check if it is the desired behaviour

    m_filterLengthText->SetValue(wxString::Format("%d",
                                 (int)m_pKirk->GetColumnLength()));
//    BuildFilterLengthList();
}

void Aurora::KirkebyDialog::OnDipoleFilteringRadio( wxCommandEvent& event )
{
    m_pKirk->SetFilterType(KirkebyEffect::FilterType::Dipole);
    m_crossTalkCheck->Enable();
    m_multiIRsCheck->Disable();
    m_pKirk->SetCols(2);

    m_filterLengthText->SetValue(wxString::Format("%d",
                                 (int)m_pKirk->GetColumnLength()));
//    BuildFilterLengthList();
}

void Aurora::KirkebyDialog::OnCrossTalkCheck( wxCommandEvent& event )
{
   m_pKirk->SetCancelCrosstalk(event.IsChecked());
}

void Aurora::KirkebyDialog::OnMultiIRCheck( wxCommandEvent& event )
{
   int nCols;

   if(event.IsChecked())
   {
      m_multiIRNumberLabel->Enable();
      m_multiIRText->Enable();

      if(!(nCols = ::TextPtrToInt(m_multiIRText))) 
      {
        nCols = 1;
      }
      m_pKirk->SetCols(nCols);
   }
   else
   {
      m_multiIRNumberLabel->Disable();
      m_multiIRText->Disable();
   }
}

void Aurora::KirkebyDialog::OnMultiIrText( wxCommandEvent& event )
{
   int nCols = ::TextPtrToInt(m_multiIRText);

   if(nCols <= 0)
   {
       nCols = 1;
       m_multiIRText->SetValue("1");
   }
   m_pKirk->SetCols(nCols);

   m_filterLengthText->SetValue(wxString::Format("%d",
                                 (int)m_pKirk->GetColumnLength()));
   //   BuildFilterLengthList();
}

/*
void Aurora::KirkebyDialog::OnFilterLengthCombo( wxCommandEvent& event )
{
    long lLength;
    event.GetString().ToLong(&lLength);
    if(lLength < m_pKirk->GetInverseFilterLength())
        lLength = m_pKirk->GetInverseFilterLength();
        
    m_pKirk->SetInverseFilterLength(AFSampleCount(lLength));
}

void Aurora::KirkebyDialog::OnFilterLengthText( wxCommandEvent& event )
{
    AFSampleCount smpcLength = 0;
    //smpcLength = AFSampleCount(TextPtrToInt(mFilterLengthText)); //TODO Get text from combo line !!!!!!!!!!!!!!!!!!
    if(smpcLength < m_pKirk->GetInverseFilterLength())
    {
        wxString wxszValue;
        smpcLength = m_pKirk->GetInverseFilterLength();
        wxszValue.Printf(wxT("%d"), int(smpcLength));
        m_pwxcmbFilterLength->SetValue(wxszValue);
    }
    
    m_pKirk->SetInverseFilterLength(smpcLength);
}
*/
void Aurora::KirkebyDialog::OnIRPText( wxCommandEvent& event )
{
   m_pKirk->SetInBandRegularisationParameter(::TextPtrToDouble(m_irpText));
}

void Aurora::KirkebyDialog::OnORPText( wxCommandEvent& event )
{
   m_pKirk->SetOutBandRegularisationParameter(::TextPtrToDouble(m_orpText));
}

void Aurora::KirkebyDialog::OnLowerCutFrqText( wxCommandEvent& event )
{
   m_pKirk->SetLowerCutFrequency(::TextPtrToDouble(m_lowerCutFrqText));
}

void Aurora::KirkebyDialog::OnHigherCutFrqText( wxCommandEvent& event )
{
   m_pKirk->SetHigherCutFrequency(::TextPtrToDouble(m_higherCutFrqText));
}

void Aurora::KirkebyDialog::OnTransitionWidthText( wxCommandEvent& event )
{
   m_pKirk->SetTransitionWidth(::TextPtrToDouble(m_transWidthText));
}

void Aurora::KirkebyDialog::OnFadeInText( wxCommandEvent& event )
{
   m_pKirk->SetFadeInLength(Aurora::SampleCount(::TextPtrToInt(m_fadeInText)));
}

void Aurora::KirkebyDialog::OnFadeOutText( wxCommandEvent& event )
{
    m_pKirk->SetFadeOutLength(Aurora::SampleCount(::TextPtrToInt(m_fadeOutText)));
}

void Aurora::KirkebyDialog::OnAverageModeChoice( wxCommandEvent& event )
{
    const int sel = event.GetSelection();
    const bool onOff = (sel > 0);

    m_atLabel->Enable(onOff);
    m_averageTypeChoice->Enable(onOff);
    m_awLabel->Enable(onOff);
    m_averageWidthText->Enable(onOff);
    m_awUnitsLabel->Enable(onOff);

    m_pKirk->SetAverageMode(sel);
}

void Aurora::KirkebyDialog::OnAverageTypeChoice( wxCommandEvent& event )
{
   const int sel = event.GetSelection();

   switch(sel)
   {
       case 0: m_awUnitsLabel->SetLabel(wxT("Spectral points")); break;
       case 1: m_awUnitsLabel->SetLabel(wxT("Octaves")); break;
   }
   m_pKirk->SetAverageType(sel);
}

void Aurora::KirkebyDialog::OnAverageWidthText( wxCommandEvent& event )
{
   // TODO Has to be converted to spectral point if necessary
   const double w = ::TextPtrToDouble(m_averageWidthText);
   m_pKirk->SetAverageWidth(w);
}

void Aurora::KirkebyDialog::OnAutorangeCheck( wxCommandEvent& event )
{
   if(event.IsChecked())
   {
       m_gainLabel->Disable();
       m_gainText->Disable();
       m_gainUnitsLabel->Disable();
       m_pKirk->SetAutorange(true);
   }
   else
   {
       m_gainLabel->Enable();
       m_gainText->Enable();
       m_gainUnitsLabel->Enable();
       m_pKirk->SetAutorange(false);
   }
}

void Aurora::KirkebyDialog::OnGainText( wxCommandEvent& event )
{
   m_pKirk->SetGaindB(::TextPtrToDouble(m_gainText));
}

void Aurora::KirkebyDialog::OnHelp( wxCommandEvent& event )
{
    wxString path;
    path << "Docs" << TRAILING_SLASH << "kirk_filt.html";
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::KirkebyDialog::OnCancel( wxCommandEvent& event )
{
   EndModal(false);
}

void Aurora::KirkebyDialog::OnOk( wxCommandEvent& event )
{
    const int filterLength = ::TextPtrToInt(m_filterLengthText);
    const int trackLength = m_pKirk->GetSelectionLength();
    
    if(filterLength <= 0 || filterLength < trackLength)
    {
        Aurora::KirkebyEffect::MessageBox("Wrong filter length: must be greater "
                                          "than 0 and always greater than IR",
                                          Aurora::MessageType::Error);
        return;
    }
    m_pKirk->SetInverseFilterLength((Aurora::SampleCount)filterLength);
    EndModal(true);
}

// ---- 'ctors
Aurora::KirkebyDialog::KirkebyDialog(wxWindow* pParent, 
                                     Aurora::KirkebyEffect* pKirk, 
                                     const bool bIsStereo)
  : KirkebyDlg( pParent ), 
    m_pKirk(pKirk)
{
	// Set caption
	wxString title("Aurora for Audacity - Kirkeby Filter Inversion Tool - v.");
	title << Aurora::ModuleVersionString();
    SetTitle(title);

	// Draw logo
	wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

	pLogoSizer->Add( new wxStaticBitmap( m_logoPanel,
                                        wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 0 ),
                    0, wxALL, 5 );

	pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	pLogoSizer->Add( new wxStaticBitmap( m_logoPanel,
                                          wxID_ANY,
                                          Aurora::ArtProvider::GetBitmap("kirk_logo"),
                                          wxDefaultPosition,
                                          wxDefaultSize, 0 ),
                    0, wxALL, 5 );

	m_logoPanel->SetSizer( pLogoSizer );
	m_logoPanel->Layout();

	// Init widgets
    m_monoFilteringButton->SetValue(true);
    
    wxString s;
    s.Printf("%.1f / %d / %d", 
              m_pKirk->GetSamplerate(), 
              m_pKirk->GetRows(), 
              int(m_pKirk->GetRowLength()));
    m_irInfoLabel->SetLabel(s);

   if(m_pKirk->GetChannels() == 2)
   {
        m_stereoFilteringButton->Enable();
        m_dipoleFilteringButton->Enable();
   }

   // Setup filter length combo box
   //BuildFilterLengthList();    
    s.Printf("%d", int(m_pKirk->GetInverseFilterLength()));
    m_filterLengthText->SetValue(s);
    
    if (!bIsStereo)
    {
        m_monoFilteringButton->SetLabel(wxT("Mono (no CrossTalk cancellation)"));
    }
   s.Printf("%.3f", m_pKirk->GetInBandRegularisationParameter());
   m_irpText->SetValue(s);
   s.Printf("%.3f", m_pKirk->GetOutBandRegularisationParameter());
   m_orpText->SetValue(s);
   
   s.Printf("%.1f", m_pKirk->GetLowerCutFrequency());
   m_lowerCutFrqText->SetValue(s);
   s.Printf("%.1f", m_pKirk->GetHigherCutFrequency());
   m_higherCutFrqText->SetValue(s);
   
   s.Printf("%.3f", m_pKirk->GetTransitionWidth());
   m_transWidthText->SetValue(s);
   
   s.Printf("%d", int(m_pKirk->GetFadeInLength()));
   m_fadeInText->SetValue(s);
   s.Printf("%d", int(m_pKirk->GetFadeOutLength()));
   m_fadeOutText->SetValue(s);
   
   const int sel = m_pKirk->GetAverageMode();
   m_averageModeChoice->SetSelection(sel);
   
   if(sel > KirkebyEffect::AverageMode::None)
   {
      m_atLabel->Enable();
      m_averageTypeChoice->Enable();
      m_awLabel->Enable();
      m_averageWidthText->Enable();
      m_awUnitsLabel->Enable();
   }
   m_averageTypeChoice->SetSelection(m_pKirk->GetAverageType());
   s.Printf("%.3f", m_pKirk->GetAverageWidth());
   m_averageWidthText->SetValue(s);

   const bool check = m_pKirk->IsAutorangeSet();
   m_autorangeCheck->SetValue(check);
   
   if(!check)
   {
      m_gainLabel->Enable();
      m_gainText->Enable();
      m_gainUnitsLabel->Enable();
   }
   s.Printf("%.1f", m_pKirk->GetGaindB());
   m_gainText->SetValue(s);   
}

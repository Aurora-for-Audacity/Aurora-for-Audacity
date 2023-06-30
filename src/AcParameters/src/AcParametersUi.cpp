/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  gui.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::AcParametersDialog
\brief Dialog used with Aurora::AcParametersEffect.

*//****************************************************************//**

\class Aurora::AcParametersShowDialog
\brief Window that shows the acoustical parameter.

*//****************************************************************//**

\class Aurora::StoreGValueDialog
\brief The dialog showed when Store G-value button is pressed

*//*******************************************************************/
#include <aurora.h>

// From Audacity source tree
#include <ModuleManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>
#include <widgets/ProgressDialog.h>

#include <audacity/ModuleInterface.h>
#include "AcParametersBase.h"

#include "AcParametersDialogs.h"
#include "AcParametersExports.h"
#include "AcParametersPlot.h"
#include "AcParametersEffect.h"

#include "AcParametersUi.h"

//----------------------------------------------------------------------------
// Aurora::AcParametersDialog
//----------------------------------------------------------------------------
bool Aurora::AcParametersDialog::ChooseFileToAppend()
{
    wxString fn = m_pAp->GetFileToAppend();

    wxFileDialog openFileDialog(this,
                                "Append results to file",
                                "", fn,
                                "*.*", wxFD_SAVE);

    if (openFileDialog.ShowModal() != wxID_OK)
    {
        return false;
    }
    fn = openFileDialog.GetPath();

    if (fn.IsEmpty())
    {
        return false;
    }

    m_pFileNameText->SetValue(fn);
    m_pAp->SetFileToAppend(fn);   
    return true;
}

void Aurora::AcParametersDialog::OnCloseDialog(wxCloseEvent& event)
{
    EndModal(false);
}

    // --- Checkbox events ---
void Aurora::AcParametersDialog::OnNoiseCorrCheck(wxCommandEvent &event) 
{
   m_pAp->EnableNoiseCorrection(event.IsChecked());
}

void Aurora::AcParametersDialog::OnEDTCheck(wxCommandEvent &event) 
{
   m_pAp->EnableEDT(event.IsChecked());
}

void Aurora::AcParametersDialog::OnComputeStageParamCheck(wxCommandEvent &event) 
{
   m_pAp->EnableStageParameters(event.IsChecked());
}

void Aurora::AcParametersDialog::OnAvgModeCheck(wxCommandEvent &event) 
{
   m_pAp->EnableAverageMode(event.IsChecked());
}

void Aurora::AcParametersDialog::OnAppendResultsToFile(wxCommandEvent &event) 
{
   if(event.IsChecked())
   {
       m_pFileNameText->Enable();
       m_pOpenFileButton->Enable();
       
       if (m_pFileNameText->GetValue().IsEmpty() 
           && ! ChooseFileToAppend())
       {
           m_pAppendResultsCheck->SetValue(false);
       }
   }
   else
   {
       m_pFileNameText->Disable();
       m_pOpenFileButton->Disable();
       m_pAp->SetFileToAppend("");
   }
}

// --- TextCtrl events ---
void Aurora::AcParametersDialog::OnEditRevTimeMinExtreme(wxCommandEvent &event) 
{
   double d = 0.0;
   d = ::TextPtrToDouble(m_pUserMinLevelText);
   m_pAp->SetUserMinLevel(d);

}

void Aurora::AcParametersDialog::OnEditRevTimeMaxExtreme(wxCommandEvent &event) 
{
   double d = ::TextPtrToDouble(m_pUserMaxLevelText);
   m_pAp->SetUserMaxLevel(d);
}

void Aurora::AcParametersDialog::OnEditDirSoundTrigValue(wxCommandEvent &event) 
{
   double d = ::TextPtrToDouble(m_pDstText);
   m_pAp->SetDirectSoundTrigValue(d);
}

void Aurora::AcParametersDialog::OnEditPeakSPLValue(wxCommandEvent &event) 
{
   double d = ::TextPtrToDouble(m_pFullScaleText);
   m_pAp->SetFullScale(d);
}

void Aurora::AcParametersDialog::OnEditProbeMicDist(wxCommandEvent &event) 
{
   double d = ::TextPtrToDouble(m_pProbeMicsDistText);
   m_pAp->SetProbeMicsDistance(d / 1000.0); // converted from mm to m.
}

void Aurora::AcParametersDialog::OnEditCValue(wxCommandEvent &event) 
{
   double d = ::TextPtrToDouble(m_pSoundSpeedText);
   m_pAp->SetSoundSpeedValue(d);
}

void Aurora::AcParametersDialog::OnEditAppendFileName(wxCommandEvent &event) 
{
   wxString fn = m_pFileNameText->GetLineText(0);
   m_pAp->SetFileToAppend(fn);
}

// --- Radiobuttons events ---

void Aurora::AcParametersDialog::StereoModeCtl(int mode)
{   
   switch(mode) 
   {    
       case ID_2CH_MODE_INTENSITY_PROBE_RADIO:
          m_pProbeMicsDistLabel->Enable();
          m_pProbeMicsDistText->Enable();
          m_pProbeDistUnitLabel->Enable();
          m_pSoundSpeedLabel->Enable();
          m_pSoundSpeedText->Enable();
          m_pSoundSpeedUnitLabel->Enable();
          m_pIaccLabel->Disable();
          m_pIaccChoice->Disable();
          break;
          
       case ID_2CH_MODE_BINAURAL_RADIO:
          m_pProbeMicsDistLabel->Disable();
          m_pProbeMicsDistText->Disable();
          m_pProbeDistUnitLabel->Disable();
          m_pSoundSpeedLabel->Disable();
          m_pSoundSpeedText->Disable();
          m_pSoundSpeedUnitLabel->Disable();
          m_pIaccLabel->Enable();
          m_pIaccChoice->Enable();
          break;
          
       default:
          m_pProbeMicsDistLabel->Disable();
          m_pProbeMicsDistText->Disable();
          m_pProbeDistUnitLabel->Disable();
          m_pSoundSpeedLabel->Disable();
          m_pSoundSpeedText->Disable();
          m_pSoundSpeedUnitLabel->Disable();
          m_pIaccLabel->Disable();
          m_pIaccChoice->Disable();
          break;
   };
    
}

void Aurora::AcParametersDialog::OnOctaveBandsRadio( wxCommandEvent& event )
{
    m_pAp->SetOctaveFraction(1);
}

void Aurora::AcParametersDialog::OnThirdOctaveBandsRadio( wxCommandEvent& event )
{
    m_pAp->SetOctaveFraction(3);
}

void Aurora::AcParametersDialog::OnOmniMicClick(wxCommandEvent &event) 
{
   m_pAp->SetSource(Aurora::AcousticalParameters::Source::TwoOmniMics);
   StereoModeCtl(ID_2CH_MODE_OMNI_RADIO);
}

void Aurora::AcParametersDialog::OnSoundfieldClick(wxCommandEvent &event) 
{
   m_pAp->SetSource(Aurora::AcousticalParameters::Source::SoundfieldWY);
   StereoModeCtl(ID_2CH_MODE_SOUNDFIELD_RADIO);
}

void Aurora::AcParametersDialog::OnPUProbeClick(wxCommandEvent &event) 
{
   m_pAp->SetSource(Aurora::AcousticalParameters::Source::PuProbe);
   StereoModeCtl(ID_2CH_MODE_PU_PROBE_RADIO);
}

void Aurora::AcParametersDialog::OnIntensityProbeClick(wxCommandEvent &event) 
{
   m_pAp->SetSource(Aurora::AcousticalParameters::Source::IntensityProbe);
   StereoModeCtl(ID_2CH_MODE_INTENSITY_PROBE_RADIO);
}

void Aurora::AcParametersDialog::OnBinauralClick(wxCommandEvent &event) 
{
   m_pAp->SetSource(Aurora::AcousticalParameters::Source::Binaural);
   StereoModeCtl(ID_2CH_MODE_BINAURAL_RADIO);
}

void Aurora::AcParametersDialog::On4ChModeOmniRadio( wxCommandEvent& event )
{
    if (m_pFourOmniRbutton->GetValue())
    {
        m_pAp->SetSource(Aurora::AcousticalParameters::Source::FourOmniMics);
    }
}

void Aurora::AcParametersDialog::On4ChModeSoundfieldWXYZRadio( wxCommandEvent& event )
{
   // Quando selezionato, devono essere mostrati LE LF LG calcolati a partire
   // da W e Y
    if (m_pFuMaRbutton->GetValue())
    {
        m_pAp->SetSource(Aurora::AcousticalParameters::Source::AmbisonicsFuMa);
    }
}

void Aurora::AcParametersDialog::On4ChModeSoundfieldWYXZRadio( wxCommandEvent& event )
{
   // Idem c.s.
    if (m_pAmbixRbutton->GetValue())
    {
        m_pAp->SetSource(Aurora::AcousticalParameters::Source::AmbisonicsAmbix);
    }
}
      
// --- Choice events ---
void Aurora::AcParametersDialog::OnIACCChoice(wxCommandEvent &event) 
{
   switch(event.GetSelection()) 
   {
      case 0: m_pAp->SetIACCSpan(Aurora::AcousticalParameters::IACCSpan::Early); break;
      case 1: m_pAp->SetIACCSpan(Aurora::AcousticalParameters::IACCSpan::Late);  break;
      case 2: m_pAp->SetIACCSpan(Aurora::AcousticalParameters::IACCSpan::All);   break;
   }
}

// --- Buttons events ---
void Aurora::AcParametersDialog::OnOpenFile(wxCommandEvent &event)
{
    if (! ChooseFileToAppend() && m_pFileNameText->GetValue().IsEmpty())
    {
        m_pAppendResultsCheck->SetValue(false);
    }
}

void Aurora::AcParametersDialog::OnOk(wxCommandEvent &event)
{
   EndModal(true);
}

void Aurora::AcParametersDialog::OnCancel(wxCommandEvent &event)
{
   EndModal(false);
}

// --- Ctors ---

Aurora::AcParametersDialog::AcParametersDialog(wxWindow* parent,
                                               Aurora::AcParametersEffect* ap)
  : AcParDlg(parent),
    m_pAp(ap)
{
    // Set caption
    wxString title("Aurora for Audacity - ISO 3382 Acoustical Parameters Setup - v.");
    title << Aurora::ModuleVersionString();
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add(new wxStaticBitmap(m_pLogoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap(wxT("Aurora_logo")),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0), 
                    0, wxALL, 5 );
    pLogoSizer->Add(0, 0, 1, wxEXPAND, 5);
    pLogoSizer->Add(new wxStaticBitmap(m_pLogoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap(wxT("ap_logo")),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 0), 
                    0, wxALL, 5 );

    m_pLogoPanel->SetSizer( pLogoSizer );
    m_pLogoPanel->Layout();

   // Init widgets
   wxString str;
   
   str.Printf("%.1f", m_pAp->GetUserMinLevel());
   m_pUserMinLevelText->SetValue(str);
   
   str.Printf("%.1f", m_pAp->GetUserMaxLevel());
   m_pUserMaxLevelText->SetValue(str);
   
   m_pNoiseCorrectionCheck->SetValue(m_pAp->IsNoiseCorrectionEnabled());
   m_pEDTCheck->SetValue(m_pAp->IsEDTEnabled());
   m_pStageCheck->SetValue(m_pAp->AreStageParametersEnabled());
   m_pAvgModeCheck->SetValue(m_pAp->IsAverageModeEnabled());

   // Direct Sound Trigger text control
   str.Printf("%.1f", m_pAp->GetDirectSoundTrigValue());
   m_pDstText->SetValue(str);
   
   // Peak SPL value text control
   str.Printf("%.1f", m_pAp->GetFullScale());
   m_pFullScaleText->SetValue(str);
   
   // Stereo mode panel   
   str.Printf("%.1f", 1000.0 * m_pAp->GetProbeMicsDistance()); // m -> mm
   m_pProbeMicsDistText->SetValue(str);
   
   str.Printf("%.1f", m_pAp->GetSoundSpeedValue());
   m_pSoundSpeedText->SetValue(str);
   

   int nCh = m_pAp->GetChannelsCount();
   
   if(nCh == 2)
   {
      m_pTwoOmniRadio->Enable();
      m_pAmbisonicsWYRbutton->Enable();
      m_pPUProbeRbutton->Enable();
      m_pIntensityProbeRbutton->Enable();
      m_pBinauralRbutton->Enable();
   }
   else if(nCh == 4)
   {
      m_pFourOmniRbutton->Enable();
      m_pAmbisonicsLabel->Enable();
      m_pFuMaRbutton->Enable();
      m_pAmbixRbutton->Enable();
   }
}



//----------------------------------------------------------------------------
// Aurora::AcParametersShowDialog
//----------------------------------------------------------------------------
// --- Useful methods ---

Aurora::AcParametersDecayPlot* Aurora::AcParametersShowDialog::DecayPlot()
{
    return dynamic_cast<Aurora::AcParametersDecayPlot*>(m_pPlot);
}

Aurora::AcParametersBarPlot*  Aurora::AcParametersShowDialog::BarPlot()
{
    return dynamic_cast<Aurora::AcParametersBarPlot*>(m_pPlot);
}

void Aurora::AcParametersShowDialog::UpdateParametersValues(const bool bInit)
{
    int row = 0;

    for (auto& param : m_shownParameters)
    {                
        auto& values = m_pAp->Results(m_nCurrentChannel).GetSpectrum(param);
        
        values.ForEach(true, 
                       [&](const size_t col, 
                           const float fcb, 
                           Aurora::AcousticalParameters::TResults::TParameter& par)
        {
            wxString str = (par.isValid ? wxString::Format("%f", par.value)
                                        : "-.-");

            if(bInit)
            {
                m_pGrid->SetCellRenderer(row, (int)col, new wxGridCellFloatRenderer(-1, 3));
                m_pGrid->SetCellBackgroundColour(row, (int)col, *wxWHITE);
            }
            m_pGrid->SetCellValue(row, (int)col, str);
        });

        ++row;
    }
}

static wxString ExtractMeasureUnit(wxString& label)
{
    wxString s = label.AfterFirst('[');
    
    if (s.IsEmpty())
    {
        return label;
    }
    s = s.BeforeLast(']');
    assert(s.Length() > 0);
    
    label = label.BeforeFirst('[');
    return s;
}

void Aurora::AcParametersShowDialog::UpdatePlot(const bool bReplaceData)
{
    wxString msg;
    
    if( m_bIsScopeOn )
    {
        if(bReplaceData)
        {
            auto dplot = DecayPlot();
            dplot->ReplaceSerie(0, m_pAp->Track(m_nCurrentChannel));
            dplot->ReplaceSerie(1, m_pAp->Decay(m_nCurrentChannel));
        }
        
        const auto& fcbs = m_pAp->Results(m_nCurrentChannel).Frequencies();
        
        if (m_nCurrentBand < (int)fcbs.size())
        {
            msg.Printf("%.1f Hz band filtered ", fcbs[m_nCurrentBand]);
        }
        else if (m_nCurrentBand == fcbs.size())
        {
            // A weighted
            msg = "A filtered ";
        }
        else if (m_nCurrentBand == fcbs.size() + 1)
        {
            msg = "Unfiltered ";
        }
        
        msg << "Impulse Response";
        
    }
    else
    {
        auto bplot = BarPlot();
	  //Setup plot for bar chart drawing
        msg = "Chart of ";

        msg << m_pGrid->GetRowLabelValue(m_nCurrentParameter);
        wxString mu = ExtractMeasureUnit(msg);
        
        if (! mu.IsEmpty())
        {
            bplot->GetLevelAxis().SetTitle(mu);
            bplot->GetLevelAxis().SetMeasureUnit(mu);
        }
    }
    wxStaticBox* pSb  = wxDynamicCast(FindWindowById(ID_PLOT_SBOX, this),
                                      wxStaticBox);
    pSb->SetLabel(msg);

    m_pPlot->Refresh();
}

Aurora::AcParametersDecayPlot* Aurora::AcParametersShowDialog::CreateDecayPlot()
{
    int nWidth, nHeight;
    m_pPlot->GetSize(&nWidth, &nHeight);
    m_pPlot->Destroy(); 
    m_pPlot = nullptr;

    wxSizer* bPlotSizer = m_pPlotPanel->GetSizer();

    if (bPlotSizer == nullptr)
    {
        bPlotSizer = new wxBoxSizer(wxVERTICAL);
        m_pPlotPanel->SetSizer(bPlotSizer);
    }

    m_pPlot = new Aurora::AcParametersDecayPlot(m_pPlotPanel, 
                                                ID_PLOT,
                                                wxDefaultPosition, 
                                                wxSize(nWidth, nHeight));
    bPlotSizer->Add(m_pPlot, 1, wxEXPAND|wxALL, 5 );
    m_pPlotPanel->Layout();
    return DecayPlot();
}

Aurora::AcParametersBarPlot* Aurora::AcParametersShowDialog::CreateBarPlot()
{
    int nWidth, nHeight;
    m_pPlot->GetSize(&nWidth, &nHeight);
    m_pPlot->Destroy(); 
    m_pPlot = nullptr;

    wxSizer* bPlotSizer = m_pPlotPanel->GetSizer();

    if (bPlotSizer == nullptr)
    {
        bPlotSizer = new wxBoxSizer(wxVERTICAL);
        m_pPlotPanel->SetSizer(bPlotSizer);
    }

    m_pPlot = new Aurora::AcParametersBarPlot(m_pPlotPanel, 
                                              ID_PLOT,
                                              wxDefaultPosition, 
                                              wxSize(nWidth, nHeight));
    bPlotSizer->Add(m_pPlot, 1, wxEXPAND|wxALL, 5 );
    
    m_pPlotPanel->Layout();
    return BarPlot();
}

void Aurora::AcParametersShowDialog::SetupDecayPlot()
{
    auto dplot = DecayPlot();
    auto bplot = BarPlot();

    if(bplot)
    {
        dplot = CreateDecayPlot();
    }
    const double fullscale = m_pAp->GetFullScale();
    const double maxdB = fullscale;
    const double mindB = fullscale - 120;
    

    dplot->GetTimeAxis().SetTitle("Time [s]");
    dplot->GetTimeAxis().ShowTitle(true);
    dplot->GetTimeAxis().ShowGrid(true);
    dplot->GetTimeAxis().SetFormat(Aurora::Axis::RF_REAL);
    dplot->GetTimeAxis().SetMeasureUnit(wxT("s"));

    dplot->GetLevelAxis().SetTitle("Level [dB]");
    dplot->GetLevelAxis().ShowTitle(true);
    dplot->GetLevelAxis().ShowGrid(true);
	dplot->GetLevelAxis().SetRange(mindB, maxdB, Aurora::Axis::RF_LINEARdB);
    dplot->GetLevelAxis().SetMeasureUnit(wxT("dB"));

    dplot->SetHorzAxisAutorange(true);
    dplot->SetVertAxisAutorange(false);

    dplot->GetLegend()->Show(true);

    Aurora::AcParametersTimeDataSerie* pTds;
    pTds = new Aurora::AcParametersTimeDataSerie(m_pAp->Track(m_nCurrentChannel),
                                                 Aurora::AcParametersTimeDataSerie::HT_RMS);
    pTds->SetLabel("RMS");
    pTds->SetLineColour(0,0,255);
    pTds->SetFullscale(fullscale);
    dplot->AddSerie(pTds);

    pTds = new Aurora::AcParametersTimeDataSerie(m_pAp->Decay(m_nCurrentChannel),
                                                 Aurora::AcParametersTimeDataSerie::HT_DECIMATE);
    pTds->SetLabel("Schroeder decay");
    pTds->SetLineColour(255,0,0);
    pTds->SetFullscale(fullscale);
    dplot->AddSerie(pTds);
}

void Aurora::AcParametersShowDialog::SetupBarPlot()
{
    auto dplot = DecayPlot();
    auto bplot = BarPlot();

    if(dplot)
    {
        bplot = CreateBarPlot();
    }
    
    wxColour col(wxT("DARK SLATE GREY")); // Should be MEDIUM_GRAY...
    
    auto& res  = m_pAp->Results(m_nCurrentChannel);
    
    // ---> Bar plot
    int np = 0;
    
    for (auto& param : m_shownParameters)
    {
        bplot->AddSerie(res.GetSpectrum(param), col, col);
        bplot->ShowSerie(np++, false); // Defaults to false
    }
    const auto& fcbs = res.Frequencies();
    wxString title = fcbs.size() > 10 ? "Third-Octave band [Fc/Hz]"
                                      : "Octave band [Fc/Hz]";
    wxArrayString categories;

    for (auto fcb : fcbs)
    {
        categories.Add(Aurora::GetBandShortLabel(fcb));
    }
    categories.Add("A");
    categories.Add("Lin");
    bplot->GetBandsAxis().AddCategories(categories);
    bplot->ShowSerie(m_nCurrentParameter);

    bplot->GetBandsAxis().SetTitle(title);
    bplot->GetBandsAxis().ShowTitle(true);
    bplot->GetBandsAxis().ShowGrid(false);

    bplot->GetLevelAxis().SetTitle("dB");
    bplot->GetLevelAxis().ShowTitle(true);
    bplot->GetLevelAxis().ShowGrid(true);
    bplot->GetLevelAxis().SetFormat(Aurora::AxisBase::RF_REAL);
    bplot->GetLevelAxis().SetMeasureUnit(wxT("dB"));

    bplot->SetVertAxisAutorange(true);
}

void Aurora::AcParametersShowDialog::OnCloseDialog(wxCloseEvent& event)
{
    StoreGeometry();
    EndModal(false);
}

// --- Buttons Methods ---
void Aurora::AcParametersShowDialog::OnHelp(wxCommandEvent &event)
{
    wxString path;
    path << "Docs" << TRAILING_SLASH << "ac_par.html";
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::AcParametersShowDialog::OnSetupButton(wxCommandEvent& event)
{
    PromptSetupDialog();
}

void Aurora::AcParametersShowDialog::OnSaveResults(wxCommandEvent &event) 
{
    Aurora::AcParametersExports exportator(m_pAp);
    exportator.SetExportedParameters(m_shownParameters);
    exportator.SaveToFile();
}

void Aurora::AcParametersShowDialog::OnCopyResults(wxCommandEvent &event) 
{
    Aurora::AcParametersExports exportator(m_pAp);
    exportator.SetExportedParameters(m_shownParameters);
    exportator.CopyEntireTableToClipboard();
}

void Aurora::AcParametersShowDialog::OnCopySelection(wxCommandEvent &event) 
{
    Aurora::AcParametersExports exportator(m_pAp);
    exportator.SetExportedParameters(m_shownParameters);
    exportator.CopyGridSelectionToClipboard(m_pGrid);
}

void Aurora::AcParametersShowDialog::OnStoreGRefSig(wxCommandEvent &event) 
{
    if (m_pAp->GetOctaveFraction() > 1)
    {
        Aurora::AcParametersEffect::MessageBox("Sorry but strenGht calculation works "
                                               "only when octave bands analysis is selected.",
                                               Aurora::MessageType::Info);
        return;
    }

    auto pDlg = new Aurora::StoreGValueDialog(m_pAp, this);
    pDlg->CenterOnParent();

    if(pDlg->ShowModal())
    {
        pDlg->Destroy();

        if(!m_pAp->CalculateAcousticParameters())
        {
            Aurora::AcParametersEffect::MessageBox("Some errors has been occourred during "
                                                   "parameters calculation.",
                                                   Aurora::MessageType::Info);
            return;
        }
        UpdateParametersValues();
    }
    else
    {
        pDlg->Destroy();
    }
}

void Aurora::AcParametersShowDialog::OnOk(wxCommandEvent &event)
{
#ifdef MULTIRATE_FILTERING
    Aurora::StoreGeometry();
    EndModal(false);    
#else
    wxString name;
    wxString bandLabel = m_pGrid->GetColLabelValue(m_nCurrentBand);

    for(int ch = 0; ch < int(m_pAp->GetChannelsCount()); ch++)
    {
        auto& fcbs = m_pAp->Results(ch).Frequencies();
        
        m_pAp->UpdateFilteredTrack(ch, bandLabel);

        name = m_pAp->Track(ch).GetName();
        
        if (m_nCurrentBand == fcbs.size())
        {
            name << "_A";
        }
        else if (m_nCurrentBand == fcbs.size() + 1)
        {
            name << "_not";
        }
        else
        {
            name << "_" << int(fcbs[m_nCurrentBand]);
        }

        name << "_filtered_[" << (ch+1) << "]";

        m_pAp->Track(ch).SetName(name);
    }
    
    name = m_pAp->GetFileToAppend();
    
    if (! name.IsEmpty())
    {
        Aurora::AcParametersExports exportator(m_pAp);
        exportator.SetExportedParameters(m_shownParameters);
        exportator.SetAppendToFileName(name);
        exportator.AppendResultsToFile();
    }
    
    StoreGeometry();
    EndModal(true);
#endif
}

void Aurora::AcParametersShowDialog::OnClose(wxCommandEvent &event)
{
    StoreGeometry();
    EndModal(false);
}

void  Aurora::AcParametersShowDialog::OnChannelListSelect( wxCommandEvent& event )
{
   m_nCurrentChannel = event.GetSelection();
   m_pAp->UpdateFilteredTrack(m_nCurrentChannel, 
                              m_pGrid->GetColLabelValue(m_nCurrentBand));
   UpdatePlot();
   UpdateParametersValues();
}

void Aurora::AcParametersShowDialog::OnGridCellRightClick( wxGridEvent& event )
{
   PopupMenu(m_contextualMenu.get());
}

void  Aurora::AcParametersShowDialog::OnGraphChange( wxGridEvent& event )
{
   if(event.GetRow() < 0)
   {
        m_nCurrentBand = event.GetCol();
        wxString band = m_pGrid->GetColLabelValue(m_nCurrentBand);
        
        m_pAp->UpdateFilteredTrack(m_nCurrentChannel, band);
        
        if(!m_bIsScopeOn)
        {
            SetupDecayPlot();
            m_bIsScopeOn = true;
            UpdatePlot(false);
            return;
        }
    }
    else if (event.GetCol() < 0)
    {
        wxString label = m_pGrid->GetRowLabelValue(event.GetRow());
        
        if(m_bIsScopeOn)
        {
            m_nCurrentParameter = event.GetRow();
            SetupBarPlot();
            m_bIsScopeOn = false;
        }
        else
        {
            auto bplot = BarPlot();
            bplot->ShowSerie(m_nCurrentParameter, false);
            m_nCurrentParameter = event.GetRow();
            bplot->ShowSerie(m_nCurrentParameter);
        }
   }
   UpdatePlot();
}


void Aurora::AcParametersShowDialog::OnGridLabelDoubleClick( wxGridEvent& event )
{
	int nRow = event.GetRow(),
	    nCol = event.GetCol();

    if ((nCol < 0) && (nRow < 0))
    {
        m_pGrid->SelectAll();
    }
    else if (nCol < 0)
    {
        m_pGrid->SelectRow(nRow);
    }
    else if (nRow < 0)
    {
        m_pGrid->SelectCol(nCol);
    }
}

void Aurora::AcParametersShowDialog::OnChar(wxKeyEvent &event)
{
#ifndef __WXMAC__
    if( (event.GetModifiers() == 3) )              // Ctrl+C  ascii translated code.
#else
    if( (event.GetModifiers() == wxMOD_CMD) &&    // Command+C  on Apple machines
    	 event.GetKeyCode() == 'c' )
#endif
    {
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_COPY_SELECTION_MENU);
        OnCopySelection(cmd);
    }
}

void Aurora::AcParametersShowDialog::PromptSetupDialog()
{
    auto pDlg = new Aurora::AcParametersDialog(this, m_pAp);
    pDlg->CenterOnParent();

    if(!pDlg->ShowModal())
    {
        pDlg->Destroy();
        return;
    }

    m_pAp->StoreConfigurationValues();
    pDlg->Destroy();

    if(! m_pAp->CalculateAcousticParameters())
    {
        Aurora::AcParametersEffect::MessageBox("Some errors has been occourred during "
                                               "parameters calculation.",
                                               Aurora::MessageType::Info);
        return;
    }
    Init();
}

void Aurora::AcParametersShowDialog::StoreGeometry()
{
    int nWidth = 0, nHeight = 0;
    this->GetSize(&nWidth, &nHeight);

    int nHorzSplitPos = m_pHorzSplitter->GetSashPosition();

    Aurora::Config cfg;
    cfg.Write("/Aurora/Aurora::AcParameters/ShowDialogWidth", nWidth);
    cfg.Write("/Aurora/Aurora::AcParameters/ShowDialogHeight", nHeight);
    cfg.Write("/Aurora/Aurora::AcParameters/HorzSplitterPosition", nHorzSplitPos);
}

void Aurora::AcParametersShowDialog::RestoreGeometry()
{
    Aurora::Config cfg;
    int nPos;

    if(cfg.Read("/Aurora/Aurora::AcParameters/HorzSplitterPosition", &nPos))
    {
        m_pHorzSplitter->SetSashPosition(nPos);
    }
    
    int nWidth = 0, nHeight = 0;
    
    if(cfg.Read("/Aurora/Aurora::AcParameters/ShowDialogWidth", &nWidth) &&
       cfg.Read("/Aurora/Aurora::AcParameters/ShowDialogHeight", &nHeight))
    {
        this->SetSize(nWidth, nHeight);
    }
}

#include <cctype>

static std::string str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
    {
        return std::toupper(c);
    });
    return s;
}

void Aurora::AcParametersShowDialog::ResetParametersList()
{
    m_shownParameters.clear();

    std::vector<std::string> params(m_pAp->Results(m_nCurrentChannel).Parameters());

    for (int row = 0; row < m_pGrid->GetNumberRows(); ++row)
    {
        std::string rl = m_pGrid->GetRowLabelValue(row).ToStdString();
        rl = str_toupper(rl);
                
        for (auto it = params.begin(); it != params.end(); ++it)
        {            
            auto p = str_toupper(*it);

            if (rl == p)
            {
                m_shownParameters.push_back(*it);
                params.erase(it);
                break;
            }
            else if (rl.find(p) == 0) // rl starts with p
            {
                m_shownParameters.push_back(*it);
                params.erase(it);
                break;
            }
            else if (p.size() > 8 && rl.find(p.substr(0, 4)) == 0) // rl starts with 
            {                                                      // the first 4 characters of p
                // peakiness, millisecs, impuls, cases...
                m_shownParameters.push_back(*it);
                params.erase(it);
                break;
            }
            else if (   rl.find("IACC") != std::string::npos
                     &&  p.find("IACC") != std::string::npos
                     &&  p[0] == rl[0])
            {
                // IACC cases
                m_shownParameters.push_back(*it);
                params.erase(it);
                break;
            }
        }
    }
}

void Aurora::AcParametersShowDialog::Init()
{
    wxString str;

    // Setup Tuser values
    auto pSt  = wxDynamicCast(FindWindowById(ID_LOW_TUSER_LIMIT_STEXT, this),
                              wxStaticText);
    assert(pSt);
    str.Printf("%.1f", m_pAp->GetUserMinLevel());
    pSt->SetLabel(str);

    str.Printf("%.1f", m_pAp->GetUserMaxLevel());
    pSt  = wxDynamicCast(FindWindowById(ID_UP_TUSER_LIMIT_STEXT, this),
                         wxStaticText);
    assert(pSt);
    pSt->SetLabel(str);

    // Set mic/probe type
    switch(m_pAp->GetSource())
    {
        case Aurora::AcousticalParameters::Source::SoundfieldWY:   
            str = "Ambisonics (WY only)"; 
            break;
        case Aurora::AcousticalParameters::Source::PuProbe:
            str = "Pressure/Velocity probe"; 
            break;
        case Aurora::AcousticalParameters::Source::IntensityProbe: 
            str = "Intensimetric probe";
            break;
        case Aurora::AcousticalParameters::Source::AmbisonicsFuMa: // WXYZ 
            str = "Ambisonics 1st (FuMa)";   //(WXYZ)");       
            break;
        case Aurora::AcousticalParameters::Source::AmbisonicsAmbix: // WYZX 
            str = "Ambisonics 1st (Ambix)";  //(WYZX)");   
            break;
        case Aurora::AcousticalParameters::Source::Binaural:
            str = "Binaural pair";
            break;
        default: 
            str = "Omnidirectional mic(s)";  
            break;
    }
    m_pSelectedMicLabel->SetLabel(str);

    // Setup Channels listbox
    auto pLb  = wxDynamicCast(FindWindowById(ID_CHANNELS_LIST, this),
                              wxListBox);
    assert(pLb);
    int nt = (int)m_pAp->Tracks().size();

    for(int nCh = 0; nCh < nt; nCh++)
    {
        str = m_pAp->Track(nCh).GetName();

        if(m_pAp->IsStereo())
        {
            switch(nCh)
            {
                case CH_LEFT:  str << " [L]"; break;
                case CH_RIGHT: str << " [R]"; break;
            }
        }
        else if(m_pAp->IsFuMa())
        {
            switch(nCh)
            {
                case 0:  str << " [W]"; break;
                case 1:  str << " [X]"; break;
                case 2:  str << " [Y]"; break;
                case 3:  str << " [Z]"; break;
            }
        }
        else if(m_pAp->IsAmbix())
        {
            switch(nCh)
            {
                case 0:  str << " [W]"; break;
                case 1:  str << " [Y]"; break;
                case 2:  str << " [Z]"; break;
                case 3:  str << " [X]"; break;
            }
        }
        else if(m_pAp->Tracks().size() > 1)
        {
            str << " [" << (nCh+1) << "]";
        }

        if(nCh > int(pLb->GetCount()) - 1 )
        {
            pLb->Append(str);
        }
        else
        {
            pLb->SetString(nCh, str);
        }
    }
    pLb->SetSelection(m_nCurrentChannel);

    //--------- Labels generation -------------
    auto pGrd = wxDynamicCast(FindWindowById(ID_PARAM_GRID, this), wxGrid);
    assert(pGrd);
    
    // --- adjust rows
    int nLastRow = 11;

    if(m_pAp->AreStageParametersEnabled())
    {            
        int nNeededRows = 17 - pGrd->GetNumberRows();
        
        if (nNeededRows > 0)
        {
            pGrd->AppendRows(nNeededRows);
        }

        pGrd->SetRowLabelValue(nLastRow++, "St1 [dB]");
        pGrd->SetRowLabelValue(nLastRow++, "St2 [dB]");
        pGrd->SetRowLabelValue(nLastRow++, "StLate [dB]");
    }

    switch(m_pAp->GetSource())
    {
        case Aurora::AcousticalParameters::Source::SoundfieldWY:
        case Aurora::AcousticalParameters::Source::PuProbe:
        case Aurora::AcousticalParameters::Source::IntensityProbe:
        case Aurora::AcousticalParameters::Source::AmbisonicsFuMa:
        case Aurora::AcousticalParameters::Source::AmbisonicsAmbix:

            pGrd->SetRowLabelValue(nLastRow++, "JLF");
            pGrd->SetRowLabelValue(nLastRow++, "JLFC");
            pGrd->SetRowLabelValue(nLastRow++, "LJ [dB]");
            break;

        case Aurora::AcousticalParameters::Source::Binaural:

            switch(m_pAp->GetIACCSpan())
            {
                case Aurora::AcousticalParameters::IACCSpan::Early:
                    pGrd->SetRowLabelValue(nLastRow++, "IACC (Early)");
                    break;
                case Aurora::AcousticalParameters::IACCSpan::Late:
                    pGrd->SetRowLabelValue(nLastRow++, "IACC (Late)");
                    break;
                case Aurora::AcousticalParameters::IACCSpan::All:
                    pGrd->SetRowLabelValue(nLastRow++, "IACC (All)");
                    break;
            }
            pGrd->SetRowLabelValue(nLastRow++, "t IACC [ms]");
            pGrd->SetRowLabelValue(nLastRow++, "W IACC [ms]");
            break;

        default:
            pGrd->SetRowLabelValue(nLastRow++, "Peakiness [dB]");
            pGrd->SetRowLabelValue(nLastRow++, "Millisecs [dB]");
            pGrd->SetRowLabelValue(nLastRow++, "Impulsivs [dB]");
            break;

    }
    ResetParametersList();

    // -- adjust cols (default is Octave bands, so 12 columns
    const auto& fcbs = m_pAp->Results(0).Frequencies();
    
    int neededCols = int(fcbs.size() + 2 - pGrd->GetNumberCols());
    
    if (neededCols > 0)
    {
        pGrd->AppendCols(neededCols); // should be 20 always...
        int col = 0;
        
        for (auto fcb : fcbs)
        {
            pGrd->SetColLabelValue(col++, GetBandShortLabel(fcb));
        }
        pGrd->SetColLabelValue(col++, "A");
        pGrd->SetColLabelValue(col,   "Lin"); 
    }
    
    UpdateParametersValues(true);

    // Remove unnecessaries grid rows
    if(pGrd->GetNumberRows() > nLastRow)
    {
        pGrd->DeleteRows(nLastRow, pGrd->GetNumberRows() - nLastRow);
    }
    wxString bandStr = pGrd->GetColLabelValue(m_nCurrentBand);
    m_pAp->UpdateFilteredTrack(m_nCurrentChannel, bandStr);

    // Plot initialization

    // ---> Echo & Decay plot
    //if(dplot || m_pBarPlot)
    //	UpdatePlot();
}

Aurora::AcParametersShowDialog::AcParametersShowDialog(wxWindow* pParent,
		                                               Aurora::AcParametersEffect* ap)
  : AcParShowDlg(pParent),
    mParent(pParent),
    m_pAp(ap)
{
	// Set caption
    wxString title("Aurora for Audacity - ISO 3382 Acoustical Parameters Calculation Module - (v.");
    title << Aurora::ModuleVersionString() << ")";
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add(new wxStaticBitmap(m_pLogoPanel,
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap(wxT("Aurora_logo")),
                                       wxDefaultPosition,
                                       wxDefaultSize, 0), 
                    0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    pLogoSizer->Add(new wxStaticBitmap(m_pLogoPanel,
                                    wxID_ANY,
                                    Aurora::ArtProvider::GetBitmap(wxT("ap_logo")),
                                    wxDefaultPosition,
                                    wxDefaultSize, 0), 
                0, wxALL, 5 );

    m_pLogoPanel->SetSizer( pLogoSizer );
    m_pLogoPanel->Layout();
// ---
    m_pGrid->SetDefaultCellBackgroundColour(Aurora::GetCanvasColour());

    SetupDecayPlot();
    Init();

    RestoreGeometry();
    UpdatePlot();

    // init contextual menu event

    m_contextualMenu.reset(new wxMenu);
    m_contextualMenu->Append(ID_COPY_SELECTION_MENU, "&Copy\tCtrl+C");
    m_contextualMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, 
                              wxCommandEventHandler(Aurora::AcParametersShowDialog::OnCopySelection), 
                              NULL, this );
    m_pGrid->Connect(wxEVT_CHAR, 
                    wxCharEventHandler(Aurora::AcParametersShowDialog::OnChar), 
                    NULL, this);
}

//----------------------------------------------------------------------------
// Aurora::StoreGValueDialog
//----------------------------------------------------------------------------
void Aurora::StoreGValueDialog::OnCloseDialog(wxCloseEvent& event)
{
    EndModal(false);
}

// --- TextCtl events --
void Aurora::StoreGValueDialog::OnEditDistanceField(wxCommandEvent &event) 
{
   wxWindow   *wnd = FindWindowById(ID_G_DIST_TEXT, this);
   wxTextCtrl  *tc = wxDynamicCast(wnd, wxTextCtrl);
   wxString str = tc->GetLineText(0);
   str.ToCDouble(&mDist);
}

// --- Buttons events
void Aurora::StoreGValueDialog::OnRestoreDefaults ( wxCommandEvent& event )
{
    m_pAp->RestoreDefaultReferenceLevels();
    EndModal(true);
}

void Aurora::StoreGValueDialog::OnCancel(wxCommandEvent &event)
{
   EndModal(false);
}
void Aurora::StoreGValueDialog::OnOK(wxCommandEvent &event)
{
    Aurora::Config cfg;
    m_pAp->SetGSoundSourceDistance(float(mDist));
    m_pAp->ReplaceGReferenceValues();
    m_pAp->StoreConfigurationGReferenceLevels(cfg);
    EndModal(true);
}

// --- Ctors
Aurora::StoreGValueDialog::StoreGValueDialog(Aurora::AcParametersEffect *ap, 
                                             wxWindow * parent)
  : GStoreDlg(parent),
    m_pAp(ap)
{
   mDist =  m_pAp->GetGSoundSourceDistance();

   wxString str;
   str.Printf("%4.1f", mDist);
   wxWindow *wnd = FindWindowById(ID_G_DIST_TEXT, this);
   wxTextCtrl *tc = wxDynamicCast(wnd, wxTextCtrl);
   tc->SetValue(str);
   // To be added in parent class:wxTextValidator(wxFILTER_NUMERIC));

   // To prevent data destroy due to a uncontrolled
   // EVT_TEXT on wxTextCtrl creation (Windows).
   mDist =  m_pAp->GetGSoundSourceDistance();
}

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  gui.cpp

  Simone Campanini

*******************************************************************//**

\class XFunctionsDialog 
\brief Dialog used with EffectXFunctions.

*//****************************************************************//**

\class XFunctionsShowDialog
\brief Window that shows xfunctions results.

*//*******************************************************************/
#include <aurora.h>

#include "correlator.h"

#include "XFunctionsEffect.h"

#include "XFunctionsData.h"
#include "XFunctionsPlot.h"
#include "XFunctionsExports.h"
#include "XFunctionsArt.h"
#include "XFunctionsDialogs.h"
#include "XFunctionsUi.h"

//----------------------------------------------------------------------------
// XFunctionsDialog implementation
//----------------------------------------------------------------------------


void Aurora::XFunctionsDialog::BuildFftChoice(const int nDefaultSel)
{
   //first clear control...
   m_fftSizeChoice->Clear();
       
   // ...then fill it...
   wxString value;
   int dataSize = (int)m_pXf->GetInputVectorsLength();
   double exp   = 9.0; // 2^9 = 512
   int   size   = (int)std::pow(2.0, exp);
   
   while(size < dataSize)
   {
       value.Printf("%d", size);
       m_fftSizeChoice->Append(value);
       exp += 1.0;
       size = (int)std::pow(2.0, exp);
   }
   
   // ...and set default sel.
   m_fftSizeChoice->SetSelection(nDefaultSel < (int(exp) - 9) ? nDefaultSel : 0);
}

void Aurora::XFunctionsDialog::ChooseXFunction(const int index)
{
    assert(index >= 0 && index < Aurora::XFunctionType::kCoherence);

    m_pXf->SetXFunctionType(index);

    if (index == Aurora::XFunctionType::kCrossCorrelation)
    {
        m_lagLabel->Enable();
        m_lagValueLabel->Enable();
    }
    else
    {
        m_lagLabel->Disable();
        m_lagValueLabel->Disable();
    }
    UpdateLag();

    m_xfunctionChoice->Enable(index < Aurora::XFunctionType::kPUCalibrationSwTube);
    m_equationBmp->SetBitmap(wxArtProvider::GetBitmap(m_equationBitmapPtr[index]));
}

void Aurora::XFunctionsDialog::UpdateLag() 
{
   wxString value("- s");
   
   if(m_lagValueLabel->IsEnabled())
   {
      double dbExp = 9.0 + m_fftSizeChoice->GetSelection();
      value.Printf("%.3f s", std::pow(2.0, dbExp)/(2.0*m_pXf->GetSamplerate()));
   }   
   m_lagValueLabel->SetLabel(value);
}

void Aurora::XFunctionsDialog::OnCloseEvent( wxCloseEvent& event)
{
	EndModal(false);
}

void Aurora::XFunctionsDialog::OnFftSizeChoice( wxCommandEvent& event )
{
    wxString value = event.GetString();
    m_pXf->SetFFTLength(Aurora::SampleCount(::StringToLong(value)));
    UpdateLag();
}

void Aurora::XFunctionsDialog::OnWindowChoice( wxCommandEvent& event )
{
    m_pXf->SetWindowType(event.GetSelection());
}

void Aurora::XFunctionsDialog::OnXFunctionChoice( wxCommandEvent& event)
{
   const int nSel = event.GetSelection();
   assert(nSel < Aurora::XFunctionType::kPUCalibrationSwTube);

   ChooseXFunction(nSel);
}

void Aurora::XFunctionsDialog::OnSWTubeCheck( wxCommandEvent& event )
{
    int sel = -1;

    if(m_swTubeCheck->IsChecked())
    {
        m_freeFieldCheck->SetValue(false);

        sel = Aurora::XFunctionType::kPUCalibrationSwTube;
        ChooseXFunction(sel);        

        m_diracCheck->SetValue(false);
        m_diracCheck->Disable();
        m_pXf->GetOptions().diracPulse = false;
    }
    else if (! m_freeFieldCheck->IsChecked())
    {
        sel = m_xfunctionChoice->GetSelection();
        ChooseXFunction(sel);

        m_diracCheck->Enable();
    }
}

void Aurora::XFunctionsDialog::OnFreeFieldCheck( wxCommandEvent& event )
{
    int sel = -1;

    if(m_freeFieldCheck->IsChecked())
    {
        m_swTubeCheck->SetValue(false);

        sel = Aurora::XFunctionType::kPUCalibrationFreeField;
        ChooseXFunction(sel);

        m_diracCheck->SetValue(false);
        m_diracCheck->Disable();
        m_pXf->GetOptions().diracPulse = false;
    }
    else if (! m_swTubeCheck->IsChecked())
    {
        sel = m_xfunctionChoice->GetSelection();
        ChooseXFunction(sel);

        m_diracCheck->Enable();
    }
}

void Aurora::XFunctionsDialog::OnSoundSpeedText( wxCommandEvent& event )
{
    m_pXf->SetSoundSpeed(::TextPtrToDouble(m_soundSpeedText));
}

void Aurora::XFunctionsDialog::OnProbeDistanceText( wxCommandEvent& event )
{
    m_pXf->SetProbeDistance(::TextPtrToDouble(m_probeDistText));
}

void Aurora::XFunctionsDialog::OnFmaxText( wxCommandEvent& event )
{
    m_pXf->SetProbeMaxFreq(::TextPtrToDouble(m_fmaxText));
}

void Aurora::XFunctionsDialog::OnEnableFollowingFilterCheck( wxCommandEvent& event )
{
    if(event.IsChecked())
    {
        m_followLeftRadio->Enable();
        m_followRightRadio->Enable();
        m_bandwidthText->Enable();
    }
    else
    {
        m_followLeftRadio->Disable();
        m_followRightRadio->Disable();
        m_bandwidthText->Disable();
    }
}

void Aurora::XFunctionsDialog::OnFollowLeftRadio( wxCommandEvent& event )
{
    m_pXf->SetFollowingChannel(CH_LEFT);
}

void Aurora::XFunctionsDialog::OnFollowRightRadio( wxCommandEvent& event )
{
    m_pXf->SetFollowingChannel(CH_RIGHT);
}

void Aurora::XFunctionsDialog::OnBandwidthText( wxCommandEvent& event )
{
    m_pXf->SetFollowingBandwidth(::TextPtrToDouble(m_bandwidthText));
}

void Aurora::XFunctionsDialog::OnNormalizeLeftChannelCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().normalize = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnHilbertCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().squaredHilbertTransform = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnShiftToHalfWindowCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().shiftToHalfWindow = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnTimeReversalCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().timeReversal = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnCoherenceCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().coherenceWeighting = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnTriggerLevelText( wxCommandEvent& event )
{
    m_pXf->SetTriggerLevel(::TextPtrToDouble(m_triggerText));
}

void Aurora::XFunctionsDialog::OnDiracPulseCheck( wxCommandEvent& event )
{
    m_pXf->GetOptions().diracPulse = event.IsChecked();
}

void Aurora::XFunctionsDialog::OnHelp( wxCommandEvent& event )
{
    wxString path = "";
    path << "Docs" << TRAILING_SLASH << "x_funct.html";
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::XFunctionsDialog::OnOk( wxCommandEvent& event )
{
    EndModal(true);
}

void Aurora::XFunctionsDialog::OnCancel( wxCommandEvent& event )
{
    EndModal(false);
}

Aurora::XFunctionsDialog::XFunctionsDialog(wxWindow* pParent, 
                                           Aurora::XFunctionsEffect* pXf)
  : XFunctionsDlg(pParent),
    m_pXf(pXf)
{
    // Set caption
    wxString title("Aurora for Audacity - Cross Functions Calculation Module - v.");
    title << Aurora::ModuleVersionString();
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add( new wxStaticBitmap(m_logoPanel, 
                                        wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                        wxDefaultPosition, 
                                        wxDefaultSize, 
                                        0), 
                     0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );
    pLogoSizer->Add( new wxStaticBitmap(m_logoPanel, 
                                        wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("xfn_logo"),
                                        wxDefaultPosition, 
                                        wxDefaultSize, 0 ), 
                     0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();
    

    Aurora::InitXFunctionsArtProvider();

    // equations bitmaps
    m_equationBitmapPtr.Add("eq_autocorr");     // XFT_AUTO_CORRELATION,
    m_equationBitmapPtr.Add("eq_crosscorr");    // XFT_CROSS_CORRELATION,
    m_equationBitmapPtr.Add("eq_whitened_csp"); // XFT_WHITE_CORRELATION,
    m_equationBitmapPtr.Add("eq_crosspower");   // XFT_CROSS_POWER,
    m_equationBitmapPtr.Add("eq_h1");           // XFT_TF_H1,
    m_equationBitmapPtr.Add("eq_h2");           // XFT_TF_H2,
    m_equationBitmapPtr.Add("eq_h3");           // XFT_TF_H3,
    m_equationBitmapPtr.Add("eq_alpha");        // XFT_ALPHA, 
    m_equationBitmapPtr.Add("eq_alpha");        // XFT_ALPHA, 
    m_equationBitmapPtr.Add("eq_pu_swtube");    //  PU SW
    m_equationBitmapPtr.Add("eq_pu_freefield"); //  PU FF

    wxString value;

    int nSel = iround(log(double(m_pXf->GetFFTLength())) / log(2.0));
    BuildFftChoice(nSel - 9); // starts from 2^9 = 512
    m_windowChoice->SetSelection(m_pXf->GetWindowType());

    //XFunction type
    nSel = m_pXf->GetXFunctionType();
    bool disableDiracCheck = false;

    assert(nSel < Aurora::XFunctionType::kCoherence);

    if (nSel < Aurora::XFunctionType::kPUCalibrationSwTube)
    {
        m_xfunctionChoice->SetSelection(nSel);
    }
    else
    {
        m_xfunctionChoice->SetSelection(0);
        m_xfunctionChoice->Disable();
        
        // PU Probe stuffs
        m_swTubeCheck->SetValue(m_pXf->IsProbeRigidTerminated());
        m_freeFieldCheck->SetValue(m_pXf->IsProbeFreeField());
        disableDiracCheck = true;
    }
    m_equationBmp->SetBitmap(wxArtProvider::GetBitmap(m_equationBitmapPtr[nSel]));

    // Eventually update lag text
    if (nSel == Aurora::XFunctionType::kCrossCorrelation)
    {
        m_lagLabel->Enable();
        m_lagValueLabel->Enable();
    }
    else
    {
        m_lagLabel->Disable();
        m_lagValueLabel->Disable();
    }
    UpdateLag();

    value.Printf("%.f", m_pXf->GetSoundSpeed());
    m_soundSpeedText->SetValue(value);
    
    value.Printf("%.1f", m_pXf->GetProbeDistance());
    m_probeDistText->SetValue(value);
    
    value.Printf("%.1f", m_pXf->GetProbeMaxFreq());
    m_fmaxText->SetValue(value);

    //Following filter stuffs
    m_enableFFilterCheck->SetValue(m_pXf->IsFollowingFilterEnabled());
    value.Printf("%.3f", m_pXf->GetFollowingBandwidth());
    m_bandwidthText->SetValue(value);

    // Checkers init
    auto& options = m_pXf->GetOptions();

    m_normalizeLeftCheck->SetValue(options.normalize);
    m_hilbertCheck->SetValue(options.squaredHilbertTransform);
    m_shiftToHalfCheck->SetValue(options.shiftToHalfWindow);
    m_timeReversalCheck->SetValue(options.timeReversal);
    m_coherenceCheck->SetValue(options.coherenceWeighting);

    if (disableDiracCheck)
    {
        m_diracCheck->Disable();
        options.diracPulse = false;
    }
    m_diracCheck->SetValue(options.diracPulse);
 
    value.Printf("%.1f", m_pXf->GetTriggerLevel());
    m_triggerText->SetValue(value);
}

//----------------------------------------------------------------------------
// XFunctionsShowDialog implementation
//----------------------------------------------------------------------------
void Aurora::XFunctionsShowDialog::AddPlotDataSeries(Aurora::XFunctionsPlot* pPlot, 
                                                     const wxColour& lineColor)
{
    pPlot->AddSerie(m_pXf,
                    m_pXf->GetAutocorrelationSpectrum(CH_LEFT),
                    Aurora::XFunctionsPlotType::AutoCorrelationLeft,
                    lineColor);
    pPlot->AddSerie(m_pXf,
                    m_pXf->GetAutocorrelationSpectrum(CH_RIGHT),
                    Aurora::XFunctionsPlotType::AutoCorrelationRight,
                    lineColor);

    const int xfnType = m_pXf->GetXFunctionType();

    if(xfnType != Aurora::XFunctionType::kAutoCorrelation)
    {
        auto& spectrum = m_pXf->GetFunctionSpectrum();

        // Add magnitude, phase, real, imag. series
        pPlot->AddSerie(m_pXf,
                        spectrum,
                        Aurora::XFunctionsPlotType::Magnitude,
                        lineColor);

        pPlot->AddSerie(m_pXf,
                        spectrum,
                        Aurora::XFunctionsPlotType::Phase,
                        lineColor);

        pPlot->AddSerie(m_pXf,
                        spectrum,
                        Aurora::XFunctionsPlotType::Real,
                        lineColor);

        pPlot->AddSerie(m_pXf,
                        spectrum,
                        Aurora::XFunctionsPlotType::Imaginary,
                        lineColor);
    }
    
    if(    xfnType != Aurora::XFunctionType::kAlphaPU
        && xfnType != Aurora::XFunctionType::kAlphaPP)
    {
        // Add alpha and coherence plot for the two PU Calibration functions
        pPlot->AddSerie(m_pXf,
                        m_pXf->GetAlphaSpectrum(),
                        Aurora::XFunctionsPlotType::Alpha,
                        lineColor);

        pPlot->AddSerie(m_pXf,
                        m_pXf->GetCoherenceSpectrum(),
                        Aurora::XFunctionsPlotType::Coherence,
                        lineColor);
    }
    else if(! m_pXf->IsProbeFreeField() && ! m_pXf->IsProbeRigidTerminated())
    {
        pPlot->AddSerie(m_pXf,
                        m_pXf->GetCoherenceSpectrum(),
                        Aurora::XFunctionsPlotType::Coherence,
                        lineColor);
    }
}

void Aurora::XFunctionsShowDialog::UpdatePlotData(Aurora::XFunctionsPlot* pPlot, 
                                                  wxString& plotName)
{
    int nPlotType;

    if(! pPlot->SetPlotTypeByString(plotName, nPlotType))
    {
        printf("Plot not found!\n");
        return;
    }
    bool bIsLog = !m_linearScaleCheck->IsChecked();

    pPlot->GetFreqAxis().SetTitle("Frequency [Hz]");
    pPlot->GetFreqAxis().SetFormat(Aurora::Axis::RF_REAL);
    pPlot->GetFreqAxis().SetLog(bIsLog);
    SetFrequencyRange(pPlot, m_frequencyRangeChoice->GetSelection(), bIsLog);

    switch(nPlotType)
    {
        case Aurora::XFunctionsPlotType::AutoCorrelationLeft:
        case Aurora::XFunctionsPlotType::AutoCorrelationRight:
        case Aurora::XFunctionsPlotType::Magnitude:
        case Aurora::XFunctionsPlotType::Real:
        case Aurora::XFunctionsPlotType::Imaginary:
            pPlot->GetLevelAxis().SetTitle("Level [dB]");
            pPlot->GetLevelAxis().SetRange(-80.0, 80.0, Aurora::Axis::RF_LINEARdB);
            break;

        case Aurora::XFunctionsPlotType::Coherence:
            pPlot->GetLevelAxis().SetTitle("Coherence");
            pPlot->GetLevelAxis().SetRange(0, 1, Aurora::Axis::RF_REAL);
            break;

        case Aurora::XFunctionsPlotType::Phase:
            pPlot->GetLevelAxis().SetTitle("Degrees [°]");
            pPlot->GetLevelAxis().SetRange(-180, 180, Aurora::Axis::RF_REAL);
            break;

        case Aurora::XFunctionsPlotType::Alpha:
            pPlot->GetLevelAxis().SetTitle("Alpha");
            pPlot->GetLevelAxis().SetRange(0.0, 1.2, Aurora::Axis::RF_REAL);
            break;
    }
    pPlot->SetHorzAxisAutorange(false);
    pPlot->SetVertAxisAutorange(false);
}

void Aurora::XFunctionsShowDialog::GetChoiceItems(wxArrayString& list)
{
    list.Clear();
    wxString fnName = "Autocorrelation";
    list.Add(fnName + " Left");
    list.Add(fnName + " Right");

    int nXFnType = m_pXf->GetXFunctionType();

    switch(nXFnType)
    {
        case Aurora::XFunctionType::kAutoCorrelation:
            break;

        case Aurora::XFunctionType::kCrossCorrelation:
            fnName = "Crosscorrelation";
            break;

        case Aurora::XFunctionType::kWhiteCorrelation:
            fnName = "White Crosscorr.";
            break;

        case Aurora::XFunctionType::kCrossPower:
            fnName = "Cross-Power Spect.";
            break;

        case Aurora::XFunctionType::kH1:
            fnName = "H1";
            break;

        case Aurora::XFunctionType::kH2:
            fnName = "H2";
            break;

        case Aurora::XFunctionType::kH3:
            fnName = "H3";
            break;

        case Aurora::XFunctionType::kAlphaPU:
        case Aurora::XFunctionType::kAlphaPP:
            fnName = "Alpha";
            list.Add("Coherence");
            break;

        case Aurora::XFunctionType::kPUCalibrationFreeField:
            list.Add("PU FF Calibration");
            fnName = "Dirac delta";
            break;

        case Aurora::XFunctionType::kPUCalibrationSwTube:
            list.Add("PU SW Calibration");
            fnName = "Dirac delta";
            break;
    }

    if(nXFnType != Aurora::XFunctionType::kAutoCorrelation)
    {
        list.Add(fnName + " Magn.");
        list.Add(fnName + " Phase");
        list.Add(fnName + " Real");
        list.Add(fnName + " Imag.");
    }

    if(    nXFnType != Aurora::XFunctionType::kAlphaPU
        && nXFnType != Aurora::XFunctionType::kAlphaPP)
    {
        list.Add("Coherence");
        list.Add("Alpha");
    }
    else if(! m_pXf->IsProbeFreeField() && ! m_pXf->IsProbeRigidTerminated())
    {
        list.Add("Coherence");
    }
}

void Aurora::XFunctionsShowDialog::BuildPlotChoice(wxChoice* choice, 
                                                   const int nDefaultSel)
{
   //first clear control...
    choice->Clear();
       
    // ...then fill it...
    wxArrayString list;
    GetChoiceItems(list);

    for(unsigned int i = 0; i < list.GetCount(); i++)
    {
        choice->Append(list[i]);
    }
    // ...and set default selection
    choice->SetSelection(nDefaultSel);
}

void Aurora::XFunctionsShowDialog::SetPlotLagAxis(Aurora::XFunctionsPlot* pPlot)
{
    // TODO: deprecated???
   double dbLag = double( m_pXf->GetSpectrumLength()/(2.0*m_pXf->GetSamplerate()) );
   pPlot->GetFreqAxis().SetTitle("Lag [s]");
   pPlot->GetFreqAxis().SetLog(false);
   pPlot->GetFreqAxis().SetRange(-dbLag, dbLag);
}

void Aurora::XFunctionsShowDialog::SetFrequencyRange(Aurora::XFunctionsPlot* pPlot,
                                                     const int nChoice, 
                                                     const bool bIsLog)
{
    switch(nChoice)
    {
        case 0 : pPlot->GetFreqAxis().SetRange((bIsLog) ? 1.0 : 0.0, 
                                                2000.0 );                    
            break; // 0 -> 2kHz
        
        case 1 : pPlot->GetFreqAxis().SetRange((bIsLog) ? 1.0 : 0.0,
                                                m_pXf->GetSamplerate()/2.0 ); 
            break; // 0 -> Fny
    }
}


void Aurora::XFunctionsShowDialog::OnCloseEvent( wxCloseEvent& event)
{
    StoreGeometry();
    EndModal(false);
}

void Aurora::XFunctionsShowDialog::OnFrequencyRangeChoice( wxCommandEvent& event )
{
    int nChoice = event.GetSelection();
    SetFrequencyRange(m_upperPlot, nChoice, !m_linearScaleCheck->IsChecked());
    SetFrequencyRange(m_lowerPlot, nChoice, !m_linearScaleCheck->IsChecked());
    m_upperPlot->Refresh();
    m_lowerPlot->Refresh();
}

void Aurora::XFunctionsShowDialog::OnLinearScaleCheck( wxCommandEvent& event )
{
    if(event.IsChecked())
    {
        m_upperPlot->GetFreqAxis().SetLog(false);
        m_lowerPlot->GetFreqAxis().SetLog(false);
    }
    else
    {
        m_upperPlot->GetFreqAxis().SetLog(true);
        m_lowerPlot->GetFreqAxis().SetLog(true);
    }
    m_upperPlot->Refresh();
    m_lowerPlot->Refresh();
}

void Aurora::XFunctionsShowDialog::OnSaveToTimeDelayCheck( wxCommandEvent& event )
{
	// eventually TODO but in fact not needed. See OnClose
}

void Aurora::XFunctionsShowDialog::OnExportAllSpectrumsCheck( wxCommandEvent& event )
{
    wxLogDebug("Export all %s", event.IsChecked() ? "checked" : "unchecked");
}

void Aurora::XFunctionsShowDialog::OnSaveSpectrumBtn( wxCommandEvent& event )
{
    Aurora::XFunctionsExports exporter(m_upperPlot, m_lowerPlot);

    double min, max;
    m_upperPlot->GetFreqAxis().GetRange(min, max); // These should be the same for both.
    exporter.SetFrequencyBounds(min, max);
    exporter.SetExportAll(m_exportAllCheck->IsChecked());

    exporter.SaveToFile();
}

void Aurora::XFunctionsShowDialog::OnCopySpectrumBtn( wxCommandEvent& event )
{
    Aurora::XFunctionsExports exporter(m_upperPlot, m_lowerPlot);

    double min, max;
    m_upperPlot->GetFreqAxis().GetRange(min, max); // These should be the same for both.
    exporter.SetFrequencyBounds(min, max);
    exporter.SetExportAll(m_exportAllCheck->IsChecked());

    exporter.CopyEntireTableToClipboard();
}

void Aurora::XFunctionsShowDialog::OnExportWaveformsCheck( wxCommandEvent& event )
{
    // eventually TODO but in fact not needed. See OnClose
}

void Aurora::XFunctionsShowDialog::OnFirstPlotChoice( wxCommandEvent& event )
{
    wxString label = m_upperPlotChoice->GetString(event.GetSelection());
    UpdatePlotData(m_upperPlot, label);

    m_upperPlot->Refresh();
}

void Aurora::XFunctionsShowDialog::OnSecondPlotChoice( wxCommandEvent& event )
{
    wxString label = m_lowerPlotChoice->GetString(event.GetSelection());
    UpdatePlotData(m_lowerPlot, label);

    m_lowerPlot->Refresh();
}

void Aurora::XFunctionsShowDialog::OnHelp( wxCommandEvent& event )
{
    wxString path = "";
    path << "Docs" << TRAILING_SLASH << "x_funct.html";
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::XFunctionsShowDialog::OnClose( wxCommandEvent& event )
{
    if(m_saveToTimeDelayCheck->IsEnabled())
    {
        if(m_saveToTimeDelayCheck->IsChecked())
        {
            Aurora::XFunctionsExports exporter(m_upperPlot, m_lowerPlot);

            exporter.SetExportAll(m_exportAllCheck->IsChecked());
            exporter.AppendTimeDelay(m_pXf->GetTriggerTime(),
                                     m_pXf->GetXCorrelationMaxDelay());
        }
    }  
    StoreGeometry();
    EndModal(m_exportWaveformsCheck->IsChecked());
}

void Aurora::XFunctionsShowDialog::StoreGeometry()
{
	int nWidth, nHeight;
	this->GetSize(&nWidth, &nHeight);

    Aurora::Config cfg;

//	int nHorzSplitPos = m_pHorzSplitter->GetSashPosition();

	cfg.Write("/Aurora/XFunctions/ShowDialogWidth", nWidth);
	cfg.Write("/Aurora/XFunctions/ShowDialogHeight", nHeight);
//	cfg.Write("/Aurora/XFunctions/HorzSplitterPosition", nHorzSplitPos);
}

void Aurora::XFunctionsShowDialog::RestoreGeometry()
{
    Aurora::Config cfg;

//	int nPos;
//	if(m_pCfg->Read("/Aurora/XFunctions/HorzSplitterPosition", &nPos))
//	    m_pHorzSplitter->SetSashPosition(nPos);
    int nWidth = 0, nHeight = 0;
    
    if(   cfg.Read("/Aurora/XFunctions/ShowDialogWidth", &nWidth) 
       && cfg.Read("/Aurora/XFunctions/ShowDialogHeight", &nHeight))
    {
        this->SetSize(nWidth, nHeight);
    }
}


Aurora::XFunctionsShowDialog::XFunctionsShowDialog(wxWindow* pParent, 
                                                   Aurora::XFunctionsEffect *pXf)
  : XFunctionsShowDlg(pParent),
    m_pXf(pXf)
{
    // Set caption
    wxString title("Aurora for Audacity - Cross Functions Calculation Module - v.");
    title << Aurora::ModuleVersionString();
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add( new wxStaticBitmap(m_logoPanel, 
                                        wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                        wxDefaultPosition, 
                                        wxDefaultSize, 
                                        0 ),
                     0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );
    pLogoSizer->Add( new wxStaticBitmap(m_logoPanel, 
                                        wxID_ANY,
                                        Aurora::ArtProvider::GetBitmap("xfn_logo"),
                                        wxDefaultPosition, 
                                        wxDefaultSize, 
                                        0 ), 
                     0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();


    wxString value;
    // ------- Fill Result boxes
    // Input waveform
    value.Printf("%d", int(m_pXf->GetFFTLength()));
    m_fftSizeValueLabel->SetLabel(value);
    
    value.Printf("%d", int(m_pXf->GetSamplerate()));
    m_rateValueLabel->SetLabel(value);
    
    value.Printf("%d", int(m_pXf->GetBlocksNumber()));
    m_nblocksValueLabel->SetLabel(value);
    
    // Statistics
    m_transformTypeLabel->SetLabel(m_pXf->GetCurrentXFunctionLabel());
    value.Printf("%.2f", m_pXf->GetRMS().left);
    m_rmsLeftValueLabel->SetLabel(value);

    value.Printf("%.2f", m_pXf->GetRMS().right);
    m_rmsRightValueLabel->SetLabel(value);

    value.Printf("%.2f", m_pXf->GetRMS().cross);
    m_rmsCrossValueLabel->SetLabel(value);

    // Time of flight (only for Cross-Correlation)
    if(m_pXf->GetXFunctionType() == Aurora::XFunctionType::kCrossCorrelation)
    {
        m_timeOfFlightValueLabel->Enable();
        m_triggerTimeLabel->Enable();
        m_triggerTimeValueLabel->Enable();
        m_triggerTimeAtLabel->Enable();
        m_triggerDelayValueLabel->Enable();
        
        m_nccMaxLabel->Enable();
        m_nccMaxValueLabel->Enable();
        m_nccMaxAtLabel->Enable();
        m_nccMaxDelayValueLabel->Enable();
        
        m_saveToTimeDelayCheck->Enable();
        
        value.Printf("%.1f%%", m_pXf->GetTriggerLevel());
        m_triggerTimeValueLabel->SetLabel(value);

        value.Printf("%.4f", m_pXf->GetXCorrelationMax());
        m_nccMaxValueLabel->SetLabel(value);

        const double maxt = m_pXf->GetFFTLength() / m_pXf->GetSamplerate();
        double t = m_pXf->GetTriggerTime();
        
        if (std::fabs(t) < maxt)
        {
            value.Printf("%.3f s", t);
        }
        else
        {
            value = "n.a.";
        }
        m_triggerDelayValueLabel->SetLabel(value);

        t = m_pXf->GetXCorrelationMaxDelay();
        
        if (std::fabs(t) < maxt)
        {
            value.Printf("%.3f s", t);
        }
        else
        {
            value = "n.a.";
        }
        m_nccMaxDelayValueLabel->SetLabel(value);
    }
    else
    {
        m_triggerTimeValueLabel->SetLabel("- %");
        m_triggerDelayValueLabel->SetLabel("- s");
        m_nccMaxValueLabel->SetLabel("- %");
        m_nccMaxDelayValueLabel->SetLabel("- s");
    }
    BuildPlotChoice(m_upperPlotChoice, 0);
    BuildPlotChoice(m_lowerPlotChoice, 1);

    // By default horizontal axis is linear
    m_linearScaleCheck->SetValue(true);

    AddPlotDataSeries(m_upperPlot, *wxBLUE);
    AddPlotDataSeries(m_lowerPlot, *wxRED);

    value = m_upperPlotChoice->GetStringSelection();
    UpdatePlotData(m_upperPlot, value);
    value = m_lowerPlotChoice->GetStringSelection();
    UpdatePlotData(m_lowerPlot, value);

    m_upperPlot->GetFreqAxis().ShowGrid(true);
    m_upperPlot->GetFreqAxis().ShowTitle(true);
    m_upperPlot->GetLevelAxis().ShowGrid(true);
    m_upperPlot->GetLevelAxis().ShowTitle(true);
    m_upperPlot->SetTopBorder(false);

    m_lowerPlot->GetFreqAxis().ShowGrid(true);
    m_lowerPlot->GetFreqAxis().ShowTitle(true);
    m_lowerPlot->GetLevelAxis().ShowGrid(true);
    m_lowerPlot->GetLevelAxis().ShowTitle(true);
    m_lowerPlot->SetTopBorder(false);

    m_upperPlot->Refresh();
    m_lowerPlot->Refresh();

    RestoreGeometry();
}


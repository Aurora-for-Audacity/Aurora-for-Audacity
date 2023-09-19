/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiUi.cpp

  Simone Campanini

*******************************************************************//**

\class STISetupDialog \
\brief Dialog used with EffectAuroraSTI.

*//*******************************************************************/

#include <aurora.h>

#include <wx/filename.h>

#include "sti.h"
#include "StiTrack.h"
#include "StiEffect.h"
#include "StiExports.h"
#include "StiData.h"
#include "StiPlot.h"
#include "StiDialogs.h"
#include "StiSpectrumDialog.h"
#include "StiSpectrum.h"
#include "StiUi.h"

//----------------------------------------------------------------------------
// STISetupDialog implementation
//----------------------------------------------------------------------------

void Aurora::STISetupDialog::InitTrackChoice(wxChoice* choice)
{
   if(!choice->IsEmpty()) 
   {    
       choice->Clear();
       choice->Append("None");
   }
   choice->Append(*(m_pSti->GetTrackNamesArray()));
}

void Aurora::STISetupDialog::FillSigNoiseGrid()
{
    for(int nCh = 0; nCh < 2; nCh++)
    {
        auto& results  = m_pSti->GetResults(nCh);
               
        // 125 Hz to 8 kHz loop,
        
        for (int row = 0; row < m_splTable->GetNumberRows(); ++row)
        {
            wxString label = m_splTable->GetRowLabelValue(row);
            const float fcb = Aurora::GetFcbFromLabel(label);

            m_splTable->SetCellValue(row, nCh,
                                     wxString::Format("%.1f", 
                                                      results.noiseSpectrum[fcb]));
            m_splTable->SetCellValue(row, nCh + 2,
                                     wxString::Format("%.1f",
                                                      results.signalSpectrum[fcb]));
            m_splTable->SetCellValue(row, nCh + 4,
                                     wxString::Format("%.1f",
                                                      results.sigNoiseSpectrum[fcb]));
        }
    }
}

void Aurora::STISetupDialog::OnLeftFullscaleText( wxCommandEvent& event )
{
   m_pSti->SetFullScaleLevel(::TextPtrToDouble(m_fsLeftText), CH_LEFT);
}
    
void Aurora::STISetupDialog::OnRightFullscaleText( wxCommandEvent& event )
{
    m_pSti->SetFullScaleLevel(::TextPtrToDouble(m_fsRightText), CH_RIGHT);
}
    
void Aurora::STISetupDialog::OnCalibLeftChoice( wxCommandEvent& event )
{
    m_pSti->SetCalibrationTrackIdx(CH_LEFT, event.GetSelection());
    
    if(m_pSti->IsCalibrationTrackSet(CH_LEFT) && ! m_rightCalChoice->IsEnabled())
    {
        m_rightCalChoice->Enable();
        m_rightCalLevelLabel->Enable();
        m_calibrateButton->Enable();
    }
    else if(!m_pSti->IsCalibrationTrackSet(CH_LEFT))
    {
        m_rightCalChoice->Disable();
        m_rightCalLevelLabel->Disable();
        m_calibrateButton->Disable();
        // TODO reset right index
    }
}

void Aurora::STISetupDialog::OnCalibRefLeftText( wxCommandEvent& event )
{
    m_pSti->SetCalibrationLevel(::TextPtrToDouble(m_leftCalLevelText), CH_LEFT);
}

void Aurora::STISetupDialog::OnCalibRightChoice( wxCommandEvent& event )
{
    m_pSti->SetCalibrationTrackIdx(CH_RIGHT, event.GetSelection());
}

void Aurora::STISetupDialog::OnCalibRefRightText( wxCommandEvent& event )
{
    m_pSti->SetCalibrationLevel(::TextPtrToDouble(m_rightCalLevelText), CH_RIGHT);
}

void Aurora::STISetupDialog::OnDoFullscaleCalibration( wxCommandEvent& event )
{
   if(m_pSti->AreCalibrationTracksSet())
   {
        if( !m_pSti->DoFullscaleCalibration(true) )  // stereo
        {
            Aurora::STIEffect::MessageBox("Calibration failed.",
                                          Aurora::MessageType::Error);
            return;
        }

        m_fsLeftText->SetValue(wxString::Format("%.1f", 
                                                m_pSti->GetFullScaleLevel(CH_LEFT)) );
        m_fsRightText->SetValue(wxString::Format("%.1f", 
                                                 m_pSti->GetFullScaleLevel(CH_RIGHT)) );
   }
   else if(m_pSti->IsCalibrationTrackSet(CH_LEFT))
   {
        if(! m_pSti->DoFullscaleCalibration() )  // mono 
        {
            Aurora::STIEffect::MessageBox("Calibration failed.",
                                          Aurora::MessageType::Error);
            return;
        }

        m_fsLeftText->SetValue(wxString::Format("%.1f", 
                                                m_pSti->GetFullScaleLevel(CH_LEFT)) );
   }
   else 
   {
       Aurora::STIEffect::MessageBox("No track(s) selected.",
                                     Aurora::MessageType::Error);
       return;
   }
    Aurora::STIEffect::MessageBox("Calibration done successifully.",
                                  Aurora::MessageType::Info);
}

void Aurora::STISetupDialog::OnNoiseLeftChoice( wxCommandEvent& event )
{
    m_pSti->SetNoiseTrackIdx(CH_LEFT, event.GetSelection());
    
    if(m_pSti->IsNoiseTrackSet(CH_LEFT) && ! m_rightNoiseChoice->IsEnabled())
    {
        m_rightNoiseChoice->Enable();
        
        if(m_pSti->IsSignalTrackSet(CH_LEFT) && ! m_calculateSPLButton->IsEnabled())
        {
            m_calculateSPLButton->Enable();
        }
    }
    else if(!m_pSti->IsNoiseTrackSet(CH_LEFT))
    {
        m_rightNoiseChoice->Disable();
        m_calculateSPLButton->Disable();
        // TODO reset right index
    }
}

void Aurora::STISetupDialog::OnNoiseRightChoice( wxCommandEvent& event )
{
    m_pSti->SetNoiseTrackIdx(CH_RIGHT, event.GetSelection());
}

void Aurora::STISetupDialog::OnSignalLeftChoice( wxCommandEvent& event )
{
    m_pSti->SetSignalTrackIdx(CH_LEFT, event.GetSelection());
    
    if(m_pSti->IsSignalTrackSet(CH_LEFT) && ! m_rightSignalChoice->IsEnabled())
    {
        m_rightSignalChoice->Enable();
        if(m_pSti->IsNoiseTrackSet(CH_LEFT) && ! m_calculateSPLButton->IsEnabled())
        {
            m_calculateSPLButton->Enable();
        }
    }
    else if(!m_pSti->IsSignalTrackSet(CH_LEFT))
    {
        m_rightSignalChoice->Disable();
        // TODO reset right index
        m_calculateSPLButton->Disable();
    }
}

void Aurora::STISetupDialog::OnSignalRightChoice( wxCommandEvent& event )
{
    m_pSti->SetSignalTrackIdx(CH_RIGHT, event.GetSelection());
}

void Aurora::STISetupDialog::OnSignalRadio( wxCommandEvent& event )
{
    m_pSti->SetSignalPlusNoise(false);
}

void Aurora::STISetupDialog::OnSignalPlusNoiseRadio( wxCommandEvent& event )
{
    m_pSti->SetSignalPlusNoise(true);
}

void Aurora::STISetupDialog::OnSplTableRightClick( wxGridEvent& event )
{
   PopupMenu(m_splTableContextMenu.get());
}
    
void Aurora::STISetupDialog::OnLoadSplValues( wxCommandEvent& event )
{
    Aurora::STISpectrumExports exporter(m_pSti, m_pSti->IsStereo());
    
   if(exporter.OpenSavePrompt(Aurora::STISpectrumExports::OPEN_PROMPT))
   {
       FillSigNoiseGrid();
   }
}

void Aurora::STISetupDialog::OnSaveSplValues( wxCommandEvent& event )
{
    Aurora::STISpectrumExports exporter(m_pSti, m_pSti->IsStereo());

    // Always save stereo data
    exporter.OpenSavePrompt(STISpectrumExports::SAVE_PROMPT);
}

void Aurora::STISetupDialog::OnComputeSNRatio( wxCommandEvent& event )
{
    bool bIsStereo = false;
   
    if( m_pSti->AreSignalAndNoiseSet(CH_LEFT) &&
        m_pSti->AreSignalAndNoiseSet(CH_RIGHT) )
    {
        bIsStereo = true;
    }
    else if(! m_pSti->AreSignalAndNoiseSet(CH_LEFT) )
    {
        // At least the left channel must be set
        return;
    }
    
    if(! m_pSti->DoSNRatioCalculation(bIsStereo))
    {
        Aurora::STIEffect::MessageBox("Error calculating signal level.\n"
                                      "Noise level too high in some bands.",
                                      Aurora::MessageType::Error);
    }
    
    FillSigNoiseGrid();

    auto dlg = new Aurora::STISpectrumDialog(this, m_pSti, bIsStereo);
    dlg->SetClientSize(900, 440);
    dlg->Centre();
    dlg->ShowModal();
    
    delete dlg;
}

void Aurora::STISetupDialog::OnLeftIRChoice( wxCommandEvent& event )
{
    const int nSel = event.GetSelection();
    m_pSti->SetIrTrackIdx(CH_LEFT, nSel);

    if(nSel && !m_rightIrChoice->IsEnabled())
    {
        m_rightIrChoice->Enable();
    }
    else if(!nSel)
    {
        m_rightIrChoice->Disable();
        // TODO reset right index
    }
    
    if( (!m_okButton->IsEnabled()) && (nSel) )
    {
        m_okButton->Enable();
    }
    
    if(!nSel && !m_pSti->IsIrTrackSet(CH_RIGHT))
    {
        m_okButton->Disable();
    }
}

void Aurora::STISetupDialog::OnRightIRChoice( wxCommandEvent& event )
{
    int nSel = event.GetSelection();
    m_pSti->SetIrTrackIdx(CH_RIGHT, nSel);
    
    if( (!m_okButton->IsEnabled()) && (nSel) )
    {
        m_okButton->Enable();
    }
    
    if(!nSel && !m_pSti->IsIrTrackSet(CH_LEFT))
    {
        m_okButton->Disable();
    }
}

void Aurora::STISetupDialog::OnFatText( wxCommandEvent& event )
{
    m_pSti->SetFirstArriveThreshold(::TextPtrToDouble(m_fatText) / 100.0);
}

void Aurora::STISetupDialog::OnHelp( wxCommandEvent& event )
{
   wxString path = "Docs/sti.html";
   wxFileName help_path(path);
   help_path.MakeAbsolute();

   ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::STISetupDialog::OnSplTableKeyPress( wxKeyEvent& event )
{
#ifndef __WXMAC__
    if( (event.GetModifiers() == 3) )              // Ctrl+C  ascii translated code.
#else
    if( (event.GetModifiers() == wxMOD_CMD) &&    // Command+C  on Apple machines
    	 event.GetKeyCode() == 99 )
#endif
    {   
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_SPL_COPY_SELECTION_MENU);
        OnSplTableCopySelection(cmd);
    }
    //printf("keycode = %d\n", int(event.GetKeyCode()));
}

void Aurora::STISetupDialog::OnSplTableCopySelection(wxCommandEvent & event)
{
    Aurora::STISpectrumExports exporter(m_pSti, m_pSti->IsStereo());
    exporter.CopyGridSelectionToClipboard(m_splTable);
}

// 'ctors
Aurora::STISetupDialog::STISetupDialog(wxWindow* parent, 
                                       Aurora::STIEffect* sti)
  : STISetupDlg( parent ),
    m_pSti(sti)
{
    // Set caption
    wxString title(wxT("Aurora for Audacity - Speech Transmission Index Calculation Module - (v."));
    title << Aurora::ModuleVersionNumber(Aurora::ReleaseType::ART_BETA, 0) << ")";
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("Aurora_logo")), 
                    0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("sti_logo")), 
                     0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();

   // Fill choices with track names
   InitTrackChoice(m_leftCalChoice);
   InitTrackChoice(m_rightCalChoice);
   InitTrackChoice(m_leftNoiseChoice);
   InitTrackChoice(m_rightNoiseChoice);
   InitTrackChoice(m_leftSignalChoice);
   InitTrackChoice(m_rightSignalChoice);
   InitTrackChoice(m_leftIrChoice);
   InitTrackChoice(m_rightIrChoice);
   
    // Initialize choices
    m_leftCalChoice->SetSelection(0);
    m_rightCalChoice->SetSelection(0);
    m_leftNoiseChoice->SetSelection(0);
    m_rightNoiseChoice->SetSelection(0);
    m_leftSignalChoice->SetSelection(0);
    m_rightSignalChoice->SetSelection(0);
    m_leftIrChoice->SetSelection(0);
    m_rightIrChoice->SetSelection(0);    

   // Init default fullscale levels
   m_fsLeftText->SetValue(wxString::Format("%.1f", m_pSti->GetFullScaleLevel(CH_LEFT)));
   m_fsRightText->SetValue(wxString::Format("%.1f", m_pSti->GetFullScaleLevel(CH_RIGHT)));
   
   // Init default calibration level (should be 94.0 dB)
   m_leftCalLevelText->SetValue(wxString::Format("%.1f", m_pSti->GetCalibrationLevel(CH_LEFT)));
   m_rightCalLevelText->SetValue(wxString::Format("%.1f", m_pSti->GetCalibrationLevel(CH_RIGHT)));
   
   // Init First Arrive Threshold Percentage (default 20 %)
   m_fatText->SetValue(wxString::Format("%.1f", 100.0 * m_pSti->GetFirstArriveThreshold()));

   FillSigNoiseGrid();

   // Set grid colors
   Aurora::UpdateGridBackgroundColor(m_splTable);

   // init grids contextual menu event
   m_splTableContextMenu.reset(new wxMenu);   
   m_splTableContextMenu->Append(ID_SPL_COPY_SELECTION_MENU, "&Copy\tCtrl+C");
   m_splTableContextMenu->Connect(wxEVT_COMMAND_MENU_SELECTED,  
                                  wxCommandEventHandler(Aurora::STISetupDialog::OnSplTableCopySelection), 
                                  NULL, this );
   m_splTable->Connect(wxEVT_CHAR, 
                       wxCharEventHandler(Aurora::STISetupDialog::OnSplTableKeyPress), 
                       NULL, this);
}

Aurora::STISetupDialog::~STISetupDialog()
{
   m_splTableContextMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,
                                     wxCommandEventHandler(Aurora::STISetupDialog::OnSplTableCopySelection), 
                                     NULL, this );
   m_splTable->Disconnect(wxEVT_CHAR, 
                          wxCharEventHandler(Aurora::STISetupDialog::OnSplTableKeyPress), 
                          NULL, this);
}


//----------------------------------------------------------------------------
// STIShowDialog
//----------------------------------------------------------------------------
void Aurora::STIShowDialog::UpdateTables()
{
    wxString value;
    
    auto& results  = m_pSti->GetResults(m_nCurrentChannel);
    auto& modFrequencies = results.tMTFf.GetFrequencies();
    
    int row = 0;
    int col = 0;
    
    for(auto mf : modFrequencies)
    {
        col = 0;
        auto& spect = results.tMTFf.GetSpectrum(mf);
        
        spect.ForEach(false, [&](const size_t i, const float fcb, double& mtfValue)
        {
            value.Printf("%.3f", mtfValue);
            m_mtfTable->SetCellValue(row, col, value);
            col++;
        });
        row++;
    }
    col = 0;
    
    results.aMTI.ForEach(false, [&](const size_t i, const float fcb, double& mtiValue)
    {
        value.Printf("%.3f", mtiValue);
        m_stiTable->SetCellValue(0, col, value);
        ++col;
    });

    value.Printf("%.3f", results.STImale);
    m_stiMaleValue->SetLabel(value);

    value.Printf("%.3f", results.STIfemale);
    m_stiFemaleValue->SetLabel(value);

    value.Printf("%.3f", results.RaSTI);
    m_rastiValue->SetLabel(value);   

    value.Printf("%.3f", results.STItel);
    m_stitelValue->SetLabel(value);

    value.Printf("%.3f", results.STIpa);
    m_stipaValue->SetLabel(value);
}


void Aurora::STIShowDialog::UpdatePlot()
{
    double adbSerie[14];
    //Some cleaning...
    m_pPlot->ClearSerie();
    m_pPlot->GetLegend()->Clear();

    auto& results  = m_pSti->GetResults(m_nCurrentChannel);
    auto& modFrequencies = results.tMTFf.GetFrequencies();
    auto fcbs = results.tMTFf.GetSpectrum(modFrequencies[0]).RangedFrequencies();
    
    int i = 0;
    
    for(auto fcb : fcbs)
    {
        int np = 0;
        Aurora::STISpectrum::Band band(fcb);
        
        for(auto mf : modFrequencies)            
        {
            adbSerie[np] = 100.0 * results.tMTFf.GetValue(mf, fcb);
            np++;
        }
        wxColour col;
        Aurora::ChooseColour(col, i++);

        wxString label = band.GetShortLabel();
        label << " Hz";

        auto ds = new Aurora::DataSerie(14);
        ds->SetLabel(label);
        ds->SetLineColour(col);
        ds->Copy(adbSerie, 14);

        m_pPlot->AddSerie(ds);
    }
    
    m_pPlot->Refresh();
}


void Aurora::STIShowDialog::OnMtfTableRigtClick( wxGridEvent& event )
{
    PopupMenu(m_mtfTableContextMenu.get());
}

void Aurora::STIShowDialog::OnMtfTableKeyPress( wxKeyEvent& event) 
{
#ifndef __WXMAC__
    if( (event.GetModifiers() == 3) )              // Ctrl+C  ascii translated code.
#else
    if( (event.GetModifiers() == wxMOD_CMD) &&    // Command+C  on Apple machines
        (event.GetKeyCode() == 99) )
#endif
    {   
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_MTF_COPY_SELECTION_MENU);
        OnMtfTableCopySelection(cmd);
    }
}

void Aurora::STIShowDialog::OnSTITableRightClick( wxGridEvent& event )
{
    PopupMenu(m_stiTableContextMenu.get());
}

void Aurora::STIShowDialog::OnSTITableKeyPress( wxKeyEvent& event) 
{
#ifndef __WXMAC__
    if( (event.GetModifiers() == 3) )              // Ctrl+C  ascii translated code.
#else
    if( (event.GetModifiers() == wxMOD_CMD) &&    // Command+C  on Apple machines
    	 event.GetKeyCode() == 'c' )
#endif
    {   
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_STI_COPY_SELECTION_MENU);
        OnSTITableCopySelection(cmd);
    }
}

void Aurora::STIShowDialog::OnLeftRadio( wxCommandEvent& event )
{
    m_nCurrentChannel = CH_LEFT;
    UpdateTables();
    UpdatePlot();
    m_pPlot->Refresh();
}

void Aurora::STIShowDialog::OnRightRadio( wxCommandEvent& event )
{
    m_nCurrentChannel = CH_RIGHT;
    UpdateTables();
    UpdatePlot();
    m_pPlot->Refresh();
}

void Aurora::STIShowDialog::OnSnCorrectionCheck( wxCommandEvent& event )
{
    m_pSti->SetSNCorrection(event.IsChecked());
    m_pSti->Compute(CH_LEFT);
    
    if(m_bIsStereo) 
    {
        m_pSti->Compute(CH_RIGHT);
    }
    UpdateTables();
    UpdatePlot();
}

void Aurora::STIShowDialog::OnMaskCorrectionCheck( wxCommandEvent& event )
{
    m_pSti->SetMaskingCorrection(event.IsChecked());
    m_pSti->Compute(CH_LEFT);
    
    if(m_bIsStereo) 
    {
        m_pSti->Compute(CH_RIGHT);
    }    
    UpdateTables();
    UpdatePlot();
}

void Aurora::STIShowDialog::OnSave( wxCommandEvent& event )
{
    Aurora::STIShowExports exporter(m_pSti, m_pSti->IsStereo());
    exporter.SavePrompt();
}

void Aurora::STIShowDialog::OnHelp( wxCommandEvent& event )
{
   wxString path = "Docs/sti.html";
   wxFileName help_path(path);
   help_path.MakeAbsolute();

   ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::STIShowDialog::OnOk( wxCommandEvent& event )
{
    EndModal(true);
}

void Aurora::STIShowDialog::OnMtfTableCopySelection(wxCommandEvent & event)
{
    Aurora::STIShowExports exporter(m_pSti, m_pSti->IsStereo());
    exporter.CopyGridSelectionToClipboard(m_mtfTable);
}

void Aurora::STIShowDialog::OnSTITableCopySelection(wxCommandEvent & event)
{
    Aurora::STIShowExports exporter(m_pSti, m_pSti->IsStereo());
    exporter.CopyGridSelectionToClipboard(m_stiTable);   
}


// 'ctors
Aurora::STIShowDialog::STIShowDialog(wxWindow* parent,
                                     Aurora::STIEffect* sti, 
                                     const bool bIsStereo )
 : STIShowDlg( parent ),
   m_pSti(sti),
   m_bIsStereo(bIsStereo),
   m_nCurrentChannel(CH_LEFT)
{
    // Set caption
    wxString title("Aurora for Audacity - Speech Transmission Index Calculation Module - (v.");
    title << Aurora::ModuleVersionNumber(Aurora::ReleaseType::ART_BETA, 0) << ")";
    SetTitle(title);

    // Draw logo
    wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("Aurora_logo")), 
                    0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, 
                                       wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("sti_logo")), 
                     0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();

    // Create plot & plot notebook page
    m_plotPage = new wxPanel( m_notebook);
    wxBoxSizer* plotSizer = new wxBoxSizer( wxVERTICAL );
    
    m_pPlot = new Aurora::STIPlot(m_plotPage, ID_PLOT);
    plotSizer->Add( m_pPlot, 1, wxALL|wxEXPAND, 5 );
    
    m_plotPage->SetSizer( plotSizer );
    m_plotPage->Layout();
    plotSizer->Fit( m_plotPage );
        
    m_notebook->AddPage( m_plotPage, "MTF Values Graph", true );

    // Initialize plot
    const wxString frqLabels[] = { "0.63", "0.8", "1.0", "1.25",
                                   "1.6",  "2.0", "2.5", "3.15",
                                   "4.0",  "5.0", "6.3", "8.0",
                                   "10.0", "12.5" };

    m_pPlot->GetFrqAxis().SetTitle("M");
    m_pPlot->GetFrqAxis().ShowTitle(true);
    m_pPlot->GetFrqAxis().ShowGrid(true);
    m_pPlot->GetFrqAxis().AddCategories(frqLabels, 14);

    m_pPlot->GetSTIAxis().SetTitle("%");
    m_pPlot->GetSTIAxis().ShowTitle(true);
    m_pPlot->GetSTIAxis().ShowGrid(true);
    m_pPlot->GetSTIAxis().SetRange(0.0, 100.0, Aurora::Axis::RF_INT);

    m_pPlot->GetLegend()->Show(true);


   // Disable channels check buttons if mono analysis
    if(!m_bIsStereo)
    {
        m_leftRadioButton->Disable();
        m_rightRadioButton->Disable();
    }
    
    Aurora::UpdateGridBackgroundColor(m_mtfTable);
    Aurora::UpdateGridBackgroundColor(m_stiTable);

    // init grids contextual menu event
    m_mtfTableContextMenu.reset(new wxMenu);   
    m_mtfTableContextMenu->Append(ID_MTF_COPY_SELECTION_MENU, "&Copy\tCtrl+C");
    m_mtfTableContextMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, 
                                   wxCommandEventHandler(Aurora::STIShowDialog::OnMtfTableCopySelection),
                                   NULL, this );
    m_mtfTable->Connect(wxEVT_CHAR, 
                        wxCharEventHandler(Aurora::STIShowDialog::OnMtfTableKeyPress), 
                        NULL, this);

    m_stiTableContextMenu.reset(new wxMenu);  
    m_stiTableContextMenu->Append(ID_STI_COPY_SELECTION_MENU, "&Copy\tCtrl+C");
    m_stiTableContextMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Aurora::STIShowDialog::OnSTITableCopySelection), NULL, this );
    m_stiTable->Connect(wxEVT_CHAR, 
                        wxCharEventHandler(Aurora::STIShowDialog::OnSTITableKeyPress), 
                        NULL, this);

    // Initialize values
    m_leftRadioButton->SetValue(true); // It always starts with left channel selected
    m_snCorrectionCheck->SetValue(m_pSti->IsSNCorrection());
    m_maskCorrectionCheck->SetValue(m_pSti->IsMaskingCorrection());
    
    m_pSti->Compute(CH_LEFT);
    
    if(m_bIsStereo) 
    {
        m_pSti->Compute(CH_RIGHT);
    }
    UpdateTables();
    UpdatePlot();
}

Aurora::STIShowDialog::~STIShowDialog()
{
   m_mtfTableContextMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,
                                     wxCommandEventHandler(Aurora::STIShowDialog::OnMtfTableCopySelection), 
                                     NULL, this );
   m_mtfTable->Disconnect(wxEVT_CHAR, 
                          wxCharEventHandler(Aurora::STIShowDialog::OnMtfTableKeyPress), 
                          NULL, this);
   m_stiTableContextMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED,
                                     wxCommandEventHandler(Aurora::STIShowDialog::OnSTITableCopySelection), 
                                     NULL, this );
   m_stiTable->Disconnect(wxEVT_CHAR, 
                          wxCharEventHandler(Aurora::STIShowDialog::OnSTITableKeyPress), 
                          NULL, this);
}


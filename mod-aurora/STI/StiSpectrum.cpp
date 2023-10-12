/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiSpectrum.cpp

  Simone Campanini

**********************************************************************/

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
// STISpectrumGridCellColoredRectRenderer implementation
//----------------------------------------------------------------------------
class STISpectrumGridCellColoredRectRenderer : public wxGridCellStringRenderer
{
    public:
    void Draw(wxGrid &grid, 
                wxGridCellAttr& attr, 
                wxDC &dc,
                const wxRect& rect, 
                int nRow, 
                int nCol, 
                bool bIsSelected) override
    {
        wxGridCellStringRenderer::Draw(grid, attr, dc, rect, nRow, nCol, bIsSelected);

        wxColour col;
        Aurora::ChooseColour(col, nRow);

        wxRect r = rect;
        r.SetWidth(r.width/2);
        r.SetHeight(r.height/2);
        r.x += (rect.width - r.width)/2;
        r.y += (rect.height - r.height)/2; 

        dc.SetPen(*wxBLACK);
        dc.SetBrush(wxBrush(col));
        dc.DrawRectangle(r);
    }
};

//----------------------------------------------------------------------------
// STISpectrumDialog implementation
//----------------------------------------------------------------------------

void Aurora::STISpectrumDialog::FillTable()
{
    auto& results = m_pSti->GetResults(m_nCurrentChnl);
    wxString value;
    
    int  col = 1;
    
    results.noiseSpectrum.ForEach(false, 
                                  [&](const size_t i, const float fcb, double& noiseValue)
    {
        value.Printf("%.1f", noiseValue); 
        m_spectrumTable->SetCellValue(0, col, value);
        
        value.Printf("%.1f", results.signalSpectrum[fcb]);
        m_spectrumTable->SetCellValue(1, col, value);
        
        value.Printf("%.1f", results.sigNoiseSpectrum[fcb]);
        m_spectrumTable->SetCellValue(2, col, value);
        ++col;
    });
}

void Aurora::STISpectrumDialog::OnCloseDialog( wxCloseEvent& event )
{ 
    StoreGeometry();
    EndModal(true);     
}

void Aurora::STISpectrumDialog::OnChannelSelect( wxCommandEvent& event )
{
    if(event.GetSelection() == m_nCurrentChnl) 
    {
        return;
    }
    
    m_nCurrentChnl = event.GetSelection();
    auto& results = m_pSti->GetResults(m_nCurrentChnl);
    
    m_spectrumPlot->ReplaceSerie(0, results.noiseSpectrum);
    m_spectrumPlot->ReplaceSerie(1, results.signalSpectrum);
    m_spectrumPlot->ReplaceSerie(2, results.sigNoiseSpectrum);
    
    FillTable();    
    m_spectrumPlot->Refresh();
}

void Aurora::STISpectrumDialog::OnLevelTableLabelClick( wxGridEvent& event )
{
    m_spectrumPlot->ToggleSerieVisibility(event.GetRow());
    m_spectrumPlot->Refresh();
}

void Aurora::STISpectrumDialog::OnLevelTableRightClick( wxGridEvent& event )
{
    PopupMenu(m_levelContextMenu);
}

void Aurora::STISpectrumDialog::OnLevelTableKeyPress( wxKeyEvent& event )
{
#ifndef __WXMAC__
    if( (event.GetModifiers() == 3) )              // Ctrl+C  ascii translated code.
#else
    if( (event.GetModifiers() == wxMOD_CMD) &&    // Command+C  on Apple machines
    	 event.GetKeyCode() == 99 )
#endif
    {   
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_LEVEL_COPY_SELECTION_MENU);
        OnLevelTableCopySelection(cmd);
    }
}

void Aurora::STISpectrumDialog::OnLevelTableCopySelection(wxCommandEvent & event)
{
    Aurora::STISpectrumExports exporter(m_pSti, m_pSti->IsStereo());
    exporter.CopyGridSelectionToClipboard(m_spectrumTable);
}

void Aurora::STISpectrumDialog::StoreGeometry()
{
    Aurora::Config cfg;
    int nWidth, nHeight;
    this->GetSize(&nWidth, &nHeight);

    int nHorzSplitPos = m_verticalSplitter->GetSashPosition();

    cfg.Write("/Aurora/STI/SpectrumDialogWidth", nWidth);
    cfg.Write("/Aurora/STI/SpectrumDialogHeight", nHeight);
    cfg.Write("/Aurora/STI/SpectrumDialogSplitterPosition", nHorzSplitPos);
}

void Aurora::STISpectrumDialog::RestoreGeometry()
{
    Aurora::Config cfg;
    int nPos;
    
    if(cfg.Read("/Aurora/STI/SpectrumDialogSplitterPosition", &nPos))
    {
        m_verticalSplitter->SetSashPosition(nPos);
    }
    int nWidth, nHeight;
    
    if(cfg.Read("/Aurora/STI/SpectrumDialogWidth", &nWidth) &&
       cfg.Read("/Aurora/STI/SpectrumDialogHeight", &nHeight))
    {
        this->SetSize(nWidth, nHeight);
    }
}

Aurora::STISpectrumDialog::STISpectrumDialog(wxWindow* pParent,
                                             Aurora::STIEffect* sti,
                                             const bool bIsStereo)
  : STISpectrumDlg(pParent),
    m_pSti(sti),
    m_bIsStereo(bIsStereo)
{
    // Set caption
    wxString title("Aurora for Audacity - Octave Bands Analisys - (v.");
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

    // Set data
    auto& results = m_pSti->GetResults(CH_LEFT);

    wxArrayString bandLabels;
    const auto& fcbs = results.noiseSpectrum.Frequencies();

    for (auto fcb : fcbs)
    {
        bandLabels.Add(Aurora::GetBandShortLabel(fcb));
    }
    bandLabels.Add("A");
    bandLabels.Add("Lin");

    m_spectrumPlot->GetBandsAxis().SetTitle("Octave Bands [Hz]");
    m_spectrumPlot->GetBandsAxis().ShowTitle(true);
    m_spectrumPlot->GetBandsAxis().AddCategories(bandLabels);

    m_spectrumPlot->GetLevelAxis().SetTitle("Level [dB]");
    m_spectrumPlot->GetLevelAxis().ShowTitle(true);
    m_spectrumPlot->GetLevelAxis().ShowGrid(true);
    m_spectrumPlot->GetLevelAxis().SetRange(30.0, 130.0, Aurora::Axis::RF_REAL);    
    
    m_spectrumPlot->AddSerie(results.noiseSpectrum);
    m_spectrumPlot->AddSerie(results.signalSpectrum);
    m_spectrumPlot->AddSerie(results.sigNoiseSpectrum);
    
    if(!m_bIsStereo) 
    {
        m_channelSelectLabel->Disable();
        m_channelSelectChoice->Disable();
    }

    Aurora::UpdateGridBackgroundColor(m_spectrumTable);

    wxGridCellAttr*  pCellAttr = new wxGridCellAttr();
    pCellAttr->SetReadOnly();
    
    for(int nRow = 0; nRow < 3; nRow++)
    {
        m_spectrumTable->SetCellRenderer(nRow, 0, new STISpectrumGridCellColoredRectRenderer);
        m_spectrumTable->SetRowAttr(nRow, pCellAttr->Clone());
    }
#ifdef __WXGTK__
    m_spectrumTable->SetRowLabelSize(140);
#endif
    FillTable();
    
    RestoreGeometry();

    m_spectrumPlot->Refresh();

    // init table context menu
    m_levelContextMenu = new wxMenu;   
    m_levelContextMenu->Append(ID_LEVEL_COPY_SELECTION_MENU, "&Copy\tCtrl+C");
    m_levelContextMenu->Connect(wxEVT_COMMAND_MENU_SELECTED, 
                                wxCommandEventHandler(Aurora::STISpectrumDialog::OnLevelTableCopySelection), 
                                NULL, this );
    m_spectrumTable->Connect(wxEVT_CHAR, wxCharEventHandler(Aurora::STISpectrumDialog::OnLevelTableKeyPress), 
                             NULL, this);
}

Aurora::STISpectrumDialog::~STISpectrumDialog()
{
   m_levelContextMenu->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(Aurora::STISpectrumDialog::OnLevelTableCopySelection), 
                                  NULL, this );
   m_spectrumTable->Disconnect(wxEVT_CHAR,
                               wxCharEventHandler(Aurora::STISpectrumDialog::OnLevelTableKeyPress), 
                               NULL, this);
}

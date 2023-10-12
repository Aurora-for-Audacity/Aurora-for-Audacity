/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  ThAnalyzerUi.cpp

  Simone Campanini

*******************************************************************//**

\class TimeHistoryAnalyzerDialog 
\brief Dialog used with EffectTimeHistoryAnalyzer.

*//****************************************************************//**

\class TimeHistoryAnalyzerShow
\brief Window that shows the time history analysis.

*//*******************************************************************/
#include <aurora.h>

#include "TimeHistoryAnalyzer.h"
#include "ThAnalyzerDialogs.h"
#include "ThAnalyzerPlot.h"
#include "ThAnalyzerExports.h"
#include "ThAnalyzerEffect.h"
#include "ThAnalyzerDialogs.h"

#include "ThAnalyzerUi.h"

//----------------------------------------------------------------------------
// THAGridCellColoredRectRenderer implementation
//----------------------------------------------------------------------------
class ThaGridCellColoredRectRenderer : public wxGridCellStringRenderer
{
  public:
    void Draw(wxGrid &grid, 
              wxGridCellAttr& attr, 
              wxDC& dc,
              const wxRect& rect, 
              int row, 
              int col, 
              bool isSelected) override
    {
        wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

        wxColour fillColor;
        Aurora::ChooseColour(fillColor, col);

        wxRect r = rect;
        r.SetWidth(r.width/2);
        r.SetHeight(r.height/2);
        r.x += (rect.width - r.width)/2;
        r.y += (rect.height - r.height)/2; 

        dc.SetPen(*wxBLACK);
        dc.SetBrush(wxBrush(fillColor));
        dc.DrawRectangle(r);
    }
};

//----------------------------------------------------------------------------
// TimeHistoryAnalyzerFrame implementation
//----------------------------------------------------------------------------
void Aurora::TimeHistoryAnalyzerFrame::UpdateResults()
{
    //Setup grid
    int nCh;
    int nChnlsCount = (int)m_pEffect->GetSignalTracks().size();
    int nColsCount  = m_resultsGrid->GetNumberCols();
    int nNeededCols = (nChnlsCount+1) - nColsCount; //there is also the Avg column

    if(nNeededCols > 0)
    {
        m_resultsGrid->AppendCols(nNeededCols);
        InitColumns(nColsCount, nChnlsCount);
    }
    else if(nNeededCols < 0)
    {
        m_resultsGrid->DeleteCols(nChnlsCount+1, -nNeededCols);
    }

    wxString label;

    for(nCh = 0; nCh < nChnlsCount; nCh++)
    {
        label =  m_pEffect->GetSignalTrack(nCh).GetName();
        FillColumn(nCh, nChnlsCount, label);
        m_resultsGrid->AutoSizeColumn(nCh);
    }
    // Avg channel
    label = "Avg";
    FillColumn(nChnlsCount, nChnlsCount, label);

    //::AuroraUpdateGridBackgroundColor(m_resultsGrid);

    m_nCurrentChannel = 0;

    // ----------------- Plot update -----------------
    auto& track = m_pEffect->GetSignalTrack(m_nCurrentChannel);
    const double fullScale = track.GetFullScale();

    m_pPlot->GetTimeAxis().SetFormat(Aurora::Axis::RF_REAL);
    m_pPlot->GetLevelAxis().SetRange(0.0, fullScale, Aurora::Axis::RF_LINEARdB);

    m_pPlot->SetHorzAxisAutorange(true);
    m_pPlot->SetVertAxisAutorange(false);

    m_pPlot->ClearSerie();

    wxColour lineColor;
    Aurora::ChooseColour(lineColor, m_nCurrentChannel);


    auto& results = m_pEffect->GetResults();

    for(int nPlotType = 0; 
            nPlotType < Aurora::TimeHistoryAnalyzer::N_TIME_CONST; nPlotType++)
    {
        auto& dataBuffer = results[m_nCurrentChannel].GetBuffer(nPlotType);
        
        auto tds = new Aurora::ThAnalyzerTimeDataSerie(dataBuffer,
                                                       // Samplerate: there is a sample every 1 ms.
                                                       1000.0,          
                                                       fullScale);
        tds->SetLineColour(lineColor);
        m_pPlot->AddSerie(tds);
    }
    m_pPlot->ShowSerie(size_t(m_nCurrentPlotType));
    m_pPlot->Refresh();

    if(! m_plotTypeChoice->IsEnabled())
    {
        m_plotTypeChoice->Enable();
    }
}

void Aurora::TimeHistoryAnalyzerFrame::InitColumns(const int nStartCol, 
                                                   const int nEndCol)
{
    int nRow;

    for(int nCol = nStartCol;
            nCol < ((nEndCol < 0) ? m_resultsGrid->GetNumberCols() : nEndCol);
            nCol ++)
    {
        for(nRow = 0; nRow < (TimeHistoryAnalyzer::N_PARAMETERS + 2); nRow++)
        {
            m_resultsGrid->SetCellBackgroundColour(nRow, nCol, *wxWHITE);
        }
        m_resultsGrid->SetCellRenderer(0, nCol, new ThaGridCellColoredRectRenderer);
    }

}

void Aurora::TimeHistoryAnalyzerFrame::FillColumn(const int nCh, 
                                                  const int nChnlsCount, 
                                                  wxString& label)
{
    m_resultsGrid->SetColLabelValue(nCh, label);

    wxString value;

    if(nCh < nChnlsCount)
    {
        switch(m_pEffect->GetSignalTrack(nCh).GetSelectedFilter())
        {
            case Aurora::AudioTrack::FLT_LIN: value = "Wide Band";       break;
            case Aurora::AudioTrack::FLT_ITU: value = "ITU BandPass";    break;
            case Aurora::AudioTrack::FLT_IEC: value = "IEC A-weighting"; break;
            case Aurora::AudioTrack::FLT_ISO: value = "ISO whole body";  break;
            case Aurora::AudioTrack::FLT_UNI: value = "UNI LP 250 Hz";   break;
        }
    }
    else
    {
        value = "-";
    }
    m_resultsGrid->SetCellValue(1, nCh, value);

    int nParId = Aurora::TimeHistoryAnalyzer::THA_FSL;
    
    if(nCh == nChnlsCount) 
    {
        nParId++;
    }
    auto& results = m_pEffect->GetResults();

    while(nParId < Aurora::TimeHistoryAnalyzer::THA_DUR)
    {
        value.Printf("%.2f", results[nCh].GetParameter(nParId));
        m_resultsGrid->SetCellValue(nParId+2, nCh, value);
        nParId++;
    }

    value.Printf("%.3f", results[nCh].GetParameter(nParId));
    m_resultsGrid->SetCellValue(nParId+2, nCh, value);
    nParId++;

    value = results[nCh].IsImpulsive() ? "yes" : "no";

    m_resultsGrid->SetCellValue(nParId+2, nCh, value);
}

void Aurora::TimeHistoryAnalyzerFrame::StoreGeometry()
{
    int nWidth, nHeight;
    this->GetSize(&nWidth, &nHeight);

    int nHorzSplitPos = m_pHorzSplitter->GetSashPosition();
    int nVertSplitPos = m_pVertSplitter->GetSashPosition();

    Aurora::Config cfg;
    
    cfg.Write("/Aurora/TimeHistoryAnalyzer/DialogWidth", nWidth);
    cfg.Write("/Aurora/TimeHistoryAnalyzer/DialogHeight", nHeight);
    cfg.Write("/Aurora/TimeHistoryAnalyzer/HorzSplitterPosition", nHorzSplitPos);
    cfg.Write("/Aurora/TimeHistoryAnalyzer/VertSplitterPosition", nVertSplitPos);
}

void Aurora::TimeHistoryAnalyzerFrame::RestoreGeometry()
{
    int nWidth = 800, nHeight = 600;
    Aurora::Config cfg;
    
    if(cfg.Read("/Aurora/TimeHistoryAnalyzer/DialogWidth", &nWidth) &&
       cfg.Read("/Aurora/TimeHistoryAnalyzer/DialogHeight", &nHeight))
    {
        this->SetSize(nWidth, nHeight);
    }

    int nPos;

    if(cfg.Read("/Aurora/TimeHistoryAnalyzer/HorzSplitterPosition", &nPos))
    {
        m_pHorzSplitter->SetSashPosition(nPos);
    } 

    if(cfg.Read("/Aurora/TimeHistoryAnalyzer/VertSplitterPosition", &nPos))
    {
        m_pVertSplitter->SetSashPosition(nPos);
    }

}

// ---- events handlers
void Aurora::TimeHistoryAnalyzerFrame::OnCloseDialog( wxCloseEvent& event )
{
	  StoreGeometry();
    EndModal(false);
}

void Aurora::TimeHistoryAnalyzerFrame::OnFullscaleGridContentChanged( wxGridEvent& event )
{
    int nRow = event.GetRow();
    int nCol = event.GetCol();
    int nTracks = int(m_pEffect->GetSignalTracks().size());

    wxASSERT_MSG(nCol < nTracks,
                 "THADlg: The requested column is out of range!!!");

    switch(nRow)
    {
        case 0: // track name row
        {
            wxString name = m_fullscalesGrid->GetCellValue(nRow, nCol);
            wxString label;
            label << "Ch " << nCol+1 << "/" <<  name;
            m_calibrationGrid->SetColLabelValue(nCol, label);
            m_calibrationGrid->AutoSizeColumn(nCol); 
            m_aCalibrationColsLabels[nCol] = label;
            break;
        }


        case 2: // fullscales row
        {
            for(int nCh = 0; nCh < nTracks; nCh++)
            {
                m_fullscalesGrid->SetCellBackgroundColour(2, nCh, *wxWHITE);
            }
            break;
        }
    }
}

void Aurora::TimeHistoryAnalyzerFrame::OnAnalyze( wxCommandEvent& event )
{
    int nCh = 0;
    
    Aurora::Config cfg;
    auto& signalTracks = m_pEffect->GetSignalTracks();

    for(int nCol = 0; nCol < (int)signalTracks.size(); nCol++)
    {
        wxString trackName = m_fullscalesGrid->GetCellValue(0, nCol);

        if(trackName != "None")
        {
            wxString filterName = m_fullscalesGrid->GetCellValue(1, nCol);

            wxString value = m_fullscalesGrid->GetCellValue(2, nCol);
            const double fsValue = ::StringToDouble(value);

            wxString path;
            // store fullscale value to configuration file
            path.Printf("/Aurora/TimeHistoryAnalyzer/FullScale_Ch_%d", nCh + 1);
            cfg.Write(path, fsValue);

            value = m_calibrationGrid->GetCellValue(1, nCol);
            const double refValue = ::StringToDouble(value);

            // store reference value to configuration file
            path.Printf(wxT("/Aurora/TimeHistoryAnalyzer/Calibration_Ch_%d"), nCh + 1);
            cfg.Write(path, refValue);

            cfg.Flush();
            
            const int index = m_pEffect->GetTrackIndexFromName(trackName);
            signalTracks[nCh].SetName(trackName);
            signalTracks[nCh].SetIndex(index);
            signalTracks[nCh].SetFilterFromString(filterName);
            signalTracks[nCh].SetFullscale(fsValue);
            nCh++;
        }
    }

    if(nCh == 0)
    {
        Aurora::TimeHistoryAnalyzerEffect::MessageBox("No tracks selected. Please "
                                                      "choose almost one...",
                                                      Aurora::MessageType::Error);
        return;
    }

    if(! m_pEffect->DoAnalysis())
    {
        Aurora::TimeHistoryAnalyzerEffect::MessageBox("An error has been occourred "
                                                      "during analysis process.",
                                                      Aurora::MessageType::Error);
        return;
    }

    UpdateResults();
}

void Aurora::TimeHistoryAnalyzerFrame::OnRemoveDCCheck( wxCommandEvent& event )
{
    m_pEffect->EnableRemoveDC(event.IsChecked());
}

void Aurora::TimeHistoryAnalyzerFrame::OnCalibrationGridContentChanged( wxGridEvent& event )
{}

void Aurora::TimeHistoryAnalyzerFrame::OnCalibrate( wxCommandEvent& event )
{
    int nCh = 0;
    double refValue;

    auto& calibTracks = m_pEffect->GetCalibrationTracks();
    calibTracks.clear();
    
    wxString trackName, value;

    if(m_ch1ForAllCheck->IsChecked())
    {
        // get Ch 1 track name
        value = m_calibrationGrid->GetCellValue(0, 0);

        if(value == "None")
        {
            Aurora::TimeHistoryAnalyzerEffect::MessageBox("No calibration track selected.",
                                                          Aurora::MessageType::Error);
            return;
        }

        trackName = m_calibrationGrid->GetCellValue(0, 0);
        value     = m_calibrationGrid->GetCellValue(1, 0);
        refValue  = ::StringToDouble(value);

        const int index = m_pEffect->GetTrackIndexFromName(trackName);
     
        calibTracks.resize(1);
        calibTracks[0].SetName(trackName);
        calibTracks[0].SetIndex(index);
        calibTracks[0].SetReferenceLevel(refValue);

        std::vector<double> fullScales;
        fullScales.resize(1);        

        if(!m_pEffect->DoFullscaleCalibration(fullScales))
        {
            calibTracks.clear();

            Aurora::TimeHistoryAnalyzerEffect::MessageBox("Calibration failed!",
                                                          Aurora::MessageType::Error);
            return;
        }

        // Update Fullscale grid
        value.Printf("%.2f", fullScales[0]);

        for(nCh = 0; nCh < int(m_pEffect->GetSignalTracks().size()); nCh++)
        {
            m_fullscalesGrid->SetCellValue(2, nCh, value);
            m_fullscalesGrid->SetCellBackgroundColour(2, nCh, *wxGREEN);
        }
    }
    else
    {
        const size_t nTracks = m_pEffect->GetSignalTracks().size();     
        calibTracks.resize(nTracks);
        
        for(nCh = 0; nCh < (int)nTracks; nCh++)
        {
            trackName = m_calibrationGrid->GetCellValue(0, nCh);

            if(trackName != "None")
            {
                value  = m_calibrationGrid->GetCellValue(1, nCh);
                refValue = ::StringToDouble(value);
     
                const int index = m_pEffect->GetTrackIndexFromName(trackName);
                calibTracks[nCh].SetName(trackName);
                calibTracks[nCh].SetIndex(index);
                calibTracks[nCh].SetReferenceLevel(refValue);
            }
        }

        std::vector<double> fullScales(nTracks, 0.0);

        if(!m_pEffect->DoFullscaleCalibration(fullScales))
        {
            calibTracks.clear();

            Aurora::TimeHistoryAnalyzerEffect::MessageBox("Calibration failed!",
                                                          Aurora::MessageType::Error);
            return;
        }

        // Update Fullscale grid
        for(nCh = 0; nCh < (int)nTracks; nCh++)
        {
            value.Printf("%.2f", fullScales[nCh]);
            m_fullscalesGrid->SetCellValue(2, nCh, value);
            m_fullscalesGrid->SetCellBackgroundColour(2, nCh, *wxGREEN);
        }
    }
    calibTracks.clear();

    Aurora::TimeHistoryAnalyzerEffect::MessageBox("Calibration done successifully!",
                                                  Aurora::MessageType::Info);
}

void Aurora::TimeHistoryAnalyzerFrame::OnApplyToAllCheck( wxCommandEvent& event )
{
    int nCol;
    
    if(event.IsChecked())
    {
        wxString value = "All channels ";
        m_calibrationGrid->SetColLabelValue(0, value);

        value = " ";

        for(nCol = 1; nCol < int(m_pEffect->GetSignalTracks().size()); nCol++ )
        {
            m_calibrationGrid->SetColLabelValue(nCol, value);
            m_calibrationGrid->SetCellBackgroundColour(0, nCol, Aurora::GetCanvasColour());
            m_calibrationGrid->SetCellBackgroundColour(1, nCol, Aurora::GetCanvasColour());
        }
    }
    else
    {
        m_calibrationGrid->SetColLabelValue(0, m_aCalibrationColsLabels[0]);

        for(nCol = 1; nCol < int(m_pEffect->GetSignalTracks().size()); nCol++ )
        {
            m_calibrationGrid->SetColLabelValue(nCol, m_aCalibrationColsLabels[nCol]);
            m_calibrationGrid->SetCellBackgroundColour(0, nCol, *wxWHITE);
            m_calibrationGrid->SetCellBackgroundColour(1, nCol, *wxWHITE);
        }
    }
}

void Aurora::TimeHistoryAnalyzerFrame::OnSelectDisplay( wxCommandEvent& event )
{
    m_nCurrentPlotType = event.GetSelection();
    m_pPlot->ShowSerie(m_nCurrentPlotType);
    m_pPlot->Refresh();
}

void Aurora::TimeHistoryAnalyzerFrame::OnGridCellRightClick( wxGridEvent& event )
{
    PopupMenu(m_pContextMenu.get());
}

void Aurora::TimeHistoryAnalyzerFrame::OnTogglePlot( wxGridEvent& event )
{
    int nCh = event.GetCol();
    auto& results = m_pEffect->GetResults();

    if(nCh < (int)m_pEffect->GetSignalTracks().size())
    {
        m_nCurrentChannel = nCh;
        wxColour lineColor;
        Aurora::ChooseColour(lineColor, m_nCurrentChannel);
        
        const double fullScale = m_pEffect->GetSignalTrack(nCh).GetFullScale();

        for(int nPlotType = 0; 
                nPlotType < Aurora::TimeHistoryAnalyzer::N_TIME_CONST; 
                nPlotType++)
        {
            m_pPlot->ReplaceTimeSerie(nPlotType,
                                      results[m_nCurrentChannel].GetBuffer(nPlotType),
                                      fullScale,
                                      lineColor);
        }
        m_pPlot->ShowSerie(size_t(m_nCurrentPlotType));
        m_pPlot->Refresh();
    }
}

void Aurora::TimeHistoryAnalyzerFrame::OnExport( wxCommandEvent& event )
{
    m_exporter.SetCurrentPlotType(m_nCurrentPlotType);
    m_exporter.SaveToFile();
}

void Aurora::TimeHistoryAnalyzerFrame::OnHelp( wxCommandEvent& event )
{
    wxString path = wxT("");
    path << wxT("Docs") << TRAILING_SLASH << wxT("itup56.html");
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::TimeHistoryAnalyzerFrame::OnAddTracksBtn( wxCommandEvent& event )
{
    StoreGeometry();
    EndModal(true);
}

void Aurora::TimeHistoryAnalyzerFrame::OnOk( wxCommandEvent& event )
{
    StoreGeometry();
    EndModal(false);
}

void Aurora::TimeHistoryAnalyzerFrame::OnCopyTableToClipboard( wxCommandEvent& event )
{
    m_exporter.CopyEntireTableToClipboard();
}

void Aurora::TimeHistoryAnalyzerFrame::OnCopySelection( wxCommandEvent& event )
{
    m_exporter.CopyGridSelectionToClipboard(m_resultsGrid);
}

/** 'ctors */
Aurora::TimeHistoryAnalyzerFrame::TimeHistoryAnalyzerFrame(wxWindow* pParent,
                                                            Aurora::TimeHistoryAnalyzerEffect* effect)
  : THADialog(pParent),
    m_pEffect(effect),
    m_exporter(dynamic_cast<Aurora::TimeHistoryAnalyzer*>(effect))
{
    // Set caption
    wxString title("Aurora for Audacity - Time History Analyzer - (v.");
    title << Aurora::ModuleVersionString() << ")";
    SetTitle(title);

    // Setup graphics.
    InitArtProvider();

    //Setup logo
    wxBoxSizer* pLogoSizer;
    pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("Aurora_logo"),
    	                               wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0 ), 
                     0, wxALL, 5 );

    pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("tha_logo"),
    	                               wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0 ),
                    0, wxALL, 5 );

    m_logoPanel->SetSizer( pLogoSizer );
    m_logoPanel->Layout();

    // Setup track names list
    m_pEffect->SetTracksNamesArray(m_aTrackNames);
    m_aTrackNames.Insert(wxString("None"), 0);

    int nTracksCount = (int)m_pEffect->GetSignalTracks().size();
    int nColsCount   = m_fullscalesGrid->GetNumberCols();
    int nNeededCols  = (nTracksCount - nColsCount);
    int nCol         = 0;

    if(nNeededCols > 0)
    {
        m_fullscalesGrid->AppendCols(nNeededCols);
        m_calibrationGrid->AppendCols(nNeededCols);
    }

   // grid on setup page.

    m_fullscalesGrid->SetDefaultCellBackgroundColour(Aurora::GetCanvasColour());
    m_calibrationGrid->SetDefaultCellBackgroundColour(Aurora::GetCanvasColour());

    const wxString filterLabels[] = 
    { 
        "None",
        "ITU Band-Pass filter (160-5500 Hz)",
        "IEC \"A\" weighting filter",
        "ISO 2631 whole body combined",
        "UNI 9916 Low Pass 250 Hz" 
    };

    Aurora::Config cfg;
    double   dbValue;
    wxString value, path;

    for(nCol = 0; nCol < nTracksCount; nCol++)
    {
        value.Printf("Ch %d", nCol + 1);
        m_fullscalesGrid->SetColLabelValue(nCol, value);

        m_fullscalesGrid->SetCellEditor(0, nCol, new wxGridCellChoiceEditor(m_aTrackNames));
        m_fullscalesGrid->SetCellEditor(1, nCol, new wxGridCellChoiceEditor(5, filterLabels));
        m_fullscalesGrid->SetCellEditor(2, nCol, new wxGridCellFloatEditor(4,2)); // width 4, precision 2 (0.00)

        m_fullscalesGrid->SetCellValue(0, nCol, m_aTrackNames[nCol+1]);
        m_fullscalesGrid->SetCellValue(1, nCol, m_aTrackNames[0]); // Not the same meaning, but the same string...

        // Get stored data from configuration file
        path.Printf("/Aurora/TimeHistoryAnalyzer/FullScale_Ch_%d", nCol + 1);
        
        if(! cfg.Read(path, &dbValue))
        {
            dbValue = 120.0; // default fullscale value
        }

        value.Printf("%.2f", dbValue);
        m_fullscalesGrid->SetCellValue(2, nCol, value);

        m_fullscalesGrid->SetCellBackgroundColour(0, nCol, *wxWHITE);
        m_fullscalesGrid->SetCellBackgroundColour(1, nCol, *wxWHITE);
        m_fullscalesGrid->SetCellBackgroundColour(2, nCol, *wxWHITE);
    }
    m_removeDcCheck->SetValue(m_pEffect->IsRemoveDCEnabled());

    // grid on calibration page
    for(nCol = 0; nCol < nTracksCount; nCol++)
    {
        value.Printf("Ch %d", nCol+1);
        m_aCalibrationColsLabels.Add(value); // Backup...

        value << "/" << m_aTrackNames[nCol+1];
        m_calibrationGrid->SetColLabelValue(nCol, value);

        m_calibrationGrid->SetCellEditor(0, nCol, new wxGridCellChoiceEditor(m_aTrackNames));
        m_calibrationGrid->SetCellEditor(1, nCol, new wxGridCellFloatEditor(4,2)); // width 4, precision 2 (0.00)

        m_calibrationGrid->SetCellValue(0, nCol, m_aTrackNames[0]);

        // Get stored data from configuration file
        path.Printf("/Aurora/TimeHistoryAnalyzer/Calibration_Ch_%d", nCol + 1);

        if(! cfg.Read(path, &dbValue))
        {
            dbValue = 94.0; // default reference value
        }
        value.Printf("%.2f", dbValue);
        m_calibrationGrid->SetCellValue(1, nCol, value);

        m_calibrationGrid->SetCellBackgroundColour(0, nCol, *wxWHITE);
        m_calibrationGrid->SetCellBackgroundColour(1, nCol, *wxWHITE);
    }

    //Sizing..try to get text width
    wxFont font = m_fullscalesGrid->GetDefaultCellFont();
    size_t unTextWidth = 34; // The size of the longer filter label.

    for(int n = 0; n < nTracksCount; n++)
    {
        if(m_aTrackNames[n].Length() > unTextWidth) 
        {
            unTextWidth = m_aTrackNames[n].Length(); 
        }
    }

    int nFsColSize  = iround(unTextWidth * font.GetPointSize() * 0.6), // avg. font width.
    nCalColSize = iround((unTextWidth + 5) * font.GetPointSize() * 0.6); 

    for(nCol = 0; nCol < nColsCount; nCol++) // ???
    {
        m_fullscalesGrid->SetColSize(nCol, nFsColSize);
        m_calibrationGrid->SetColSize(nCol, nCalColSize);
    }

    // Columns that don't contain data should be grayed
    value = " ";
    for(nCol = nTracksCount; nCol < nColsCount; nCol++ )
    {
        m_fullscalesGrid->SetColLabelValue(nCol, value);
        m_calibrationGrid->SetColLabelValue(nCol, value);
    }

   // ------- init results table ------
   m_resultsGrid->AppendCols(nTracksCount);

   //m_resultsGrid->SetDefaultCellBackgroundColour(m_wxcolPanelsBckgnd); 
   m_resultsGrid->SetDefaultCellBackgroundColour(*wxWHITE);
   //m_resultsGrid->SetDefaultCellBackgroundColour(::AuroraGetCanvasColour());

    int nParId;
    for(nCol = 0; nCol < nTracksCount; nCol++)
    {
        value.Printf("Ch %d", nCol+1);
        m_resultsGrid->SetColLabelValue(nCol, value);

        m_resultsGrid->SetCellRenderer(0, nCol, new ThaGridCellColoredRectRenderer);
        m_resultsGrid->SetCellValue(1, nCol, "Wide Band");
        m_resultsGrid->SetCellBackgroundColour(1, nCol, *wxWHITE);

        for(nParId  = 2; nParId <= TimeHistoryAnalyzer::N_PARAMETERS; nParId++)
        {
            m_resultsGrid->SetCellValue(nParId, nCol, "-");
            m_resultsGrid->SetCellBackgroundColour(nParId, nCol, *wxWHITE);
        }
    }
    value = "Avg";
    m_resultsGrid->SetColLabelValue(nCol, value);

    for(nParId  = 1; nParId <= TimeHistoryAnalyzer::N_PARAMETERS; nParId++)
    {
        m_resultsGrid->SetCellValue(nParId, nCol, "-");
        m_resultsGrid->SetCellBackgroundColour(nParId, nCol, *wxWHITE);
    }

    m_resultsGrid->SetCellValue(1, nCol, wxT("-"));

//   ::AuroraUpdateGridBackgroundColor(m_resultsGrid);

   // ----------------- Plot Initialization -----------------
   m_plotTypeChoice->SetSelection(m_nCurrentPlotType);
    
   m_pPlot->GetTimeAxis().SetTitle("Time [s]");
   m_pPlot->GetTimeAxis().ShowTitle(true);
   m_pPlot->GetTimeAxis().ShowGrid(true);
   m_pPlot->GetTimeAxis().SetRange(0.0, 10.0, Aurora::Axis::RF_REAL);

   m_pPlot->GetLevelAxis().SetTitle("Level [dB]");
   m_pPlot->GetLevelAxis().ShowTitle(true);
   m_pPlot->GetLevelAxis().ShowGrid(true);
   m_pPlot->GetLevelAxis().SetRange(0.0, 120.0, Aurora::Axis::RF_LINEARdB);

   m_pPlot->SetTopBorder(false);

   RestoreGeometry();

   // init contextual menu event
    m_pContextMenu.reset(new wxMenu);
    m_pContextMenu->Append(ID_COPY_SELECTION_MENU, "&Copy selection\tCtrl+C");
    m_pContextMenu->Append(ID_COPY_TABLE_MENU,     "&Copy entire table");
    m_pContextMenu->Append(ID_EXPORT_TABLE_MENU,   "&Export to file...");
    m_pContextMenu->Connect(ID_COPY_SELECTION_MENU, 
                            wxEVT_COMMAND_MENU_SELECTED,
                            wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnCopySelection), 
                            NULL, this );
    m_pContextMenu->Connect(ID_COPY_TABLE_MENU, 
                            wxEVT_COMMAND_MENU_SELECTED,
                            wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnCopyTableToClipboard), 
                            NULL, this );
    m_pContextMenu->Connect(ID_EXPORT_TABLE_MENU, 
                            wxEVT_COMMAND_MENU_SELECTED,
                            wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnExport), 
                            NULL, this );

}

Aurora::TimeHistoryAnalyzerFrame::~TimeHistoryAnalyzerFrame()
{
    m_pContextMenu->Disconnect(ID_COPY_SELECTION_MENU, 
                               wxEVT_COMMAND_MENU_SELECTED,
                               wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnCopySelection), 
                               NULL, this );
    m_pContextMenu->Disconnect(ID_COPY_TABLE_MENU, 
                               wxEVT_COMMAND_MENU_SELECTED,
                               wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnCopyTableToClipboard), 
                               NULL, this );
    m_pContextMenu->Disconnect(ID_EXPORT_TABLE_MENU, 
                               wxEVT_COMMAND_MENU_SELECTED,
                               wxCommandEventHandler(TimeHistoryAnalyzerFrame::OnExport),
                               NULL, this );
}

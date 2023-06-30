/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries

  afplot.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::Plot
\brief Plot parent class (MUST be derived!)

*//*******************************************************************/
#include <aurora.h>

//----------------------------------------------------------------------------
// Aurora::LegendEntry implementation
//----------------------------------------------------------------------------

//#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
//WX_DEFINE_OBJARRAY(TArrayOfLegendEntry);

//----------------------------------------------------------------------------
// Aurora::Legend implementation
//----------------------------------------------------------------------------
void Aurora::Legend::FindMaxLabelsDims(wxDC& dc)
{
    for(auto& entry : m_aEntries)
    {
        wxSize sz = dc.GetTextExtent(entry.GetLabel());

        if(sz.GetWidth() > m_maxLabelSizes.GetWidth())
        {
            m_maxLabelSizes.SetWidth(sz.GetWidth());
        }
        
        if(sz.GetHeight() > m_maxLabelSizes.GetHeight())
        {
            m_maxLabelSizes.SetHeight(sz.GetHeight());
        }
    }
}

void Aurora::Legend::AddLabel(wxString& label, const wxColour& colour)
{
    wxColour col = colour;
    
    if(col == wxNullColour)
    {
        Aurora::ChooseColour(col, int(m_aEntries.size()));
    }
    m_aEntries.emplace_back(Aurora::LegendEntry(label, col));
}

void Aurora::Legend::AddLabel(const wxChar* label, const wxColour& colour)
{
    wxColour col = colour;
    
    if(col == wxNullColour)
    {
        Aurora::ChooseColour(col, int( m_aEntries.size()));
    }
    m_aEntries.emplace_back(Aurora::LegendEntry(label, col));
}

void Aurora::Legend::RemoveLabel(const int nIdx)
{
    assert(nIdx < (int)m_aEntries.size());
    auto it = m_aEntries.begin();    
    m_aEntries.erase(it + nIdx);
}

void Aurora::Legend::Draw(wxDC& dc, wxRect& clientArea)
{
   int nLabelsCount = (int)m_aEntries.size();
   
   if(m_bShow && (nLabelsCount > 0))
   {
        dc.SetFont(m_font);

        if(m_maxLabelSizes == wxSize(0,0))
        {
                FindMaxLabelsDims(dc);
        }
        // Evaluate legend box sizes (5 is the Legend border, 10 is the sample coloured line size)
        m_boundingBox.width  = LGD_MARGIN + LGD_LINE_WIDTH + LGD_MARGIN + m_maxLabelSizes.GetWidth() + LGD_MARGIN;
        m_boundingBox.height = LGD_MARGIN + nLabelsCount*(m_maxLabelSizes.GetHeight() + LGD_MARGIN);

        switch(m_nPosition)
        {
            case TOP_LEFT:
                    m_boundingBox.x = clientArea.GetLeft() + LGD_BORDER;
                    m_boundingBox.y = clientArea.GetTop()  + LGD_BORDER;
                    break;

            case TOP_RIGHT:
                    m_boundingBox.x = clientArea.GetRight() - (m_boundingBox.width + LGD_BORDER);
                    m_boundingBox.y = clientArea.GetTop()   + LGD_BORDER;
                    break;

            case BOTTOM_RIGHT:
                    m_boundingBox.x = clientArea.GetRight()  - (m_boundingBox.width  + LGD_BORDER);
                    m_boundingBox.y = clientArea.GetBottom() - (m_boundingBox.height + LGD_BORDER);
                    break;

            case BOTTOM_LEFT:
                    m_boundingBox.x = clientArea.GetLeft()   + LGD_BORDER;
                    m_boundingBox.y = clientArea.GetBottom() - (m_boundingBox.height + LGD_BORDER);
                    break;
        }
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawRectangle(m_boundingBox);
    /*
	   printf("[canvas] l r t b %d %d %d %d\n", clientArea.GetLeft(), clientArea.GetRight(),
			                                    clientArea.GetTop(), clientArea.GetBottom());
	   printf("[canvas] x y w h %d %d %d %d\n", clientArea.x, clientArea.y,
			                                    clientArea.width, clientArea.height);

	   printf("[legend] x y w h : %d %d %d %d\n", m_boundingBox.x, m_boundingBox.y, m_boundingBox.width, m_boundingBox.height);
       fflush(stdout);
*/
        wxPen pen;
        for(int n = 0; n < nLabelsCount; n++)
        {
            pen.SetColour(m_aEntries[n].GetColour());
            dc.SetPen(pen);
            dc.DrawLine(m_boundingBox.x + LGD_MARGIN,
                        m_boundingBox.y + LGD_MARGIN 
                        + m_maxLabelSizes.GetHeight()/2 
                        + n*(m_maxLabelSizes.GetHeight() + LGD_MARGIN),
                        m_boundingBox.x + LGD_MARGIN + LGD_LINE_WIDTH,
                        m_boundingBox.y + LGD_MARGIN 
                        + m_maxLabelSizes.GetHeight()/2
                        + n*(m_maxLabelSizes.GetHeight() + LGD_MARGIN));

            dc.SetPen(*wxBLACK_PEN);
            dc.DrawText(m_aEntries[n].GetLabel(),
                        m_boundingBox.x + LGD_MARGIN + LGD_LINE_WIDTH + LGD_MARGIN,
                        m_boundingBox.y + LGD_MARGIN 
                        + n*(m_maxLabelSizes.GetHeight() + LGD_MARGIN));
        }
   }
}

Aurora::Legend::Legend(const int nPosition)
  : m_nPosition(nPosition),
    m_boundingBox(0,0,0,0),
    m_maxLabelSizes(0,0),    
    m_font(LEGEND_FONT_SIZE,
           wxFONTFAMILY_SWISS,
           wxFONTSTYLE_NORMAL,
           wxFONTWEIGHT_NORMAL),
    m_bShow(false)
{}


//----------------------------------------------------------------------------
// Aurora::PlotBase implementation
//----------------------------------------------------------------------------
void Aurora::PlotBase::Autorange(const std::vector<double>& serie,
                                 double& min,
                                 double& max,
                                 const bool bIsLog)
{
    Autorange(serie.data(), serie.size(), min, max, bIsLog);
}

void Aurora::PlotBase::Autorange(const double* pSerie, 
                                 const size_t unLength, 
                                 double& min, 
                                 double& max, 
                                 const bool bIsLog)
{
    // TODO Missing Log case!!!!
    if(bIsLog) return; // to be removed....

    max = -1.0e-12;
    min =  1.0e+12;
    
    for(size_t un = 0; un < unLength; un++)
    {
        max = (pSerie[un] < max) ? max : pSerie[un];
        min = (pSerie[un] > min) ? min : pSerie[un];
    }

    const double range = max - min;
    double div   = 0.000001;

    while(range * div < 1) 
    {
        div *= 10;        
    }

    const double step  = 0.5 / div;
    max = step * ceil( max/ step );

    if(min < 0)
    {
        min = step * floor( min/ step );
    }
}


void Aurora::PlotBase::UpdatePlot(wxDC& dc, const wxSize& sz)
{
    // Cannot draw anything without axis (maybe an Assert fit better)!
    if(!m_pHorzAxis || !m_pVertAxis) 
    {
        return;
    }

    // Set up grid, background & rulers
    m_nWidth  = sz.x;
    m_nHeight = sz.y;

    //Borders calculation
    m_nBBorder = m_pHorzAxis->GetHeight(dc);
    m_nTBorder = (m_bIsTBorderOn) ? m_nBBorder : ((m_Title.bShow) ? m_nBBorder : 0);
    m_nLBorder = m_pVertAxis->GetWidth(dc);
    m_nRBorder = (m_bIsRBorderOn) ?  m_nLBorder : 0;

#ifndef __WXOSX__
    m_canvasColor = GetBackgroundColour();
#endif
    wxBrush brush(m_canvasColor);
    wxPen   pen(m_bordersColor); 
   
    dc.SetBrush(brush);
    dc.SetPen(*wxTRANSPARENT_PEN);

    // Canvas plot background
    wxRect wxrectCanvas(0, 0, m_nWidth, m_nHeight);
    dc.DrawRectangle(wxrectCanvas);

    // Client area border
    m_clientArea.x      = m_nLBorder;
    m_clientArea.y      = m_nTBorder;
    m_clientArea.width  = m_nWidth  - (m_nLBorder + m_nRBorder);
    m_clientArea.height = m_nHeight - (m_nTBorder + m_nBBorder) + 1;

    brush.SetColour(m_backgroundColor);
    dc.SetBrush(brush);
    dc.SetPen(pen);
    dc.DrawRectangle(m_clientArea);

    // Draw plot title
    if(m_Title.bShow)
    {
        dc.SetFont(m_Title.font);
        dc.DrawText( m_Title.label,
                    (m_nWidth - dc.GetTextExtent(m_Title.label).GetWidth())/2,
                    m_nTBorder/2 - PLOT_TITLE_FONT_SIZE );
    }

    // Axis update, sizing and drawing
    UpdateHorizontalAxis();
    UpdateVerticalAxis();

    // Draw rulers
    m_pHorzAxis->SetBounds( dc,
                            m_nLBorder,                // l
                            m_nHeight - m_nBBorder,    // r
                            m_nWidth - m_nRBorder,     // t
                            m_nHeight,                 // b
                            m_nHeight - (m_nTBorder + m_nBBorder) ); // gridlength

    m_pVertAxis->SetBounds( dc,
                            0,                        // l
                            m_nTBorder,               // r
                            m_nLBorder,               // t
                            m_nHeight - m_nBBorder,   // b
                            m_nWidth - (m_nLBorder + m_nRBorder) ); // gridlength

    m_pHorzAxis->Draw(dc);
    m_pVertAxis->Draw(dc);

    // Finally it draws the plot!
    Draw(dc);

    // Draw Legend
    if(m_pLegend)
    {
        m_pLegend->Draw(dc, m_clientArea);
    }
    
    if(m_bIsSelectionOn)
    {
        DrawSelectionRegion(dc);
    }
    
    // Paint border again
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(pen);
    dc.DrawRectangle(m_clientArea);
}

void Aurora::PlotBase::DrawSelectionRegion(wxDC& dc)
{
    if(m_selectionArea.width != 0 && m_selectionArea.height != 0)
    {
        dc.SetBrush(wxColour(255,255,0));
        dc.SetPen(*wxTRANSPARENT_PEN);
//       dc.SetLogicalFunction(wxEQUIV); // or wxOR_REVERSE [on mac these don't work]
        dc.SetLogicalFunction(wxXOR);
        dc.DrawRectangle(m_selectionArea);
        dc.SetLogicalFunction(wxCOPY); // restore default function.
    }
}

// --- handlers

void Aurora::PlotBase::OnPaint(wxPaintEvent& evt)
{
    const wxSize sz = GetClientSize();
    wxAutoBufferedPaintDC adc(this);
    UpdatePlot(adc, sz);
}

void Aurora::PlotBase::OnRightButtonClick(wxMouseEvent& evt)
{
    const int nX = evt.GetX(); 
    const int nY = evt.GetY();
    wxString label(wxT("Set "));

    wxMenu menu;

    if(m_clientArea.Contains(nX, nY))
    {
        menu.Append(ID_COPY_BITMAP, wxT("&Copy\tCtrl+C"));
#if wxUSE_METAFILE
        menu.Append(ID_COPY_VECTORIAL, wxT("Copy as vectorial"));
#endif
        menu.AppendCheckItem(ID_SHOW_LEGEND_MCHK, wxT("Show legend"));
        menu.Check(ID_SHOW_LEGEND_MCHK, GetLegend()->IsShown());
        PopupMenu(&menu);
    }
    else if(m_pHorzAxis->GetRulerBounds().Contains(nX, nY) && m_bIsHorzRangeMutable)
    {
        label << m_pHorzAxis->GetName() << wxT(" axis range");
        menu.Append(ID_SET_X_AXIS_RANGE, label);
        menu.AppendCheckItem(ID_X_AUTORANGE_MCHK, wxT("Autorange"));
        menu.Check(ID_X_AUTORANGE_MCHK, m_bIsHorzAutorangeOn);
        PopupMenu(&menu);
    }
    else if(m_pVertAxis->GetRulerBounds().Contains(nX, nY) && m_bIsVertRangeMutable)
    {
        label << m_pVertAxis->GetName() << wxT(" axis range");
        menu.Append(ID_SET_Y_AXIS_RANGE, label);
        menu.AppendCheckItem(ID_Y_AUTORANGE_MCHK, wxT("Autorange"));
        menu.Check(ID_Y_AUTORANGE_MCHK, m_bIsVertAutorangeOn);
        PopupMenu(&menu);
    }
}

#if wxUSE_METAFILE
void Aurora::PlotBase::OnCopyVectorial(wxCommandEvent& evt)
{
    const wxString caption(_("wxPlot"));
    wxSize sz = GetClientSize();
    sz *= 2;

    wxMetafileDC dc(wxEmptyString, sz.x, sz.y, caption);
    
    if(dc.IsOk()) 
    {
        UpdatePlot(dc, sz);
        wxMetafile *mf = dc.Close();
        
        if(!mf) 
        {
            wxClipboardLocker locker;
            
            if(!locker) 
            {
                Aurora::EffectBase::MessageBox("Cannot open the clipboard",
                                                Aurora::MessageType::Error);
            }
            else
            {
                mf->SetClipboard();                
            }
            delete mf;
        }
    }
}
#endif

void Aurora::PlotBase::OnCopyBitmap(wxCommandEvent& evt)
{
    wxSize sz = GetClientSize();
    sz *= 2;

    wxBitmap bmp(m_nWidth, m_nHeight);
    wxMemoryDC dc(bmp);

    if(dc.IsOk())
    {
        UpdatePlot(dc, sz);
        wxClipboardLocker locker;
        
        if(!locker)
        {
            Aurora::EffectBase::MessageBox("Cannot open the clipboard",
                                            Aurora::MessageType::Error);
        }
        else if ( !wxTheClipboard->AddData(new wxBitmapDataObject(bmp)) )
        {
            Aurora::EffectBase::MessageBox("Can't copy data to the clipboard!",
                                            Aurora::MessageType::Error);
        }
    }
}

void Aurora::PlotBase::OnShowSetXAxisRange(wxCommandEvent& evt)
{	
    auto dlg = new Aurora::SetAxisRangeDialog(m_pParent, 
                                              ID_SET_X_AXIS_DLG, 
                                              m_pHorzAxis.get());
    if(dlg->ShowModal())
    {
        SetHorzAxisAutorange(false);
        Refresh();
    }
    delete dlg;
}

void Aurora::PlotBase::OnShowSetYAxisRange(wxCommandEvent& evt)
{
    auto dlg = new Aurora::SetAxisRangeDialog(m_pParent, 
                                            ID_SET_Y_AXIS_DLG,
                                            m_pVertAxis.get());
    if(dlg->ShowModal())
    {
        SetVertAxisAutorange(false);
        Refresh();
    }
    delete dlg;
}

void Aurora::PlotBase::OnXAutorangeMenuCheck(wxCommandEvent& evt)
{
    m_bIsHorzAutorangeOn = evt.IsChecked();
    Refresh();
}

void Aurora::PlotBase::OnYAutorangeMenuCheck(wxCommandEvent& evt)
{
    m_bIsVertAutorangeOn = evt.IsChecked();
    Refresh();
}

void Aurora::PlotBase::OnShowLegendMenuCheck(wxCommandEvent& evt)
{
    GetLegend()->Show(evt.IsChecked());
    Refresh();
}


void Aurora::PlotBase::OnChar(wxKeyEvent &evt)
{
#if defined __WXOSX__    
    if((evt.GetKeyCode() == 99) && (evt.GetModifiers() == wxMOD_CMD))   // Command+C
#else        
    if(evt.GetKeyCode() == 3)   // Ctrl+C
#endif
    {
        wxCommandEvent cmd(wxEVT_COMMAND_MENU_SELECTED, ID_COPY_BITMAP);
        OnCopyBitmap(cmd);
    }
}

// ----------------- Selection management handlers ------------------------
void Aurora::PlotBase::OnMouseStartSelection(wxMouseEvent& evt)
{
    const int nX = evt.GetX();
    const int nY = evt.GetY();
    
    if(m_bIsHorzRangeMutable && m_pHorzAxis->GetRulerBounds().Contains(nX, nY))
    {
        m_bIsSelectionOn     = true;
        m_bIsHorzAutorangeOn = false;
        m_selectionArea.x  = nX;        
        m_selectionArea.y  = m_pHorzAxis->GetRulerBounds().y;        
        m_selectionArea.width  = 0;
        m_selectionArea.height = m_pHorzAxis->GetRulerBounds().height;

    }
    else if(m_bIsVertRangeMutable && m_pVertAxis->GetRulerBounds().Contains(nX, nY))
    {    
        m_bIsSelectionOn     = true;
        m_bIsVertAutorangeOn = false;
        m_selectionArea.x  = m_pVertAxis->GetRulerBounds().x;                
        m_selectionArea.y  = nY;
        m_selectionArea.width  = m_pVertAxis->GetRulerBounds().width;
        m_selectionArea.height = 0;
    }    
}

void Aurora::PlotBase::OnMouseSelection(wxMouseEvent& evt)
{
    const int nX = evt.GetX();
    const int nY = evt.GetY();

    if(m_bIsSelectionOn && evt.LeftIsDown())
    {
        if(m_pHorzAxis->GetRulerBounds().Contains(nX, nY))
        {
            m_selectionArea.width = nX - m_selectionArea.x;
            Refresh();
        }
        else if(m_pVertAxis->GetRulerBounds().Contains(nX, nY))
        {
            m_selectionArea.height = nY - m_selectionArea.y;
            Refresh();

        }
    }
}

void Aurora::PlotBase::OnMouseEndSelection(wxMouseEvent& evt)
{
    const int nX = evt.GetX();
    const int nY = evt.GetY();
    int nMin = 0;
    int nMax = 0;

    if(m_bIsSelectionOn)
    {
        if(m_pHorzAxis->GetRulerBounds().Contains(nX, nY))
        {
            nMin = m_selectionArea.x - m_pHorzAxis->GetBounds().x;
            nMax = nMin + m_selectionArea.width;
            
            if(nMax < nMin)
            {
                int nTmp = nMin;
                nMin = nMax;
                nMax = nTmp;
            }
            m_pHorzAxis->SetPixelRange(nMin, nMax);
        }
        else if(m_pVertAxis->GetRulerBounds().Contains(nX, nY))
        {
            nMax = m_selectionArea.y - m_pVertAxis->GetBounds().y;
            nMin = nMax + m_selectionArea.height;
            
            nMax = m_pVertAxis->GetBounds().height - nMax; // values flipping
            nMin = m_pVertAxis->GetBounds().height - nMin;
            
            if(nMax < nMin)
            {
                int nTmp = nMin;
                nMin = nMax;
                nMax = nTmp;
            }
            m_pVertAxis->SetPixelRange(nMin, nMax);
        }
 
        m_bIsSelectionOn = false;
        Refresh();
    }
}

Aurora::Legend* Aurora::PlotBase::GetLegend ()
{
    if(!m_pLegend)
    {
        m_pLegend.reset(new Aurora::Legend);
    }
    return m_pLegend.get();
}

void Aurora::PlotBase::ShowLegend(const bool bValue)
{
    if(!m_pLegend && !bValue) 
    {
        return;
    }

    if(!m_pLegend)
    {
        m_pLegend.reset(new Aurora::Legend);
    }
    m_pLegend->Show(bValue);
}

// --- ctors
Aurora::PlotBase::PlotBase( wxWindow *parent,   wxWindowID id,
                        const wxPoint& pos, const wxSize& size,
                        const long style )
  : wxControl(parent, id, pos, size, style),
    m_pParent(parent),

    //m_canvasColor(wxT("#EDEDED")),     // default canvas colour...on Mac!
    m_canvasColor(Aurora::GetCanvasColour()),
    m_backgroundColor(wxT("#FFFFFF")), // default plot background is white
    m_bordersColor(wxT("#000000")),    // default borders are black

    m_nTBorder(0), m_nBBorder(0),
    m_nLBorder(0), m_nRBorder(0),
    m_bIsTBorderOn(true), m_bIsRBorderOn(true),

    m_bIsHorzAutorangeOn(true),    m_bIsVertAutorangeOn(true),
    m_bIsHorzRangeMutable(true),   m_bIsVertRangeMutable(true),
                             
    m_bIsSelectionOn(false)

{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    SetInitialSize(size);

    m_nWidth  = size.x;
    m_nHeight = size.y;

    m_Title.bShow     = false;
    m_Title.label = wxT("Plot Title");
    m_Title.font.SetPointSize(PLOT_TITLE_FONT_SIZE);
    m_Title.font.SetFamily(wxFONTFAMILY_SWISS);
    m_Title.font.SetStyle(wxFONTSTYLE_NORMAL);
    m_Title.font.SetWeight(wxFONTWEIGHT_NORMAL);

    Connect(wxEVT_PAINT,
            wxPaintEventHandler(Aurora::PlotBase::OnPaint),
            NULL, this);
    Connect(wxEVT_CHAR,       
            wxCharEventHandler(Aurora::PlotBase::OnChar),
            NULL, this);
    Connect(wxEVT_RIGHT_DOWN, 
            wxMouseEventHandler(Aurora::PlotBase::OnRightButtonClick),
            NULL, this);
    Connect(wxEVT_LEFT_DOWN,  
            wxMouseEventHandler(Aurora::PlotBase::OnMouseStartSelection), 
            NULL, this);
    Connect(wxEVT_MOTION,
            wxMouseEventHandler(Aurora::PlotBase::OnMouseSelection),
            NULL, this);
    Connect(wxEVT_LEFT_UP,
            wxMouseEventHandler(Aurora::PlotBase::OnMouseEndSelection),
            NULL, this);

    Connect(ID_COPY_BITMAP,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnCopyBitmap),
            NULL, this );
    #if wxUSE_METAFILE
    Connect(ID_COPY_VECTORIAL,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnCopyVectorial),
            NULL, this );
    #endif

    Connect(ID_SET_X_AXIS_RANGE, 
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnShowSetXAxisRange), 
            NULL, this );
    Connect(ID_SET_Y_AXIS_RANGE,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnShowSetYAxisRange),
            NULL, this );
    Connect(ID_X_AUTORANGE_MCHK,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnXAutorangeMenuCheck),
            NULL, this );
    Connect(ID_Y_AUTORANGE_MCHK,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnYAutorangeMenuCheck),
            NULL, this );
    Connect(ID_SHOW_LEGEND_MCHK,
            wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(Aurora::PlotBase::OnShowLegendMenuCheck),
            NULL, this );
}

Aurora::PlotBase::~PlotBase()
{
   Disconnect(wxEVT_PAINT,
              wxPaintEventHandler(Aurora::PlotBase::OnPaint),
              NULL, this);
   Disconnect(wxEVT_CHAR,
              wxCharEventHandler(Aurora::PlotBase::OnChar),
              NULL, this);
   Disconnect(wxEVT_RIGHT_DOWN,
              wxMouseEventHandler(Aurora::PlotBase::OnRightButtonClick),
              NULL, this);
   Disconnect(wxEVT_LEFT_DOWN,
              wxMouseEventHandler(Aurora::PlotBase::OnMouseStartSelection),
              NULL, this);
   Disconnect(wxEVT_MOTION,
              wxMouseEventHandler(Aurora::PlotBase::OnMouseSelection),
              NULL, this);
   Disconnect(wxEVT_LEFT_UP,
              wxMouseEventHandler(Aurora::PlotBase::OnMouseEndSelection),
              NULL, this);

   Disconnect(ID_COPY_BITMAP,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnCopyBitmap),
              NULL, this );
#if wxUSE_METAFILE
   Disconnect(ID_COPY_VECTORIAL,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnCopyVectorial),
              NULL, this );
#endif
   Disconnect(ID_SET_X_AXIS_RANGE,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnShowSetXAxisRange),
              NULL, this );
   Disconnect(ID_SET_Y_AXIS_RANGE,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnShowSetYAxisRange),
              NULL, this );
   Disconnect(ID_X_AUTORANGE_MCHK,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnXAutorangeMenuCheck),
              NULL, this );
   Disconnect(ID_Y_AUTORANGE_MCHK,
              wxEVT_COMMAND_MENU_SELECTED, 
              wxCommandEventHandler(Aurora::PlotBase::OnYAutorangeMenuCheck),
              NULL, this );
   Disconnect(ID_SHOW_LEGEND_MCHK,
              wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Aurora::PlotBase::OnShowLegendMenuCheck),
              NULL, this );
}


//----------------------------------------------------------------------------
// Aurora::SetAxisRangeDialog implementation
//----------------------------------------------------------------------------
void Aurora::SetAxisRangeDialog::OnCancel( wxCommandEvent& event )
{
    EndModal(false);
}

void Aurora::SetAxisRangeDialog::OnOk( wxCommandEvent& event )
{
    m_min = ::TextPtrToDouble(m_pMinText);
    m_max = ::TextPtrToDouble(m_pMaxText);

    if(m_max <= m_min)
    {
        Aurora::EffectBase::MessageBox("Invalid bounds.",
                                       Aurora::MessageType::Error);
        EndModal(false);
    }
    else
    {
        m_pAxis->SetRange(m_min, m_max);
        EndModal(true);
    }
}

Aurora::SetAxisRangeDialog::SetAxisRangeDialog(wxWindow* pParent, 
                                               wxWindowID id, 
                                               Aurora::AxisBase* pAxis,
                                               const wxString& title, 
                                               const wxPoint& pos, 
                                               const wxSize& size, 
                                               long style)
  : wxDialog( pParent, id, title, pos, size, style ),
    m_pAxis(pAxis),
    m_nId(id)
{
    assert(m_pAxis);
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    
    wxBoxSizer* mainSizer = new wxBoxSizer( wxVERTICAL );
    
    m_pUpperPanel = new wxPanel( this, 
                                 wxID_ANY, 
                                 wxDefaultPosition, 
                                 wxDefaultSize, 
                                 wxTAB_TRAVERSAL );
    wxFlexGridSizer* flexGridSizer = new wxFlexGridSizer( 2, 3, 0, 0 );
    flexGridSizer->SetFlexibleDirection( wxBOTH );
    flexGridSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    
    wxStaticText* minLabel = new wxStaticText( m_pUpperPanel, 
                                               wxID_ANY, 
                                               wxT("Min:"), 
                                               wxDefaultPosition, 
                                               wxDefaultSize, 0 );
    minLabel->Wrap( -1 );
    flexGridSizer->Add( minLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    m_pMinText = new wxTextCtrl( m_pUpperPanel, 
                                 ID_MIN_TC, 
                                 wxT("-100.000000"), 
                                 wxDefaultPosition, 
                                 wxDefaultSize, 
                                 wxTE_RIGHT );
    m_pMinText->SetMinSize( wxSize( 100,-1 ) );
    
    flexGridSizer->Add( m_pMinText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    wxStaticText* minUnitLabel = new wxStaticText( m_pUpperPanel, 
                                                   ID_MIN_MU_ST, 
                                                   wxT("dB"), 
                                                   wxDefaultPosition, 
                                                   wxDefaultSize, 0 );
    minUnitLabel->Wrap( -1 );
    minUnitLabel->SetMinSize( wxSize( 30,-1 ) );
    
    flexGridSizer->Add( minUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    wxStaticText* maxLabel = new wxStaticText( m_pUpperPanel, 
                                               wxID_ANY, 
                                               wxT("Max:"), 
                                               wxDefaultPosition, 
                                               wxDefaultSize, 0 );
    maxLabel->Wrap( -1 );
    flexGridSizer->Add( maxLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    m_pMaxText= new wxTextCtrl( m_pUpperPanel, 
                                wxID_ANY, 
                                wxT("100.000000"), 
                                wxDefaultPosition, 
                                wxDefaultSize, 
                                wxTE_RIGHT );
    m_pMaxText->SetMinSize( wxSize( 100,-1 ) );
    
    flexGridSizer->Add( m_pMaxText, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    wxStaticText* maxUnitLabel = new wxStaticText( m_pUpperPanel, 
                                                   ID_MAX_MU_ST, 
                                                   wxT("dB"), 
                                                   wxDefaultPosition, 
                                                   wxDefaultSize, 0 );
    maxUnitLabel->Wrap( -1 );
    flexGridSizer->Add( maxUnitLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    m_pUpperPanel->SetSizer( flexGridSizer );
    m_pUpperPanel->Layout();
    flexGridSizer->Fit( m_pUpperPanel );
    mainSizer->Add( m_pUpperPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
    
    mainSizer->Add( new wxStaticLine( this, 
                                     wxID_ANY, 
                                     wxDefaultPosition, 
                                     wxDefaultSize, 
                                     wxLI_HORIZONTAL ), 
                   0, wxEXPAND | wxALL, 5 );
    
    m_pLowerPanel = new wxPanel( this, 
                                 wxID_ANY, 
                                 wxDefaultPosition, 
                                 wxDefaultSize, 
                                 wxTAB_TRAVERSAL );
    wxBoxSizer* buttonsHSizer = new wxBoxSizer( wxHORIZONTAL );
    
    m_pCancelButton = new wxButton( m_pLowerPanel, 
                                    wxID_ANY, 
                                    wxT("Cancel"), 
                                    wxDefaultPosition, 
                                    wxDefaultSize, 0 );
    buttonsHSizer->Add( m_pCancelButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
    m_pOkButton = new wxButton( m_pLowerPanel, 
                                wxID_ANY, 
                                wxT("Ok"), 
                                wxDefaultPosition, 
                                wxDefaultSize, 0 );
    m_pOkButton->SetDefault(); 
    buttonsHSizer->Add( m_pOkButton, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
    
    m_pLowerPanel->SetSizer( buttonsHSizer );
    m_pLowerPanel->Layout();
    buttonsHSizer->Fit( m_pLowerPanel );
    mainSizer->Add( m_pLowerPanel, 0, wxALL|wxALIGN_RIGHT, 5 );
    
    this->SetSizer( mainSizer );
    this->Layout();
    mainSizer->Fit( this );
    
    // Connect Events
    m_pCancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, 
                              wxCommandEventHandler( Aurora::SetAxisRangeDialog::OnCancel ), 
                              NULL, this );
    m_pOkButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, 
                          wxCommandEventHandler( Aurora::SetAxisRangeDialog::OnOk ), 
                          NULL, this );

    // My own setup
    wxString s = wxT("Set ");    
    s << m_pAxis->GetName() << wxT(" axis range");
    SetTitle(s);

    m_pAxis->GetRange(m_min, m_max);

    m_pMinText->SetValue(wxString::Format(wxT("%f"), m_min));
    m_pMaxText->SetValue(wxString::Format(wxT("%f"), m_max));

    minUnitLabel->SetLabel(m_pAxis->GetMeasureUnit());
    maxUnitLabel->SetLabel(m_pAxis->GetMeasureUnit());
    
    CentreOnParent();
}


// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283

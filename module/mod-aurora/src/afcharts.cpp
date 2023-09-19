/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afcharts.cpp

  Simone Campanini - 16/3/2020

**********************************************************************/
#include <aurora.h>

//----------------------------------------------------------------------------
// Aurora::ChartBase implementation
//----------------------------------------------------------------------------
Aurora::Axis& Aurora::ChartBase::GetLevelAxis()
{
    auto p = dynamic_cast<Aurora::Axis*>(m_pVertAxis.get());
    assert(p);
    return *p;
}

Aurora::CategoriesAxis& Aurora::ChartBase::GetCategoriesAxis()
{
    auto p = dynamic_cast<Aurora::CategoriesAxis*>(m_pHorzAxis.get());
    assert(p);
    return *p;
}

void Aurora::ChartBase::UpdateVerticalAxis()
{
    if (m_aDataSeries.empty())
    {
        return;
    }

    GetLegend()->Clear();

    for (auto& serie : m_aDataSeries)
    {
        if (serie->IsShown())
        {
            GetLegend()->AddLabel(serie->GetLabel(),
                                  serie->GetLineColour());
        }
    }

    double lmin = 1.0e+12;
    double lmax = -1.0e+12;

    if (this->m_bIsVertAutorangeOn)
    {

        double lmin1 = 1.0e+12;
        double lmax1 = -1.0e+12;

        for (auto& serie : m_aDataSeries)
        {
            if (serie->IsShown())
            {
                this->Autorange(serie->data(),
                    serie->GetLength(),
                    lmin1,
                    lmax1);

                lmin = (lmin1 < lmin) ? lmin1 : lmin;
                lmax = (lmax1 > lmax) ? lmax1 : lmax;
            }
        }
    }
    else
    {
        GetLevelAxis().GetRange(lmin, lmax);
    }
    GetLevelAxis().SetRange(lmin, lmax);
}

Aurora::ChartBase::ChartBase(wxWindow *parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size)
 : DataPlot(parent, id, pos, size)
{
    SetVerticalAxis(new Axis(wxVERTICAL));
    SetHorizontalAxis(new CategoriesAxis());
    this->m_bIsHorzRangeMutable = false; //The horizontal axis range cannot be changed!
}

//----------------------------------------------------------------------------
// BarPlot implementation
//----------------------------------------------------------------------------
void Aurora::BarPlot::Draw(wxDC& dc)
{
    const int nPlotWidth = GetClientAreaWidth();
    const int nPlotHeight = GetClientAreaHeight();
    int nXofs = GetClientAreaHorzOffset()
                + int(GetCategoriesAxis().GetCategoriesStart());
    int nYofs = GetClientAreaVertOffset();

    int nShownSeriesCount = 0;
    int nCategoriesCount = int(GetCategoriesAxis().GetCategoriesCount());
    int nCategoriesStep = int(GetCategoriesAxis().GetCategoriesStep());

    for (auto& serie : m_aDataSeries)
    {
        if (serie->IsShown())
        {
            nShownSeriesCount++;
        }
    }

    wxPen   pen;
    wxBrush brush;

    // -------------------- Bars horizontal spacing things... --------------------

    int nBarHeight = 0;
    int nBarWidth = int(std::ceil(0.67 * (nPlotWidth
                                  / (nShownSeriesCount* nCategoriesCount))));
    if (nBarWidth < 10)
    {
        nBarWidth = 10; // 10 pixel is the minimum size
    }

    int nBarSpace = 6; // px
    int nBarsWidth = (nBarWidth*nShownSeriesCount + nBarSpace * (nShownSeriesCount - 1));
    int nBarsSpace = nCategoriesStep - nBarsWidth;

    while (nBarsSpace < nBarSpace)
    {
        // Prevents bar groups overlap
        nBarSpace--;

        if (nBarWidth > 10)
        {
            nBarWidth--;
        }
        nBarsWidth = (nBarWidth*nShownSeriesCount + nBarSpace * (nShownSeriesCount - 1));
        nBarsSpace = nCategoriesStep - nBarsWidth;
    }
    nXofs -= nBarsWidth / 2;

    // -------------------- Bars vertical scaling things --------------------

    double dbYmax, dbYmin;
    GetLevelAxis().GetRange(dbYmin, dbYmax);

    double dbYRange = dbYmax - dbYmin;
    double dbYScale = nPlotHeight / dbYRange;

    int nZeroPos = int(dbYmax*nPlotHeight / dbYRange);

    wxRect barArea(0,             // x
                   nYofs + ((nZeroPos > nPlotHeight) ? nPlotHeight
                                                     : ((nZeroPos < 0) ? 0 
                                                                       : nZeroPos)),     // y
                   nBarWidth,     // width
                   nPlotHeight);  // height

// -------------------- Bars draw cycle --------------------
    for (int nCat = 0; nCat < nCategoriesCount; nCat++)
    {
        int k = 0;

        for (auto& serie : m_aDataSeries)
        {
            if (serie->IsShown())
            {
                pen.SetColour(serie->GetLineColour());
                brush.SetColour(serie->GetFillColour());
                dc.SetPen(pen);
                dc.SetBrush(brush);

 //                barArea.x = nXofs + nCat*(nBarWidth + nBarSpace);
                barArea.x = nXofs + nCat * (nBarsWidth + nBarsSpace)
                                  +    k * (nBarWidth  + nBarSpace);
                k++;

                nBarHeight = int(std::ceil(dbYScale * (serie->at(nCat))));

                if (nZeroPos > nPlotHeight)
                {
                    if (nBarHeight > 0)
                    {
                        nBarHeight -= nZeroPos - nPlotHeight;
                    }
                    else
                    {
                        nBarHeight += nZeroPos - nPlotHeight;
                    }
                }
                else if (nZeroPos < 0)
                {
                    if (nBarHeight > 0)
                    {
                        nBarHeight += nZeroPos;
                    }
                    else
                    {
                        nBarHeight -= nZeroPos;
                    }
                }

                // Bar clipping
                if ((nBarHeight > 0) && (barArea.y - nBarHeight) < nYofs)
                {
                    nBarHeight = barArea.y - nYofs;
                }
                else if ((nBarHeight < 0) && (barArea.y - nBarHeight) > (nYofs + nPlotHeight))
                {
                    nBarHeight = -(nYofs + nPlotHeight - barArea.y);
                }

                barArea.height = -nBarHeight;

                dc.DrawRectangle(barArea);
            }
        }
    }
}

//----------------------------------------------------------------------------
// LinePlot implementation
//----------------------------------------------------------------------------
void Aurora::LinePlot::Draw(wxDC& dc)
{
    int nPlotHeight = GetClientAreaHeight();
  //int nSeriesCount = int(m_aDataSeries.size());
    int nCategoriesCount = int(GetCategoriesAxis().GetCategoriesCount());
    int nCategoriesStart = int(GetCategoriesAxis().GetCategoriesStart());
    int nCategoriesStep = int(GetCategoriesAxis().GetCategoriesStep());

    int nXofs = GetClientAreaHorzOffset() + nCategoriesStart;
    int nYofs = GetClientAreaVertOffset();

    double dbYmax, dbYmin;
    GetLevelAxis().GetRange(dbYmin, dbYmax);

    double dbYRange = dbYmax - dbYmin;
    double dbYScale = nPlotHeight / dbYRange;

    wxPen pen;

    int nX0, nY0, nX1, nY1;
    size_t unCat;

    for (auto& serie : m_aDataSeries)
    {
        if (serie->IsShown())
        {
            pen.SetColour(serie->GetLineColour());
            dc.SetPen(pen);

            auto pdbSerie = serie->data();

            nY0 = (int)std::ceil(dbYScale * (dbYRange - (pdbSerie[0] - dbYmin)));

            if (nY0 < 0)
            {
                nY0 = 0;
            }
            else if (nY0 > nPlotHeight)
            {
                nY0 = nPlotHeight;
            }

            if (nY0 < 0)
            {
                nY0 = 0;
            }
            else if (nY0 > nPlotHeight)
            {
                nY0 = nPlotHeight - 1;
            }

            for (unCat = 1; unCat < size_t(nCategoriesCount - 1); unCat++, nY0 = nY1)
            {
                nX0 = nXofs + unCat * nCategoriesStep;
                nX1 = nX0 + nCategoriesStep;

                nY1 = (int)ceil(dbYScale * (dbYRange - (pdbSerie[unCat] - dbYmin)));
                if (nY1 < 0)
                {
                    nY1 = 0;
                }
                else if (nY1 > nPlotHeight)
                {
                    nY1 = nPlotHeight;
                }

                dc.DrawLine(nX0,
                    nYofs + ((nY0 < nPlotHeight) ? nY0 : nPlotHeight),
                    nX1,
                    nYofs + ((nY1 < nPlotHeight) ? nY1 : nPlotHeight));
            }
        }
    }
}

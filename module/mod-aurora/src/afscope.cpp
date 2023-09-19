/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afscope.h

  Simone Campanini

**********************************************************************/

#include <aurora.h>

// -------------------- ScopeBase definitions -----------------------

Aurora::Axis& Aurora::ScopeBase::GetTimeAxis()
{
    return GetHorzAxis();
}

/// Returns a pointer to vertical (Level, usually) axis.
Aurora::Axis& Aurora::ScopeBase::GetLevelAxis()
{
    return GetVertAxis();
}

/// Returns a pointer to horizontal (Time) axis.
Aurora::Axis& Aurora::ScopeBase::GetHorzAxis()
{
    auto p = dynamic_cast<Aurora::Axis*>(m_pHorzAxis.get());
    assert(p);
    return *p;
}

/// Returns a pointer to vertical (Level, usually) axis.
Aurora::Axis& Aurora::ScopeBase::GetVertAxis()
{
    auto p = dynamic_cast<Aurora::Axis*>(m_pVertAxis.get());
    assert(p);
    return *p;
}

void Aurora::ScopeBase::Draw(wxDC& dc)
{
    const int nPlotWidth = this->GetClientAreaWidth();
    const int nPlotHeight = this->GetClientAreaHeight();
    int nXofs = this->GetClientAreaHorzOffset();
    int nYofs = this->GetClientAreaVertOffset();

    const double dbYmin = GetLevelAxis().GetMin();
    const double dbYRange = GetLevelAxis().GetMax() - dbYmin;
    const double dbYScale = nPlotHeight / dbYRange;

    wxPen pen;
    wxBrush brush;

    //------------- Draw waveform ----------------
    for (auto& serie : this->m_aDataSeries)
    {
        if (serie->IsShown())
        {
            auto pdbData = serie->data();

            if (!pdbData)
            {
                continue;
            }

            pen.SetColour(serie->GetLineColour());
            dc.SetPen(pen);

            int nX0 = 0, nY0 = 0, nX1 = 0, nY1 = 0;

            // Scaling formula: (Lrange - (L - Lmin)) * H/Lrange
            // where L is the series level, H the plot height.
            nY0 = (int)std::ceil(dbYScale * (dbYRange - (pdbData[0] - dbYmin)));

            if (nY0 < 0)
            {
                nY0 = 0;
            }

            if (nY0 > nPlotHeight)
            {
                nY0 = nPlotHeight - 1;
            }

            for (nX0 = 0, nX1 = 1; nX1 < (nPlotWidth - 1); nX0++, nX1++, nY0 = nY1)
            {
                nY1 = (int)ceil(dbYScale * (dbYRange - (pdbData[nX1] - dbYmin)));

                if (nY1 < 0)
                {
                    nY1 = 0;
                }

                if (nY1 > nPlotHeight)
                {
                    nY1 = nPlotHeight - 1;
                }

                dc.DrawLine(nX0 + nXofs, nY0 + nYofs, nX1 + nXofs, nY1 + nYofs);
            }
        }
    }
}

Aurora::ScopeBase::ScopeBase(wxWindow*  parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size)
   : Aurora::DataPlot(parent, id, pos, size)
{
    SetHorizontalAxis(new Aurora::Axis());
    SetVerticalAxis(new Aurora::Axis(wxVERTICAL));

    GetLevelAxis().SetName(wxT("Level"));
    GetTimeAxis().SetName(wxT("Time"));
}

// -------------------- SScope definitions -----------------------

Aurora::Axis& Aurora::SScope::GetFreqAxis()
{
    return GetHorzAxis();
}

Aurora::Axis& Aurora::SScope::GetHorzAxis()
{
    auto p = dynamic_cast<Aurora::Axis*>(this->m_pHorzAxis.get());
    assert(p);
    return *p;
}

void Aurora::SScope::UpdateHorizontalAxis()
{
    if (m_aDataSeries.empty())
    {
        return;
    }

    double fmin, fmax;
    GetFreqAxis().GetRange(fmin, fmax);

    // Range check
    // NB: All time series should have the same length
    auto ds = this->m_aDataSeries[0].get();
    auto sds = dynamic_cast<Aurora::TSpectralDataSerie*>(ds);

    if (!sds->IsOk(fmin, fmax))
    {
        this->SetHorzAxisAutorange(true);
    }

    if (this->m_bIsHorzAutorangeOn)
    {
        fmin = (GetFreqAxis().IsLog()) ? 1.0 : 0.0;
        fmax = sds->GetMax();
    }

    GetFreqAxis().SetRange(fmin, fmax);
}

void Aurora::SScope::UpdateVerticalAxis()
{
    double fmin, fmax;
    GetFreqAxis().GetRange(fmin, fmax);

    if (this->m_aDataSeries.empty())
    {
        return;
    }

    for (auto& serie : this->m_aDataSeries)
    {
        auto sds = dynamic_cast<Aurora::TSpectralDataSerie*>(serie.get());

        if (serie->IsShown())
        {
            // TODO:: manage log axis!!
            // This is the only difference between Scope and SScope 
            sds->CalculateDataSerie(GetClientAreaWidth(),
                                    fmin,
                                    fmax,
                                    GetFreqAxis().IsLog());

            if (GetLegend())
            {
                GetLegend()->AddLabel(sds->GetLabel(), sds->GetLineColour());
            }
        }
    }

    if (this->m_bIsVertAutorangeOn)
    {
        double min =  1.0e+12;
        double max = -1.0e-12;
        double min1 = 0;
        double max1 = 0;

        for (auto& serie : m_aDataSeries)
        {
            const auto sds = dynamic_cast<Aurora::TSpectralDataSerie*>(serie.get());

            Autorange(*sds, min1, max1);

            min = (min1 < min) ? min1 : min;
            max = (max1 > max) ? max1 : max;
        }
        GetLevelAxis().SetRange(min, max);
    }
    else
    {
        wxLogMessage("[afsscope] NO autoranging");
    }
}

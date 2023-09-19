/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afdataplot.h

  Simone Campanini - 16/3/2020

**********************************************************************/
#include <aurora.h>

void Aurora::DataPlot::ShowSerie(const size_t unIdx, const bool bShow)
{
    wxASSERT_MSG(unIdx < m_aDataSeries.size(),
        wxT("Show request failed: index out of bounds."));
    m_aDataSeries[unIdx]->Show(bShow);
}

void Aurora::DataPlot::InsertSerie(const size_t unIdx, DataSerie* data)
{
    wxASSERT_MSG(unIdx < m_aDataSeries.size(),
        wxT("Insert request failed: index out of bounds."));
    auto it = m_aDataSeries.begin();
    m_aDataSeries.emplace(it + unIdx, data);
}

void Aurora::DataPlot::ReplaceSerie(const size_t unIdx, DataSerie* ds)
{
    m_aDataSeries[unIdx].reset(ds);
}

void Aurora::DataPlot::DeleteSerie(const size_t nIdx)
{
    wxASSERT_MSG(nIdx < m_aDataSeries.size(),
        wxT("Delete request failed: index out of bounds."));
    auto it = m_aDataSeries.begin();
    m_aDataSeries.erase(it + nIdx);
}

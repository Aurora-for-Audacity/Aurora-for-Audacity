/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  ThAnalyzerPlot.cpp

  Simone Campanini

*******************************************************************//**

\class THAPlot
\brief The decay graphic

*//*******************************************************************/

#include <aurora.h>

#include "TimeHistoryAnalyzer.h"
#include "ThAnalyzerTrack.h"
#include "ThAnalyzerPlot.h"


//----------------------------------------------------------------------------
// THAPlot implementation
//----------------------------------------------------------------------------
void Aurora::ThAnalyzerPlot::ShowSerie(const size_t unIdx)
{
	for(size_t un = 0; un < m_aDataSeries.size(); un++)
	{
		m_aDataSeries[un]->Show(un == unIdx);
	}
}

void Aurora::ThAnalyzerPlot::ReplaceTimeSerie(const size_t nIdx, 
                                              const std::vector<double>& samples,
	                                          const double dbFullScale,
                                              wxColour& lineColor, 
                                              wxColour& brushColor)
{
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();
    auto tds = dynamic_cast<Aurora::ThAnalyzerTimeDataSerie*>(ds);
    assert(tds);

	wxASSERT_MSG(nIdx < m_aDataSeries.size(),
				 wxT("ThAnalyzer: Replacement unexistent serie (check the index)."));
	tds->CopyTimeData(samples.data(), samples.size());
	tds->SetFullScale(dbFullScale);

	wxColour col;

	if(lineColor != wxNullColour)
	{
		tds->SetLineColour(lineColor);
	}
	else
	{   // Choose color automatically
		Aurora::ChooseColour(col, int(nIdx));
		tds->SetLineColour(col);
	}

	if(brushColor != wxNullColour)
	{
		tds->SetFillColour(brushColor);
	}
	else
	{   // Choose color automatically
		Aurora::ChooseColour(col, int(nIdx));
		tds->SetFillColour(col);
	}
}

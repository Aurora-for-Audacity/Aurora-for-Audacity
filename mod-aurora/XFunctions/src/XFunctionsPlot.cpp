
/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsPlot.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::XFunctionsPlot
\brief The result window plot

*//*******************************************************************/

#include <aurora.h>

#include "correlator.h"
#include "XFunctionsData.h"
#include "XFunctionsPlot.h"


//----------------------------------------------------------------------------
// Aurora::XFunctionsPlot implementation
//----------------------------------------------------------------------------
void Aurora::XFunctionsPlot::AddSerie(Aurora::Correlator* correlatorPtr, 
                                      Aurora::ComplexSpectrum& data,
                                      const Aurora::XFunctionsPlotType handlerType,
                                      const wxColour& lineColor,
                                      const bool bShow)
{
    auto sz = m_aDataSeries.size();
    m_aDataSeries.emplace_back(new Aurora::XFunctionsSpectralDataSerie);

    if(m_aDataSeries.size() > sz)
    {
        Aurora::DataSerie* ds = m_aDataSeries.back().get();
        auto tds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);
        assert(tds);

        tds->SetHandlerId(handlerType);
        tds->SetCorrelatorPtr(correlatorPtr);
        tds->SetRate(correlatorPtr->GetSamplerate());
        tds->CopySpectralData(data);
        tds->Show(bShow);

        wxColour emptyColor;
        
        if(lineColor != wxNullColour)
        {
            tds->SetLineColour(lineColor);
        }
        else
        {   // Choose color emptyColormatically
            Aurora::ChooseColour(emptyColor, (int)sz);
            tds->SetLineColour(emptyColor);
        }
        double max, min;
        tds->FindMaxMin(max, min);
        wxLogMessage("Max, min: %f, %f ", max, min);
    }
}

bool Aurora::XFunctionsPlot::SetPlotType(const Aurora::XFunctionsPlotType nPlotType)
{    
    if(m_aDataSeries.empty())
    {
        // No data serie present
        return false;
    }
    assert(nPlotType >= 0 && nPlotType <= Aurora::XFunctionsPlotType::Coherence);

    bool bFound = false;

    for(size_t un = 0; un < m_aDataSeries.size(); un++)
    {
        Aurora::DataSerie* ds = m_aDataSeries[un].get();
        auto xds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds);
        assert(xds);

        if(xds->GetHandlerId() == nPlotType)
        {
            xds->Show(true);
            bFound = true;
        }
        else
        {
            xds->Show(false);
        }
    }

    return bFound;
}

bool Aurora::XFunctionsPlot::SetPlotTypeByString(wxString& plotType)
{
    //Search for plot type
    if(plotType.Find("Magn") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Magnitude); 
    }
    else if(plotType.Find("Phase") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Phase);
    }
    else if(plotType.Find("Real") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Real);
    }
    else if(plotType.Find("Imag") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Imaginary);
    }
    else if(plotType.Find("Left") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::AutoCorrelationLeft);
    }
    else if(plotType.Find("Right") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::AutoCorrelationRight);
    }
    else if(plotType.Find("Coher") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Coherence);
    }
    else if(plotType.Find("Alpha") != wxNOT_FOUND)
    { 
        return SetPlotType(Aurora::XFunctionsPlotType::Alpha);
    }

    return false;
}

bool Aurora::XFunctionsPlot::SetPlotTypeByString(wxString& plotType, int& nPlotType)
{
    bool ok = SetPlotTypeByString(plotType);
    
    if(ok)
    {
        int n = 0;        
       
        for (auto& ds : m_aDataSeries)
        {
            auto xds = dynamic_cast<Aurora::XFunctionsSpectralDataSerie*>(ds.get());

            if (xds->IsShown())
            {
                nPlotType = n;
                break;
            }
            n++;
        }        
    }
    return ok;
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

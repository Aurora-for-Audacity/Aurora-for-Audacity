/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  STIPlot.cpp

  Simone Campanini

*******************************************************************//**

\class OctaveBandsPlot
\brief The octave band bars graphic chart

*//*******************************************************************/
#include <aurora.h>

#include "sti.h"
#include "StiTrack.h"
#include "StiData.h"
#include "StiPlot.h"

//----------------------------------------------------------------------------
// OctaveBandsPlot implementation
//----------------------------------------------------------------------------
void Aurora::OctaveBandsPlot::AddSerie(OctaveBandsDataSerie* pSerie,
                                       wxColour& lineColor, 
                                       wxColour& fillColor)
{
    int nIdx = int(m_aDataSeries.size());
    m_aDataSeries.emplace_back(pSerie);
    
    if(int(m_aDataSeries.size()) > nIdx)
    {
        wxColour defColor;
        
        if(lineColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetLineColour(lineColor);
        }
        else
        {   // Choose color automatically
            Aurora::ChooseColour(defColor, nIdx);
            m_aDataSeries[nIdx]->SetLineColour(defColor);
        }

        if(fillColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetFillColour(fillColor);
        }
        else
        {
            Aurora::ChooseColour(defColor, nIdx);
            m_aDataSeries[nIdx]->SetFillColour(defColor);
        }
    }
}

void Aurora::OctaveBandsPlot::AddSerie(const double* adbData, 
                                       const size_t unLength,
                                       wxColour& lineColor, 
                                       wxColour& fillColor)
{
    wxASSERT_MSG(unLength == 12, wxT("OctaveBandsPlot: wrong array size"));
    int nIdx = int(m_aDataSeries.size());
    m_aDataSeries.emplace_back(new OctaveBandsDataSerie(adbData));
    
    if(int(m_aDataSeries.size()) > nIdx)
    {
        wxColour defColor;
        if(lineColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetLineColour(lineColor);
        }
        else
        {   // Choose color automatically
            Aurora::ChooseColour(defColor, nIdx);
            m_aDataSeries[nIdx]->SetLineColour(defColor);
        }

        if(fillColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetFillColour(fillColor);
        }
        else
        {
            Aurora::ChooseColour(defColor, nIdx);
            m_aDataSeries[nIdx]->SetFillColour(defColor);
        }
    }
}

void Aurora::OctaveBandsPlot::AddSerie(Aurora::STISpectrum& spectrum,
                                       wxColour& lineColor, 
                                       wxColour& fillColor)
{
    assert(spectrum.Length() == 10 && spectrum.Begin() == 31.5f);    
    double data[12];
    
    spectrum.ForEach(true, [&](const size_t i, const float fcb, double& value)
    {
        data[i] = value;
    });
    AddSerie(data, 12, lineColor, fillColor);
    
}

void Aurora::OctaveBandsPlot::ReplaceSerie(const size_t nIdx, 
                                           const double* adbData, 
                                           const size_t unLength)
{
    wxASSERT_MSG(nIdx < m_aDataSeries.size(),
                "OctaveBandsPlot: Replacement unexistent serie (check the index).");
    wxASSERT_MSG(unLength == m_aDataSeries[nIdx]->GetLength(),
                 "OctaveBandsPlot: Replacement serie lenght different than existing one.");
    
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();    
    ds->Copy(adbData, unLength);
}

void Aurora::OctaveBandsPlot::ReplaceSerie(const size_t nIdx, 
                                           Aurora::STISpectrum& spectrum)
{
    assert(spectrum.Length() == 10 && spectrum.Begin() == 31.5f);    
    double data[12];
    
    spectrum.ForEach(true, [&](const size_t i, const float fcb, double& value)
    {
        data[i] = value;
    });
    ReplaceSerie(nIdx, data, 12);
}

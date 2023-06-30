/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  plot.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::AcParametersDecayPlot
\brief The decay graphic

*//*******************************************************************/

#include <aurora.h>

#include "AcParametersBase.h"
#include "AcParametersTrack.h"
#include "AcParametersPlot.h"

//----------------------------------------------------------------------------
// Aurora::AcParametersDecayPlot
//----------------------------------------------------------------------------

void Aurora::AcParametersDecayPlot::AddSerie(Aurora::AcParametersTimeDataSerie* tds)
{
    Aurora::DataPlot::AddSerie((Aurora::DataSerie*)tds);
}

void Aurora::AcParametersDecayPlot::AddSerie(Aurora::Sample* pData, 
                                             const size_t unLength,
                                             const double dbRate,
                                             const int nHandlerType,
                                             wxColour& lineColor, 
                                             wxColour& brushColor)
{
    int nIdx = int(m_aDataSeries.size());
    m_aDataSeries.emplace_back(new Aurora::AcParametersTimeDataSerie());

    if(int(m_aDataSeries.size()) > nIdx)
    {
        Aurora::DataSerie* ds = m_aDataSeries.back().get();
        auto tds = dynamic_cast<Aurora::AcParametersTimeDataSerie*>(ds);
        assert(tds);
        tds->SetHandler(nHandlerType);
        tds->SetRate(dbRate);
        tds->CopyTimeData(pData, unLength);

        wxColour emptyColor;
        
        if(lineColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetLineColour(lineColor);
        }
        else
        {   // Choose color emptyColormatically
            Aurora::ChooseColour(emptyColor, nIdx);
            m_aDataSeries[nIdx]->SetLineColour(emptyColor);
        }

        if(brushColor != wxNullColour)
        {
            m_aDataSeries[nIdx]->SetFillColour(brushColor);
        }
        else
        {   // Choose color emptyColormatically
            Aurora::ChooseColour(emptyColor, nIdx);
            m_aDataSeries[nIdx]->SetFillColour(emptyColor);
        }
    }
}

void Aurora::AcParametersDecayPlot::AddSerie(Aurora::AcParametersAudioTrack& track,
                                             const int nHandlerType,
                                             wxColour& lineColor,
                                             wxColour& brushColor)
{
    AddSerie(track.Filtered().Samples(),
             (size_t)track.GetLength(),
             track.Filtered().GetRate(),
             nHandlerType, 
             lineColor, 
             brushColor);
}

void Aurora::AcParametersDecayPlot::AddSerie(Aurora::TSchroederDecay& decay,
                                             const int nHandlerType,
                                             wxColour& lineColor,
                                             wxColour& brushColor)
{
    AddSerie(decay.data(), 
             (size_t)decay.GetLength(), 
             decay.GetRate(),
             nHandlerType, 
             lineColor, 
             brushColor);
}

void Aurora::AcParametersDecayPlot::ReplaceSerie(const size_t nIdx, 
                                                 Aurora::Sample* pData, 
                                                 const size_t unLength)
{
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();
    auto tds = dynamic_cast<Aurora::AcParametersTimeDataSerie*>(ds);
    assert(tds);

    wxASSERT_MSG(nIdx < m_aDataSeries.size(),
                    wxT("Aurora::AcParametersDecayPlot: Replacing unexistent serie (check the index)."));
    wxASSERT_MSG(unLength == tds->GetSamplesCount(),
                    wxT("Aurora::AcParametersDecayPlot: Replacement serie's lenght different than existing one."));
    tds->CopyTimeData(pData, unLength);
}

void Aurora::AcParametersDecayPlot::ReplaceSerie(const size_t nIdx, 
                                                 Aurora::AcParametersAudioTrack& track)
{
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();
    auto tds = dynamic_cast<Aurora::AcParametersTimeDataSerie*>(ds);
    assert(tds);
    tds->CopyTimeData(track.Filtered().Samples(),
                      size_t(track.Filtered().Length()));
    tds->SetRate(track.Filtered().GetRate());
}

void Aurora::AcParametersDecayPlot::ReplaceSerie(const size_t nIdx, 
                                                 Aurora::TSchroederDecay& decay)
{
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();
    auto tds = dynamic_cast<Aurora::AcParametersTimeDataSerie*>(ds);
    assert(tds);

    tds->CopyTimeData(decay.data(), size_t(decay.GetLength()));
    tds->SetRate(decay.GetRate());
}

//----------------------------------------------------------------------------
// Aurora::AcParametersBarPlot implementation
//----------------------------------------------------------------------------
void Aurora::AcParametersBarPlot::AddSerie(AcParametersSpectrum& spectrum,
                                           wxColour& lineColor,
                                           wxColour& brushColor)
{
    m_buffer.resize(spectrum.Length() + 2);
    
    spectrum.ForEach(true,
                     [&](const size_t bdi,
                         const float  fcb,
                         Aurora::AcousticalParameters::TResults::TParameter& par)
    {
        m_buffer[bdi] = (par.isValid ? par.value : 0.0);
    });
    AddSerie(m_buffer.data(), m_buffer.size(), lineColor, brushColor);
}

void Aurora::AcParametersBarPlot::AddSerie(const double* adbData, 
                                           const size_t unLength,
                                           wxColour& lineColor, 
                                           wxColour& brushColor)
{
    int nIdx = int(m_aDataSeries.size());
    m_aDataSeries.emplace_back(new Aurora::DataSerie());

    if(int(m_aDataSeries.size()) > nIdx)
    {
        DataSerie* ds = m_aDataSeries.back().get();
        assert(ds);
        ds->Copy(adbData, unLength);

        wxColour emptyColor;
        
        if(lineColor != wxNullColour)
        {
            ds->SetLineColour(lineColor);
        }
        else
        {  // Choose color emptyColormatically
            Aurora::ChooseColour(emptyColor, nIdx);
            ds->SetLineColour(emptyColor);
        }

        if(brushColor != wxNullColour)
        {
            ds->SetFillColour(brushColor);
        }
        else
        {	
            // Choose color emptyColormatically
            Aurora::ChooseColour(emptyColor, nIdx);
            m_aDataSeries[nIdx]->SetFillColour(emptyColor);
        }

    }
}

void Aurora::AcParametersBarPlot::ReplaceSerie(const size_t nIdx, 
                                               const double* adbData, 
                                               const size_t unLength)
{
    wxASSERT_MSG(nIdx < m_aDataSeries.size(),
                 wxT("Aurora::AcParametersBarPlot: Replacement unexistent serie (check the index)."));
    wxASSERT_MSG(unLength == m_aDataSeries[nIdx]->size(),
                 wxT("Aurora::AcParametersBarPlot: Replacement serie lenght different than existing one."));
    m_aDataSeries[nIdx]->Copy(adbData, unLength);
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

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  plot.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::ConvolverPlot
\brief The decay graphic

*//*******************************************************************/
#include <aurora.h>

#include "ConvolverPlot.h"

//----------------------------------------------------------------------------
// Aurora::ConvolverPlot implementation
//----------------------------------------------------------------------------
void Aurora::ConvolverTimeDataSerie::Rms(const size_t unWindowLength, double& tlo, double& thi)
{
	//Samples that come in a single pixel
    Aurora::SampleCount iMax = (Aurora::SampleCount) floor( ((thi - tlo) * m_dbRate)/unWindowLength );

    Aurora::SampleCount k, i, t0, t1;
//	printf("\nLo : %f, Hi: %f\n", tlo, thi);
    auto length = m_timeSamples.size();

    for(k = 0; k < unWindowLength; k++)
    {
        // RMS on 1 px calculation
        double rms = 0.0;
        t0 = k*iMax + tlo * m_dbRate;
        t1 = t0 + iMax;

        if(!m_bIsTimeReversed)
        {
            for(i = t0; i < t1; i++)
            {
                if (i < length)
                {
                    rms += m_timeSamples[i] * m_timeSamples[i];
                }
            }
        }
        else
        {
            for(i = t0; i < t1; i++)
            {
                if (i < m_timeSamples.size())
                {
                    rms += m_timeSamples[(length - 1) - i] * m_timeSamples[(length - 1) - i];
                }
            }
        }
            rms /= (t1 - t0);

            at(k) = dB(rms); // + m_dbFullScale; // this must be done when plotting!
    //		printf("(%d, %d, %f)", int(t0), int(t1), m_aTData[k]);
    }
//    printf("\n"); fflush(stdout);
}

void Aurora::ConvolverTimeDataSerie::CalculateDataSerie(const size_t unWindowLength, double& tlo, double& thi)
{
	// No calculation is needed: use existing datas.
    resize(unWindowLength);

    switch(m_nHandlerType)
    {
        case HT_RMS: Rms(unWindowLength, tlo, thi); break;
        default:     Rms(unWindowLength, tlo, thi); break;
    }

}

bool Aurora::ConvolverTimeDataSerie::IsOk(const double tlo, const double thi)
{
    if ((tlo < 0) || (thi < 0))    
    {
        // Time cannot be negative
        return false;
    }

    if (tlo >= thi)                
    {
        // We cannot reverse time!
        return false;
    }

    if ((thi - tlo) < 1.0 / m_dbRate)
    {
        // Time interval smaller than sampling time are not accepted
        return false;
    }   

	return true;
}

//#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
//WX_DEFINE_OBJARRAY(TArrayOfConvolverTimeDataSerie);

//----------------------------------------------------------------------------
// Aurora::ConvolverPlot
//----------------------------------------------------------------------------
void Aurora::ConvolverPlot::AddTimeSerie(const Aurora::Vector<Sample>& data,
	                                     const double dbRate, 
	                                     const bool bIsTimeReversed,
                                         wxColour& lineColor, 
                                         wxColour& brushColor)
{
	int nIdx = int(m_aDataSeries.size());
	m_aDataSeries.emplace_back(new Aurora::ConvolverTimeDataSerie());

	if(int(m_aDataSeries.size()) > nIdx)
	{
        Aurora::DataSerie* ds = m_aDataSeries.back().get();
        auto tds = dynamic_cast<Aurora::ConvolverTimeDataSerie*>(ds);
        assert(tds);
        tds->SetRate(dbRate);
        tds->CopyTimeData(data);
		tds->SetTimeReversed(bIsTimeReversed);

		wxColour wxcolAuto;

		if(lineColor != wxNullColour)
		{
			tds->SetLineColour(lineColor);
		}
		else
		{   // Choose color automatically
			Aurora::ChooseColour(wxcolAuto, nIdx);
			tds->SetLineColour(wxcolAuto);
		}

		if(brushColor != wxNullColour)
		{
			tds->SetFillColour(brushColor);
		}
		else
		{   // Choose color automatically
			Aurora::ChooseColour(wxcolAuto, nIdx);
			tds->SetFillColour(wxcolAuto);
		}

	}
}

void Aurora::ConvolverPlot::ReplaceTimeSerie(const size_t nIdx, 
	                                         const Aurora::Vector<Sample>& data, 
	                                         const bool bIsTimeReversed)
{
    Aurora::DataSerie* ds = m_aDataSeries[nIdx].get();
    auto tds = dynamic_cast<Aurora::ConvolverTimeDataSerie*>(ds);
    assert(tds);

	wxASSERT_MSG(nIdx < m_aDataSeries.size(),
				 wxT("Aurora::ConvolverPlot: Replacement unexistent serie (check the index)."));
	wxASSERT_MSG(data.Length() == tds->GetSamplesCount(),
				 wxT("Aurora::ConvolverPlot: Replacement serie lenght different than existing one."));
	tds->CopyTimeData(data);
	tds->SetTimeReversed(bIsTimeReversed);
}

// *** constructor
Aurora::ConvolverPlot::ConvolverPlot(  wxWindow *parent, wxWindowID id )
   : Aurora::Scope<Sample>(parent, id)
   { }

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

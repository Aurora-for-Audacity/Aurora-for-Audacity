/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator
  
  STIPlot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_STI_PLOT_H__
#define __AURORA_STI_PLOT_H__

#include <aurora.h>

namespace Aurora
{
    class OctaveBandsDataSerie;
    class STISpectrum;

    //----------------------------------------------------------------------------
    // OctaveBandsPlot
    //----------------------------------------------------------------------------
    class OctaveBandsPlot : public Aurora::BarPlot
    {
    public:
        void ToggleSerieVisibility(const int nIdx) { m_aDataSeries[nIdx]->ToggleVisibility(); }

        void AddSerie(Aurora::OctaveBandsDataSerie* pSerie,
                      wxColour& wxcolLine  = wxNullColour, 
                      wxColour& wxcolBrush = wxNullColour);
        
        void AddSerie(const double* adbData, 
                      const size_t unLength,
                      wxColour& wxcolLine = wxNullColour, 
                      wxColour& wxcolBrush = wxNullColour);
        
        void AddSerie(Aurora::STISpectrum& spectrum,
                      wxColour& wxcolLine = wxNullColour, 
                      wxColour& wxcolBrush = wxNullColour);
        
        void ReplaceSerie(const size_t nIdx, 
                          const double* adbData, 
                          const size_t unLength);

        void ReplaceSerie(const size_t nIdx, 
                          Aurora::STISpectrum& spectrum);
        
        // A sort of alias, just to make things easier...
        CategoriesAxis& GetBandsAxis () { return GetCategoriesAxis(); }


        OctaveBandsPlot(wxWindow* pParent, 
                        wxWindowID id,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize)
        : Aurora::BarPlot(pParent, id, pos, size)
        { }
    };

    //----------------------------------------------------------------------------
    // STIPlot
    //----------------------------------------------------------------------------
    class STIPlot : public Aurora::LinePlot
    {
      public:
        Axis&           GetSTIAxis() { return GetLevelAxis(); }
        CategoriesAxis& GetFrqAxis() { return GetCategoriesAxis(); }


        STIPlot(wxWindow *pParent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize)
            : LinePlot(pParent, id, pos, size)
        { }

    };
} // namespace Aurora

#endif  //__AURORA_STI_PLOT_H__


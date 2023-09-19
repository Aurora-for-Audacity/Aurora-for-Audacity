/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions
  
  plot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_XFUNCTIONS_PLOT_H__
#define __AURORA_XFUNCTIONS_PLOT_H__

#include <aurora.h>

#include "correlator.h"

namespace Aurora
{
    class Correlator;

    //----------------------------------------------------------------------------
    // XFunctionsPlot
    //----------------------------------------------------------------------------
    class XFunctionsPlot: public Aurora::SScope
    {
    public:
        // XFunctionPlotType enum defined in correlator.h

        // --- set/getters
        void AddSerie(Aurora::Correlator* correlatorPtr,
                      Aurora::ComplexSpectrum& data,
                      const XFunctionsPlotType  handlerType,
                      const wxColour& plotColor = wxNullColour,
                      const bool bShow           = false);

        bool SetPlotType        (const XFunctionsPlotType nPlotType);
        bool SetPlotTypeByString(wxString& plotType);
        bool SetPlotTypeByString(wxString& plotType, int& nPlotType);

        // --- ctors
        XFunctionsPlot(wxWindow *pParent, wxWindowID id )
            : Aurora::SScope(pParent, id)
        { }
    };

} // namespace Aurora

#endif  // __AURORA_XFUNCTIONS_PLOT_H__

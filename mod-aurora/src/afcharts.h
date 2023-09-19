/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common Libraries

  afcharts.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFCHARTS_H__
#define __AURORA_AFCHARTS_H__

#include "afdataplot.h"

//----------------------------------------------------------------------------
// Aurora::ChartBase
//----------------------------------------------------------------------------
//  This template act as a base class for bar and line charts class templates
//----------------------------------------------------------------------------

namespace Aurora
{
    class Axis;
    class CategoriesAxis;

    class ChartBase: public DataPlot
    {
      protected:
        virtual void UpdateHorizontalAxis() override { }
        virtual void UpdateVerticalAxis() override;

      public:

        Axis& GetLevelAxis();
        CategoriesAxis& GetCategoriesAxis();

        // --- ctors
        ChartBase(wxWindow *parent, 
                  wxWindowID id,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize);
        virtual ~ChartBase() {}
    };

    //----------------------------------------------------------------------------
    // BarPlot
    //----------------------------------------------------------------------------
    //  This is a template of a 'bar chart' type plot, where there is a categories
    //  axis and a level axis.
    //----------------------------------------------------------------------------
    class BarPlot: public ChartBase
    {
     protected:

        void Draw(wxDC &dc);

      public:

        // --- ctors
        BarPlot(wxWindow *parent, 
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize)
         : ChartBase(parent, id, pos, size)
        { }

        ~BarPlot() {}
    };

    //----------------------------------------------------------------------------
    // LinePlot
    //----------------------------------------------------------------------------
    //  This is a template of a 'line chart' type plot, where there is a categories
    //  axis and a level axis.
    //----------------------------------------------------------------------------
    class LinePlot: public ChartBase
    {
      protected:

        void Draw(wxDC &dc);

      public:

        // --- ctors
        LinePlot(wxWindow *parent, 
                 wxWindowID id,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize)
        : ChartBase(parent, id, pos, size)
        {}

        ~LinePlot() {}
    };

} // namespace Aurora
#endif  // __AURORA_AFCHARTS_H__

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

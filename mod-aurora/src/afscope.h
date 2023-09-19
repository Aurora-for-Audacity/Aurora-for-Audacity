/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries
  
  afscope.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFSCOPE_H__
#define __AURORA_AFSCOPE_H__

#include "afdataplot.h"

namespace Aurora
{
    //----------------------------------------------------------------------------
    // ScopeBase
    //----------------------------------------------------------------------------
    // \brief This acts as a base class for Scope and SScope class templates.
    /// This class should not be instantiated directly, use, instead,
    /// Scope(), SScope(), Chart(), BarPlot().
    //----------------------------------------------------------------------------

    class ScopeBase : public Aurora::DataPlot 
    {
      protected:        
        void Draw(wxDC& dc);

      public:   
        /// Returns a pointer to horizontal (Time) axis.
        Axis& GetTimeAxis();

        /// Returns a pointer to vertical (Level, usually) axis.
        Axis& GetLevelAxis();

        /// Returns a pointer to horizontal (Time) axis.
        virtual Axis& GetHorzAxis();

        /// Returns a pointer to vertical (Level, usually) axis.
        virtual Axis& GetVertAxis();

        /// The constructor.
        /// \param parent - parent window
        /// \param id - window identifier
        /// \param pos - window position
        /// \param size - window size
        ScopeBase(wxWindow *parent,
                  wxWindowID id,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize );
        ~ScopeBase() {}
    };

    //----------------------------------------------------------------------------
    // Scope
    //----------------------------------------------------------------------------
    /// This is a template of a 'scope' type plot, designed to show a continuous
    /// time-dependend set of real data; it has an horizontal time axis and a
    /// vertical 'level' axis.
    //----------------------------------------------------------------------------
    template<typename T> 
    class Scope : public ScopeBase
    {
      protected:
       virtual void UpdateVerticalAxis() override;
       virtual void UpdateHorizontalAxis() override;

     public:
       /// The constructor.
       /// \param parent - parent window
       /// \param id - window identifier
       /// \param pos - window position
       /// \param size - window size
       Scope(wxWindow *parent,
             wxWindowID id,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize )
        : ScopeBase(parent, id, pos, size)
        { }

       ~Scope() {}
    };

    // --------------------- Scope Template definitions ------------------------

    template<typename T>
    void Scope<T>::UpdateHorizontalAxis()
    {
        if (this->m_aDataSeries.empty())
        {
            return;
        }

        double tmin, tmax;
        GetTimeAxis().GetRange(tmin, tmax);

        // Range check
        // NB: All time series should have the same length
        auto ds = this->m_aDataSeries[0].get();
        auto tds = dynamic_cast<TTimeDataSerie<T>*>(ds);

        if (!tds->IsOk(tmin, tmax))
        {
            this->SetHorzAxisAutorange(true);
        }

        if (this->m_bIsHorzAutorangeOn)
        {
            tmin = (GetTimeAxis().IsLog()) ? 1.0 : 0.0;
            tmax = tds->GetMax();
        }

        GetTimeAxis().SetRange(tmin, tmax);
    }

    template<typename T>
    void Scope<T>::UpdateVerticalAxis()
    {
        if(this->m_aDataSeries.empty()) 
        {
            return;
        }

        double tmin, tmax;
        this->GetTimeAxis().GetRange(tmin, tmax);
        
        this->GetLegend()->Clear();
        
        for(auto it = this->m_aDataSeries.begin(); it != this->m_aDataSeries.end(); ++it)
        {         
            auto serie = dynamic_cast<TTimeDataSerie<T>*>(it->get());

            if(serie->IsShown())
            {
                serie->CalculateDataSerie(this->GetClientAreaWidth(), 
                                          tmin, 
                                          tmax);

                this->GetLegend()->AddLabel(serie->GetLabel(),
                                            serie->GetLineColour());
            }
        }

        if(this->m_bIsVertAutorangeOn)
        {
            double min = 1.0e+12;
            double max = -1.0e-12;
    //        this->m_pLevelAxis->GetRange(min, max);
            double min1 = 0;
            double max1 = 0;

            for(auto it = this->m_aDataSeries.begin(); it != this->m_aDataSeries.end(); ++it)
            {        
                auto serie = dynamic_cast<TTimeDataSerie<T>*>(it->get());

                if(serie->IsShown())
                {
                   this->Autorange(serie->data(),
                                   serie->size(),
                                   min1, max1);

                   min = (min1 < min) ? min1 : min;
                   max = (max1 > max) ? max1 : max;
                }
            }
            this->GetLevelAxis().SetRange(min, max);
        }
    }

    //----------------------------------------------------------------------------
    // SScope
    //----------------------------------------------------------------------------
    /// This is a template of a 'spectral scope' type plot, designed to show an
    /// audio spectrum; it has an horizontal logarithmic frequency axis and a
    /// vertical 'level' axis.
    //----------------------------------------------------------------------------
    class SScope : public ScopeBase
    {
     protected:
       virtual void UpdateHorizontalAxis() override;
       virtual void UpdateVerticalAxis() override;
       
     public:   
       /// Returns a pointer to horizontal (Frequency) axis.
         Axis& GetFreqAxis();

       /// Returns a pointer to horizontal (Frequency) axis.
        virtual Axis& GetHorzAxis() override;

       /// The constructor.
       /// \param parent - parent window
       /// \param id - window identifier
       /// \param pos - window position
       /// \param size - window size
       SScope(wxWindow* parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize )
         : ScopeBase(parent, id, pos, size)
        { 
            GetFreqAxis().SetName(wxT("Frequency")); 
        }
    };


}
#endif  // __AURORA_AFSCOPE_H__
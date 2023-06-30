/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common Libraries
  
  afdataplot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFDATAPLOT_H__
#define __AURORA_AFDATAPLOT_H__

#include "afdata.h"
#include "afplot.h"

namespace Aurora
{
    //----------------------------------------------------------------------------
    // DataPlot
    //----------------------------------------------------------------------------
    /// \brief This is a template of a 'plot with data' (see afdata.h).
    /// The 'next step' after PlotBase, with the addition of structures
    /// (called \e data \e series) useful for represented data.\n
    /// This class should not be instantiated directly, use, instead,
    /// Scope(), SScope(), Chart(), BarPlot().
    //----------------------------------------------------------------------------

    class DataPlot : public Aurora::PlotBase
    {
      protected:
         std::vector< std::unique_ptr<DataSerie> > m_aDataSeries;

      public:
       
        DataSerie* GetDataSerie(const size_t idx) { return m_aDataSeries[idx].get(); }
        
        size_t GetDataSeriesCount() const { return m_aDataSeries.size();  }

        /// Set data serie visibility flag
        /// \param nIdx - data serie index
        /// \param bShow - visibility flag.
        virtual void ShowSerie(const size_t nIdx, const bool bShow = true);

        void AddSerie(DataSerie* pData) { m_aDataSeries.emplace_back(pData); }

        void InsertSerie (const size_t nIdx, DataSerie* pData);
        /**
        * @brief Replace an object in the list, taking ownership of the pointer.
        * @param nIdx The index of the object to replace
        * @param ds The replacing object.
        */
        void ReplaceSerie(const size_t nIdx, DataSerie* ds);
        void DeleteSerie (const size_t nIdx);
        void ClearSerie  () { m_aDataSeries.clear(); }

        /// The constructor.
        /// \param parent - parent window
        /// \param id - window identifier
        /// \param pos - window position
        /// \param size - window size
        DataPlot(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize )
        : PlotBase(parent, id, pos, size)
        {}
       
        ~DataPlot() {}    
    };


    // ------------------------------------------ Template definitions ---------------------------------------------

} // namespace Aurora
#endif  // __AURORA_DATAPLOT_H__

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

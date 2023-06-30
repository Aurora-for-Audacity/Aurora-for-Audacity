/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters
  
  plot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ACPARAM_PLOT__
#define __AURORA_ACPARAM_PLOT__
#include <aurora.h>

#include "AcParametersBase.h"
#include "AcParametersData.h"

class AcParametersAudioTrack;
class TSchroederDecay;

namespace Aurora
{
    //----------------------------------------------------------------------------
    // AcParametersDecayPlot declaration
    //
    /// \brief The decay plot for Acoustical Parameter module.
    /// This class is designed to show a decay plot, tipically obtained from
    /// a room Impulse Response by Schroeder backward integration.
    //----------------------------------------------------------------------------
    class AcParametersDecayPlot: public Scope <Sample>
    {
      public:
        /// Append data serie to plot.
        /// \param pData - a pointer to an object that holds decay datas
          // TBR: not needed
         void AddSerie(AcParametersTimeDataSerie* tds); // Not exactly what I want: this is already defined in AFDataPlot! 

      protected:
          
        /// Append data serie to plot.
        /// \param psmpData - a pointer to a vector that holds decay datas
        /// \param unLength - data vector length
        /// \param dbRate - data sample rate in hertz
        /// \param nHandlerType - data handler identifier
        /// \param lineColor - line color
        /// \param brushColor - brush color, not really necessary..
        void AddSerie(Sample* psmpData,
                      const size_t unLength,
                      const double dbRate,
                      const int nHandlerType,
                      wxColour& lineColor  = wxNullColour, 
                      wxColour& brushColor = wxNullColour);

        /// Replace a data serie
        /// \param nIdx - index of data serie to be replaced
        /// \param psmpData - a pointer to a vector that holds new datas
        /// \param unLength - data vector length
        void ReplaceSerie(const size_t nIdx,
                          Sample* psmpData,
                          const size_t unLength);

      public:
        /** 
         * @brief Add a track to scope.
         * @param nHandlerType Data handler identifier
         * @param lineColor Line color
         * @param brushColor Brush color, not really necessary...
         */
        void AddSerie(AcParametersAudioTrack& track,
                      const int nHandlerType,
                      wxColour& lineColor  = wxNullColour, 
                      wxColour& brushColor = wxNullColour);

        /**
         * @brief Add a Schroeder decay to scope.
         * @param nHandlerType Data handler identifier
         * @param lineColor Line color
         * @param brushColor Brush color, not really necessary...
         */
        void AddSerie(TSchroederDecay& decay,
                        const int nHandlerType,
                        wxColour& lineColor  = wxNullColour, 
                        wxColour& brushColor = wxNullColour);

        /**
         * @brief Repace the shown track.
         * @param nIdx Index of data serie to be replaced.
         * @param track The new track.
         */
        void ReplaceSerie(const size_t nIdx, AcParametersAudioTrack& track);

        /**
         * @brief Repace the shown decay.
         * @param nIdx Index of data serie to be replaced.
         * @param decay The new decay.
         */
        void ReplaceSerie(const size_t nIdx, TSchroederDecay& decay);

        /// The constructor
        /// \param parent - pointer to parent window/widget
        /// \param id - widget ID
        /// \param pos - widgets position
        /// \param size - widget size
        AcParametersDecayPlot(wxWindow *parent, 
                              wxWindowID id,
                              const wxPoint& pos = wxDefaultPosition,
                              const wxSize& size = wxSize(DECAY_PLOT_X_SIZE,  
                                                          DECAY_PLOT_Y_SIZE))
        : Scope<Sample>(parent, id, pos, size)
        {}
    };

    //----------------------------------------------------------------------------
    // AcParametersBarPlot declaration
    //
    /// \brief The decay plot for Acoustical Parameter module.
    /// This class is used to represent graphically the values of the
    /// acoustical parameters by octave band. Currently it accepts only
    /// 12 values data series: the 10 octave bands, plus A filtered and
    /// the flat average (Lin).
    //----------------------------------------------------------------------------
    class AcParametersBarPlot: public BarPlot//<TArrayOfDataSerie, 
                                             // DataSerie>
    {
      public:
       /**
        * @brief Append a data serie.
        * @param spectrum The data to load.
        * @param lineColor - line color
        * @param brushColor - brush color, not really necessary..
        */
        void AddSerie(AcParametersSpectrum& spectrum,
                      wxColour& lineColor  = wxNullColour,
                      wxColour& brushColor = wxNullColour);

        /// Append a data serie.
        /// \param adbData - vector holding the parameter values
        /// \param unLength - vector length...but for the current version
        ///                   MUST be 12.
        /// \param lineColor - line color
        /// \param brushColor - brush color, not really necessary..
        void AddSerie(const double* adbData,
                      const size_t unLength,
                      wxColour& lineColor = wxNullColour,
                      wxColour& brushColor = wxNullColour);

        /// Replace a data serie
        /// \param nIdx - index of data serie to be replaced
        /// \param adbData - a pointer to a vector that holds new datas
        /// \param unLength - data vector length...12!
        void ReplaceSerie(const size_t nIdx, 
                          const double* adbData, 
                          const size_t unLength);

        /// Returns a pointer to horizontal axis.
        CategoriesAxis& GetBandsAxis() { return GetCategoriesAxis(); }

        /// The constructor
        /// \param parent - pointer to parent window/widget
        /// \param id - widget ID
        /// \param pos - widgets position
        /// \param size - widget size
        AcParametersBarPlot(wxWindow *parent,
                            wxWindowID id,
                            const wxPoint& pos = wxDefaultPosition,
                            const wxSize& size = wxDefaultSize)
        : BarPlot(parent, id, pos, size)
        { }

    private:
        std::vector<double> m_buffer;

    };
}
#endif  // __AURORA_ACPARAM_PLOT__

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



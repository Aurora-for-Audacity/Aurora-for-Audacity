/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver
  
  plot.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_CONVOLVER_PLOT_H__
#define __AURORA_CONVOLVER_PLOT_H__

namespace Aurora
{
    class ConvolverTimeDataSerie : public TTimeDataSerie<Sample>
    {
    public:
        enum { HT_RMS = 100 };

    private:
        bool m_bIsTimeReversed = false;

        // Data handlers
        void Rms(const size_t unWindowLength, double& dbTLo, double& dbTHi);

    public:
        void CalculateDataSerie(const size_t nWindowLength, double& dbTLo, double& dbTHi);

        bool IsOk(const double dbTLo, const double dbTHi);

        bool IsTimeReversed() const { return m_bIsTimeReversed; }
        void SetTimeReversed(const bool bValue) { m_bIsTimeReversed = bValue; }

        ConvolverTimeDataSerie& operator=(ConvolverTimeDataSerie&& tds)
        {
            TTimeDataSerie::operator=(std::move(tds));
            return *this;
        }

        ConvolverTimeDataSerie& operator=(const ConvolverTimeDataSerie& tds)
        {
            TTimeDataSerie::operator=(tds);
            return *this;
        }

        ConvolverTimeDataSerie() { }

        ConvolverTimeDataSerie(ConvolverTimeDataSerie&& tds)
            : TTimeDataSerie<Sample>(std::move(tds))
        { }

        ConvolverTimeDataSerie(const ConvolverTimeDataSerie& tds)
            : TTimeDataSerie<Sample>(tds)
        { }

        ConvolverTimeDataSerie(const double dbRate)
            : TTimeDataSerie<Sample>(dbRate, HT_RMS)
        { }

        ConvolverTimeDataSerie(const size_t unVectorLength,
                               const double dbRate)
            : TTimeDataSerie<Sample>(unVectorLength, dbRate, HT_RMS)
        { }
        /*
        ConvolverTimeDataSerie(const size_t unVectorLength,
            Sample* psmpVector,
            const double dbRate = 44100.0)
            : TTimeDataSerie<Sample>(unVectorLength, psmpVector, dbRate, HT_RMS)
        {}*/

    };
}
//WX_DECLARE_OBJARRAY(Aurora::ConvolverTimeDataSerie, TArrayOfConvolverTimeDataSerie);
typedef std::vector<Aurora::ConvolverTimeDataSerie> TArrayOfConvolverTimeDataSerie;

namespace Aurora
{
    //----------------------------------------------------------------------------
    // ConvPlot
    //----------------------------------------------------------------------------

    class ConvolverPlot: public Scope <Sample>
    {
      public:
    //   void SetTimeReversed(const bool bValue);

       void AddTimeSerie(const Vector<Sample>& data, 
                         const double dbRate, 
                         const bool bIsTimeReversed,
                         wxColour& wxcolLine = wxNullColour, 
                         wxColour& wxcolBrush = wxNullColour);

       void ReplaceTimeSerie(const size_t nIdx, 
                             const Vector<Sample>& data, 
                             const bool bIsTimeReversed);

       // --- ctors
       ConvolverPlot(  wxWindow *parent, wxWindowID id ); // on vc++ the inlined constructor doesn't link
    };
} // namespace Aurora

#endif  // __AURORA_CONVOLVER_PLOT__

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

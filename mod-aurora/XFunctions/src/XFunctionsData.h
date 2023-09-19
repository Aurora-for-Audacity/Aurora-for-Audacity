/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions
  
  data.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_XFUNCTIONS_DATA_H__
#define __AURORA_XFUNCTIONS_DATA_H__

#ifndef USE_KISS_FFT
#  define USE_KISS_FFT 1
#endif

#include <tools/kiss_fftr.h>

namespace Aurora
{
    //----------------------------------------------------------------------------
    // XFSpectrumDataHandler function pointer typedef
    //----------------------------------------------------------------------------
    class XFunctionsSpectralDataSerie;
    typedef double (XFunctionsSpectralDataSerie::*TXFAvgFunction)(const int f0,
                                                                  const int f1);

    //----------------------------------------------------------------------------
    // XFSpectrumDataSerie
    //----------------------------------------------------------------------------
    class XFunctionsSpectralDataSerie : public TSpectralDataSerie
    {
    public:
        // XFunctionPlotType enum defined in correlator.h

    protected:
        Aurora::Correlator* m_correlator = nullptr;
        TXFAvgFunction m_fpAverage;

        double Rll      (const int f0, const int f1);
        double Rrr      (const int f0, const int f1);
        double Magn     (const int f0, const int f1);
        double Phaze    (const int f0, const int f1);
        double SqrReal  (const int f0, const int f1);
        double SqrImag  (const int f0, const int f1);
        double Alpha    (const int f0, const int f1); 
        double Coherence(const int f0, const int f1);

    public:
        void CalculateDataSerie(const size_t nWindowLength,
                                const double dbLo,
                                const double dbHi,
                                const bool bIsLog) override;
        
        void FindMaxMin(double& max, double& min);

        bool IsOk(const double fLo, const double fHi) override;

        double GetCorrelationLag() const { return (double)m_spectrum.GetFrequencyBinsWidth(); }        
        Aurora::Correlator& GetCorrelator() { return *m_correlator;  }

        void SetCorrelatorPtr(Aurora::Correlator* c) { m_correlator = c; }
        void SetHandlerId(const XFunctionsPlotType type);

        void CopySpectralData(const ComplexSpectrum& src);

    // copy operators & methods
        XFunctionsSpectralDataSerie& operator=(XFunctionsSpectralDataSerie&& sds);        
        XFunctionsSpectralDataSerie& operator=(const XFunctionsSpectralDataSerie& sds);
    
    // --- 'ctors
        XFunctionsSpectralDataSerie() { }
        
        XFunctionsSpectralDataSerie(XFunctionsSpectralDataSerie&& sds)
         : TSpectralDataSerie(std::move(sds)),
            m_correlator(sds.m_correlator)
        { }

         XFunctionsSpectralDataSerie(const XFunctionsSpectralDataSerie& sds)
         : TSpectralDataSerie(sds),
            m_correlator(sds.m_correlator)
        { }

        XFunctionsSpectralDataSerie(Aurora::Correlator* correlator,
                                    const XFunctionsPlotType nHandlerType);
    };

} // namespace aurora

typedef std::vector<Aurora::XFunctionsSpectralDataSerie> TArrayOfXFunctionsSpectralDataSeries;

#endif  // __AURORA_XFUNCTIONS_DATA_H__

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

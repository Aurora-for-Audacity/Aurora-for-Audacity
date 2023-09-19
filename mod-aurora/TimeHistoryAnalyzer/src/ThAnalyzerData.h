/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
 Time History Analyzer
  
  ThAnalyzerData.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ITUP56_DATA_H__
#define __AURORA_ITUP56_DATA_H__

//----------------------------------------------------------------------------
// AFTimeDataSerie declaration
//----------------------------------------------------------------------------
namespace Aurora
{
    class ThAnalyzerTimeDataSerie : public TTimeDataSerie<double>
    {
      public:
        enum 
        { 
            HT_RMS = 100, 
            HT_DECIMATE
        };

      private:
        double m_dbFullScale;

        // Data handlers
        void Rms     (const size_t unWindowLength, double& tLo, double& tHi) { }
        void Decimate(const size_t unWindowLength, double& tLo, double& tHi);

    public:
        void CalculateDataSerie(const size_t nWindowLength, 
                                double& dbTLo, 
                                double& dbTHi) override;

        bool IsOk(const double dbTLo, const double dbTHi) override; 

        void   SetFullScale(const double dbValue) { m_dbFullScale = dbValue; }
        double GetFullScale() const { return m_dbFullScale; }

      ThAnalyzerTimeDataSerie(ThAnalyzerTimeDataSerie&& tds)
       : TTimeDataSerie<double>(std::move(tds))
       { }

      ThAnalyzerTimeDataSerie(ThAnalyzerTimeDataSerie& tds)
       : TTimeDataSerie<double>(tds)
       { }

      ThAnalyzerTimeDataSerie(ThAnalyzerAudioTrack& track,
                              const int handlerType = HT_DECIMATE);

      ThAnalyzerTimeDataSerie(const std::vector<double> buffer, 
                              const double rate      = 44100.0, 
                              const double fullScale = 120.0, 
                              const int  handlerType = HT_DECIMATE)
       : TTimeDataSerie<double>(buffer.size(), rate, handlerType),
         m_dbFullScale(fullScale)
      {
          CopyTimeData(buffer.data(), buffer.size());
      }
    };
} // namespace

typedef std::vector<Aurora::ThAnalyzerTimeDataSerie> TArrayOfThAnalyzerTimeDataSerie;


#endif  // __AURORA_ITUP56_DATA_H__

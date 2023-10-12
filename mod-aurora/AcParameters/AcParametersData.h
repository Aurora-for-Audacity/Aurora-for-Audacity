/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters
  
  data.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ACPARAM_DATA_H__
#define __AURORA_ACPARAM_DATA_H__

#include <afdata.h>

class AcParametersAudioTrack;
//class Aurora::TSchroederDecay;

namespace Aurora
{
    //----------------------------------------------------------------------------
    // AFTimeDataSerie
    //----------------------------------------------------------------------------

    class AcParametersTimeDataSerie : public TTimeDataSerie<Sample>
    {
    public:
        enum 
        { 
            HT_RMS = 100, 
            HT_DECIMATE 
            
        };

    private:
        double m_fullscale = 120.0;

        // Data handlers
        void Rms     (const size_t unWindowLength, double& dbTLo, double& dbTHi);
        void Decimate(const size_t unWindowLength, double& dbTLo, double& dbTHi);

    public:
        void CalculateDataSerie(const size_t nWindowLength, 
                                double& dbTLo, 
                                double& dbTHi) override;

        bool IsOk(const double dbTLo, const double dbTHi) override;

        void SetFullscale(const double fs) { m_fullscale = fs;  }

        AcParametersTimeDataSerie& operator=(AcParametersTimeDataSerie&& tds)
        {
            TTimeDataSerie::operator=(std::move(tds));
            return *this;
        }

        AcParametersTimeDataSerie& operator=(const AcParametersTimeDataSerie& tds)
        {
            TTimeDataSerie::operator=(tds);
            return *this;
        }
        
        AcParametersTimeDataSerie() { }

        AcParametersTimeDataSerie(AcParametersTimeDataSerie&& tds)
         : TTimeDataSerie<Sample>(std::move(tds))
        { }

        AcParametersTimeDataSerie(const AcParametersTimeDataSerie& tds)
         : TTimeDataSerie<Sample>(tds)
        { }

        AcParametersTimeDataSerie(AcParametersAudioTrack& track, 
                                  const int nHandlerType = HT_DECIMATE);
        
        AcParametersTimeDataSerie(TSchroederDecay& decay, 
                                 const int nHandlerType = HT_DECIMATE);

    };
}

typedef std::vector<Aurora::AcParametersTimeDataSerie> TArrayOfAcParametersTimeDataSerie;

//----------------------------------------------------------------------------
// AFDataSerie
//----------------------------------------------------------------------------
typedef std::vector<Aurora::DataSerie> TArrayOfDataSerie;

#endif // __AURORA_ACPARAM_DATA_H__

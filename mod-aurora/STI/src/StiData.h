/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator
  
  StiData.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_STI_DATA_H__
#define __AURORA_STI_DATA_H__

namespace Aurora
{

    //----------------------------------------------------------------------------
    // OctaveBandsDataSerie
    //----------------------------------------------------------------------------
    class OctaveBandsDataSerie : public Aurora::DataSerie
    {
    // 31.5 63 125 250 500 1k 2k 4k 8k 16k A lin
    public:
        OctaveBandsDataSerie()
        : Aurora::DataSerie(12)
        { }

        OctaveBandsDataSerie(const double* pdbSerie)
        : Aurora::DataSerie(12)
        { 
            Copy(pdbSerie, 12); 
        }

        OctaveBandsDataSerie(const float* pflSerie)
        : Aurora::DataSerie(12)
        { 
            Copy(pflSerie, 12);             
        }
    };
}

typedef std::vector<Aurora::OctaveBandsDataSerie> TArrayOfStiOctaveBandsDataSerie;
typedef std::vector<Aurora::DataSerie>            TArrayOfAuroraDataSerie;

#endif  //__AURORA_STI_DATA_H__

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

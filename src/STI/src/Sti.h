/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  sti.h

  Angelo Farina - Simone Campanini

***********************************************************************/
#ifndef __AURORA_STI_H__
#define __AURORA_STI_H__

#include <array>
#include <map>

namespace Aurora
{
    class STIAudioTrack;
    
    class STISpectrum : public Aurora::Spectrum<double>
    {
      public:
        enum Range
        {
            Full,
            Reduced,
            FiveBands,
            
            SevenBands = 1
        };
        
        enum Profile
        {
            DefaultNoise,
            DefaultSignal,
            MaskingThreshold,
            MaskingRange,     // A.Farina STI computation
            MaskingSlope,     // A.Farina STI computation
            FemaleCorrection,
            AlphaMale,        //coeff di peso voce maschile
            BetaMale,
            AlphaFemale,       //coeff di peso voce femminile
            BetaFemale,

            OMSTItel,  // used in tracks
            OMRaSTI //,
            //OMMTI
        };
                
      public:
          static const STISpectrum& MaskingThresholdsProfile();
          static const STISpectrum& MaskingRangeProfile();
          static const STISpectrum& MaskingSlopeProfile();
          static const STISpectrum& FemaleCorrectionProfile();
          static const STISpectrum& AlphaMaleProfile();
          static const STISpectrum& BetaMaleProfile();
          static const STISpectrum& AlphaFemaleProfile();
          static const STISpectrum& BetaFemaleProfile();
          static const STISpectrum& OMSTItelProfile();
          static const STISpectrum& OMRaSTIProfile();

          void SetProfile(const STISpectrum::Profile profile);

          void SetRange(const STISpectrum::Range range);
          void SetRange(const float fcbStart, const float fcbEnd);
          
          STISpectrum& operator=(const STISpectrum& s);
          
          STISpectrum() { Reset(); }
          STISpectrum(const STISpectrum& s);
          STISpectrum(const float fcbStart, const float fcbEnd);
          STISpectrum(const STISpectrum::Profile profile);
    };    
    
    /**
     * @brief The Speech Transfer Index computation class
     */
    class STI : public Aurora::ProgressMeterWrapper
    {
      public:
        // OctaveBands defined in commdefs.h

        enum Genre 
        { 
            Female = 0, 
            Male         
        };
    
        /**
        * @brief Container for STI computations.
        */
        class ModulationTransferFunctionsTable
        {
            std::map<double, Aurora::STISpectrum> m_table;
            
            std::array<double, 14> m_frequencies = 
            {
                0.63, 0.8, 1.0, 1.25, 1.6,  2.0,  2.5, 
                3.15, 4.0, 5.0, 6.3,  8.0, 10.0, 12.5
            };
            
          public:
            void Clear();
            
            const std::array<double, 14>& GetFrequencies() const { return m_frequencies; }
            
            STISpectrum& GetSpectrum(const double modulationFrequency);
            
            double GetValue(const double modulationFrequency,
                            const float fcb) const;
            
            void SetValue(const double modulationFrequency,
                          const float fcb,
                          const double value);
            
            ModulationTransferFunctionsTable() { Clear(); }
        };
        
        // *** Results vectors & tables
        class Results
        {
          public:
            Aurora::STISpectrum noiseSpectrum;
            Aurora::STISpectrum signalSpectrum;             
            Aurora::STISpectrum sigNoiseSpectrum;
            
          protected:  
            ModulationTransferFunctionsTable tMTF;   // These are the original values
            Aurora::STISpectrum  aRaSTI;  // five bands...
            Aurora::STISpectrum  aSTItel; // reduced (7) bands
            
          public:  
            ModulationTransferFunctionsTable tMTFf;  // These are final values,             
            Aurora::STISpectrum  aRaSTIf;               // so they  can be
            Aurora::STISpectrum  aSTItelf;              // denoised, masked , etc
            
            double STImale   = 0.0;
            double STIfemale = 0.0;
            double STIpa     = 0.0;
            
            Aurora::STISpectrum  aMTI; // reduced (7) bands (Modulation Transfer Indices)
            double RaSTI  = 0.0;
            double STItel = 0.0;
            
            Results();
            
            friend class Aurora::STI;
        };
        
      private:        
        std::array<Results, 2> m_results;

        bool   m_bErrorFlag          = false;
        
        double m_dbSamplerate         = 0.0; // TBR: not used...
        double m_dbCalibLevel     [2] = {  94.0,  94.0 };
        double m_dbFullScaleLevel [2] = { 120.0, 120.0 };
        bool   m_bIsSignalPlusNoise   = false;
        double m_dbFAT                = 0.20; // First Arrive Threshold :
                                              //  0 -> 1.0 NOT percentage!!
        bool   m_bSNCorrection      = false;
        bool   m_bMaskingCorrection = false;
        
        double Flatten(double& dbValue, const double dbFlat);
        
      protected:
        
        void CalibrateFullscale(STIAudioTrack& calibrationSignal,
                                const int nChnl);
        
        void ComputeOctaveSpectrum(STIAudioTrack& track, 
                                   STISpectrum& octaveSpectrum, 
                                   const int nChnl);
        
        bool StoreSpectrums(STIAudioTrack& signal, 
                            STIAudioTrack& noise, 
                            const int nChnl);  
        
        bool CalculateMatrix(STIAudioTrack& track, 
                             const int nChnl);
    public:
        /**
         * @brief Calculate Speech Transmission Index
         * @param nChnl The channel
         */
        bool Compute(const int nChnl);

    private:
        void ApplyMaskingCorrection(const int nChnl, 
                                    const STISpectrum& sigPlusNoiseLevel, 
                                    const STISpectrum& AMF);
        
        void DoSNR   (const int nChnl);
        void DoSTI   (const int nChnl, const int nSex);
        void DoRaSTI (const int nChnl);
        void DoSTItel(const int nChnl);
        void DoSTIPa (const int nChnl);

    public:
        // checkers
        bool IsSignalPlusNoise  () const { return m_bIsSignalPlusNoise; }
        bool IsSNCorrection     () const { return m_bSNCorrection; }
        bool IsMaskingCorrection() const { return m_bMaskingCorrection; }
        
        // getters
        double GetSamplerate           () const { return m_dbSamplerate; }
        double  GetFirstArriveThreshold() const { return m_dbFAT; }
        
        double GetCalibrationLevel(const int nChnl) const 
        { 
            return m_dbCalibLevel [nChnl];             
        }
        
        double GetFullScaleLevel(const int nChnl) const 
        { 
            return m_dbFullScaleLevel [nChnl]; 
        }
        
        const Results& GetResults(const int channel) const 
        { 
            return m_results[channel]; 
        } 

        Results& GetResults(const int channel)
        {
            return m_results[channel];
        }

        // setters    
        void SetSignalLevel(const double value, 
                            const float  fcb, 
                            const int    nChnl, 
                            const bool   bUpdateSigNoiseLevel = false);

        void SetNoiseLevel(const double value, 
                           const float  fcb, 
                           const int    nChnl, 
                           const bool   bUpdateSigNoiseLevel = false);

        void SetSigNoiseLevel(const double value, 
                              const float  fcb, 
                              const int    nChnl);

        void SetSigNoiseLevel(const float fcb, const int nChnl);

        void SetSignalPlusNoise  (const bool spn) { m_bIsSignalPlusNoise = spn; }
        void SetSNCorrection     (const bool snc) { m_bSNCorrection = snc; }
        void SetMaskingCorrection(const bool mc)  { m_bMaskingCorrection = mc; }

        void SetSamplerate(const double fs) { m_dbSamplerate = fs; }
        void SetFirstArriveThreshold(const double fat) { m_dbFAT = fat; }
        
        void SetCalibrationLevel(const double ldB,  const int nChnl);
        void SetFullScaleLevel  (const double fsdB, const int nChnl);
        
        void ReadConfigurationValues();
        void StoreConfigurationValues();

    protected:
        wxString GetCaptionString() const override { return wxString("Aurora STI"); }

    public:
        // 'ctors
        STI() { }
        ~STI() { }
    };

} // namespace Aurora

#endif //__AURORA_STI_H__



/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters
  
  AcParametersBase.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_AFACPAR_H__
#define __AURORA_AFACPAR_H__

#include <array>
#include <unordered_map>

#include "AcParametersTrack.h"

#define FIND_VECTOR_MAX(x, max, bd) \
   for(unsigned long k = 0; k < bd; k++) \
       if(x > max) max = x;

class SCOctaveBandAnalysisBase;

namespace Aurora
{
    //----------------------------------------------------------------------------
    // TSchroederDecay
    //
    /// \brief The Schroeder decay holder
    //
    /// This code calculates the Schroeder decay from an impulse response and
    /// holds it.

    class TSchroederDecay : public std::vector<Aurora::Sample>
    {
      private:
        Aurora::SampleCount m_fat = 0;         // First Arrival Time
        double m_dbTP2 = 0.0;
        double m_dbRate = 0.0;

        public:
        void Dump(const int idx = -1);

        Aurora::SampleCount GetLength() const { return size(); }
        double GetTP2  () const { return m_dbTP2; }
        double GetRate () const { return m_dbRate; }

        void FindFirstArrivalTime();
        Aurora::SampleCount GetFirstArrivalTimeInSamples() const { return m_fat; }

        void SetData(Aurora::Sample* src, const Aurora::SampleCount length);

        void DoBackwardIntegration(AcParametersAudioTrack& track);
        void Init(AcParametersAudioTrack& track);
        void Init(const Aurora::SampleCount length);

        TSchroederDecay() { }
        TSchroederDecay(const Aurora::SampleCount length);
    };

    //----------------------------------------------------------------------------
    // AFAcousticalParameters
    //
    /// \brief The Acoustical Parameters calculation class.
    //
    /// The code of this class belongs directly from the original
    /// Angelo Farina's Cool Edit/Audition plug-in code. In this
    /// version many features has been added, like the multichannel
    /// analysis.
    //----------------------------------------------------------------------------

    class AcousticalParameters : public Aurora::ProgressMeterWrapper
    {
      public:   
          
        /**
         * @brief Supported sources.
         */
        enum Source
        {
            Mono,
            
            TwoOmniMics,
            SoundfieldWY,
            PuProbe,
            IntensityProbe,
            Binaural,
            
            FourOmniMics,
            AmbisonicsFuMa,  ///< WXYZ (W channel -3 dB)
            AmbisonicsAmbix  ///< WYZX           
        };
        
        /**
         * @brief IACC Computation time intervals.
         */
        enum IACCSpan 
        { 
            None = 0,
            Early,   ///< 0 - 80 ms
            Late,    ///< 80 ms - inf
            All      ///< 0 - inf
        };        

        enum Errors 
        { 
            ERR_NO_FAT = 5000, 
            ERR_SPATIAL_IR_TOO_SHORT, 
            ERR_NO_CONFIG_SOURCE, 
            ERR_UNKNOWN 
        };

        // -------------------------- Data structures ----------------------------


        class TResults
        {
          public:
              
            struct TParameter 
            { 
                double  value = 0.0; 
                bool  isValid = false; 
                
                TParameter() { }
                TParameter(const double v,
                           const bool   valid = true);
            };                        
            
          private:
              
             static const std::vector<std::string> m_parameterNames;  /*=
            {
                "Signal",   "Noise", 
                "strenGth", "C50",    "C80",  "D50",  "Ts",   
                "EDT",      "Tuser",  "T20",  "T30",

                "Peakiness", " Millisecondness",  "Impulsiveness",

                "St1",     "St2",        "StLate",     // StageParameters (14..)
                "IACC",    "tauIACC",    "widthIACC",  // BinauralParameters (17..)
                "Jlf",     "Jlfc",       "Lj",          // SpatialParameters  (20..)
            }; */
                    
            std::unordered_map< std::string, Aurora::Spectrum<TParameter> > m_table;
            
          public:
              
            const std::vector<std::string>& Parameters() const
            {
                return m_parameterNames;
            }
            
            const std::vector<float>& Frequencies() const
            {
                return m_table.at("Signal").Frequencies();
            }
            
            Aurora::Spectrum<TParameter>& GetSpectrum(const std::string& parameterName)
            {
                return m_table.at(parameterName);
            }
            
            const TParameter& Get(const std::string& parameterName, const float fcb) const
            {
                return m_table.at(parameterName).GetValue(fcb);
            }
            
            void Set(const std::string& parameterName,
                     const float  fcb,
                     const double value,
                     const bool   valid = true);
                        
            void SetSpectrumType(const Aurora::SpectrumType type);

            TResults();
        };
            
        // -------------------------- Input parameters ----------------------------
      private:
        double m_dbUserMinLevel      =    -5.0;   // dB
        double m_dbUserMaxLevel      =   -15.0;   // dB
        double m_dbDirectSoundTrig   =    20.0;   // dB
        double m_dbProbeMicsDistance =     0.012; // m
        double m_dbSoundSpeed        =   340.0;   // m/s

      protected:
        struct Flags
        {
            bool noiseCorrectionEnabled = true;
            bool edtEnabled             = false;
            bool stageEnabled           = false;
            bool averageEnabled         = false;
        } m_flags;
        
        int m_nSource         = Source::Mono;
        int m_nIaccSpan       = IACCSpan::All;
        int m_octaveFraction = 1; // default is 1/1 octave bands

        double m_dbSoundSourceDistance = 10.0; // cm

        //------------------------- Tracks data --------------------------------
        TArrayOfAcParametersAudioTracks m_aAudioTracks;

        //------------------------- Parameters attributes -------------------------      
        Aurora::Spectrum<double> m_referenceLevels; ///< strength reference levels vector;

      private:
        std::vector<std::unique_ptr<SCOctaveBandAnalysisBase>> m_vOba;

        std::vector<TSchroederDecay> m_decays;
        std::vector<TResults>        m_results;

        /**
         * @brief Convert given time in seconds in samples for track of channel ch.
         */
        Aurora::SampleCount TimeToSamples(const int ch, const double seconds) const;

        /**
         * @brief Compute first arrival time in samples for track of channel ch.
         */
        Aurora::SampleCount FirstArrivalTime(const int ch) const;

        /**
         * @brief Compute length from first arrival time in samples for track of channel ch.
         */
        Aurora::SampleCount LengthFromFAT(const int ch) const;

        /**
         * @brief Returns the shorter First arrival time between left and right channel.
         */
        Aurora::SampleCount GetShorterFAT();

        /**
         * @brief Compute ISO3382 T60 family parameters.
         * @param nCh The track channel.
         * @param dbStart The level in dB at which will start the interpolation
         * to -60 dB level.
         * @param dbDelta ---
         * @return The computed value.
         */                        
        double ReverberationTime(const int nCh,
                                 const double dbStart,
                                 const double dbDelta);


        /**
         * @brief Compute ISO3382 C[width] parameter.
         * @param nCh The track channel.
         * @param width The width in milliseconds of the integration interval.
         * @param delay ---
         * @return The computed value.
         */                        
        double Clarity(const int nCh, const double width, const double delay);

        /**
         * @brief Compute ISO3382 D[width] parameter.
         * @param nCh The track channel.
         * @param width The width in milliseconds of the integration interval.
         * @param delay ---
         * @return The computed value.
         */                        
        double Definition(const int nCh, const double width, const double delay);

        /**
         * @brief Compute ISO3382 Jlf parameter.
         * @param delay ---
         * @return The computed value.
         */                        
        double EarlyLateralFraction(const double delay);

        /**
         * @brief Compute ISO3382 Jlfc parameter.
         * @param delay ---
         * @return The computed value.
         */                
        double EarlyLateralFractionCosine(const double delay);
        
        /**
         * @brief Compute ISO3382 Lj parameter.
         * @param fcb The center band frequency of the filter (see \c Aurora::Spectrum
         * for exception values);
         * @param delay ---
         * @return The computed value.
         */                
        double LateLateralSoundLevel(const float fcb, const double delay);
        
        /**
         * @brief Compute ISO3382 IACC parameters.
         * @param delay ---
         * @param pTauIACC The also computed IACC delay.
         * @param pWidthIACC The also computed IACC width.
         * @return The computed value.
         */        
        double InterAuralCrossCorrelation(const double  delay,
                                          double* pTauIACC,
                                          double* pWidthIACC);

        /**
         * @brief Compute Farina's peakiness parameter.
         * @param nCh The track channel.
         * @return The computed value.
         */
        double Peakiness(const int nCh);
        
        /**
         * @brief Compute Farina's millisecondness parameter.
         * @param nCh The track channel.
         * @param pImpulsivs The also computed impulsiveness parameter.
         * @return The computed value.
         */
        double Millisecondness(const int nCh, double* pImpulsivs);
        
        /**
         * @brief Compute stage parameters (St1, St2, StLate).
         * @param par The parameter identifier.
         * @param nCh The track channel.
         * @param delay ---
         * @return The computed value.
         */
        double Stage(const char* par, const int nCh, const double delay);
        
        /**
         * @brief Compute strength (G) parameter.
         * @param nCh Track channel.
         * @param fcb The center band frequency of the filter (see \c Aurora::Spectrum
         * for exception values);
         * @return The computed value.
         */
        double Strength(const int nCh, const float fcb);
        
        void   PreProcessIntensimetricSignal(AcParametersAudioTrack& left);

        protected:   
        virtual void ShowErrorMessage(const int nErrNo) = 0;

        virtual void PreProcess() = 0;
        virtual void PostProcess() = 0;
        
      public: 
          
        /**
         * @brief The main parameters computation loop.
         */  
        bool CalculateAcousticParameters();
        
        /**
         * @brief Computes "mono" parameters.
         * @param nCh Track channel.
         * @param fcb The center band frequency of the filter (see \c Aurora::Spectrum
         * for exception values);
         * @param delay ---
         */
        void CalculateBandMonoParameters(const int nCh, 
                                         const float fcb, 
                                         const double delay);
        
        /**
         * @brief Computes parameter that needs a spatial capture source, like
         * a binaural microphone or a more complex microphone array.
         * @param fcb The center band frequency of the filter (see \c Aurora::Spectrum
         * for exception values);
         * @param delay ---
         */
        bool CalculateSpatialParameters(const float fcb, 
                                        const double delay);
        
        /** @brief Calculate Band A parameters as the mean of 250 to 2000 Hz
         *  bands parameters.
         *  @param nCh The track channel.
         */
        void CalculateBandAParametersAvg(int nCh);

        //------------------------- Class user interface -------------------------   
        // --- 1st part: user parameters status & setup (pre processing)

        // --- checkers
        bool IsNoiseCorrectionEnabled()  const { return m_flags.noiseCorrectionEnabled; }
        bool IsEDTEnabled()              const { return m_flags.edtEnabled; }
        bool AreStageParametersEnabled() const { return m_flags.stageEnabled; }
        bool IsAverageModeEnabled()      const { return m_flags.averageEnabled; }

        bool IsMono()   const { return (m_nSource == Source::Mono); }
        bool IsStereo() const { return ( (m_nSource > int(Source::Mono)) && 
                                         (m_nSource < int(Source::FourOmniMics)) ); }
        bool IsTetra()  const { return (m_nSource > int(Source::Binaural)); }
        bool IsQuad()   const { return (m_nSource > int(Source::Binaural)); }

        bool IsTwoOmniMics()         const { return (m_nSource == Source::TwoOmniMics); }
        bool IsSoundfieldWY()        const { return (m_nSource == Source::SoundfieldWY); }
        bool IsAPUProbe()            const { return (m_nSource == Source::PuProbe); }
        bool IsAnIntensityProbe()    const { return (m_nSource == Source::IntensityProbe); }
        bool IsABinauralPair()       const { return (m_nSource == Source::Binaural); }
        bool IsFourOmniMics()        const { return (m_nSource == Source::FourOmniMics); }
        bool IsFuMa()      const { return (m_nSource == Source::AmbisonicsFuMa); }
        bool IsAmbix()     const { return (m_nSource == Source::AmbisonicsAmbix); }
                
        // --- getters        
        std::vector<TSchroederDecay>&    Decays () { return m_decays; }
        TArrayOfAcParametersAudioTracks& Tracks () { return m_aAudioTracks; }

        const TResults&       Results(const int nch) const { return m_results[nch]; }
        
        TResults&             Results(const int nch) { return m_results[nch]; }
        TSchroederDecay&        Decay(const int nch) { return m_decays[nch]; }
        AcParametersAudioTrack& Track(const int nch) { return m_aAudioTracks[nch]; }

        int GetChannelsCount() const { return (int)m_results.size(); }

        double GetFullScale() const;
        double GetUserMinLevel()           { return m_dbUserMinLevel; }
        double GetUserMaxLevel()           { return m_dbUserMaxLevel; }
        double GetProbeMicsDistance()      { return m_dbProbeMicsDistance; } // m
        double GetSoundSpeedValue()        { return m_dbSoundSpeed; }
        double GetDirectSoundTrigValue()   { return m_dbDirectSoundTrig; }

        int    GetSource()                 { return m_nSource; }
        int    GetIACCSpan()               { return m_nIaccSpan; }

        double& GetGSoundSourceDistance()  { return m_dbSoundSourceDistance; }
        
        // --- setters
        void SetFullScale(const double dbValue);

        void SetUserMinLevel(const double dbValue) { m_dbUserMinLevel = dbValue; }
        void SetUserMaxLevel(const double dbValue) { m_dbUserMaxLevel = dbValue; }
        
        void EnableNoiseCorrection(const bool en) { m_flags.noiseCorrectionEnabled = en; }
        void EnableEDT            (const bool en) { m_flags.edtEnabled = en; }
        void EnableStageParameters(const bool en) { m_flags.stageEnabled = en; }
        void EnableAverageMode    (const bool en) { m_flags.averageEnabled = en; }
        
        void SetProbeMicsDistance   (const double dist) { m_dbProbeMicsDistance = dist; } // m
        void SetSoundSpeedValue     (const double c)    { m_dbSoundSpeed = c; }
        void SetDirectSoundTrigValue(const double trig);

        void SetSource(const AcousticalParameters::Source src) { m_nSource = src; }
        void SetIACCSpan(const int nValue)   { m_nIaccSpan = nValue; }
        
        void SetGSoundSourceDistance(const double dbDist) { m_dbSoundSourceDistance = dbDist; }
        
        Aurora::Spectrum<double>& GReferenceLevels() { return m_referenceLevels; }
        
        void ReplaceGReferenceValues();

        int    GetOctaveFraction() const { return m_octaveFraction; }
        void   SetOctaveFraction(const int fraction);

    protected:
        wxString GetCaptionString() const override { return wxString("Aurora Acoustical Parameters"); }
        
    public:
        //------------------------- Class user interface -------------------------   
        // --- 2nd part: get acustic parameters values (post processing)

        /**
         * @brief Apply a filter.
         * @param nChnl The chosen track channel.
         * @param fcb The center band frequency of the filter (see \c Aurora::Spectrum
         * for exception values);
         */
        void UpdateFilteredTrack(const int nChnl, const float fcb);

        /**
         * @brief Restore standard levels for G parameter calculation.
         */
        void RestoreDefaultReferenceLevels();

        // Init/Deinitialization method
        bool Init();
        void Destroy();

        // --- Ctors
        AcousticalParameters() { }
        virtual ~AcousticalParameters();
    };


    typedef Aurora::Spectrum<Aurora::AcousticalParameters::TResults::TParameter> AcParametersSpectrum;
    
} // namespace Aurora

#endif // __AURORA_AFACPAR_H__


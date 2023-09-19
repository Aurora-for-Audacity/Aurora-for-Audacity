/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  afthan.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_ITUP56_AFTHAN_H__
#define __AURORA_ITUP56_AFTHAN_H__

#include <array>

#include "ThAnalyzerTrack.h"

namespace Aurora
{
    class TimeHistoryAnalyzer : public ProgressMeterWrapper
    {
    public:
        
        enum Channel { //CH_LEFT, CH_RIGHT, 
                                CH_AVG = 2 };

        enum Sizes 
        {
            N_CHANNELS     = 2,
            N_BANDS        = 12,
            N_CHANNELS_EXT = 3  // 2 + avg channel
        };

        enum TimeConstants
        {
            TC_PEAK = 0, // 1 sample (local peak)
            TC_RMS,      // 1 ms  (TC_INST)
            TC_ITU,      // 30 ms
            TC_IMP,      // 35 ms up, 1.5 s down
            TC_FAST,     // 125 ms
            TC_SLOW,     // 1 s
            N_TIME_CONST
        };

        enum Parameters 
        {
            THA_FSL,   // Full Scale level
            THA_AVG,   // Average level
            THA_SEL,   // Single Event level
            THA_TDU,   // Total Duration
            THA_ASL,   // Active Speech level
            THA_THS,   // Threshold level
            THA_ACT,   // Activity factor
            THA_MPK,   // Max Peak level
            THA_MIM,   // MaxSPL Impulse
            THA_MFS,   // MaxSPL Fast
            THA_MSL,   // MaxSPL Slow
            THA_DUR,   // Duration of the impulsive event
            THA_IMP,   // Impulsive event??
            N_PARAMETERS
        };

        struct TResults
        {
            std::array<double, N_PARAMETERS>               m_aParameters;
            std::array< std::vector<double>, N_TIME_CONST> m_aBuffers;

            bool    IsImpulsive() const { return (m_aParameters[THA_IMP] == 1.0) ? true : false; }

            double  GetParameter(const int nIdx) const { return m_aParameters[nIdx]; }
            std::vector<double>& GetBuffer(const int nIdx) { return m_aBuffers[nIdx]; }
            
            const std::vector<double>& GetBuffer(const int nIdx) const { return m_aBuffers[nIdx]; }

            size_t  GetBufferLength()  const { return m_aBuffers[0].size(); }            

            void ResizeBuffers(const size_t newLength);

            TResults& operator=(const TResults& r);
            
            TResults() { }
            TResults(const TResults& r);
            TResults(const size_t buffersLength);
        };

    private:

        struct TITUFactors
        {
            double m_dbG          = 0;
            double m_dbFast       = 0;
            double m_dbSlow       = 0;
            double m_dbImpRaising = 0;
            double m_dbImpFalling = 0;

            void SetAsAfactors(const double ts, const double rate);
            void SetAsBfactors(const double ts, const double rate);

            TITUFactors() { }
        };

        struct TITULevels
        {
            double runFast = 0.0;
            double runSlow = 0.0;
            double runImp  = 0.0;
            double run35ms = 0.0;
            double maxFast = 0.0;
            double maxSlow = 0.0;
            double maxImp  = 0.0;
            double max1ms  = 0.0;
        };
        
    protected:
        bool  m_bRemoveDC   = false;

        TArrayOfThAnalyzerAudioTracks  m_aSignalTracks;
        std::vector<TResults> m_aResults;

        //--- private methods
        double Average  (const int nParam);
        double dBAverage(const int nParam);
        void   FillAvgParameters();

        // --- manage multi gauge progress meter
        wxString GetCaptionString() const override { return wxString("Aurora ITU P56"); }

    public:
        
        /// Processing method
        bool Analyze();
        bool AnalyzeTrack(const int nCh);

        // --- Get/Setters
        int GetChannelsCount()  const { return (int)m_aSignalTracks.size(); }
        void SetChannelsNumber(const int channels);

        bool IsRemoveDCEnabled() { return m_bRemoveDC;  }
        void EnableRemoveDC(const bool onOff) { m_bRemoveDC = onOff; }
        
        ThAnalyzerAudioTrack& GetSignalTrack(const int idx) { return  m_aSignalTracks[idx]; }
        TArrayOfThAnalyzerAudioTracks& GetSignalTracks() { return  m_aSignalTracks; }
        std::vector<TResults>& GetResults() { return m_aResults; }

    private:
        void Init();

    public:
        // --- ctors
        TimeHistoryAnalyzer() { }
        virtual ~TimeHistoryAnalyzer() { }

    };
} // namespace Aurora

#endif // __AURORA_ITUP56_AFTHAN_H__

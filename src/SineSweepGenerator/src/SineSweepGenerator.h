/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Generator

  sswgen.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_SSWGEN_H__
#define __AURORA_SSWGEN_H__

#include <commdefs.h>

namespace Aurora
{
    class SineSweepGenerator
    {
       public:
          enum Channels   
          { 
              //CH_LEFT, CH_RIGHT,             
              CH_INV_FILT = 2, 
              CH_PULSES, 
              _N_TRACKS_ 
          };

          enum SweepTypes 
          { 
              ST_LINEAR, 
              ST_LOG, 
              ST_PINK 
          };

          enum FadeTypes  
          { 
              FT_RECT, 
              FT_HAMMING, 
              FT_HANN, 
              FT_QUARTER_SINE, 
              FT_LINEAR, 
              FT_GAUSSIAN, 
              FT_BLACKMANN, 
              FT_BLACK_HARRIS, 
              FT_LOG 
          };

          enum ErrorCodes 
          { 
              ERR_OK = 0,
        	    ERR_SWEEP_LIMITS = 5000, 
              ERR_DURATION, 
              ERR_AMPLITUDE, 
              ERR_CHANNELS_N,
        	    ERR_FADE_IN,             
              ERR_FADE_OUT, 
              ERR_SILENCE,   
              ERR_CYCLES_N 
          };
           
       private:

          int    m_nErrNo = ERR_OK;

          double m_dbRate            = 48000.0; // Hz
          double m_dbHighFrq         = 22000.0; // Hz
          double m_dbLowFrq          = 22.0;    // Hz
          double m_dbSweepDuration   = 10.0; // s 
          double m_dbAmplitude       =  1.0;
          double m_dbSilenceDuration =  8.0; // s
          int    m_nCycles           = 1;
          double m_dbFadeInDuration  = 0.1; // s
          double m_dbFadeOutDuration = 0.1; // s
          int    m_nFadeInType      = FT_HANN;
          int    m_nFadeOutType     = FT_HANN;
          double m_dbDeltaL         =  0.0;
          int    m_nChnlsCount      =  2; // Sweep Mono + Inverse filter
          int    m_nSweepChnlsCount =  1; // default mono
          int    m_nFilterChnlIdx   =  1; // sweep on channel 0, filter on channel 1
          int    m_nPulsesChnlIdx   = -1; // by default no control pulses
          
          std::vector<TSampleVector> m_buffers;
          
          SampleCount m_buffersLength = 0; // samples
          SampleCount m_sweepLength   = 0; // samples
          SampleCount m_filterLength  = 0; // samples
          
          int m_nSweepType = ST_LOG;
         
          bool m_bControlPulses = false;
          
          //--- private methods
          void InitDataBuffers();
          void DeleteDataBuffers();
          
       public:
         // Check for input errors
         int ErrorCheck();
          
         /// Processing method
         bool Generate();

         /// Fill data block (module interface)
         void FillBlock(Sample* pData, 
                        SampleCount len, 
                        SampleCount written, 
                        int nTrack);
         
        // --- Checkers
        bool IsControlPulsesSet()    const { return m_bControlPulses; }

        // --- Get/Setters
        int   GetErrorCode()         const { return m_nErrNo; }
        void  GetErrorMessage(wxString& wxszMsg);

        double GetSamplerate()        const { return m_dbRate; }
        double GetStartFrequency()    const { return m_dbLowFrq; }
        double GetEndFrequency()      const { return m_dbHighFrq; }
        double GetHighFrequency()     const { return m_dbHighFrq; }
        double GetLowFrequency()      const { return m_dbLowFrq; }
        double GetSweepDuration()     const { return m_dbSweepDuration; }
        double GetSilenceDuration()   const { return m_dbSilenceDuration; }
        double GetAmplitude()         const { return m_dbAmplitude; }
        int    GetCyclesNumber()      const { return m_nCycles; }
        int    GetRepetitionsNumber() const { return m_nCycles; }
        int    GetSweepType()         const { return m_nSweepType; }
        int    GetNeededTracks()      const { return m_nChnlsCount; }
        double GetFadeInDuration()    const { return m_dbFadeInDuration; }
        double GetFadeOutDuration()   const { return m_dbFadeOutDuration; }
        int    GetFadeInType()        const { return m_nFadeInType; }
        int    GetFadeOutType()       const { return m_nFadeOutType; }
        double GetDeltaL()            const { return m_dbDeltaL; }
        int    GetChannelsNumber()    const { return m_nChnlsCount; }
        int    GetSweepChnlsNumber()  const { return m_nSweepChnlsCount; }
        int    GetFilterChannel()     const { return m_nFilterChnlIdx; }
        int    GetPulsesChannel()     const { return m_nPulsesChnlIdx; }
        double GetTotalDuration()     const { return m_buffersLength/m_dbRate; }
        
        const TSampleVector&  GetBuffer(int nCh) const { return m_buffers[nCh]; }
        const TSampleVector&  GetFilter(int nCh) const { return m_buffers[m_nFilterChnlIdx]; }

        SampleCount GetBuffersLength() const { return m_buffersLength; }        
        SampleCount GetFilterLength()  const { return m_filterLength; }
        
        void SetSamplerate     (double dbValue)  { m_dbRate = dbValue; }
        void SetStartFrequency (double dbValue)  { m_dbLowFrq = dbValue; }
        void SetEndFrequency   (double dbValue)  { m_dbHighFrq = dbValue; }
        void SetHighFrequency  (double dbValue)  { m_dbHighFrq = dbValue; }
        void SetLowFrequency   (double dbValue)  { m_dbLowFrq = dbValue; }
        void SetSweepDuration  (double dbValue)  { m_dbSweepDuration   = (dbValue < 100) ? dbValue : dbValue*m_dbRate; }
        void SetSilenceDuration(double dbValue)  { m_dbSilenceDuration = (dbValue < 100) ? dbValue : dbValue*m_dbRate; }
        void SetAmplitude      (double dbValue)  { m_dbAmplitude = dbValue; }
        void SetCyclesNumber   (int nValue)      { m_nCycles = nValue; }
        void SetRepetitionsNumber(int nValue)    { m_nCycles = nValue; }
        void SetSweepType        (int nValue)    { m_nSweepType = nValue; }
        void SetControlPulses   (bool bValue);
        void SetFadeInDuration  (double dbValue) { m_dbFadeInDuration  = (dbValue < 100) ? dbValue : dbValue*m_dbRate; }
        void SetFadeOutDuration (double dbValue) { m_dbFadeOutDuration = (dbValue < 100) ? dbValue : dbValue*m_dbRate; }
        void SetFadeInType      (int nValue)     { m_nFadeInType = nValue; }
        void SetFadeOutType     (int nValue)     { m_nFadeOutType = nValue; }
        void SetDeltaL          (double dbValue) { m_dbDeltaL = dbValue; }
        void SetChannelsNumber  (int nValue)     { m_nChnlsCount = nValue; }
        void SetSweepChnlsNumber(int nValue);
        
        void SetBuffersLength(SampleCount len, 
                              const bool bInitBuffer = false, 
                              const int nCh = 0);    
        
        void Init() { }
        void Destroy();
        
         // --- ctors
        SineSweepGenerator() { }
         ~SineSweepGenerator();

    };
} // namespace Aurora

#endif // __AURORA_SSWGEN_H__

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions
  
  correlator.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_XFUNCTIONS_CORRELATOR_H__
#define __AURORA_XFUNCTIONS_CORRELATOR_H__

// ---------------------------------------------
// a correlation calculation library
// ---------------------------------------------

#include <array>

#include <kiss_fft.h>
#include <tools/kiss_fftr.h>

namespace Aurora
{
    enum XFunctionType
    {
        kAutoCorrelation,
        kCrossCorrelation,
        kWhiteCorrelation,
        kCrossPower,
        kH1,
        kH2,
        kH3,
        kAlphaPU,
        kAlphaPP,
        kPUCalibrationSwTube,
        kPUCalibrationFreeField,

        kCoherence,
        kDiracDelta // not really an xfunction ...
    };

    enum XFunctionsPlotType
    {
        AutoCorrelationLeft,
        AutoCorrelationRight,
        Magnitude,
        Phase,
        Real,
        Imaginary,
        Alpha,
        Coherence,

        AdHoc = 0xFFFF
    };


    
    /**
     * @brief The correlation computer.
     */
    class Correlator : public ProgressMeterWrapper
    {
    public:

        /**
         * @brief Holds the computed data to be exported in the workbench.
         */
        class OutputTrack : public SamplesVector
        {
            wxString m_label;
            
        public:
            const wxString& GetLabel () const { return m_label; }
            void SetLabel(const wxString& label) { m_label = label; }
            
            OutputTrack() { }
            OutputTrack(const wxString& label, Aurora::SamplesVector& track)
            : m_label(label)
            {}            
        };

        /**
         * @brief Basically this encapsulates a vector and its rapresentation
         * in the frequency domain
         */
        struct TVector
        {
            SamplesVector   t;
            ComplexSpectrum f;

            void Resize(Aurora::SampleCount tSize,
                        Aurora::SampleCount fSize = 0);

            void FFT();
            void IFFT(const float rescale = true);

            TVector()  { }
            ~TVector() { Dealloc();  }

        private:
            void Dealloc();

            kiss_fftr_cfg fwd = nullptr;
            kiss_fftr_cfg rwd = nullptr;
        };
        
        enum IntensimetricProbeType
        {
            kPU,
            kPP
        };

        struct TOptions
        {
            // Note: in the original code, the UI flag "Normalize"
            //       corresponds to RemoveDC code option
            bool normalize               = false;
            bool shiftToHalfWindow       = false;
            bool coherenceWeighting      = false;
            bool squaredHilbertTransform = false;
            bool timeReversal            = false;
            bool diracPulse              = false;           
        } m_options;

    protected:
        int m_nXFnTypeId      = Aurora::XFunctionType::kCrossCorrelation;
        int mChannelsToExport = 1; // in the original code, they are always 2

        Aurora::Sample m_samplerate = 44100;

        Aurora::SampleCount  m_fftLength = 2048; // Is the fft logical length

        Aurora::SampleCount m_lowerBound = 0;
        Aurora::SampleCount m_higherBound = 0;

        // I/0 vectors/structs
        std::array<SamplesVector, 2> m_inputs; //Aurora::Sample    *m_aasmpIn[2];
        std::array<OutputTrack, 4> m_outputs; // can be more than four..

        // RMS data vector
        struct TRMS
        {
            double left = 0.0;
            double right = 0.0;
            double cross = 0.0;
        } m_RMS;

        struct TMax
        {
            Aurora::Sample left      = 0.0;
            Aurora::Sample right     = 0.0;
            Aurora::Sample xcorr     = 0.0;

            Aurora::SampleCount index = 0; // CrossCorrelation peak index
        } m_Gmax;

        Aurora::Sample m_GlrMax   =  0.0; // MaxCC
        double   m_dbDelay        =  0.0;  // flytime??
        double   m_dbTriggerLevel = 50.0;
        double   m_dbTriggerTime  =  0.0;

        // shaping window vector
        SamplesVector  m_window; // *m_window;

        // blocks vectors
        TVector m_left;
        TVector m_right;
        TVector m_Gll;
        TVector m_Grr;
        TVector m_Glr;

        TVector m_C;     // Coherence        
        TVector m_Hx;    // Hx filter       
        TVector m_delta; // Dirac's delta spectrum
        TVector m_alpha; // Alpha / PU calibration
        TVector m_Wlr;   //   
        TVector m_hilbertLeft;  // Hilbert transform vectors
        TVector m_hilbertRight;

        // various
        int m_nWindowType = WT_HANNING; // TODO: use correct type!

        //Following filter variables
        bool   m_bIsFollowingFilterOn = false;
        int    m_nFollowChn = 0;
        double m_dbFollowBW = 0.3333333;

        // PUProbe variables
        double m_dbSoundSpeed   =  343.0;
        double m_dbMicsDistance =   40.0;
        double m_dbFmax         = 1900.0;

        // processing flag
        bool m_bIsProcessed = false;

        // --- init methods
        void InitWindow(const Aurora::SampleCount size);

        // --- data serie handling methods
    public:
        double Rll  (const int f0, const int f1);
        double Rrr  (const int f0, const int f1);
        double Alpha(const int f0, const int f1);

    protected:
        // -- vector ops methods
        double Delay (const Aurora::SampleCount max_idx);
        double Trigger(const Aurora::SamplesVector& x);

        // --- transformation & filtering methods    
        void HilbertTransform(const ComplexSpectrum& in, ComplexSpectrum& out);
        void HFilter (const ComplexSpectrum& n, const ComplexSpectrum& d);
        void H3Filter(const ComplexSpectrum& n, const ComplexSpectrum& d);

        // --- signal generation methods
        void Dirac();

        // --- alpha & PU probe calibration methods
        void Alpha(const IntensimetricProbeType pt);
        void PUCalibrationSWTube();    // -> CalibPU
        void PUCalibrationFreeField(); // -> CalibPU2

        /// --- correlation methods
        void AutoSpectrum(ComplexSpectrum& out,
                          const ComplexSpectrum& in,
                          Aurora::SampleCount current_block);

        void CrossSpectrum(Aurora::SampleCount current_block);
        void Coherence();    // -> COHFilter
        void WhitenedCrossSpectrumPhase();  // -> COHFilter2 
        void CoherenceWeighting(const ComplexSpectrum& weights);
        void CrossPowerSpectrumPhase(Aurora::SampleCount current_block);

        // --- processing methods    
        bool DoCrossCorrelation();  // -> ReadAndAverageData;

        void PostProcessAutocorrelation();
        void PostProcessCrossFunction(TVector& data);

    public:
        bool Process();

    public:
        // --- checkers, get-setters
        TOptions& GetOptions() { return m_options;  }
        const TOptions& GetOptions() const { return m_options; }

        int      GetXFunctionType()             const { return m_nXFnTypeId; }
        Aurora::Sample GetSamplerate()          const { return m_samplerate; }
        int      GetChannelsToExport()          const { return mChannelsToExport; }
        int      GetWindowType()                const { return m_nWindowType; }
        Aurora::SampleCount GetFFTLength()      const { return m_fftLength; }
        Aurora::SampleCount GetSpectrumLength() const { return m_left.f.Length(); }
        Aurora::SampleCount GetBlocksNumber()   const;

        const TRMS& GetRMS() const { return m_RMS; }

        double  GetXCorrelationMax()      const { return m_GlrMax; }
        double  GetXCorrelationMaxDelay() const { return m_dbDelay; }

        double GetTriggerLevel() const { return m_dbTriggerLevel; }
        double GetTriggerTime()  const { return m_dbTriggerTime; }

        // following filter
        bool   IsFollowingFilterEnabled() const { return m_bIsFollowingFilterOn; }
        int    GetFollowingChannel()      const { return m_nFollowChn; }
        double GetFollowingBandwidth()    const { return m_dbFollowBW; }

        // PUProbe 
        bool   IsProbeFreeField()       const { return (m_nXFnTypeId == Aurora::XFunctionType::kPUCalibrationFreeField); }
        bool   IsProbeRigidTerminated() const { return (m_nXFnTypeId == Aurora::XFunctionType::kPUCalibrationSwTube); }
        double GetSoundSpeed()    const { return m_dbSoundSpeed; }
        double GetProbeDistance() const { return m_dbMicsDistance; }
        double GetProbeMaxFreq()  const { return m_dbFmax; }

        // Input 
        Aurora::SampleCount GetInputVectorsLength() const { return m_inputs[0].Length(); }
        Aurora::SamplesVector& GetInputVector(const int chn) { return m_inputs[chn]; }

        // Results
        const Aurora::Correlator::OutputTrack& GetResultsVector(const int chn) const
        {
            return m_outputs[chn];
        }

        const ComplexSpectrum& Gll() const { return m_Gll.f; }
        const ComplexSpectrum& Grr() const { return m_Grr.f; }
        const ComplexSpectrum& Glr() const { return m_Glr.f; }
        const ComplexSpectrum& C()   const { return m_C.f;  }
        
        ComplexSpectrum& GetAutocorrelationSpectrum(const int ch);
        ComplexSpectrum& GetCrosscorrelationSpectrum() { return m_Glr.f; }
        ComplexSpectrum& GetCoherenceSpectrum()        { return m_C.f; }
        ComplexSpectrum& GetCrossPowerSpectrum()       { return m_Wlr.f; }
        ComplexSpectrum& GetH1FunctionSpectrum()       { return m_Hx.f; }
        ComplexSpectrum& GetH2FunctionSpectrum()       { return m_Hx.f; }
        ComplexSpectrum& GetH3FunctionSpectrum()       { return m_Hx.f; }
        ComplexSpectrum& GetDiracPulseSpectrum()       { return m_delta.f; }
        ComplexSpectrum& GetAlphaSpectrum()                     { return m_alpha.f; }
        ComplexSpectrum& GetProbeFreeFieldCalibrationSpectrum() { return m_alpha.f; }
        ComplexSpectrum& GetProbeSWTubeCalibrationSpectrum()    { return m_alpha.f; }
        ComplexSpectrum& GetHilbertTransform(const int ch);
        ComplexSpectrum& GetFunctionSpectrum(const int xfn, const int ch = 0);
        ComplexSpectrum& GetFunctionSpectrum(const int nCh = 0) { return GetFunctionSpectrum(m_nXFnTypeId, nCh); }

        // - setters
        void ResizeInput(const Aurora::SampleCount len, const int ch);        
        void ResizeInputs(const Aurora::SampleCount len);
        
        void SetXFunctionType(const int type)              { m_nXFnTypeId = type; }
        void SetSamplerate(const Aurora::Sample value)     { m_samplerate = value; }
        void SetWindowType(const int type)                 { m_nWindowType = type; }
        void SetFFTLength(const Aurora::SampleCount value);
        
        void SetTriggerLevel(const double value) { m_dbTriggerLevel = value; }
        void SetTriggerTime(const double value)  { m_dbTriggerTime = value; }
        
        // following filter
        void SetBounds();
        void SetFollowingFilter(const bool value)  { m_bIsFollowingFilterOn = value; }
        void SetFollowingChannel(const int value);
        void SetFollowingBandwidth(const double value) { m_dbFollowBW = value; }
                
        void SetSoundSpeed(const double value)         { m_dbSoundSpeed = value; }
        void SetProbeDistance(const double value)      { m_dbMicsDistance = value; }
        void SetProbeMaxFreq(const double value)       { m_dbFmax = value; }
        
    protected:
        wxString GetCaptionString() const override { return wxString("Aurora XFunctions"); }

    public:
        // ---'ctors 
        Correlator();
        ~Correlator() { }
    };
}

#endif //__AURORA_XFUNCTIONS_CORRELATOR_H__


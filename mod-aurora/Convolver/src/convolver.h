/****************************************************************
 *                                                              *
 *                       Convolver class                        *
 *                                                              *
 * This class is a partitioned convolution engine: it simply    *
 * takes as input a vector and a filter, then evaluates the     *
 * convolution product between them.                            *
 * It is not very optimized, but it's simple and works quite    *
 * well.                                                        *
 *                                                              *
 * ------------------------------------------------------------ *
 * Usage:                                                       *
 *                                                              *
 * 1) Instantiate the class using one of the constructors       *
 *    provided;                                                 *
 * 2) if not done by the constructor, set the filter and the    *
 *    vector using 'SetFilter' and 'SetVector' methods;         *
 * 3) call the 'Process' method to start calculations;          *
 * 4) get results using 'GetResultSize' and 'GetResultVector'   *
 *    methods.                                                  *
 * 5) At this point you can load another filter or another      *
 *    vector using 'SetFilter' and/or 'SetVector' and recall    *
 *    'Process'.                                                *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * by Simone Campanini 2009                                     *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * convolver.h                                                  *
 *                                                              *
 ***************************************************************/

/*
  This file contains the convolution class.

 */
#ifndef __AURORA_CONVOLVER_H__
#define __AURORA_CONVOLVER_H__

#define USE_KISS_FFT 1

#include <tools/kiss_fftr.h>

#include <string.h>

/*
  PRESERVE LENGTH
  ---------------
  Si identifica il "Picco centrale" della rfisposta all'impulso. Esso
  deve corrispondere allo "zero time" della convoluzione. Quindi, se la
  IR era lunga N campioni, essa tendenzialmente "ritarda" di N-1
  campioni il segnale con essa convoluto. Ma, se devo rispettare lo
  "zero time" al campione P, vuol dire che, una volta fatta la
  convoluzione, buttero' via i primi P-1 campioni all'inizio del segnale
  convoluto, e gli ultimi N-P campini alla fine. In questo modo, il
  segnale convoluto, anziche' esere lungo M+N-1 campioni, sara' lungo M
  campioni, come il segnale originale.
  
*/

namespace Aurora
{
    class ConvolverBase : public Aurora::ProgressMeterWrapper
    {
        SampleCount m_N = 0;

        SampleCount m_fftLength    = 0;
        SampleCount m_inputLength  = 0;
        SampleCount m_filterLength = 0;
                
        SamplesVector m_input; //vectors to be convolved
        SamplesVector m_output; //results vector

        std::shared_ptr<Sample> m_filter; 
        
        SamplesVector m_xt;
        SamplesVector m_ht;
        SamplesVector m_yt;
        
        ComplexVector m_Xf;
        ComplexVector m_Hf;
        ComplexVector m_Yf;
        
        kiss_fftr_cfg m_fftForwardCfg  = nullptr;
        kiss_fftr_cfg m_fftBackwardCfg = nullptr;
        
    protected:
        
        struct Options
        {
            bool preserveInputWhenLoadingFilter = false;
            bool preserveOutputLength           = false;
            bool reverseFilter                  = false;
            bool autorange                      = false;
            bool firstBlockAutorange            = false;
            bool removeDC                       = true;
            bool applyGain                      = false;
            
        } m_options;
        
        Sample m_gain = -6.0; // dB
        Sample m_fbMax = 0.0;
        SampleCount m_peakPosition = 0;
    
        SampleCount BlockSize() const;
        Sample      FindMax(Sample *v, SampleCount len) const;

#if USE_INPUT_RESIZE        
        void ResizeInputAndOutput();
#endif
        void LoadBlock(const SampleCount from, const SampleCount to);
        void StoreResultBlock(const SampleCount from);

        void BlockConvolve();

        void FFT (const SamplesVector&  in, ComplexVector& out);
        void IFFT(const ComplexVector&  in, SamplesVector& out);

        // ***** Set Input data
    public:

        void SetFilter(TSampleVector& f);
        void SetVector(TSampleVector& v);

        // ***** Pre-processing methods
        virtual void PreProcess();
        
    protected:    
        virtual void ReverseFilter();
        virtual void FindFilterPeakPosition(); 
        virtual void PreserveOutputLength();
        
    public:
        SampleCount FindPeakPosition(const Sample* filter, 
                                     const SampleCount filtarLength);
        void PreserveLength(Sample* data, 
                            const SampleCount dataLength,
                            const SampleCount filterLength, 
                            const SampleCount peakPostion);
        
        SampleCount FindPeakPosition(const SamplesVector& filter);
        SampleCount PreserveLength(SamplesVector& data,
                                   const SampleCount filterLength,
                                   const SampleCount peakPosition);
    public:
        // *** THIS should be called for calculations...
        virtual bool Process(); 
        
        // ***** This methods does the convolution calculations
        bool Convolve();

        // ***** Post-processing methods
        virtual void PostProcess();
        
    protected:
        virtual void Gain();
        virtual void Autorange(); // these operate on output vector.
        virtual void RemoveDC();
        
    public:
        void Gain     (SamplesVector& v);
        void Autorange(SamplesVector& v);
        void RemoveDC (SamplesVector& v);
    
        // ***** Methods to get results
        const SamplesVector& GetResultVector() const { return m_output; }

        // ***** General purpose get/setters, boolcheck
        Options& GetOptions() { return m_options; }
        const Options& GetOptions() const { return m_options; }
        
        Sample GetGainValue()     const { return m_gain; }  // dB
        SampleCount GetFFTSize()  const { return m_N; }  // samples
        Sample GetFirstBlockMax() const { return m_fbMax; }
        
        void SetGainValue(Sample value) { m_gain  = value; } // dB
        
    protected:
        virtual void InitProgressMeter(const int total);
        virtual bool UpdateProgressMeter(const int step);
        virtual void DestroyProgressMeter();

    public:
        // ***** ctors    
        ConvolverBase() { }
        ConvolverBase(SamplesVector& input,
                      SamplesVector& filter,
                      const int opts = 0);

        ~ConvolverBase();

    }; 
} // namespace Aurora

#endif // __AURORA_CONVOLVER_H__

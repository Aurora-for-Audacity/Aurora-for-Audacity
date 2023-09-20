/****************************************************************
 *                                                              *
 *                       Aurora::ConvolverBase class                        *
 *                                                              *
 * This class is a partitioned convolution engine: it simply    *
 * takes as input a vector and a filter, then evaluates the     *
 * convolution product between them.                            *
 * It is not very optimized, but it's simply and works quite    *
 * fine.                                                        *
 * ------------------------------------------------------------ *
 *                                                              *
 * by Simone Campanini 2009-20                                  *
 *                                                              *
 * ------------------------------------------------------------ *
 *                                                              *
 * convolver.cpp                                                *
 *                                                              *
 ***************************************************************/

/*
  This file contains the convolution class code.

 */
#include <aurora.h>

#include "convolver.h"

void Aurora::ConvolverBase::SetFilter(Aurora::SamplesVector& f)
{
#if USE_INPUT_RESIZE    
    // Save old m_filterLength value
    Aurora::SampleCount oldFilterLength = m_filterLength;
#endif
    
    m_filterLength = f.Length();
    
    if( ! m_Hf.IsNull())
    {        
        kiss_fftr_free(m_fftForwardCfg);
        kiss_fftr_free(m_fftBackwardCfg);
    }        
    
    //Search for the optimal size.
    m_N = BlockSize(); // chosen so overlap = N - filterLength
    m_fftLength = m_N / 2 + 1;  // according to kiss fft documentation

    m_xt.Resize(m_N);
    m_ht.Resize(m_N);
    m_yt.Resize(m_N);
    
    m_Xf.Resize(m_fftLength);
    m_Hf.Resize(m_fftLength);
    m_Yf.Resize(m_fftLength);
    
    m_fftForwardCfg  = kiss_fftr_alloc((int)m_N, 0, NULL, NULL);
    m_fftBackwardCfg = kiss_fftr_alloc((int)m_N, 1, NULL, NULL);

#if USE_INPUT_RESIZE
    //Adjust vector head in the case you want to process the input data
    // with a new filter.
    
    if( m_bPreserveInputWhenLoadingFilter && (oldFilterLength != m_filterLength) )
    {
        ResizeInputAndOutput();
    }
#else
    if (! m_input.IsNull())
    {
        m_output.Resize(m_filterLength + m_input.Length());
    }
#endif
    //initialize filter vector
    //LoadFilter();
    m_ht.Clear(); // TODO, maybe ht can be local!
    m_ht.Copy(f);

    FFT(m_ht, m_Hf);
}

void Aurora::ConvolverBase::SetVector(Aurora::SamplesVector& v)
{
#if USE_INPUT_RESIZE    
    assert(m_filterLength > 0);

    m_inputLength = v.Length();
    m_input.Resize(m_filterLength + v.Length()); 
    m_output.Resize(m_input.Length());

    m_input.Clear();
    m_input.Copy(v, 0, m_filterLength); // copy input data with zeroed offset of 
                                        // m_filterLength samples.
#else
    m_input.Resize(v.Length());
    m_input.Copy(v);

    if (m_filterLength > 0)
    {
        m_output.Resize(m_filterLength + m_input.Length());
    }
#endif    
}

#if USE_INPUT_RESIZE
void Aurora::ConvolverBase::ResizeInputAndOutput()
{
    assert (m_inputLength > 0 && m_input.Length() > 0);

    auto oldFilterLength = m_input.Length() - m_inputLength;
    auto newLength = m_inputLength + m_filterLength;

    Aurora::SamplesVector resized(newLength);
#if 1
    // copy from input vector jumping over the zeros...
    resized.Copy(m_input, oldFilterLength, m_filterLength);
    m_input = resized; // using move semantic
    m_output.Resize(m_input.Length());
    
    
#else
    //TBR
    Aurora::Sample *p = new Aurora::Sample[size];
    
    memset(p, 0, m_filterLength * sizeof(Aurora::Sample));
    memcpy(p + m_filterLength, m_input + oldHLength, (m_xLength - oldHLength) * sizeof(Aurora::Sample));
    
    delete [] m_input;

    m_input = p;
    m_xLength += m_filterLength - oldHLength;

    // Resizing x mean resizing y too
    m_yLength = m_xLength;
    delete [] m_output;
    m_output = new Aurora::Sample[m_yLength];
#endif
}
#endif

Aurora::SampleCount Aurora::ConvolverBase::BlockSize() const
{
    // Block size must be a power of 2
    Aurora::SampleCount l = 2;
    while( l < m_filterLength ) l *= 2;
    return l *= 2;
}

Aurora::Sample Aurora::ConvolverBase::FindMax(Aurora::Sample * v, 
                                              Aurora::SampleCount len) const
{
    Aurora::SampleCount i = 0;
    Aurora::Sample max = 0;
    
    for(i=0; i < len; i++)
    {
        max = (std::fabs(v[i]) > max) ? std::fabs(v[i]) : max;
    }
    return max;
}

void Aurora::ConvolverBase::ReverseFilter()
{
    m_ht.Reverse();
    FFT(m_ht, m_Hf);
}

Aurora::SampleCount Aurora::ConvolverBase::FindPeakPosition(const Aurora::SamplesVector& filter)
{
    return FindPeakPosition(filter.GetData(), filter.Length());
}

Aurora::SampleCount Aurora::ConvolverBase::FindPeakPosition(const Aurora::Sample* pH,
                                                            const Aurora::SampleCount hLength)
{
    // Finds the main peak position of the IR (filter),
    // needed by the preserve length function.
    m_peakPosition = 0;
    Aurora::Sample tmp = 0.0;

    for(Aurora::SampleCount i = 0; i < hLength; i++)
    {
        if(std::fabs(pH[i]) > tmp)
        {
            tmp = fabs(pH[i]);
            m_peakPosition = i;
        }
    }
    return m_peakPosition;
}

void Aurora::ConvolverBase::FindFilterPeakPosition()
{
    FindPeakPosition(m_ht);
}

void Aurora::ConvolverBase::LoadBlock(const Aurora::SampleCount from, 
                                      const Aurora::SampleCount to)
{
#if USE_INPUT_RESIZE
        Aurora::SampleCount chunkSize = to - from;
        memcpy(m_xt, m_input + from, chunkSize * sizeof(Aurora::Sample));
        
        //Zeropad if the case...
        if(chunkSize < m_N)
        {
            memset(m_xt + chunkSize, 0, (m_N - chunkSize) * sizeof(Aurora::Sample));
        }

#else
    // to accomplish overlap-save algorithm the first 'm_filterLength' values
    // of the input vector has to be zeroed and its length is augmented by this
    // amount. Instead of instantiating a new vector beginning with zeros, this
    // will be done algorithmically.
    Aurora::SampleCount srcOffset = 0;
    Aurora::SampleCount dstOffset = 0;
    Aurora::SampleCount chunkSize = 0;

    m_xt.Clear();

    if (from < m_filterLength)
    {
        if (to < m_filterLength)
        {
            chunkSize = 0;
        }
        else
        {
            chunkSize = to - m_filterLength;
            srcOffset = 0;
            dstOffset = m_filterLength - from;
        }
    }
    else
    {
        chunkSize = to - from;
        srcOffset = from - m_filterLength;
    }
    
    if (chunkSize > 0)
    {
        m_xt.Copy(m_input, srcOffset, dstOffset, chunkSize);
        // No zeropad because xt is always cleared
    }
#endif
}

void Aurora::ConvolverBase::StoreResultBlock(const Aurora::SampleCount from)
{
    Aurora::SampleCount to;

    //Prevents overflow...
    if( (from + m_N - m_filterLength) > m_output.Length() ) 
    {
        to = m_output.Length();
    }
    else 
    {
        to = from + m_N - m_filterLength;
    }
    m_output.Copy(m_yt, m_filterLength, from, (to - from));

    // First block autorange: finds max on first block (from = 0)
    if(!from)
    {
        m_fbMax = m_output.FindAbsMax(to);
    }
}

void Aurora::ConvolverBase::PreProcess()
{
    if(m_options.reverseFilter)
    {
       ReverseFilter();
    }
}

void Aurora::ConvolverBase::PostProcess()
{
    if( m_options.autorange || m_options.firstBlockAutorange )
    {
       Autorange();
    }
   
    if(m_options.removeDC)
    {
       RemoveDC();
    }
   
    if(m_options.applyGain)
    {
       Gain();
    }
}

bool Aurora::ConvolverBase::Process()
{
   PreProcess();
   
   if(!Convolve()) 
   {
       return false;
   }
   PostProcess();
   return true;
}

bool Aurora::ConvolverBase::Convolve()
{
    InitProgressMeter((int)m_output.Length());

    // Convolution engine following the overlap & save scheme

    Aurora::SampleCount i  = 0;
    Aurora::SampleCount il = 0;
    Aurora::SampleCount L  = m_N - m_filterLength;    

    while( i < m_output.Length())
    {
        // Overlap & Save loop
        il = ( (i + m_N) < m_output.Length() ) ? i + m_N 
                                               : m_output.Length();
        LoadBlock(i, il); 

        FFT(m_xt, m_Xf);

        BlockConvolve();

        IFFT(m_Yf, m_yt);

        StoreResultBlock(i); // the last 'm_filterLength' samples are good!
            
        i += L;

        if (! UpdateProgressMeter((int)il))
        {
            DestroyProgressMeter();
            return false;
        }
    }
    
    DestroyProgressMeter();
	return true;
}

void Aurora::ConvolverBase::BlockConvolve()
{
    // Convolve a partition of input vectors
    kiss_fft_cpx* xf = m_Xf.Samples();
    kiss_fft_cpx* hf = m_Hf.Samples();
    kiss_fft_cpx* yf = m_Yf.Samples();
    
    for (Aurora::SampleCount i = 0; i < m_fftLength; i++)
    {
        yf[i].r = (xf[i].r * hf[i].r - xf[i].i * hf[i].i); //  real part
        yf[i].i = (xf[i].r * hf[i].i + xf[i].i * hf[i].r); //  imaginary part
    }
}

void Aurora::ConvolverBase::FFT(const Aurora::SamplesVector& in,
                                      Aurora::ComplexVector& out)
{
    kiss_fftr(m_fftForwardCfg, in.CSamples(), out.Samples());
}

void Aurora::ConvolverBase::IFFT(const Aurora::ComplexVector& in,
                                       Aurora::SamplesVector& out)
{
    kiss_fftri(m_fftBackwardCfg, in.CSamples(), out.Samples());
    
    // rescale inverse transform
    const float oneOverN = 1.0f / (float)m_N;
    out *= oneOverN;
}

void Aurora::ConvolverBase::Gain(Aurora::SamplesVector& v)
{
    Aurora::Sample g = std::pow(10.0, m_gain/20.0);
    v *= g;
}

void Aurora::ConvolverBase::Autorange(Aurora::SamplesVector& v)
{
    // Normalize (deafult -6 dB FS) ...no up to 0 dB! 26/3/2021
    Aurora::Sample invMax = (m_options.firstBlockAutorange) ? 1.0 / m_fbMax // (* 2)
                                                            : 1.0 / v.FindAbsMax();
    v *= invMax;
    m_gain = dB20(invMax);
}

void Aurora::ConvolverBase::RemoveDC (Aurora::SamplesVector& v)
{
    // First step: find DC component (vector average)
    Aurora::Sample dc = v.Mean();
    // Second step: remove effectively DC component
    v -= dc;
}

void Aurora::ConvolverBase::PreserveLength(Aurora::Sample* data, 
                                       const Aurora::SampleCount dataLength, 
                                       const Aurora::SampleCount filterLength, 
                                       const Aurora::SampleCount peakPosition)
{
    // Basically, we can translate back the
    // result vector of the same retard
    // introduced by the filtering operation.
    // The position of the IR main peak gives
    // the retard length. We can also discard
    // the last (m_filterLength - m_peakPosition)
    // samples.
    if(peakPosition == 0)
    {
        return;
    }
    
    for(Aurora::SampleCount i=0; i < (dataLength - filterLength); i++)
    {
        data[i] = data[i + peakPosition];
    }
    memset(data + (dataLength - filterLength), 0, filterLength * sizeof(Aurora::Sample));
}

Aurora::SampleCount Aurora::ConvolverBase::PreserveLength(Aurora::SamplesVector& data,
                                                          const Aurora::SampleCount filterLength,
                                                          const Aurora::SampleCount peakPosition)
{
    if (peakPosition == 0)
    {
        return 0;
    }

    data.ShiftLeft(peakPosition);
    data.Resize(data.Length() - (filterLength - peakPosition));
    return data.Length();
}


void Aurora::ConvolverBase::PreserveOutputLength()
{
    PreserveLength(m_output, m_filterLength, m_peakPosition);
}

void Aurora::ConvolverBase::Gain()
{
    Gain(m_output);
}

void Aurora::ConvolverBase::Autorange()
{
    Autorange(m_output);
}

void Aurora::ConvolverBase::RemoveDC()
{
    RemoveDC(m_output);
}

void Aurora::ConvolverBase::InitProgressMeter(const int total)
{
    ProgressMeterWrapper::Show("Aurora Convolver", total, true);
}

bool Aurora::ConvolverBase::UpdateProgressMeter(const int step)
{
    return ProgressMeterWrapper::Update(step);
}

void Aurora::ConvolverBase::DestroyProgressMeter()
{
    ProgressMeterWrapper::Destroy();
}

/*
Aurora::ConvolverBase::ConvolverBase()
{
   memset(m_abOpts, 0, AFC_N_OPTS * sizeof(bool));
}
*/

Aurora::ConvolverBase::ConvolverBase(Aurora::SamplesVector& vector,
                                     Aurora::SamplesVector& filter,
                                     const int opts)
{ 
    //memset(m_abOpts, 0, AFC_N_OPTS * sizeof(bool));
   
    SetVector(vector);
    SetFilter(filter);
}

Aurora::ConvolverBase::~ConvolverBase()
{   
    if (m_fftForwardCfg)
    {
        kiss_fftr_free(m_fftForwardCfg);
    }

    if (m_fftBackwardCfg)
    {
        kiss_fftr_free(m_fftBackwardCfg);
    }
}

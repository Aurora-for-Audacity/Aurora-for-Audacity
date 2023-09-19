/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions
  
  correlator.cpp

  Simone Campanini

**********************************************************************/

#include <aurora.h>

#include "correlator.h"

///////////////////////////////////////////////////
// Correlator class implementation
///////////////////////////////////////////////////

/*
   IPP Perm format of FFT Transform result:
   
   Idx:  0    1      2    3    ... N-2      N-1      [N]
   Perm: R(0) R(N/2) R(1) I(1) ... R(N/2-1) I(N/2-1)
   
   where: R(0) is the DC component (no imag. part)
          R(N/2) is the Nyquist freq. component (no imag. part)
   
*/

// kss_fft real transform produces output complex vector
// that is N/2 + 1 long, DC is located at u[0].r and nyquist
// component at u[N/2].r


#define epsilon   1.0e-12
#define epsilon2 (1.0e-12 * 1.0e-12)

// ------------------------------------------ Initialization & setup methods ---

void Aurora::Correlator::TVector::Resize(Aurora::SampleCount tSize, 
                                         Aurora::SampleCount fSize)
{
    assert (tSize > 0);
    
    if (fSize <= 0)
    {
        fSize = tSize/2 + 1;
    }
    
    t.Resize(tSize, false);
    f.Resize(fSize, false);

    Dealloc();

    fwd = kiss_fftr_alloc((int)tSize, 0, nullptr, nullptr);
    rwd = kiss_fftr_alloc((int)tSize, 1, nullptr, nullptr);
}
            
void Aurora::Correlator::TVector::Dealloc()
{
    if (fwd)
    {
        kiss_fftr_free(fwd);
    }

    if (rwd)
    {
        kiss_fftr_free(rwd);
    }
}

void Aurora::Correlator::TVector::FFT()
{
    kiss_fftr(fwd, t.CSamples(), f.Samples());
}

void Aurora::Correlator::TVector::IFFT(const float rescale)
{
    kiss_fftri(rwd, f.CSamples(), t.Samples());

    if (rescale)
    {
        t /= (float)t.Length();
    }
}

// ----------------------------------------------------------------------------

double Aurora::Correlator::Rll(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return dB(m_Gll.f.SquareModule(f0));
    }    
    double sum = 0.0;
    Aurora::Range range(f0, f1);

    m_Gll.f.ForEach(range, [&](const size_t i, float& real, float& imag) 
    {
        sum += double(real * real + imag * imag); // square module
    });
    return dB(sum/(f1 - f0));
}

double Aurora::Correlator::Rrr(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return dB(m_Grr.f.SquareModule(f0));
    }    
    double sum = 0.0;
    Aurora::Range range(f0, f1);

    m_Grr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        sum += double(real * real + imag * imag); // square module
    });
    return dB(sum/(f1 - f0));
}

double Aurora::Correlator::Alpha(const int f0, const int f1)
{
    if(f0 == f1)
    {
        return 2.0 * std::fabs(m_Glr.f.Real(f0)) 
                     / ( (m_Gll.f.Real(f0) + m_Grr.f.Real(f0)) / 2.0 
                         + std::fabs(m_Glr.f.Real(f0)) );
    }
    double sum = 0.0;

    Aurora::Range range(f0, f1);

    m_Glr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        sum += 2.0 * std::fabs(real)
               / ((m_Gll.f.Real(i) + m_Grr.f.Real(i)) / 2.0 + std::fabs(real));
    });
    return sum/(f1 - f0);
}

void Aurora::Correlator::InitWindow(const Aurora::SampleCount length)
{
    m_window.Resize(length, false);
    m_window.MakeWindow((Aurora::SamplesVector::WindowType)m_nWindowType);    
}

// ------------------------------------------------------------ Math methods ---
/* NOT USED
static void ComplexRatio(kiss_fft_cpx& q, kiss_fft_cpx& n, kiss_fft_cpx& d)
{
    Aurora::Sample den = SqrMod(d) + 1.0e-12;

    Re(q) = (Re(n)*Re(d) + Im(n)*Im(d)) / den;
    Im(q) = (Im(n)*Re(d) - Re(n)*Im(d)) / den;
}
*/
// ------------------------------------------------------- Vector ops methods --
double Aurora::Correlator::Delay(const Aurora::SampleCount maxIndex)
{
    // Delay from the origin calculation
    Aurora::SampleCount timeSpan;

    if( ! m_options.shiftToHalfWindow )
    {
        timeSpan = m_fftLength/2;
    }
    else if( maxIndex < (m_fftLength/2) )
    {
        timeSpan = 0;
    }
    else
    {
        timeSpan = m_fftLength;
    }
    const double delay = double(maxIndex - timeSpan) /  m_samplerate; // in s

    return ( m_options.timeReversal ? -delay : delay);
}

double Aurora::Correlator::Trigger(const Aurora::SamplesVector& samples)
{
    // Search for trigger value
    Aurora::Sample triggerValue = Aurora::Sample(0.01 * m_dbTriggerLevel); // maximum is 1.0
    Aurora::SampleCount triggerPos = 0;
    Aurora::SampleCount i = 0;
    
    while( (i < m_fftLength) && ( triggerPos == 0) )
    {
        if( std::fabs(samples[i]) > triggerValue )  
        {
            triggerPos = i;
        }
        i++;
    }
    return Delay(triggerPos);
}

// --------------------------------------- Transformation & filtering methods --


void Aurora::Correlator::HilbertTransform(const Aurora::ComplexSpectrum& in,
                                                Aurora::ComplexSpectrum& out)
{
    // Hilbert transform: H{w} = Im(w) + j*(-Re(w))
    out.SetDC(0.0);
    out.SetNyquist(0.0);

    out.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        real =  in.Imag(i);
        imag = -in.Real(i);
    });
}


void Aurora::Correlator::HFilter(const Aurora::ComplexSpectrum& n,
                                 const Aurora::ComplexSpectrum& d)
{
    // m_Hx = n / d (rapporto complesso)
    m_Hx.f.SetDC     (n.DC() / (d.DC() + epsilon)); //DC
    m_Hx.f.SetNyquist(n.Ny() / (d.Ny() + epsilon)); //Nyquist

    // complex division...TODO: place it in ComplexSpectrum!

    m_Hx.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        const float den = d.SquareModule(i) + epsilon2;
        real = (n.Real(i) * d.Real(i) + n.Imag(i) * d.Imag(i)) / den;
        imag = (n.Imag(i) * d.Real(i) - n.Real(i) * d.Imag(i)) / den;
    });
}

void Aurora::Correlator::H3Filter(const Aurora::ComplexSpectrum& n,
                                  const Aurora::ComplexSpectrum& d)
{
    // Mette in pfcpxH la radice quadrata del rapporto complesso fra gli spettri 
    // di n e d
    m_Hx.f.SetDC     (std::sqrt(n.DC() / (d.DC() + epsilon)));
    m_Hx.f.SetNyquist(std::sqrt(n.Ny() / (d.Ny() + epsilon)));

    m_Hx.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag) 
    {
        real = std::sqrt(n.Real(i) / (d.Real(i) + epsilon));
        imag = 0.0f;
    });
}

// ------------------------------------------------ Signal generation methods --

void Aurora::Correlator::Dirac()
{
    // Mette in Out una delta di Dirac finestrata
    double f = 0;

    m_delta.f.SetDC(1.0);
    m_delta.f.SetNyquist(1.0);

    m_delta.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        f = m_samplerate * double(i) / m_fftLength;

        real = ((f < 24.0) || (f > m_dbFmax)) ? 0.0f : 1.0f;
        imag = 0.0f;
    });
}

// ------------------------------------- Alpha & PU Probe calibration methods --
inline void _cpx_ratio(const kiss_fft_cpx& n, 
                       const float dr, const float di,
                       kiss_fft_cpx& nOverd)
{
    const float den = (dr * dr + di * di);
    nOverd.r = (n.r * dr + n.i * di) / den;
    nOverd.i = (n.i * dr - n.r * di) / den;
}

inline void _cpx_ratio(const float nr, const float ni,
                       const float dr, const float di,
                       kiss_fft_cpx& nOverd)
{
    const float den = (dr * dr + di * di);
    nOverd.r = (nr * dr + ni * di) / den;
    nOverd.i = (ni * dr - nr * di) / den;
}

void Aurora::Correlator::Alpha(const Aurora::Correlator::IntensimetricProbeType pt)
{
    // Calcolo Alpha (normalizzata fra zero e 1)

    const float x        = float(m_dbMicsDistance/1000.0); // d è in mm, x è in metri
    const float binWidth = m_samplerate / float(m_fftLength);

    m_alpha.f.SetDC(0.0);
    m_alpha.f.SetNyquist(0.0);

    if (pt == Aurora::Correlator::IntensimetricProbeType::kPU)
    {
        m_alpha.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
        {
            const float f = binWidth * float(i);
#if 1
            // updated from Aurora for Audition 4.5
            if (f < 24.0f || f > m_dbFmax)
            {
                real = 0.0f;
                imag = 0.0f;
            }
            else
            {
                // alfa = 2*I/(Dc + I) - formula intensimetrica Farina/Fausti
                const float Glr_i = std::fabs(m_Glr.f.Real(i));
                real = 2.0f * Glr_i
                    / ((m_Grr.f.Real(i) + m_Gll.f.Real(i)) / 2.0f + Glr_i + epsilon);
                imag = 0.0f;
            }
#else
            const float k = 2.0 * M_PI * f / m_dbSoundSpeed;
            const float arg = 2.0f * k * x;

            const float Zr = -m_Glr.f.Real(i) / (m_Grr.f.Real(i) + epsilon);  // pv/vv = p/v
            const float Zi = -m_Glr.f.Imag(i) / (m_Grr.f.Real(i) + epsilon);

            const float Zsq = Zr * Zr + Zi * Zi;
            const float Zden = Zsq + 1.0f + 2.0f * Zr;

            const float Rr = (std::cos(arg) * (Zsq - 1.0f)
                - 2.0f * std::sin(arg) * Zi) / Zden;

            const float Ri = (std::sin(arg) * (Zsq - 1.0f)
                + 2.0f * std::cos(arg) * Zi) / Zden;

            const float Rsq = Rr * Rr - Ri * Ri;
            const float Rden = 1.0f + Rsq - 2.0f * Rr;

            real = (1.0f - Rsq) / Rden;
            imag = 2.0f * Ri / Rden;
#endif

        });
    }
    else if (pt == IntensimetricProbeType::kPP)
    {
        // from Aurora for Audition 4.5

        m_alpha.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
        {

            const float f = binWidth * float(i);
            const float csi = 2.0f * M_PI * f * x / m_dbSoundSpeed;

            // Metodo classico (Chunge e Blaser):
            // R = (H12 - exp(-jks)) / (exp(jks) - H12)
            // Z = (1+R) / (1-R)

            kiss_fft_cpx h;
            _cpx_ratio(m_Glr.f[i], m_Gll.f.Real(i), 0.0f, h);

            const float cos_csi = std::cos(csi);
            const float sin_csi = std::sin(csi);

            kiss_fft_cpx r;
            _cpx_ratio( h.r - cos_csi, h.i + sin_csi,
                        cos_csi - h.r, sin_csi - h.i, r);
            
            kiss_fft_cpx z;
            _cpx_ratio(1.0f + r.r,  r.i, 
                       1.0f - r.r, -r.i, z);

            real = z.r;
            imag = z.i;
        });
    }
    else
    {
        assert(1);
    }
}

void Aurora::Correlator::PUCalibrationSWTube() 
{
    // Se d e` finita, mette in Out il rapporto complesso fra gli spettri di Glr e Grr, e 
    //moltiplica per [0,tan(2*pi*f*d/c)]
    const double d = m_dbMicsDistance / 1000.0; // d in metri
    const double t = d / m_dbSoundSpeed;
    const double binWidth = m_samplerate / double(m_fftLength);
   
    m_alpha.f.SetDC(0.0);
    m_alpha.f.SetNyquist(0.0);

    m_alpha.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        const double f = binWidth * double(i);

        if ((f < 24.0) || (f > m_dbFmax))
        {
            real = 0.0f;
            imag = 0.0f;
        }
        else
        {
            const float tank = (float) std::tan(2.0 * M_PI * f * t);
            const float den  = m_Glr.f.SquareModule(i) + epsilon;

            kiss_fft_cpx hvp;
            // rapporto complesso Gpp/Gpv 
            hvp.r = (  m_Gll.f.Real(i) * m_Glr.f.Real(i)
                     + m_Gll.f.Imag(i) * m_Glr.f.Imag(i)) / den;

            hvp.i = ( m_Gll.f.Imag(i) * m_Glr.f.Real(i)
                    - m_Gll.f.Real(i) * m_Glr.f.Imag(i)) / den;

            real = -hvp.i * tank;
            imag =  hvp.r * tank;
        }
    });
}

void Aurora::Correlator::PUCalibrationFreeField() 
{
    // Mette in Out il rapporto complesso fra gli spettri di P e V, e 
    // divide per [1+1/jkd]
    // (p = left, v = right)

    const double d = m_dbMicsDistance / 1000.0; // d in metri
    const double t = d / m_dbSoundSpeed;
    const double binWidth = m_samplerate / double(m_fftLength);

    m_alpha.f.SetDC(0.0);
    m_alpha.f.SetNyquist(0.0);

    m_alpha.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        const double f = binWidth * double(i);

        if ((f < 24.0) || (f > m_dbFmax))
        {
            real = 0.0f;
            imag = 0.0f;
        }
        else
        {
            const float k   = float(2.0 * M_PI * f * t);
            const float den = m_Glr.f.SquareModule(i) + epsilon;
              
            kiss_fft_cpx hvp;
            // rapporto complesso Gpp/Gpv 
            hvp.r = (  m_Gll.f.Real(i) * m_Glr.f.Real(i)
                     + m_Gll.f.Imag(i) * m_Glr.f.Imag(i)) / den;

            hvp.i = (  m_Gll.f.Imag(i) * m_Glr.f.Real(i)
                     - m_Gll.f.Real(i) * m_Glr.f.Imag(i)) / den;

            if (k > 0.0f)
            {
                // formula del campo sferico
                real = (hvp.r + hvp.i / k);
                imag = (hvp.i - hvp.r / k);
            }
            else
            {
                // d=inf -> puro rapporto spettrale fra p e v senza correzione
                //          per la distanza...
                real = hvp.r;
                imag = hvp.i;
            }
        }
    });
}

// ----------------------------------------------------- Correlation methods ---

void Aurora::Correlator::AutoSpectrum(      Aurora::ComplexSpectrum& out,
                                      const Aurora::ComplexSpectrum& in,
                                      Aurora::SampleCount currentBlock)
{
    // It applies Welch's method
    //  https://ccrma.stanford.edu/~jos/sasp/Welch_s_Method.html#sec:pwelch
   // Aurora::SampleCount i;
    Aurora::Sample norm  = Aurora::Sample(currentBlock);
    Aurora::Sample dnorm = Aurora::Sample(currentBlock + 1);

    if (m_lowerBound == 0)
    {
        out.SetDC     ( (out.DC() * norm + in.DC() * in.DC()) / dnorm );
        out.SetNyquist( (out.Ny() * norm + in.Ny() * in.Ny()) / dnorm );
    }
    
    Aurora::Range range(1, m_lowerBound + 1);

    out.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;  // real
        imag = 0.0f; //Imag. always 0 in the auto-spectrum
    });

    range.first  = range.second;
    range.second = m_higherBound;

    out.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        // out = in * conj(in)
        real = (real * norm + in.Real(i) * in.Real(i)
                            + in.Imag(i) * in.Imag(i)) / dnorm;
        imag = 0.0f; //Imag. always 0 in the auto-spectrum
    });

    range.first = range.second;
    range.second = m_fftLength / 2;

    out.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;  // real
        imag = 0.0f; //Imag. always 0 in the auto-spectrum
    });
}

void Aurora::Correlator::CrossSpectrum(Aurora::SampleCount currentBlock)
{
    // It applies Welch's method
    //  https://ccrma.stanford.edu/~jos/sasp/Welch_s_Method.html#sec:pwelch

    Aurora::Sample norm  = Aurora::Sample(currentBlock);
    Aurora::Sample dnorm = Aurora::Sample(currentBlock + 1);

    if(m_lowerBound == 0)
    {
        m_Glr.f.SetDC(      (m_Glr.f.DC() * norm + m_left.f.DC() * m_right.f.DC()) / dnorm);
        m_Glr.f.SetNyquist( (m_Glr.f.Ny() * norm + m_left.f.Ny() * m_right.f.Ny()) / dnorm);
    }

    Aurora::Range range(1, m_lowerBound + 1);

    m_Glr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;
        imag = imag * norm / dnorm;
    });
    range.first = range.second;
    range.second = m_higherBound;

    m_Glr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        // L * conj(R)
        real = (real * norm + m_left.f.Real(i) * m_right.f.Real(i)
                            + m_left.f.Imag(i) * m_right.f.Imag(i)) / dnorm;

        imag = (imag * norm + m_left.f.Imag(i) * m_right.f.Real(i)
                            - m_left.f.Real(i) * m_right.f.Imag(i)) / dnorm;
    });

    range.first = range.second;
    range.second = m_fftLength / 2;

    m_Glr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;
        imag = imag * norm / dnorm;
    });
}
	    
void Aurora::Correlator::Coherence()
{
    // Calcolo funzione di coerenza (normalizzata fra zero e 1)
    //
    // C_xy = |G_xy|^2 / (G_xx * G_yy)
    // 
    // where G_zz are the cross and auto spectral density
    
    m_C.f.SetDC(std::sqrt(    m_Glr.f.DC() * m_Glr.f.DC() 
                           / (m_Gll.f.DC() * m_Grr.f.DC() + epsilon) ));

    m_C.f.SetNyquist(std::sqrt(    m_Glr.f.Ny() * m_Glr.f.Ny() 
                                / (m_Gll.f.Ny() * m_Grr.f.Ny() + epsilon) ));

    m_C.f.ForEachSpectralBin([&](const size_t i, float& real, float& imag)
    {
        real = std::sqrt(   m_Glr.f.SquareModule(i)
                         / (m_Gll.f.Real(i)* m_Grr.f.Real(i) + epsilon));
        imag = 0.0f;
    });
}

void Aurora::Correlator::WhitenedCrossSpectrumPhase()
{
    // Calcolo del Whitened cross-spectrum phase (CSP) - Farina/Zanetta

    Aurora::SampleCount midx = 0;
    Aurora::Sample    modMax = m_Glr.f.FindAbsMax(midx);
    Aurora::Range range(1, m_fftLength / 2 + 1);

    m_Glr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        const float mod = std::sqrt(real*real + imag*imag);

        if (mod > (0.01*modMax))   // Tengo solo i 40 dB superiori.....
        {
            real = real / mod;
            imag = imag / mod;
        }
        else
        {
            real = 0.0f;
            imag = 0.0f;
        }
    });
}

void Aurora::Correlator::CoherenceWeighting(const Aurora::ComplexSpectrum& weights)
{
    // It weights first spectrum with the second one
    Aurora::Range range(1, m_fftLength / 2 + 1);

    m_C.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        const float sqRe = weights.SquareReal(i);
        real = real * sqRe;
        imag = imag * sqRe;
    });
}

void Aurora::Correlator::CrossPowerSpectrumPhase(Aurora::SampleCount currentBlock)
{
    auto norm  = float(currentBlock);
    auto dnorm = float(currentBlock + 1);

    if (m_lowerBound == 0)
    {
        m_Wlr.f.SetDC(     m_Wlr.f.DC() * norm + m_left.f.DC() * m_right.f.DC() / dnorm );
        m_Wlr.f.SetNyquist(m_Wlr.f.Ny() * norm + m_left.f.Ny() * m_right.f.Ny() / dnorm );
    }

    Aurora::Range range(1, m_lowerBound + 1);

    m_Wlr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;
        imag = imag * norm / dnorm;
    });
    range.first = range.second;
    range.second = m_higherBound;

    m_Wlr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        // re + j*im = Lf * conj(Rf)
        const float lrr =   m_left.f.Real(i) * m_right.f.Real(i)
                          + m_left.f.Imag(i) * m_right.f.Imag(i); // Real
        const float lri =   m_left.f.Imag(i) * m_right.f.Real(i)
                          - m_left.f.Real(i) * m_right.f.Imag(i); // Imag
/*
        const float ll =   m_left.f.Real(i) * m_left.f.Real(i)
                         + m_left.f.Imag(i) * m_left.f.Imag(i);
        const float rr =   m_right.f.Real(i) * m_right.f.Real(i)
                         + m_right.f.Imag(i) * m_right.f.Imag(i);
        float den = std::sqrt(ll) + std::sqrt(rr);                 // Omologo
 */
        float den = std::sqrt(lrr*lrr + lri * lri);                // Farina

        if (den == 0.0f)
        {
            den = 1.0f;
        }
        real = (real * norm + lrr / den) / dnorm;
        imag = (imag * norm + lri / den) / dnorm;
    });
    range.first = range.second;
    range.second = m_fftLength / 2;

    m_Wlr.f.ForEach(range, [&](const size_t i, float& real, float& imag)
    {
        real = real * norm / dnorm;
        imag = imag * norm / dnorm;
    });
}

// ------------------------------------------------------ Processing methods ---

bool Aurora::Correlator::DoCrossCorrelation() 
{
    const Aurora::SampleCount inputLength    = m_inputs[0].Length();
    const Aurora::SampleCount blockLength    = m_fftLength;
    const Aurora::SampleCount numberOfBlocks = GetBlocksNumber();
        
    if(numberOfBlocks == 0 )
    {
        return false;
    }
    ProgressMeterWrapper::SetRange((int)numberOfBlocks + 1);

    Aurora::SampleCount i = 0;
    Aurora::SampleCount j = 0;

    // Analysis window calculation. TODO: what happens if m_fftLength = 1 ???
    InitWindow(blockLength);
    
    m_RMS.left  = 0.0;
    m_RMS.right = 0.0;
    m_RMS.cross = 0.0;
    
    for(Aurora::SampleCount block = 0; block <= numberOfBlocks; block++)
    {
        // Check input vectors offset: if the last part is too short...
        if( ((j = block * (blockLength / 4)) + blockLength) >= inputLength)
        {    
           // ...rebuild a shorter window!
           InitWindow(inputLength - j);
        }

        m_left.t.Clear();
        m_right.t.Clear();
        
        // li estraggo in lt e rt, applicando la finestra
        for (i = 0; (i < blockLength) && (j < inputLength); i++)
        {
            m_left.t [i] = m_inputs[CH_LEFT][j]  * m_window[i];
            m_right.t[i] = m_inputs[CH_RIGHT][j] * m_window[i];
            
            m_RMS.left  += m_left.t [i] * m_left.t [i];
            m_RMS.right += m_right.t[i] * m_right.t[i];
            m_RMS.cross += m_left.t [i] * m_right.t[i];
            j++;
        } 
       
        // Ora calcolo le FFT dirette dei dati
        m_left.FFT(); 
        m_right.FFT();
        
        // Set correlation bounds, if the case
        SetBounds();
    
        // Infine calcolo i valori spettrali medi
        AutoSpectrum(m_Gll.f, m_left.f, block);
        AutoSpectrum(m_Grr.f, m_right.f, block);
        CrossSpectrum(block);

        if(m_nXFnTypeId == Aurora::XFunctionType::kCrossPower) 
        {
            // Calcolo Cross-power Spectrum Phase secondo Omologo
           CrossPowerSpectrumPhase(block);
        }
        
        // aggiorno il progress meter
        if(! ProgressMeterWrapper::Update((int)block + 1))
        {
            return false;
        }
    }    
    // Calcolo i valori complessivi RMS normalizzati sulla lunghezza totale effettiva
    // e corretti per il tipo di finestra impiegata
    const double leff = (double)(numberOfBlocks * blockLength);
    
    m_RMS.left   = std::sqrt(m_RMS.left  / leff);
    m_RMS.right  = std::sqrt(m_RMS.right / leff);
    m_RMS.cross /= leff;

    //Now we have: in m_Gll and m_acpxGrr the autospectrums of left and right channels,
    //             in m_Glr the cross-spectrum of left and right channels,
    //             in m_RMS  the rms values of the input vectors,
    //             in m_Wlr the cross-power spectrum phase (if option selected)
    
    return true;
}

void Aurora::Correlator::PostProcessAutocorrelation()
{
    ProgressMeterWrapper::SetMessage("Post-processing Autocorrelation...");
    ProgressMeterWrapper::SetRange(100);

    m_Gll.IFFT();
    m_Grr.IFFT();
    
    // ---------------------------------------------------- Shift to half option
    if(m_options.shiftToHalfWindow)
    {
        m_Gll.t.Rotate(m_fftLength/2);
        m_Grr.t.Rotate(m_fftLength/2);
    }
    // --------------------------------------------------- Get Mean Square Power
    // At lag 0, autocorrelation reduces to average power
    //    r_x[0] = 1/N sum_{m=0}^{N-1} |x(m)|^2
    // cfr: https://ccrma.stanford.edu/~jos/mdft/Autocorrelation.html#21254
    const double Glr_0 = m_Glr.t[0];  // aka CC0...
    ProgressMeterWrapper::Update(30);

    // ------------------------------------------------ Hilbert transform option
    if(m_options.squaredHilbertTransform)
    {
        HilbertTransform(m_Gll.f, m_hilbertLeft.f);
        HilbertTransform(m_Grr.f, m_hilbertRight.f);
        
        m_hilbertLeft.IFFT();
        m_hilbertRight.IFFT();
    
        if(m_options.shiftToHalfWindow)
        {
           m_hilbertLeft.t.Rotate(m_fftLength/2);
           m_hilbertRight.t.Rotate(m_fftLength/2);
        }
        
        for(Aurora::SampleCount i = 0; i < m_fftLength; i++)
        {
           m_Gll.t[i] = m_Gll.t[i] * m_Gll.t[i] + m_hilbertLeft.t[i]  * m_hilbertLeft.t[i];
           m_Grr.t[i] = m_Grr.t[i] * m_Grr.t[i] + m_hilbertRight.t[i] * m_hilbertRight.t[i];
        }
    }  
    else 
    {    
        m_Gll.t.RemoveDC();
        m_Gll.t.RemoveDC();
    }
    ProgressMeterWrapper::Update(80);
        
    //--------------------------------------------------- ....and finally normalize to calculated values
    if (m_options.normalize)
    {
        m_Gll.t.Normalize();
        m_Grr.t.Normalize();
    }
    ProgressMeterWrapper::Update(100);
}

void Aurora::Correlator::PostProcessCrossFunction(Aurora::Correlator::TVector& data)
{
    ProgressMeterWrapper::SetMessage("Post-processing XFunction...");
    ProgressMeterWrapper::SetRange(100);

    // --------------------------------------- Coherence calculation
    Coherence();
    ProgressMeterWrapper::Update(10);
    
    // --------------------------------------- Coherence weighting option
    if(m_options.coherenceWeighting)
    {
        CoherenceWeighting(data.f);
    }
    data.IFFT();   // cross-correlation
    m_C.IFFT(); // coherence

    // just for give some mental order...
    Aurora::SamplesVector& out = data.t;
    const Aurora::ComplexSpectrum& in = data.f;
    
	ProgressMeterWrapper::Update(20);
    
    // --------------------------------------- Shift to half option
    if(m_options.shiftToHalfWindow)
    {
        out.Rotate(m_fftLength / 2);
        m_C.t.Rotate(m_fftLength / 2);
        m_delta.t.Rotate(m_fftLength / 2); // this costs a very little...
    }
    ProgressMeterWrapper::Update(40);
    
    // ---------------------------------------- Get Mean Square Power
    // At lag 0, autocorrelation reduces to average power
    //    r_x[0] = 1/N sum_{m=0}^{N-1} |x(m)|^2
    // cfr: https://ccrma.stanford.edu/~jos/mdft/Autocorrelation.html#21254
    Aurora::Sample Glr_0 = m_Glr.t[0];  // aka CC0....
    
    // --------------------------------------- Hilbert transform option
    if(m_options.squaredHilbertTransform)
    {
        HilbertTransform(in, m_hilbertLeft.f);
        m_hilbertLeft.IFFT();

        if(m_options.shiftToHalfWindow)
        {
           m_hilbertLeft.t.Rotate(m_fftLength/2);
        }
        
        for(Aurora::SampleCount i = 0; i < m_fftLength; i++)
        {
           out[i] = out[i] * out[i] + m_hilbertLeft.t[i] * m_hilbertLeft.t[i]; // sq. mod.
        }
    } 
    else 
    {    
        out.RemoveDC();
    }
    ProgressMeterWrapper::Update(60);
    
    //---------------------------------------- Searching for maximum values...          
    m_Gmax.xcorr = out.GetAbsMax(m_Gmax.index);
    
    // --------------------------------------- Crosscorrelation Max scaling factor
    m_GlrMax = (m_RMS.cross / m_RMS.left / m_RMS.right) / Glr_0; // TBR
    ProgressMeterWrapper::Update(80);
    
    // --------------------------------------- Cross correlation stuffs...
    if(! m_options.normalize && m_nXFnTypeId == Aurora::XFunctionType::kCrossCorrelation)
    {
        // Note: in the original code, the UI flag "Normalize"
        //       corresponds to RemoveDC code option

        // Normalized correlation:
        //NC = Sumi( A(i)*B(i) ) / Sqrt( Sumi( A(i)*A(i) ) * Sumi( B(i)*B(i) )
        //MaxCC = (Xmax/CC0) * (RMSlr/RMSl/RMSr));
            
        // visualizzo il valore vero della CC normalizzata
        m_Gmax.xcorr = Glr_0 / m_RMS.cross * m_RMS.left * m_RMS.right;
    }
    ProgressMeterWrapper::Update(90);
    
    //--------------------------- ....and finally normalize to calculated values
    out.Normalize(m_Gmax.xcorr > 0 ? m_Gmax.xcorr : 1.0);

    if (m_nXFnTypeId == Aurora::XFunctionType::kCrossCorrelation)
    {
        // Delay calculation
        m_dbDelay = Delay(m_Gmax.index);

        // Fly time calculation
        m_dbTriggerTime = Trigger(m_Glr.t);
    }
    ProgressMeterWrapper::Update(100);
}


bool Aurora::Correlator::Process()
{
    assert(m_inputs[0].Length() > 0);
    
    ProgressMeterWrapper::Show("Computing...", 100, true);
    
    // Call to pre-process method
    if (! DoCrossCorrelation())
    {
        ProgressMeterWrapper::Destroy();
        return false;
    }

    //Now we have: in m_Gll and m_Grr the autospectrums of left and right channels,
    //             in m_Glr the cross-spectrum of left and right channels,
    //             in m_adbRMS  the rms values of the input vectors,
    //             in m_Wlr the cross-power spectrum phase (if option selected)

    wxLogDebug("lb: %ld, hb: %ld\n", long(m_lowerBound), long(m_higherBound));

    //  Aurora::Sample Glr_0 =0.0;  // CC0
    
    switch(m_nXFnTypeId)
    {    
       case Aurora::XFunctionType::kAutoCorrelation:
            PostProcessAutocorrelation();
            m_outputs[0].Copy(m_Gll.t);
            m_outputs[0].SetLabel("AutoCorrelation left");
            m_outputs[1].Copy(m_Grr.t);
            m_outputs[1].SetLabel("AutoCorrelation right");;
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;
          
       case Aurora::XFunctionType::kCrossCorrelation:
            PostProcessCrossFunction(m_Glr);
            m_outputs[0].Copy(m_Glr.t);
            m_outputs[0].SetLabel("CrossCorrelation");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;

       case Aurora::XFunctionType::kWhiteCorrelation:
            WhitenedCrossSpectrumPhase();
            PostProcessCrossFunction(m_Glr);
            m_outputs[0].Copy(m_Glr.t);
            m_outputs[0].SetLabel("WhiteCorrelation");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;
          
       case Aurora::XFunctionType::kCrossPower:
            PostProcessCrossFunction(m_Wlr);
            m_outputs[0].Copy(m_Wlr.t);
            m_outputs[0].SetLabel("CrossPower");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;
          
       case Aurora::XFunctionType::kH1:
            HFilter(m_Glr.f, m_Gll.f);                 // now m_Hx is filled
            PostProcessCrossFunction(m_Hx);
            m_outputs[0].Copy(m_Hx.t);
            m_outputs[0].SetLabel("TransferFunction_H1");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;

       case Aurora::XFunctionType::kH2:
            HFilter(m_Grr.f, m_Glr.f);                 // now m_Hx is filled
            PostProcessCrossFunction(m_Hx);
            m_outputs[0].Copy(m_Hx.t);
            m_outputs[0].SetLabel("TransferFunction_H2");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;
          
       case Aurora::XFunctionType::kH3:
            H3Filter(m_Gll.f,m_Grr.f);                // H3, rapporto fra i moduli degli autospettri
            PostProcessCrossFunction(m_Hx);
            m_outputs[0].Copy(m_Hx.t);
            m_outputs[0].SetLabel("TransferFunction_H3");
            m_outputs[1].Copy(m_C.t);
            m_outputs[1].SetLabel("Coherence");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;

       case Aurora::XFunctionType::kAlphaPU: 
            Coherence();      // Coerenza
            m_C.IFFT();
            Alpha(IntensimetricProbeType::kPU);      // Absorption Coefficient canale left
            m_alpha.IFFT();
            m_alpha.t.RemoveDC();

            if (m_options.shiftToHalfWindow)
            {
                m_delta.t.Rotate(m_fftLength / 2);
                m_alpha.t.Rotate(m_fftLength / 2);
                m_C.t.Rotate(m_fftLength / 2);
            }
            m_outputs[0].Copy(m_delta.t);
            m_outputs[0].SetLabel("DiracDelta");
            m_outputs[1].Copy(m_alpha.t);
            m_outputs[1].SetLabel("AlphaPU");
            mChannelsToExport = (m_options.diracPulse ? 3 : 2);
            break;

       case Aurora::XFunctionType::kAlphaPP:
           Coherence();      // Coerenza
           m_C.IFFT();
           Alpha(IntensimetricProbeType::kPP);      // Absorption Coefficient canale left
           m_alpha.IFFT();
           m_alpha.t.RemoveDC();

           if (m_options.shiftToHalfWindow)
           {
               m_delta.t.Rotate(m_fftLength / 2);
               m_alpha.t.Rotate(m_fftLength / 2);
               m_C.t.Rotate(m_fftLength / 2);
           }
           m_outputs[0].Copy(m_delta.t);
           m_outputs[0].SetLabel("DiracDelta");
           m_outputs[1].Copy(m_alpha.t);
           m_outputs[1].SetLabel("AlphaPP");
           mChannelsToExport = (m_options.diracPulse ? 3 : 2);
           break;

       case Aurora::XFunctionType::kPUCalibrationFreeField:
            Dirac(); 
            PUCalibrationFreeField(); // 1/H2/(1+1/jkd)
            m_delta.IFFT();
            m_alpha.IFFT();
            m_alpha.t.RemoveDC();

            if (m_options.shiftToHalfWindow)
            {
                m_delta.t.Rotate(m_fftLength / 2);
                m_alpha.t.Rotate(m_fftLength / 2);
            }
            m_outputs[0].Copy(m_delta.t);
            m_outputs[0].SetLabel("DiracDelta");
            m_outputs[1].Copy(m_alpha.t);
            m_outputs[1].SetLabel("PU_Probe_Calib_Free_field");
            mChannelsToExport = 2;
            break;
            
       case Aurora::XFunctionType::kPUCalibrationSwTube:
            Dirac();               // Delta di Dirac canale Left
            PUCalibrationSWTube(); // 1/H2 *tan(kd)
            m_delta.IFFT();
            m_alpha.IFFT();
            m_alpha.t.RemoveDC();

            if (m_options.shiftToHalfWindow)
            {
                m_delta.t.Rotate(m_fftLength / 2);
                m_alpha.t.Rotate(m_fftLength / 2);
            }
            m_outputs[0].Copy(m_delta.t);
            m_outputs[0].SetLabel("DiracDelta");
            m_outputs[1].Copy(m_alpha.t);
            m_outputs[1].SetLabel("PU_Probe_Calib_SW_tube");
            mChannelsToExport = 2;
            break;
    }    

    if (mChannelsToExport > 2 && m_options.diracPulse)
    {
        m_outputs[2].Copy(m_delta.t);
        m_outputs[2].SetLabel("DiracDelta");
    }
    
    // Convert RMS values in deciBels
    m_RMS.left  = dB20(m_RMS.left);
    m_RMS.right = dB20(m_RMS.right);
    m_RMS.cross = dB(std::fabs(m_RMS.cross));

    return (m_bIsProcessed = true);
}

// ---------------------------------------------------------------------- set & get methods ---
Aurora::ComplexSpectrum& Aurora::Correlator::GetAutocorrelationSpectrum(const int ch)
{ 
    return (ch == 0) ? m_Gll.f : m_Grr.f;     
}

Aurora::ComplexSpectrum& Aurora::Correlator::GetHilbertTransform(const int ch)
{ 
    return ch == 0 ? m_hilbertLeft.f : m_hilbertRight.f; 
}

Aurora::ComplexSpectrum& Aurora::Correlator::GetFunctionSpectrum(const int nXfn, const int nCh)
{
   switch(nXfn)
   {    
      case Aurora::XFunctionType::kAutoCorrelation:        return (nCh == 0) ? m_Gll.f : m_Grr.f;
      case Aurora::XFunctionType::kCrossCorrelation:       return m_Glr.f;
      case Aurora::XFunctionType::kWhiteCorrelation:       return m_Glr.f;
      case Aurora::XFunctionType::kCrossPower:             return m_Wlr.f;
      case Aurora::XFunctionType::kH1:
      case Aurora::XFunctionType::kH2:
      case Aurora::XFunctionType::kH3:                     return m_Hx.f;
      case Aurora::XFunctionType::kCoherence:              return m_C.f;
      case Aurora::XFunctionType::kAlphaPU:
      case Aurora::XFunctionType::kPUCalibrationSwTube:
      case Aurora::XFunctionType::kPUCalibrationFreeField: return m_alpha.f;
      case Aurora::XFunctionType::kDiracDelta:             return m_delta.f;
   }
   return m_Gll.f;
}

void Aurora::Correlator::SetBounds()
{
    assert(m_fftLength > 0);
    const Aurora::SampleCount spectrumLength = m_fftLength / 2 + 1;

    if(m_bIsFollowingFilterOn)
    {
        
        Aurora::SampleCount iMax = 0;
        auto max = (m_nFollowChn == CH_LEFT) ? m_left.f.FindAbsMax(iMax) 
                                             : m_right.f.FindAbsMax(iMax);
        wxLogDebug("Found max for channel %d: %f at sample %d",
                   m_nFollowChn, max, (int)iMax);
        
        const double bandFactor = std::exp2(m_dbFollowBW/2.0);

        m_higherBound = Aurora::SampleCount(iMax * bandFactor);
        m_lowerBound  = Aurora::SampleCount(iMax / bandFactor);

        if (m_lowerBound > spectrumLength)
        {
            m_lowerBound = 0;
        }

        if( m_higherBound > spectrumLength)
        {
            m_higherBound = spectrumLength - 1;
        }   
    } 
    else 
    {
        m_lowerBound  = 0;
        m_higherBound = spectrumLength - 1;
    }
}

void Aurora::Correlator::ResizeInput(const Aurora::SampleCount len, const int ch)
{
    m_inputs[ch].Resize(len, false);
}

void Aurora::Correlator::ResizeInputs(const Aurora::SampleCount len)
{
    for (auto& in : m_inputs)
    {
        in.Resize(len, false);
    }    
}

void Aurora::Correlator::SetFFTLength(const Aurora::SampleCount value)
{ 
    m_fftLength = value; 
    //m_blockLength = value;
    //m_spectrumLength = 1 + value/2;     

    m_left.Resize(m_fftLength);
    m_right.Resize(m_fftLength);
    m_Gll.Resize(m_fftLength);
    m_Grr.Resize(m_fftLength);
    m_Glr.Resize(m_fftLength);
    m_C.Resize(m_fftLength);
    m_Hx.Resize(m_fftLength);
    m_Wlr.Resize(m_fftLength);
    m_hilbertLeft.Resize(m_fftLength);
    m_hilbertRight.Resize(m_fftLength);
    m_alpha.Resize(m_fftLength);    
    m_delta.Resize(m_fftLength);
    m_delta.t[0] = 1.0; // Dirac pulse at time 0.
    m_Hx.Resize(m_fftLength);
}

Aurora::SampleCount Aurora::Correlator::GetBlocksNumber() const
{
    assert(m_inputs[0].Length() > 0);
    return (m_inputs[0].Length() - m_fftLength) / (m_fftLength / 4 + 1);
}

void Aurora::Correlator::SetFollowingChannel(const int value)  
{ 
    m_nFollowChn = (value == CH_RIGHT) ? CH_RIGHT : CH_LEFT; 
}

// ------------------------------------------------------------------ 'ctors ---

Aurora::Correlator::Correlator()
{
    SetFFTLength(m_fftLength);

    //Used when following filter is enabled
    m_lowerBound  = 0;
    m_higherBound = m_fftLength/2 + 1;
}

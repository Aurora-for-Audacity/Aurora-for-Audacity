/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  afaudio.h

  Angelo Farina - Simone Campanini

***********************************************************************/

#include <aurora.h>

Aurora::SampleCount Aurora::AudioSamples::TimeToSamples(const double seconds) const
{
    return (SampleCount)(seconds * m_rate);
}

void Aurora::AudioSamples::foreach(const Aurora::Range& range,
                                   std::function<void(Aurora::Sample sample)> handler)
{
    auto sample = Samples();

    if (range.first < range.second)
    {
        for (Aurora::SampleCount i = range.first; i < range.second; ++i)
        {
            handler(sample[i]);
        }
    }
    else
    {
        for (Aurora::SampleCount i = range.first; i >= range.second; --i)
        {
            handler(sample[i]);
        }
    }
}

void Aurora::AudioSamples::Copy(const Aurora::AudioSamples& as)
{
    Aurora::TSamplesVector::Copy(as.m_pData, as.m_length);
    m_rate = as.m_rate;
    m_fullscale = as.m_fullscale;
}

Aurora::AudioSamples& Aurora::AudioSamples::operator=(Aurora::AudioSamples&& as)
{
    Aurora::TSamplesVector::operator=(std::move(as));
    m_rate = as.m_rate;
    m_fullscale = as.m_fullscale;
    return *this;
}

//----------------------------------------------------------------------------
// Aurora::AudioTrack implementation
//----------------------------------------------------------------------------

bool Aurora::AudioTrack::Resize(const Aurora::SampleCount length)
{
    AudioSamples::Resize(length, false);
    m_filteredTrack.Resize(length, false);
    return true;
}

void Aurora::AudioTrack::Destroy()
{
    TSamplesVector::Destroy();
    m_filteredTrack.Destroy();
}

void Aurora::AudioTrack::ResetFilteredTrack(const double gain)
{
    m_filteredTrack.Copy(*this);

    if (gain != 1.0)
    {
        m_filteredTrack *= (Aurora::Sample)gain;
    }
}

void Aurora::AudioTrack::ReplaceTrackWithFiltered(const double gain)
{
    Copy(m_filteredTrack);
    operator*=((Aurora::Sample)gain);
}

void Aurora::AudioTrack::MakeDelta(const Aurora::SampleCount delay)
{
    assert(m_length > 0);

    Clear();

    if (delay > m_length)
    {
        m_pData[m_length / 2] = 1.0;
    }
    else
    {
        m_pData[delay] = 1.0;
    }
}

void Aurora::AudioTrack::MakeSine(const double amplitude,
                                  const double frequency,
                                  const double phase)
{
    const double fs = GetRate();
    assert(m_length > 0 && fs > 0);

    const double arg0 = 2.0 * M_PI * frequency / fs;

    for (size_t k = 0; k < m_length; k++)
    {
        m_pData[k] = amplitude * std::sin(arg0 * double(k) + phase);
    }
}

void Aurora::AudioTrack::IIR(const double dbAlpha, 
                             const double dbBeta, 
                             const double dbGamma,
                             const double dbMu, 
                             const double dbSigma)
{
   // code by Angelo Farina
   Aurora::SampleCount n = 0;
   double x_n;
   double x_nm1 = 0.0;
   double x_nm2 = 0.0;
   double y_nm1 = 0.0;
   double y_nm2 = 0.0;
   Aurora::Sample* data = m_filteredTrack.Samples();

   x_n = (double)data[n];
   // x_(n) = 2 * (Alfa * (x_n + Mu * x_nm1 + dbSigma * x_nm2) + dbGamma * y_nm1 - dbBeta * y_nm2) - Basic
   data[n] = Aurora::Sample( 2.0 * (dbAlpha * (x_n + dbMu*x_nm1 + dbSigma*x_nm2) 
                                    + dbGamma*y_nm1 - dbBeta*y_nm2) );
   x_nm2 = x_nm1;
   x_nm1 = x_n;

   n = 1;
   x_n = data[n];
   y_nm1 = data[n-1];
   data[n] = Aurora::Sample( 2.0 * (dbAlpha * (x_n + dbMu*x_nm1 + dbSigma*x_nm2) 
                                    + dbGamma*y_nm1 - dbBeta*y_nm2) );
   x_nm2 = x_nm1;
   x_nm1 = x_n;

   for(n = 2;n < m_filteredTrack.Length(); n++)
   {
      x_n   = data[n];
      y_nm1 = data[n-1];
      y_nm2 = data[n-2];
      // WARNING: feedback components should have minus sign: -> -dbGamma*y_nm1 ???
      data[n] = Aurora::Sample( 2.0 * (dbAlpha * (x_n + dbMu*x_nm1 + dbSigma*x_nm2) 
                                       + dbGamma*y_nm1 - dbBeta*y_nm2) );
      x_nm2 = x_nm1;
      x_nm1 = x_n;
   }
}

void Aurora::AudioTrack::Gain(const double g)
{   
   m_filteredTrack *= (Aurora::Sample)g;
}

void Aurora::AudioTrack::GaindB(const double G)
{
    // G [dB] -> g
    const Aurora::Sample g = std::pow(10.0f, G / 20.0f);
    m_filteredTrack *= g;
}

void Aurora::AudioTrack::Filter(const int nType, 
                                const double f0, 
                                const double fq)
{
   // code by Angelo Farina

   // Two poles IIR filter
   double alpha = 0.0;
   double mu    = 0.0;
   double sigma = 0.0;
   
   const double fs = m_filteredTrack.GetRate();
   assert(fs > 0.0f);

   const double theta0 = 2.0 * M_PI * f0 / fs;
   double d      = 2.0 * std::tan(theta0 / 2.0 / fq) / std::sin(theta0);
   double beta   = 0.5 * (1.0 - 0.5*d*std::sin(theta0)) / (1.0 + 0.5*d*std::sin(theta0));
   double gamma  = (0.5 + beta) * std::cos(theta0);

   switch(nType)
   {
       case BPF:
          mu    = 0.0;
          alpha = (0.5 - beta) / 2.0;
          sigma = -1.0;
          break;
       case LPF:
          mu    = 2.0;
          alpha = (0.5 + beta - gamma) / 4.0;
          sigma = 1.0;
          break;
       case HPF:
          mu    = -2.0;
          alpha = (0.5 + beta + gamma) / 4.0;
          sigma = 1.0;
          break;
       case NotchF: 
          mu    = -2.0 * std::cos(theta0);
          alpha = (0.5 + beta) / 2.0;
          sigma = 1.0;
          break;
   } 

   IIR(alpha, beta, gamma, mu, sigma);
}


void Aurora::AudioTrack::AFilter()
{
    // code by Angelo Farina
    // IIR two-poles filter
    const double fs = m_filteredTrack.GetRate();
    assert(fs > 0.0f);

    //  filter 1 - high pass 20.6 Hz
    double c_1 =  1.0 - std::exp(-2.0 * M_PI * (20.6) / fs);
    double c_1m = 1.0 - c_1;
    //  filter 2 - high pass 20.6 Hz
    double c_2 =  1.0 - std::exp(-2.0 * M_PI * (20.6) / fs);
    double c_2m = 1.0 - c_2;
    //  filter 3 - high pass 107.7 Hz
    double c_3 =  1.0 - std::exp(-2.0 * M_PI * (107.7) / fs);
    double c_3m = 1.0 - c_3;
    //  filter 4 - high pass 737.9 Hz
    double c_4 =  1.0 - std::exp(-2.0 * M_PI * (737.9) / fs);
    double c_4m = 1.0 - c_4;
    //  filter 5 - low pass 12200 Hz
    double c_5 =  1.0 - std::exp(-2.0 * M_PI * (12200.0) / fs);
    double c_5m = 1.0 - c_5;
    //  filter 6 - low pass 12200 Hz
    double c_6 =  1.0 - std::exp(-2.0 * M_PI * (12200.0) / fs);
    double c_6m = 1.0 - c_6;

    // the following are delays
    double delta_1 = 0.0;
    double delta_2 = 0.0;
    double delta_3 = 0.0;
    double delta_4 = 0.0;

    Aurora::SampleCount k = 0;
    Aurora::Sample* data = m_filteredTrack.Samples();

    // The first sample is elaborated separately.
    //   first 4 high pass filters 
    double A = data[k] - delta_1;
    delta_1 = data[k] * c_1 + delta_1 * c_1m;
    double B = A - delta_2;
    delta_2 = A * c_2 + delta_2 * c_2m;
    double C = B - delta_3;
    delta_3 = B * c_3 + delta_3 * c_3m;
    double D = C - delta_4;
    delta_4 = C * c_4 + delta_4 * c_4m;
   
    double E=D;
    double F=D;
    //   2 last low pass filters
    E = D * c_5 + E * c_5m;
    F = E * c_6 + F * c_6m;
    data[k] = Aurora::Sample(F/0.858);
   
    for(k = 1;k < m_filteredTrack.Length(); k++)
    {
        //    first 4 high pass filters
        A = data[k] - delta_1;
        delta_1 = data[k] * c_1 + delta_1 * c_1m;
        B = A - delta_2;
        delta_2 = A * c_2 + delta_2 * c_2m;
        C = B - delta_3;
        delta_3 = B * c_3 + delta_3 * c_3m;
        D = C - delta_4;
        delta_4 = C * c_4 + delta_4 * c_4m;
      
        //    2 last low pass filters
        E = D * c_5 + E * c_5m;
        F = E * c_6 + F * c_6m;
        data[k] = Aurora::Sample(F/0.858); // correction to make 1kHz gain  0 dB
    }
}

void Aurora::AudioTrack::LFilter()
{
    // code by Angelo Farina
 
    double f0, fq;

    //  filter 1 - high pass 10 Hz
    f0 = 10.0f; // Hz
    fq = 0.707f;
    Filter(HPF, f0, fq);
    //  filter 2 - high pass 14 Hz
    f0 = 14.0f; // Hz
    fq = 0.707f;
    Filter(HPF, f0, fq);
    //  filter 3 - high pass  20 Hz
    f0 = 20.0f; // Hz
    fq = 0.707f;
    Filter(HPF, f0, fq);
    /*
    //  filter 4 - low pass 20 kHz
    f0 = 20000.0f; // Hz
    fq = 0.707f;
    Filter(LPF, nCh, f0, fq);
    //  filter 5 - low pass 20.4 kHz
    f0 = 20400.0f; // Hz
    fq = 0.707f;
    Filter(LPF, nCh, f0, fq);
    //  filter 6 - low pass 20.9 kHz
    f0 = 20900.0f; // Hz
    fq = 0.707f;
    Filter(LPF, nCh, f0, fq);
    */
}

void Aurora::AudioTrack::ISO_8041_Filter()
{
   // Ponderazione Accelerazione Asse Generico secondo norma ISO-8041
   //void AccFilter(float  *X,float fSamp,long IRL)
    const double fs = m_filteredTrack.GetRate();
    assert(fs > 0.0f);

// filtro 1 - passa alto 0.7943 Hz
   const double c_1 =  1.0 - std::exp(-2.0 * M_PI * (0.7943) / fs);
   const double c_1m = 1.0 - c_1;
// filtro 2 - passa alto 0.7943 Hz
   const double c_2 =  1.0 - std::exp(-2.0 * M_PI * (0.7943) / fs);
   const double c_2m = 1.0 - c_2;
// filtro 3 - passa basso 5.684 Hz
   const double c_3 =  1.0 - std::exp(-2.0 * M_PI * (5.684) / fs);
   const double c_3m = 1.0 - c_3;
// filtro 4 - passa basso 100 Hz
   const double c_4 =  1.0 - std::exp(-2.0 * M_PI * (100.0) / fs);
   const double c_4m = 1.0 - c_4;
// filtro 5 - passa basso 100 Hz
   const double c_5 =  1.0 - std::exp(-2.0 * M_PI * (100.0) / fs);
   const double c_5m = 1.0 - c_5;

   double delta_1 = 0.0;
   double delta_2 = 0.0;
   double A,B,C=0.0, D=0.0, E=0.0;
   
   Aurora::Sample* data = m_filteredTrack.Samples();

   for(Aurora::SampleCount k = 0; k < m_filteredTrack.Length(); k++) 
   {        
//      primi 2 filtri passa alto in cascata
      A = data[k] - delta_1;
      delta_1 = data[k] * c_1 + delta_1 * c_1m;
      B = A - delta_2;
      delta_2 = A * c_2 + delta_2 * c_2m;

//      3 filtri passa basso conclusivi
      C = B * c_3 + C * c_3m;
      D = C * c_4 + D * c_4m;
      E = D * c_5 + E * c_5m;
      data[k] = Aurora::Sample(E*1.19); // correzione per rendere il guadagno ad 1 Hz = 0 dB
   }
}

void Aurora::AudioTrack::ITU_P56_Filter()
{
    // code by Angelo Farina
    // Filtro passa-banda secondo ITU-P56 (160-5500 Hz)
    double f0,fq;
   
    //  filtro 1 - passa alto 110 Hz
    f0=110.0; // Hz
    fq=0.7;   // Butterworth
    Filter(HPF, f0, fq);
    //  filtro 2 - passa alto 90 Hz
    f0=90.0;  // Hz
    fq=1.5;
    Filter(HPF, f0, fq);
    //  filtro 3 - passa basso 6500 Hz
    f0=6500.0; // Hz
    fq=0.8;
    Filter(LPF, f0, fq);
    //  filtro 4 - passa basso 7500 Hz
    f0=7500.0; // Hz
    fq=1.8;     //era 1.9
    Filter(LPF, f0, fq);
    //  filtro 5 - passa banda 1100 Hz
    f0=1100.0; // Hz
    fq=0.11;
    Filter(BPF, f0, fq);

    // Correzione guadagno -0.11 dB
    Gain(-0.15);
}

void Aurora::AudioTrack::UNI_9916_Filter()
{
    // Filtro passa basso a 250 Hz - UNI 9916
    double f0,fq;
   
    //  filtro 1 - passa basso 250 Hz
    f0=320.0; // Hz
    fq=1.0;   // Butterworth
    Filter(LPF, f0, fq);
    //  filtro 2 - passa basso 260 Hz
    f0=360.0;  // Hz
    fq=0.6;
    Filter(LPF, f0, fq);
    //  filtro 3 - passa basso 270 Hz
    f0=400.0; // Hz
    fq=1.1;
    Filter(LPF, f0, fq);
    //  filtro 4 - passa basso 280 Hz
    f0=500.0; // Hz
    fq=0.5;     //era 1.9
    Filter(LPF, f0, fq);

    // Correzione guadagno 0.0 dB
    Gain(0.0);
}

void Aurora::AudioTrack::OctaveFilter(const double fc)
{   
    // code by Angelo Farina

    double f0, fq;
    // Band filtering with six-poles IIR filter
    // first filter (2 poles)
    f0 = 0.755f * fc;
    fq = 6.0f;
    Filter(BPF, f0, fq);
    // second filter (2 poles)
    f0 = fc;
    fq = 3.0f;
    Filter(BPF, f0, fq);
    // third filter (2 poles)
    f0 = 1.33f * fc;
    fq = 6.0f;
    Filter(BPF, f0, fq);
}


void Aurora::AudioTrack::RemoveMean()
{
   // code by Angelo Farina

   Aurora::Sample mean = Mean();
   operator-=(mean);
}

void  Aurora::AudioTrack::NoiseCorrection()
{
   double   nc = 0.0;
   const int L = (int)m_filteredTrack.Length();
   Aurora::Sample* data = m_filteredTrack.Samples();

   // Now will calculate the mean of the IR's last 16th energy
   for(int i = (L - L/16); i < L; i++)
   {
         const double sample = data[i] * data[i];
         nc += sample;
   }

   m_noiseCorrFactor = nc / double(L/16); // Square of the noise RMS value
                                          // in fact, it is a Leq
}

double Aurora::AudioTrack::RMS()
{
    double RMS = 0;
    const int L = (int)m_filteredTrack.Length();
    Aurora::Sample* data = m_filteredTrack.Samples();

    for (int i = 0; i < L; i++)
    {
        RMS += double(data[i] * data[i]);
    }
    return RMS/double(L);
}

double Aurora::AudioTrack::Leq()
{
    const double rms = RMS();
    return (rms > 0.0 ? 10.0 * std::log10(rms) : -200.0);
}

void Aurora::AudioTrack::SetTrack(Aurora::Sample *pSrc,
                                 const Aurora::SampleCount len)
{
    Resize(len);
    ScalarVector<Aurora::Sample>::Copy(pSrc, len);
    ResetFilteredTrack();
}

void Aurora::AudioTrack::SetFullscale(const double fs)
{
    Aurora::AudioSamples::SetFullscale(fs);
    m_filteredTrack.SetFullscale(fs);
}

Aurora::AudioTrack& Aurora::AudioTrack::operator=(Aurora::AudioTrack&& at)
{
    Aurora::AudioSamples::operator=(std::move(at));
    m_filteredTrack = std::move(at.m_filteredTrack);
    m_noiseCorrFactor = at.m_noiseCorrFactor;
    m_nSelectedFilter = at.m_nSelectedFilter;
    return *this;
}


Aurora::AudioTrack::AudioTrack(AudioTrack&& at)
 : Aurora::AudioSamples(std::move(at)),
    m_filteredTrack(std::move(at.m_filteredTrack)),
    m_noiseCorrFactor(at.m_noiseCorrFactor),
    m_nSelectedFilter(at.m_nSelectedFilter)
{ }

Aurora::AudioTrack::AudioTrack(const SampleCount length, const double dbRate)
  : Aurora::AudioSamples(length, dbRate)
{
    m_filteredTrack.Resize(length);
}

Aurora::AudioTrack::~AudioTrack()
{
    Destroy();
}

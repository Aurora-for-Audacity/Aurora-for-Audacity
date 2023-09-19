/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  AcParametersBase.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::AcousticalParameters
\brief A powerful analizing tool for room acoustics

  This class is an acoustical parameter evaluation tool:
  given a room's impulse response, it evaluates the most important
  parameter, like reverberation time, clarity index, and so on.
  It uses the Schroeder's integral technique, and is a port for
  Audacity of the original Angelo Farina's plug-in for Adobe
  Audition.

*//*******************************************************************/
#include <aurora.h>

#include "AcParametersBase.h"

//#define MULTIRATE_FILTERING 1 

void DumpData(Aurora::Sample* data, const int length, const int id, const char* tag)
{
    FILE* f;
    int k;

    char str[32];
    if(data != 0)
    {
        sprintf(str, "/tmp/%s_%d", tag, id);
        std::cout << "AcParameters dump data " << id << " to " << str << std::endl;
        if((f = fopen(str, "w")) != 0)
        {
            k = 0;

            while(k < length)
            {
                fprintf(f, "%.18e\n", data[k++]);
            }
            fclose(f);
        }
    }
}

//----------------------------------------------------------------------------
// TSchroederDecay implementation
//----------------------------------------------------------------------------

void Aurora::TSchroederDecay::SetData(Aurora::Sample* src, 
                                      const Aurora::SampleCount length)
{
    memcpy(data(), src, length * sizeof(Aurora::Sample));
}

void Aurora::TSchroederDecay::FindFirstArrivalTime()
{
    Aurora::Sample last = front(); // should be the max too...
    Aurora::Sample threshold = last * 1.0e-4;
    Aurora::SampleCount decayStart = 1;

    Aurora::Sample* s = data();

    while(decayStart < size())
    {
        if ((last - s[decayStart]) > threshold)
        {
            break;
        }
        last = s[decayStart];
        decayStart++;
    }

    printf("Decay start: %d\n", int(decayStart));

    m_fat = decayStart;
}

void Aurora::TSchroederDecay::DoBackwardIntegration(Aurora::AcParametersAudioTrack& track)
{
    // Calculate Schroeder Integral.
    // The resulting decay will be stored in m_apsmpDecay vector.
    // It returns the short factor t*p^2, useful for TS determination.
    // TO DO: negative returns on error.
    m_dbRate = track.GetFilteredSamples().GetRate();
    
    int length = (int)track.GetFilteredSamples().Length();
    int i2     = (int)(length - track.GetFirstArrivalTime() *m_dbRate);
    Aurora::Sample* samples = track.Filtered().Samples();
        
    const double nc = track.GetNoiseCorrFactor();
    double asq = 0;
    double sum = 0.0;
    
    if(length != size())
    {  
        Init(length); 
    }

    m_dbTP2 = 0.0;

    for(int i = length-1; i >= 0; i--)
    {
        // Amplitude square. If noise correction is deactivated,
        //  the correction factor is set to 0.0.
        asq = double(samples[i] * samples[i]) - nc;

        // if FAT is not reached, update i2 and dbTP2
        if(i2)
        {
            m_dbTP2 += asq * (i2--);
        }
        // update squares' total
        sum += asq;

        // store actual integral value.
        operator[](i) = Aurora::Sample(sum);
    }

    // samples to seconds conversion
    m_dbTP2 /= track.GetFilteredSamples().GetRate();

    // Find decay real start
    FindFirstArrivalTime();
}

void Aurora::TSchroederDecay::Init(Aurora::AcParametersAudioTrack& track)
{
    Init(track.GetFilteredSamples().Length());
    DoBackwardIntegration(track);
}

void Aurora::TSchroederDecay::Init(const Aurora::SampleCount length)
{
    if (length > 0)
    {
        resize(length);
    }
    
    memset(data(), 0, size() * sizeof(Aurora::Sample));
}

// *** 'ctors

Aurora::TSchroederDecay::TSchroederDecay(const Aurora::SampleCount length)
{ Init(length); }

//----------------------------------------------------------------------------
// Aurora::AcousticalParameters::TResults implementation
//----------------------------------------------------------------------------
Aurora::AcousticalParameters::TResults::TParameter::TParameter(const double v,
                                                               const bool   valid)
    : value(v), isValid(valid) 
{
    if (isnan(value) || isinf(value))
    {
        // nan and infinite values are always unacceptable.
        isValid = false;
    }
}

const std::vector<std::string> Aurora::AcousticalParameters::TResults::m_parameterNames =
{
    "Signal",   "Noise",
    "strenGth", "C50",    "C80",  "D50",  "Ts",
    "EDT",      "Tuser",  "T20",  "T30",

    "Peakiness", "Millisecondness",  "Impulsiveness",

    "St1",     "St2",        "StLate",     // StageParameters (14..)
    "IACC",    "tauIACC",    "widthIACC",  // BinauralParameters (17..)
    "Jlf",     "Jlfc",       "Lj",          // SpatialParameters  (20..)
};

void Aurora::AcousticalParameters::TResults::Set(const std::string& parameterName,
                                                 const float fcb,
                                                 const double value,
                                                 const bool   valid)
{
    m_table.at(parameterName).SetValue(fcb, TParameter(value, valid));
}

void Aurora::AcousticalParameters::TResults::SetSpectrumType(const Aurora::SpectrumType type)
{
    m_table.clear();
    
    for(auto& np : m_parameterNames)
    {
        m_table[np] = Aurora::Spectrum<TParameter>(type);
    }
}

Aurora::AcousticalParameters::TResults::TResults()
{ 
    SetSpectrumType(Aurora::SpectrumType::Octave);
}


//----------------------------------------------------------------------------
// Aurora::AcousticalParameters implementation
//----------------------------------------------------------------------------
Aurora::SampleCount Aurora::AcousticalParameters::TimeToSamples(const int ch, 
                                                                const double seconds) const
{
    // aka DELTA_T
    return m_aAudioTracks[ch].TimeToSamples(seconds);
}


Aurora::SampleCount Aurora::AcousticalParameters::FirstArrivalTime(const int ch) const
{
  // aka FAT_IN_SAMPLES
#ifdef MULTIRATE_FILTERING
    return m_decays[ch].GetFirstArrivalTimeInSamples();
#else
    return (Aurora::SampleCount)(m_aAudioTracks[ch].GetFirstArrivalTime() *
                                 m_aAudioTracks[ch].GetFilteredSamples().GetRate());
#endif
}


Aurora::SampleCount Aurora::AcousticalParameters::LengthFromFAT(const int ch) const
{
  // aka LENGTH_FROM_FAT_IN_SAMPLES
  return (Aurora::SampleCount)(m_aAudioTracks[ch].GetFilteredSamples().Length() 
                               - FirstArrivalTime(ch));
}

Aurora::SampleCount Aurora::AcousticalParameters::GetShorterFAT()
{
    const int right = 
        (m_nSource == Aurora::AcousticalParameters::Source::AmbisonicsFuMa ) ? 2 : 1;

    Aurora::SampleCount lfat = 0;
    Aurora::SampleCount rfat = 0;

#ifdef MULTIRATE_FILTERING
    lfat = m_decays[CH_LEFT].GetFirstArrivalTimeInSamples();
    rfat = m_decays[right].GetFirstArrivalTimeInSamples();
#else
    lfat = FirstArrivalTime(CH_LEFT);
    rfat = FirstArrivalTime(right);
#endif

    return (lfat < rfat) ? lfat : rfat;
}


double Aurora::AcousticalParameters::ReverberationTime(const int nCh,
                                                       const double dBstart,
                                                       const double dBdelta)
{
    // Code by Angelo Farina
    Aurora::AcParametersAudioTrack& track = m_aAudioTracks[nCh];

    // The time immediately after the direct wave
    int i1 = int(  track.GetFirstArrivalTime()
                 * track.GetFilteredSamples().GetRate() );
    int decayLength = (int)m_decays[nCh].GetLength();

    const double L1 =  dB(m_decays[nCh][i1]); // first sample (dB)
    const double L0 =  L1 - dBstart;          // Reverberation time start sample (dB value)
    const double dBend = -dBstart - dBdelta;     // Reverberation time end sample (dB value)

    double y = 0.0,
    sumX  = 0.0,
    sumY  = 0.0,
    sumX2 = 0.0,
    sumY2 = 0.0,
    sumXY = 0.0;

    // find the decay start time
    while( (i1 < decayLength) && (L0 < dB(m_decays[nCh][i1])) )
    {
        i1++;
    }

    if(i1 == track.GetFilteredSamples().Length())
    {
        return -1.0; // failed
    }

    // initialize x
    double x  = 1.0;
    double y1 = dB(m_decays[nCh][i1]) - L1;
    double y2 = 0.0;

    // search and sum cycle
    do
    {
        y = dB(m_decays[nCh][i1]) - L1;

        if ((y >= dBend) && (i1 < decayLength))
        {
            sumX  += x;
            sumY  += y;
            sumXY += y * x;
            sumX2 += x * x;
            sumY2 += y * y;
            y2 = y;
            x += 1.0; 
        }
        i1++;
    } while ((y >= dBend) && (i1 < decayLength));

    // Parameters' limit validity: it should be lower in
    // noise correction case
    int iValid = 0;

    if(m_flags.noiseCorrectionEnabled)
    {
        iValid = (decayLength - decayLength/16);
    }
    else
    {
        iValid = (decayLength - decayLength/64);
    }

    if(i1 > iValid)
    {
        return -1.0; // failed
    }
    // regression calculation
    double N = x - 1.0;
    double denom = N * sumX2 - (sumX * sumX);

    if(denom == 0.0)
    {
        return -1.0;
    }
    double B = (N * sumXY - sumX * sumY) / denom;

    if(m_flags.edtEnabled) 
    {
        B = (y2 - y1) / (N-1);
    }

    if(B == 0.0)
    {
        return -1.0;
    }

    // Sabine's reverberation time
    const double T = (-60.0 / B / track.GetFilteredSamples().GetRate());
    const double denom1 = (N * sumX2 - sumX * sumX) * (N * sumY2 - sumY * sumY);

    if(denom1 == 0.0)
    {
        return -1.0;
    }

    double R = std::fabs( (N * sumXY - sumX * sumY) * (N * sumXY - sumX * sumY)  / denom1);

    if( (R < 0.85) && (dBstart > 0.5) )
    {
        //Insufficient correlation coefficient for rev. time
        // calculation, but not for EDT
        return -1.0;
    }

    return T;
}



//------------------------- Parameters methods -------------------------

double Aurora::AcousticalParameters::Clarity(const int nCh, 
                                             const double width, 
                                             const double delay)
{
   //Clarity index calculation (C50, C80, ...)
   //NB: width must be in seconds!
   double rate = Aurora::SampleCount(m_aAudioTracks[nCh].GetFilteredSamples().GetRate());

    Aurora::SampleCount delta = 0;
    Aurora::SampleCount fat   = 0;

    delta = rate *(width + delay);

#ifdef MULTIRATE_FILTERING
    fat = m_decays[nCh].GetFirstArrivalTimeInSamples();
#else
    fat = Aurora::SampleCount(m_aAudioTracks[nCh].GetFirstArrivalTime() * rate);
#endif

    printf("c50: delta %d, fat %d, dec_fat %f, dec_fat_delta %f\n",
             (int)delta, (int)fat,
              m_decays[nCh][fat],m_decays[nCh][fat + delta]);
    // result in dB
    return ( 10.0 *log10((m_decays[nCh][fat] - m_decays[nCh][fat + delta]) /
                         m_decays[nCh][fat + delta]));
}

double Aurora::AcousticalParameters::Definition(const int nCh, 
                                                const double width, 
                                                const double delay)
{
   //Definition index calculation (D50, ...)
   //NB: width must be in seconds!
    double rate = Aurora::SampleCount(m_aAudioTracks[nCh].GetFilteredSamples().GetRate());
    Aurora::SampleCount delta = 0;
    Aurora::SampleCount fat   = 0;

    delta = rate *(width + delay);

#ifdef MULTIRATE_FILTERING
    fat = m_decays[nCh].GetFirstArrivalTimeInSamples();
#else
    fat = Aurora::SampleCount(m_aAudioTracks[nCh].GetFirstArrivalTime() * rate);
#endif

    // result in %
    return (100.0 * (( m_decays[nCh][fat] - m_decays[nCh][fat + delta] ) /
                      m_decays[nCh][fat] ));
}

double Aurora::AcousticalParameters::EarlyLateralFraction(const double delay)
{
   // aka: JLF
   Aurora::SampleCount delta_5  = TimeToSamples(CH_LEFT, 0.005 + delay);
   Aurora::SampleCount delta_80 = TimeToSamples(CH_LEFT, 0.080 + delay);
   Aurora::SampleCount f8Fat    = 0;
   Aurora::SampleCount omniFat  = 0;
   
   double       gain = 1.0;
   int omniMicIndex  = 0;
   int f8MicIndex    = 1;

   switch(m_nSource)
   {
       case Aurora::AcousticalParameters::Source::SoundfieldWY:
           gain = 0.5;  // It's a Soundfield: need double W energy
           break;

       case Aurora::AcousticalParameters::Source::AmbisonicsFuMa: // WXYZ
           gain = 0.5; 
           f8MicIndex = 2;
           break;

       case Aurora::AcousticalParameters::Source::AmbisonicsAmbix: //WYZX
           f8MicIndex = 1;
           break;
   }
    
#ifdef MULTIRATE_FILTERING
   lfat = m_decays[left].GetFirstArrivalTimeInSamples();
   rfat = m_decays[right].GetFirstArrivalTimeInSamples();
#else
   f8Fat   = m_aAudioTracks[f8MicIndex].GetFirstArrivalTimeInSamples(true);
   omniFat = m_aAudioTracks[omniMicIndex].GetFirstArrivalTimeInSamples(true);
#endif

    double sumEight =   m_decays[f8MicIndex][f8Fat + delta_5]
                      - m_decays[f8MicIndex][f8Fat + delta_80];
    double sumOmni  =   m_decays[omniMicIndex][omniFat]
                      - m_decays[omniMicIndex][omniFat + delta_80];
    

   return gain * sumEight / sumOmni;
}

double Aurora::AcousticalParameters::EarlyLateralFractionCosine(const double delay)
{
   // aka: JLFC
    Aurora::SampleCount i;
    Aurora::SampleCount delta_5  = TimeToSamples(CH_LEFT, 0.005 + delay);
    Aurora::SampleCount delta_80 = TimeToSamples(CH_LEFT, 0.080 + delay);
    Aurora::SampleCount fat      = GetShorterFAT();

    double sumOmni      = 0.0;
    double sumOmniEight = 0.0;
    double gain = 1.0;

   Aurora::Sample* omniSamples = m_aAudioTracks[CH_LEFT].Filtered().Samples();
   Aurora::Sample* f8Samples = nullptr;
   
   switch(m_nSource)
   {
       case Aurora::AcousticalParameters::Source::SoundfieldWY:
           gain = M_SQRT1_2;  // It's a Soundfield: need double W energy
           f8Samples = m_aAudioTracks[1].Filtered().Samples(); //Y
           break;

       case Aurora::AcousticalParameters::Source::AmbisonicsFuMa: //WXYZ:
           gain = M_SQRT1_2;
           f8Samples = m_aAudioTracks[2].Filtered().Samples(); //Y
           break;

       case Aurora::AcousticalParameters::Source::AmbisonicsAmbix: //WYZX:
           f8Samples = m_aAudioTracks[1].Filtered().Samples(); //Y
           break;

       default:
    	   f8Samples = m_aAudioTracks[1].Filtered().Samples();
    	   break;
   }
   

   for (i = fat + delta_5; i < (fat + delta_80); i++)
   {
       sumOmniEight += std::fabs(f8Samples[i] * omniSamples[i]);
   }
   
   for (i = fat; i < (fat + delta_80); i++)
   {
       sumOmni += omniSamples[i] * omniSamples[i];
   }
   
   return gain * sumOmniEight/sumOmni;
}

double Aurora::AcousticalParameters::LateLateralSoundLevel(const float fcb, 
                                                           const double delay)
{
    int f8MicIndex = 1;

    switch(m_nSource)
    {
        case Aurora::AcousticalParameters::Source::SoundfieldWY:
            break;

        case Aurora::AcousticalParameters::Source::AmbisonicsFuMa: // WXYZ
            f8MicIndex = 2;
            break;

        case Aurora::AcousticalParameters::Source::AmbisonicsAmbix: //WYZX
            f8MicIndex = 1;
            break;
    }
    auto& track = m_aAudioTracks[f8MicIndex];
    
    const auto delta_80    = track.TimeToSamples(0.080 + delay);
    const auto fat         = track.GetFirstArrivalTimeInSamples(true);
    const auto fullScaledB = track.GetFullScale();
    const auto ratedB      = dB(track.GetFilteredSamples().GetRate());


    // aka LJ is given
    // by the logarithmic ratio between the late energy of the left
    // channel (after the first 80 ms) and the 10 m reference value.
    // Result in dB
#if 0
    return dB(m_decays[f8MicIndex][fat + delta_80]) - m_referenceLevels.GetValue(fcb);
#else
    const double L80ms = dB(m_decays[f8MicIndex][fat + delta_80])
                         + fullScaledB - ratedB;
    
    return L80ms - m_referenceLevels.GetValue(fcb);
#endif
}

double Aurora::AcousticalParameters::InterAuralCrossCorrelation(const double delay,
                                                                double* ptauIACC,
                                                                double* pwidthIACC)
{
    auto& left  = m_aAudioTracks[CH_LEFT];
    auto& right = m_aAudioTracks[CH_RIGHT];

    int delta_1  = (int)TimeToSamples(CH_LEFT, 0.001 + delay);
    int delta_80 = (int)TimeToSamples(CH_LEFT, 0.080 + delay);

    // Integration limits definition (t1 and t2)
    int t1, t2;
    int fat = (int)GetShorterFAT();
    int L   = (int)left.GetFilteredSamples().Length();

    switch (m_nIaccSpan)
    {
       case IACCSpan::Early: // 0-80 ms
            t1 = fat;
            t2 = fat + delta_80;
            break;

       case IACCSpan::Late: // 80-inf ms
            t1 = fat   + delta_80;
            t2 = L - delta_1;
            break;

       case IACCSpan::All: // 0-inf ms
            t1 = fat;
            t2 = L - delta_1;
            break;

        default: // error, force 0
            m_nIaccSpan = IACCSpan::Early;
            t1 = fat;
            t2 = fat + delta_80;
            break;
    }

    if ((t1 - delta_1) < 0)
    {
        t1 = delta_1; // prevents negative index ?????
    }
   
    // istantiate buffer to store the crosscorrelation
    std::vector<double> R(2 * delta_1 + 1, 0.0);

    Aurora::Sample *lsamples = left.Filtered().Samples();
    Aurora::Sample *rsamples = right.Filtered().Samples();

    double sumL = 0.0;
    double sumR = 0.0;

    // Denominator calculation
    for (auto t = t1; t < t2; t++)
    {
      sumL += double(lsamples[t] * lsamples[t]);
      sumR += double(rsamples[t] * rsamples[t]);
    }
    const double den = std::sqrt(sumL*sumR);
   
    // CC computation and finding peak
    double IACC = 0.0;
    int j = 0, jmax = 0;
    
    for ( ; j <= delta_1; ++j)
    {
        double sumLR = 0.0;
        
        for (int i = t1; i < t2; ++i)
        {
            sumLR += lsamples[i] * rsamples[i - j];
        }
        if (sumLR > IACC)
        {
            IACC = sumLR;
            jmax = -j; // maximum position, left side
        }
        sumLR = 0.0;
        
        for(int i = t1; i < t2; ++i)
        {
            sumLR += lsamples[i] * rsamples[i + j];
        }
        if (sumLR > IACC)
        {
            IACC = sumLR;
            jmax = j; // maximum position, right side
        }
    }
    
    // Finding j values where CC falls to 0.9*max
    double limit = 0.9 * IACC;
    int j1 = 0, j2 = 0;
    
    for (j = 0; j <= delta_1; ++j)
    {
        double sumLR = 0.0;
        
        for (int i = t1; i < t2; ++i)
        {
            sumLR += lsamples[i] * rsamples[i - j + jmax];
        }
        if (sumLR <= limit)
        {
            j1 = -j;
            break; // found left side, exiting
        }
    }
    for (j = 0; j <= delta_1; ++j)
    {
        double sumLR = 0.0;
        
        for(int i = t1; i < t2; ++i)
        {
            sumLR += lsamples[i] * rsamples[i + j + jmax];
        }
        if (sumLR <= limit)
        {
            j2 = j;
            break; // found right side, exiting
        }
    }
    const double fs = left.GetFilteredSamples().GetRate();
    
    IACC /= den;
    *ptauIACC   =      jmax * 1000.0 / fs;
    *pwidthIACC = (j2 - j1) * 1000.0 / fs;
    
    return IACC;
}

double Aurora::AcousticalParameters::Peakiness(const int nCh)
{
   double sq;
   double max = 0.0;
   double sum = 0.0;

   Aurora::Sample* data = m_aAudioTracks[nCh].Filtered().Samples();

   for(Aurora::SampleCount i=0;
       i < m_aAudioTracks[nCh].GetFilteredSamples().Length();
       i++)
   {
      if((sq = data[i]*data[i]) > max)
      {
          max = sq;
      }
      sum += sq;
   }
   
   sum /= m_aAudioTracks[nCh].GetFilteredSamples().Length(); // Samples RMS value

   return dB(max/sum);
}


double Aurora::AcousticalParameters::Millisecondness(const int nCh,
		                                       double* pImpulsivs)
{
   Aurora::AcParametersAudioTrack& track = m_aAudioTracks[nCh];
   int i, j;

   // Build 1 ms RMS data vector
   Aurora::Sample *data = track.Filtered().Samples();

   int decimationStep = int(track.GetFilteredSamples().GetRate() / 1000.0); // 1 ms rate
       
   if(decimationStep == 0)
   {
        return -200.0;
   }
   int undersDataLength  = ((int)track.Filtered().Length() / decimationStep) + 1;

   std::vector<double> undersIr(undersDataLength, 0.0);
   
   for(i = (int)track.GetFirstArrivalTime(); i < (int)track.Filtered().Length(); i++)
   {
      j = i / decimationStep;
      undersIr[j] += data[i] * data[i];
   }

   // Rescale buffer values to obtain correct dB levels
   const double dbScaleFactor  = std::pow( 10.0, track.GetFullScale()/10.0) 
                                 / track.GetRate();

   for (i = 0; i < undersDataLength; i++)
   {
       undersIr[i] *= dbScaleFactor;
   }

   double value;
   double max       = 0.0;
   double sum       = 0.0;
   double impVal    = 0.0;
   double impValMax = 0.0;

   for(i = 0; i < (undersDataLength - 35); i++)
   {
       if ((value = undersIr[i]) > max)
       {
           max = value;
       }

      sum += value;
        
      impVal = 0.0;

      for (j = 0; j < 35; j++)
      {
          //Impx is the mean value over 35 ms: it's the Impulse value
          impVal += undersIr[i + j];
      }

      if (impVal > impValMax) 
      { 
          impValMax = impVal; 
      }
   }

   sum /= (double(undersDataLength) - 35.0); // RMS of Millisecondness buffer

   // This is the Impulsiveness value (maximum of Impulse means over 35 ms)...
   *pImpulsivs = 10.0 * std::log10( impValMax/(35.0 * sum) );

   // ...and this the millisecondness one.
   return dB(max/sum);
}

double Aurora::AcousticalParameters::Stage(const char* par, 
                                           const int nCh, 
                                           const double delay)
{
    Aurora::AcParametersAudioTrack& track = m_aAudioTracks[nCh];

    Aurora::SampleCount deltaT1  = 0;
    Aurora::SampleCount deltaT2  = 0;
    Aurora::SampleCount delta_10 = track.TimeToSamples(0.010 + delay);

    if (strstr(par, "St1") != nullptr)
    {
        deltaT1 = track.TimeToSamples(0.020 + delay);
        deltaT2 = track.TimeToSamples(0.100 + delay);
    }
    else if (strstr(par, "St2") != nullptr)
    {
        deltaT1 = track.TimeToSamples(0.020 + delay);
        deltaT2 = track.TimeToSamples(0.200 + delay);
    }
    else if (strstr(par, "StLate") != nullptr)
    {
        deltaT1 = track.TimeToSamples(0.100 + delay);
        deltaT2 = track.TimeToSamples(1.000 + delay);
    }

    deltaT1 += track.TimeToSamples(delay);
    deltaT2 += track.TimeToSamples(delay);

    // 5: St1, need IR lenght (from FAT) of almost 100 ms
    // 6: St2, need IR lenght (from FAT) of almost 200 ms
    // 7: StL, need IR lenght (from FAT) of almost 1000 ms

#ifdef MULTIRATE_FILTERING
    Aurora::SampleCount fat = m_decays[nCh].GetFirstArrivalTimeInSamples();
#else
    Aurora::SampleCount fat = track.GetFirstArrivalTimeInSamples(true);
#endif

    Aurora::SampleCount d = track.GetFilteredSamples().Length() - fat;

    if( d < deltaT2)
    {
        return 0.0;
    }

    return  dB(  (m_decays[nCh][fat + deltaT1] - m_decays[nCh][fat + deltaT2]) 
               / (m_decays[nCh][fat] - m_decays[nCh][fat + delta_10]));
}

double Aurora::AcousticalParameters::Strength(const int nCh, const float fcb) 
{
   // StrenGth: It's given by the subtraction from Signal of the 
   //           10 m reference value (function of frequency)
   return m_results[nCh].Get("Signal", fcb).value - m_referenceLevels.GetValue(fcb);
}

void Aurora::AcousticalParameters::PreProcessIntensimetricSignal(Aurora::AcParametersAudioTrack& left)
{
    if (m_results.size() < 2)
    {
        return;
    }

    auto& right = m_aAudioTracks[CH_RIGHT];

    const float dt = 1.0f / right.GetRate();  // sampling time interval in seconds
    const float factor = m_dbSoundSpeed * dt / m_dbProbeMicsDistance;

    // Copy Ir to IrFiltered: this is the work buffer. (MAYBE UNUSEFUL: check program flux !!)
    left.ResetFilteredTrack();
    right.ResetFilteredTrack();

    // Now applying band pass filtering
    left.LFilter();
    right.LFilter();

    Aurora::Sample *lsamples = left.Filtered().Samples();
    Aurora::Sample *rsamples = right.Filtered().Samples();
    float velocity = 0;

    // Evaluating pressure (p) and velocity (v)
    for(Aurora::SampleCount i=0; i< left.GetLength(); i++)
    {
        // dbPressure = (p1+p2)/2
        const float pressure  = 0.5 * (lsamples[i] + rsamples[i]);
        // dbVelocity = Integral((p2-p1)/d*c*dbDt) - Euler's law
        velocity += factor* (rsamples[i] - lsamples[i]);

        lsamples[i] = pressure; // left  = dbPressure
        rsamples[i] = velocity; // right = dbVelocity
    }

    // Now removing DC component
    left.RemoveMean();
    right.RemoveMean();

    // Re-applying 20-20kHz band pass filtering
    left.LFilter();
    right.LFilter();

    // Copy back processed Irs.
    left.ReplaceTrackWithFiltered();
    right.ReplaceTrackWithFiltered();
}

bool Aurora::AcousticalParameters::CalculateAcousticParameters()
{
    auto& left = m_aAudioTracks[CH_LEFT];
    size_t nbands = m_results[0].Frequencies().size() + 2;

    //Create progress bar.
    int nProg = 0;
    ProgressMeterWrapper::Show("Computing ISO 3382 Acoustical Parameters...",
                               1 + int(2 * nbands * m_results.size()));
    //PP probe filtering, mono & stereo

    // Intensimetric Probe (P-P) elaboration
    if (IsAnIntensityProbe())  // PP probe
    {
        PreProcessIntensimetricSignal(left);
    }

    ProgressMeterWrapper::Update(++nProg);

    //First Arrival Time search: start with left channel (is this the correct place??)
    size_t nCh = 0;

    for (auto& res : m_results)
    {
        // With multirate filtering code, this doesn't work very well [SC]
        if (!m_aAudioTracks[nCh].FindFirstArrivalTime(m_dbDirectSoundTrig))
        {
            ShowErrorMessage(ERR_NO_FAT);
            ProgressMeterWrapper::Destroy();
            return false;
        }

        auto stype = Aurora::SpectrumType::Octave;

        if (m_octaveFraction == 3)
        {
            stype = Aurora::SpectrumType::ThirdOctave;
        }
        res.SetSpectrumType(stype);
        ++nCh;
    }

#ifdef MULTIRATE_FILTERING

    for (nCh = 0; nCh < m_results.size(); nCh++)
    {
        const double fs = m_aAudioTracks[nCh].GetRate();

        if (m_octaveFraction == 3)
        {
            m_vOba.emplace_back(new SCThirdOctaveBandAnalysis(fs));
        }
        else
        {
            m_vOba.emplace_back(new SCOctaveBandAnalysis(fs));
        }
        m_vOba[nCh]->Set(&(m_aAudioTracks[nCh]));
    }
#endif

    // Noise correction and acoustical parameters calculation for bands 1-10 

    // Definition of the delay for current octave filter group i is:
    // d_i = d_0/2^i
    // where d0 = 50 ms @ 31.5 Hz
    double di = (double)(nbands - 3); 
    int group = m_octaveFraction;

    std::vector<float> fcbs = m_results[0].Frequencies();
    fcbs.push_back(Aurora::AcParametersSpectrum::Band::AweightedBin());
    fcbs.push_back(Aurora::AcParametersSpectrum::Band::LinearBin());

    // -------------------- Parameters loop ----------------------
    // The loop is reversed and with inner channels loop to speedup
    // a bit the calculations when multirate filter is selected.
    for (auto it = fcbs.rbegin(); it != fcbs.rend(); ++it)
    {
        const float fcb = *it;
        Aurora::AcParametersSpectrum::Band band(fcb);
        nCh = 0;

        for (auto& track : m_aAudioTracks)
        {
            // Apply gain to mIrFiltered (to be verified..... = 22.2 dB ???)
            const double gain = (fcb > 10.0f && fcb < 22000.0f) ? 12.8825
                                                                : 1.0;

            // if(White2Pink)
            //    gain *= pow(10.0, ( (5.0 - g_adbOctaveBandFcb[m_nActiveBand]) * 3.0102999566398119521373889472449)/20 );

            double delay = 0.0;

#ifdef MULTIRATE_FILTERING

            //Applying appropriate filter
            if (band.IsAweighted())
            {
                track.ResetFilteredTrack(gain);
                track.AFilter();
            }
            else if (band.IsLinear())
            {
                track.ResetFilteredTrack(gain);
                track.LFilter();
            }
            else
            {
                if (m_vOba[nCh]->Filter() < 0)
                {
                    wxLogMessage("Filtering failed!");
                    continue;
                }

#if 1
                delay = 0.05 / std::pow(2.0, di);
#else
                for (int i = 1; i <= nBand; i++)
                {
                    delay *= 0.5;  // for every octave divide by two
                }
#endif
            }

#else
            track.ResetFilteredTrack(gain);

            //Applying appropriate filter
            if (band.IsAweighted())
            {
                track.AFilter();
            }
            else if (band.IsLinear())
            {
                track.LFilter();
            }
            else
            {
                track.OctaveFilter(fcb);

                // group delay: 50 ms for 31.5 band, then for every octave
                //              band divide by two
                delay = 0.05 / std::exp2(di);
                printf("group delay for %f: %f (%f)\n", fcb, delay, di);
            }
#endif

            //Subctracting mean value
            track.RemoveMean();

            // If is the case, do noise correction factor evaluation
            // otherwise, set it to 0.0
            if (m_flags.noiseCorrectionEnabled)
            {
                track.NoiseCorrection();
            }
            // Schroeder Integral calculation
            m_decays[nCh].DoBackwardIntegration(track);

            CalculateBandMonoParameters(int(nCh), fcb, delay);

            // Update progress meter
            if (! ProgressMeterWrapper::Update(++nProg))
            {
                ProgressMeterWrapper::Destroy();
                return false;
            }

            // Evaluating Acoustic Parameters for the current band (remember: band 11 is WIDE)
            if ((   IsStereo()
                 || IsABinauralPair()
                 || IsSoundfieldWY()
                 || IsFuMa()
                 || IsAmbix()) && nCh == 1)
            {
                CalculateSpatialParameters(fcb, delay);
            }
            // Update progress meter
            if (! ProgressMeterWrapper::Update(++nProg))
            {
                ProgressMeterWrapper::Destroy();
                return false;
            }

            ++nCh;
        } // channels' for

        // update exponent for delay calculation.
        if (! band.IsAverage())
        {
            --group;

            if (group == 0)
            {
                di -= (double)m_octaveFraction;
                group = m_octaveFraction;
            }
        }

    } // bands' while 

    if (m_flags.averageEnabled)
    {
        for (int nCh = 0; nCh < (int)m_aAudioTracks.size(); ++nCh)
        {
            // If m_flags.averageEnabled is set, every Band A parameter value is
            //  the mean of bands 250 to 2000 Hz parameters values
            CalculateBandAParametersAvg(nCh);
            continue;
        }
    }

    ProgressMeterWrapper::Destroy();

    //   PostProcess();
    return true;
}


void Aurora::AcousticalParameters::CalculateBandMonoParameters(const int nCh,
                                                               const float fcb,
                                                               const double delay)
{
    Aurora::AcParametersAudioTrack& track = m_aAudioTracks[nCh];
    
    Aurora::SampleCount delta_50 = track.TimeToSamples(0.050 + delay);  // 50 ms + delay delta
    Aurora::SampleCount delta_80 = track.TimeToSamples(0.080 + delay);  // 80 ms + delay delta
    
#ifdef MULTIRATE_FILTERING
    Aurora::SampleCount fat = m_decays[ch].GetFirstArrivalTimeInSamples();
#else
    Aurora::SampleCount fat = track.GetFirstArrivalTimeInSamples(true);
#endif

    const double  fullScaledB = track.GetFullScale();
    const double sampleratedB = dB(track.GetFilteredSamples().GetRate());

    // 0: Signal
    // The result is a SEL - FS (= m_dbFullScale [dB]) diluted on 1 s.
    m_results[nCh].Set("Signal", 
                       fcb,
                       dB(m_decays[nCh][0]) + fullScaledB - sampleratedB);

    // 1: Noise
    if(track.GetNoiseCorrFactor() > 0.0)
    {
        // m_adbNoiseCorrFactor is a Leq, so it has to be multiplied by the number of samples, then
        // diluted on 1 s.
        m_results[nCh].Set("Noise", 
                           fcb,
                           dB(track.GetNoiseCorrFactor() * m_decays[nCh].GetLength()) 
                           + fullScaledB - sampleratedB);
    }
    else
    {
        m_results[nCh].Set("Noise", fcb, 0.0);
    }
    //----------------------------------------------------------------------!!!!

    // 2: strength;
    m_results[nCh].Set("strenGth", fcb, Strength(nCh, fcb));

    if( (m_decays[nCh].GetLength() - fat) > delta_50 )
    {
        // 3: C50, needs IR lenght of 50 ms (starting from FAT)
        m_results[nCh].Set("C50", fcb, Clarity(nCh, 0.05, delay));

        // 5: D50, idem c.s.
        m_results[nCh].Set("D50", fcb, Definition(nCh, 0.05, delay));
    }

    // 4: C80, needs IR lenght of 80 ms (starting from FAT)
    if( (m_decays[nCh].GetLength() - fat) > delta_80 )
    {
        m_results[nCh].Set("C80", fcb, Clarity(nCh, 0.08, delay));
    }

    // 6: TS, it exist always (N.B. in ms, not in s!!!)  
    double value = m_decays[nCh].GetTP2() * 1000.0 / m_decays[nCh][fat];

    m_results[nCh].Set("Ts", fcb, value, value > 0);

    //7: EDT - 10dB
    value = ReverberationTime(nCh, 0.1, 10.0);
    
    m_results[nCh].Set("EDT", fcb, value, value > 0.0);

    // If m_flags.stageEnabled is true, evaluate Stage Parameters, that subsitutes
    //  D50, Ts and EDT
    if(m_flags.stageEnabled) 
    {
        //5->14: ST1
        value = Stage("St1", nCh, delay);
        
        m_results[nCh].Set("St1", fcb, value, value != 0.0);

        //6->15: ST2 ->
        value = Stage("St2", nCh, delay);
        
        m_results[nCh].Set("St2", fcb, value, value != 0.0);

        //7->16: STL
        value = Stage("StLate", nCh, delay);
        
        m_results[nCh].Set("StLate", fcb, value, value != 0.0);
    }

    //8: RT-USER
    value = ReverberationTime(nCh, std::fabs(m_dbUserMinLevel),
                              std::fabs(m_dbUserMaxLevel - m_dbUserMinLevel));
    m_results[nCh].Set("Tuser", fcb, value, value > 0.0);

    //9: RT-20dB
    value = ReverberationTime(nCh, 5.0, 20.0);
    
    m_results[nCh].Set("T20", fcb, value, value > 0.0);

    //10: RT-30dB
    value = ReverberationTime(nCh, 5.0, 30.0);
    
    m_results[nCh].Set("T30", fcb, value, value > 0.0);

    // 11: Peakiness calculation (only if mono or 2 indep. omni input signal)
    m_results[nCh].Set("Peakiness", fcb,  Peakiness(nCh));

    // 12: Millisecondness calculation (only if mono or 2 indep. omni input signal)
    double impulsivs = 0.0;
    value = Millisecondness(nCh, &impulsivs);
    
    m_results[nCh].Set("Millisecondness", fcb, value,     value > -200.0);
    m_results[nCh].Set("Impulsiveness",   fcb, impulsivs, value > -200.0);
}

bool Aurora::AcousticalParameters::CalculateSpatialParameters(const float fcb, 
                                                              const double delay)  
{
   double widthIACC, tauIACC;
   Aurora::SampleCount delta_80 = TimeToSamples(CH_LEFT, 0.080 + delay);

   Aurora::SampleCount ld = LengthFromFAT(CH_LEFT);
   Aurora::SampleCount rd = LengthFromFAT(CH_RIGHT);

   //For all these calcules, need Ir length of almost 80 ms (from FAT)
    if( (ld  < delta_80) && (rd < delta_80) )
    {
        ShowErrorMessage(ERR_SPATIAL_IR_TOO_SHORT);
        return false;
    }

    switch (m_nSource)
    {
        case Aurora::AcousticalParameters::Source::SoundfieldWY:
        case Aurora::AcousticalParameters::Source::PuProbe:
        case Aurora::AcousticalParameters::Source::IntensityProbe:
        case Aurora::AcousticalParameters::Source::AmbisonicsFuMa:
        case Aurora::AcousticalParameters::Source::AmbisonicsAmbix:
        {
            // 20: Lateral Fraction, needs IR lenght of 80 ms (starting from FAT)
            double value = EarlyLateralFraction(delay);
            m_results[CH_LEFT].Set("Jlf",  fcb, value);
            m_results[CH_RIGHT].Set("Jlf", fcb, value);

            // 21: LFC, needs at least 80ms IR lenght starting from FAT
            value = EarlyLateralFractionCosine(delay);
            m_results[CH_LEFT].Set("Jlfc", fcb, value);
            m_results[CH_RIGHT].Set("Jlfc", fcb, value);

            // 22: LG, needs at least 80ms IR lenght starting from FAT
            value = LateLateralSoundLevel(fcb, delay);
            m_results[CH_LEFT].Set("Lj", fcb, value);

            // *****FIXIT**** in the soundfield case it needs a correction***********FIXIT*****
            m_results[CH_RIGHT].Set("Lj", fcb, value);
            break;
        }

        case Aurora::AcousticalParameters::Source::Binaural:
        {
            // IACC=17, Tau IACC=18, w IACC=19 evaluation
            // 14, etc., needs at least 80ms IR lenght starting from FAT
            double value = InterAuralCrossCorrelation(delay, &tauIACC, &widthIACC);
            m_results[CH_LEFT].Set("IACC",      fcb, value,     widthIACC > 0.0);
            m_results[CH_LEFT].Set("tauIACC",   fcb, tauIACC,   widthIACC > 0.0);
            m_results[CH_LEFT].Set("widthIACC", fcb, widthIACC, widthIACC > 0.0);
            
            m_results[CH_RIGHT].Set("IACC",      fcb, value,     widthIACC > 0.0);
            m_results[CH_RIGHT].Set("tauIACC",   fcb, tauIACC,   widthIACC > 0.0);
            m_results[CH_RIGHT].Set("widthIACC", fcb, widthIACC, widthIACC > 0.0);
            break;
        }
    }
    return true;
}

void  Aurora::AcousticalParameters::CalculateBandAParametersAvg(int nCh)
{
    // Calculate Band A parameters as the mean of 250 to 2000 Hz
    // bands parameters.
    for (auto& pkey : m_results[nCh].Parameters())
    {
        auto& ps = m_results[nCh].GetSpectrum(pkey);

        Aurora::AcousticalParameters::TResults::TParameter avg = 0.0;
        double count = 0.0;

        ps.ForEach(false,
                   [&](const size_t i,
                       const float fcb,
                       Aurora::AcousticalParameters::TResults::TParameter& par)
        {
            if (fcb >= 250.0f && fcb <= 2000.0f)
            {
                avg.value += par.isValid ? par.value : 0.0;
                count += 1.0;

                if (! par.isValid)
                {
                    avg.isValid = false;
                }
            }
        });
        avg.value /= count; // 4 if octave bands

        ps.SetAverageValue("A", avg);
    }
}     


//------------------------- Class user interface -------------------------   
// --- 2nd part: get acustic parameters values (post processing)

void Aurora::AcousticalParameters::SetDirectSoundTrigValue(const double trig) 
{
    if (trig > -1.0) 
    { 
        m_dbDirectSoundTrig =  -1.0; 
    }
    else if (trig < -60.0) 
    { 
        m_dbDirectSoundTrig = -60.0; 
    }
    else
    {
        m_dbDirectSoundTrig = trig;
    }
}

void Aurora::AcousticalParameters::UpdateFilteredTrack(const int nChnl,
                                                       const float fcb)
{
#ifdef MULTIRATE_FILTERING
    Aurora::AcParametersSpectrum::Band band(fcb);

    if(! band.IsAverage())
    {
        m_vOba[nChnl]->FilterForBand(fcb); 
    }
    else
    {
        m_aAudioTracks[nChnl].ApplyFilter(fcb);
    }
#else
	m_aAudioTracks[nChnl].ApplyFilter(fcb);
#endif

    if(m_flags.noiseCorrectionEnabled)
    {
        m_aAudioTracks[nChnl].NoiseCorrection();
    }
    else
    {
        m_aAudioTracks[nChnl].GetNoiseCorrFactor() = 0.0;
    }

    m_decays[nChnl].DoBackwardIntegration(m_aAudioTracks[nChnl]);
}

double Aurora::AcousticalParameters::GetFullScale() const
{
    return m_aAudioTracks[0].GetFullScale();
}

void Aurora::AcousticalParameters::SetFullScale(const double fs)
{
    for (auto& track : m_aAudioTracks)
    {
        track.SetFullscale(fs);
    }
}

void Aurora::AcousticalParameters::ReplaceGReferenceValues()
{
    auto& signal = m_results[CH_LEFT].GetSpectrum("Signal");
    
    signal.ForEach(true,
                   [&](const size_t i, 
                       const float fcb,
                       Aurora::AcousticalParameters::TResults::TParameter& par)
    {
        if (par.isValid)
        {            
            m_referenceLevels.SetValue(fcb,
                                       par.value
                                       + dB20(m_dbSoundSourceDistance / 10.0));
        }
        
        //Update parameters table
        for (size_t ch = 0; ch < m_results.size(); ++ch)
        {
            m_results[ch].Set("strenGth", fcb, Strength(int(ch), fcb));
        }
    });
}

void Aurora::AcousticalParameters::RestoreDefaultReferenceLevels()
{
     m_referenceLevels.ForEach(false,
                               [&](const size_t i, const float fcb, double& value)
    {
        value = 69.0;   // Source ref. lev. @ 10m - Lw=100 dB
    });
     
    // Source ref. lev. @ 10m - Lw=110 dB(Lin)
    m_referenceLevels.SetAverageValue("Lin", 79.0);
    
    // Source ref. lev. @ 10m - Lw=108 dB(A)
    m_referenceLevels.SetAverageValue("A",   77.0);
    
    // Update parameters table
    const auto& frqs = m_referenceLevels.Frequencies();
    
    for (auto& fcb : frqs)
    {
        for (int ch = 0; ch < (int)m_results.size(); ++ch)
        {
            m_results[ch].Set("strenGth", fcb, Strength(ch, fcb));
        }
    }
}


void Aurora::AcousticalParameters::SetOctaveFraction(const int fraction)
{
    assert(fraction == 1 || fraction == 3);
    m_octaveFraction = fraction;
    
    Aurora::SpectrumType spectrumType = Aurora::SpectrumType::Octave;
    
    if (m_octaveFraction == 3)
    {
        spectrumType = Aurora::SpectrumType::ThirdOctave;
    }

    for (auto& r : m_results)
    {
        r.SetSpectrumType(spectrumType);
    }
    m_referenceLevels.SetType(spectrumType);
}

bool Aurora::AcousticalParameters::Init()
{
    const size_t nchannels = m_aAudioTracks.size();

    if(nchannels != m_results.size())
    {
        m_results.resize(nchannels);
        m_decays.resize(nchannels);

        SetOctaveFraction(1);

        // ------------- Initialize strength reference levels vector
        RestoreDefaultReferenceLevels();
    }
    return true;
}

void Aurora::AcousticalParameters::Destroy()
{
#ifdef MULTIRATE_FILTERING
    /* not needed anymore TBR
    if(m_vOba.size() > 0)
    {
        for(int nCh = 0; nCh < m_nChnlsCount; nCh++)
        {
            delete m_vOba[nCh];
        }
    }*/
#endif

}


Aurora::AcousticalParameters::~AcousticalParameters()
{
    Destroy();
}


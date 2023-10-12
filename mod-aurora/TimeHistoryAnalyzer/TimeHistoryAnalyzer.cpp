/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  afthan.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::TimeHistoryAnalyzer
\brief The Aurora's powerful analyzer of time history data

  blabla
*//*******************************************************************/

#include <aurora.h>
#include <widgets/ProgressDialog.h>

#include "TimeHistoryAnalyzer.h"

//----------------------------------------------------------------------------
// Aurora::TimeHistoryAnalyzer::TResults implementation
//----------------------------------------------------------------------------
void Aurora::TimeHistoryAnalyzer::TResults::ResizeBuffers(const size_t newLength)
{  
    for (auto& b : m_aBuffers)
    {
        b.resize(newLength);
    }
}

Aurora::TimeHistoryAnalyzer::TResults& Aurora::TimeHistoryAnalyzer::TResults::operator=(const TResults& r)
{
    m_aParameters = r.m_aParameters;
    m_aBuffers    = r.m_aBuffers;
    return *this;
}

Aurora::TimeHistoryAnalyzer::TResults::TResults(const TResults& r) 
{
    m_aParameters = r.m_aParameters;
    m_aBuffers    = r.m_aBuffers;
}

Aurora::TimeHistoryAnalyzer::TResults::TResults(const size_t buffersLength)
{
    ResizeBuffers(buffersLength);
}

void Aurora::TimeHistoryAnalyzer::TITUFactors::SetAsAfactors(const double ts, 
                                                             const double rate)
{
    // Ag (cost. tempo ITU P56  dTau = 30 ms)
    m_dbG = std::exp(-1.0 / (0.03 * rate));

    // Fattore A per Fast = 0.125 s
    m_dbFast = std::exp(-ts / 2.0 / 0.125) * ts / 0.125;

    // Fattore A per Slow = 1 s
    m_dbSlow = std::exp(-ts / 2.0 / 1.0) * ts / 1.0;

    // Fattore A per Impulse crescente
    m_dbImpRaising = std::exp(-ts / 2.0 / 0.035) * ts / 0.035;

    // Fattore A per Impulse decrescente
    m_dbImpFalling = std::exp(-ts / 2.0 / 1.5) * ts / 1.5;
}

void Aurora::TimeHistoryAnalyzer::TITUFactors::SetAsBfactors(const double ts, 
                                                             const double rate)
{
    SetAsAfactors(ts, rate);
    m_dbG    = 1.0 - m_dbG;
    m_dbFast = 1.0 - m_dbFast;
    m_dbSlow = 1.0 - m_dbSlow;
    m_dbImpRaising = 1.0 - m_dbImpRaising;
    m_dbImpFalling = 1.0 - m_dbImpFalling;
}

//----------------------------------------------------------------------------
// Aurora::TimeHistoryAnalyzer implementation
//----------------------------------------------------------------------------
double Aurora::TimeHistoryAnalyzer::Average(const int nParam)
{
   double sum = 0.0;
   
   for(size_t nCh = 0; nCh < m_aSignalTracks.size(); nCh++)
   {
      sum += m_aResults[nCh].GetParameter(nParam);
   }
   return sum / double(m_aSignalTracks.size());
}

double Aurora::TimeHistoryAnalyzer::dBAverage(const int nParam)
{
   double sum = 0.0;

   for(size_t nCh = 0; nCh < m_aSignalTracks.size(); nCh++)
   {
      sum += undB(m_aResults[nCh].GetParameter(nParam));
   }
   return dB(sum/double(m_aSignalTracks.size()));
}

void  Aurora::TimeHistoryAnalyzer::FillAvgParameters()
{
    const size_t channelsCount = m_aSignalTracks.size();

    if(channelsCount > 1)
    {
    	auto&  avgParams = m_aResults[channelsCount].m_aParameters;
    	double totalDuration = m_aResults[0].GetParameter(THA_TDU);

        // AVG data is in the last place of m_aparams vector
        avgParams[THA_AVG] = dBAverage(THA_AVG);
        avgParams[THA_SEL] = dBAverage(THA_AVG) + dB(totalDuration);
        avgParams[THA_TDU] = totalDuration;
        avgParams[THA_ASL] = dBAverage(THA_ASL);
        avgParams[THA_THS] = Average  (THA_THS);
        avgParams[THA_ACT] = Average  (THA_ACT);
        avgParams[THA_MPK] = dBAverage(THA_MPK);
        avgParams[THA_MIM] = dBAverage(THA_MIM);
        avgParams[THA_MFS] = dBAverage(THA_MFS);
        avgParams[THA_MSL] = dBAverage(THA_MSL);
        avgParams[THA_DUR] = Average  (THA_DUR);

        for(size_t nCh = 0; nCh < channelsCount; nCh++)
        {
            if(m_aResults[nCh].GetParameter(THA_IMP) > 0)
            {
               avgParams[THA_IMP] =  1.0;
            }
        }
    }
}

bool Aurora::TimeHistoryAnalyzer::Analyze()
{
    if(m_aSignalTracks.empty())
    {
        return false;
    }
    Init();

    const int channelsCount = (int)m_aSignalTracks.size();
    
    std::vector<int> ranges = { channelsCount, 100 };
    ProgressMeterWrapper::Show("Computing Time History statistics...", ranges);
    //InitProgressMeter(channelsCount);

    for(int ch = 0; ch < channelsCount; ch++)
    {
        if(!AnalyzeTrack(ch))
        {
            ProgressMeterWrapper::Destroy();
            return false;
        }
    }
    FillAvgParameters();
    ProgressMeterWrapper::Destroy();

    return true;
}

bool Aurora::TimeHistoryAnalyzer::AnalyzeTrack(const int nCh)
{
    auto& track = m_aSignalTracks[nCh];

    // Code by A.Farina
   
    // ********************* If requested, remove DC component ********************
    if(m_bRemoveDC)
    {
       track.RemoveMean();
    }

    // *************************** Apply selected filter ***************************
   track.Filter();

    const double rate = track.GetRate();

    const Aurora::SampleCount trackLength = track.Length();
    // N. di campioni che fanno circa 1ms
    const Aurora::SampleCount stepLength  = Aurora::SampleCount(rate/1000.0);
    // Numero di step
    Aurora::SampleCount  stepsCount  = trackLength/stepLength;

    if((stepsCount*stepLength) > trackLength)
    {
        stepsCount -= 1;  // It prevents overflow
    }   

    // Lunghezza esatta di uno step, in s
    const double ts = stepLength/rate;  

    TITUFactors ituA;
    ituA.SetAsAfactors(ts, rate);

    TITUFactors ituB;
    ituB.SetAsBfactors(ts, rate);

    TITULevels ituL;

    long   iFast=0;

    Aurora::SampleCount j;

    // preparazione vettori per ITU P56
    double levelThresholds         [30]; //soglie di livello
    size_t samplesOverThreshold    [30]; //numero di campioni oltre la soglia
    size_t lastSampleOverThreshold [30]; //memoria dall'ultimo campione sopra soglia

    const size_t unHangoverLength = unRound(0.2 / ts); //hangover time in samples, rounded (H = 0.2)

    levelThresholds        [0] = 1.0/M_SQRT2;
    samplesOverThreshold   [0] = 0;
    lastSampleOverThreshold[0] = unHangoverLength;

    for(j = 1; j < 30; j++)    //scendo a passi di circa 3 dB
    {    
        levelThresholds        [j] = levelThresholds[j-1]/M_SQRT2;
        samplesOverThreshold   [j] = 0;
        lastSampleOverThreshold[j] = unHangoverLength;
    }

    // ******************************** Parte 1 ********************************

    // inizializzo a zero il progress meter
    wxString msg;
    msg.Printf("Analyzing channel %d Time History...", nCh + 1);
    ProgressMeterWrapper::SetMessage(msg);
    ProgressMeterWrapper::Update(nCh, 0);
    ProgressMeterWrapper::SetRange((int)stepsCount, 1);
 
    Aurora::SampleCount step = 0, un = 0;
    double rmsOn1ms = 0.0;
    double max      = 0.0;
    double leq      = 0.0;
    double totalSum = 0.0;
    double envp     = 0.0;

    auto& buffers = m_aResults[nCh].m_aBuffers; // Just for better readability...

     // inizio il ciclo con step 1ms, parto da 0
    for(step = 0; step < stepsCount; step++)
    {
        rmsOn1ms = 0.0;
        double localMax = 0.0;
        double       pi = 0.0;

        for(un = 0; un < stepLength; un++)   //ciclo interno sul blocchetto di 1ms
        {
            const double sample    = track [un + step*stepLength]; // campione corrente
            const double sqrSample = sample*sample;                // campione al quadrato
            rmsOn1ms  += sqrSample;                          // somma dei quadrati

            if(sqrSample > max)
            {
                max = sqrSample; // ricerca del max peak istantaneo
            }

            if(sqrSample > localMax)
            {
                localMax = sqrSample; // ricerca del max peak istantaneo locale
            }
            // valore intermedio
            pi   = ituA.m_dbG * pi   + ituB.m_dbG * std::fabs(sample);
            // Envelope secondo ITU P56
            envp = ituA.m_dbG * envp + ituB.m_dbG * std::fabs(pi);
        }
        // WARNING: maybe swapped???
        rmsOn1ms /= stepLength; // Valore short-Leq di 1ms
        totalSum += rmsOn1ms;   // Accumulo il totale generale per il Leq

        // Ciclo ricerca threshold ITU P56
        for(j = 1; j < 30; j++)
        {
            if(envp < levelThresholds[j])
            {
                if(lastSampleOverThreshold[j] < unHangoverLength)
                {
                    samplesOverThreshold   [j] += 1;
                    lastSampleOverThreshold[j] += 1;
                }
            }
            else
            {
                samplesOverThreshold   [j] += 1;
                lastSampleOverThreshold[j]  = 0;
            }
        }
        // samplesOverThreshold[j] è ora uguale al numero di campioni sopra il
        // threshold j-esimo

        ituL.runFast = ituA.m_dbFast * rmsOn1ms + ituB.m_dbFast * ituL.runFast;
        ituL.runSlow = ituA.m_dbSlow * rmsOn1ms + ituB.m_dbSlow * ituL.runSlow;

        // Costante di tempo Impulse (nuova versione), 
        ituL.run35ms =    ituA.m_dbImpRaising * rmsOn1ms
                        + ituB.m_dbImpRaising * ituL.run35ms; // questo è l'RMS 35ms corrente
        ituL.runImp  = (ituL.run35ms > ituL.runImp) ? ituL.run35ms
                                                    : ituL.runImp * ituB.m_dbImpFalling;

        // memorizzo i valori max Fast, Slow, Impulse
        if (ituL.runFast > ituL.maxFast) 
        { 
            ituL.maxFast = ituL.runFast;
            iFast = step; 
        }
        
        if (ituL.runSlow > ituL.maxSlow)   
        {
            ituL.maxSlow = ituL.runSlow;
        }
        
        if (ituL.runImp  > ituL.maxImp) 
        {   
            ituL.maxImp  = ituL.runImp;
        }
        
        if (rmsOn1ms > ituL.max1ms) 
        {
            ituL.max1ms  = rmsOn1ms;
        }

        // dbAggiungo ai 5 buffers i valori RMS correnti, già in dB
        buffers[TC_PEAK][step] = dB(localMax);  // memorizzo per il grafico il Peak  corrente
        buffers[TC_RMS] [step] = dB(rmsOn1ms);  // memorizzo per il grafico il valore istantaneo
        buffers[TC_ITU] [step] = dB(envp*envp); // memorizzo per il grafico il valore Envelope ITU
        buffers[TC_FAST][step] = dB(ituL.runFast);     // memorizzo per il grafico il Fast corrente
        buffers[TC_SLOW][step] = dB(ituL.runSlow);     // memorizzo per il grafico il Slow corrente
        buffers[TC_IMP] [step] = dB(ituL.runImp);      // memorizzo per il grafico il Imp  corrente

        // aggiorno il progress meter

        if((step % 100) == 0)   // to speedup not update at every step
        {
            if(! ProgressMeterWrapper::Update((int)step, 1))
            {
                return false;
            }
        }
    } // end of step for
        
    // ******************************** Parte 2 ********************************    
        
    // Converto tutti i risultati in dB
    max            = dB(max);
    ituL.maxFast = dB(ituL.maxFast);
    ituL.maxSlow = dB(ituL.maxSlow);
    ituL.maxImp  = dB(ituL.maxImp);
    ituL.max1ms  = dB(ituL.max1ms);
    leq          = dB(totalSum/trackLength);
    // divido per il n. di blocchetti, ed ho l'energia media

    bool   bIsImpulsive = false;
    double dbPulseDuration = 0.0;
    Aurora::SampleCount leftBound  = 0;
    Aurora::SampleCount rightBound = 0;
    
    // Verifica componente impulsiva (ituL, iFast, stepsCount) -> dbPulseDuration
    // Prima condizione: Lmax,imp - Lmax,slow >= 6 dB
    if((ituL.maxImp - ituL.maxSlow) >= 6.0)
    {        
        // Seconda condizione: dbPulseDuration < 1s
        for(step = iFast; step > 0; step--)          // inizio il ciclo con step 1ms
        {
            if(buffers[TC_FAST][step] < (ituL.maxFast - 10.0))
            {
                leftBound = step;
                break;
            }
        }
        
        for(step = iFast; step < stepsCount; step++)          // inizio il ciclo con step 1ms
        {
            if (buffers[TC_FAST][step] < (ituL.maxFast - 10.0))
            {
                rightBound = step;
                break;
            }
        }
            
        if((leftBound > 0) && (rightBound > 0))
        {
            dbPulseDuration = ts * (rightBound - leftBound - 1);
        }
        
        if(dbPulseDuration < 1.0)
        {
            bIsImpulsive = true;
        }
    }

    // ******************************** Parte 3 ********************************

    // Calcolo Active Speech level        TODO  ActiveSpeechLevel(totalSum, a, c) => (Afin)
    const double M = 15.9; //Threshold standard secondo ITU P56
    double Aj,Cj;
    double Afin = 0, Jfin = 0;
    double Deltaj;
    double Deltajm1 = 50.0;     //tutta l'energia impaccata in un unico campione
    
    for(j=29; j>0; j--)         // inizio il ciclo sui 50 possibili valori di Threshold
    {
        if(samplesOverThreshold[j] == 0)
        {
            samplesOverThreshold[j] = 1;
        }
        
        Aj = dB(totalSum/samplesOverThreshold[j]);
        Cj = dB20(levelThresholds[j]);
        Deltaj = Aj-Cj;
        
        if(Deltaj <= M) 
        {
            Jfin = j + (M-Deltaj)/(Deltajm1-Deltaj);
            
            Afin =   dB(totalSum / samplesOverThreshold[j]) 
                   + (Jfin - j) * (dB(totalSum / samplesOverThreshold[j+1]) 
                   - dB(totalSum / samplesOverThreshold[j]));
            break; // ho trovato la soluzione, esco dal ciclo.
        }
        Deltajm1 = Deltaj;
    }
    
    // ******************************** Parte 4 ********************************
    // Salvataggio Risultati
    auto& params = m_aResults[nCh].m_aParameters;

    const double fullScale = track.GetFullScale();
    const double leqFs     = leq + fullScale; // Leq scaled

    params[THA_FSL] = fullScale;                    // Full Scale level
    params[THA_AVG] = leqFs;                        // Average level
    params[THA_SEL] = leqFs + dB(trackLength/rate); // Single Event level
    params[THA_TDU] = trackLength/rate;             // Total Duration
    params[THA_ASL] = Afin +  fullScale;            // Active Speech level
    params[THA_THS] = Afin +  fullScale - M;        // Threshold level
    params[THA_ACT] = undB(leq-Afin) * 100;         // Activity factor (%)
    params[THA_MPK] = max + fullScale;              // Max Peak level
    params[THA_MIM] = ituL.maxImp  + fullScale;     // MaxSPL Impulse
    params[THA_MFS] = ituL.maxFast + fullScale;     // MaxSPL Fast
    params[THA_MSL] = ituL.maxSlow + fullScale;     // MaxSPL Slow
    params[THA_DUR] = dbPulseDuration;              // Duration of the impulsive event
    params[THA_IMP] = (bIsImpulsive) ? 1.0 : 0.0;   // Impulsive event??

    // ritorno ok
    return true;
}

void Aurora::TimeHistoryAnalyzer::SetChannelsNumber(const int nChannels)
{
    assert(nChannels > 0);
    m_aSignalTracks.resize(nChannels);
}

void Aurora::TimeHistoryAnalyzer::Init()
{
    assert(!m_aSignalTracks.empty());

    m_aResults.resize(m_aSignalTracks.size() + 1);
    int ch = 0;

    for(auto& track : m_aSignalTracks)
    {
        const size_t stepLength  = track.GetRate() / 1000.0;
        m_aResults[ch++].ResizeBuffers(track.Length() / stepLength);
    }
}


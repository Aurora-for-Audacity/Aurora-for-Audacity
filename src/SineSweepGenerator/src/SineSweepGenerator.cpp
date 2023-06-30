/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Generator

  sswgen.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::SineSweepGenerator
\brief The Aurora's generator of logarithmic sine sweep signal

  blabla
*//*******************************************************************/
#include <aurora.h>

#include <widgets/ProgressDialog.h>

#include "SineSweepGenerator.h"

void Aurora::SineSweepGenerator::InitDataBuffers()
{
    m_buffers.clear();
    m_buffers.resize(m_nChnlsCount);

    for (int i = 0; i < m_nFilterChnlIdx; ++i)
    {
        m_buffers[i].Resize(m_buffersLength, false);
    }
    m_buffers[m_nFilterChnlIdx].Resize(m_filterLength, false);

    if (m_bControlPulses)
    {
        m_buffers[m_nPulsesChnlIdx].Resize(m_buffersLength, false);
    }
}

void Aurora::SineSweepGenerator::DeleteDataBuffers()
{
    m_buffers.clear();
    m_buffersLength = 0;
}


int Aurora::SineSweepGenerator::ErrorCheck()
{
    if (   m_dbLowFrq  < 0.0 
        || m_dbHighFrq < 0.0 
        || m_dbLowFrq  >= m_dbHighFrq)
    {
        m_nErrNo = int(ERR_SWEEP_LIMITS);
    }

    if (m_dbSweepDuration <= 0.0)
    {
        m_nErrNo = int(ERR_DURATION);
    }

    if (   m_dbAmplitude < 0.0 
        || m_dbAmplitude > 1.0)
    {
        m_nErrNo = int(ERR_AMPLITUDE);
    }

    if (m_nChnlsCount <= 0)
    {
        m_nErrNo = int(ERR_CHANNELS_N);
    }

    if (m_dbFadeInDuration < 0.0 ||
        m_dbFadeInDuration > m_dbSweepDuration)
    {
        m_nErrNo = int(ERR_FADE_IN);
    }

    if (m_dbFadeOutDuration < 0.0 ||
        m_dbFadeOutDuration > m_dbSweepDuration)
    {
        m_nErrNo = int(ERR_FADE_OUT);
    }

    if (m_dbSilenceDuration < 0.0)
    {
        m_nErrNo = int(ERR_SILENCE);
    }

    if (m_nCycles < 0)
    {
        m_nErrNo = int(ERR_CYCLES_N);
    }

    return m_nErrNo;
}

void  Aurora::SineSweepGenerator::GetErrorMessage(wxString& msg)
{
	switch(m_nErrNo)
	{
	    case Aurora::SineSweepGenerator::ERR_SWEEP_LIMITS:
	    	msg = wxT("Error in sweep limits.\nCheck start and end frequencies.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_DURATION:
	    	msg = wxT("Error in sweep duration: can't be zero or < 0!!");
	        break;

	    case Aurora::SineSweepGenerator::ERR_AMPLITUDE:
	    	msg = wxT("Error in sweep amplitude: must be between 0 and 1.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_CHANNELS_N:
	    	msg = wxT("Error in channels number: must be at least 1.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_FADE_IN:
	    	msg = wxT("Error in fade-in duration: must be shorter than sweep duration and not < 0.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_FADE_OUT:
	    	msg = wxT("Error in fade-out duration: must be shorter than sweep duration and not < 0.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_SILENCE:
	    	msg = wxT("Error in silence duration: can't be < 0.");
	        break;

	    case Aurora::SineSweepGenerator::ERR_CYCLES_N:
	    	msg = wxT("Error in number of cycles: must be positive!");
	        break;
	}

}

bool Aurora::SineSweepGenerator::Generate()
{
    if (m_dbSweepDuration == 0)
    {
        return false;
    }

    // inizializzazione
   const double LL = m_dbSweepDuration * m_dbRate;
   const int l2     = int(m_dbSweepDuration * m_dbRate + 0.5);
   const int l0     = int(m_dbFadeInDuration * m_dbRate);       // primi  samples di fade-in
   const int l1     = l2 - int(m_dbFadeInDuration * m_dbRate);  // ultimi samples di fade-out
   const int l      = int((m_dbSweepDuration + m_dbSilenceDuration)* m_dbRate + 0.5);

   m_buffersLength = Aurora::SampleCount(l * m_nCycles * m_nSweepChnlsCount);
   m_filterLength  = l2;

   InitDataBuffers();
   
    const double w_1            = 2.0 * M_PI * m_dbLowFrq  / m_dbRate;
    const double w_2            = 2.0 * M_PI * m_dbHighFrq / m_dbRate;
    const double deltaOmega     =  w_2 - w_1;
    const double deltaOmegaHalf = (w_2 - w_1) / 2.0;
    const double NOctave        = std::log(w_2/w_1) / std::log(2.0);
    const double N              = LL / NOctave;
    const double w              = 2.0 * M_PI * std::pow(2.0, -NOctave)
                                      * N / std::log(2.0)
                                      * m_dbHighFrq / m_dbRate;
    // ciclo principale
    double a;
    double s = 0, as = 0;
   
    for(int cycle = 0; cycle < m_nCycles; cycle++)
    {
        const double g = m_dbAmplitude * std::pow(10.0,
                                                  m_dbDeltaL * (double)cycle / 20.0);
        for(int i = 0; i < l2; i++)
        {
            const double ii = (double)i;
            a = 1.0;
           
           // ---------------------------------------------------------- fade-in
           if(i < l0)
           {
                double x = ii/double(l0);

                switch(m_nFadeInType)
                {
                    case FT_RECT:
                       break;

                    case FT_HAMMING:
                       a *= 0.53836 - 0.46164 * std::cos(M_PI * x);
                       break;

                    case FT_HANN:
                       a *= 0.5 * (1.0 - std::cos(M_PI * x));
                       break;

                    case FT_QUARTER_SINE:
                       a *= std::sin(x * M_PI / 2.0);
                       break;

                    case FT_LINEAR:
                       a *= x;
                       break;

                    case FT_GAUSSIAN:
                    {
                       const double xx = (x - 1.0) * 4.5;
                       a *= std::exp(-0.5 * xx * xx);
                       break;
                    }
                    case FT_BLACKMANN:
                       a *= 0.42 - 0.5  * std::cos(M_PI * x)
                                 + 0.08 * std::cos(2.0 * M_PI * x);
                       break;

                    case FT_BLACK_HARRIS:
                       a *= 0.3635819 - 0.4891775 * std::cos(M_PI * x)
                                      + 0.1365995 * std::cos(2.0 * M_PI * x)
                                      - 0.0106411 * std::cos(3.0 * M_PI * x);
                       break;

                    case FT_LOG:
                       a *= std::log(1.0 + x * x * (M_E - 1.0));
                       break;
                }
            } // end of fade in
       
            // -------------------------------------------------------- fade-out
            if(i > l1)
            {
                const double x = (double)(l2 - i) / (double)(l2 - l1);

                switch(m_nFadeOutType)
                {
                    case FT_RECT:
                       break;

                    case FT_HAMMING:
                        a *= 0.53836 - 0.46164 * std::cos(M_PI * x);
                        break;

                    case FT_HANN:
                        a *= 0.5 * (1.0 - std::cos(M_PI * x));
                        break;

                    case FT_QUARTER_SINE:
                        a *= std::sin(x * M_PI / 2.0);
                        break;

                    case FT_LINEAR:
                        a *= x;
                        break;

                    case FT_GAUSSIAN:
                    {
                        const double xx = 4.5 * (x - 1.0);
                        a *= std::exp(-0.5 * xx * xx);
                        break;
                    }
                    case FT_BLACKMANN:
                        a *= 0.42 - 0.5  * std::cos(M_PI * x)
                                  + 0.08 * std::cos(2.0 * M_PI * x);
                       break;

                    case FT_BLACK_HARRIS:
                        a *= 0.3635819 - 0.4891775 * std::cos(M_PI * x)
                                       + 0.1365995 * std::cos(2.0 * M_PI * x)
                                       - 0.0106411 * std::cos(3.0 * M_PI * x);
                        break;

                    case FT_LOG:
                        a *= std::log(1.0 + x * x *(M_E - 1.0));
                        break;
                }
            } // end of fade out
        
            // ---------------------------------------------------- Sweep choice
            switch(m_nSweepType)
            {
                case ST_LINEAR:
                    s  = std::sin(ii * (w_1 + deltaOmegaHalf * ii/ LL));
                    as = a*s*g;
                    break;

                case ST_LOG:
                    s   = std::sin(w * (std::pow(2.0, ii /N) - 1.0));
                    as  = a*s*g;
                    s  *= std::pow(2.0, (-NOctave * (LL - ii - 1.0) / LL));
                    break;

                case ST_PINK:
                    s  = std::sin(ii * (w_1 + deltaOmegaHalf * ii / LL));
                    //decrease amplitude with frequency/time
                    as = a*s*g / std::sqrt((w_1 + deltaOmega * ii / LL) / w_1);
                    //Increase amplitude with frequency/time
                    s *=  std::sqrt((w_1 + deltaOmega * ii/LL) / w_1)
                        / std::sqrt(w_2/w_1);
                   break;
            }

            // Store evaluated data
            for(int ch = 0; ch < m_nSweepChnlsCount; ch++)
            {
                // Sweep Multi
                m_buffers[ch][i + l*cycle + l*m_nCycles*ch] = (Aurora::Sample)as;
            }
           
            // filtro inverso
            m_buffers[m_nFilterChnlIdx][l2-1-i] = (Aurora::Sample)s;

        } // next i

        // -------------------- generazione burst per avanzamento tavola rotante
        if(m_bControlPulses)
        {
            int Iniz = l2 + (l-l2)/5;               // Posizionatore ad 1/5 del silenzio
            int ms7  = (int)ceil(0.007 * m_dbRate); // n. campioni che corrisponde a 7 ms
            int Fin  = Iniz + ms7;                  // Durata burst = 7 ms

            if( (Fin > l) || ((l-l2) < ms7) )
            {
                // Praticamente non c'? silenzio, metto il burst negli ultimi 7 ms
                // del segnale
                Iniz = l-ms7;
                Fin  = l;
            }

            for(int k = Iniz; k < Fin; k++)   //burst
            {
                // segnale TTL tutto al FS superiore
                m_buffers[m_nPulsesChnlIdx][k + l*cycle] = 1.0;
            }
        }
     
    } // ---------------- next cycle

    return true;
}

// NOT USED...TBR???
void Aurora::SineSweepGenerator::FillBlock(Aurora::Sample* pData, 
                                           Aurora::SampleCount len, 
                                           Aurora::SampleCount written, 
                                           int nTrack)
{
    m_buffers[nTrack].CopyTo(pData, len, written); 
}

void Aurora::SineSweepGenerator::SetControlPulses(bool bValue)
{ 
    if(m_bControlPulses) 
    {
        // m_bControlPulses is 'true': effect only if value is 'false'
        if(!bValue) 
        {
            m_nPulsesChnlIdx = -1;
            m_nChnlsCount--;
        }
      } 
   else 
   {    
      // m_bControlPulses is 'false': effect only if value is 'true'
      if(bValue) 
      {
          m_nPulsesChnlIdx = m_nFilterChnlIdx + 1;
          m_nChnlsCount++;
      } 
    }
    m_bControlPulses = bValue;
}

void Aurora::SineSweepGenerator::SetSweepChnlsNumber(int nValue)   
{ 
   m_nSweepChnlsCount = nValue;
   m_nFilterChnlIdx   = m_nSweepChnlsCount;

   if(m_bControlPulses) 
   {
       m_nPulsesChnlIdx = m_nFilterChnlIdx + 1;
       m_nChnlsCount    = m_nPulsesChnlIdx + 1;
   }
   else
   {
       m_nChnlsCount = m_nFilterChnlIdx + 1;
   }
}

void Aurora::SineSweepGenerator::SetBuffersLength(Aurora::SampleCount len, 
                                                  const bool bInitBuffers,  
                                                  const int nCh)
{
    m_buffersLength = len;
    // TODO....
}

void Aurora::SineSweepGenerator::Destroy()
{
    DeleteDataBuffers();
}

// --- ctors
Aurora::SineSweepGenerator::~SineSweepGenerator()
{
   DeleteDataBuffers();
}

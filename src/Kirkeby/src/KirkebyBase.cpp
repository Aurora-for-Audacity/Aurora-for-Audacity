/*************************************Comple*********************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  afkirk.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::KirkebyBase
\brief The main Kirkeby class

  Bla bla bla

*//*******************************************************************/
#include <aurora.h>
#include "KirkebyBase.h"


void Aurora::KirkebyBase::InitVectors()
{
    // When this private function is called, input vectors
    // should be already instantiated, but output vectors 
    // should't...

    switch(m_nFilterType)
    {
        case Aurora::KirkebyBase::FilterType::Mono:
            m_colLength = m_rowLength / m_nCols;
            
            m_Hf.Resize(1, 1, 1 + m_colLength / 2);
            m_Ht.Resize(m_nRows, m_nCols, m_colLength);
            break;
            
        case Aurora::KirkebyBase::FilterType::Stereo:
            // Here we've to do a fake: HRL and HRR are obtained
            // by swapping the input IRs, so from 2 IRs, we
            // make 4. For this reason while m_nCols value is 1
            // we have in fact 2 columns...
            m_colLength = m_rowLength;
            
            m_Hf.Resize(m_nRows, 2, 1 + m_colLength/2);
            m_Ht.Resize(m_nRows, 2, m_colLength);
            break;
            
        case Aurora::KirkebyBase::FilterType::Dipole:
            m_nCols = 2;
        case Aurora::KirkebyBase::FilterType::Matrix:
            m_colLength = m_rowLength / m_nCols;
            
            m_Hf.Resize(m_nRows, m_nCols, 1 + m_colLength/2);
            m_Ht.Resize(m_nRows, m_nCols, m_colLength);
            break;
    }
}

void Aurora::KirkebyBase::DeleteVectors()
{
    // TODO should be removed...
}

void Aurora::KirkebyBase::InitFft()
{
    m_fftForwardCfg  = kiss_fftr_alloc((int)m_colLength, 0, NULL, NULL);
    m_fftBackwardCfg = kiss_fftr_alloc((int)m_colLength, 1, NULL, NULL);
}

void Aurora::KirkebyBase::DeleteFft()
{
    if(m_fftForwardCfg)
    {
        kiss_fftr_free(m_fftForwardCfg);
        m_fftForwardCfg = nullptr;
    }

    if (m_fftBackwardCfg)
    {
        kiss_fftr_free(m_fftBackwardCfg);
        m_fftBackwardCfg = nullptr;
    }
}

void Aurora::KirkebyBase::FFT(      Aurora::ComplexSpectrum& out,
                              const Aurora::SamplesVector& in)
{
    kiss_fftr(m_fftForwardCfg, in.CSamples(), out.Samples());
}

void Aurora::KirkebyBase::IFFT(      Aurora::SamplesVector& out, 
                               const Aurora::ComplexSpectrum& in)
{
    kiss_fftri(m_fftBackwardCfg, in.CSamples(), out.Samples());

    // kissFft antitransform needs to be rescaled!
    out /= (float)m_colLength;
}
      

   
void Aurora::KirkebyBase::ApplyGain()
{       
    m_Ht.ForEach([this](Aurora::TSampleVector& cell)
    {
        cell *= (float)m_dbGain;
    });
}

// --------------------------------------------- Farina's

void Aurora::KirkebyBase::ComputeMovingAverage(Aurora::ComplexVectorBase& Hf,
                                              const Aurora::KirkebyBase::ComplexComponent realOrImag)
{
    // [esseci] It joints together myMedia2 and MediaLplus because it processes
    // at the same time real (magnitude) and imaginary (phase) parts.
    
    // A.Farina:
    // differisce da MediaL perche' gestisce "meglio" la media asimmetrica
    // N e' la dimensione effettiva del vettore da mediare, quindi Length/2+1
    // span e' il numero di lineee spettrali su cui si media
    const int length   = (int)Hf.GetLength();
    const int span     = (int)m_dbAvgWidth;
    const int halfSpan = span/2;
    
    Aurora::ComplexVector Hst(length);  // results vector [SC]
    Aurora::ComplexVector temp(span);  // in questo vettore memorizzo gli "span"
                                        // valori per eseguire la media
    int j;
    int i;  //servono per eseguire la media asimmetrica nella prima frazione
            //informazioni per la finestra di Hanning
    
    const float twoPi = 2.0f * M_PI;
    
    //Calcolo la somma dei pesi TODO -> Precalculate Window and window's weights sum !!
    float weightsSum = 0.0f;   //somma dei pesi della finestra

    for(j = 0; j < span; j++)   
    {
        weightsSum += 0.5f * (1.0f - std::cos(twoPi * j / (m_dbAvgWidth - 1.0f)));
                                                       // (span-1)))
    }

    int jc; // centro banda
    
    // 1. jc = 1  ->  half_span (cerco di non mediare la componente DC)
    for(jc = 1; jc <= halfSpan; jc++) 
    { 
        // Reset temporary vector [SC]
        temp.Clear();
        
        //Carico i valori a sinistra di jc
        for(j = (jc - 1), i = 1; j > 0; j--, i++) //tolgo l'uguale, non voglio mediare usando la DC
        {
            Re(temp[halfSpan - i]) = Re(Hf[j]);
            Im(temp[halfSpan - i]) = Im(Hf[j]);
        }

        //carico i valori a destra di jc
        for(j = jc, i = 0; j <= (jc + halfSpan); j++, i++)
        {
            Re(temp[halfSpan + i]) = Re(Hf[j]);
            Im(temp[halfSpan + i]) = Im(Hf[j]);
        }

        //eseguo la finestratura hanning
        for(j = 0; j < halfSpan; j++) 
        {
            // contiene il valore calcolate della finestra nelm punto indicato
            // da wd_pnt
            const float execWd = 0.5f * (1.0f - std::cos(twoPi * j
                                                         / (m_dbAvgWidth - 1.0f)));
                                                        // (span-1)))
            Re(temp[j]) *= execWd;
            Re(temp[span - j - 1]) *= execWd;
            
            Im(temp[j]) *= execWd;
            Im(temp[span - j - 1]) *= execWd;
        }
    
        // average        
        float avgr = 0.0;
        float avgi = 0.0;
        
        for(i = 0; i < span; i++)
        {
            avgr += Re(temp[i]);
            avgi += Im(temp[i]);
        }
        Re(Hst[jc]) = avgr / weightsSum;
        Im(Hst[jc]) = avgi / weightsSum;
    
    } //passo ad un altro jc
    
    // 2. jc = N - half_span -> N ---------------------------------
    for(jc = (length - halfSpan); jc < length; jc++)
    {
        // Reset temporary vector [SC]
        temp.Clear();
        
        //carico i valori
        for(j = (jc - halfSpan), i = 0; j < length; j++, i++)
        {
            Re(temp[i]) = Re(Hf[j]);
            Im(temp[i]) = Im(Hf[j]);
        }

        //eseguo la finestratura
        for(j = 0; j < halfSpan; j++) 
        {   
            // contiene il valore calcolate della finestra nel punto indicato
            // da wd_pnt
            const float execWd = 0.5f * (1.0f - std::cos(twoPi * j
                                                         / (m_dbAvgWidth - 1.0f)));
                                                        // (span-1)))
            Re(temp[j]) *= execWd;
            Re(temp[span - j - 1]) *= execWd;
            
            Im(temp[j]) *= execWd;
            Im(temp[span - j - 1]) *= execWd;
        }
    
        // average
        float avgr = 0.0;
        float avgi = 0.0;
        
        for(i = 0; i < span; i++)
        {
            avgr += Re(temp[i]);
            avgi += Im(temp[i]);
        }
        Re(Hst[jc]) = avgr / weightsSum;
        Im(Hst[jc]) = avgi / weightsSum;

    }  //passo ad un'altro jc

    // 3. jc = half_span -> N-half_span ---------------------------------------
    for(jc = halfSpan; jc < (length - halfSpan); jc++)
    {
        // Reset temporary vector [SC]
        temp.Clear();
        
        //carico i valori che serviranno per il filtraggio TODO -> memcpy???
        for(j = (jc - halfSpan), i = 0; j < (jc + halfSpan); j++, i++)
        {
            Re(temp[i]) = Re(Hf[j]);
            Im(temp[i]) = Im(Hf[j]);
        }

        //adesso uso i valori acquisiti per creare il punto j in uscita
        for(j = 0; j < halfSpan; j++)
        {
            // contiene il valore calcolate della finestra nel punto indicato
            // da wd_pnt
            const float execWd = 0.5f * (1.0f - std::cos(twoPi * j
                                                         / (m_dbAvgWidth - 1.0f)));
                                                        // (span-1)))
            Re(temp[j]) *= execWd;
            Re(temp[span - j - 1]) *= execWd;
            
            Im(temp[j]) *= execWd;
            Im(temp[span - j - 1]) *= execWd;
        } // della finestra
    
        //adesso uso i valori finestrati per ottenere il valore "j-esimo" di uscita
        float avgr = 0.0;
        float avgi = 0.0;
        
        for(i = 0; i < span; i++)
        {
            avgr += Re(temp[i]);
            avgi += Im(temp[i]);
        }
        Re(Hst[jc]) = avgr / weightsSum;
        Im(Hst[jc]) = avgi / weightsSum;

    }//del ciclo su "jc"
    
    // Copy results of computation

    switch (realOrImag)
    {
        case ComplexComponent::Real: // (or Magnitude)
            for (j = 1; j < length; j++)
            {
                Re(Hf[j]) = Re(Hst[j]);
            }
            break;

        case ComplexComponent::Imag: // (or Phase)        
            for (j = 1; j < length; j++)
            {                
                Im(Hf[j]) = Im(Hst[j]);
            }
            break;

        case ComplexComponent::Both:
            Hf.Copy(Hst);
            break;
    }
}

void Aurora::KirkebyBase::ComputeOctaveSmoothing(Aurora::ComplexVectorBase& Hf)
{
    ComputeOctaveSmoothing(Hf, ComplexComponent::Real);
    ComputeOctaveSmoothing(Hf, ComplexComponent::Imag);
}

void Aurora::KirkebyBase::ComputeOctaveSmoothing(Aurora::ComplexVectorBase& Hf,
                                                 const ComplexComponent realOrImag)
{
    const int length = (int)Hf.GetLength();
    Aurora::TSampleVector Hst(length);

    const double smoothFact = m_dbAvgWidth; // ????
    
    double rad2x    = std::sqrt(std::pow(2.0, smoothFact)); // radice quadrata di 2^SmoothFact
    double invRad2x = 1.0 / rad2x;              // 1 / radice quadrata di 2^SmoothFact
    Aurora::Sample hstTot = 0.0; // buffer per le sommatorie
    
    // inizializzo HsTot, Hs[0] e j1o j2o
    Hst[0] = realOrImag == ComplexComponent::Imag ? Hf[0].i 
                                                  : Hf[0].r;

    int j1  = 0;
    int j2  = 0;   // estremi banda
    int j1o = 0;
    int j2o = 0; // estremi banda precedente
    int totjs = 0;   // frequenze presenti in una banda
    
    // ciclo principale (centro banda)
    for(int jc = 1; jc < length; jc++)
    {
        j1 = int(invRad2x * jc + 0.5); // estremo inferiore banda
        j2 = int(rad2x * jc);        // estremo superiore banda
        totjs = j2 - j1 + 1;              // totale frequenze nella banda
        
        // elimino dal totale tutti i termini della banda precedente 
        // a sinistra della banda corrente
        for(int j = j1o; j < j1; j++)
        {
            hstTot -= (realOrImag == ComplexComponent::Imag ? Hf[j].i 
                                                            : Hf[j].r);
        }
        // inserisco nel totale tutti i termini a destra della banda precedente 
        // presenti nella banda corrente        
        for(int j = (j2o+1); j <= j2; j++)
        {
#if 1
            const int idx = (j <= length ? j : 2 *length - j);
            hstTot += (realOrImag == ComplexComponent::Imag ? Hf[idx].i 
                                                            : Hf[idx].r);
#else
            hstTot += (j<=length) ? (bImag ? Hf[j].i : Hf[j].r)
                                  : (bImag ? Hf[2*length-j].i : Hf[2*length-j].r); // N1-(j-N1) = 2*N1-j
#endif                                  
        }   
        // calcolo il valore dell'Hs corrente
        Hst[jc] = hstTot / (Aurora::Sample)totjs;
        
        // aggiorno il valore di j1o e j2o
        j1o = j1;
        j2o = j2;
    }   
    
    // ricopio la risposta in frequenza smoothed da Hs a H
    if (realOrImag == ComplexComponent::Imag)
    {
        for(int j = 1; j < length; j++)
        {        
            Hf[j].i = Hst[j];
        }
    }
    else
    {
        for(int j = 1; j < length; j++)
        {        
            Hf[j].r = Hst[j];
        }
    }
}


void Aurora::KirkebyBase::ComputeAverage(Aurora::ComplexSpectrum& Hf)
{
	switch(m_nAvgMode)
	{
		case Aurora::KirkebyBase::AverageMode::ReIm:
		   //MyMedia2(RE,IM,TLen,TipoFinestraFlag,MediaDim,lpAmp,ci); //Medie Re e IM
            switch(m_nAvgType)
            {
                case Aurora::KirkebyBase::AverageType::Linear:
                    ComputeMovingAverage(Hf);
                    break;

                case Aurora::KirkebyBase::AverageType::Octave:
                    ComputeOctaveSmoothing(Hf);
                    break;
            }

            //ScriviDatiName("media_1.txt",H,TLen,ci,lpAmp); //Scrivo nel chiamante
            break;
            
        case Aurora::KirkebyBase::AverageMode::MagPh:
            //calcolo modulo e fase
            Aurora::PolarComplexVector hfp(Hf);

            //Eseguo l'unwrap della fase
            hfp.UnwrapPhase();

            //Adesso eseguo la media
            //MyMedia2(RE,IM,TLen,TipoFinestraFlag,MediaDim,lpAmp,ci);
            switch(m_nAvgType)
            {
                case Aurora::KirkebyBase::AverageType::Linear:
                    ComputeMovingAverage(hfp);
                    break;

                case Aurora::KirkebyBase::AverageType::Octave:
                    ComputeOctaveSmoothing(hfp);
                    break;
            }

            //Ritorno in coordinate rettangolari
            hfp.ToRectangular(Hf);

            //ScriviDatiName("media_2.txt",H,TLen,ci,lpAmp); //Scrivo nel chiamante
            break;
	}
}

void Aurora::KirkebyBase::ComputeCepstrum(Aurora::ComplexSpectrum& Hf)
{
    //This function shuold do following steps with the input vector (Harray)
    //
    //  1) calculates a new complex vector that has, for each element, 
    //      Re = the logarithm of the magnitude square value
    //           of the input vector.
    //      Im = 0
    Aurora::ComplexSpectrum Uf(Hf.Length());
    
    Aurora::SampleCount i;

    for(i = 0; i < Hf.Length(); i++)
    {
        Uf[i].r = (Hf[i].r == 0) ? std::exp(-32.0f)
                                 : std::log(Hf.Module(i));
    }
    //  2) calculates the IFFT of the vector previously obtained
    Aurora::SamplesVector xt(m_colLength);
    IFFT(xt, Uf);

    // [Now psmpXt contains the cepstrum of pcpxH. But there is more...]
    
    //  3) doubles the amplitude of the transformed (real!) vector (!!)
    for(i = 1; i < (xt.Length()-1); i++) 
    {
        xt[i] *= 2.0;
    }

    //  4) calculates the FFT of the transformed and doubled vector
    FFT(Uf, xt);
    
    //  5) calculates a new complex vector (MParray) with, for each element,
    //      Re = exp(Re(in))*cos(Im(in))
    //      Im = exp(Re(in))*sin(Im(in))
    Aurora::ComplexSpectrum MPf(Hf.Length());
    
    for(i = 0; i < Hf.Length(); i++)
    {
    	 MPf[i].r = std::exp(Uf[i].r) * std::cos(Uf[i].i);
    	 MPf[i].i = std::exp(Uf[i].r) * std::sin(Uf[i].i);
    }
    
    //  6) calculates the IFFT of the vector previously obtained
    IFFT(xt, MPf);  // TODO: not really used...can be removed!
    
    //  7) calculates the EP components as EParray = Harray/MParray
    Aurora::ComplexSpectrum EPf(Hf.Length());
    EPf = Hf / MPf; // Overload power !!
    
    //  8) converts EParray and MParray from rectangular to polar coordinates
    Aurora::PolarComplexVector EPfp(EPf);
    Aurora::PolarComplexVector MPfp(MPf);
    
    //  9) unwraps 'polar' EParray's phase and puts to '0' all MParray elements 
    //     phase component
    EPfp.UnwrapPhase();

    for(i = 0; i < MPf.Length(); i++)  
    {
        MPfp.SetPhas(i, 0.0);
    }

    // 10) averages the unwrapped EParray phase and the MParray magnitude with 
    //     'MediaLplus' or 'OctaveSmoothing', depending by the user choice
    switch(m_nAvgType)
    {
        case AverageType::Linear: 
              ComputeMovingAverage(EPfp, ComplexComponent::Phase);
              ComputeMovingAverage(MPfp, ComplexComponent::Magnitude);
        	  break;
        	  
        case AverageType::Octave: 
              ComputeOctaveSmoothing(EPfp, ComplexComponent::Phase);
              ComputeOctaveSmoothing(MPfp, ComplexComponent::Magnitude);
        	  break;
    }
    
    // 11) converts EParray and MParray from polar to rectangular coordinates
    EPfp.ToRectangular(EPf);
    MPfp.ToRectangular(MPf);
    
    // 12) obtains averaged system response H = MParray * EParray
    Hf = MPf*EPf;
    
    // 13) frees allocated memory
}

void Aurora::KirkebyBase::ComputeKirkebyModulusInversion(Aurora::ComplexSpectrum& Hf)
{
    // input data:  Hf
    // output data: Hf

    //float RegPar=lpAmp->RegParam;   --> m_dbInRP
    //float RegPar2=lpAmp->RegParam2; --> m_dbOutRP
    //float Transwidth=lpAmp->trwid;  --> m_dbTransWidth
    const double halfTw = m_dbTransWidth / 2.0;
    const double length = (double)Hf.Length();

    auto i1a = Aurora::SampleCount(length * m_dbLowCutFrq  * std::exp2(-halfTw)/m_dbRate);
    auto i1b = Aurora::SampleCount(length * m_dbLowCutFrq  * std::exp2( halfTw)/m_dbRate);
    auto i2a = Aurora::SampleCount(length * m_dbHighCutFrq * std::exp2(-halfTw)/m_dbRate);
    auto i2b = Aurora::SampleCount(length * m_dbHighCutFrq * std::exp2( halfTw)/m_dbRate);

    Aurora::ComplexSpectrum Hsf(Hf.Length());

    // DC and FNyquist don't have imaginary part
    Hsf.SetDC     ( Hf.DC() / (Hf.DC() * Hf.DC() + m_dbOutRP) );
    Hsf.SetNyquist( Hf.Ny() / (Hf.Ny() * Hf.Ny() + m_dbOutRP) );

    // Check extremes
    if(i1a < 1) i1a = 1;
    if(i1b < 1) i1b = 1;
    if(i2a < (Hf.Length() - 1)) i2a = (Hf.Length() - 1);
    if(i2b < (Hf.Length() - 1)) i2b = (Hf.Length() - 1);

    // Lower Out band
    Aurora::SampleCount i;

    for(i = 1; i < i1a; i++)
    {
        Hsf[i].r =  Hf[i].r / (Hf.SquareModule(i) + m_dbOutRP);
        Hsf[i].i = -Hf[i].i / (Hf.SquareModule(i) + m_dbOutRP);
    }
    Aurora::Sample rp;

    for(i = i1a; i < i1b; i++)
    {
        rp = expf( logf(m_dbOutRP) + (logf(m_dbInRP)-logf(m_dbOutRP))*(i-i1a)/(i1b-i1a) );
        Hsf[i].r =  Hf[i].r / (Hf.SquareModule(i) + rp);
        Hsf[i].i = -Hf[i].i / (Hf.SquareModule(i) + rp);
    }

    // Inner band
    for(i = i1b; i < i2a; i++)
    {
        Hsf[i].r =  Hf[i].r / (Hf.SquareModule(i) + m_dbInRP);
        Hsf[i].i = -Hf[i].i / (Hf.SquareModule(i) + m_dbInRP);
    }

    // Superior transition band
    for(i = i2a; i < i2b; i++)
    {
        rp = expf( logf(m_dbInRP) + (logf(m_dbOutRP)-logf(m_dbInRP))*(i-i2a)/(i2b-i2a) );
        Hsf[i].r =  Hf[i].r / (Hf.SquareModule(i) + rp);
        Hsf[i].i = -Hf[i].i / (Hf.SquareModule(i) + rp);
    }

    // Lower Out band
    for(i = i2b; i < length; i++)
    {
        Hsf[i].r =  Hf[i].r / (Hf.SquareModule(i) + m_dbOutRP);
        Hsf[i].i = -Hf[i].i / (Hf.SquareModule(i) + m_dbOutRP);
    }

    //Store result
    Hf.Copy(Hsf);
}

void Aurora::KirkebyBase::ComputeKirkebyRatioModulusInversion(Aurora::ComplexSpectrum& Uf,
                                                              Aurora::ComplexSpectrum& Wf)
{
    // input data:  pcpxU and pcpxW
    // output data: pcpxU

    assert(Uf.Length() == Wf.Length());
    
    //float RegPar=lpAmp->RegParam;   --> m_dbInRP
    //float RegPar2=lpAmp->RegParam2; --> m_dbOutRP
    //float Transwidth=lpAmp->trwid;  --> m_dbTransWidth
    const double halfTw = m_dbTransWidth / 2.0;
    const double length = (double)Uf.Length();

    auto i1a = Aurora::SampleCount(length * m_dbLowCutFrq  * std::exp2(-halfTw) / m_dbRate);
    auto i1b = Aurora::SampleCount(length * m_dbLowCutFrq  * std::exp2(halfTw) / m_dbRate);
    auto i2a = Aurora::SampleCount(length * m_dbHighCutFrq * std::exp2(-halfTw) / m_dbRate);
    auto i2b = Aurora::SampleCount(length * m_dbHighCutFrq * std::exp2(halfTw) / m_dbRate);

    Aurora::ComplexSpectrum Hsf(Uf.Length());
    
    // DC and FNyquist don't have imaginary part
    Hsf.SetDC     (Uf.DC() * Wf.DC() /  (Uf.DC() * Wf.DC() + m_dbOutRP));
    Hsf.SetNyquist(Uf.Ny() * Wf.Ny() /  (Uf.Ny() * Wf.Ny() + m_dbOutRP));

    // Check extremes
    if(i1a < 1) i1a = 1;
    if(i1b < 1) i1b = 1;
    if(i2a < (Uf.Length() - 1)) i2a = (Uf.Length() - 1);
    if(i2b < (Uf.Length() - 1)) i2b = (Uf.Length() - 1);

        // Lower Out band
    Aurora::SampleCount i;
    for(i = 1; i < i1a; i++)
    {
        Hsf[i].r = (Uf[i].r * Wf[i].r + Uf[i].i * Wf[i].i) 
                     / (Wf.SquareModule(i) + m_dbOutRP);
        Hsf[i].i = (Uf[i].i * Wf[i].r - Uf[i].r * Wf[i].i) 
                     / (Wf.SquareModule(i) + m_dbOutRP);
    }
    double rp;
    for(i = i1a; i < i1b; i++)
    {
        rp = std::exp(   std::log(m_dbOutRP) 
                      + (std::log(m_dbInRP) - std::log(m_dbOutRP)) * (i-i1a)/(i1b-i1a) );
        Hsf[i].r = (Uf[i].r * Wf[i].r + Uf[i].i * Wf[i].i) 
                     / (Wf.SquareModule(i) + Aurora::Sample(rp));
        Hsf[i].i = (Uf[i].i * Wf[i].r - Uf[i].r * Wf[i].i) 
                     / (Wf.SquareModule(i) + Aurora::Sample(rp));
    }

    // Inner band
    for(i = i1b; i < i2a; i++)
    {
        Hsf[i].r = (Uf[i].r * Wf[i].r + Uf[i].i * Wf[i].i)
                    / (Wf.SquareModule(i) + m_dbInRP);
        Hsf[i].i = (Uf[i].i * Wf[i].r - Uf[i].r * Wf[i].i) 
                    / (Wf.SquareModule(i) + m_dbInRP);
    }

    // Superior transition band
    for(i = i2a; i < i2b; i++)
    {
        rp = std::exp(   std::log(m_dbInRP)
                      + (std::log(m_dbOutRP) - std::log(m_dbInRP)) * (i-i2a)/(i2b-i2a) );
        Hsf[i].r = (Uf[i].r * Wf[i].r + Uf[i].i * Wf[i].i) 
                    / (Wf.SquareModule(i) + Aurora::Sample(rp));
        Hsf[i].i = (Uf[i].i * Wf[i].r - Uf[i].r * Wf[i].i) 
                    / (Wf.SquareModule(i) + Aurora::Sample(rp));
    }

    // Lower Out band
    for(i = i2b; i < length; i++)
    {
        Hsf[i].r = (Uf[i].r * Wf[i].r + Uf[i].i * Wf[i].i) 
                    / (Wf.SquareModule(i) + m_dbOutRP);
        Hsf[i].i = (Uf[i].i * Wf[i].r - Uf[i].r * Wf[i].i) 
                    / (Wf.SquareModule(i) + m_dbOutRP);
    }

    //Store result
    Uf.Copy(Hsf);
}

// -----------------------------------------------------------------------------
// Main functions
// -----------------------------------------------------------------------------
bool Aurora::KirkebyBase::ComputeMonoInverseFilter(Aurora::TSampleVector& Ht)
{
    // 1) Calculate FFT
    FFT(m_Hf(0,0), Ht); 

    // 2) Calculate Average
    switch(m_nAvgMode)
    {
        case Aurora::KirkebyBase::AverageMode::ReIm:
        case Aurora::KirkebyBase::AverageMode::MagPh:
            ComputeAverage(m_Hf(0,0));
            break;

        case Aurora::KirkebyBase::AverageMode::Cepstrum:
            ComputeCepstrum(m_Hf(0,0));
            break;
    } 

    // 3) inversione modulo, trasformazione in numeri complessi (e aggiunta fase lineare)
    ComputeKirkebyModulusInversion(m_Hf(0,0));

    // 4) calculate iFFt
    IFFT(Ht, m_Hf(0,0)); 

    return true;
}


#define Hllf m_Hf(0,0)
#define Hlrf m_Hf(0,1)
#define Hrrf m_Hf(1,1)
#define Hrlf m_Hf(1,0)

bool Aurora::KirkebyBase::ComputeQuadInverseFilter(Aurora::TSampleMatrix& Ht)
{
    FFT(m_Hf(0,0), Ht(0,0)); // Hll
    FFT(m_Hf(0,1), Ht(0,1)); // Hlr
    FFT(m_Hf(1,1), Ht(1,1)); // Hrr
    FFT(m_Hf(1,0), Ht(1,0)); // Hrl

    // 2) Calculate Average
    switch(m_nAvgMode)
    {
        case Aurora::KirkebyBase::AverageMode::ReIm:
        case Aurora::KirkebyBase::AverageMode::MagPh:
            ComputeAverage(m_Hf(0,0)); // Hll
            ComputeAverage(m_Hf(0,1)); // Hlr
            ComputeAverage(m_Hf(1,0)); // Hrl
            ComputeAverage(m_Hf(1,1)); // Hrr            
            break;

        case Aurora::KirkebyBase::AverageMode::Cepstrum:
            ComputeCepstrum(m_Hf(0,0)); // Hll
            ComputeCepstrum(m_Hf(0,1)); // Hlr
            ComputeCepstrum(m_Hf(1,0)); // Hrl
            ComputeCepstrum(m_Hf(1,1)); // Hrr
            break;
    }

    // 3) inversione modulo, trasformazione in numeri complessi (e aggiunta fase lineare)
    Aurora::SampleCount i;
    Aurora::ComplexSpectrum Df;

    if(m_bCancelCrosstalk)
    {
        // inversione con il solo scopo di cancellare il cross-talk, senza equalizzazione
        // Ora calcolo il rapporto n. 1 (filtro Frl=-Hrl/Hll)
        ComputeKirkebyRatioModulusInversion(m_Hf(1,0), m_Hf(0,0)); //Hrl holds result

        for(i=0; i < m_Hf(0,0).Length(); i++)
        {
            m_Hf(0, 0)[i].r = 1.0f;
            m_Hf(0, 0)[i].i = 0.0f; // Hll = 1 + i*0
        }
        // Ora calcolo il rapporto n. 2 (filtro Flr=-Hlr/Hrr)
        ComputeKirkebyRatioModulusInversion(m_Hf(0,1), m_Hf(1,1)); //Hrl holds result

        for(i=0; i < m_Hf(0,0).Length(); i++)
        {
            m_Hf(1, 1)[i].r = 1.0f;
            m_Hf(1, 1)[i].i = 0.0f; // Hrr = 1 + i*0
        }
    }
    else if(m_bActiveNoiseControl)
    {
        // inversione per calcolo filtro inverso ANC per Sasib
        // Ora calcolo il denominatore
        Df.Resize(m_Hf(0,0).Length());

        Df.SetDC     ( m_Hf(0,0).DC()*m_Hf(1,1).DC() - m_Hf(0,1).DC()*m_Hf(1,0).DC() ); // Hll*Hrr - Hlr*Hrl
        Df.SetNyquist( m_Hf(0,0).Ny()*m_Hf(1,1).Ny() - m_Hf(0,1).Ny()*m_Hf(1,0).Ny() ); // Hll*Hrr - Hlr*Hrl

        for(i=1; i < Df.Length()-1; i++)
        {
            // Re(Df) = Re(Hll)*Re(Hrr) - Im(Hll)*Im(Hrr) - Re(Hlr)*Re(Hrl) + Im(Hlr)*Im(Hrl)
            Df[i].r =   m_Hf(0,0)[i].r*m_Hf(1,1)[i].r 
                      - m_Hf(0,0)[i].i*m_Hf(1,1)[i].i 
                      - m_Hf(0,1)[i].r*m_Hf(1,0)[i].r 
                      + m_Hf(0,1)[i].i*m_Hf(1,0)[i].i;
            
            // Im(Df) = Re(Hll)*Im(Hrr) + Im(Hll)*Re(Hrr) - Re(Hlr)*Im(Hrl) - Im(Hlr)*Re(Hrl)
            Df[i].i =   m_Hf(0,0)[i].r*m_Hf(1,1)[i].i 
                      + m_Hf(0,0)[i].i*m_Hf(1,1)[i].r 
                      - m_Hf(0,1)[i].r*m_Hf(1,0)[i].i 
                      - m_Hf(0,1)[i].i*m_Hf(1,0)[i].r;
        }

        // inversione denominatore
        ComputeKirkebyModulusInversion(Df);

        // Hll = Hrr*Df
        m_Hf(0,0).SetDC     (m_Hf(1,1).DC() * Df.DC());
        m_Hf(0,0).SetNyquist(m_Hf(1,1).Ny() * Df.Ny());

        for(i=1; i < Df.Length()-1; i++)
        {
            m_Hf(0,0)[i].r =  m_Hf(1,1)[i].r * Df[i].r 
                            - m_Hf(1,1)[i].i * Df[i].i;
            m_Hf(0,0)[i].i =  m_Hf(1,1)[i].i * Df[i].r 
                            + m_Hf(1,1)[i].r * Df[i].i;
        }
        //pcpxHll holds the result
    }
    else
    {
        // inversione completa alla Kirkeby, cross-talk cancelling ed equalizzazione
        // Ora calcolo il denominatore
        Df.Resize(m_Hf(0,0).Length());

        Df.SetDC     (  m_Hf(0,0).DC()*m_Hf(1,1).DC()
                      - m_Hf(0,1).DC()*m_Hf(1,0).DC() ); // Hll*Hrr - Hlr*Hrl
        Df.SetNyquist(  m_Hf(0,0).Ny()*m_Hf(1,1).Ny() 
                      - m_Hf(0,1).Ny()*m_Hf(1,0).Ny() ); // Hll*Hrr - Hlr*Hrl
        
        for(i=1; i < Df.Length()-1; i++)
        {
            // Re(Df) = Re(Hll)*Re(Hrr) - Im(Hll)*Im(Hrr) - Re(Hlr)*Re(Hrl) + Im(Hlr)*Im(Hrl)
            Df[i].r =   m_Hf(0,0)[i].r * m_Hf(1,1)[i].r
                      - m_Hf(0,0)[i].i * m_Hf(1,1)[i].i 
                      - m_Hf(0,1)[i].r * m_Hf(1,0)[i].r 
                      + m_Hf(0,1)[i].i * m_Hf(1,0)[i].i;
            
            // Im(Df) = Re(Hll)*Im(Hrr) + Im(Hll)*Re(Hrr) - Re(Hlr)*Im(Hrl) - Im(Hlr)*Re(Hrl)
            Df[i].i =   m_Hf(0,0)[i].r * m_Hf(1,1)[i].i 
                      + m_Hf(0,0)[i].i * m_Hf(1,1)[i].r 
                      - m_Hf(0,1)[i].r * m_Hf(1,0)[i].i 
                      - m_Hf(0,1)[i].i * m_Hf(1,0)[i].r;
        }
        
        // inversione denominatore
        ComputeKirkebyModulusInversion(Df);

        Aurora::Sample s = m_Hf(1,1).DC() * Df.DC();
        m_Hf(0,1).SetDC( - m_Hf(0,1).DC() * Df.DC()); // Hlr
        m_Hf(1,0).SetDC( - m_Hf(1,0).DC() * Df.DC()); // Hrl
        m_Hf(1,1).SetDC(   m_Hf(0,0).DC() * Df.DC()); // Hrr
        m_Hf(0,0).SetDC( s * Df.DC()); // Hll

        s = m_Hf(1,1).Ny() * Df.Ny();
        m_Hf(0,1).SetNyquist( - m_Hf(0,1).Ny() * Df.Ny()); // Hlr
        m_Hf(1,0).SetNyquist( - m_Hf(1,0).Ny() * Df.Ny()); // Hrl
        m_Hf(1,1).SetNyquist(   m_Hf(0,0).Ny() * Df.Ny()); // Hrr
        m_Hf(0,0).SetNyquist( s * Df.Ny()); // Hll
        
        Aurora::Sample realL, imagL;
        Aurora::Sample realR, imagR;

        for(i=1; i < Df.Length()-1; i++)
        {
            realL = m_Hf(1,1)[i].r*Df[i].r - m_Hf(1,1)[i].i*Df[i].i; // Hrr
            imagL = m_Hf(1,1)[i].i*Df[i].r + m_Hf(1,1)[i].r*Df[i].i; // Hrr
            realR = m_Hf(0,0)[i].r*Df[i].r - m_Hf(0,0)[i].i*Df[i].i; // Hll
            imagR = m_Hf(0,0)[i].i*Df[i].r + m_Hf(0,0)[i].r*Df[i].i; // Hll
            m_Hf(0, 0)[i].r = realL;
            m_Hf(0, 0)[i].i = imagL;
            m_Hf(1, 1)[i].r = realR;
            m_Hf(1, 1)[i].i = imagR;

            realL = m_Hf(0,1)[i].r*Df[i].r - m_Hf(0,1)[i].i*Df[i].i; // Hlr
            imagL = m_Hf(0,1)[i].i*Df[i].r + m_Hf(0,1)[i].r*Df[i].i; // Hlr
            realR = m_Hf(1,0)[i].r*Df[i].r - m_Hf(1,0)[i].i*Df[i].i; // Hrl
            imagR = m_Hf(1,0)[i].i*Df[i].r + m_Hf(1,0)[i].r*Df[i].i; // Hrl
            m_Hf(0, 1)[i].r = realL;
            m_Hf(0, 1)[i].i = imagL;
            m_Hf(1, 0)[i].r = realR;
            m_Hf(1, 0)[i].r = imagR;
        }
    }

    // 4) calculate iFFt 
    IFFT(Ht(0,0), m_Hf(0,0)); // Hll
    IFFT(Ht(0,1), m_Hf(0,1)); // Hlr
    IFFT(Ht(1,1), m_Hf(1,1)); // Hrr
    IFFT(Ht(1,0), m_Hf(1,0)); // Hrl
    
    return true;
}

// --------------------------------------------- Setup methods
bool Aurora::KirkebyBase::Init()
{
    InitVectors();
    InitFft();
    return true;
}

// --------------------------------------------- Processig methods
bool Aurora::KirkebyBase::PreProcess()
{
   if(IsFilterStereo())
   {
       // Here we have to fix the fake done in InitVectors()
        m_nCols = 2;
        m_Ht(1,0).Copy(m_Ht(0,1)); // Hrl = Hlr
        m_Ht(1,1).Copy(m_Ht(0,0)); // Hrr = Hll
   }
   
    assert(m_inputDataLength > 0);
   int col, row;
   
   // Do fade in/out 
   for(row = 0; row < m_nRows; row++)
   {
      for(col = 0; col < m_nCols; col++)
      {
         if(m_fadeInLength > 0) 
         {
             m_Ht(row, col).FadeIn(m_fadeInLength);
         }

         if(m_fadeOutLength > 0)
         {
             // do fade out on the real size of the vector, not on the padded one!
             int from = -1;
             
             if (m_Ht(row, col).Length() != m_inputDataLength)
             {
                 from = (int)(m_inputDataLength - m_fadeOutLength);
             }
             m_Ht(row, col).FadeOut(m_fadeOutLength, from);
         }
      }
   }
   return true;
}


bool Aurora::KirkebyBase::Process() 
{
    // 1) Init() should be already called!

    // 2) Pre-process (fade-in/out, ecc.)
    if(!PreProcess()) 
    {
        return false;
    }
 
    // 3) Spectrum inversion cycle

    switch(m_nFilterType)
    {
      //Farina: 0 - mono, 1 - dual mono, 
      //        2 - stereo (stereo dipole symm.), 
      //        3 - 2x2 (complete stereo dipole)

        case Aurora::KirkebyBase::FilterType::Mono:
        {
            int row, col;
            
            for(row = 0; row < m_nRows; row++)
            {
                for(col = 0; col < m_nCols; col++)
                {
                    ComputeMonoInverseFilter(m_Ht(row, col));
                }
            }
            break;
        }
        case Aurora::KirkebyBase::FilterType::Stereo:
        {
            if(m_bCancelCrosstalk)
            {
                ComputeQuadInverseFilter(m_Ht);
            }
            else
            {
                ComputeMonoInverseFilter(m_Ht(0,0));
                ComputeMonoInverseFilter(m_Ht(1,0));
            }
            break;
        }
        case Aurora::KirkebyBase::FilterType::Dipole:
        {
            ComputeQuadInverseFilter(m_Ht);
            break;
        }
    }
   
    if(!PostProcess()) 
    {
        return false;
    }
   
   return true;
}

bool Aurora::KirkebyBase::PostProcess()
{
    if (m_bAutorange)
    {
        double max = 0.0;

        m_Ht.ForEach([&max](Aurora::TSampleVector& cell)
        {
            auto m = cell.GetAbsMax();

            if (m > max)
            {
                max = m;
            }
        });

        if (max < 0)
        {
            return false;
        }
        m_dbGain = 1.0 / max;        
    }    
    ApplyGain();

    return true;
}
void Aurora::KirkebyBase::SetRows(const int nr) 
{ 
    m_nRows = nr; 
}

void Aurora::KirkebyBase::SetCols(const int nc) 
{
    m_nCols = nc;
    m_colLength = m_rowLength / (IsFilterStereo() ? 1 : m_nCols);
}

void Aurora::KirkebyBase::SetChannels(const int nch) 
{ 
    SetRows(nch); 
}

void Aurora::KirkebyBase::SetInputRowLength(const SampleCount irl) 
{
    //m_rowLength = irl;
    //m_colLength = irl / (IsFilterStereo() ? 1 : m_nCols);
    m_inputDataLength = irl;
}

void Aurora::KirkebyBase::SetInputTrackLength(const SampleCount itl) 
{ 
    SetInputRowLength(itl); 
}

void Aurora::KirkebyBase::SetFftLength(const SampleCount N) 
{
    m_colLength = N;
    m_rowLength = N * m_nCols;
}

void Aurora::KirkebyBase::SetInverseFilterLength(const SampleCount N) 
{
    m_colLength = N;
    m_rowLength = N * m_nCols;
}

void Aurora::KirkebyBase::Init(const int rows, const int cols)
{
    m_nRows = rows;
    m_nCols = cols;
}

void Aurora::KirkebyBase::Destroy()
{
   DeleteVectors();
   DeleteFft();
}

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283

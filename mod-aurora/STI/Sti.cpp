/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  sti.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::STI
\brief The Audacity module for Aurora STI class.

  This class calculates the Speech Transmission Index, given a
  a room Impulse Response, a Background Noise recording and
  the artificial mouth probe signal recording (standard IEC 60268-16 2003).

*//*******************************************************************/
#include <aurora.h>

#include "sti.h"
#include "StiTrack.h"

const Aurora::STISpectrum& Aurora::STISpectrum::MaskingThresholdsProfile() 
{
    static const Aurora::STISpectrum mt(Aurora::STISpectrum::Profile::MaskingThreshold);
    return mt;
}

const Aurora::STISpectrum& Aurora::STISpectrum::MaskingRangeProfile() 
{
    static const Aurora::STISpectrum mr(Aurora::STISpectrum::Profile::MaskingRange);
    return mr;
}

const Aurora::STISpectrum& Aurora::STISpectrum::MaskingSlopeProfile() 
{
    static const Aurora::STISpectrum ms(Aurora::STISpectrum::Profile::MaskingSlope);
    return ms;
}

const Aurora::STISpectrum& Aurora::STISpectrum::FemaleCorrectionProfile() 
{
    static const Aurora::STISpectrum fc(Aurora::STISpectrum::Profile::FemaleCorrection);
    return fc;
}

const Aurora::STISpectrum& Aurora::STISpectrum::AlphaMaleProfile() 
{
    static const Aurora::STISpectrum am(Aurora::STISpectrum::Profile::AlphaMale);
    return am;
}

const Aurora::STISpectrum& Aurora::STISpectrum::BetaMaleProfile() 
{
    static const Aurora::STISpectrum bm(Aurora::STISpectrum::Profile::BetaMale);
    return bm;
}

const Aurora::STISpectrum& Aurora::STISpectrum::AlphaFemaleProfile() 
{
    static const Aurora::STISpectrum af(Aurora::STISpectrum::Profile::AlphaFemale);
    return af;
}

const Aurora::STISpectrum& Aurora::STISpectrum::BetaFemaleProfile() 
{
    static const Aurora::STISpectrum bf(Aurora::STISpectrum::Profile::BetaFemale);
    return bf;
}

const Aurora::STISpectrum& Aurora::STISpectrum::OMSTItelProfile() 
{
    static const Aurora::STISpectrum oms(Aurora::STISpectrum::Profile::OMSTItel);
    return oms;
}

const Aurora::STISpectrum& Aurora::STISpectrum::OMRaSTIProfile() 
{
    static const Aurora::STISpectrum omr(Aurora::STISpectrum::Profile::OMRaSTI);
    return omr;
}

void Aurora::STISpectrum::SetProfile(const Aurora::STISpectrum::Profile profile)
{
    switch(profile)
    {
        case Aurora::STISpectrum::Profile::DefaultNoise:
        {
            double defNoiseProfile[] = {  0.0,  0.0, 48.0, 45.0, 42.0, 39.0, 
                                         36.0, 33.0, 30.0,  0.0,  0.0,  0.0 };
            Aurora::Spectrum<double>::SetRange(31.5f, 16000.0f);

            ForEach(true, [&](const size_t i, const float fcb, double& value)
            {
                value = defNoiseProfile[i];
            });
            break;
        }
        
        case Aurora::STISpectrum::Profile::DefaultSignal:
        {
            double defSignalProfile[] = {  0.0,  0.0, 70.9, 70.9, 67.2, 61.2, 
                                          55.2, 49.2, 43.2,  0.0,  0.0,  0.0  };
            SetRange(31.5f, 16000.0f);
            
            ForEach(true, [&](const size_t i, const float fcb, double& value)
            {
                value = defSignalProfile[i];
            });            
            break;
        }
        
        case Aurora::STISpectrum::Profile::MaskingThreshold:
        {
            double thresholds[] = { 0.0, 0.0, 46.0, 27.0, 12.0, 
                                    6.5, 7.5,  8.0, 12.0,  0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = thresholds[i];
            });                     
            break;
        }
        
        case Aurora::STISpectrum::Profile::MaskingRange: 
        {
            double maskingRange[] = {  0.0,  0.0,  45.5, 55.5,  65.5, 
                                      75.5, 85.5, 95.5, 200.0,   0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = maskingRange[i];
            });                     
            break;
        }
        
        case Aurora::STISpectrum::Profile::MaskingSlope:
        {
            double maskingSlope[] = { 0.0,      0.0,  0.0,      0.0001, 0.000316, 
                                      0.003162, 0.01, 0.031622, 0.1,    0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = maskingSlope[i];
            });                     
            break;
        }

        case Aurora::STISpectrum::Profile::FemaleCorrection:
        {
           double femaleCorrection[] = {  0.0,  0.0,  0.0,  2.4, -1.1, 
                                         -2.3, -3.0,  2.1,  6.8,  0.0 };  
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = femaleCorrection[i];
            });                     
            break;
        }

        case Aurora::STISpectrum::Profile::AlphaMale:
        {
            double alphaM[] = { 0.0,   0.0,   0.085, 0.127, 0.230, 
                                0.233, 0.309, 0.224, 0.173, 0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = alphaM[i];
            });                     
            break;
        }
        
        case Aurora::STISpectrum::Profile::BetaMale:            
        {
            double betaM[]  = { 0.0,   0.0,   0.085, 0.078, 0.065, 
                                0.011, 0.047, 0.095, 0.000, 0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = betaM[i];
            });                     
            break;        
        }
        
        case Aurora::STISpectrum::Profile::AlphaFemale:
        {
            double alphaF[] = { 0.0,   0.0,   0.000, 0.117, 0.223, 
                                0.216, 0.328, 0.250, 0.194, 0.0 };  
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = alphaF[i];
            });                     
            break;
        }
        
        case Aurora::STISpectrum::Profile::BetaFemale:            
        {
            double betaF[]  = { 0.0,   0.0,   0.000, 0.099, 0.066, 
                                0.062, 0.025, 0.076, 0.000, 0.0 }; 
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = betaF[i];
            });                     
            break;        
        }
        
        case Aurora::STISpectrum::Profile::OMSTItel:            
        {
            double OMSTItel[]  = { 0.0,  0.0,  1.12, 11.33,  0.71, 
                                   2.83, 6.97, 1.78,  4.53,  0.0   };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = OMSTItel[i];
            });                     
            break;        
        }

        case Aurora::STISpectrum::Profile::OMRaSTI:            
        {
            double OMRaSTI[]  =  { 0.0, 0.0,  0.0, 0.7, 1.4,
                                   2.8, 5.6, 11.2, 0.0, 0.0 };
            SetRange(125.0f, 8000.0f);
            
            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = OMRaSTI[i];
            });                     
            break;        
        }
/*
        case Aurora::STISpectrum::Profile::OMMTI:
        {
            double OM[] = { 0.63, 0.8, 1.0, 1.25, 1.6,  2.0,  2.5,
                            3.15, 4.0, 5.0, 6.3,  8.0, 10.0, 12.5 };
            SetRange(125.0f, 8000.0f);

            ForEach(false, [&](const size_t i, const float fcb, double& value)
            {
                value = OM[i];
            });
            break;
        }
        */
    }
}

void Aurora::STISpectrum::SetRange(const Aurora::STISpectrum::Range range)
{
    
    switch(range)
    {
        case Aurora::STISpectrum::Range::Full:
            SetRange(31.5f, 16000.0f);
            break;
            
        case Aurora::STISpectrum::Range::Reduced:
            SetRange(125.0f, 8000.0f);
            break;

        case Aurora::STISpectrum::Range::FiveBands:
            SetRange(250.0f, 4000.0f);
            break;
    }
}

void Aurora::STISpectrum::SetRange(const float fcbStart, const float fcbEnd)
{
    Aurora::Spectrum<double>::SetRange(fcbStart, fcbEnd);
}

Aurora::STISpectrum& Aurora::STISpectrum::operator=(const STISpectrum& s)
{
    Aurora::Spectrum<double>::operator=(s);
    return *this;
}

Aurora::STISpectrum::STISpectrum(const Aurora::STISpectrum& s)
 : Aurora::Spectrum<double>(s)
{ }

Aurora::STISpectrum::STISpectrum(const float fcbStart, const float fcbEnd)
{
    assert(fcbStart < fcbEnd);
    SetRange(fcbStart, fcbEnd);
}

Aurora::STISpectrum::STISpectrum(const Aurora::STISpectrum::Profile profile)
{
    SetProfile(profile);
}

// --- STI class implementation ------------------------------------------------
void Aurora::STI::ModulationTransferFunctionsTable::Clear()
{
    m_table.clear();
    
    for (auto frq : m_frequencies)
    {
        m_table.emplace(std::make_pair(frq, Aurora::STISpectrum(125.0f, 8000.0f)));
    }
}
            
Aurora::STISpectrum& Aurora::STI::ModulationTransferFunctionsTable::GetSpectrum(const double modulationFrequency)
{
    return m_table[modulationFrequency];
}
            
double Aurora::STI::ModulationTransferFunctionsTable::GetValue(const double modulationFrequency,
                                                               const float fcb) const
{
    return m_table.at(modulationFrequency).GetValue(fcb);
}
            
void Aurora::STI::ModulationTransferFunctionsTable::SetValue(const double modulationFrequency,
                                                             const float fcb,
                                                             const double value)
{
    m_table[modulationFrequency].SetValue(fcb, value);
}

Aurora::STI::Results::Results()
{
    noiseSpectrum.SetProfile(Aurora::STISpectrum::Profile::DefaultNoise);
    signalSpectrum.SetProfile(Aurora::STISpectrum::Profile::DefaultSignal);
    
    aRaSTI.SetRange(Aurora::STISpectrum::Range::FiveBands);
    aRaSTIf.SetRange(Aurora::STISpectrum::Range::FiveBands);
    
    aSTItel.SetRange(Aurora::STISpectrum::Range::SevenBands);
    aSTItelf.SetRange(Aurora::STISpectrum::Range::SevenBands);
    aMTI.SetRange(Aurora::STISpectrum::Range::SevenBands);
}

double Aurora::STI::Flatten(double& value, const double flat)
{
    if(value < -flat)
    {
        value = -flat;
    }
    else if(value > flat)
    {
        value = flat;
    }
    return value;
}

void Aurora::STI::CalibrateFullscale(Aurora::STIAudioTrack& calibrationSignal, 
                                     const int nChnl)
{
    // Farina's
    calibrationSignal.ResetFilteredTrack();
    m_dbFullScaleLevel[nChnl] = m_dbCalibLevel[nChnl] - calibrationSignal.Leq(); 
}

void Aurora::STI::ComputeOctaveSpectrum(Aurora::STIAudioTrack& track, 
                                        Aurora::STISpectrum& octaveSpectrum, 
                                        const int nChnl)
{   
    octaveSpectrum.ForEach(false, 
                           [&](const size_t bd, const float fcb, double& value)    
    {
        // Needed before every filtering to restore unfiltered vector
        track.ResetFilteredTrack();
        track.STIOctaveFilter(fcb);
        value = track.Leq() + m_dbFullScaleLevel[nChnl];
        ProgressMeterWrapper::Update((int)bd + 1, 1);
    });
    
    // A band (11th)
    track.ResetFilteredTrack();
    track.AFilter();
    octaveSpectrum.SetAverageValue("A", track.Leq() + m_dbFullScaleLevel[nChnl]);
    ProgressMeterWrapper::Update(11, 1);

    // Lin band (12th)
    track.ResetFilteredTrack();
    octaveSpectrum.SetAverageValue("Lin", track.Leq() + m_dbFullScaleLevel[nChnl]);
    ProgressMeterWrapper::Update(12, 1);

}

bool Aurora::STI::StoreSpectrums(Aurora::STIAudioTrack& signal, 
                                 Aurora::STIAudioTrack& noise, 
                                 const int nChnl)
{ 

    wxString msg;
    msg.Printf("Background Noise: processing %s channel.", nChnl ? "right" 
                                                                 : "left");
    ProgressMeterWrapper::SetMessage(msg);

    Aurora::STI::Results& res = m_results[nChnl];
    
    // Store noise
    ComputeOctaveSpectrum(noise, res.noiseSpectrum, nChnl);
    
    msg.Printf("Test Signal: processing %s channel.",  nChnl ? "right"
                                                             : "left");
    ProgressMeterWrapper::SetMessage(msg);

    Aurora::STISpectrum octaveSpectrum;

    // Store signal
    ComputeOctaveSpectrum(signal, octaveSpectrum, nChnl);
    
    if(m_bIsSignalPlusNoise)
    {        
        octaveSpectrum.ForEach(true, [&](const size_t i, const float fcb, double& value)
        {
            res.sigNoiseSpectrum.SetValue(fcb, value);
            
            if(value - res.noiseSpectrum[fcb] > 0.0)
            {
                res.signalSpectrum.SetValue(fcb, 
                    dB(  std::pow(10.0, value/10.0) 
                       - std::pow(10.0, res.noiseSpectrum[fcb]/10.0)));
            } 
            else
            {
                res.signalSpectrum.SetValue(fcb, 0.0);
                m_bErrorFlag = true;
            }
        });
        
    }
    else
    {
        octaveSpectrum.ForEach(true, [&](const size_t i, const float fcb, double& value)
        {
            // Update Signal and SigNoise
            SetSignalLevel(value, fcb, nChnl, true); 
        });
        
    }

    return !m_bErrorFlag;
}

bool Aurora::STI::CalculateMatrix(Aurora::STIAudioTrack& track, const int nChnl)
{
    wxString msg;
    msg.Printf("Processing %s channel.", nChnl ? "right" : "left");
    ProgressMeterWrapper::SetMessage(msg);
    
    if(! track.FindFirstArrivalTime(m_dbFAT))
    {
        // Maybe some things to do on ERROR state...
        return false;                
    }
    
    Aurora::STI::Results& res = m_results[nChnl];
    bool ok = true;
        
    res.aSTItel.ForEachBreakable(false, [&](const size_t bd, 
                                            const float fcb, 
                                            double& value)
    {
        track.ResetFilteredTrack(); // to be done every time before filtering!
        track.STIOctaveFilter((double)fcb);
        
        track.CalculateMTF(fcb, res.tMTF);
        
        value =  track.CalculateMTF_STItel(fcb);
        
        if(fcb == 2000.0f)
        {
            track.CalculateMTF_RaSTI(res.aRaSTI);
        }

        if(! ProgressMeterWrapper::Update((int)bd - 1, 1))
        {
            ok = false;
            return false; // break loop
        }
        return true;
    });
    return ok; 
}

bool Aurora::STI::Compute(const int nChnl)
{
    // STI computation function.

    // A.Farina    
    const auto& maskingRange = Aurora::STISpectrum::MaskingRangeProfile();  
    const auto& maskingSlope = Aurora::STISpectrum::MaskingSlopeProfile();  
        
    //Auditory Masking Factor, dipende dal livello sonoro secondo la tabella
    // sovrastante
    Aurora::STISpectrum amf(125.0f, 8000.0f);
    
    //correzione allo spettro del segnale Female rispetto al segnale Male
    const auto& femaleCorrection = Aurora::STISpectrum::FemaleCorrectionProfile();  
    
    Aurora::STISpectrum snld(125.0f, 8000.0f); // sig NoiseLevelDiff; 
    Aurora::STISpectrum spnl(125.0f, 8000.0f); // sig PlusNoiseLevel; 
    
    Aurora::STI::Results& res = m_results[nChnl];
        
    for(int nSex = 0; nSex < 2; nSex++)
    {
        snld.ForEach(false, [&](const size_t i, const float fcb, double& snValue)
        {
            if(nSex == Genre::Female)
            {
                //SNR con correzione (caso 'female')
                snValue = res.signalSpectrum[fcb] + femaleCorrection[fcb]
                          - res.noiseSpectrum[fcb];
                          
                // calcolo livello totale segnale + rumore con correzione 
                // (caso 'female')
                spnl.SetValue(fcb,
                              dBsum((res.signalSpectrum[fcb] + femaleCorrection[fcb]), 
                                     res.noiseSpectrum[fcb]));
            }
            else
            {
                //SNR senza correzione (caso 'male')
                snValue = res.signalSpectrum[fcb] - res.noiseSpectrum[fcb];
                // calcolo livello totale segnale + rumore senza correzione 
                // (caso 'male')
                spnl.SetValue(fcb, dBsum(res.signalSpectrum[fcb],  
                                         res.noiseSpectrum[fcb]));
            }
            
            const auto& fcbs = maskingRange.Frequencies();

            // calcolo Auditory Masking Factor (AMF)
            amf.ForEachBreakable(false, [&](const size_t n, const float afcb, double& amfValue) -> bool
            {
                const double spnValue = spnl[fcb]; // or n???
                
                if(   spnValue >= maskingRange[fcbs[n]]
                   && spnValue <  maskingRange[fcbs[n+1]])
                {
                    amfValue = maskingSlope[afcb];
                    return false; // exit from loop
                }
                return true;
            });
        });
        
        auto& modFrequencies = res.tMTFf.GetFrequencies();
        
        for (auto mf : modFrequencies)
        {
            auto& dest = res.tMTFf.GetSpectrum(mf);
            auto& orig = res.tMTF.GetSpectrum(mf);
            
            dest.ForEach(false, [&](const size_t bd, const float fcb, double& mtfValue)
            {
                if(m_bSNCorrection)
                {
                    mtfValue = orig.GetValue(fcb)
                               / ( 1.0 + std::pow(10.0, (-snld.GetValue(fcb)/10.0)) );
                }
                else
                {
                    mtfValue = orig.GetValue(fcb);
                }
            });
        }
        
        res.aRaSTIf.ForEach(false, [&](const size_t bd, const float fcb, double& value)
        {
            if(m_bSNCorrection)
            {
                value = res.aRaSTI.GetValue(fcb)
                        / ( 1.0  + std::pow(10.0, (-snld[2000.0f] / 10.0)) );
            }
            else
            {
                value = res.aRaSTI.GetValue(fcb);
            }            
        });

        res.aSTItelf.ForEach(false, [&](const size_t bd, const float fcb, double& value)
        {
            if(m_bSNCorrection)
            {
                value = res.aSTItel.GetValue(fcb)
                        / ( 1.0 + std::pow(10.0, (-snld.GetValue(fcb)/10.0)) );
            }
            else
            {
                value = res.aSTItel.GetValue(fcb);
            }            
        });
        
        // applico le correzioni (masking and threshold) all'mtf per ottenere 
        // il valore finale 
        if(m_bMaskingCorrection)
        {
            amf.SetValue(125.0f, 0.0f);
            ApplyMaskingCorrection(nChnl, spnl, amf);
        }
        DoSNR(nChnl);
        DoSTI(nChnl, nSex);
        DoRaSTI(nChnl);
    }
    //These are only for males...
    DoSTItel(nChnl);
    DoSTIPa(nChnl);

    return true;
}

void Aurora::STI::ApplyMaskingCorrection(const int nChnl, 
                                         const Aurora::STISpectrum& sigPlusNoiseLevel, 
                                         const Aurora::STISpectrum& amf)
{
    // Masking has to be evaluated from 250 Hz 
    assert(amf[125.0f] == 0.0f);

    const auto& thresholds = Aurora::STISpectrum::MaskingThresholdsProfile();
    Aurora::STI::Results& res = m_results[nChnl];
    
    auto& modFrequencies = res.tMTFf.GetFrequencies();
        
    for (auto mf : modFrequencies)
    {
        auto& ms = res.tMTFf.GetSpectrum(mf);
        const auto& fcbs = ms.Frequencies();

        ms.ForEach(false, [&](const size_t bdi, const float fcb, double& mtfValue)
        {
            // Effetto di mascheramento della banda inferiore
            // adiacente:
            // la prima banda ha solo Threshold, non ha Masking
            // le altre 6 bande hanno sia Threshold che Masking
            // ...so it has to be mask(125) = 0.

            const double mask = amf[fcb] * sigPlusNoiseLevel[fcbs[bdi-1]]; 
            const double a = std::pow(10.0, sigPlusNoiseLevel[fcb]) / 10.0;
            
            mtfValue =  mtfValue *  a / (a + mask + thresholds[fcb]);
        });
    }
    
    {
        // No masking for RaSTI
        const auto& fcbs = res.aSTItelf.Frequencies();

        res.aSTItelf.ForEach(false, [&](const size_t bdi, const float fcb, double& stValue)
        {
            // Effetto di mascheramento della banda inferiore
            // adiacente:
            // la prima banda ha solo Threshold, non ha Masking
            // le altre 6 bande hanno sia Threshold che Masking

            const double mask = amf[fcb] * sigPlusNoiseLevel[fcbs[bdi - 1]]; 
            const double a = std::pow(10.0, sigPlusNoiseLevel[fcb] / 10.0);

            stValue = stValue * a / (a + mask + thresholds[fcb]);
        }); 
    }
}   

void Aurora::STI::DoSNR(const int nChnl)
{ 
    Aurora::STI::Results& res = m_results[nChnl];
    
    //-------------------------------------------------------------- Calcolo SNR 
    res.aMTI.ForEach(false, [&](const size_t bd, const float fcb, double& mtiValue)
    {
        double tiSum = 0.0;
        
        auto& modFrequencies = res.tMTFf.GetFrequencies();
        
        for (auto mf : modFrequencies)
        {
            const double mtfValue = res.tMTFf.GetValue(mf, fcb);
            double snr = dB( mtfValue / (1.0 - mtfValue) );
            //calcolo TI
            double ti = (Flatten(snr, 15.0) + 15.0) / 30.0;
            
            //calcolo l'MTI medio di ciascuna banda
            tiSum += ti;
        }
    
        mtiValue = tiSum / 14.0; // Valori STI di banda
    });

}

void Aurora::STI::DoSTI(const int nChnl, const int nSex)
{
    const auto& alphaM = Aurora::STISpectrum::AlphaMaleProfile();
    const auto&  betaM = Aurora::STISpectrum::BetaMaleProfile();
    const auto& alphaF = Aurora::STISpectrum::AlphaFemaleProfile();
    const auto&  betaF = Aurora::STISpectrum::BetaFemaleProfile();
    
    double alpha = 0.0;
    double beta  = 0.0;
    
    Aurora::STI::Results& res = m_results[nChnl];
    const auto& fcbs = res.aMTI.Frequencies();
    
   //----------------------------------------------------------------calcolo STI    
    if(nSex == Aurora::STI::Genre::Male)
    {        
        // TODO: these loops work correctly only if aMTI value at 16kHz is 0!!!
        res.aMTI.ForEach(false, [&](const size_t bd, const float fcb, double& mtiValue)
        {
            const double nextMtiValue = res.aMTI.GetValue(fcbs[bd + 1]);
            
            alpha += alphaM[fcb] * mtiValue;
            beta  += betaM [fcb] * std::sqrt(mtiValue * nextMtiValue);
        });
        res.STImale = alpha - beta;
    }
    else
    {
        res.aMTI.ForEach(false, [&](const size_t bd, const float fcb, double& mtiValue)
        {
            const double nextMtiValue = res.aMTI.GetValue(fcbs[bd + 1]);
            
            alpha += alphaF[fcb] * mtiValue;
            beta  += betaF [fcb] * std::sqrt(mtiValue * nextMtiValue);
        });
        res.STIfemale = alpha - beta;
    }
}

void Aurora::STI::DoRaSTI(const int nChnl)
{
   double snr = 0.0;

   //------------------------------------------------------------- calcolo RaSTI
    double tiRaSTI_1 = 0.0;
    
    Aurora::STI::Results& res = m_results[nChnl];

    auto& modFrequencies = res.tMTFf.GetFrequencies();
        
    for(size_t n = 2; n < modFrequencies.size(); n += 3)
    {
        // 500 Hz band
        const double mtfValue = res.tMTFf.GetValue(modFrequencies[n], 500.0f);
        snr = dB( mtfValue / (1.0 - mtfValue) );
        
        //calcolo TI
        tiRaSTI_1 += (Flatten(snr, 15.0) + 15.0) / 30.0;
    }
    tiRaSTI_1 /= 4.0; // Valore STI di banda 500 Hz, media di 4 Mod.freq.
    
    double tiRaSTI_2 = 0.0; // 2000 Hz
    
    res.aRaSTIf.ForEach(false, [&](const size_t bd, const float fcb, double& rastiValue)
    {
        snr = dB( rastiValue / (1.0 - rastiValue) );
        //calcolo TI
        tiRaSTI_2 += (Flatten(snr, 15.0) + 15.0) / 30.0;
    });
    tiRaSTI_2 /= 5.0; // Valore STI di banda 500 Hz, media di 5 Mod.freq.
    res.RaSTI = 4.0 / 9.0 * tiRaSTI_1 + 5.0 / 9.0 * tiRaSTI_2;
}

void Aurora::STI::DoSTItel(const int nChnl)
{
    Aurora::STISpectrum tiSTItel(125.0f, 8000.0f);

    double snr = 0.0;

    Aurora::STI::Results& res = m_results[nChnl];
    // ---------------------------------------------------------- calcolo STItel    
    
    res.aSTItelf.ForEach(false, [&](const size_t bd, const float fcb, double& stitelValue)
    {
        snr = dB( stitelValue / (1.0 - stitelValue) );
     
        //calcolo STItel di banda
        tiSTItel.SetValue(fcb, (Flatten(snr, 15.0) + 15.0) / 30.0);
    });
    
    //calcolo valore finale di STItel
    const auto& alphaM = Aurora::STISpectrum::AlphaMaleProfile();
    const auto&  betaM = Aurora::STISpectrum::BetaMaleProfile();
        
    double alpha = 0.0;
    double beta  = 0.0;
    
    auto fcbs = tiSTItel.RangedFrequencies();
    
    for (size_t i = 0; i < fcbs.size() - 1; ++i)
    {
        alpha += alphaM[fcbs[i]] * tiSTItel[fcbs[i]];
        beta  += betaM [fcbs[i]] * std::sqrt(tiSTItel[fcbs[i]] * tiSTItel[fcbs[i + 1]]);
    }
    // last value
    alpha += alphaM[8000.0f] * tiSTItel[8000.0f];
    beta  += betaM [8000.0f];

    res.STItel = alpha - beta;
}

void Aurora::STI::DoSTIPa(const int nChnl)
{
    Aurora::STISpectrum MTIpa;
    MTIpa.SetRange(125.0f, 8000.0f);
    
    double snr = 0.0;
    double ti  = 0.0;    
    double mtf_125;
    double mtf_250;
        
    Aurora::STI::Results& res = m_results[nChnl]; 
    //------------------------------------------------------------ calcolo STIPa
    // 250 Hz (con media anche su 125 Hz)
    // 1.0 Hz
    mtf_125 = res.tMTFf.GetValue(1.0, 125.0f);
    mtf_250 = res.tMTFf.GetValue(1.0, 250.0f);
    
    snr = dB( ((mtf_125 + mtf_250) / 2.0) / (1.0 - (mtf_250 - mtf_125) / 2.0) );    
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0; //calcolo TI
    
    // 5.0 Hz
    mtf_125 = res.tMTFf.GetValue(5.0, 125.0f);
    mtf_250 = res.tMTFf.GetValue(5.0, 250.0f);
    
    snr = dB( ((mtf_125 + mtf_250) / 2.0) / (1.0 - (mtf_250 - mtf_125) / 2.0)  );    
    //calcolo TI
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;
    
    // Valore STI di banda 125/250 Hz, media di 2 Mod.freq.
    MTIpa[250.0f] = ti / 2.0; 
    
    // 500 Hz
    // 0.63 Hz
    double mtf = res.tMTFf.GetValue(0.63, 500.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0;
    
    // 3.15 Hz
    mtf = res.tMTFf.GetValue(3.15, 500.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;
    
    // Valore STI di banda 500 Hz, media di 2 Mod.freq.
    MTIpa[500.0f] = ti / 2.0; 
    
    // 1000 Hz
    // 2.0 Hz
    mtf = res.tMTFf.GetValue(2.0, 1000.0f);
    snr = dB( mtf /  (1.0 - mtf) );
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0; //calcolo TI
    
    // 10 Hz
    mtf = res.tMTFf.GetValue(10.0, 1000.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;

    // Valore STI di banda 1000 Hz, media di 2 Mod.freq.
    MTIpa[1000.0f] = ti / 2.0; 
    
    // 2000 Hz
    // 1.25 Hz
    mtf = res.tMTFf.GetValue(1.25, 2000.0f);
    snr = dB( mtf /  (1.0 - mtf) );
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0; 
    
    // 6.25 Hz
    mtf = res.tMTFf.GetValue(6.3, 2000.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;
    
     // Valore STI di banda 2000 Hz, media di 2 Mod.freq.
    MTIpa[2000.0f] = ti / 2.0;

    // 4000 Hz
    // 0.8 Hz
    mtf = res.tMTFf.GetValue(0.8, 4000.0f);
    snr = dB( mtf /  (1.0 - mtf) );
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0; 
    
    // 4.0 Hz
    mtf = res.tMTFf.GetValue(4.0, 4000.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;
    
    // Valore STI di banda 4000 Hz, media di 2 Mod.freq.
    MTIpa[4000.0f] = ti / 2.0; 
    
    // 8000 Hz
    // 2.5 Hz
    mtf = res.tMTFf.GetValue(2.5, 8000.0f);
    snr = dB( mtf /  (1.0 - mtf) );
    ti = (Flatten(snr, 15.0) + 15.0) / 30.0; 
    
    // 12.5 Hz
    mtf = res.tMTFf.GetValue(12.5, 8000.0f);
    snr = dB( mtf / (1.0 - mtf) );
    ti += (Flatten(snr, 15.0) + 15.0) / 30.0;
    
    // Valore STI di banda 8000 Hz, media di 2 Mod.freq.
    MTIpa[8000.0f] = ti / 2.0; 
    
    //calcolo finale di STIpa male
    const auto& alphaM = Aurora::STISpectrum::AlphaMaleProfile();
    const auto&  betaM = Aurora::STISpectrum::BetaMaleProfile();
    
    double alpha = 0.0;
    double beta  = 0.0;
    
    auto fcbs = MTIpa.RangedFrequencies();
    
    for (size_t i = 0; i < fcbs.size() - 1; ++i)
    {
        alpha += alphaM[fcbs[i]] * MTIpa[fcbs[i]];
        beta  += betaM [fcbs[i]] * std::sqrt(MTIpa[fcbs[i]] * MTIpa[fcbs[i + 1]]);
    }
    // last value
    alpha += alphaM[8000.0f] * MTIpa[8000.0f];
    beta  += betaM [8000.0f];

    res.STIpa = alpha - beta;
}


void Aurora::STI::ReadConfigurationValues()
{
    Aurora::Config cfg;
    
    double dbValue;
    wxString path;
    
    if(cfg.Read("/Aurora/STI/FullScale_Ch_1", &dbValue))  
    {
        SetFullScaleLevel(dbValue, CH_LEFT);
    }
    
    if(cfg.Read("/Aurora/STI/FullScale_Ch_2", &dbValue))  
    {
        SetFullScaleLevel(dbValue, CH_RIGHT);       
    }
    
    if(cfg.Read("/Aurora/STI/Calibration_Ch_1", &dbValue)) 
    {
        SetCalibrationLevel(dbValue, CH_LEFT);       
    }
    
    if(cfg.Read("/Aurora/STI/Calibration_Ch_2", &dbValue))  
    {
        SetCalibrationLevel(dbValue, CH_RIGHT);    
    }

    for(int nChnl = Aurora::Channel::Left; nChnl < 2; nChnl++)
    {
        const auto& fcbs = m_results[nChnl].signalSpectrum.Frequencies();
        
        for(auto fcb : fcbs)
        {
            path.Printf("/Aurora/STI/SignalLevel_Bd_%d_Ch_%d", (int)fcb, nChnl+1);
            
            if(cfg.Read(path, &dbValue)) 
            { 
                SetSignalLevel(dbValue, fcb, nChnl);
            }
        }

        for(auto fcb : fcbs)
        {
            path.Printf("/Aurora/STI/NoiseLevel_Bd_%d_Ch_%d", (int)fcb, nChnl+1);

            if(cfg.Read(path, &dbValue))  
            { 
                SetNoiseLevel(dbValue, fcb, nChnl, true); 
            }
        }
    }
}

void Aurora::STI::StoreConfigurationValues()
{
    Aurora::Config cfg;

    wxString path;

    cfg.Write("/Aurora/STI/FullScale_Ch_1", GetFullScaleLevel(CH_LEFT));
    cfg.Write("/Aurora/STI/FullScale_Ch_2", GetFullScaleLevel(CH_RIGHT));

    cfg.Write("/Aurora/STI/Calibration_Ch_1", GetCalibrationLevel(CH_LEFT));
    cfg.Write("/Aurora/STI/Calibration_Ch_2", GetCalibrationLevel(CH_RIGHT));

    for(int nChnl = Aurora::Channel::Left; nChnl < 2; nChnl++)
    {
        Aurora::STI::Results& res = m_results[nChnl];
        const auto& fcbs = res.signalSpectrum.Frequencies();
        
        for(auto fcb : fcbs)
        {
            path.Printf("/Aurora/STI/SignalLevel_Bd_%d_Ch_%d", (int)fcb, nChnl+1);
            cfg.Write(path, res.signalSpectrum.GetValue(fcb));
        }

        for(auto fcb : fcbs)
        {
            path.Printf("/Aurora/STI/NoiseLevel_Bd_%d_Ch_%d", (int)fcb, nChnl+1);
            cfg.Write(path, res.noiseSpectrum.GetValue(fcb));
        }
    }

    cfg.Flush();
}

void Aurora::STI::SetSignalLevel(const double value, 
                                 const float  fcb, 
                                 const int    nChnl, 
                                 const bool   bUpdateSigNoiseLevel) 
{ 
    m_results[nChnl].signalSpectrum.SetValue(fcb, value); 
    
    if(bUpdateSigNoiseLevel) 
    {
        SetSigNoiseLevel(fcb, nChnl);
    }
}

void Aurora::STI::SetNoiseLevel(const double value, 
                                const float  fcb, 
                                const int    nChnl, 
                                const bool   bUpdateSigNoiseLevel) 
{ 
    m_results[nChnl].noiseSpectrum.SetValue(fcb, value); 

    if(bUpdateSigNoiseLevel) 
    {
        SetSigNoiseLevel(fcb, nChnl);
    }
}

void Aurora::STI::SetSigNoiseLevel(const double value, 
                                   const float  fcb, 
                                   const int    nChnl) 
{ 
    m_results[nChnl].sigNoiseSpectrum.SetValue(fcb, value); 
} 

void Aurora::STI::SetSigNoiseLevel(const float fcb, const int nChnl) 
{ 
    m_results[nChnl].sigNoiseSpectrum.SetValue(fcb, 
        dBsum( m_results[nChnl].signalSpectrum.GetValue(fcb),
               m_results[nChnl].noiseSpectrum.GetValue(fcb) )); 
}

void Aurora::STI::SetCalibrationLevel(const double dbValue, const int nChnl) 
{ 
    m_dbCalibLevel [nChnl] = dbValue; 
}

void Aurora::STI::SetFullScaleLevel(const double dbValue,const int nChnl) 
{ 
    m_dbFullScaleLevel [nChnl] = dbValue; 
}

//
//  STISpectrum.cpp
//  Audacity
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#include "STISpectrum.h"


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

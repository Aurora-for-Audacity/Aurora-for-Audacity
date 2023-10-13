//
//  STISpectrum.hpp
//  Audacity
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#ifndef STISpectrum_hpp
#define STISpectrum_hpp

#include <aurora.h>

namespace Aurora{
class STISpectrum : public Aurora::Spectrum<double>
{
public:
    enum Range
    {
        Full,
        Reduced,
        FiveBands,
        
        SevenBands = 1
    };
    
    enum Profile
    {
        DefaultNoise,
        DefaultSignal,
        MaskingThreshold,
        MaskingRange,     // A.Farina STI computation
        MaskingSlope,     // A.Farina STI computation
        FemaleCorrection,
        AlphaMale,        //coeff di peso voce maschile
        BetaMale,
        AlphaFemale,       //coeff di peso voce femminile
        BetaFemale,
        
        OMSTItel,  // used in tracks
        OMRaSTI //,
        //OMMTI
    };
    
public:
    static const STISpectrum& MaskingThresholdsProfile();
    static const STISpectrum& MaskingRangeProfile();
    static const STISpectrum& MaskingSlopeProfile();
    static const STISpectrum& FemaleCorrectionProfile();
    static const STISpectrum& AlphaMaleProfile();
    static const STISpectrum& BetaMaleProfile();
    static const STISpectrum& AlphaFemaleProfile();
    static const STISpectrum& BetaFemaleProfile();
    static const STISpectrum& OMSTItelProfile();
    static const STISpectrum& OMRaSTIProfile();
    
    void SetProfile(const STISpectrum::Profile profile);
    
    void SetRange(const STISpectrum::Range range);
    void SetRange(const float fcbStart, const float fcbEnd);
    
    STISpectrum& operator=(const STISpectrum& s);
    
    STISpectrum() { Reset(); }
    STISpectrum(const STISpectrum& s);
    STISpectrum(const float fcbStart, const float fcbEnd);
    STISpectrum(const STISpectrum::Profile profile);
};
}

#endif /* STISpectrum_hpp */

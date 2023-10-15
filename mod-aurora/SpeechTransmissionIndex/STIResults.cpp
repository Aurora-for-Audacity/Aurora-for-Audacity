//
//  STIResults.cpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#include "STIResults.h"

Aurora::Results::Results()
{
    noiseSpectrum.SetProfile(Aurora::STISpectrum::Profile::DefaultNoise);
    signalSpectrum.SetProfile(Aurora::STISpectrum::Profile::DefaultSignal);
    
    aRaSTI.SetRange(Aurora::STISpectrum::Range::FiveBands);
    aRaSTIf.SetRange(Aurora::STISpectrum::Range::FiveBands);
    
    aSTItel.SetRange(Aurora::STISpectrum::Range::SevenBands);
    aSTItelf.SetRange(Aurora::STISpectrum::Range::SevenBands);
    aMTI.SetRange(Aurora::STISpectrum::Range::SevenBands);
}

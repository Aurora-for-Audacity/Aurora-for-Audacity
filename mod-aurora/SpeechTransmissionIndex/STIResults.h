//
//  STIResults.hpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#ifndef STIResults_hpp
#define STIResults_hpp

#include "STISpectrum.h"
#include "ModulationTransferFunctionsTable.h"

namespace Aurora {
// *** Results vectors & tables

class Results
{
    
public:
    Aurora::STISpectrum noiseSpectrum;
    Aurora::STISpectrum signalSpectrum;
    Aurora::STISpectrum sigNoiseSpectrum;
    
    ModulationTransferFunctionsTable tMTF;   // These are the original values
    Aurora::STISpectrum  aRaSTI;  // five bands...
    Aurora::STISpectrum  aSTItel; // reduced (7) bands
    
public:
    ModulationTransferFunctionsTable tMTFf;  // These are final values,
    Aurora::STISpectrum  aRaSTIf;               // so they  can be
    Aurora::STISpectrum  aSTItelf;              // denoised, masked , etc
    
    double STImale   = 0.0;
    double STIfemale = 0.0;
    double STIpa     = 0.0;
    
    Aurora::STISpectrum  aMTI; // reduced (7) bands (Modulation Transfer Indices)
    double RaSTI  = 0.0;
    double STItel = 0.0;
    
    Results();
    
    
};

}

#endif /* STIResults_hpp */

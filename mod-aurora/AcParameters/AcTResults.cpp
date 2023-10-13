//
//  AcTResults.cpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#include "AcTResults.h"


//----------------------------------------------------------------------------
// Aurora::AcousticalParameters::TResults implementation
//----------------------------------------------------------------------------
Aurora::TResults::TParameter::TParameter(const double v,
                                                               const bool   valid)
    : value(v), isValid(valid)
{
    if (isnan(value) || isinf(value))
    {
        // nan and infinite values are always unacceptable.
        isValid = false;
    }
}

const std::vector<std::string> Aurora::TResults::m_parameterNames =
{
    "Signal",   "Noise",
    "strenGth", "C50",    "C80",  "D50",  "Ts",
    "EDT",      "Tuser",  "T20",  "T30",

    "Peakiness", "Millisecondness",  "Impulsiveness",

    "St1",     "St2",        "StLate",     // StageParameters (14..)
    "IACC",    "tauIACC",    "widthIACC",  // BinauralParameters (17..)
    "Jlf",     "Jlfc",       "Lj",          // SpatialParameters  (20..)
};

void Aurora::TResults::Set(const std::string& parameterName,
                                                 const float fcb,
                                                 const double value,
                                                 const bool   valid)
{
    m_table.at(parameterName).SetValue(fcb, TParameter(value, valid));
}

void Aurora::TResults::SetSpectrumType(const Aurora::SpectrumType type)
{
    m_table.clear();
    
    for(auto& np : m_parameterNames)
    {
        m_table[np] = Aurora::Spectrum<TParameter>(type);
    }
}

Aurora::TResults::TResults()
{
    SetSpectrumType(Aurora::SpectrumType::Octave);
}


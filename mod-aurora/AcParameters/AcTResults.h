//
//  AcTResults.hpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#ifndef AcTResults_hpp
#define AcTResults_hpp

#include <aurora.h>

namespace Aurora {

class TResults
{
public:
    
    struct TParameter
    {
        double  value = 0.0;
        bool  isValid = false;
        
        TParameter() { }
        TParameter(const double v,
                   const bool   valid = true);
    };
    
private:
    
    static const std::vector<std::string> m_parameterNames;  /*=
                                                              {
                                                              "Signal",   "Noise",
                                                              "strenGth", "C50",    "C80",  "D50",  "Ts",
                                                              "EDT",      "Tuser",  "T20",  "T30",
                                                              
                                                              "Peakiness", " Millisecondness",  "Impulsiveness",
                                                              
                                                              "St1",     "St2",        "StLate",     // StageParameters (14..)
                                                              "IACC",    "tauIACC",    "widthIACC",  // BinauralParameters (17..)
                                                              "Jlf",     "Jlfc",       "Lj",          // SpatialParameters  (20..)
                                                              }; */
    
    std::unordered_map< std::string, Aurora::Spectrum<TParameter> > m_table;
    
public:
    
    const std::vector<std::string>& Parameters() const
    {
        return m_parameterNames;
    }
    
    const std::vector<float>& Frequencies() const
    {
        return m_table.at("Signal").Frequencies();
    }
    
    Aurora::Spectrum<TParameter>& GetSpectrum(const std::string& parameterName)
    {
        return m_table.at(parameterName);
    }
    
    const TParameter& Get(const std::string& parameterName, const float fcb) const
    {
        return m_table.at(parameterName).GetValue(fcb);
    }
    
    void Set(const std::string& parameterName,
             const float  fcb,
             const double value,
             const bool   valid = true);
    
    void SetSpectrumType(const Aurora::SpectrumType type);
    
    TResults();
};


}



#endif /* AcTResults_hpp */

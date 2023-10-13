//
//  ModulationTransferFunctionsTable.hpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#ifndef ModulationTransferFunctionsTable_hpp
#define ModulationTransferFunctionsTable_hpp

#include <aurora.h>
#include "STISpectrum.h"

namespace Aurora
{

/**
 * @brief Container for STI computations.
 */
class ModulationTransferFunctionsTable
{
    std::map<double, Aurora::STISpectrum> m_table;
    
    std::array<double, 14> m_frequencies =
    {
        0.63, 0.8, 1.0, 1.25, 1.6,  2.0,  2.5,
        3.15, 4.0, 5.0, 6.3,  8.0, 10.0, 12.5
    };
    
public:
    void Clear();
    
    const std::array<double, 14>& GetFrequencies() const { return m_frequencies; }
    
    STISpectrum& GetSpectrum(const double modulationFrequency);
    
    double GetValue(const double modulationFrequency,
                    const float fcb) const;
    
    void SetValue(const double modulationFrequency,
                  const float fcb,
                  const double value);
    
    ModulationTransferFunctionsTable() { Clear(); }
};

}
#endif /* ModulationTransferFunctionsTable_hpp */

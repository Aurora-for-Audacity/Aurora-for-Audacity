//
//  ModulationTransferFunctionsTable.cpp
//  mod-aurora
//
//  Created by Matthew Hamilton on 13/10/2023.
//

#include "ModulationTransferFunctionsTable.h"

void Aurora::ModulationTransferFunctionsTable::Clear()
{
    m_table.clear();
    
    for (auto frq : m_frequencies)
    {
        m_table.emplace(std::make_pair(frq, Aurora::STISpectrum(125.0f, 8000.0f)));
    }
}
            
Aurora::STISpectrum& Aurora::ModulationTransferFunctionsTable::GetSpectrum(const double modulationFrequency)
{
    return m_table[modulationFrequency];
}
            
double Aurora::ModulationTransferFunctionsTable::GetValue(const double modulationFrequency,
                                                               const float fcb) const
{
    return m_table.at(modulationFrequency).GetValue(fcb);
}
            
void Aurora::ModulationTransferFunctionsTable::SetValue(const double modulationFrequency,
                                                             const float fcb,
                                                             const double value)
{
    m_table[modulationFrequency].SetValue(fcb, value);
}

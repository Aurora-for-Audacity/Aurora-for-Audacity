#include <stdio.h>

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

#include <wxcmn.h>
#include "aurora.h"
#include "sigwin.h"
#include "firwin.h"
#include "octfir.h"

    
double SCOctaveBandFirPreset::Fm()
{
    return 1000.0 * pow(10.0, Exp/10.0);
}

double SCOctaveBandFirPreset::Fnyq()
{
    return Rate/2.0;
}

SCOctaveBandFirPreset& SCOctaveBandFirPreset::operator=(const SCOctaveBandFirPreset& right)
{
    Exp     = right.Exp;
    LpN     = right.LpN;     HpN     = right.HpN;
    LpDelta = right.LpDelta; HpDelta = right.HpDelta;
    Rate    = right.Rate;

    OctaveFraction = right.OctaveFraction;
    
    return *this;
}

SCOctaveBandFirPreset::SCOctaveBandFirPreset()
 : Exp(0.0),
   LpN(0),     HpN(0),
   LpDelta(0), HpDelta(0),
   Rate(0),    OctaveFraction(1.0)
{}

SCOctaveBandFirPreset::SCOctaveBandFirPreset(const double exp, 
                                             const double rate,
                                             const int    lpN, 
                                             const int    hpN, 
                                             const double lpDelta,
                                             const double hpDelta,
                                             const double octaveFraction)
 : Exp(exp),
   LpN(lpN),         HpN(hpN),
   LpDelta(lpDelta), HpDelta(hpDelta),
   Rate(rate),       OctaveFraction(octaveFraction)
{}

// ----------------------------------------------------------------------------
void SCOctaveBandFir::Dump(const int id, const char* tag)
{
    FILE* f;
    int k;
    
    char str[32];
    if(m_adbTaps != 0)
    {
        sprintf(str, "/tmp/%s_%d", tag, id);
        std::cout << "Dumping filter " << id << " to " << str << std::endl;
        if((f = fopen(str, "w")) != 0)
        {
            k = 0;
            
            while(k < m_nNtaps)
            {
                fprintf(f, "%.18e\n", m_adbTaps[k++]);
            }
            fclose(f);
        }
    }
}

void SCOctaveBandFir::Dump(std::vector<double>& v, const int id, const char* tag)
{
    FILE* f;
    int k;

    char str[32];
	sprintf(str, "/tmp/%s_%d", tag, id);
	std::cout << "Dumping filter " << id << " to " << str << std::endl;
	if((f = fopen(str, "w")) != 0)
	{
		k = 0;

		while(k < (int)v.size())
		{
			fprintf(f, "%.18e\n", v[k++]);
		}
		fclose(f);
	}
}

void SCOctaveBandFir::Convolve(std::vector<double>& lp, 
                               std::vector<double>& hp)
{
    int k, i;
    
    for(k = 0; k < m_nNtaps; k++)
    {
        m_adbTaps[k] = 0.0;
        
        for(i = 0; i < k+1; i++)
        {
            m_adbTaps[k] += Aurora::Sample(lp[i] * hp[k-i]);
        }
    }
}
   
void SCOctaveBandFir::SpectralInversion()
{
    std::vector<double> hp(m_nNtaps + 1, 0.0);
    std::vector<double> lp(m_nNtaps + 1, 0.0);
    
    Aurora::Sample* hp_taps = m_HPFilter.Get();
    Aurora::Sample* lp_taps = m_LPFilter.Get();
    
    int k;
    for(k = 0; k < m_Preset.HpN; k++)
    {
        hp[k] = -double(hp_taps[k]);
    }
    
    hp[m_Preset.HpN/2] = hp[m_Preset.HpN/2] + 1.0;

    for(k = 0; k < m_Preset.LpN; k++)
    {
        lp[k] = double(lp_taps[k]);
    }

    //m_HPFilter.Dump(int(m_Preset.Exp), "hpfu");
    //Dump(hp, int(m_Preset.Exp), "hpfi");
    //m_LPFilter.Dump(int(m_Preset.Exp), "lpf");

    Convolve(lp, hp);
}

bool SCOctaveBandFir::Process()
{
    if(m_Preset.Exp  == 0.0 ||
       m_Preset.LpN  == 0   ||
       m_Preset.LpN  == 0   ||
       m_Preset.Rate == 0.0)
    {
        m_nErr = ERR_INVALID_PRESET;
        return false;
    }
    
    double alpha    = pow(2.0, 1.0/(2.0*m_Preset.OctaveFraction));
    double hpCutoff = (m_Preset.Fm()/alpha) / m_Preset.Fnyq() - m_Preset.HpDelta;
    double lpCutoff = (m_Preset.Fm()*alpha) / m_Preset.Fnyq() + m_Preset.LpDelta;
   
#ifdef __AUDEBUG__
    printf("HP: %d, %.3f\n", m_Preset.HpN, 
                             hpCutoff);
#endif
    m_HPFilter.Create(m_Preset.HpN, 
                      hpCutoff, 
                      SCSignalWindows::WT_BLACKMAN);
#ifdef __AUDEBUG__
    printf("LP: %d, %.3f\n", m_Preset.LpN, 
                             lpCutoff);
#endif
    m_LPFilter.Create(m_Preset.LpN, 
                      lpCutoff, 
                      SCSignalWindows::WT_BLACKMAN);
    
    if(!m_HPFilter.IsOk())
    {
        m_nErr = ERR_BAD_HP_FILTER;
        return false;
    }
        
    if(!m_LPFilter.IsOk())
    {
        m_nErr = ERR_BAD_LP_FILTER;
        return false;
    }

    m_nNtaps = m_Preset.HpN + m_Preset.LpN - 1;

    if(m_Preset.HpN > m_Preset.LpN)
    {
    	m_LPFilter.Pad(m_Preset.HpN, true);
    	m_nNtaps = 2*m_Preset.HpN - 1;
    }
    
    Destroy();

    m_adbTaps = new Aurora::Sample [m_nNtaps];
    
    SpectralInversion();
    
    return true;
}

void SCOctaveBandFir::Create(SCOctaveBandFirPreset& preset)
{
    m_Preset = preset;
    
    Process();
}

void SCOctaveBandFir::Destroy()
{
    if(m_adbTaps)
    {
        delete [] m_adbTaps;
        m_adbTaps = 0;
    }
}

SCOctaveBandFir::SCOctaveBandFir()
 : m_adbTaps(0),
   m_nNtaps(0),
   m_LPFilter(),
   m_HPFilter(),
   m_Preset(),
   m_nErr(0)
{}

SCOctaveBandFir::SCOctaveBandFir(SCOctaveBandFirPreset& preset)
 : m_adbTaps(0),
   m_nNtaps(0),
   m_LPFilter(),
   m_HPFilter(),
   m_Preset(),
   m_nErr(0)
{ Create(preset); }

SCOctaveBandFir::~SCOctaveBandFir()
{ Destroy(); }

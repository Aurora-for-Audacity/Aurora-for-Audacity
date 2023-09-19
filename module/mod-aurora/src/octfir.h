//
// SCOctaveBandFir
//
// This class, starting from some parameters, should calculate
// the UNI octave bands FIR filters. it uses c++ implementation
// of 'firwin' scipy function.
//
// esseci 2013
//

#pragma once

class SCSignalWindows;
class SCFirWin;

struct SCOctaveBandFirPreset
{   
    double Exp;
    int    LpN;
    int    HpN;
    double LpDelta;
    double HpDelta;
    double Rate;
    double OctaveFraction;
    
    double Fm();
    double Fnyq();
    
    SCOctaveBandFirPreset& operator=(const SCOctaveBandFirPreset& right);
    
    SCOctaveBandFirPreset();
    SCOctaveBandFirPreset(const double exp, 
                          const double rate,
                          const int    lpN, 
                          const int    hpN, 
                          const double lpDelta  = 0.0,
                          const double hpDelta  = 0.0,
                          const double octaveFraction = 1.0);
};

class SCOctaveBandFir
{
  public:
    enum Errors { ERR_OK = 0, 
                  ERR_BAD_HP_FILTER = 600, ERR_BAD_LP_FILTER,
                  ERR_INVALID_PRESET
    };      
    
  protected:
    Aurora::Sample* m_adbTaps;
    int       m_nNtaps;
    
    SCFirWin m_LPFilter;
    SCFirWin m_HPFilter;
    
    SCOctaveBandFirPreset m_Preset;

    int m_nErr;
    
    void Convolve(std::vector<double>& lp,
                  std::vector<double>& hp);
    void SpectralInversion();
    
  public:
    void Dump(const int id, const char* tag);
    void Dump(std::vector<double>& v, const int id, const char* tag);
    
    bool Process();
    
    int  GetError() const { return m_nErr; }
    bool IsOk()     const { return (!m_nErr) ? true : false; }
    
    Aurora::Sample* Get()      const { return m_adbTaps; }
    int       GetNtaps() const { return m_nNtaps; }
    int       GetOrder() const { return m_nNtaps; }
    
    void Create(SCOctaveBandFirPreset& preset);
    void Destroy();
      
    SCOctaveBandFir();
    SCOctaveBandFir(SCOctaveBandFirPreset& preset);
    ~SCOctaveBandFir();
};

//
// SCFirWin
//
// This class, starting from some parameters, should calculate
// a FIR filter coefficients. This code was taken from SciPy,
// so is a sort of c++ implementation of the firwin function.
//
// esseci 2013
//
#pragma once

class SCSignalWindows;

class SCFirWin
{
  public:
   enum Errors { ERR_OK = 0, 
                 ERR_BAD_N = 500,     ERR_BAD_CUTOFF_VEC,
                 ERR_BAD_WINDOW,      ERR_BAD_WIDTH,
                 ERR_BAD_FNYQ,        ERR_BAD_CUTOFF_VALUES,
                 ERR_BAD_CUTOFF_PROG, ERR_PASS_NYQUIST
    };
                 
    
    
  protected:
    int                 m_nNtaps;
    std::vector<double> m_vdbCutoffs;
    double              m_dbWidth;
    int                 m_nWindowType;
    bool                m_bPassZero;
    bool                m_bPassNyquist;
    bool                m_bScale;
    double              m_dbFnyq;
    
    int                 m_nErr;

    SCSignalWindows     m_Window;
    Aurora::Sample*     m_adbTaps;
    
    void Sinc(std::vector<double>& x);
    
    bool Process();
    
  public:     
    void Dump(const int id, const char* tag);
    void TestConv(int M, int N); // REMOVE BEFORE FLIGHT
    void Pad(const int M, const bool center = false);
    
    Aurora::Sample* Get()          const { return m_adbTaps; }
    int       GetTapsCount() const { return m_nNtaps; }
    int       GetOrder()     const { return m_nNtaps; }
      
    int  GetError() const { return m_nErr; }
    bool IsOk()     const { return (m_nErr == ERR_OK) ? true : false; }

    bool Create(const int    N, 
                double       cutoff, 
                const int    window   = 10,
                const double width    = 0.0,
                const bool   passZero = true,
                const bool   scale    = true,
                const double fnyq     = 1.0);

    bool Create(const int    N, 
                double*      cutoffs, 
                const int    cutoffsCount, 
                const int    window   = 10,
                const double width    = 0.0,
                const bool   passZero = true,
                const bool   scale    = true,
                const double fnyq     = 1.0);
    
    void Destroy();
      
      
    SCFirWin();
    SCFirWin(const int    N, 
             double       cutoff, 
             const int    window   = 10,
             const double width    = 0.0,
             const bool   passZero = true,
             const bool   scale    = true,
             const double fnyq     = 1.0);
    
    SCFirWin(const int    N, 
             double*      cutoffs, 
             const int    cutoffsCount, 
             const int    window   = 10,
             const double width    = 0.0,
             const bool   passZero = true,
             const bool   scale    = true,
             const double fnyq     = 1.0);
    
    ~SCFirWin();
    
};

//
// SCSignalWindows
//
// This class, generates windows used in signal processing
//
// esseci 2013
//
#pragma once

class SCSignalWindows
{
  public:
    enum Errors { ERR_OK = 0, ERR_NOT_IMPL = 500 };
    
    enum Types { 
        WT_BARTHANN,
        WT_BARTLETT,
        WT_BLACKMAN,
        WT_BLACKMANHARRIS,
        WT_BOHMAN,
        WT_BOXCAR,
        WT_CHEBWIN,            //  Dolph-Chebyshev window.
        WT_FLATTOP,            // Flat top window.
        WT_GAUSSIAN,           // Return a Gaussian window standard-deviation std.
        WT_GENERALGAUSSIAN,    // Return a window with a generalized Gaussian shape.
        WT_HAMMING,            // The Hamming window.
        WT_HANN,               // The Hann window.
        WT_KAISER,             // Return a Kaiser window of length M with shape parameter beta.
        WT_NUTTALL,            // A minimum 4-term Blackman-Harris window according to Nuttall.
        WT_PARZEN,             // The Parzen window.
        WT_SLEPIAN,            // The slepian window.
        WT_TRIANG
    };
    
  protected:
    Aurora::Sample*    m_smpWindow;
    unsigned int m_unLength;
    int          m_nType;
    int          m_nErr;
    
    void BartlettHann();
    void Bartlett();
    void Blackman(const double a0, const double a1, 
                  const double a2, const double a3 = 0.0,
                  const double a4 = 0.0);
    void Bohman(); // not finished
    void Boxcar();
    void DolphChebyshev(const double alpha); // not finished 
    void Gaussian(const double sigma, const double p = 1.0);
    void Hamming(const double alpha);
    void Hann();
    double MBesselZeroOrder(const double n);
    void   Kaiser(const double beta);
    void Parzen();  // not finished
    void Slepian(); // not finished
    void Triang();
      
  public:
    static double KaiserBeta(const double atten);
    static double KaiserAtten(const int N, const double width);
    static double KaiserOrd(int* N, const double ripple, const double width);
      
    Aurora::Sample*    Get()       const { return m_smpWindow; }
    unsigned int GetLength() const { return m_unLength; }
    int          GetType()   const { return m_nType; }
    int          GetError()  const { return m_nErr; }
      
    void Create(unsigned int M, const int type, double p1 = 0.0, double p2 = 0.0);
    void Destroy();
    
    SCSignalWindows();
    SCSignalWindows(unsigned int M, const int type, double p1 = 0.0, double p2 = 0.0);    
    ~SCSignalWindows();
};

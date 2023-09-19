/******************************
 *
 *  Third Octave Band Filter
 *
 ******************************/
 
#ifndef __octfilt_h__
#define __octfilt_h__

#define DUMP_TAPS

class SCFilter
{
  public:
    enum Errors { NO_ERR = 0,
    	          ERR_FM = 700,        ERR_RATE,
    	          ERR_CREATING_FILTER, ERR_NOT_IMPL };
    
  protected:
      
    // Attributes
    int    m_nErr = NO_ERR;
    
    double m_dbRate;
    double m_dbFnyq;
    int    m_nOrder;
                
    std::vector<Aurora::Sample> m_B;
    std::vector<Aurora::Sample> m_A;

    bool   m_bIsFir = true;

    // Private methods

    virtual void IIR(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length);
    virtual void FIR(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length);
   
    void init(const Aurora::Sample* b,
              const Aurora::Sample* a = 0);
    
    virtual void init() {}

  public:
#ifdef DUMP_TAPS      
    void Dump(const int id, const char* tag);
#endif
    
    void UseFIR() { m_bIsFir = true;  init(); }
    void UseIIR() { m_bIsFir = false; init(); }

    bool IsOk() const { return (m_nErr == NO_ERR) ? true : false; }
    int  GetError() const { return m_nErr; }
private:   
    bool Apply(Aurora::Sample* yt, const Aurora::Sample* xt, const Aurora::SampleCount length);
public:
    bool Apply(const Aurora::TSampleVector& in, Aurora::TSampleVector& out);

    void SetRate(const double rate) { m_dbRate  = rate; init();  }

    // Firwin like constructor
    SCFilter(const int    N,
             double       cutoff,
             const int    window   = 10,
             const double width    = 0.0,
             const bool   passZero = true,
             const bool   scale    = true,
             const double fnyq     = 1.0);

    SCFilter(const int N,
             Aurora::Sample* b,
             Aurora::Sample* a = 0,
             const bool   isFir = true,
             const double rate  = 48000.0);

    virtual ~SCFilter() { }
};

class SCOctaveBandFilterBase : public SCFilter
{
protected:
    double m_dbFmExp;

public:
    virtual void Create(const double rate,
                        const double fmExp,
                        const bool   isFir = true) {}

    SCOctaveBandFilterBase();
    SCOctaveBandFilterBase(const double rate,
                           const double fmExp,
                           const bool   isFir = true);

	virtual ~SCOctaveBandFilterBase() {}
};

class SCOctaveBandFilter : public SCOctaveBandFilterBase
{
  public:
    void Create(const double rate,
                const double fmExp,
                const bool   isFir = true);

    SCOctaveBandFilter();
    SCOctaveBandFilter(const double rate,
                       const double fmExp,
                       const bool   isFir = true);
};

class SCThirdOctaveBandFilter : public SCOctaveBandFilterBase
{
  public:
    void Create(const double rate,
                const double fmExp,
                const bool   isFir = true);

    SCThirdOctaveBandFilter();
    SCThirdOctaveBandFilter(const double rate,
                            const double fmExp,
                            const bool   isFir = true);
};
 
 
#endif //__octfilt_h__

/******************************
 *
 *  Third Octave Band Analysis
 *
 ******************************/

//class Aurora::AudioTrack;
class SCFilter;
class SCOctaveBandFilter;
class SCThirdOctaveBandFilter;

class SCOctaveBandAnalysisBase
{
  public:
    enum { IIR_B = 0, IIR_A };
    
  protected:
    
    // Attributes
    double        m_dbRate             = 0.0;
    bool          m_bIsPreFilteringFIR = false;
    unsigned int  m_unCurrentBand      = 10000;
    unsigned int  m_unBandsCount       = 10;
    
    Aurora::AudioTrack* m_pTrack = nullptr;
    
    Aurora::AudioSamples m_xtLp;
    Aurora::AudioSamples m_xtRes;

    int       m_nDecimationFactor = 0;
    Aurora::SampleCount m_smpcMinLength = 0;
    double    m_dbG = 12.0;

    std::unique_ptr<SCFilter> m_pLpFilter;
    std::vector< std::unique_ptr<SCOctaveBandFilterBase> > m_filterBank;

    virtual void ChooseLPFilterSet();
    virtual int  FcbToIndex(const float fcb) = 0;
    
    void Decimate(const unsigned int N, 
                  Aurora::Sample* dest, 
                  Aurora::Sample* src, 
                  Aurora::SampleCount length);

    void Decimate(const unsigned int N,
                  const Aurora::AudioSamples& in,
                  Aurora::AudioSamples& out);
    
public:
    
    // Interface
    virtual bool Set(Aurora::AudioTrack* track)    { return false; }
    virtual int  Filter() = 0;
    virtual int  FilterForBand(const float fcb) = 0;

    unsigned int GetBandsCount() const { return m_unBandsCount; }
    
    // 'ctors
    SCOctaveBandAnalysisBase(const double       rate,
                             const bool         firPreFiltering,
                             const unsigned int bandsCount);
    virtual ~SCOctaveBandAnalysisBase() { }

};

class SCOctaveBandAnalysis : public SCOctaveBandAnalysisBase
{
private:
	enum { FBP_16k = 0, FBP_8k };

    virtual int FcbToIndex(const float fcb) override;

public:    
 
    // Interface
    bool Set(Aurora::AudioTrack* track) override;
    int  Filter() override;
    int  FilterForBand(const float fcb) override;
    
    // 'ctors
    SCOctaveBandAnalysis(const double rate,
                         const bool   firPreFiltering = true);
    ~SCOctaveBandAnalysis() { } 

};

class SCThirdOctaveBandAnalysis : public SCOctaveBandAnalysisBase
{
  private:
	enum { FBP_20k = 0, FBP_16k, FBP_12k5, FBP_10k };

    virtual int FcbToIndex(const float fcb) override;
          
    // Interface
    bool Set(Aurora::AudioTrack* track) override;
    int  Filter() override;
    int  FilterForBand(const float fcb) override;

    // 'ctors
    SCThirdOctaveBandAnalysis(const double rate, 
                              const bool   firPreFiltering = true);
    ~SCThirdOctaveBandAnalysis() { }
};

// ----------------------------------------------------------------------------
class SCOctaveBandData
{
  private:
    double       m_dbRate;
    double*      m_adbData;    
    unsigned int m_unLength;
    float        m_flFm;
    
  public:
    void Init(const unsigned int length, const bool zeroFill = false);
    
    double  GetRate() const { return m_dbRate; }
    double* GetData() const { return m_adbData; }
    unsigned int GetLength() const { return m_unLength; }
    float GetNominalFm() const { return m_flFm; }
    
    void SetRate(const double rate) { m_dbRate = rate; }
    void SetData(double* data, const unsigned int length = 0);
    void SetLength(const unsigned int length) { m_unLength = length; }
    void SetNominalFm(const float fm) { m_flFm = fm; }

    // 'ctors    
    SCOctaveBandData(const double rate = 48000.0);
    SCOctaveBandData(double* data, 
                     const unsigned int length,
                     const float fm,
                     const double rate = 48000.0);
    ~SCOctaveBandData();
};

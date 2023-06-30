#include <aurora.h>

float g_aflOctaveBandFc[] = {   31.5,   63.0,  100.0,  50.0,   500.0,
                              1000.0, 2000.0, 4000.0, 8000.0, 16000.0 };

float g_aflThirdOctaveBandFc[] = {   25.0,    31.5,    40.0,    50.0,    63.0,
                                     80.0,   100.0,   125.0,   160.0,   200.0,
                                    250.0,   315.0,   400.0,   500.0,   630.0,
                                    800.0,  1000.0,  1250.0,  1600.0,  2000.0,
                                   2500.0,  3150.0,  4000.0,  5000.0,  6300.0,
                                   8000.0, 10000.0, 12500.0, 16000.0, 20000.0 };

// ----------------------------------------------------------------------------
// scipy: signal.firwin(31, 1.0/2.0, window='hamming')
Aurora::Sample g_adbLpFilterTaps_48[] = {  -1.700396903673608018e-03, 1.757971997228029832e-18,
                                   2.937331570890679838e-03, -3.276763392097393135e-18,
                                   -6.730091366404413432e-03, 6.051733351876545153e-18,
                                   1.409388790399193625e-02, -9.603064360709015505e-18,
                                   -2.678503582005385686e-02, 1.331669902433829708e-17,
                                   4.909896059357541503e-02, -1.655051617211110519e-17,
                                   -9.693833277630080347e-02, 1.874535944087102577e-17,
                                   3.156195633244823218e-01, 5.008082269469846803e-01,
                                   3.156195633244823218e-01, 1.874535944087102268e-17,
                                   -9.693833277630083123e-02, -1.655051617211110827e-17,
                                   4.909896059357543585e-02, 1.331669902433830170e-17,
                                   -2.678503582005387768e-02, -9.603064360709020128e-18,
                                   1.409388790399193972e-02, 6.051733351876545153e-18,
                                   -6.730091366404411697e-03, -3.276763392097394291e-18,
                                   2.937331570890678537e-03, 1.757971997228029832e-18,
                                   -1.700396903673608018e-03 };

Aurora::Sample g_adbLpFilterB_48[] = { 0.0053233 ,
                               0.04258637,  0.14905228,  
                               0.29810456,  0.3726307 ,  
                               0.29810456,  0.14905228,  
                               0.04258637,  0.0053233  };
                            
Aurora::Sample g_adbLpFilterA_48[] = {  1.0,
                               -0.94338214,  2.00431824, 
                               -1.60880355,  1.45032361, 
                               -0.83004411,  0.39328284, 
                               -0.12313816,  0.0217768  };

// ----------------------------------------------------------------------------

void Dump(Aurora::Sample* data, const int length, const int id, const char* tag)
{
    FILE* f;
    int k;
    
    char str[32];
    if(data != 0)
    {
        sprintf(str, "/tmp/%s_%d", tag, id);
        std::cout << "Dumping Lp " << id << " to " << str << std::endl;
        if((f = fopen(str, "w")) != 0)
        {
            k = 0;
            
            while(k < length)
            {
                fprintf(f, "%.18e\n", data[k++]);
            }
            fclose(f);
        }
    }
}

void SCOctaveBandAnalysisBase::ChooseLPFilterSet()
{

	if(m_bIsPreFilteringFIR)
	{
		// Parameter used in the SciPy 'Decimate' function
		m_pLpFilter.reset(new SCFilter(31, 0.5, SCSignalWindows::WT_HAMMING));
	}
	else
	{
		// IIR stuffs ... to be removed ....
		Aurora::Sample* a = nullptr;
		Aurora::Sample* b = nullptr;
		int order = 0;

		if(m_dbRate == 48000.0)
		{
			a = g_adbLpFilterA_48;
			b = g_adbLpFilterB_48;
			order = sizeof(g_adbLpFilterB_48);
		}
//                if(m_dbRate == 44100.0)
//                    return g_adbLpFilterB_44k1;

		order /= sizeof(Aurora::Sample);
		m_pLpFilter.reset(new SCFilter((const int)order, b, a, m_dbRate));
	}

}

void SCOctaveBandAnalysisBase::Decimate(const unsigned int N,
                                        Aurora::Sample* dest,
                                        Aurora::Sample* src,
                                        Aurora::SampleCount  length)
{
    Aurora::SampleCount k, i;
    for(k = 0, i = 0; k < length; k += N, i++)
    {
        dest[i] = src[k];
    }   
}

void SCOctaveBandAnalysisBase::Decimate(const unsigned int N,
                                        const Aurora::AudioSamples& in,
                                        Aurora::AudioSamples& out)
{
    auto src = in.CSamples();
    auto dest = out.Samples();

    Aurora::SampleCount k, i;

    for (k = 0, i = 0; k < in.Length(); k += N, i++)
    {
        dest[i] = src[k];
    }
}

SCOctaveBandAnalysisBase::SCOctaveBandAnalysisBase(const double rate,
                                                   const bool   firPreFiltering,
                                                   const unsigned int bandsCount)
: m_dbRate(rate),
  m_bIsPreFilteringFIR(firPreFiltering),
  m_unBandsCount(bandsCount)
{
    assert(m_dbRate == 48000.0);
}

// ----------------------------------------------------------------------------
int SCOctaveBandAnalysis::FcbToIndex(const float fcb)
{
    // center band frequency is given by fcb = 1000.0 * 10^(x/10)
    // where x = [-16, 13]
    const float x = std::round(10.0f * std::log10(fcb / 1000.0f));
    const int index = 5 + (int)(x / 3.0f);
    return index;
}

bool SCOctaveBandAnalysis::Set(Aurora::AudioTrack* track)
{
    // 0. Init filters
    m_pLpFilter.reset(nullptr);
    
    ChooseLPFilterSet();
    
    m_filterBank[FBP_16k]->Create(m_dbRate, 12.0);
    m_filterBank[FBP_8k]->Create(m_dbRate,   9.0);

    m_pTrack = track;
    m_unCurrentBand = Aurora::SpectrumBand::Octave_f16k + 1;

    Aurora::SampleCount length = m_pTrack->GetLength();

    m_xtLp.Resize(length);
    m_xtRes.Resize(length);

    m_nDecimationFactor = 2;
    m_dbG = 12.0;

    m_pLpFilter->Dump(0, "ap_lp");
    m_filterBank[FBP_16k]->Dump(0, "ap_bp16k");
    m_filterBank[FBP_8k]->Dump(0, "ap_bp8k");

    return true;
}

int SCOctaveBandAnalysis::Filter()
{
	if(m_unCurrentBand == 0 || m_unCurrentBand == 10000)
	{
		return -1;
	}

    const Aurora::AudioSamples& in = *m_pTrack;
    Aurora::AudioSamples&      out = m_pTrack->Filtered();

    auto minLength = (Aurora::SampleCount)( (m_dbRate/m_nDecimationFactor)/
    	                                   (2000.0*pow(10.0, (m_dbG-3.0)/10.0)) );
    m_unCurrentBand--;
    
    switch(m_unCurrentBand)
    {
        case Aurora::SpectrumBand::Octave_f16k:
            m_filterBank[FBP_16k]->Apply(in, out);
            m_dbG -= 3.0;
            break;

        case Aurora::SpectrumBand::Octave_f8k:
            m_filterBank[FBP_8k]->Apply(in, out);
            m_dbG -= 3.0;
            break;
    
        case Aurora::SpectrumBand::Octave_f4k:
            m_xtRes.Copy(in);

        default:
        {
            auto lengthRes = out.Length();

            if ((m_dbG < -15.0) || (lengthRes / 2 < minLength))
            {
                return -2;
            }
            // 2. Decimate
            m_pLpFilter->Apply(m_xtRes, m_xtLp);
            Decimate(2, m_xtRes.Samples(), m_xtLp.Samples(), lengthRes);

            lengthRes /= 2;
            m_dbRate /= 2;
            m_xtRes.ZeroPad(lengthRes);

            // 3. Analyze next band
            out.Resize(lengthRes);
            out.SetRate(m_dbRate);
            m_filterBank[FBP_8k]->Apply(m_xtRes, out);

            // 4. prepare for next
            m_nDecimationFactor *= 2;
            m_dbG -= 3.0;
            break;
        }
    }
    return int(m_unCurrentBand);
}

int SCOctaveBandAnalysis::FilterForBand(const float fcb)
{
    assert(fcb >= 31.5f || fcb <= 16000.0f);
    assert(m_pTrack != nullptr);

    const Aurora::AudioSamples& in = *m_pTrack;
    Aurora::AudioSamples&      out = m_pTrack->Filtered();

    Aurora::SampleCount length    = m_pTrack->GetLength();
    Aurora::SampleCount lengthRes = length;

    double rate = m_pTrack->GetRate();

    if (fcb == 16000.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_16k]->Apply(in, out);

        return (int)Aurora::SpectrumBand::Octave_f16k;
    }

    if (fcb == 8000.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_8k]->Apply(in, out);
        return (int)Aurora::SpectrumBand::Octave_f8k;
    }

    // manage all other bands.
    const int targetBandIndex = FcbToIndex(fcb);
    Aurora::SampleCount minLength = 0;

    const double decFactor = std::pow(2.0, double(Aurora::SpectrumBand::Octave_f8k - targetBandIndex));
    const double         g = 3.0 * double(targetBandIndex) - 15.0;

    rate /= decFactor;
    minLength = (Aurora::SampleCount)( rate/ (1000.0*pow(10.0, g/10.0)) );

	if( Aurora::SampleCount(lengthRes / decFactor) < minLength )
	{
#ifdef __AUDEBUG__
		std::cout << "IR too short for band" << targetBandIndex
				    << " should be at least"   <<  int(decFactor*minLength)
					<< "\n";
#endif
		return -2;
	}

    SCFilter LpFilter(31, 1.0/decFactor, SCSignalWindows::WT_HAMMING);
    m_xtRes.Copy(in);
    LpFilter.Apply(m_xtRes, m_xtLp);
    Decimate(int(decFactor), m_xtLp, m_xtRes);

    lengthRes = Aurora::SampleCount(double(length) / decFactor); //n
    m_xtRes.ZeroPad(lengthRes);

    out.Resize(lengthRes);
    out.SetRate(rate);
    m_filterBank[FBP_8k]->Apply(m_xtRes, out);

    return targetBandIndex;
}

SCOctaveBandAnalysis::SCOctaveBandAnalysis(const double rate,
                                           const bool   firPreFiltering)
: SCOctaveBandAnalysisBase(rate, firPreFiltering, 10)
{
    for (int i = 0; i < 2; i++)
    {
        m_filterBank.emplace_back(new SCOctaveBandFilter);
    }
}

// ----------------------------------------------------------------------------
int SCThirdOctaveBandAnalysis::FcbToIndex(const float fcb)
{
    // center band frequency is given by fcb = 1000.0 * 10^(x/10)
    // where x = [-16, 13]
    const float x = std::round(10.0f * std::log10(fcb / 1000.0f));
    const int index = 5 + (int)(x / 3.0f);
    return index;
}

bool SCThirdOctaveBandAnalysis::Set(Aurora::AudioTrack* track)
{
    // 0. Init filters
    m_pLpFilter.reset(nullptr);
    
    ChooseLPFilterSet();

    m_filterBank[FBP_20k]->Create(m_dbRate,  13.0);
    m_filterBank[FBP_16k]->Create(m_dbRate,  12.0);
    m_filterBank[FBP_12k5]->Create(m_dbRate, 11.0);
    m_filterBank[FBP_10k]->Create(m_dbRate,  10.0);

    m_pTrack = track;
    m_unCurrentBand = Aurora::SpectrumBand::ThirdOctave_f20k + 1;

    auto length = m_pTrack->GetLength();

    m_xtLp.Resize(length);
    m_xtRes.Resize(length);

    m_nDecimationFactor = 2;
    m_dbG = 12.0;

    return true;
}

int SCThirdOctaveBandAnalysis::Filter()
{
	if(m_unCurrentBand == 0 || m_unCurrentBand == 10000)
	{
		return -1;
	}
    const Aurora::AudioSamples& in = *m_pTrack;
    Aurora::AudioSamples&      out = m_pTrack->Filtered();

    Aurora::SampleCount length    = 0;
    Aurora::SampleCount lengthRes = 0;

    auto minLength = (Aurora::SampleCount)(  (m_dbRate / m_nDecimationFactor)
                                           / (2000.0 * std::pow(10.0, (m_dbG-3.0)/10.0)) );
    m_unCurrentBand--;

    switch(m_unCurrentBand)
    {
        case Aurora::SpectrumBand::ThirdOctave_f20k:
            m_filterBank[FBP_20k]->Apply(in, out);
			m_dbG -= 1.0;
			break;

        case Aurora::SpectrumBand::ThirdOctave_f16k:
            m_filterBank[FBP_16k]->Apply(in, out);
			m_dbG -= 1.0;
			break;

       case Aurora::SpectrumBand::ThirdOctave_f12k5:
           m_filterBank[FBP_12k5]->Apply(in, out);
			m_dbG -= 1.0;
			break;

       case Aurora::SpectrumBand::ThirdOctave_f10k:
           m_filterBank[FBP_10k]->Apply(in, out);
			m_dbG -= 1.0;

			break;

       case Aurora::SpectrumBand::ThirdOctave_f8k:
           m_xtRes.Copy(in);

       case Aurora::SpectrumBand::ThirdOctave_f4k:
       case Aurora::SpectrumBand::ThirdOctave_f2k:
       case Aurora::SpectrumBand::ThirdOctave_f1k:
       case Aurora::SpectrumBand::ThirdOctave_f500:
       case Aurora::SpectrumBand::ThirdOctave_f250:
       case Aurora::SpectrumBand::ThirdOctave_f125:
       case Aurora::SpectrumBand::ThirdOctave_f63:
       case Aurora::SpectrumBand::ThirdOctave_f31_5:

       	   length = in.Length();

           lengthRes = length;

           if( (m_dbG < -15.0) || (lengthRes / 2 < minLength) )
           {
        	   return -2;
           }

		   // 2. Decimate
           m_pLpFilter->Apply(m_xtRes, m_xtLp);
           Decimate(2, m_xtLp, m_xtRes);		   

		   lengthRes /= 2;
           m_xtRes.ZeroPad(lengthRes);

		   // 3. Analyze next band
		   m_pTrack->Filtered().Resize(lengthRes);
		   m_pTrack->Filtered().SetRate(m_dbRate / m_nDecimationFactor);
           m_filterBank[FBP_16k]->Apply(m_xtRes, out);

		   // 4. prepare for next
		   m_dbG -= 1.0;
    	   break;

       case Aurora::SpectrumBand::ThirdOctave_f6k3:
       case Aurora::SpectrumBand::ThirdOctave_f3k15:
       case Aurora::SpectrumBand::ThirdOctave_f1k6:
       case Aurora::SpectrumBand::ThirdOctave_f800:
       case Aurora::SpectrumBand::ThirdOctave_f400:
       case Aurora::SpectrumBand::ThirdOctave_f200:
       case Aurora::SpectrumBand::ThirdOctave_f100:
       case Aurora::SpectrumBand::ThirdOctave_f50:
       case Aurora::SpectrumBand::ThirdOctave_f25:
       {
           auto length = in.Length();
           auto lengthRes = length;

           if ((m_dbG < -15.0) || (lengthRes / 2 < minLength))
           {
               return -2;
           }
           out.Resize(lengthRes);
           out.SetRate(m_dbRate / m_nDecimationFactor);
           m_filterBank[FBP_12k5]->Apply(m_xtRes, out);
           m_dbG -= 1.0;
           break;
       }
       case Aurora::SpectrumBand::ThirdOctave_f5k:
       case Aurora::SpectrumBand::ThirdOctave_f2k5:
       case Aurora::SpectrumBand::ThirdOctave_f1k25:
       case Aurora::SpectrumBand::ThirdOctave_f630:
       case Aurora::SpectrumBand::ThirdOctave_f315:
       case Aurora::SpectrumBand::ThirdOctave_f160:
       case Aurora::SpectrumBand::ThirdOctave_f80:
       case Aurora::SpectrumBand::ThirdOctave_f40:
       {
           auto length = out.Length(); // TBR ???
           auto lengthRes = length;

           if ((m_dbG < -15.0) || (lengthRes / 2 < minLength))
           {
               return -2;
           }
           m_pTrack->Filtered().Resize(lengthRes);
           out.SetRate(m_dbRate / m_nDecimationFactor);
           m_filterBank[FBP_10k]->Apply(m_xtRes, out);

           m_nDecimationFactor *= 2;
           m_dbG -= 1.0;
           break;
       }
    }
        
    return int(m_unCurrentBand);
}
 
int SCThirdOctaveBandAnalysis::FilterForBand(const float fcb)
{
    assert(fcb >= 25.0f || fcb <= 20000.0f);
    assert(m_pTrack != nullptr);

    const Aurora::AudioSamples& in = *m_pTrack;
    Aurora::AudioSamples&      out = m_pTrack->Filtered();

    Aurora::SampleCount length    = m_pTrack->GetLength();
    Aurora::SampleCount lengthRes = length;

    double rate = m_pTrack->GetRate();

    if (fcb == 20000.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_20k]->Apply(in, out);
        return (int)Aurora::SpectrumBand::ThirdOctave_f20k;
    }

    if (fcb == 16000.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_16k]->Apply(in, out);
        return (int)Aurora::SpectrumBand::ThirdOctave_f16k;
    }

    if (fcb == 12500.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_12k5]->Apply(in, out);
        return (int)Aurora::SpectrumBand::ThirdOctave_f8k;
    }

    if (fcb == 10000.0f)
    {
        out.Resize(length);
        out.SetRate(rate);
        m_filterBank[FBP_10k]->Apply(in, out);
        return (int)Aurora::SpectrumBand::ThirdOctave_f10k;
    }

    // all the other bands managed here:

    const int targetBandIndex = FcbToIndex(fcb);
    Aurora::SampleCount minLength = 0;

    int targetBandGroup = 0;
    int filterIdx       = 0;

    if((targetBandIndex - 2) < 0)
    {
        targetBandGroup = 0;
        filterIdx = targetBandIndex + 1;
    }
    else
    {
        targetBandGroup = (targetBandIndex - 2)/3 + 1;
        filterIdx       = (3 * targetBandGroup - targetBandIndex) + 2;
    }

    const double decFactor = std::pow(2.0, double(9 - targetBandGroup));
    const double         g = double(targetBandIndex) - 16.0;

    rate /= decFactor;
    minLength = (Aurora::SampleCount)( rate/ (1000.0*pow(10.0, g/10.0)) );

 	if( Aurora::SampleCount(lengthRes / decFactor) < minLength )
 	{
#ifdef __AUDEBUG__
 		std::cout << "IR too short for band" << targetBandIndex
 				    << " should be at least"   <<  int(decFactor*minLength)
 					<< "\n";
#endif
 		return -2;
 	}

    SCFilter LpFilter(31, 1.0/decFactor, SCSignalWindows::WT_HAMMING);

    m_xtRes.Copy(in);
    LpFilter.Apply(m_xtRes, m_xtLp);
    Decimate(int(decFactor), m_xtLp, m_xtRes);

    lengthRes = Aurora::SampleCount(double(length) / decFactor); //n
    m_xtRes.ZeroPad(lengthRes);

    out.Resize(lengthRes);
    out.SetRate(rate);
    m_filterBank[filterIdx]->Apply(m_xtRes, out);

    return targetBandIndex;
}

SCThirdOctaveBandAnalysis::SCThirdOctaveBandAnalysis(const double rate, 
                                                     const bool   firPreFiltering)
  : SCOctaveBandAnalysisBase(rate, firPreFiltering, 30)
{
    for (int i = 0; i < 4; i++)
    {
        m_filterBank.emplace_back(new SCThirdOctaveBandFilter);
    }
}

// ----------------------------------------------------------------------------
void SCOctaveBandData::Init(const unsigned int length, const bool zeroFill)
{
    if(length > 0)
        m_unLength = length;
    
    if(m_adbData)  
        delete [] m_adbData;
        
    assert(m_unLength > 0);
    m_adbData = new double [m_unLength];
    
    if(zeroFill)
        memset(m_adbData, 0, length * sizeof(double));    
}

void SCOctaveBandData::SetData(double* data, const unsigned int length)
{    
    Init(length);
    memcpy(m_adbData, data, length * sizeof(double));
}
    
SCOctaveBandData::SCOctaveBandData(const double rate)
 : m_dbRate(rate),
   m_adbData(0),
   m_unLength(0),
   m_flFm(0.0)
{}

SCOctaveBandData::SCOctaveBandData(double* data,
                                   const unsigned int length,
                                   const float fm,
                                   const double rate)
 : m_dbRate(rate),
   m_adbData(0),
   m_unLength(length),
   m_flFm(fm)
{
    SetData(data);
}

SCOctaveBandData::~SCOctaveBandData()
{
    if(m_adbData) 
        delete [] m_adbData;
}

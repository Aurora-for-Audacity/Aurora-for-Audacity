/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common libraries

  affilter.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_AFFILTER_H__
#define __AURORA_AFFILTER_H__

//class Aurora::AudioTrack;

namespace Aurora
{
	//----------------------------------------------------------------------------
	// Filter
	/// \brief The base class for IIR filtering.
	/// This class provides a container for audio data with some fundamental
	/// operations/calculus on it.
	//----------------------------------------------------------------------------

	class Filter
	{

	public:
		enum Errors
		{
			ERR_OK = 5000,
			ERR_DATA,
			ERR_LENGTH_MISMATCH,
			ERR_RATE_MISMATCH
		};

		   /// Implemented filters tags.
	    enum FilterType
	    {
			BPF = 0, LPF, HPF, NotchF,

	        FLT_LIN,     // not implemented??
	        FLT_ITU,     // Band-pass (160-5500 Hz)
	        FLT_IEC,     // A-weighting filter
	        FLT_ISO,     // 2631 whole combined (???)
	        FLT_UNI,
	        FLT_NONE      // Useful for selections...
	    };

	   private:
	    int  m_nErr;
		int  m_nCh;


		/// Time domain IIR filter function, called by every filtering method.
		/// \f[ y[n] = 2 ( \alpha (x[n] + \mu x[n-1] + \sigma x[n-2]) + \gamma y[n-1] - \beta y[n-2] ) \f]
		/// \param alpha, beta, gamma, mu, sigma - The filter parameters
		void IIR(AudioTrack& dst, AudioTrack& src,
				 const double alpha, const double beta, const double gamma,
			     const double mu,    const double sigma);

	  public:

		/// Filtering function wrapper for low-pass, band-pass, high-pass and notch
		/// \param type - Filter type, must be one of LPF, BPF, HPF, NotchF.
		/// \param dst - Destination data container
		/// \param src - Source data container
		/// \param f0 - Cutoff frequency
		/// \param fQ - Q factor.
		void DoFilter(AudioTrack& dst, AudioTrack& src,
				      const int type, const double f0, const double fQ);

		/// A-weigthed filter.
		/// \param dst - Destination data container
		/// \param src - Source data container
		void AFilter(AudioTrack& dst, AudioTrack& src); //... bool preserve = true);

		/// High-pass filter, cutoff 20 Hz.
		/// \param dst - Destination data container
		/// \param src - Source data container
		void LFilter(AudioTrack& dst, AudioTrack& src);

		/// ISO 8042 filter. Used in accelerometer measures for the generic axis
		/// ponderation.
		/// \param dst - Destination data container
		/// \param src - Source data container
		void ISO_8041_Filter(AudioTrack& dst, AudioTrack& src);

		/// ITU P56 filter. Pass band filter 160 - 5500 Hz
		/// \param dst - Destination data container
		/// \param src - Source data container
		void ITU_P56_Filter(AudioTrack& dst, AudioTrack& src);

		/// UNI 9916 filter. Low pass, cutoff 250 Hz. Used in vibration measures.
		/// \param dst - Destination data container
		/// \param src - Source data container
		void UNI_9916_Filter(AudioTrack& dst, AudioTrack& src);

		/// IEC 61260 Octave bands filter.
		/// \param dst - Destination data container
		/// \param src - Source data container
		/// \param dbFc - Center band frequency in Hz, one of 31.5, 63, 125, 250, 500
		///               1000, 2000, 4000, 8000, 16000.
		void OctaveFilter(AudioTrack& dst, AudioTrack& src, const double dbFc);

		/// DC component remover.
		/// \param track - Destination data container
		void RemoveMean(AudioTrack& track);

		void SetChannel(const int ch) { m_nCh = ch; }
		int GetChannel() const { return m_nCh; }

		int GetError() const { return m_nErr; }

	    Filter(const int ch = 0);
	    ~Filter();
	};
}

#endif //__AURORA_AFFILTER_H__

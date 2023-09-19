/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  afaudio.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_AFAUDIO_H__
#define __AURORA_AFAUDIO_H__

//----------------------------------------------------------------------------
// AudioTrackBase
/// \brief The base class of audio data holder.
/// This class provides a container for audio data with some fundamental
/// operations/calculus on it.
//----------------------------------------------------------------------------
namespace Aurora
{
    typedef std::pair<SampleCount, SampleCount> Range;

    class AudioSamples : public TSamplesVector
    {
        double m_rate      = 0;     ///< Sampling rate in Hz.
        double m_fullscale = 120.0; ///< Full scale in dB
    public:

        double GetRate() const { return m_rate;  }
        double GetFullScale() const { return m_fullscale;  }

        SampleCount TimeToSamples(const double seconds) const;
        
        virtual void SetRate(const double r) { m_rate = r;  }
        virtual void SetFullscale(const double fs) { m_fullscale = fs;  }

        void foreach(const Range& range,
                     std::function<void(Sample sample)> handler);

        void Copy(const AudioSamples& as);

        AudioSamples& operator=(AudioSamples&& as);
        AudioSamples& operator=(const AudioSamples& as) = delete;

        AudioSamples() { }
        AudioSamples(const SampleCount length, const double rate)
            : ScalarVector(length), m_rate(rate)
        { }

        AudioSamples(AudioSamples&& as)
            : ScalarVector(std::move(as)), m_rate(as.m_rate), m_fullscale(as.m_fullscale)
        { }

        AudioSamples(const AudioSamples& as) = delete;
    };

    //----------------------------------------------------------------------------
    // AudioTrack
    //
    /// \brief Audio data container with time domain filtering tools.
    /// This class can modify stored data with the following implemented
    /// filters (code written by Angelo Farina):
    /// - \b IEC \b 61260 Octave bands
    /// - \b ITU \b P56 150-550 Hz band pass
    /// - \b ISO \b 8041 Generic axis acceleration ponderation
    /// - \b UNI \b 9916 250 Hz low pass
    /// - \b A-weighting filter.
    /// It can also remove the DC component and performs a noise correction.\n
    /// Note that all the filters operation will have place on a copy of the
    /// stored audio date, so before a filtering method call, a call to
    /// ResetFilteredTrack() should be done, otherwise the result is not
    /// guaranteed...
    //
    //----------------------------------------------------------------------------

    class AudioTrack : public AudioSamples
    {
       public:
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


       protected:
          AudioSamples  m_filteredTrack;  ///< The filtered data vector.
          double        m_noiseCorrFactor = 0.0;   ///< Noise correction flag.
          int           m_nSelectedFilter = 0;     ///< The filter selected by user.         

       public:   // TODO: Some reordering has to be done...

          /// If a track length is set, initialize nCh channel datas.
          virtual bool Resize(const SampleCount length);

          /// Destroy nCh channel datas.
          virtual void Destroy();          
       
          // --- Utils
        
          /// Restore the filtered data vector. This method should be called
          /// every time a new filtering operation is needed.
          /// \param gain - gain to apply to data before filtering.
          void ResetFilteredTrack(const double gain = 1.0);
          
          /// Set nCh audio data to filtered version (loosing backup).
          /// \param gain - gain to apply.
          void ReplaceTrackWithFiltered(const double gain = 1.0);
        
          /**
           * @brief Usually not needed, but loaded tracks export this value,
           * so we don't have to recompute it.
           */
          void SetMin(const Sample min) { m_fMin = min; }

          /**
           * @brief Usually not needed, but loaded tracks export this value,
           * so we don't have to recompute it.
           */
          void SetMax(const Sample max) { m_fMax = max; }
        
        // --- Filtering
       private:
            /// Time domain IIR filter function, called by every filtering method.
            /// \f[ y[n] = 2 ( \alpha (x[n] + \mu x[n-1] + \sigma x[n-2]) + \gamma y[n-1] - \beta y[n-2] ) \f]
            /// \param alpha One of the IIR filter parameter
            /// \param beta One of the IIR filter parameter
            /// \param gamma One of the IIR filter parameter
            /// \param mu One of the IIR filter parameter
            /// \param sigma  One of the IIR filter parameter
            void IIR(const double alpha,
                     const double beta,
                     const double gamma,
                     const double mu,
                     const double sigma);

       public:

           /**
            * @brief Make a Dirac's delta
            * @param delay The delta's delay in samples.
            */
           void MakeDelta(const SampleCount delay);

           /**
            * @brief Make a sinusoidal signal
            * @param amplitude The amplitude (-1,1) of the sinus function.
            * @param frequency The function frequency in  Hertz
            * @param phase The function phase in radians
            */
           void MakeSine(const double amplitude,
                         const double frequency,
                         const double phase = 0.0);

          /// Apply gain to filtered data.
          /// \param g - Gain normalized
          void Gain(const double g); 
          
          /// Apply gain to filtered data.
          /// \param G - Gain level in dB
          void GaindB(const double G); // G [dB]

          /// Filtering function wrapper for low-pass, band-pass, high-pass and notch
          /// \param type - Filter type, must be one of LPF, BPF, HPF, NotchF.
          /// \param f0 - Cutoff frequency
          /// \param fQ - Q factor.
          void Filter (const int type, 
                       const double f0, 
                       const double fQ);

          /// A-weigthed filter.
          void AFilter (); //... bool preserve = true);

          /// High-pass filter, cutoff 20 Hz.
          void LFilter ();

          /// ISO 8042 filter. Used in accelerometer measures for the generic axis
          /// ponderation.
          void ISO_8041_Filter ();

          /// ITU P56 filter. Pass band filter 160 - 5500 Hz
          void ITU_P56_Filter ();

          /// UNI 9916 filter. Low pass, cutoff 250 Hz. Used in vibration measures.
          void UNI_9916_Filter ();

          /// IEC 61260 Octave bands filter.
          /// \param dbFc - Center band frequency in Hz, one of 31.5, 63, 125, 250, 500
          ///               1000, 2000, 4000, 8000, 16000.
          void OctaveFilter (const double dbFc);

          /// DC component remover.
          void RemoveMean ();

          /// Noise correction function.
          void NoiseCorrection ();
          
          /// Root Mean Square of the filtered data. Like the base class method (that
          /// operates on backup audio data) the value calculated is proportional to
          /// energy, so no square root is performed.
          virtual double RMS();

          /// Get Equivalent Level of the signal stored. Calculated starting
          /// from RMS().
          double Leq();

          /// Get pointer to filtered data vector.
          /// \return Filtered data vector.
          const AudioSamples& GetFilteredSamples  () const { return m_filteredTrack; }
          AudioSamples& Filtered() { return m_filteredTrack; }

          Sample  GetFilteredSquaredSample (const SampleCount k) const { return m_filteredTrack[k] * m_filteredTrack[k]; }
          
          /// Store audio data like Aurora::AudioTrackBase method, but also optionally initialize filtered vector.
          /// \sa Aurora::AudioTrackBase::SetTrack()
          virtual void SetTrack(Aurora::Sample *pSrc, 
                                const Aurora::SampleCount len);          
       
          int  GetSelectedFilter() const { return m_nSelectedFilter; }
          void SetFilter    (const int nType) { m_nSelectedFilter = nType; }
          void SetFilterType(const int nType) { m_nSelectedFilter = nType; }

          double& GetNoiseCorrFactor() { return m_noiseCorrFactor; }

          virtual void SetFullscale(const double fs) override;

          AudioTrack& operator=(AudioTrack&& at);
          AudioTrack& operator=(const AudioTrack& as) = delete;

          AudioTrack() { }

          AudioTrack(AudioTrack&& at);
          AudioTrack(const AudioTrack& at) = delete;

          /// The constructor.
          /// \param dbRate - the data sampling rate in Hz
          AudioTrack(const SampleCount length, const double dbRate = 48000.0);
          ~AudioTrack();
    };
} // namespace Aurora

#endif // __AURORA_IR_H__

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Acoustical Parameters

  AcParametersTrack.h
 
  Angelo Farina - Simone Campanini
 
 **********************************************************************/
#ifndef __AURORA_APAUDIOTRACK_H__
#define __AURORA_APAUDIOTRACK_H__


namespace Aurora
{
    //-------------------------------------------------------------------------------------
    // AcParametersAudioTrack
    //
    /// \brief Audio data holder.
    /// Holds audio datas - got from Audacity track(s) in some way - 
    /// and do some time domain operations on them. This class is designed
    /// for Acoustical Parameters modules, so the data stored hopefully has
    /// to be Room Impulse Responses.
    //-------------------------------------------------------------------------------------
    class AcParametersAudioTrack : public Aurora::AudioTrack
    {
    private:
        wxString m_name; ///< Audio data label

        /// Impulse response's First Arrival Time, IOW direct wave arrival time.
        double m_dbFAT = 0.0;

    public:
        wxString& GetName() { return m_name; }
        void SetName(const wxChar* name) { m_name = name; }
        void SetName(const wxString& name) { m_name = name; }

        /// Tries to find the First Arrival Time of the stored Impulse Response
        /// \param dbDirectSoundTrigdB - the direct wave trigger level in deciBel
        /// \return true on success, false othervise (try with lower trigger level)
        bool FindFirstArrivalTime(const double dbDirectSoundTrigdB);

        /// Returns the audio data length - in samples - starting from First Arrival Time
        SampleCount GetLengthFromFAT() const;

        /// \sa GetLengthFromFAT()
        SampleCount GetSamplesFromFAT() const;

        /// It returns the delay of the First Arrival Time in seconds.
        double GetFirstArrivalTime() const { return m_dbFAT; }

        /// @brief Return delay of the First Arrival Time in samples
        /// @param useFilteredTrackRate Use the sample rate of filtered track
        /// instead of the rate of unfiltered track.
        SampleCount GetFirstArrivalTimeInSamples(const bool useFilteredTrackRate = false);
        
        /// Applies the selected filter to an octave band
        /// \param fcb Center band frequency (see \c Aurora::Spectrum for 
        /// exception values).  
        void ApplyFilter(const float fcb);

        AcParametersAudioTrack& operator=(AcParametersAudioTrack&& at);
        AcParametersAudioTrack& operator=(const AcParametersAudioTrack& as) = delete;

        AcParametersAudioTrack() { }
        AcParametersAudioTrack(AcParametersAudioTrack&& at);
        AcParametersAudioTrack(const AcParametersAudioTrack& at) = delete;

        /// The constructor
        /// @param length Lenght of the track
        /// @param dbRate Sampling rate of the audio data
        AcParametersAudioTrack(const SampleCount length,
                               const double dbRate = 48000.0)
            : AudioTrack(length, dbRate),
            m_name(wxT("Trackname"))
        { }
    };
} // namespace Aurora

typedef std::vector<Aurora::AcParametersAudioTrack> TArrayOfAcParametersAudioTracks;

#endif // __AURORA_APAUDIOTRACK_H__

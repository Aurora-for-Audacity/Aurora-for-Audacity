/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  StiTrack.h

  Angelo Farina - Simone Campanini

***********************************************************************/
#ifndef __STI_TRACK_H__
#define __STI_TRACK_H__

namespace Aurora
{
    class STIAudioTrack : public Aurora::AudioTrack
    {
        SampleCount m_fat = 0; ///< First Arrival Time in samples.
        
      public:  
        bool FindFirstArrivalTime(const double dbThreshold);
        
        void STIOctaveFilter(const float fcb);
        
        void CalculateMTF(const float fcb,
                          STI::ModulationTransferFunctionsTable& aMTF);    
        
        double CalculateMTF_STItel(const float fcb);
        
        void CalculateMTF_RaSTI (Aurora::STISpectrum& aMTF_RaSTI);
        
        STIAudioTrack& operator=(STIAudioTrack&& at);
        STIAudioTrack& operator=(const STIAudioTrack& at) = delete;
        
        // --- Ctors
        STIAudioTrack() { }
        STIAudioTrack(STIAudioTrack&& at) : AudioTrack(std::move(at)) { }
        STIAudioTrack(const STIAudioTrack& at) = delete;
        
        /**
         * @brief The constructor
         * @param rate The sampling rate of the track
         */
        STIAudioTrack(const SampleCount length,
                      const double rate)
         : AudioTrack(length, rate)
         { }
    };
} // namespace Aurora

#endif //__STI_TRACK_H__

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283

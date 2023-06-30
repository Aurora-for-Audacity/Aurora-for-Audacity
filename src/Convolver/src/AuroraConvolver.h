/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  auconv.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_CONVOLVER_AUCONV_H__
#define __AURORA_CONVOLVER_AUCONV_H__

#include "multivolver.h"

namespace Aurora
{
    class ConvolverController
    {

        // TODO: add a multivolver instance, store input and
        // output vectors directly in this instance (remove
        // mInput, mOutputData, etc). Store filters track
        // here and move to multivolver instance through a
        // split method, in order to populate correctly
        // matrix cells.
    
        double mRate                = 0.0;
        bool   mMatrixMode          = false;
        bool   mCancelCrossTalk     = false;
        bool   mFullAutorange       = true;
        bool   mFirstBlockAutorange = false;
        bool   mTimeReversedIr      = false;
        bool   mPreserveLength      = false;
        double mGain                = 0.0; // dB
              
        std::vector < Aurora::SamplesVector > mFilters; // replaces mIr
        
        int  mUseThisFilterIndex = -1; // replacement for oneForAll
                
        std::unique_ptr<Multivolver> mAfm;
        
      public:

        bool ResizeFilterTrack   (const int idx, const SampleCount length);
        bool ResizeInputTrack(const int idx, const SampleCount length);
        bool CheckSamplerate(const double value);
        
        
        // *** Get/setters
        bool IsMatrixMode()          const { return mMatrixMode; }
        bool IsCancelCrossTalk()     const { return mCancelCrossTalk; }
        bool IsFullAutorange()       const { return mFullAutorange; }
        bool IsFirstBlockAutorange() const { return mFirstBlockAutorange; }
        bool IsTimeReversedIr()      const { return mTimeReversedIr; }
        bool IsLengthPreserved()     const { return mPreserveLength; }
        bool OneForAll()             const { return (mUseThisFilterIndex >=0); }
        double GetGain()             const { return mGain; }
        double GetSamplerate()       const { return mRate; }
        
        std::vector < Aurora::SamplesVector >& GetFilters() { return mFilters; }
        
        SamplesVector& GetInputTrack (const int idx);
        SamplesVector& GetOutputTrack(const int idx);
                
        int FilterRows   () const;
        int FilterColumns() const;

        SampleCount GetFFTLength();
        
        void SetSamplerate(const double value)  { mRate = value; }
        void SetGain      (const double value)  { mGain = value; }
        
        void SetFilterMatrixDimensions(const int nInputs  = 1, 
                                       const int nFilters = 1);
        
        void SetMatrixMode         (const bool value);
        void SetCancelCrossTalk    (const bool value);
        void SetFullAutorange      (const bool value);
        void SetFirstBlockAutorange(const bool value);
        void SetTimeReversedIr     (const bool value);
        void SetPreserveLength     (const bool value);

        // *** processing
      private:
        void DoMatrixConvolution();
        void DoStereoConvolution();   // only if Cancel Crosstalk is set
        void DoCancelCrossTalkConvolution();
        void DoMultitrackConvolution();
        void DoMultitrackMonoConvolution();
        
      public: 
        bool DoConvolution();
        
        void ReadConfigurationValues();
        void StoreConfigurationValues();
        
        void Reset();
        void Destroy();
        
        // *** Ctors
        ConvolverController() { }
        ~ConvolverController() { Destroy();  }
    };
} // namespace Aurora

#endif // __AURORA_CONVOLVER_AUCONV_H__


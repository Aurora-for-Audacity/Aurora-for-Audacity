/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  audtypes.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_AUDTYPES_H__
#define __AURORA_AUDTYPES_H__

// This files works as wrap between Audacity types definitions
// and Aurora classes

#define _AF_USE_FLOAT_

   // from Audacity
   //#include <Audacity.h>
   #include <TranslatableString.h>
   #include <SampleFormat.h>
   #include <Sequence.h>

namespace Aurora
{
   typedef  float         Sample;     
   typedef  sampleFormat  SampleFormat;
   typedef  samplePtr     SamplePtr;   

   // now - Audacity 2.3.3 - sampleCount is a class!
   typedef  size_t        SampleCount;

    enum MessageType
    {
        Info,
        Warning,
        Error,
        YesNo
    };
}

#ifdef USE_KISS_FFT

#else

#include <fftw3.h>

#ifdef _AF_USE_FLOAT_
   typedef fftwf_complex Complex_t;
   typedef fftwf_plan    FftPlan_t;
   #define PLAN_DFT_R2C     fftwf_plan_dft_r2c_1d
   #define PLAN_DFT_C2R     fftwf_plan_dft_c2r_1d
   #define EXEC_DFT_R2C     fftwf_execute_dft_r2c
   #define EXEC_DFT_C2R     fftwf_execute_dft_c2r
   #define DESTROY_DFT_PLAN fftwf_destroy_plan
#else
   typedef  fftw_complex  Complex_t;
   typedef  fftw_plan     FftPlan_t;
   #define PLAN_DFT_R2C     fftw_plan_dft_r2c_1d
   #define PLAN_DFT_C2R     fftw_plan_dft_c2r_1d
   #define EXEC_DFT_R2C     fftw_execute_dft_r2c
   #define EXEC_DFT_C2R     fftw_execute_dft_c2r
   #define DESTROY_DFT_PLAN fftw_destroy_plan
#endif
#endif
   
#endif // __AURORA_AUDTYPES_H__

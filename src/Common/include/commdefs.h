/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  commdefs.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_COMMDEFS_H__
#define __AURORA_COMMDEFS_H__

// --------------------------------------------------------------------
// Defines used to catch memory leaks

#ifdef _DEBUG
#  ifdef  _MSC_VER
#    define _CRTDBG_MAP_ALLOC
#    include <stdlib.h>
#    include <crtdbg.h>
#  endif
#endif
// --------------------------------------------------------------------

// When cmake build system has been adopted, these macros are defined from
// Audacity build command line

#ifdef _MSC_VER
#  ifdef BUILDING_AUDACITY
#    define AUDACITY_DLL_API _declspec(dllexport)
#    define STRINGS_API      _declspec(dllexport)
#  else
#    ifdef _DLL
#      define AUDACITY_DLL_API _declspec(dllimport)
#      define STRINGS_API      _declspec(dllimport)
#    else
#      define AUDACIY_DLL_API
#      define STRINGS_API
#    endif
#  endif

#else
#  define AUDACITY_DLL_API
#  define STRINGS_API
#endif

#define PROHIBITED =delete
#define EXPERIMENTAL_SPECTRAL_EDITING 1

#ifdef _MSC_VER
#  define AUDACITY_VERSION_STRING   "3.0.3-alpha-20210629"
#  define AUDACITY_VERSION_wchar_t L"3.0.3-alpha-20210629"
#else
#  define AUDACITY_VERSION_STRING   "3.0.3-alpha-20210805"
#  define AUDACITY_VERSION_wchar_t L"3.0.3-alpha-20210805"
#endif

// --------------------------------------------------------------------


#include "audtypes.h"
#include "version.h"

#define   AURORA_NAME   "Aurora"

#include <cassert>
#include <cmath>
#include <cstring>
#include <vector>

// ----- from unixsh math.h
#ifndef M_PI
    #define M_PI        3.1415926535897932384626433
#endif
#ifndef M_SQRT2
    #define M_SQRT2     1.41421356237309504880  /* sqrt(2) */
#endif
#ifndef M_SQRT1_2
    #define M_SQRT1_2   0.70710678118654752440  /* 1/sqrt(2) */
#endif
#ifndef M_E
    #define M_E         2.7182818284590452354   /* e */
#endif

// #ifndef TWO_PI
//     #define TWO_PI      6.2831853   /* 2*pi */
// #endif

// ----- custom
#define dB(x)     ((x > 0) ? 10.0*log10(x) : -120.0)
#define dB20(x)   ((x > 0) ? 20.0*log10(x) : -120.0)
#define undB(x)   (pow(10.0, x/10.0))
#define undB20(x) (pow(10.0, x/20.0))
#define dBsum(x,y) (10.0*log10(pow(10.0, x/10.0) + pow(10.0, y/10.0)))

// channels macro
#ifndef CH_LEFT
    #define CH_LEFT 0
#endif

#ifndef CH_RIGHT
    #define CH_RIGHT 1
#endif

namespace Aurora
{
    enum Channel
    {
        Left = 0,
        Right
    };
}

// these are for fftw_complex vectors
#ifndef _FFT_MACRO_
#define _FFT_MACRO_

#ifdef USE_KISS_FFT
#  define Re(c) c.r         // real part of c kiss_fft_cpx
#  define Im(c) c.i         // imag part of c kiss_fft_cpx
#  define Mod(c)          std::sqrt(c.r*c.r + c.i*c.i) // Module, where 'c' is a kiss_fft_cpx
#  define SqrMod(c)                (c.r*c.r + c.i*c.i)  // Square module, where 'c' is a kiss_fft_cpx
#  define Magnitude(c)    std::sqrt(c.r*c.r + c.i*c.i)  // Magnitude and Mod should be alias.
#  define SqrMagnitude(c)           (c.r*c.r + c.i*c.i)
#  define Phase(c)        ( -std::atan2(c.i, c.r))
#  define DegPhase(c)     (-(std::atan2(c.i, c.r) * 180.0f) / M_PI)
#  define MagPol(c) c.r
#  define PhPol(c)  c.i
#  define RePol(c) c.r * std::cos(c.i)
#  define ImPol(c) c.r * std::sin(c.i)

#else // fftw
#  define Re(c) c[0]         // real part of c fftw[f]_complex
#  define Im(c) c[1]         // imag part of c fftw[f]_complex
#  define Mod(c)    sqrt(c[0]*c[0] + c[1]*c[1]) // Module, where 'c' is a fftw[f]_complex
#  define SqrMod(c)     (c[0]*c[0] + c[1]*c[1])  // Square module, where 'c' is a fftw[f]_complex
#  define Magnitude(c)    sqrt(c[0]*c[0] + c[1]*c[1])  // Magnitude and Mod should be alias.
#  define SqrMagnitude(c)      (c[0]*c[0] + c[1]*c[1])
#  define Phase(c)        (-atan2(c[1], c[0])) 
#  define DegPhase(c)     (-(atan2(c[1], c[0]) * 180) / M_PI)
#  define Mag(c) c[0]
#  define Ph(c)  c[1]
#endif

#endif

// useful for rounding to nearest integer
#define iround(d)  ( int((d >= 0) ? floor(d + 0.5) : ceil(d - 0.5)) )
#define afround(d) (Aurora::SampleCount(d + 0.5))
#define unRound(d) (size_t(d + 0.5))


#define __WX_NATIVE_PROG_DLG__ 1
#define __ITUP56_SINGLE_FRAME__ 1

#endif // __AURORA_COMMDEFS_H__

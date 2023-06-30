/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  aurora.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_H__
#define __AURORA_H__

/**
 *  \mainpage
 *  This is Aurora, a powerful set of plugin for advanced
 *  audio elaborations and analysis written originally by Angelo Farina
 *  as plug-ins for Cool Edit/Audition, and then converted as Audacity
 *  modules by Simone Campanini.
 */

#include "wxcmn.h"
#include "commdefs.h"

#include "../src/afart.h"
#include "../src/afvector.h"
#include "../src/afaudio.h"
#include "../src/afaxis.h"
#include "../src/afcharts.h"
#include "../src/afcomplex.h"
#include "../src/afspectrum.h"
#include "../src/afconfig.h"
#include "../src/afdata.h"
#include "../src/afdataplot.h"
#include "../src/afexports.h"
#include "../src/affilter.h"
#include "../src/afplot.h"
#include "../src/afscope.h"
#include "../src/arrows.h"

#include "../src/gpfuncts.h"
#include "../src/gtprogind.h"

#include "../src/sigwin.h"
#include "../src/octfilt.h"
#include "../src/firwin.h"
#include "../src/octfir.h"
#include "../src/mroctbd.h"


// from Audacity include directory
#include <audacity/Types.h>
#include <audacity/ComponentInterface.h>
#include <audacity/ModuleInterface.h>
#include <audacity/EffectInterface.h>

// from Audacity source code
//#include <Audacity.h>
#include <effects/Effect.h>

#include "../src/component.h"
#include "../src/AuroraModule.h"

// ----------------------------------------------------------------------
// modules defines and typedefs (from mod-script-pipe)
// ----------------------------------------------------------------------

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBSCRIPT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCRIPT_PIPE_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


/* Magic for dynamic library import and export. This is unfortunately
 * compiler-specific because there isn't a standard way to do it. Currently it
 * works with the Visual Studio compiler for windows, and for GCC 4+. Anything
 * else gets all symbols made public, which gets messy */
 /* The Visual Studio implementation */
#ifdef _MSC_VER
#  define AURORA_DLL_IMPORT _declspec(dllimport)
#  ifdef BUILDING_AURORA
#    define AURORA_DLL_API _declspec(dllexport)
#  elif _DLL
#    define AURORA_DLL_API _declspec(dllimport)
#  else
#    define AUDACITY_DLL_API
#  endif
#endif //_MSC_VER

/* The GCC implementation */
#ifdef CC_HASVISIBILITY // this is provided by the configure script, is only
// enabled for suitable GCC versions
/* The incantation is a bit weird here because it uses ELF symbol stuff. If we
 * make a symbol "default" it makes it visible (for import or export). Making it
 * "hidden" means it is invisible outside the shared object. */
#define AURORA_DLL_IMPORT __attribute__((visibility("default")))
#ifdef BUILDING_AURORA
#  define AURORA_DLL_API __attribute__((visibility("default")))
#else
#  define AURORA_DLL_API __attribute__((visibility("default")))
#endif
#endif

// the following comes from mod-script-pipt/ScripterCallback.cpp

#ifdef _MSC_VER
#  define DLL_API _declspec(dllexport)
#  define DLL_IMPORT _declspec(dllimport)
#else
#  define DLL_API __attribute__ ((visibility("default")))
#  define DLL_IMPORT
#endif

#endif // __AURORA_VERSION_H__

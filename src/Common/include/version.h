/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  version.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_VERSION_H__
#define __AURORA_VERSION_H__

#include "aurora.h"

#define  AURORA_VERSION_String   "5.3" //"4.3"
#define  AURORA_VERSION_MAJOR   5
#define  AURORA_VERSION_MINOR   3

//The build number counter.
//The global variable is instantiated in build_count.cpp source file
#define AURORA_BUILD_COUNT g_auroraBuildCounter

extern int g_auroraBuildCounter;

namespace Aurora
{
    wxString ModuleVersionString();
}

#endif // __AURORA_VERSION_H__  

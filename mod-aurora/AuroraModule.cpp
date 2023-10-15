///**********************************************************************
// 
// Aurora for Audacity: A Powerful multiplatform Acoustic calculations
// plugin collection
// 
// Common
// 
// Aurora::AuroraModule.h
// 
// Simone Campanini - 7/1/2020
// 
// **********************************************************************/
#include "AuroraModule.h"

DEFINE_VERSION_CHECK

//----------------------------------------------------------------------------
// Module callback
//----------------------------------------------------------------------------
extern "C" DLL_API int ModuleDispatch(ModuleDispatchTypes type)
{
        switch (type)
        {
            case ModuleTerminate:
            case ModuleInitialize:
            case AppInitialized:
            case AppQuiting:
            case ProjectInitialized:
            case ProjectClosing:
                break;
            default:
                break;
        }
    
    return 1;
};

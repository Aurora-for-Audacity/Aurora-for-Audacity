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
    std::cout << __func__ << "\n";
    if (type == ModuleInitialize)
    {
//        EffectManager & em = EffectManager::Get();
//        auto sinSweepGenerator = std::make_unique<Aurora::SineSweepGeneratorEffect>();
//        em.RegisterEffect(std::move(sinSweepGenerator));
        
//                    em.RegisterEffect(new Aurora::AcParametersEffect);
        //            em.RegisterEffect(new Aurora::STIEffect);
        //            em.RegisterEffect(new Aurora::TimeHistoryAnalyzerEffect);
        //            em.RegisterEffect(new Aurora::XFunctionsEffect);
        //            em.RegisterEffect(new Aurora::KirkebyEffect);
        //            em.RegisterEffect(new Aurora::ConvolverEffect);
    }
    else
    {
        switch (type)
        {
            case ModuleTerminate:
            case AppInitialized:
            case AppQuiting:
            case ProjectInitialized:
            case ProjectClosing:
                break;
            default:
                break;
        }
    }
    
    return 1;
};

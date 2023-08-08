/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
					   plugin collection

  Common

  Aurora::Module.h

  Simone Campanini - 7/1/2020

**********************************************************************/
#if defined _DEBUG && defined USE_VLD
/*
    The Visual Leakage Detector include!
    see it on github for installation instructions.
*/
#  include <vld.h>
#endif

#include <aurora.h>
#include <AcParameters/src/AcParametersEffect.h>
#include <Convolver/src/ConvolverEffect.h>
#include <Kirkeby/src/KirkebyEffect.h>
#include <SineSweepGenerator/src/SineSweepEffect.h>
#include <STI/src/StiEffect.h>
#include <TimeHistoryAnalyzer/src/ThAnalyzerEffect.h>
#include <XFunctions/src/XFunctionsEffect.h>

// From Audacity source tree
#include <ModuleManager.h>
#include <effects/EffectManager.h>
//#include <ModuleConstants.h>

/*
    NOTE: plugin configurations goes to
    C:\Users\<username>\AppData\Roaming\audacity
*/

PluginPath Aurora::Module::GetPath()
{
#if 0
    return PluginPath("Z:\\src\\audacity-minsrc-2.3.3\\win\\Debug\\modules"); // PluginPath is a wxString
#else
    return {};
#endif
}

ComponentInterfaceSymbol Aurora::Module::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("AuroraPluginsModule") };
}

VendorSymbol Aurora::Module::GetVendor()
{
    return VendorSymbol{ XO("Unipr") };
}

wxString Aurora::Module::GetVersion()
{
    return wxString(AURORA_VERSION_String);
}

TranslatableString Aurora::Module::GetDescription()
{
    return TranslatableString  { XO("Aurora Plugins Module for Audacity") };
}

EffectFamilySymbol Aurora::Module::GetOptionalFamilySymbol()
{
    return EffectFamilySymbol { XO("Aurora") };
}


const FileExtensions & Aurora::Module::GetFileExtensions()
{
    return m_ext;
}

FilePath Aurora::Module::InstallPath()
{
    return {};
}

bool Aurora::Module::AutoRegisterPlugins(PluginManagerInterface & pluginManager)
{
    return false;
}

PluginPaths Aurora::Module::FindPluginPaths(PluginManagerInterface & pluginManager)
{
    return m_paths;
}

unsigned Aurora::Module::DiscoverPluginsAtPath(const PluginPath & path,
                                               TranslatableString & errMsg,
                                               const RegistrationCallback &callback)
{
    auto effect = CreateInstance(path);

    if (effect)
    {
        if (callback)
        {
                callback(this, effect);
        }
        return 1;
    }
    return 0;
}

bool Aurora::Module::IsPluginValid(const PluginPath & path, bool bFast)
{
    if (   path.IsSameAs("Aurora/AcousticalParameters")
        || path.IsSameAs("Aurora/Convolver")
        || path.IsSameAs("Aurora/InvertKirkeby")
        || path.IsSameAs("Aurora/STI")
        || path.IsSameAs("Aurora/TimeHistoryAnalyzer")
        || path.IsSameAs("Aurora/XFunctions")
        || path.IsSameAs("Aurora/SineSweepGenerator"))
    {
        return true;
    }
    return false;
}

bool Aurora::Module::Initialize()
{
    
#ifdef _DEBUG
    // code used to catch memory leaks with visual studio.
#  ifdef _MSC_VER
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#  endif
#endif

    m_paths.push_back(PluginPath("Aurora/AcousticalParameters"));
    m_paths.push_back(PluginPath("Aurora/Convolver"));
    m_paths.push_back(PluginPath("Aurora/InvertKirkeby"));
    m_paths.push_back(PluginPath("Aurora/SineSweepGenerator"));
    m_paths.push_back(PluginPath("Aurora/STI"));
    m_paths.push_back(PluginPath("Aurora/TimeHistoryAnalyzer"));
    m_paths.push_back(PluginPath("Aurora/XFunctions"));
    
    return true;
}

ComponentInterface *Aurora::Module::CreateInstance(const PluginPath & path)
{ 
    if (path.IsSameAs("Aurora/AcousticalParameters"))
    {
            auto effect = new Aurora::AcParametersEffect;
            return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/Convolver"))
    {
            auto effect = new Aurora::ConvolverEffect;
            return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/InvertKirkeby"))
    {
        auto effect = new Aurora::KirkebyEffect;
        return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/SineSweepGenerator"))
    {
        auto effect = new Aurora::SineSweepGeneratorEffect;
        return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/STI"))
    {
        auto effect = new Aurora::STIEffect;
        return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/TimeHistoryAnalyzer"))
    {
        auto effect = new Aurora::TimeHistoryAnalyzerEffect;
        return (ComponentInterface*)effect;
    }
    else if (path.IsSameAs("Aurora/XFunctions"))
    {
        auto effect = new Aurora::XFunctionsEffect;
        return (ComponentInterface*)effect;
    }
	return nullptr; 
} 

void Aurora::Module::DeleteInstance(ComponentInterface *instance)
{ 
	if (instance)
	{
		delete instance;
	}
} 

//----------------------------------------------------------------------------
// Module callback
//----------------------------------------------------------------------------
extern "C"
{
    /// Make sure that this version of the module requires the version
    /// of Audacity it is built with.
    /// For now the versions must match exactly for Audacity to
    /// agree to load the module.
    extern DLL_API const wchar_t* GetVersionString()
    {
        return AUDACITY_VERSION_wchar_t;
    }

    extern DLL_API int ModuleDispatch(ModuleDispatchTypes type)
    {
        if (type == ModuleInitialize)
        {
            wxLogDebug("[AuroraPlugins]: dispatch %d received (init).\n", int(type));

            EffectManager & em = EffectManager::Get();

            em.RegisterEffect(new Aurora::AcParametersEffect);
            em.RegisterEffect(new Aurora::STIEffect);
            em.RegisterEffect(new Aurora::TimeHistoryAnalyzerEffect);
            em.RegisterEffect(new Aurora::XFunctionsEffect);
            em.RegisterEffect(new Aurora::SineSweepGeneratorEffect);
            em.RegisterEffect(new Aurora::KirkebyEffect);
            em.RegisterEffect(new Aurora::ConvolverEffect);
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
                    wxLogDebug("[AuroraPlugins]: dispatch %d received.\n", int(type));
                    break;

                default:
                    wxLogDebug("[AuroraPlugins]: unknown dispatch received.\n");
                    break;
            }
        }

        return 1;
    }

};



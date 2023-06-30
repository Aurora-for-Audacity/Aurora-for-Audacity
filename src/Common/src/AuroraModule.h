/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
					   plugin collection

  Common

  AuroraModule.h

  Simone Campanini - 7/1/2020

**********************************************************************/

#ifndef __AURORA_MODULE_H__
#define __AURORA_MODULE_H__

#include <aurora.h>

// ------------------------------------------------------------------------
// class Aurora::ModuleInterface
// ------------------------------------------------------------------------
namespace Aurora
{
    class Module : public ModuleInterface
    {
      protected:
        PluginPaths    m_paths;
        FileExtensions m_ext;

      public:
        // Component interface
        virtual PluginPath GetPath() override;
        virtual ComponentInterfaceSymbol GetSymbol() override;

        virtual VendorSymbol GetVendor() override;
        virtual wxString    GetVersion() override;

        // this should be overridden 
#if AUDACITY_HEX_VERSION < 0x020400
        virtual wxString GetDescription() override;
#else
        virtual TranslatableString GetDescription() override;
        
        virtual EffectFamilySymbol GetOptionalFamilySymbol() override;
#endif

        // Module interface
        virtual bool Initialize() override; // tbo if needed
        virtual void Terminate() override { } // tbo

        virtual const FileExtensions &GetFileExtensions() override;
        virtual FilePath InstallPath() override;
        virtual bool AutoRegisterPlugins(PluginManagerInterface & pluginManager) override;
        virtual PluginPaths FindPluginPaths(PluginManagerInterface & pluginManager) override;
#if AUDACITY_HEX_VERSION < 0x020400
        virtual unsigned DiscoverPluginsAtPath(const PluginPath & path,
                                               wxString &errMsg,
                                               const RegistrationCallback &callback) override;
#else
        virtual unsigned DiscoverPluginsAtPath(const PluginPath & path,
                                               TranslatableString &errMsg,
                                               const RegistrationCallback &callback) override;
#endif
        virtual bool IsPluginValid(const PluginPath & path, bool bFast) override;

        virtual ComponentInterface *CreateInstance(const PluginPath & path) override; // tbo
        virtual void DeleteInstance(ComponentInterface *instance) override; // tbo

    };
}
#endif


/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_XFUNCTIONS_MODULE_H__
#define __AURORA_XFUNCTIONS_MODULE_H__

#include "correlator.h"

class WaveTrack;
namespace Aurora
{
    class XFunctionsEffect: public Aurora::EffectBase,
                            public Aurora::Correlator
    {
      public:

    // ---------------- Standard Audacity Effects' methods ----------------

        ComponentInterfaceSymbol GetSymbol() override;
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
        EffectType GetType() override;

        bool Init() override;
        
#if AUDACITY_HEX_VERSION < 0x020400
        bool PromptUser(wxWindow* parent) override;
#else
        bool ShowInterface(wxWindow& parent,
                           const EffectDialogFactory& factory,
                           bool forceModal = false) override;
#endif

      protected:
        bool Process() override;


      public:        
        const wxString& GetXFunctionLabel(const int type)  const { return m_xfnNames[type]; }
        const wxString& GetCurrentXFunctionLabel()         const { return m_xfnNames[m_nXFnTypeId]; }
                    
        void ReadConfigurationValues();
        void StoreConfigurationValues();
        
        XFunctionsEffect();
        
    private:
        wxWindow* m_parent = nullptr;
        wxArrayString m_xfnNames;

    };

} // namespace Aurora

#endif // __AURORA_XFUNCTIONS_MODULE_H__


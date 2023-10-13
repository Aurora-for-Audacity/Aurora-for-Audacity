/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_XFUNCTIONS_MODULE_H__
#define __AURORA_XFUNCTIONS_MODULE_H__

#include <aurora.h>

#include <WaveTrack.h>
#include <effects/EffectManager.h>
#include <effects/StatefulPerTrackEffect.h>

#include "Correlator.h"

namespace Aurora
{
    class XFunctionsEffect: public StatefulPerTrackEffect,
                            public Aurora::Correlator
    {
      public:

    // ---------------- Standard Audacity Effects' methods ----------------

        ComponentInterfaceSymbol GetSymbol() const override;
        TranslatableString GetDescription() const override;
        PluginPath GetPath() const override;
        EffectType GetType() const override;

        bool Init() override;
        
//        bool ShowInterface(wxWindow& parent,
//                           const EffectDialogFactory& factory,
//                           bool forceModal = false) override;

      protected:
//        bool Process() override;


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


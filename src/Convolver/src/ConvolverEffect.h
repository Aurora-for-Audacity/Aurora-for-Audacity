/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  ConvolverEffect.h

  Angelo Farina - Simone Campanini

**********************************************************************/

#ifndef __AURORA_CONVOLVER_EFFECT_H__
#define __AURORA_CONVOLVER_EFFECT_H__

#include "AuroraConvolver.h"

class WaveTrack;

namespace Aurora
{
    class ConvolverEffect: public EffectBase,
                           public ConvolverController
    {

      private:
        bool LoadTrackData(WaveTrack *wt, int idx = 0, bool is_ir = false);

      public:

        // ---------------- Standard Audacity Effects' methods ----------------

        protected:
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

        ComponentInterfaceSymbol GetSymbol() override;
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
        EffectType GetType() override;

      public:     
        ConvolverEffect() { }
        
    private:
        wxWindow* m_parent = nullptr;

    };
}

#endif // __AURORA_CONVOLVER_MODULE_H__

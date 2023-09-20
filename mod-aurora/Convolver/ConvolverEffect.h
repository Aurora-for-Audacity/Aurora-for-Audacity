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
#include "effects/StatefulEffect.h"

class WaveTrack;

namespace Aurora
{
    class ConvolverEffect: public StatefulEffect,
                           public ConvolverController
    {
        static const ComponentInterfaceSymbol Symbol;
      private:
        bool LoadTrackData(WaveTrack *wt, int idx = 0, bool is_ir = false);

      public:

        // ---------------- Standard Audacity Effects' methods ----------------

        protected:
        bool Init() override;
        
        int ShowClientInterface(const EffectPlugin &plugin, wxWindow &parent, wxDialog &dialog, EffectEditor *pEditor, bool forceModal) const override;

      protected:
        bool Process();// override;

        ComponentInterfaceSymbol GetSymbol() const override;
        TranslatableString GetDescription() const override;
        ManualPageID ManualPage() const override;
        EffectType GetType() const override;

      public:     
        ConvolverEffect() { }
        
    private:
        wxWindow* m_parent = nullptr;

    };
}

#endif // __AURORA_CONVOLVER_MODULE_H__

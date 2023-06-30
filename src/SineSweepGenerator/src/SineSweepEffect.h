/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Sweep Generator

  module.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_SSWGEN_MODULE_H__
#define __AURORA_SSWGEN_MODULE_H__

#include "SineSweepGenerator.h"

class WaveTrack;

namespace Aurora
{
    class SineSweepGeneratorEffect : public EffectBase,
                                     public SineSweepGenerator
    {
    private:
        double mDuration;  // taken `as is` from Generator.cpp
        
    protected:
        bool Init() override;

#if AUDACITY_HEX_VERSION < 0x020400
        bool PromptUser(wxWindow* parent) override;
#else
        bool ShowInterface(wxWindow& parent,
                           const EffectDialogFactory& factory,
                           bool forceModal = false) override;
#endif
        
        void BeforeGenerate();
        bool GenerateTrack(WaveTrack *tmp, const WaveTrack &track, int ntrack);
        void Failure();
        void Success();
        bool Process() override; // taken `as is` from Generator.cpp

        virtual ComponentInterfaceSymbol GetSymbol() override;
#if AUDACITY_HEX_VERSION < 0x020400
        wxString GetDescription() override;
#else
        TranslatableString GetDescription() override;
#endif
        PluginPath GetPath() override;
        EffectType GetType() override;

    public:
        SineSweepGeneratorEffect() { }
        
    private:
        wxWindow* m_parent = nullptr;
    };
}

#endif // __AURORA_SSWGEN_MODULE_H__

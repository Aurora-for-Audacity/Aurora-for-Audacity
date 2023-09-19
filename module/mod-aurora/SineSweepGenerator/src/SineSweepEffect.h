/**********************************************************************
 
 Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 plugin collection
 
 Sine Sweep Signal Sweep Generator
 
 module.h
 
 Simone Campanini
 
 **********************************************************************/

#ifndef __AURORA_SSWGEN_MODULE_H__
#define __AURORA_SSWGEN_MODULE_H__

#include <aurora.h>

#include <effects/EffectManager.h>
#include "TimeWarper.h"
#include <Project.h>
#include <Prefs.h>
#include <ViewInfo.h>
#include <WaveTrack.h>

#include "effects/Generator.h"
#include "SyncLock.h"
#include "SineSweepGenerator.h"
#include "SineSweepDialogs.h"
#include "SineSweepUi.h"

namespace Aurora
{
/*!
 \class EffectSineSweepGenerator
 \brief The Audacity module front-end for Aurora's SineSweepGenerator class.
 
 This class is an Audacity module (plug-in) that loads on run time.
 Its core is the instantiation of a TimeHistoryAnalyzer singleton that
 does all calculations.
 */
class SineSweepGeneratorEffect : public SineSweepGenerator,
public Generator
{
public:
    static const ComponentInterfaceSymbol Symbol;
    
    SineSweepGeneratorEffect();
    virtual ~SineSweepGeneratorEffect();
    
protected:
    ComponentInterfaceSymbol GetSymbol() const override;
    TranslatableString  GetDescription() const override;
    ManualPageID ManualPage() const override;
    
    EffectType GetType() const override;
    
//    std::unique_ptr<EffectEditor> PopulateOrExchange(
//       ShuttleGui & S, EffectInstance &instance,
//       EffectSettingsAccess &access, const EffectOutputs *pOutputs) override;
//    bool TransferDataToWindow(const EffectSettings &settings) override;
//    bool TransferDataFromWindow(EffectSettings &settings) override;

    bool Init() override;
    bool ShowInterface(wxWindow& parent,
                       const EffectDialogFactory& factory,
                       bool forceModal = false);
    void BeforeGenerate() override;
    
    void Failure() override;
    void Success() override;
    
    int ShowClientInterface(const EffectPlugin &plugin, wxWindow &parent,
       wxDialog &dialog, EffectEditor *pEditor, bool forceModal)
    const override;
    
    // Generator implementation
    bool GenerateTrack(EffectSettings &settings,
                       WaveTrack *tmp, const WaveTrack &track, int ntrack) override;
    
private:
    /// do whatever additional setuip is required to initialise both the sine sweep and inverse filter tracks.
    /// Adds one more track as the EffectBase class already generates a track if the effect type is a generator.
    /// @see lib-effect/EffectBase::DoEffect method
    void setupTracks() const;
private:
    double mDuration;
    wxWindow* m_parent = nullptr;
    
};
}

#endif // __AURORA_SSWGEN_MODULE_H__

/**********************************************************************
 
 Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 plugin collection
 
 Sine Sweep Signal Generator
 
 module.cpp
 
 Angelo Farina - Simone Campanini
 
 *******************************************************************/



#include "SineSweepEffect.h"
#include "LoadEffects.h"

//----------------------------------------------------------------------------
// Aurora::ConvolverEffect implementation
//----------------------------------------------------------------------------


const ComponentInterfaceSymbol Aurora::SineSweepGeneratorEffect::Symbol
/* i18n-hint: noun */
{ XC("SineSweep", "generator") };

namespace{ BuiltinEffectsModule::Registration< Aurora::SineSweepGeneratorEffect > reg; }

Aurora::SineSweepGeneratorEffect::SineSweepGeneratorEffect()
{
    SetLinearEffectFlag(true);
}

Aurora::SineSweepGeneratorEffect::~SineSweepGeneratorEffect()
{
}

ComponentInterfaceSymbol Aurora::SineSweepGeneratorEffect::GetSymbol() const
{
    return Symbol;
}

TranslatableString Aurora::SineSweepGeneratorEffect::GetDescription() const
{
    return XO("Generate exponential sine sweep signals");
}

ManualPageID Aurora::SineSweepGeneratorEffect::ManualPage() const
{
    return L"Exponential Sine Sweep";
}

//PluginPath Aurora::SineSweepGeneratorEffect::GetPath() const
//{
//    return PluginPath("Aurora/SineSweepGenerator");
//}

EffectType Aurora::SineSweepGeneratorEffect::GetType() const
{
    return EffectTypeGenerate;
}

// end of interface implementation
//
//--------------------------------------------------------------------------
// EffectSineSweepGenerator implementation
//--------------------------------------------------------------------------
bool Aurora::SineSweepGeneratorEffect::Init()
{
    Aurora::SineSweepGenerator::SetSamplerate(mProjectRate);
    
    return true;
}

int Aurora::SineSweepGeneratorEffect::ShowClientInterface(const EffectPlugin &plugin,
                                                          wxWindow &parent,
                                                          wxDialog &dialog,
                                                          EffectEditor *pEditor,
                                                          bool forceModal) const
{
    InitArtProvider();
    Aurora::SineSweepGeneratorDialog dlog((SineSweepGenerator*)(this), &parent);
    dlog.CenterOnParent();
    
    setupTracks();
        
    return  dlog.ShowModal();
}

void Aurora::SineSweepGeneratorEffect::setupTracks() const
{
   // Add one more track as the EffectBase class already generates a track if the effect type is a generator. see lib-effect/EffectBase::DoEffect method
    
    WaveTrack *newTrack{};
    auto track = mFactory->Create();
    
    track->SetName(mTracks->MakeUniqueTrackName(WaveTrack::GetDefaultAudioTrackNamePreference()));
    newTrack = mTracks->Add(track);
    newTrack->SetSelected(true);
    
    int i = 1;
    int neededTracks = 2;
    for (auto it = mTracks->begin(); it != mTracks->end(); ++it)
    {
        Track* t = *it;
        wxString name;
        
        if(i == 1)
            name.Printf(_("Sweep Channel %d"), i);
        else
            name = _("Inverse Filter");
        
        t->SetName(name);
        i++;
    }
}

void Aurora::SineSweepGeneratorEffect::BeforeGenerate()
{
    mDuration = GetTotalDuration();
}



bool Aurora::SineSweepGeneratorEffect::GenerateTrack(EffectSettings &settings,
                                                     WaveTrack *tmp, const WaveTrack &track, int ntrack)
{
    bool bGoodResult = true;
    
    sampleCount numSamples;
    
    // Filter has different length
    numSamples = (ntrack == GetFilterChannel()) ? sampleCount(GetFilterLength()) : sampleCount(GetBuffersLength());
        
    
    decltype(numSamples) i = 0;
    Floats data{ tmp->GetMaxBlockSize() };
    
    while ((i < numSamples) && bGoodResult)
    {
        const auto block =
        limitSampleBufferSize(tmp->GetBestBlockSize(i), numSamples - i);
        
        //GenerateBlock(data.get(), track, block);
        FillBlock(data.get(), block, i.as_size_t(), ntrack);
        
        // Add the generated data to the temporary track
        tmp->Append((samplePtr)data.get(), floatSample, block);
        i += block;
        
        // Update the progress meter
        if (TrackProgress(ntrack,
                          i.as_double() / numSamples.as_double()))
        {
            bGoodResult = false;
        }
    }
    return bGoodResult;
    return true;
}


void Aurora::SineSweepGeneratorEffect::Failure()
{
    Aurora::SineSweepGenerator::Destroy();
    Aurora::MessageBox("Cannot create signal tracks.",
                       Aurora::MessageType::Error);
}

void Aurora::SineSweepGeneratorEffect::Success()
{
    Aurora::SineSweepGenerator::Destroy();
}

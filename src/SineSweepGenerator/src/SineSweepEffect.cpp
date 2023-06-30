/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Sine Sweep Signal Generator

  module.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class EffectSineSweepGenerator
\brief The Audacity module front-end for Aurora's SineSweepGenerator class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of a TimeHistoryAnalyzer singleton that 
  does all calculations.

*//*******************************************************************/
#include <aurora.h>

#include <effects/EffectManager.h>
#include <effects/TimeWarper.h>
#include <Project.h>
#include <Prefs.h>
#include <ViewInfo.h>
#include <WaveTrack.h>

#include "SineSweepGenerator.h"
#include "SineSweepDialogs.h"
#include "SineSweepUi.h"
#include "SineSweepEffect.h"

//----------------------------------------------------------------------------
// AFStepTimeWarper implementation `as is` from TimeWarper.cpp
//----------------------------------------------------------------------------
// taken `as is` from TimeWarper.cpp

///... but is really needed???
class AFStepTimeWarper : public TimeWarper
{
private:
    double mTStep;
    double mOffset;
public:
    AFStepTimeWarper(double tStep, double offset): mTStep(tStep), mOffset(offset)
    { }

    virtual double Warp(double originalTime) const
    {
        return originalTime + ((originalTime > mTStep) ? mOffset : 0.0);
    }
};


//----------------------------------------------------------------------------
// Aurora::ConvolverEffect implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::SineSweepGeneratorEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora Sine Sweep Generator") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::SineSweepGeneratorEffect::GetDescription()
{
    return _("Generate exponential sine sweep signals.");
}
#else
TranslatableString Aurora::SineSweepGeneratorEffect::GetDescription()
{
    return TranslatableString { XO("Generate exponential sine sweep signals.") };
}
#endif

PluginPath Aurora::SineSweepGeneratorEffect::GetPath()
{
    return PluginPath("Aurora/SineSweepGenerator");
}

EffectType Aurora::SineSweepGeneratorEffect::GetType()
{
    return EffectTypeGenerate;
}

// end of interface implementation

//----------------------------------------------------------------------------
// EffectSineSweepGenerator implementation
//----------------------------------------------------------------------------

bool Aurora::SineSweepGeneratorEffect::Init()
{
    Aurora::SineSweepGenerator::SetSamplerate(mProjectRate);
    return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::SineSweepGeneratorEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
        
#else
bool Aurora::SineSweepGeneratorEffect::ShowInterface(wxWindow& parent,
                                                     const EffectDialogFactory& factory,
                                                     bool forceModal)
{
    m_parent = &parent;
#endif

   InitArtProvider();

   Aurora::SineSweepGeneratorDialog dlog(this, m_parent);
   dlog.CenterOnParent();

   if( !dlog.ShowModal() )  return false;
   
   if(! Generate()) 
   {
      MessageBox("Something strange occourred.\nCannot generate Sine Sweep Signal.",
                 Aurora::MessageType::Error);
      Aurora::SineSweepGenerator::Destroy();
      return false;
   }
   // This should be set to the correct value.
   mDuration = GetTotalDuration();
   return true;
}

void Aurora::SineSweepGeneratorEffect::BeforeGenerate()
{
   int existingTracks = GetNumWaveTracks();
   int neededTracks   = GetNeededTracks();
   
   //If there are selected tracks it's a mess, better deselect all first...
   int i = (existingTracks > 1 ? 0 : 1);
    
   while(i < neededTracks) 
   {  
      auto wt = mFactory->NewWaveTrack(floatSample, mProjectRate);

      AddToOutputTracks(wt);
      i++;
   }
   // Select all tracks
   i = 1;
   wxString name;

   for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
   {    
        Track* t = *it;
        name.Printf(_("Sweep Channel %d"), i);
        
        if(IsControlPulsesSet()) 
        {          
            if (i == (neededTracks - 1))
            {
                name = _("Inverse Filter");
            }            
            else if (i == neededTracks) 
            {
                name = _("Pulses Channel");
            }
        } 
        else 
        {    
            if (i == neededTracks)
            {
                name = _("Inverse Filter");
            }
        }
        t->SetName(name);
        i++;
    }
}


//------------------------- Processing methods -------------------------
bool Aurora::SineSweepGeneratorEffect::Process()
{
   // taken `as is` from Audacity`s Generator.cpp to resolve
   // a vc++ linking problem...
   
    if (GetDuration() < 0.0)
    {
        return false;
    }

    // Set up mOutputTracks.
    // This effect needs all for sync-lock grouping.
    this->CopyInputTracks(true);

    // Iterate over the tracks
    bool bGoodResult = true;
    int ntrack = 0;

    BeforeGenerate(); // Add needed tracks to workspace

    mOutputTracks->Any().VisitWhile(bGoodResult,
                                    [&](WaveTrack *track,
                                        const Track::Fallthrough &fallthrough)
    {
        if (GetDuration() > 0.0)
        {
            AudacityProject *p = GetActiveProject();
            // Create a temporary track
            WaveTrack::Holder tmp(mFactory->NewWaveTrack(track->GetSampleFormat(),
                                                         track->GetRate()));
            // Fill it with data
            if (! GenerateTrack(&*tmp, *track, ntrack))
            {
                bGoodResult = false;
            }
            else
            {
                // Transfer the data from the temporary track to the actual one
                tmp->Flush();
                AFStepTimeWarper warper { mT0 + GetDuration(),
                                          GetDuration() - (mT1 - mT0) };
                const auto &selectedRegion = ViewInfo::Get(*p).selectedRegion;
                track->ClearAndPaste(selectedRegion.t0(),
                                     selectedRegion.t1(),
                                     &*tmp,
                                     true,
                                     false,
                                     &warper);
            }

            if (!bGoodResult)
            {
                Failure();
                return;
            }
        }
        else
        {
            // If the duration is zero, there's no need to actually
            // generate anything
            track->Clear(mT0, mT1);
        }

        ntrack++;
    },
    [&](Track *t)
    {
        if (t->IsSyncLockSelected())
        {
            t->SyncLockAdjust(mT1, mT0 + GetDuration());
        }
    });

    if (bGoodResult) 
    {
        Success();

        this->ReplaceProcessedTracks(bGoodResult);

        mT1 = mT0 + GetDuration(); // Update selection.
    }

    return bGoodResult;
}

bool Aurora::SineSweepGeneratorEffect::GenerateTrack(WaveTrack *tmp, 
                                                     const WaveTrack &track, 
                                                     int ntrack)
{
   bool bGoodResult = true;

   //numSamples = track.TimeToLongSamples(mDuration);
   sampleCount numSamples;

   //Filter has different length
   if (ntrack == GetFilterChannel())
   {
       numSamples = sampleCount(GetFilterLength());
   }
   else
   {
       numSamples = sampleCount(GetBuffersLength());
   }
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
}


void Aurora::SineSweepGeneratorEffect::Failure()
{
    Aurora::SineSweepGenerator::Destroy();
    MessageBox("Cannot create signal tracks.",
               Aurora::MessageType::Error);
}

void Aurora::SineSweepGeneratorEffect::Success()
{
    Aurora::SineSweepGenerator::Destroy();
}

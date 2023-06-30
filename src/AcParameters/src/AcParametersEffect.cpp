/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Acoustical Parameters

  AcParametersEffect.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class AcParametersEffect
\brief The Audacity module front-end for Aurora's AcParameter class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of a AcParameter singleton that does
  all calculations.

*//*******************************************************************/
#include <aurora.h>

// From Audacity source tree
#include <ModuleManager.h>
#include <PluginManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>
#include <widgets/ProgressDialog.h>

#include "AcParametersBase.h"
#include "AcParametersDialogs.h"
#include "AcParametersPlot.h"

#include "AcParametersEffect.h"

#include "AcParametersUi.h"

//----------------------------------------------------------------------------
// Aurora::AcParametersEffect implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::AcParametersEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora Acoustical Parameters") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::AcParametersEffect::GetDescription()
{
    return _("Evaluates the ISO3382 Acoustical Parameters of a room from a measured impulse response");
}
#else
TranslatableString Aurora::AcParametersEffect::GetDescription()
{
    return TranslatableString { XO("Evaluates the ISO3382 Acoustical Parameters of a room from a measured impulse response") };
}
#endif

PluginPath Aurora::AcParametersEffect::GetPath()
{
    return PluginPath("Aurora/AcousticalParameters");
}

// end of interface implementation

void Aurora::AcParametersEffect::AddProcessedTracks()
{
    // This code appends processed tracks to workspace.
    // I don't want to modify the existing ones.

    for(auto& track : m_aAudioTracks)
    {
    	wxString name = track.GetName();
        
        auto max = track.Filtered().GetAbsMax();
        wxLogDebug("Max for track %s: %f", name, (double)max);

        auto wt = mFactory->NewWaveTrack(floatSample, mProjectRate);
        wt->Append( (samplePtr)track.Filtered().Samples(),
                      floatSample,
                      track.GetLength() );
        wt->Flush();        
        wt->SetName(name);
	    AddToOutputTracks(wt);
    }

    this->ReplaceProcessedTracks(true);
}


void Aurora::AcParametersEffect::LoadTracks()
{
    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)		
    {
        WaveTrack* wt = (WaveTrack*)*it;
        
        //Get smpcStart and smpcEnd times from track
        double tstart = wt->GetStartTime();
        double tend = wt->GetEndTime();

        //Set the current bounds to whichever left marker is
        //greater and whichever right marker is less:
        double t0 = mT0 < tstart ? tstart : mT0;
        double t1 = mT1 > tend   ? tend   : mT1;

        // Process only if the right marker is to the right of the left marker
        if (t1 > t0)
        {
            //Transform the marker timepoints to samples
            Aurora::SampleCount start = wt->TimeToLongSamples(t0).as_size_t();
            Aurora::SampleCount end   = wt->TimeToLongSamples(t1).as_size_t();
            m_aAudioTracks.emplace_back(Aurora::AcParametersAudioTrack(end - start,
                                                                       wt->GetRate()));
            auto& at = m_aAudioTracks.back();

            std::pair<float, float> minMax = wt->GetMinMax(t0, t1);
            //What we need are absolute values.
            at.SetMin(std::fabs(minMax.first));
            at.SetMax(std::fabs(minMax.second));
            at.SetName(wt->GetName());

            wt->Get( (samplePtr) at.Samples(), 
                     floatSample, 
                     sampleCount(start),  
                     at.Length() );
        }
    }
}

// ---------------- AcParameters methods ------------------------------

void Aurora::AcParametersEffect::ReadConfigurationValues()
{
    Aurora::Config cfg;

   // Read parameter from stored configuration
   double dbValue;

   if (cfg.Read("/Aurora/AcParameters/UserMinLevel", &dbValue))
   {
       SetUserMinLevel(dbValue);
   }
   if (cfg.Read("/Aurora/AcParameters/UserMaxLevel", &dbValue))
   {
       SetUserMaxLevel(dbValue);
   }
   if (cfg.Read("/Aurora/AcParameters/DirectSoundTrigger", &dbValue))
   {
       SetDirectSoundTrigValue(dbValue);
   }
   if (cfg.Read("/Aurora/AcParameters/FullScale", &dbValue))
   {
       SetFullScale(dbValue);
   }
   if (cfg.Read("/Aurora/AcParameters/ProbeMicsDistance", &dbValue))
   {
       SetProbeMicsDistance(dbValue);
   }
   if (cfg.Read("/Aurora/AcParameters/SoundSpeed", &dbValue))
   {
       SetSoundSpeedValue(dbValue);
   }
   
   if (m_octaveFraction == 1)
   {
       // G works only for octave bands.
       ReadConfigurationGReferenceLevels(cfg);
   }
}

void Aurora::AcParametersEffect::StoreConfigurationValues()
{
    Aurora::Config cfg;

   cfg.Write("/Aurora/AcParameters/UserMinLevel",       GetUserMinLevel());
   cfg.Write("/Aurora/AcParameters/UserMaxLevel",       GetUserMaxLevel());
   cfg.Write("/Aurora/AcParameters/DirectSoundTrigger", GetDirectSoundTrigValue());
   cfg.Write("/Aurora/AcParameters/FullScale",          GetFullScale());
   cfg.Write("/Aurora/AcParameters/ProbeMicsDistance",  GetProbeMicsDistance());
   cfg.Write("/Aurora/AcParameters/SoundSpeed",         GetSoundSpeedValue());
}

void Aurora::AcParametersEffect::ReadConfigurationGReferenceLevels(Aurora::Config& cfg)
{
    assert(m_referenceLevels.Length() < 12);
    
    m_referenceLevels.ForEach(true,
                              [&](const size_t i, const float fcb, double& value)
    {
        wxString path =  "/Aurora/AcParameters/ReferenceLevel_";

        Aurora::AcParametersSpectrum::Band b(fcb);
        path << b.GetShortLabel(true);

        cfg.Read(path, &value);
    });   
}

void Aurora::AcParametersEffect::StoreConfigurationGReferenceLevels(Aurora::Config& cfg)
{
    assert(m_referenceLevels.Length() < 12);
    
    m_referenceLevels.ForEach(true,
                              [&](const size_t i, const float fcb, double& value)
    {
        wxString path =  "/Aurora/AcParameters/ReferenceLevel_";

        Aurora::AcParametersSpectrum::Band b(fcb);
        path << b.GetShortLabel(true);

        cfg.Write(path, value);
    });
}

void Aurora::AcParametersEffect::UpdateFilteredTrack(const int channel, 
                                                     const wxString& fcbLabel)
{
    Aurora::AcParametersSpectrum::Band b(fcbLabel);
    assert(b.IsValid());
    Aurora::AcousticalParameters::UpdateFilteredTrack(channel, (float)b);
}

void Aurora::AcParametersEffect::ShowErrorMessage(const int nErrNo)
{
    wxString msg;

    switch(nErrNo)
    {
        case ERR_NO_FAT:
            msg = "Cannot locate First Arrival Time.\nThreshold is too high.\n\nTry again with a lower Threshold.";
            break;
        case ERR_SPATIAL_IR_TOO_SHORT:
            msg = "The Impulse Response is too short (< 80 ms) for spatial parameters calculations.";
            break;
        case ERR_NO_CONFIG_SOURCE:
            msg = "No configuration file found.";
            break;
        default:
            msg = "Error unknown.";
            break;
    }
    MessageBox(msg, Aurora::MessageType::Error);
}

void Aurora::AcParametersEffect::PostProcess()
{
/*
   if(IsAppendDataToFile()) 
   {
       AppendResultsToFile(CH_LEFT);
       if(GetChannelsNumber() > 1)
    	   AppendResultsToFile(CH_RIGHT);
   }
   */
}

// ---------------- Audacity Effect methods ------------------------------

bool Aurora::AcParametersEffect::Init()
{
    m_bAborted = false;
    
    if (GetNumWaveTracks() == 0 || mT0 >= mT1)
    {
        m_bAborted = true;
        MessageBox("No selection found.", Aurora::MessageType::Error);
        return false;
    }

   this->CopyInputTracks(); // Set up mOutputTracks.

   LoadTracks();

   if(m_aAudioTracks.size() == 0)
   {
        MessageBox("Error loading tracks from workspace.", Aurora::MessageType::Error);
        return false;
   }
   Aurora::AcousticalParameters::Init();

   // read values from configuration file/registry
   ReadConfigurationValues();

   // ------------- Detect stereo mode
   const size_t nTracks = m_aAudioTracks.size();

   if (nTracks == 2)
   {
        SetSource(Aurora::AcParametersEffect::Source::TwoOmniMics); // Default stereo choice.
   }
   else if (nTracks == 4)
   {
        SetSource(Aurora::AcParametersEffect::Source::FourOmniMics); // Default tetra/soundfield choice.
   }
   return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::AcParametersEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
    
#else
bool Aurora::AcParametersEffect::ShowInterface(wxWindow& parent,
                                               const EffectDialogFactory& factory,
                                               bool forceModal)
{
    m_parent = &parent;
#endif

    InitArtProvider();
    
    auto dlg = new Aurora::AcParametersDialog(m_parent, this);
    dlg->CenterOnParent();

    if(! dlg->ShowModal())
    {        
        m_parent = nullptr;

        delete dlg;
        m_aAudioTracks.clear();
        Destroy();  // reset AcParameters 
        
        return false;
    }
    StoreConfigurationValues();

    delete dlg;
    m_bProcess = CalculateAcousticParameters();  // Tries to skip the ProgressDialog curse...

   return true;
}

//------------------------- Processing methods -------------------------
bool Aurora::AcParametersEffect::Process()
{
	// For the moment, leave this as is...
#ifndef __WXGTK__
//    m_pAp->SetProgressDialog(mProgress);
#endif

//    return (m_bProcess = m_pAp->CalculateAcousticParameters());

      // From Effect class: replaces current tracks
      // with mOutputTracks
      //ReplaceProcessedTracks(true);
	return true;
}

void Aurora::AcParametersEffect::End()
{
    if( m_bProcess && m_parent)
    {
        // Show parameters dialogs
        auto dlg = new Aurora::AcParametersShowDialog(NULL, this);
        dlg->CenterOnParent();

        if(dlg->ShowModal())
        {
            AddProcessedTracks();
        }

        delete dlg;
    }
    else if (! m_bAborted)
    {
        MessageBox("Processing interrupted by user or supernatural beings.",
                    Aurora::MessageType::Info);
    }
    m_parent = nullptr;

    Destroy(); // instead of 'delete m_pAp'
    m_aAudioTracks.clear();
}

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  module.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::TimeHistoryAnalyzerEffect
\brief The Audacity module front-end for Aurora's TimeHistoryAnalyzer class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of a TimeHistoryAnalyzer singleton that 
  does all calculations.

*//*******************************************************************/

#include <aurora.h>

// From Audacity source tree
#include <ModuleManager.h>
#include <PluginManager.h>
#include <Prefs.h>
#include <Project.h> 
#include <ViewInfo.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>
#include <widgets/ProgressDialog.h>

#include "TimeHistoryAnalyzer.h"
#include "ThAnalyzerDialogs.h"
#include "ThAnalyzerPlot.h"
#include "ThAnalyzerExports.h"
#include "ThAnalyzerEffect.h"

#include "ThAnalyzerUi.h"

//----------------------------------------------------------------------------
// Aurora::TimeHistoryAnalyzerEffect implementation
//----------------------------------------------------------------------------
#define THA_RESULTS_DLG_WIDTH   700 // TODO remove it
#define THA_RESULTS_DLG_HEIGTH  800 // TODO remove it

ComponentInterfaceSymbol Aurora::TimeHistoryAnalyzerEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora Time History Analizer") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::TimeHistoryAnalyzerEffect::GetDescription()
{
    return _("Analyze a signal in the time domain following ITU P56 stantard");
}
#else
TranslatableString Aurora::TimeHistoryAnalyzerEffect::GetDescription()
{
    return TranslatableString { XO("Analyze a signal in the time domain following ITU P56 stantard") };
}
#endif

PluginPath Aurora::TimeHistoryAnalyzerEffect::GetPath()
{
    return PluginPath("Aurora/TimeHistoryAnalyzer");
}

EffectType Aurora::TimeHistoryAnalyzerEffect::GetType()
{
    return EffectTypeAnalyze;
}

int Aurora::TimeHistoryAnalyzerEffect::GetTrackIndexFromName(wxString& name)
{
    int nChnl = 2; // This is the Audacity 'mono' case
    wxString trackName = name;
    
    if(trackName.Find("[left]") != wxNOT_FOUND)
    {
        nChnl = 0;
    }
    else if(trackName.Find("[right]") != wxNOT_FOUND)
    {
        nChnl = 1;
    }

    trackName = trackName.BeforeLast(' ');
    
    if(trackName.IsEmpty())
    {
        trackName = name;
    }

    int count   = 0;

    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
    {
        WaveTrack* wt = (WaveTrack*)*it;

        if(wt->GetName() == trackName && wt->GetChannel() == nChnl)
        {
        	return count;
        }
        count++;
    }
    return -1; // not found
}


void Aurora::TimeHistoryAnalyzerEffect::SetTracksNamesArray(wxArrayString& trackNames)
{
#if 1
    // clear array...
    
    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
    {
        WaveTrack* wt = (WaveTrack*)*it;

        wxString trackName = wt->GetName();
        wxString chName;
        Aurora::GetAudacityChannelName(chName, wt->GetChannel()); 
        
        trackName << " [" << chName << "]";
        trackNames.Add(trackName);        
    }

#else
        TrackListIterator iter(mOutputTracks);
    WaveTrack* pWt = (WaveTrack*)(iter.First());
    
    while (pWt != NULL)
    {
        trackName = wt->GetName();
        ::AuroraGetAudacityChannelName(wxszChnlName, wt->GetChannel()); // TODO check if fits THA needs
        trackName << wxT(" [") << wxszChnlName << wxT("]");
        atrackNames.Add(trackName);
        pWt = (WaveTrack*)(iter.Next());
    }
#endif
}

bool Aurora::TimeHistoryAnalyzerEffect::LoadTrackData(const int index, 
                                                      Aurora::AudioTrack* destTrack)
{
    int i = 0;
    auto it = mOutputTracks->begin();
    WaveTrack* wt = (index == 0 ? (WaveTrack*)*it : nullptr);

    while (wt == nullptr && it != mOutputTracks->end())
    {
        ++it;

        if (++i == index)
        {
            wt = (WaveTrack*)*it;
        }
    }
    if (wt == nullptr)
    {
        return false;
    }

    //Get start and end times from track
    double trackStart = wt->GetStartTime();
    double trackEnd   = wt->GetEndTime();

    //Set the current bounds to whichever left marker is
    //greater and whichever right marker is less:
    double t0 = mT0 < trackStart? trackStart: mT0;
    double t1 = mT1 > trackEnd  ? trackEnd  : mT1;

    // Process only if the right marker is to the right of the left marker
    if (t1 > t0)
    {
           

        //Transform the marker timepoints to samples
        Aurora::SampleCount start = wt->TimeToLongSamples(t0).as_size_t();
        Aurora::SampleCount   end = wt->TimeToLongSamples(t1).as_size_t();
#if 0
        // TODO: go deep with WaveClip code...
        Aurora::SampleCount clipEnd = 0;
        
        int nClips = wt->GetNumClips();
        auto& clips = wt->GetClips();

        for (WaveClipHolder& clip : clips)
        {
            WaveClip& wc = *clip;
        }
#endif
        destTrack->Resize(end - start);
        destTrack->SetRate(wt->GetRate());

        wt->Get((samplePtr) destTrack->Samples(), 
                floatSample, 
                sampleCount(start), 
                destTrack->Length() );
    }

    return true;
}


void Aurora::TimeHistoryAnalyzerEffect::AppendTracksToWorkspace()
{
    wxString name;

    if(m_aSignalTracks[0].GetSelectedFilter() != Aurora::AudioTrack::FLT_NONE)  // TODO what's mean this????
    {
        // If prompt user return true, add to workspace filtered tracks.
        for(size_t nCh = 0; nCh < m_aSignalTracks.size(); nCh++)
        {
            name = m_aSignalTracks[nCh].GetName();
            //		 name.Printf(wxT("Ch %d"), nCh);

            switch(m_aSignalTracks[nCh].GetSelectedFilter())
            {
                case Aurora::AudioTrack::FLT_ITU: name << " ITU filtered"; break;
                case Aurora::AudioTrack::FLT_IEC: name << " IEC filtered"; break;
                case Aurora::AudioTrack::FLT_ISO: name << " ISO filtered"; break;
                case Aurora::AudioTrack::FLT_UNI: name << " UNI filtered"; break;
                default: // shouldn't arrive here...
                    name << " not really filtered";
                    break;
            }
            name << " [" << int(nCh+1) << "]";

            auto wt = mFactory->NewWaveTrack(floatSample, mProjectRate);
            wt->Append((samplePtr)m_aSignalTracks[nCh].Filtered().Samples(),
		               floatSample,
		               m_aSignalTracks[nCh].GetLength() );
            wt->Flush();
            wt->SetName(name);
            AddToOutputTracks(wt);
        }
        this->ReplaceProcessedTracks(true);
    }
}


bool Aurora::TimeHistoryAnalyzerEffect::DoFullscaleCalibration(std::vector<double>& fullScales)
{
    size_t nCh = 0;
    
    for(auto& track : m_aCalibrationTracks)
    {
        auto calib = dynamic_cast<Aurora::AudioTrack*>(&track);
        
        if(! calib || ! LoadTrackData(track.GetIndex(), calib))
        {
            return false;
        }
        track.ResetFilteredTrack();

        fullScales[nCh] = track.GetReferenceLevel() - track.Leq();
        nCh++;
    }
    return true;
}

bool Aurora::TimeHistoryAnalyzerEffect::DoAnalysis()
{
    for(size_t nCh = 0; nCh < m_aSignalTracks.size(); nCh++)
    {
        auto dest = dynamic_cast<Aurora::AudioTrack*>(&m_aSignalTracks[nCh]);
        
        if(! dest || ! LoadTrackData(m_aSignalTracks[nCh].GetIndex(), dest))
        {
            return false;
        }
    }

    if(! Analyze())
    {
        MessageBox("Something strange happened.\n"
                   "Cannot calculate Time History parameters.",
                   Aurora::MessageType::Error);
        return false;
    }

    return true;
}

bool Aurora::TimeHistoryAnalyzerEffect::Init()
{
    const int nTracks = GetNumWaveTracks();

    if (nTracks == 0 || mT0 >= mT1)
    {
        MessageBox("No selection found.", Aurora::MessageType::Error);
        return false;
    }

    // The very first thing to do!
    Aurora::TimeHistoryAnalyzer::SetChannelsNumber(nTracks);

    this->CopyInputTracks(); // Set up mOutputTracks.

    return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::TimeHistoryAnalyzerEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
    
#else
bool Aurora::TimeHistoryAnalyzerEffect::ShowInterface(wxWindow& parent,
                                                      const EffectDialogFactory& factory,
                                                      bool forceModal)
{
    m_parent = &parent;
#endif

    // Setup graphics.
    InitArtProvider();

    auto dlg = new TimeHistoryAnalyzerFrame(m_parent, this);
    dlg->CenterOnParent();

    const bool ok = dlg->ShowModal();

    delete dlg;
    return ok;
}

bool Aurora::TimeHistoryAnalyzerEffect::Process()
{
	AppendTracksToWorkspace();
    return true;
}

void Aurora::TimeHistoryAnalyzerEffect::End()
{
   
}





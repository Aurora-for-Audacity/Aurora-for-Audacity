/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  ConvolverEffect.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::ConvolverEffect
\brief The Audacity module front-end for Aurora Convolver class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of an Aurora::Convolver singleton that 
  does all calculations.

*//*******************************************************************/
#include <aurora.h>
#include <wx/dnd.h>

// From Audacity source tree
#include <effects/EffectManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include "AuroraConvolver.h"
#include "TrackProperties.h"
#include "ConvolverDialogs.h"
#include "ConvolverUi.h"
#include "ConvolverEffect.h"

//----------------------------------------------------------------------------
// Aurora::ConvolverEffect implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::ConvolverEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora Convolver") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::ConvolverEffect::GetDescription()
{
    return _("Convolve the tracks of the workspace.");
}
#else
TranslatableString Aurora::ConvolverEffect::GetDescription()
{
    return TranslatableString { XO("Convolve the tracks of the workspace.") };
}
#endif

PluginPath Aurora::ConvolverEffect::GetPath()
{
    return PluginPath("Aurora/Convolver");
}

EffectType Aurora::ConvolverEffect::GetType()
{
    return EffectTypeTool;
}

// end of interface implementation

bool Aurora::ConvolverEffect::LoadTrackData(WaveTrack *wt, int idx, bool is_ir)
{
    Aurora::SampleCount start;
    Aurora::SampleCount end;

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
        //Checks the track rate, samples, minimum, maximum and name
        if (! CheckSamplerate(wt->GetRate()))
        {
            return false;
        }

        //Transform the marker timepoints to samples
        start = wt->TimeToLongSamples(t0).as_size_t();
        end   = wt->TimeToLongSamples(t1).as_size_t();

        if(is_ir)
        {
            if (ResizeFilterTrack(idx, end - start))
            {
                Aurora::Vector<Aurora::Sample>& f = GetFilters()[idx];
                
                wt->Get((samplePtr)f.Samples(),
                        floatSample,
                        sampleCount(start),
                        f.Length());
            }
        } 
        else 
        {
            if (ResizeInputTrack(idx, end - start))
            {
                Aurora::Vector<Aurora::Sample>& in = GetInputTrack(idx);
                wt->Get((samplePtr)in.Samples(),
                        floatSample,
                        sampleCount(start),
                        in.Length());
            }
        }
    }
    return true;
}

bool Aurora::ConvolverEffect::Init()
{
    if (GetNumWaveTracks() == 0 || mT0 >= mT1)
    {
        MessageBox("No selection found.", Aurora::MessageType::Error);
        return false;
    }

    Aurora::ConvolverController::Reset();
    Aurora::ConvolverController::SetSamplerate(mProjectRate); // ???
    return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::ConvolverEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
        
#else
bool Aurora::ConvolverEffect::ShowInterface(wxWindow& parent,
                                            const EffectDialogFactory& factory,
                                            bool forceModal)
{
    m_parent = &parent;
#endif

    // ------------- Select tracks to process
    int nTracks = GetNumWaveTracks();

    if (nTracks < 2)
    {
        MessageBox("Too few tracks for convolution process (at least two needed)!",
                   Aurora::MessageType::Error);
        return false;
    }

    // ------------------ Prepare track info list ---------------
    Aurora::ConvolverTrackProperties atp(nTracks);
    Aurora::ConvolverTrackInfoItem trinfo;

    this->CopyInputTracks(); // Set up mOutputTracks.

    int curTrackNum = 0;

    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
    {
        WaveTrack* wt = (WaveTrack*)*it;

        trinfo.SetIndex(curTrackNum);
        trinfo.SetName(wt->GetName());
        trinfo.SetChannel(wt->GetChannel());
        atp.AddTrackInfoItem(trinfo);
        curTrackNum++;
    }
    // ------------------ end of Prepare track info list ---------------

    // Gui stuffs
    InitArtProvider();

    {
        // Show Track Selector dialog
        Aurora::ConvolverTrackSelectorDialog dlg(m_parent, this, &atp);
        dlg.CenterOnParent();

        if (!dlg.ShowModal())
        {
            return false;
        }
    }
    
    // This initializes the MultiVolver instance!
    SetFilterMatrixDimensions(atp.GetInputRowsCount(),
                              atp.GetFilterRowsCount());

    Aurora::ConvolverTrackInfoItem* tinfo;

    // ---------- get filters --------------
    int idx = 0;

    for (idx = 0; idx < atp.GetFilterRowsCount(); ++idx)
    {
        curTrackNum = 0;

        for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
        {
            WaveTrack* wt = (WaveTrack*)*it;

            tinfo = atp.GetTrackInfoItem(curTrackNum);

            if (tinfo->IsIr() && (tinfo->GetRow() == idx))
            {
                if (! LoadTrackData(wt, idx, true))
                {
                    MessageBox("Sample rate mismatch or error copying data "
                               "from workspace!!",
                               Aurora::MessageType::Error);
                    return false;
                }
            }
            curTrackNum++;
        }
    }

    // ---------- get Input --------------
    for (idx = 0; idx < atp.GetInputRowsCount(); ++idx)
    {
        curTrackNum = 0;
        
        for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
        {
            WaveTrack* wt = (WaveTrack*)*it;

            tinfo = atp.GetTrackInfoItem(curTrackNum);

            if (tinfo->IsInput() && (tinfo->GetRow() == idx))
            {
                if (! LoadTrackData(wt, idx, false))
                {
                    MessageBox("Sample rate mismatch or error copying data "
                               "from workspace!!",
                               Aurora::MessageType::Error);
                    return false;
                }
            }
            curTrackNum++;
        }
    }

    Aurora::ConvolverDialog dlog(m_parent, this);
    dlog.CenterOnParent();

    if( !dlog.ShowModal() )
    {
        MessageBox("Convolution calculation aborted.",
                   Aurora::MessageType::Info);
        return false;
    }
    return true;
}

//------------------------- Processing methods -------------------------
bool Aurora::ConvolverEffect::Process()
{
    if(! DoConvolution())
    {
        return false;
    }
    //If everything goes right, store gain and whatever...
    StoreConfigurationValues();

    wxString name;

    for(int ch = 0; ch < FilterRows(); ++ch)
    {
        auto wt   = mFactory->NewWaveTrack(floatSample, mProjectRate);
        auto& out = GetOutputTrack(ch);       

        wt->Append((samplePtr)out.Samples(), 
                   floatSample, 
                   out.Length());
        wt->Flush();

        name.Printf(_("Convolved %d"), ch+1);
        wt->SetName(name);

        AddToOutputTracks(wt);
    }   

    // add mOutput tracks to workspace
    this->ReplaceProcessedTracks(true);
    return true;
}

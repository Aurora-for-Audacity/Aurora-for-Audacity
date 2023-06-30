/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Speech Transmission Index evaluator

  STIEffect.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class Aurora::STIEffect
\brief The Audacity module front-end for Aurora STI class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of an AuroraSTI singleton that 
  does all calculations.

*//*******************************************************************/

#include <aurora.h>

#include <ModuleManager.h>
#include <PluginManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>

#include "sti.h"
#include "StiTrack.h"
#include "StiEffect.h"
#include "StiExports.h"
#include "StiData.h"
#include "StiPlot.h"
#include "StiDialogs.h"
#include "StiSpectrumDialog.h"
#include "StiSpectrum.h"
#include "StiUi.h"

//----------------------------------------------------------------------------
// Aurora::STIEffect implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::STIEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora STI") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::STIEffect::GetDescription()
{
    return _("The Aurora Speech Transmission Index calculator.");
}
#else
TranslatableString Aurora::STIEffect::GetDescription()
{
    return TranslatableString { XO("The Aurora Speech Transmission Index calculator.") };
}
#endif

PluginPath Aurora::STIEffect::GetPath()
{
    return PluginPath("Aurora/STI");
}

EffectType Aurora::STIEffect::GetType()
{
    return EffectTypeTool;
}

bool Aurora::STIEffect::LoadTrackData(Aurora::STIAudioTrack& destTrack, 
                                      const int nCh, 
                                      const int index)
{
   
    int i = 0;
    auto it = mOutputTracks->begin();
    WaveTrack* wt = (index == 0  ? (WaveTrack*)*it : nullptr);
    
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
//       Aurora::SampleCount clipEnd = 0;

        //Transform the marker timepoints to samples
        Aurora::SampleCount start = wt->TimeToLongSamples(t0).as_size_t();
        Aurora::SampleCount   end = wt->TimeToLongSamples(t1).as_size_t();
#if 0
        // TODO: go deep with WaveClip code...

        int nClips = wt->GetNumClips();
        auto& clips = wt->GetClips();

        for (auto& clip : clips)
        {
            clipEnd = clip->GetEndSample();
        }
#endif
        destTrack.Resize(end - start);
        destTrack.SetRate(wt->GetRate());

        wt->Get((samplePtr) destTrack.Samples(), 
                floatSample, 
                sampleCount(start), 
                destTrack.Length() );
   }
   
   return true;
}

bool Aurora::STIEffect::DoFullscaleCalibration(const bool bIsStereo)
{
    const int nChnls = bIsStereo ? 2 : 1;
    std::array<Aurora::STIAudioTrack, 2> calibrationSignal;
    
    for(int nCh = 0; nCh < nChnls; nCh++)
    {
        calibrationSignal[nCh].SetRate(mProjectRate);
        
        if( ! LoadTrackData(calibrationSignal[nCh], nCh, m_anCalibrationTracksIdx[nCh]-1))
        {
            return false;
        }
        CalibrateFullscale(calibrationSignal[nCh], nCh);
    }
    return true;
}

bool Aurora::STIEffect::DoSNRatioCalculation(const bool bIsStereo)
{
    const int nChnls = bIsStereo ? 2 : 1;
    
    std::array<Aurora::STIAudioTrack, 2> signal;
    std::array<Aurora::STIAudioTrack, 2> noise;

    std::vector<int> ranges = { nChnls, 12 };
    ProgressMeterWrapper::Show("Octave Spectrum Calculation...", ranges);
    
    for(int nCh = 0; nCh < nChnls; nCh++)
    {
        signal[nCh].SetRate(mProjectRate);
        noise [nCh].SetRate(mProjectRate);
        
        if(    ! LoadTrackData(signal[nCh], nCh, m_anSignalTracksIdx[nCh]-1) 
            || ! LoadTrackData(noise [nCh], nCh, m_anNoiseTracksIdx [nCh]-1)
            || ! StoreSpectrums(signal[nCh], noise[nCh], nCh) )
        {
            ProgressMeterWrapper::Destroy();
            return false;
        }
        ProgressMeterWrapper::Update(nCh, 0); // update overall;
    }
    ProgressMeterWrapper::Destroy();
    return true;
}

bool Aurora::STIEffect::DoSTICalculation()
{
    int nChnls = m_bIsStereo ? 2 : 1;
    
    std::array<Aurora::STIAudioTrack, 2> tracks;
    
    std::vector<int> ranges = { nChnls, 7 };
    ProgressMeterWrapper::Show("Computing Speech Transmission Index...", ranges);

    for(int nCh = 0; nCh < nChnls; nCh++)
    {    
        tracks[nCh].SetRate(mProjectRate);
        
        if(! LoadTrackData(tracks[nCh], nCh, m_anIrTracksIdx[nCh]-1))
        {
            ProgressMeterWrapper::Destroy();
            return false;
        }
        CalculateMatrix(tracks[nCh], nCh);    
        ProgressMeterWrapper::Update(nCh, 0); // update overall;
    }    
    ProgressMeterWrapper::Destroy();
    return true;
}
   
bool Aurora::STIEffect::Init()
{
    if (GetNumWaveTracks() == 0 || mT0 >= mT1)
    {
        MessageBox("No selection found.", Aurora::MessageType::Error);
        return false;
    }
    
    m_trackNames.Clear();

    // The very first thing to do!
    ReadConfigurationValues();

    FindProject();

    this->CopyInputTracks(); // Set up mOutputTracks.       

   return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::STIEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
    
#else
bool Aurora::STIEffect::ShowInterface(wxWindow& parent,
                                      const EffectDialogFactory& factory,
                                      bool forceModal)
{
    m_parent = &parent;
#endif

    // ------------------ Prepare track info list ---------------
    wxString trackName;
    wxString chnlName;
        
    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)		
    {
        WaveTrack* wt = (WaveTrack*)*it; 

        trackName = wt->GetName();
        Aurora::GetAudacityChannelName(chnlName, wt->GetChannel());
        trackName << " [" << chnlName << "]";
        m_trackNames.Add(trackName);        
    }   
    // ------------------ end of Prepare track info list ---------------

    // Gui stuffs
    InitArtProvider();
    
    // Show Track Selector dialog
    STISetupDialog* pDlg = new STISetupDialog(m_parent, this);
    pDlg->CenterOnParent();   
    m_bShowResults = false;
    
    if(pDlg->ShowModal())
    {
        StoreConfigurationValues(); // Save STI environment

        // I would do all these calculation in Process() but on GTK
        // the ProgressMeter always crashes!

        if(! IsIrTrackSet(CH_LEFT) ) // At least left channel must be set
        {
            delete pDlg;
            return false;
        }    
        m_bIsStereo    = AreIrTracksSet();        
        m_bShowResults = DoSTICalculation();
    }
    
    delete pDlg;    
    return true;
}

//------------------------- Processing methods -------------------------
bool Aurora::STIEffect::Process()
{
    return false; // no modification on waveform in the
                  // workspace, so in fact no Process()
                  // has been done.
}

void Aurora::STIEffect::End()
{
    if (m_bShowResults)
    {
        STIShowDialog* pDlg = new STIShowDialog(m_parent, this, m_bIsStereo);
        pDlg->CenterOnParent();
        pDlg->ShowModal();

        delete pDlg;
    }
}

bool Aurora::STIEffect::AreCalibrationTracksSet() const 
{ 
    return (m_anCalibrationTracksIdx[CH_LEFT]  != 0 && 
            m_anCalibrationTracksIdx[CH_RIGHT] != 0);     
}

bool Aurora::STIEffect::AreIrTracksSet()  const 
{ 
    return (m_anIrTracksIdx[CH_LEFT] != 0 && m_anIrTracksIdx[CH_RIGHT] != 0);     
}

bool Aurora::STIEffect::AreSignalAndNoiseSet(const int nChnl) const
{ 
    return (m_anSignalTracksIdx[nChnl] != 0 &&  m_anNoiseTracksIdx[nChnl] != 0); 
}

// ------ Ctors
Aurora::STIEffect::STIEffect()
 : m_bIsStereo(false)
{ 
    m_anCalibrationTracksIdx [0] = 0;
    m_anNoiseTracksIdx       [0] = 0;
    m_anSignalTracksIdx      [0] = 0;
    m_anIrTracksIdx          [0] = 0;
    m_anCalibrationTracksIdx [1] = 0;
    m_anNoiseTracksIdx       [1] = 0;
    m_anSignalTracksIdx      [1] = 0;
    m_anIrTracksIdx          [1] = 0;    
}


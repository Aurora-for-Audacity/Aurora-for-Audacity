/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  module.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class EffectXFunctions
\brief The Audacity module front-end for Aurora Convolver class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of an XFunctions singleton that 
  does all calculations.

*//*******************************************************************/

#include <aurora.h>

#include <ModuleManager.h>
#include <PluginManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>

#include "correlator.h"

#include "XFunctionsData.h"
#include "XFunctionsPlot.h"
#include "XFunctionsExports.h"
#include "XFunctionsArt.h"
#include "XFunctionsDialogs.h"
#include "XFunctionsEffect.h"
#include "XFunctionsUi.h"

//----------------------------------------------------------------------------
// EffectXFunctions implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::XFunctionsEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora XFunctions") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::XFunctionsEffect::GetDescription()
{
    return _("The Aurora Cross-functions set.");
}
#else
TranslatableString Aurora::XFunctionsEffect::GetDescription()
{
    return TranslatableString { XO("The Aurora Cross-functions set.") };
}
#endif

PluginPath Aurora::XFunctionsEffect::GetPath()
{
    return PluginPath("Aurora/XFunctions");
}

EffectType Aurora::XFunctionsEffect::GetType()
{
    return EffectTypeTool;
}

bool Aurora::XFunctionsEffect::Init()
{
    Aurora::Correlator::SetSamplerate(mProjectRate);

    int nTracks = GetNumWaveTracks();

    if(nTracks != 2)
    {
        //For what reason one should analyze more than 8 tracks...    
        MessageBox("XFunctions works with two (2) tracks.\nNo less, no more.",
                   Aurora::MessageType::Error);
        return false;
    }

    this->CopyInputTracks(); // Set up mOutputTracks.
    //bool bGoodResult = true;

    int curTrackNum = 0;
    
    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)		
    {
        WaveTrack* wt = (WaveTrack*)*it; 
        //Get start and end times from track
        double trackStart = wt->GetStartTime();
        double trackEnd   = wt->GetEndTime();

        //Set the current bounds to whichever left marker is
        //greater and whichever right marker is less:
        const double t0 = mT0 < trackStart ? trackStart : mT0;
        const double t1 = mT1 > trackEnd   ? trackEnd   : mT1;
        
        if (t0 == t1)
        {
            //For what reason one should analyze more than 8 tracks...    
            MessageBox("Invalid selection. Or nothing selected at all.",
                        Aurora::MessageType::Error);
            return false;
        }

        // Process only if the right marker is to the right of the left marker
        if (t1 > t0) 
        {
            //Transform the marker timepoints to samples
            Aurora::SampleCount start = wt->TimeToLongSamples(t0).as_size_t();
            Aurora::SampleCount end   = wt->TimeToLongSamples(t1).as_size_t();
            
            ResizeInput(end - start, curTrackNum);
            auto& iv = GetInputVector(curTrackNum);
            
            wt->Get( (samplePtr)iv.Samples(),
                     floatSample,
                     sampleCount(start), 
                     iv.Length() );
        }

        //Iterate to the next track
        curTrackNum++;
    }

    return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::XFunctionsEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
    
#else
bool Aurora::XFunctionsEffect::ShowInterface(wxWindow& parent,
                                             const EffectDialogFactory& factory,
                                             bool forceModal)
{
    m_parent = &parent;
#endif

    InitArtProvider();

    XFunctionsDialog dlog(m_parent, this);
    dlog.CenterOnParent();

    if(! dlog.ShowModal()) 
    {
        return false;
    }
    return true;
}

//------------------------- Processing methods -------------------------
bool Aurora::XFunctionsEffect::Process()
{
   // Get it big!
    if(! Aurora::Correlator::Process())
    {   
        return false;
    }
    //If everything goes right, store gain and whatever...
    StoreConfigurationValues();
   
    XFunctionsShowDialog dlog(m_parent, this);
    dlog.CenterOnParent();

    if(dlog.ShowModal()) 
    {
        int ch = 0;  
        wxString name;
        
        while(ch < GetChannelsToExport())
        {      
            auto wt = mFactory->NewWaveTrack(floatSample, mProjectRate);
            wt->Append((samplePtr)GetResultsVector(ch).CSamples(), 
                       floatSample,
                       GetResultsVector(ch).Length() );
            wt->Flush();            
            wt->SetName(GetResultsVector(ch).GetLabel());
            
            AddToOutputTracks(wt);
            ch++;
        }
        this->ReplaceProcessedTracks(true);
    }

    return true;
}

void Aurora::XFunctionsEffect::ReadConfigurationValues() 
{
// Read parameter from stored configuration
    Aurora::Config cfg;
    
    bool   bValue;
    double dValue;
    Aurora::SampleCount lValue;
   
    if(cfg.Read("/Aurora/XFunctions/FftSize", &lValue))
    {
        SetFFTLength(lValue);
    }
    if(cfg.Read("/Aurora/XFunctions/WindowType", &lValue))
    {
        SetWindowType(int(lValue));
    }
    if(cfg.Read("/Aurora/XFunctions/XFunctionType", &lValue))
    {
        SetXFunctionType(int(lValue));
    }
    if(cfg.Read("/Aurora/XFunctions/SoundSpeed", &dValue))
    {
        SetSoundSpeed(dValue);
    }
    if(cfg.Read("/Aurora/XFunctions/ProbeMicDistance", &dValue))
    {
        SetProbeDistance(dValue);   
    }
    if(cfg.Read("/Aurora/XFunctions/ProbeMaxFreq", &dValue))
    {
        SetProbeMaxFreq(dValue);
    }
    if(cfg.Read("/Aurora/XFunctions/FollowingFilter", &bValue))  
    {
        SetFollowingFilter(bValue);
    }
    if(cfg.Read("/Aurora/XFunctions/FollowingBandwidth", &dValue))  
    {
        SetFollowingBandwidth(dValue);   
    }
    if(cfg.Read("/Aurora/XFunctions/Normalize", &bValue))  
    {
        m_options.normalize = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/ShiftToHalfWindow", &bValue))  
    {
        m_options.shiftToHalfWindow = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/CoherenceWeighting", &bValue))  
    {
        m_options.coherenceWeighting = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/HilbertTransform", &bValue))  
    {
        m_options.squaredHilbertTransform = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/TimeReversal", &bValue))  
    {
        m_options.timeReversal = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/DiracPulse", &bValue))
    {
        m_options.diracPulse = bValue;
    }
    if(cfg.Read("/Aurora/XFunctions/TriggerLevel", &dValue)) 
    {
        SetTriggerLevel(dValue);
    }
}

void Aurora::XFunctionsEffect::StoreConfigurationValues() 
{
    Aurora::Config cfg;
    
    cfg.Write("/Aurora/XFunctions/FftSize", GetFFTLength());
    cfg.Write("/Aurora/XFunctions/WindowType",    Aurora::SampleCount(GetWindowType()));
    cfg.Write("/Aurora/XFunctions/XFunctionType", Aurora::SampleCount(GetXFunctionType()));
    cfg.Write("/Aurora/XFunctions/SoundSpeed",       GetSoundSpeed());
    cfg.Write("/Aurora/XFunctions/ProbeMicDistance", GetProbeDistance());
    cfg.Write("/Aurora/XFunctions/ProbeMaxFreq",     GetProbeMaxFreq());
    cfg.Write("/Aurora/XFunctions/FollowingFilter",    IsFollowingFilterEnabled());
    cfg.Write("/Aurora/XFunctions/FollowingBandwidth", GetFollowingBandwidth());
    cfg.Write("/Aurora/XFunctions/Normalize",          m_options.normalize);
    cfg.Write("/Aurora/XFunctions/ShiftToHalfWindow",  m_options.shiftToHalfWindow);
    cfg.Write("/Aurora/XFunctions/CoherenceWeighting", m_options.coherenceWeighting);
    cfg.Write("/Aurora/XFunctions/HilbertTransform",   m_options.squaredHilbertTransform);
    cfg.Write("/Aurora/XFunctions/TimeReversal",       m_options.timeReversal);
    cfg.Write("/Aurora/XFunctions/DiracPulse",         m_options.diracPulse);
    cfg.Write("/Aurora/XFunctions/TriggerLevel",       GetTriggerLevel());

    // Writes immediately changes
    cfg.Flush();
}


// ------ Ctors
Aurora::XFunctionsEffect::XFunctionsEffect() 
{
    m_xfnNames.Add("Auto-Correlation");
    m_xfnNames.Add("Cross-Correlation");
    m_xfnNames.Add("White Cross Correlation");
    m_xfnNames.Add("Cross Power Spectrum Ph.");
    m_xfnNames.Add("Transfer Function H1");
    m_xfnNames.Add("Transfer Function H2");
    m_xfnNames.Add("Transfer Function H3");
    m_xfnNames.Add("Absorp.Coefficient (PU probe)");
    m_xfnNames.Add("Absorp.Coefficient (PP probe)");
    m_xfnNames.Add("PU Probe Calib. Rigid tube ");    
    m_xfnNames.Add("Pu Probe Calib. Free Field "); 

    // Read parameters value from stored configuration
    ReadConfigurationValues();
}

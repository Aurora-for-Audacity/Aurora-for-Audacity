/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Invert Kirkeby

  module.cpp

  Angelo Farina - Simone Campanini

*******************************************************************//**

\class KirkebyEffect
\brief The Audacity module front-end for Aurora's Kirkeby class.

  This class is an Audacity module (plug-in) that loads on run time.
  Its core is the instantiation of a Kirkeby singleton that does
  all calculations.

*//*******************************************************************/
#include <aurora.h>

#include <ModuleManager.h>
#include <PluginManager.h>
#include <Prefs.h>
#include <WaveTrack.h>

#include <effects/Effect.h>
#include <effects/EffectManager.h>
#include <widgets/ProgressDialog.h>

#include "KirkebyDialogs.h"
#include "KirkebyEffect.h"

#include "KirkebyUi.h"


static void kissTest()
{
    const int N = 2048;

    Aurora::AudioTrack x(N);
    Aurora::ComplexVector u;
    Aurora::SamplesVector y;

    x.MakeSine(0.6, 440.0);
    u.Resize(N);
    y.Resize(N);

    kiss_fftr_cfg fwd = kiss_fftr_alloc(N, 0, NULL, NULL);
    kiss_fftr_cfg rwd = kiss_fftr_alloc(N, 1, NULL, NULL);

    kiss_fftr(fwd, x.CSamples(), u.Samples());
    kiss_fftri(rwd, u.CSamples(), y.Samples());

    //const float yMax = y.FindAbsMax();
    y /= (float)N;
    //const float yMaxScaled = y.FindAbsMax();

    Aurora::AudioTrack x1(N);
    Aurora::ComplexVector u1;

    x1.MakeSine(0.6, 23900.0);
    u1.Resize(N);

    kiss_fftr(fwd, x1.CSamples(), u1.Samples());
    kiss_fftri(rwd, u1.CSamples(), y.Samples());

    Aurora::AudioTrack x2(N);
    Aurora::ComplexVector u2;
    
    x2.MakeSine(0.6, 1000.0);
    u2.Resize(N);
    x2 += 0.4;

    kiss_fftr(fwd, x2.CSamples(), u2.Samples());
    kiss_fftri(rwd, u2.CSamples(), y.Samples());

    kiss_fftr_free(fwd);
    kiss_fftr_free(rwd);

    // ok, kss_fft real transform produces output complex vector
    // that is N/2 + 1 long, DC is located at u[0].r and nyquist
    // component at u[N/2].

    // Antitrasformed signal is UNSCALED! You need to divide the
    // result vector by N.
}

//----------------------------------------------------------------------------
// KirkebyEffect implementation
//----------------------------------------------------------------------------
ComponentInterfaceSymbol Aurora::KirkebyEffect::GetSymbol()
{
    return ComponentInterfaceSymbol{ XO("Aurora Invert Kirkeby") };
}

#if AUDACITY_HEX_VERSION < 0x020400
wxString Aurora::KirkebyEffect::GetDescription()
{
    return _("Compute a filter inversion using Ole Kirkeby method.");
}
#else
TranslatableString Aurora::KirkebyEffect::GetDescription()
{
    return TranslatableString { XO("Compute a filter inversion using Ole Kirkeby method.") };
}
#endif

PluginPath Aurora::KirkebyEffect::GetPath()
{
    return PluginPath("Aurora/InvertKirkeby");
}

EffectType Aurora::KirkebyEffect::GetType()
{
    return EffectTypeTool;
}

int Aurora::KirkebyEffect::GetSelectionLength() const
{
    return int(m_end.as_size_t() - m_start.as_size_t());
}

bool Aurora::KirkebyEffect::LoadTracks()
{
    int  nCurChannel; // quite unuseful...
    const int length = GetSelectionLength();
    
    SetInputTrackLength(length); // Set tracks length

    int nRow = 0;
    int nCol = 0;
    const int nColsNo = GetCols();
    const int nColumnWidth = length / nColsNo;
        
    auto it = mOutputTracks->begin();
    WaveTrack* wt = (WaveTrack*)(*it);

    //Get the track rate
    SetSamplerate(wt->GetRate());
    
    while (it != mOutputTracks->end())
    {
        wt = (WaveTrack*)(*it);

        //Get current channel (unuseful..)
        nCurChannel = wt->GetChannel();

        //m_pKirk->AddTrackName(waveTrack->GetName());
        int col = 0;

        while(col < nColsNo)
        {
            auto& input = GetInputTrackItem(nRow, nCol);

            if(input.IsNull())
            {
                printf("Error in memory allocation\n");
                return false;
            }
            wt->Get((samplePtr)input.Samples(), 
                    floatSample, 
                    m_start + col * nColumnWidth,
                    nColumnWidth);
            col++;
        }

        //Iterate to the next track
        ++it;
        ++nRow;
    }

    
    return true;
}

void Aurora::KirkebyEffect::ReadConfigurationValues()
{
   // Read parameter from stored configuration
   //double value;
   //if(mCfg.Read(_("/Aurora/AcParameters/UserMinLevel"), &value))
   //  SetUserMinLevel(value);
}

void Aurora::KirkebyEffect::StoreConfigurationValues()
{
   //mCfg.Write(wxT("/Aurora/AcParameters/UserMinLevel"), GetUserMinLevel());
}
bool Aurora::KirkebyEffect::Init()
{   
#if _DEBUG
    kissTest();
#endif
    // The very first thing to do!
    const int nTracks = GetNumWaveTracks();
    
    if (nTracks == 0 || mT0 >= mT1)
    {
        MessageBox("No selection found.", Aurora::MessageType::Error);
        return false;
    }

    Aurora::KirkebyBase::Init(nTracks);
    
    SetSamplerate(mProjectRate);
    
    this->CopyInputTracks(); // Set up mOutputTracks.
    
    for (auto it = mOutputTracks->begin(); it != mOutputTracks->end(); ++it)
    {
        WaveTrack* wt = (WaveTrack*)*it;
        
        //Get start and end times from track
        double trackStart = wt->GetStartTime();
        double trackEnd   = wt->GetEndTime();
        
        //Set the current bounds to whichever left marker is
        //greater and whichever right marker is less:
        double t0 = mT0 < trackStart ? trackStart : mT0;
        double t1 = mT1 > trackEnd   ? trackEnd   : mT1;
        
        // Process only if the right marker is to the right of the left marker
        if (t1 <= t0) 
        {
            return false;
        }

        //Transform the marker timepoints to samples
        m_start = wt->TimeToLongSamples(t0);
        m_end   = wt->TimeToLongSamples(t1);

        SetInputTrackLength(m_end.as_size_t() - m_start.as_size_t());
        
        //Check if it's a single stereo track
        if(nTracks == 2 && wt->GetChannel() == 0)
        {
            m_bIsStereo = true;
        }
    }
    return true;
}

#if AUDACITY_HEX_VERSION < 0x020400
bool Aurora::KirkebyEffect::PromptUser(wxWindow* parent)
{
    m_parent = parent;
            
#else
bool Aurora::KirkebyEffect::ShowInterface(wxWindow& parent,
                                          const EffectDialogFactory& factory,
                                          bool forceModal)
{
    m_parent = &parent;
#endif

    // Gui stuffs
    InitArtProvider();

    Aurora::KirkebyDialog dlog(m_parent, this, m_bIsStereo);
    dlog.CenterOnParent();

    if(!dlog.ShowModal())
    {
        KirkebyBase::Destroy();
        return false;
    }
   
    m_bProcess = true;
    return true;
}

//------------------------- Processing methods -------------------------
bool Aurora::KirkebyEffect::Process()
{
    if(!m_bProcess)
    {
        printf("Calculation aborted!\n");
        return false;
    }
    
    if(! KirkebyBase::Init())
    {
        MessageBox("Kirkeby module initialization failed.",
                    Aurora::MessageType::Error);
        return false;
    }

    if(! LoadTracks())
    {
        MessageBox("Something strange occourred.\nCannot load tracks in memory.",
                    Aurora::MessageType::Error);
        return false;
    }

    int nRow = 0, nCol = 0;    
    wxString name;

    if(! KirkebyBase::Process())
    {
        MessageBox("Something strange occourred.\nCannot calculate Kirkeby Inverse filter.",
                    Aurora::MessageType::Error);
        return false;
    }
    
    // TODO: if(m_bIsStereo) Generate stereo output track.
    while(nRow < GetRows())
    {
        name.Printf("Inverse Filter %d", nRow + 1);
        auto wt = mFactory->NewWaveTrack(floatSample, mProjectRate);
        nCol = 0;

        while(nCol < GetCols())
        {
            samplePtr data = (samplePtr)GetOutputTrackItem(nRow, nCol).Samples();
            auto length = GetInverseFilterLength();

            wt->Append(data, floatSample, length );
            nCol++;
        }
        wt->Flush();
        wt->SetName(name);
        AddToOutputTracks(wt);
        nRow++;
    }

    this->ReplaceProcessedTracks(true);

   return true;
}


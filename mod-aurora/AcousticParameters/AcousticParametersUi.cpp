#include "AcousticParametersUi.h"
#include "widgets/valnum.h"
#include "../res/aurora_logos.h"
#include "../mod_aurora.h"
#include "Theme.h"
#include "AllThemeResources.h"
#include "Track.h"
#include "WaveTrack.h"
#include "ProjectRate.h"
#include <Aurora/AcParametersTrack.h>

#define AcousticParametersTitle XO("Acoustic Parameter Analysis")

//------------------------------------------------------------------------------------
// Hook up event handles

const std::array<wxString, 17> rowLabels =
{
    "Signal [dB]",
    "Noise [dB]",
    "Strength [dB]",
    "C50 [dB]",
    "C80 [dB]",
    "D50 [%]",
    "Ts [ms]",
    "EDT [s]",
    "Tuser [s]",
    "T20 [s]",
    "T30 [s]",
    "St1 [dB]",
    "St2 [dB]",
    "StLate [dB]",
    "IACC (All)",
    "t IACC [ms]",
    "WIACC [ms]"
};

const std::vector<std::string> rowParameters =
{
    "Signal [dB]",
    "Noise [dB]",
    "Strength [dB]",
    "C50 [dB]",
    "C80 [dB]",
    "D50 [%]",
    "Ts [ms]",
    "EDT [s]",
    "Tuser [s]",
    "T20 [s]",
    "T30 [s]",
    "St1 [dB]",
    "St2 [dB]",
    "StLate [dB]",
    "IACC (All)",
    "t IACC [ms]",
    "WIACC [ms]"
};

const std::array<wxString, 12> columnLabels =
{
    "31.5",
    "63",
    "125",
    "250",
    "500",
    "1k",
    "2k",
    "4k",
    "8k",
    "16k",
    "A",
    "Lin"
};

std::vector<double> times =
{
    0.0, 0.01, 0.02, 0.034,
};

std::vector<double> levels =
{
    -0.0, -6.0, -12.0, -24.2,
};


enum {
    FirstID = 10000,
    SetupButtonID,
    SaveRButtonID,
    CopyRButtonID,
    StoreGButtonID,
    FilterButtonID,
    CloseButtonID,
};


BEGIN_EVENT_TABLE(AcousticParametersUi, wxDialogWrapper)
EVT_CLOSE(AcousticParametersUi::OnCloseWindow)
EVT_SIZE(AcousticParametersUi::OnSize)
EVT_BUTTON(SetupButtonID,  AcousticParametersUi::OnSetup )
EVT_BUTTON(SaveRButtonID,  AcousticParametersUi::OnSaveR )
EVT_BUTTON(CopyRButtonID,  AcousticParametersUi::OnCopyR )
EVT_BUTTON(StoreGButtonID, AcousticParametersUi::OnStoreG )
EVT_BUTTON(FilterButtonID, AcousticParametersUi::OnFilter )
EVT_BUTTON(CloseButtonID, AcousticParametersUi::OnCloseButton)
END_EVENT_TABLE()

//------------------------------------------------------------------------------------

AcousticParametersUi::AcousticParametersUi(wxWindow *parent, wxWindowID id,
                                           AudacityProject &project,
                                           const TranslatableString &title,
                                           const wxPoint &pos)
: wxDialogWrapper(parent, id, title, pos, wxDefaultSize,
                  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX),
mProject{&project}
{
    std::cout << __func__ << '\n';
    SetName();
    
    Populate();
}

AcousticParametersUi::~AcousticParametersUi()
{
    std::cout << __func__ << '\n';
}

bool AcousticParametersUi::Show(bool show)
{
    std::cout << __func__ << '\n';
    bool res = wxDialogWrapper::Show(show);
    
    return res;
}

//------------------------------------------------------------------------------------
#pragma mark events

void AcousticParametersUi::OnSetup(wxCommandEvent & event)
{
    std::cout << __func__ << '\n';
}

void AcousticParametersUi::OnSaveR(wxCommandEvent & event)
{
    std::cout << __func__ << '\n';
}

void AcousticParametersUi::OnCopyR(wxCommandEvent & event)
{
    std::cout << __func__ << '\n';
}

void AcousticParametersUi::OnStoreG(wxCommandEvent & event)
{
    std::cout << __func__ << '\n';
}

void AcousticParametersUi::OnFilter(wxCommandEvent & event)
{
    std::cout << __func__ << '\n';
}

// This handles the whole radio group
void AcousticParametersUi::OnNoiseReductionChoice( wxCommandEvent & WXUNUSED(event))
{
    std::cout << __func__ << '\n';
}


//------------------------------------------------------------------------------------

void AcousticParametersUi::Populate()
{
    std::cout << __func__ << '\n';
    SetTitle(AcousticParametersTitle);
    

    
    //===================================================================
    // Draw Interface
    
    ShuttleGui S(this, eIsCreating);
    
    mAuroraLogo = LoadPngBitmap(
                                Aurora_logo_png,
                                sizeof(Aurora_logo_png)
                                );
    
    mAcousticParametersLogo = LoadPngBitmap(
                                            ap_logo_png,
                                            sizeof(ap_logo_png)
                                            );
    
    //===================================================================
    // Unfiltered Impulse Response
    
    S.SetBorder(0);
    
    S.StartVerticalLay(0);
    {
        S.StartHorizontalLay(wxEXPAND,1);
        {
            
            S.AddWindow(safenew wxStaticBitmap(
                                               S.GetParent(),
                                               wxID_ANY,
                                               mAuroraLogo));
            S.GetSizer()->AddStretchSpacer();
            S.AddWindow(safenew wxStaticBitmap(
                                               S.GetParent(),
                                               wxID_ANY,
                                               mAcousticParametersLogo));
        }
        S.EndHorizontalLay();
        
        S.AddSpace(5);
        
    }
    S.EndVerticalLay();
    
    
    S.SetSizerProportion(1);
    
    
    
    S.StartStatic(XO(""),1);
    {
        S.StartMultiColumn(2, wxEXPAND);
        {
            S.SetStretchyCol(0);
            S.SetStretchyRow(0);
            
            S.StartStatic(XO("Unfiltered Impulse Response"),2);
            {
                mPlot = safenew AuroraPlot(S.GetParent(), wxID_ANY);
                S.Prop(1).Position(wxEXPAND).MinSize({ wxDefaultCoord, 250 }).AddWindow(mPlot);
            }
            S.EndStatic();
            
            S.StartHorizontalLay(wxEXPAND, 0);
            {
                S.StartVerticalLay(0);
                {
                    mSetupButton  = S.Id(SetupButtonID).Position(wxEXPAND).AddButton(XO("Setup"));
                    S.AddSpace(5);
                    mSaveRButton  = S.Id(SaveRButtonID).Position(wxEXPAND).AddButton(XO("Save Results to File"));
                    S.AddSpace(5);
                    mCopyRButton  = S.Id(CopyRButtonID).Position(wxEXPAND).AddButton(XO("Copy Results to clipboard"));
                    S.AddSpace(5);
                    mStoreGButton = S.Id(StoreGButtonID).Position(wxEXPAND).AddButton(XO("Store G Reference Signal"));
                    S.AddSpace(5);
                    
                    S.StartStatic(XO("Mic / Probe type:"),0);
                    {
                        S.AddFixedText(XO("Binaural pair"));
                    }
                    S.EndStatic();
                    
                    S.StartStatic(XO("Channels:"),0);
                    {
                        S.AddListControlReportMode({{ XO("Channel"), wxLIST_FORMAT_RIGHT }});
                    }
                    S.EndStatic();
                    
                    S.StartStatic(XO("Tuser limits:"),0);
                    {
                        S.AddFixedText(XO("(-15.0 dB, -5.0 dB)"));
                    }
                    S.EndStatic();
                }
                S.EndVerticalLay();
            }
            S.EndHorizontalLay();
            
        }
        S.EndMultiColumn();
    }
    S.EndStatic();
    
    S.StartHorizontalLay(wxEXPAND, 1);
    {
        mResultsGrid = S.Prop(1).Position(wxEXPAND).AddGrid();
    }
    S.EndHorizontalLay();
    
    //===================================================================
    // Analyse Selected Tracks
    
    auto& tracks = TrackList::Get(*mProject);
    auto& parameterTracks = mAcousticalParameters.Tracks();
    const auto projectRate = ProjectRate(*mProject).GetRate();
    
    std::vector<AuroraPlot::PlotData> rmsTraces;
    for (auto&& track : tracks.Selected<WaveTrack>())
    {
        std::cout << "track->GetName(): " << track->GetName() << '\n';
        parameterTracks.emplace_back(Aurora::AcParametersAudioTrack(track->GetVisibleSampleCount().as_size_t(), projectRate));
        
        
        auto& audioAnalysisTrack = parameterTracks.back();
        float* analysisBuffers[] = { audioAnalysisTrack.Samples() };
        track->GetFloats(0, 1, analysisBuffers,
                         0, track->GetVisibleSampleCount().as_size_t());
        
        
        std::cout << "track->GetEndTime(): " <<track->GetEndTime()<< '\n';
        
        std::vector<float> audio(audioAnalysisTrack.Samples(),
                             audioAnalysisTrack.Samples() + track->GetVisibleSampleCount().as_size_t());
        rmsTraces.push_back(RMS(audio, 250,
                                0.0, track->GetEndTime()));
    }
    
    std::cout << "rmsTraces: " << rmsTraces.size() <<'\n';
    
    std::cout << "mPlot->GetSize().y:" << mPlot->GetSize().y << '\n';
    
    for (auto t : rmsTraces[0].y) {
        std::cout << "y: " << t << '\n';
    }
    
    mAcousticalParameters.Init();
    
    //     Then process parameterTracks
    mAcousticalParameters.CalculateAcousticParameters();
    const auto& result = mAcousticalParameters.Results(0);
    const auto& fcbs = result.Frequencies();
    //===================================================================
    // Levels should first be RMS of the audio
    // then they should be the shroeder decay
    
    AuroraPlot::PlotData plotData{20};
    
    for (int i = 0; i < 20; i++) {
        plotData[i] = {double(i)*0.3,-2.0*double(i)};
    }
    rmsTraces[0].legendTitle = "RMS 1";
    
//    mPlot->SetData({plotData.x, plotData.y, "Ch-1"});
    mPlot->SetData(rmsTraces[0]);
    
//    auto maxNum = *std::max_element(rmsTraces[0].x.begin(),rmsTraces[0].x.end());
    
//    std::cout << "Max: " << maxNum << '\n';
    
    int numColumns = int(result.Frequencies().size());
    int numRows    = int(result.Parameters().size());
    
    mResultsGrid->CreateGrid(numRows, numColumns);
    
    for (int r = 0; r < numRows; ++r)
        mResultsGrid->SetRowLabelValue(r, result.Parameters()[r]);
    
    for (int c = 0; c < numColumns; ++c)
        mResultsGrid->SetColLabelValue(c, wxString::Format(wxT("%.1f"), result.Frequencies()[c]));
    
    for (int r = 0; r < numRows; ++r)
        for (int c = 0; c < numColumns; ++c)
            mResultsGrid->SetCellValue(r, c,
                                       wxString::Format(wxT("%.1f"),
                                                        result.Get(result.Parameters ()[r],
                                                                   result.Frequencies()[c])));
    
    mResultsGrid->EnableEditing(false);
    
    // Force the grid to request enough space
    wxSize gridSize(mResultsGrid->GetRowLabelSize(),
                    mResultsGrid->GetColLabelSize());
    
    for (int c = 0; c < mResultsGrid->GetNumberCols(); ++c)
        gridSize.x += mResultsGrid->GetColSize(c);
    
    for (int r = 0; r < mResultsGrid->GetNumberRows(); ++r)
        gridSize.y += mResultsGrid->GetRowSize(r);
    
    mResultsGrid->SetMinSize(gridSize);
    //===================================================================
    // Add Close Button
    S.StartVerticalLay(0);
    {
        S.AddSpace(5);
        S.StartHorizontalLay(wxALIGN_RIGHT | wxALL, 0);
        {
            mFilterButton = S.Id(FilterButtonID).AddButton(XO("Filter"));
            mCloseButton  = S.Id(CloseButtonID).AddButton(XO("Close"));
        }
        S.EndHorizontalLay();
        S.AddSpace(5);
    }
    S.EndVerticalLay();
    //===================================================================
    
    Layout();
    wxSize size = GetSize();
    size.SetWidth(900);   // or whatever width you want
    SetMinSize(size);
    SetSize(size);
    Fit();
    Center(); // Bug 1607:
    
}

void AcousticParametersUi::UpdatePrefs()
{
    std::cout << __func__ << '\n';
}

//------------------------------------------------------------------------------------
#pragma mark - window events

void AcousticParametersUi::OnCloseWindow(wxCloseEvent &WXUNUSED(event))
{
    std::cout << __func__ << '\n';
    Show(false);
}

void AcousticParametersUi::OnSize(wxSizeEvent & WXUNUSED(event))
{
    Layout();
    
    //    mPlot->Refresh();
    //    mResultsGrid->Refresh();
    
    Refresh(true);
}

void AcousticParametersUi::OnCloseButton(wxCommandEvent &WXUNUSED(event))
{
    std::cout << __func__ << '\n';
    Show(false);
}

//------------------------------------------------------------------------------------
AuroraPlot::PlotData AcousticParametersUi::RMS(std::vector<float> audioVector,
                               const size_t unWindowWidth,
                               double lo,
                               double hi)
{
    AuroraPlot::PlotData rmsPlotData{unWindowWidth};
    
    const auto projectRate = ProjectRate(*mProject).GetRate();
    //Samples per pixel
    size_t samplesPerPixel = std::floor( ((hi - lo) * projectRate) /  double(unWindowWidth) );
    
    std::cout << "samplesPerPixel: " << samplesPerPixel << '\n';
    
    for(size_t k = 0; k < unWindowWidth; k++)
    {
        // RMS on 1 ms calculation
        double rms  = 0.0;
        size_t t0 = k*samplesPerPixel + lo * projectRate;
        size_t t1 = t0 + samplesPerPixel;
        
        for(size_t i = t0; i < t1; i++)
        {
            if(i < audioVector.size())
            {
                rms += audioVector[i] * audioVector[i];
            }
        }
        rms /= double(t1 - t0);
        
        rmsPlotData[k] = {float(k * samplesPerPixel)/projectRate, dB(rms)};
    }
    return rmsPlotData;
}

AuroraPlot::PlotData AcousticParametersUi::Decimate(std::vector<float> audioVector,
                                    const size_t unWindowLength,
                                    double& lo,
                                    double& hi)
{
    AuroraPlot::PlotData decimatedPlotData{unWindowLength};
    //Samples that come in a single pixel
    const auto projectRate = ProjectRate(*mProject).GetRate();
    const double dbCorrection = 10.0 * std::log10(projectRate / 100.0);
    
    size_t samplesPerPixel = std::floor( ((hi - lo) * projectRate) / unWindowLength );
    
    for(size_t k = 0; k < unWindowLength; k++)
    {
        size_t i = k*samplesPerPixel;
        decimatedPlotData[k] = {float(k * samplesPerPixel)/projectRate,
            (i > audioVector.size() ? 0.0 : dB(audioVector[i]) - dbCorrection
                                  + 120.0)};
    }
    
    return decimatedPlotData;
}
//------------------------------------------------------------------------------------
#pragma mark - Audcaity Menu Registration
// Bumf to hook-in to Audacity

// Remaining code hooks this add-on into the application
#include "CommandContext.h"
#include "CommandManager.h"
#include "ProjectWindows.h"

namespace
{
AttachedWindows::RegisteredFactory sAcousticParametersUiWindowKey{
    [](AudacityProject &parent) -> wxWeakRef<wxWindow>
    {
        auto &window = ProjectWindow::Get(parent);
        return safenew AcousticParametersUi(
                                            &window, -1, parent, AcousticParametersTitle,
                                            wxPoint{150, 150});
    }};

void OnOpenWindow(const CommandContext &context)
{
    std::cout << __func__ << '\n';
    auto &project = context.project;
    CommandManager::Get(project).RegisterLastAnalyzer(context);
    auto newWindow = &GetAttachedWindows(project)
        .Get<AcousticParametersUi>(sAcousticParametersUiWindowKey);
    
    newWindow->Show(true);
    newWindow->Raise();
    newWindow->SetFocus();
}


using namespace MenuRegistry;
AttachedItem sAttachment{
    Command(wxT("AcousticParameters"),
            XXO("Aurora Acoustic Parameters..."),
            OnOpenWindow,
            AudioIONotBusyFlag() | WaveTracksSelectedFlag() | TimeSelectedFlag(),
            wxT("Ctrl+Shift+T") ),
    wxT("Generate/Generators"),
    //    wxT("Analyze/Analyzers/Windows"),
};
}

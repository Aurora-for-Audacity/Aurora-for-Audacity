#include "AcousticParametersUi.h"
#include "widgets/valnum.h"
#include "../res/aurora_logos.h"
#include "../mod_aurora.h"

#define DummyAnalysisTitle XO("Acoustic Parameter Analysis")

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
    0.0, 0.01, 0.02, 0.03,
};

std::vector<double> levels =
{
    85.0, 82.5, 78.0, 74.2,
};

BEGIN_EVENT_TABLE(AcousticParametersUi, wxDialogWrapper)
EVT_CLOSE(AcousticParametersUi::OnCloseWindow)
EVT_BUTTON(wxID_CANCEL, AcousticParametersUi::OnCloseButton)
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

void AcousticParametersUi::Populate()
{
    std::cout << __func__ << '\n';
    SetTitle(DummyAnalysisTitle);
    
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
        
    }
    S.EndVerticalLay();
    
    
    S.SetSizerProportion(1);
    
    S.StartMultiColumn(2, wxEXPAND);
    {
        S.SetStretchyCol(0);
        S.SetStretchyRow(0);
        
        mPlot = safenew AuroraPlot(S.GetParent(), wxID_ANY);
        S.Prop(1).Position(wxEXPAND).MinSize({ wxDefaultCoord, 250 }).AddWindow(mPlot);
        
        S.StartHorizontalLay(wxEXPAND, 0);
        {
            S.StartVerticalLay(1);
            {
                mSetupButton  = S.AddButton(XO("Setup"));
                mSaveRButton  = S.AddButton(XO("Save Results to File"));
                mCopyRButton  = S.AddButton(XO("Copy Results to clipboard"));
                mStoreGButton = S.AddButton(XO("Store G Reference Signal"));
                
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
    
    S.StartHorizontalLay(wxEXPAND, 1);
    {
        mResultsGrid = S.Prop(1).Position(wxEXPAND).AddGrid();
    }
    S.EndHorizontalLay();
    
    //===================================================================
    mPlot->SetData(times, levels);
    mPlot->Refresh();
    
    mResultsGrid->CreateGrid(17, 12);
    
    for (size_t r = 0; r < rowLabels.size(); ++r)
        mResultsGrid->SetRowLabelValue(r, rowLabels[r]);
    
    for (size_t c = 0; c < columnLabels.size(); ++c)
        mResultsGrid->SetColLabelValue(c, columnLabels[c]);
    
    for (int r = 0; r < 17; ++r)
        for (int c = 0; c < 12; ++c)
            mResultsGrid->SetCellValue(r, c, "0");
    
    mResultsGrid->EnableEditing(false);
    
    
    // Force the grid to request enough space
    wxSize gridSize(
                    mResultsGrid->GetRowLabelSize(),
                    mResultsGrid->GetColLabelSize()
                    );
    
    for (int c = 0; c < mResultsGrid->GetNumberCols(); ++c)
        gridSize.x += mResultsGrid->GetColSize(c);
    
    for (int r = 0; r < mResultsGrid->GetNumberRows(); ++r)
        gridSize.y += mResultsGrid->GetRowSize(r);
    
    mResultsGrid->SetMinSize(gridSize);
    //===================================================================
    // Add Close Button
    S.AddStandardButtons(eCloseButton);
    mCloseButton = static_cast<wxButton *>(FindWindowById(wxID_CANCEL));
    mCloseButton->SetDefault();
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

// This handles the whole radio group
void AcousticParametersUi::OnNoiseReductionChoice( wxCommandEvent & WXUNUSED(event))
{
    
}

void AcousticParametersUi::OnCloseWindow(wxCloseEvent &WXUNUSED(event))
{
    std::cout << __func__ << '\n';
    Show(false);
}

void AcousticParametersUi::OnCloseButton(wxCommandEvent &event)
{
    std::cout << __func__ << '\n';
    Show(false);
}

//------------------------------------------------------------------------------------
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
                                            &window, -1, parent, DummyAnalysisTitle,
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
//------------------------------------------------------------------------------------

#include "AcousticParametersUi.h"
#include "widgets/valnum.h"
#include "../res/aurora_logos.h"
#include "../mod_aurora.h"
#include "Theme.h"
#include "AllThemeResources.h"

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
    mPlot->SetData(times, levels);
//    mPlot->Refresh();
    
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

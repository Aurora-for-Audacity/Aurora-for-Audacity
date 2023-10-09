#include "DummyAnalyse.h"

#define DummyAnalysisTitle XO("Dummy Analysis")

//------------------------------------------------------------------------------------
// Hook up event handles

BEGIN_EVENT_TABLE(DummyAnalyse, wxDialogWrapper)
    EVT_CLOSE(DummyAnalyse::OnCloseWindow)
    EVT_BUTTON(wxID_CANCEL, DummyAnalyse::OnCloseButton)
END_EVENT_TABLE()

//------------------------------------------------------------------------------------

DummyAnalyse::DummyAnalyse(wxWindow *parent, wxWindowID id,
                           AudacityProject &project,
                           const TranslatableString &title,
                           const wxPoint &pos)
    : wxDialogWrapper(parent, id, title, pos, wxDefaultSize,
                      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX),
      mProject{&project}
{
    std::cout << __func__<< '\n';
    SetName();

    Populate();
}

DummyAnalyse::~DummyAnalyse()
{
    std::cout << __func__<< '\n';
}

bool DummyAnalyse::Show(bool show)
{
    std::cout << __func__<< '\n';
   bool res = wxDialogWrapper::Show(show);

   return res;
}


//------------------------------------------------------------------------------------

void DummyAnalyse::Populate()
{
    std::cout << __func__<< '\n';
    SetTitle(DummyAnalysisTitle);

    ShuttleGui S(this, eIsCreating);
    
    //===================================================================
    // Add Close Button
    S.AddStandardButtons(eCloseButton);
    mCloseButton = static_cast<wxButton*>(FindWindowById( wxID_CANCEL ));
    mCloseButton->SetDefault();
    //===================================================================
    
    Layout();
    Fit();
    Center(); // Bug 1607:
    
    SetMinSize(GetSize());
}

void DummyAnalyse::UpdatePrefs()
{
    std::cout << __func__<< '\n';
}

void DummyAnalyse::OnCloseWindow(wxCloseEvent & WXUNUSED(event))
{
    std::cout << __func__<< '\n';
   Show(false);
}

void DummyAnalyse::OnCloseButton(wxCommandEvent & event)
{
    std::cout << __func__<< '\n';
    Show(false);
}

//------------------------------------------------------------------------------------
// Bumf to hook-in to Audacity

#include "commands/CommandContext.h"
#include "commands/CommandManager.h"
#include "ProjectWindows.h"

namespace
{
    AttachedWindows::RegisteredFactory sDummyAnalyseWindowKey{
        [](AudacityProject &parent) -> wxWeakRef<wxWindow>
        {
            auto &window = ProjectWindow::Get(parent);
            return safenew DummyAnalyse(
                &window, -1, parent, DummyAnalysisTitle,
                wxPoint{150, 150});
        }};

    void OnOpenWindow(const CommandContext &context)
    {
        std::cout << __func__<< '\n';
        auto &project = context.project;
        CommandManager::Get(project).RegisterLastAnalyzer(context);  //Register Plot Spectrum as Last Analyzer
        auto newWindow = &GetAttachedWindows(project)
           .Get< DummyAnalyse >( sDummyAnalyseWindowKey );

        if( VetoDialogHook::Call( newWindow ) )
           return;
        newWindow->Show(true);
        newWindow->Raise();
        newWindow->SetFocus();
        
    }

    using namespace MenuTable;
    AttachedItem sAttachment{wxT("Analyze/Analyzers/Windows"),
                             Command(wxT("DummyAnalyse"), XXO("Dummy Analyse..."),
                                     OnOpenWindow,
                                     AudioIONotBusyFlag() | WaveTracksSelectedFlag() | TimeSelectedFlag())};

}
//------------------------------------------------------------------------------------

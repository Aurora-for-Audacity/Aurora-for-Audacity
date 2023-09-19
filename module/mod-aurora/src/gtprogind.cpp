// Progress Indicator (code by Guido Truffelli)

#include <aurora.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/evtloop.h>

///////////////////////////////////////////////////////////
// GTProgressIndicator
///////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(GTProgressIndicator, wxDialog)
    EVT_BUTTON(wxID_CANCEL, GTProgressIndicator::OnCancel)
END_EVENT_TABLE()

void GTProgressIndicator::OnCancel(wxCommandEvent& event)
{
    if ( m_state == State::kFinished )
    {
        // this means that the count down is already finished and we're being
        // shown as a modal dialog - so just let the default handler do the job
        event.Skip();
    }
    else
    {
        // request to cancel was received, the next time Update() is called we
        // will handle it
        m_state = State::kCanceled;

        // save the time when the dialog was stopped
//        m_timeStop = wxGetCurrentTime();
    }
}

wxString GTProgressIndicator::msec2str(long ms) const
{
    int s = (int)ms/1000;
    const int h = s / 3600;
    s -= h*3600;
    const int m = s / 60;
    s -= m*60;
    return wxString::Format("%02d:%02d:%02d", h, m, s);
}

void GTProgressIndicator::SetRange(int nRange, int nGauge)
{
    wxASSERT(nRange >= 0 && nGauge >= 0 && nGauge < MAX_GAUGES);
    m_pGauge[nGauge]->SetRange(nRange);
}

bool GTProgressIndicator::SetProgress(int nPos, int nGauge)
{
    if (!IsShown())
    {
        return false;
    }
    
    if (! DoBeforeUpdate())
    {
        return false;
    }
    wxASSERT(nPos >= 0 && nGauge >= 0 && nGauge < MAX_GAUGES);
    m_pGauge[nGauge]->SetValue(nPos);

    const long elap = Elapsed();
    
  //  if (elap > m_nNextUpdate)
    {
        m_nNextUpdate = elap+CLOCKS_PER_SEC;
        m_pPassed->SetLabel(msec2str(elap));

        wxLongLong_t nTotValue = 0, nTotRange = 0;
        
        if (m_bGaugesIndependent && m_nGauges > 1)
        {
            for (int i = 0; i < MAX_GAUGES && m_pGauge[i]; i++)
            {
                const int nValue = m_pGauge[i]->GetValue();
                const int nRange = m_pGauge[i]->GetRange();
                
                if (nValue < nRange) // Escludi dal calcolo i processi gi� terminati
                {
                    nTotValue += nValue;
                    nTotRange += nRange;
                }
            }
        }
        else
        {
            nTotValue += m_pGauge[0]->GetValue();
            nTotRange += m_pGauge[0]->GetRange();
        }
        
        if (nTotValue > 0)
        {
            const long est = long(elap * nTotRange / nTotValue);
            m_pEstimated->SetLabel(msec2str(est));
            m_pResidual->SetLabel(msec2str(est-elap));
        }

        if (nTotValue == nTotRange)
        {
            if (m_state == State::kFinished)
            {
                // ignore multiple calls to Update(m_maximum): it may sometimes be
                // troublesome to ensure that Update() is not called twice with the
                // same value (e.g. because of the rounding errors) and if we don't
                // return now we're going to generate asserts below
                return true;
            }
            m_state = State::kFinished;
            
            // auto-hide
            Hide();
        }
        else
        {
            // not at maximum yet
            DoAfterUpdate();
        }
        
        wxYield();
        // wxDialog::Update();
    }

    return m_state != State::kCanceled;
}

void GTProgressIndicator::SetMessage(const wxString& msg)
{
    m_pMessage->SetLabel(msg);
}

void GTProgressIndicator::EnsureActiveEventLoopExists()
{
    if( ! wxEventLoopBase::GetActive())
    {
        m_tempEventLoop = new wxEventLoop;
        wxEventLoop::SetActive(m_tempEventLoop);
    }
}

void GTProgressIndicator::DisableOtherWindows()
{
    /*
    if ( HasPDFlag(wxPD_APP_MODAL) )
    {
        m_winDisabler = new wxWindowDisabler(this);
    }
    else
    {
        if ( m_parentTop )
            m_parentTop->Disable();
        m_winDisabler = NULL;
    }*/
}

void GTProgressIndicator::ReenableOtherWindows()
{
    /*
    if ( HasPDFlag(wxPD_APP_MODAL) )
    {
        wxDELETE(m_winDisabler);
    }
    else
    {
        if ( m_parentTop )
            m_parentTop->Enable();
    }*/
}

bool GTProgressIndicator::DoBeforeUpdate(bool* skip)
{
    // we have to yield because not only we want to update the display but
    // also to process the clicks on the cancel and skip buttons
    // NOTE: using YieldFor() this call shouldn't give re-entrancy problems
    //       for event handlers not interested to UI/user-input events.
    wxEventLoopBase::GetActive()->YieldFor(wxEVT_CATEGORY_UI|wxEVT_CATEGORY_USER_INPUT);

    Update();
/* not skippable....
    if ( m_skip && skip && !*skip )
    {
        *skip = true;
        m_skip = false;
        EnableSkip();
    }
*/
    return m_state != State::kCanceled;
}

void GTProgressIndicator::DoAfterUpdate()
{
    // allow the window to repaint:
    // NOTE: since we yield only for UI events with this call, there
    //       should be no side-effects
    wxEventLoopBase::GetActive()->YieldFor(wxEVT_CATEGORY_UI);
}

void GTProgressIndicator::Init(const wxString& msg,
                               const int nGauges,
                               const bool bCancellable,
                               const bool bIndependent)
{
    // We need a running event loop in order to update the dialog and be able
    // to process clicks on its buttons, so ensure that there is one running
    // even if this means we have to start it ourselves (this happens most
    // commonly during the program initialization, e.g. for the progress
    // dialogs shown from overridden wxApp::OnInit()).
    EnsureActiveEventLoopExists();

    m_bGaugesIndependent = bIndependent;
    
    const int nGap = 4;
    wxBoxSizer* pTopSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(pTopSizer);

    m_pMessage = new wxStaticText(this, wxID_ANY, msg);
    pTopSizer->Add(m_pMessage, 0, wxALL | wxEXPAND, 5);
    
    memset(m_pGauge, 0, sizeof(m_pGauge));
    m_nGauges = nGauges;
    
    for (int i = 0; i < m_nGauges; i++)
    {
        m_pGauge[i] = new wxGauge(this, 1001+i, 100, wxDefaultPosition, wxSize(400, -1));
        pTopSizer->Add(m_pGauge[i], 0, wxALL, nGap);
    }

    wxGridSizer* pTimersSizer = new wxGridSizer(1, 3, nGap, nGap);
    pTopSizer->Add(pTimersSizer, 0, wxEXPAND);

    wxStaticBoxSizer* pPassed = new wxStaticBoxSizer(wxVERTICAL,
                                                   this,
                                                   _("Elapsed Time"));
    pTimersSizer->Add(pPassed, 0, wxEXPAND);
    m_pPassed = new wxStaticText(this, 1101, wxT("00:00:00"));
    pPassed->Add(m_pPassed, 0, wxALL|wxALIGN_CENTER, 0);

    wxStaticBoxSizer* pResidual = new wxStaticBoxSizer(wxVERTICAL,
                                                     this,
                                                     _("Residual Time"));
    pTimersSizer->Add(pResidual, 0, wxEXPAND);
    m_pResidual = new wxStaticText(this, 1102, wxT("00:00:00"));
    pResidual->Add(m_pResidual, 0, wxALL|wxALIGN_CENTER, 0);

    wxStaticBoxSizer* pEstimated = new wxStaticBoxSizer(wxVERTICAL,
                                                      this,
                                                      _("Estimated Time"));
    pTimersSizer->Add(pEstimated, 0, wxEXPAND);
    m_pEstimated = new wxStaticText(this, 1103, wxT("00:00:00"));
    pEstimated->Add(m_pEstimated, 0, wxALL|wxALIGN_CENTER, 0);

    wxButton* pCancel = NULL;

    if (bCancellable)
    {
        pCancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
        pTopSizer->Add(pCancel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, nGap);
    }

    pTopSizer->SetSizeHints(this);

    //SetMessage(msg);

    Show();
    Enable();
    Raise();
    wxDialog::Update();

    m_nNextUpdate = 0;
    m_chrono.Start();
}

GTProgressIndicator::GTProgressIndicator(wxWindow* parent,
                                         const size_t nRange,
                                         const wxString& caption,
                                         const wxString& msg,
                                         bool bCancellable)
  : wxDialog(parent, wxID_ANY, caption)
{
    Init(msg, 1, bCancellable, true);
    SetRange((int)nRange, 0);
}

GTProgressIndicator::GTProgressIndicator(wxWindow* parent,
                                         const wxString& caption,
                                         const wxString& msg,
                                         const bool bCancellable,
                                         const int nGauges,
                                         const bool bGaugesIndependent)
  : wxDialog(parent, wxID_ANY, caption)
{
    Init(msg, nGauges, bCancellable, bGaugesIndependent);
}

GTProgressIndicator::~GTProgressIndicator()
{
    // takes 'as is' from wxGenericProgressDialog
    
    if ( m_tempEventLoop )
    {
        // If another event loop has been installed as active during the life
        // time of this object, we shouldn't deactivate it, but we also can't
        // delete our m_tempEventLoop in this case because it risks leaving the
        // new event loop with a dangling pointer, which it will set back as
        // the active loop when it exits, resulting in a crash. So we have no
        // choice but to just leak this pointer then, which is, of course, bad
        // and usually easily avoidable by just destroying the progress dialog
        // sooner, so warn the programmer about it.
        wxCHECK_RET
        (
            wxEventLoopBase::GetActive() == m_tempEventLoop,
            "current event loop must not be changed during "
            "wxGenericProgressDialog lifetime"
        );

        wxEventLoopBase::SetActive(nullptr);
        delete m_tempEventLoop;
    }
}


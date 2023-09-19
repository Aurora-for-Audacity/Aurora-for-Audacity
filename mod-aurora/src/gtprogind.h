#ifndef __PROGIND_H__
#define __PROGIND_H__

#include <wxcmn.h>

#include <wx/stattext.h>
#include <wx/stopwatch.h>

///////////////////////////////////////////////////////////
// GTProgressIndicator
///////////////////////////////////////////////////////////

class GTProgressIndicator : public wxDialog
{
  enum { MAX_GAUGES = 16 };
    
    wxStaticText* m_pMessage;
    
    wxGauge* m_pGauge[MAX_GAUGES];
    wxStaticText* m_pPassed;
    wxStaticText* m_pResidual;
    wxStaticText* m_pEstimated;
    wxStopWatch m_chrono;
    long m_nNextUpdate; // TBR
    
    int  m_nGauges = 0;
    bool m_bGaugesIndependent = true;
    
    wxEventLoop*      m_tempEventLoop = nullptr;
    wxWindowDisabler* m_winDisabler   = nullptr;
    
    bool m_skip  = false;
    
    enum State
    {
        kUncancelable = -1,   // dialog can't be canceled
        kCanceled,            // can be cancelled and, in fact, was
        kContinue,            // can be cancelled but wasn't
        kFinished,            // finished, waiting to be removed from screen
        kDismissed            // was closed by user after finishing
    };
    State  m_state = kUncancelable;

protected:
  DECLARE_EVENT_TABLE();
    
    void OnCancel(wxCommandEvent& evt);

    /** Taken 'as is' from wxGenericProgressDialog: a progress dialog works
     *  sending end receiving events to/from an event loop, otherwise is
     *  dead.
     */
    void EnsureActiveEventLoopExists();
    
    /** Taken 'as is' from wxGenericProgressDialog: disable all other
     *  windows.
     */
    void DisableOtherWindows();

    /** Taken 'as is' from wxGenericProgressDialog: enable all other
     *  windows when this class is destroyed.
     */
    void ReenableOtherWindows();
    
    /** Taken 'as is' from wxGenericProgressDialog: we have to yield because
     * not only we want to update the display but also to process the clicks
     * on the cancel and skip buttons.
     */
    bool DoBeforeUpdate(bool* skip = nullptr);

    /** Taken 'as is' from wxGenericProgressDialog: allow the window to
     * repaint.
     */
    void DoAfterUpdate();
    
    
  void Init(const wxString& msg,
            const int nGauges,
            const bool bCancellable,
            const bool bIndependent);
    
  wxString msec2str(long ms) const;
  long Elapsed() const { return m_chrono.Time(); }

public:
  void SetRange   (int nRange, int nGauge = 0);
  bool SetProgress(int nPos,   int nGauge = 0);
  void SetMessage(const wxString& msg);

    /** @brief Instantiate a multi gauge progress indicator.
     *  @param caption Message shown on caption
     *  @param msg Message shown on window
     *  @param bCancellable Progress interruptible or not
     *  @param nGauges Number of gauges
     *  @param bGaugesIndependent \c true if all the gauges are independent, so
     * the overall time is the sum of all progresses, false if there is a main
     * gauge (the first one) and the overall time depends only by its progress.
     */
    GTProgressIndicator(wxWindow* parent,
                        const wxString& caption,
                        const wxString& msg,
                        const bool bCancellable,
                        const int  nGauges = 1,
                        const bool bGaugesIndependent = true);
    
    GTProgressIndicator(wxWindow* parent,
                        const size_t    nRange,
                        const wxString& caption,
                        const wxString& msg,
                        bool bCancellable);
    
    virtual ~GTProgressIndicator();
};

#endif

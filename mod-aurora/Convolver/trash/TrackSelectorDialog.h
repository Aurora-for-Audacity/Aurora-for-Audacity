/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  TrackSelectorDialog.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_CONVOLVER_GUI_H__
#define __AURORA_CONVOLVER_GUI_H__

namespace Aurora
{
    //----------------------------------------------------------------------------
    // TrackDropSource
    //----------------------------------------------------------------------------
    class TrackDropSource : public wxDropSource
    {
      public:
        bool GiveFeedback(wxDragResult effect);

        TrackDropSource(wxTextDataObject& data, wxListCtrl *owner);
    };

    //----------------------------------------------------------------------------
    // TrackDropTarget
    //----------------------------------------------------------------------------
    class TrackDropTarget : public wxTextDropTarget
    {
    protected:   
        wxListCtrl* mOwner;
        
    public:
        virtual bool OnDropText(wxCoord x, 
                                wxCoord y, 
                                const wxString& data);

        TrackDropTarget(wxListCtrl *owner) : mOwner(owner) { }
    };
    
    //----------------------------------------------------------------------------
    // TrackSelectorDialog
    //----------------------------------------------------------------------------

    class TrackSelectorDialog : public TrackSelectorDlg
    {
    private:
        enum Direction { D_UP, D_DOWN, D_LEFT, D_RIGHT };
        
        Convolver*       mAc;
        TrackProperties* mAtp;
        
    protected:
        // Handlers for TrackSelectorDlg events.
        void OnToIRsBtn( wxCommandEvent& event );
        void ToAudioDataBtn( wxCommandEvent& event );
        void OnMatrixModeCheck( wxCommandEvent& event );
        void OnOneForAllCheck( wxCommandEvent& event );
        void OnMoveUpBtn( wxCommandEvent& event );
        void OnMoveDownBtn( wxCommandEvent& event );
        void OnToRightBtn( wxCommandEvent& event );
        void OnToLeftBtn( wxCommandEvent& event );
        void OnRemoveBtn( wxCommandEvent& event );
        void OnHelp( wxCommandEvent& event );
        void OnOK( wxCommandEvent& event );
        void OnCancel( wxCommandEvent& event );
        
        void InitDrag(wxListCtrl* lst);
        void OnAvailableDragInit(wxListEvent& event);
        void OnAudioDataDragInit(wxListEvent& event);
        void OnFilterDragInit(wxListEvent& event);
        
        void PopulateAvailableTracksList();
        bool MoveItemsBetweenLists(wxListCtrl* src, wxListCtrl* dest);
        bool MoveUpListItem(wxListCtrl* lst);
        bool MoveDownListItem(wxListCtrl* lst);
        void ToggleMatrixModeCheck();
        void ToggleControls();
        
    public:    
        /** Constructor */
        TrackSelectorDialog( wxWindow* parent, 
                             Convolver *ac, 
                             TrackProperties *atp );
    };

    //----------------------------------------------------------------------------
    // ConvolverDialog
    //----------------------------------------------------------------------------
    class ConvolverDialog : public AFConvolverDlg
    {
      private:
        Convolver *mAc;
        
        double TextToDoublePtr(wxTextCtrl *txt);
        
    protected:
        // Handlers for AFConvolverDlg events.
        void OnOk( wxCommandEvent& event );
        void OnCancel( wxCommandEvent& event );
        void OnHelp( wxCommandEvent& event );
        void OnFullAutorangeCheck( wxCommandEvent& event );
        void OnFirstBlockAutorangeCheck( wxCommandEvent& event );
        void OnGainText( wxCommandEvent& event );
        void OnTimeReverseIrCheck( wxCommandEvent& event );
        void OnPreserveLengthCheck( wxCommandEvent& event );
        void OnCrosstalkCancelCheck( wxCommandEvent& event );
        
    public:    
        /** Constructor */
        ConvolverDialog( wxWindow* parent, Convolver *ac );
    };
} // namespace Aurora

#endif // __AURORA_CONVOLVER_GUI__

// Indentation settings for Vim and Emacs and unique identifier for Arch, a
// version control system. Please do not modify past this point.
//
// Local Variables:
// c-basic-offset: 3
// indent-tabs-mode: nil
// End:
//
// vim: et sts=3 sw=3
// arch-tag: c05d7383-e7cd-410e-b7b8-f45f47c9e283

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  gui.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_CONVOLVER_GUI_H__
#define __AURORA_CONVOLVER_GUI_H__

namespace Aurora
{
    class ConvolverEffect;

    //----------------------------------------------------------------------------
    // ConvolverTrackDropSource
    //----------------------------------------------------------------------------
    class ConvolverTrackDropSource : public wxDropSource
    {
       public:
          bool GiveFeedback(wxDragResult effect);

          ConvolverTrackDropSource(wxTextDataObject& data, wxListCtrl *owner);
    };

    //----------------------------------------------------------------------------
    // ConvolverTrackDropTarget
    //----------------------------------------------------------------------------
    class ConvolverTrackDropTarget : public wxTextDropTarget
    {
       protected:   
          wxListCtrl* mOwner;
          
       public:

         virtual bool OnDropText(wxCoord x, wxCoord y, 
                                 const wxString& data);

         ConvolverTrackDropTarget(wxListCtrl *owner);
    };
     
    //----------------------------------------------------------------------------
    // TrackSelectorDialog
    //----------------------------------------------------------------------------

    class ConvolverTrackSelectorDialog : public AFTrackSelectorDlg
    {
     private:
        enum Direction { D_UP, D_DOWN, D_LEFT, D_RIGHT };

        ConvolverEffect*          mAc  = nullptr;
        ConvolverTrackProperties* mAtp = nullptr;
        
     protected:
        // Handlers for AFTrackSelectorDlg events.
        void OnToIRsBtn( wxCommandEvent& event );
        void ToAudioDataBtn( wxCommandEvent& event );
        void OnMatrixModeCheck( wxCommandEvent& event );
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
        
        void CheckSetup();
        
      public:    
        /** Constructor */
        ConvolverTrackSelectorDialog( wxWindow* parent, 
                                     ConvolverEffect *ac,
                                     ConvolverTrackProperties *atp );
    };

    //----------------------------------------------------------------------------
    // AuroraConvolverDialog
    //----------------------------------------------------------------------------
    class ConvolverDialog : public AFConvolverDlg
    {
       private:
        ConvolverEffect *mAc = nullptr;
          
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
        ConvolverDialog( wxWindow* parent, ConvolverEffect *ac );
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

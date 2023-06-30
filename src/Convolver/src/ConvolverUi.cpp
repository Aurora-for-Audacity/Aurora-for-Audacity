/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Aurora Convolver

  gui.cpp

  Simone Campanini

*******************************************************************//**

\class Aurora::ConvolverDialog 
\brief Dialog used with EffectAurora::Convolver.

*//*******************************************************************/
#include <aurora.h>
#include <wx/dnd.h>

#include "ConvolverEffect.h"
#include "TrackProperties.h"

#include "ConvolverDialogs.h"
#include "ConvolverUi.h"

//----------------------------------------------------------------------------
// Aurora::ConvolverTrackDropSource implementation
//----------------------------------------------------------------------------
bool Aurora::ConvolverTrackDropSource::GiveFeedback(wxDragResult effect)
{
   //printf("I'm moving!\n");
   return true;
}

Aurora::ConvolverTrackDropSource::ConvolverTrackDropSource(wxTextDataObject& data, 
                                                           wxListCtrl *owner)
   : wxDropSource(data, owner)
{}

//----------------------------------------------------------------------------
// Aurora::ConvolverTrackDropTarget implementation
//----------------------------------------------------------------------------
bool Aurora::ConvolverTrackDropTarget::OnDropText(wxCoord x,
                                                  wxCoord y,
                                                  const wxString& data)
{
    // d'n'd appends data to the list
    // data contains a tab separated list of list labels
    wxString text = data;
    wxString textItem = text.BeforeFirst(TRAILING_SLASH_CH);

    //first field contains the number of items
    long count;
    textItem.ToLong(&count);

    while(count > 0)
    {
        text = text.AfterFirst(TRAILING_SLASH_CH);
        textItem = text.BeforeFirst(TRAILING_SLASH_CH);
        mOwner->InsertItem(mOwner->GetItemCount(), textItem);

        count--;
    }

    return true;
}

Aurora::ConvolverTrackDropTarget::ConvolverTrackDropTarget(wxListCtrl *owner)
   : mOwner(owner)
{}

//----------------------------------------------------------------------------
// Aurora::ConvolverTrackSelectorDialog implementation
//----------------------------------------------------------------------------
void Aurora::ConvolverTrackSelectorDialog::OnToIRsBtn( wxCommandEvent& event )
{
    MoveItemsBetweenLists(mAvailableList, mFilterList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::ToAudioDataBtn( wxCommandEvent& event )
{
   MoveItemsBetweenLists(mAvailableList, mAudioDataList);
   CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnMatrixModeCheck( wxCommandEvent& event )
{
    mAc->SetMatrixMode(event.IsChecked());
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnMoveUpBtn( wxCommandEvent& event )
{
    MoveUpListItem(mFilterList);
    MoveUpListItem(mAudioDataList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnMoveDownBtn( wxCommandEvent& event )
{
    MoveDownListItem(mFilterList);
    MoveDownListItem(mAudioDataList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnToRightBtn( wxCommandEvent& event )
{
    MoveItemsBetweenLists(mAudioDataList, mFilterList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnToLeftBtn( wxCommandEvent& event )
{
    MoveItemsBetweenLists(mFilterList, mAudioDataList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnRemoveBtn( wxCommandEvent& event )
{
    MoveItemsBetweenLists(mFilterList, mAvailableList);
    MoveItemsBetweenLists(mAudioDataList, mAvailableList);
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnHelp( wxCommandEvent& event )
{
   wxString path;
   path << "Docs" << TRAILING_SLASH << "lin_conv.html";
   wxFileName help_path(path);
   help_path.MakeAbsolute();

   ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::ConvolverTrackSelectorDialog::OnOK( wxCommandEvent& event )
{
   int filterItemsCount = mFilterList->GetItemCount();  

   if(!filterItemsCount)
   {
      Aurora::ConvolverEffect::MessageBox("No filter(s) selected.",
                                          Aurora::MessageType::Error);
      EndModal(false);
      return;
   }

    int  i,j;
   wxString text;
   Aurora::ConvolverTrackInfoItem *tinfo;
   
   for(i = 0; i < filterItemsCount; i++)
   {
      text = mFilterList->GetItemText(i);
      
      for(j = 0; j < mAtp->GetTrackInfoCount(); j++)
      {
         tinfo = mAtp->GetTrackInfoItem(j);

         if(text == tinfo->GetLabel())
         {
             tinfo->SetAsIr();
             tinfo->SetRow(i);
         }
      }
   }
   
   int adItemsCount = mAudioDataList->GetItemCount();
    
    if(!adItemsCount)
    {
        Aurora::ConvolverEffect::MessageBox("No audio data(s) selected.",
                                            Aurora::MessageType::Error);
        EndModal(false);
        return;
    }
   
   for(i = 0; i < adItemsCount; i++)
   {
      text = mAudioDataList->GetItemText(i);

      for(j = 0; j < mAtp->GetTrackInfoCount(); j++)
      {
         tinfo = mAtp->GetTrackInfoItem(j);

         if(text == tinfo->GetLabel())
         {
             tinfo->SetAsAudioData();
             tinfo->SetRow(i);
         }
      }
   }
   mAtp->SetAudioDataRowsCount(adItemsCount);
   mAtp->SetFilterRowsCount(filterItemsCount);
    
   EndModal(true);
}

void Aurora::ConvolverTrackSelectorDialog::OnCancel( wxCommandEvent& event )
{
   EndModal(false);
}

void Aurora::ConvolverTrackSelectorDialog::InitDrag(wxListCtrl* list)
{
    wxString text;
     
    // 1: find selected items, return false is there isn't
    text.Clear();
    int selectedItemsCount = list->GetSelectedItemCount();

    text << selectedItemsCount << TRAILING_SLASH;
    int idx = 0;
    std::vector<long> itemsList(selectedItemsCount, 0);

    long item = -1;
    for( ;; )
    {
       item = list->GetNextItem(item, wxLIST_NEXT_ALL,  wxLIST_STATE_SELECTED);
       
       if( item > -1 )
       {
          text << list->GetItemText(item) << TRAILING_SLASH;
          itemsList[idx++] = item;
       }
       else
       {
           break;
       }
    }

    // 2: build DropSource ad do drag'n'drop
    wxTextDataObject tdo(text);
    Aurora::ConvolverTrackDropSource tds(tdo, list);
    tds.DoDragDrop();
   
   // 3: delete items from source
    for (auto itr = itemsList.rbegin(); itr != itemsList.rend(); ++itr)
    {
        list->DeleteItem(*itr); // itemsList[idx]);
    }
    CheckSetup();
}

void Aurora::ConvolverTrackSelectorDialog::OnAvailableDragInit(wxListEvent& event)
{
   InitDrag(mAvailableList);
}

void Aurora::ConvolverTrackSelectorDialog::OnAudioDataDragInit(wxListEvent& event)
{
    InitDrag(mAudioDataList);
}

void Aurora::ConvolverTrackSelectorDialog::OnFilterDragInit(wxListEvent& event)
{
    InitDrag(mFilterList);
}


void Aurora::ConvolverTrackSelectorDialog::PopulateAvailableTracksList()
{
   int i = 0;   
   
   while(i < mAtp->GetTrackInfoCount())
   {
       auto ti = mAtp->GetTrackInfoItem(i);
       
       if(ti) 
       {
           mAvailableList->InsertItem(i, ti->GetLabel());          
       }
       i++;
   }

}

bool Aurora::ConvolverTrackSelectorDialog::MoveItemsBetweenLists(wxListCtrl *l_src,
                                                                 wxListCtrl *l_dest)
{
    // 1: find selected items, return false is there isn't
    const int selectedItemsCount = l_src->GetSelectedItemCount();
    
    if(selectedItemsCount <= 0)
    {
        CheckSetup();
        return false;
    }

    std::vector<long> itemsList;
    long item = -1;

    for( ;; )
    {
       item = l_src->GetNextItem(item, wxLIST_NEXT_ALL,  wxLIST_STATE_SELECTED);
       
       if( item == -1 )
       {
          break;
       }
       else
       {
           itemsList.push_back(item);
       }
       // this item is selected - do whatever is needed with it
    }

    // 2: copy items from source to dest
    int idx = 0;
    wxString text;
    int destItemCount = l_dest->GetItemCount();

    for(idx = 0; idx < selectedItemsCount; idx++)
    {
        text = l_src->GetItemText(itemsList[idx]);
        l_dest->InsertItem(destItemCount + idx, text);
    }
   
    // 3: delete items from source
    for(idx = (selectedItemsCount-1); idx >= 0; idx--)
    {
       l_src->DeleteItem(itemsList[idx]);
    }
    CheckSetup();
    return true;
}

bool Aurora::ConvolverTrackSelectorDialog::MoveUpListItem(wxListCtrl *lst)
{
    // 1: find selected item, return false is there isn't or if there
    //    are more than 1!
    const int selectedItemsCount = lst->GetSelectedItemCount();
    
    if(selectedItemsCount != 1)
    {
        CheckSetup();
        return false;
    }
   
    // 2: find selected item (if it is at the position '0', it cannot
    //    be moved up!)
    long item = lst->GetNextItem(-1, wxLIST_NEXT_ALL,  wxLIST_STATE_SELECTED);
    
    if(!item)
    {
        CheckSetup();
        return false;
    }
   
    // 3: copy selected item in the new place
    wxString text;
    text = lst->GetItemText(item);
    lst->InsertItem(item-1, text);

    // 4: delete old item
    lst->DeleteItem(item+1);

    CheckSetup();
    return true;
}

bool Aurora::ConvolverTrackSelectorDialog::MoveDownListItem(wxListCtrl *lst)
{
    // 1: find selected item, return false is there isn't or if there
    //    are more than 1!
    int selectedItemsCount = lst->GetSelectedItemCount();
    
    if(selectedItemsCount != 1)
    {
        CheckSetup();
        return false;
    }
   
   // 2: find selected item (if it is at the last position, it cannot 
   //    be moved up!)
   long item = lst->GetNextItem(-1, wxLIST_NEXT_ALL,  wxLIST_STATE_SELECTED);
   if(item == (lst->GetItemCount()-1)) return false;
   
   // 3: copy selected item in the new place
   wxString text;
   text = lst->GetItemText(item);
   lst->InsertItem(item+2, text);
   
   // 4: delete old item
   lst->DeleteItem(item);
   CheckSetup();
   return true;
}

void Aurora::ConvolverTrackSelectorDialog::CheckSetup()
{
    wxString msg;
    const int filterCount = mFilterList->GetItemCount();
    const int adCount     = mAudioDataList->GetItemCount();

    if (filterCount == 0)
    {
        msg = "No output yet. Please choose data and filter.";
        mAc->SetMatrixMode(false);
        mMatrixModeCheck->SetValue(false);
        mMatrixModeCheck->Disable();
        mOkBtn->Disable();
    }
    else if (filterCount == 1)
    {
        if (adCount > 0)
        {
            if (adCount == 1)
            {
                // one for all.
                msg.Printf("One track containing the convolution product\n"
                           "between data and filter.");
                mAc->SetMatrixMode(false);
                mMatrixModeCheck->SetValue(false);
                mMatrixModeCheck->Disable();
            }
            else
            {
                if (mMatrixModeCheck->IsChecked())
                {
                    msg.Printf("%d x 1 matrix convolution product on one output track.",
                                adCount);
                }
                else
                {
                    msg.Printf("%d tracks containing the convolution products\n"
                               "with the chosen filter", adCount);
                }
                mMatrixModeCheck->Enable();
            }
            mOkBtn->Enable();
        }
        else
        {
            msg = "No output yet. Please choose data.";
            mAc->SetMatrixMode(false);
            mMatrixModeCheck->SetValue(false);
            mMatrixModeCheck->Disable();
            mOkBtn->Disable();
        }
    }
    else
    {
        if (adCount > 0)
        {
            if (mMatrixModeCheck->IsChecked())
            {
                msg.Printf("%d x %d matrix convolution product on %d output tracks.",
                           adCount, filterCount, filterCount);
            }
            else
            {
                msg.Printf("%d tracks containing one-to-one convolution products.",
                           (adCount < filterCount ? adCount : filterCount));
            }
            mMatrixModeCheck->Enable();
            mOkBtn->Enable();
        }
        else
        {
            msg = "No output yet. Please choose data.";
            mMatrixModeCheck->Disable();
            mOkBtn->Disable();
        }
    }

    m_outputDescriptionLabel->SetLabel(msg);
}

/** Constructor */
Aurora::ConvolverTrackSelectorDialog::ConvolverTrackSelectorDialog( wxWindow* parent, 
                                                                   Aurora::ConvolverEffect *ac,
                                                                   Aurora::ConvolverTrackProperties *atp )
  : AFTrackSelectorDlg(parent),
    mAc(ac), mAtp(atp)
{
	// Set caption
	wxString title("Aurora for Audacity - Convolver - v.");
    title << Aurora::ModuleVersionString();
    SetTitle(title);

	// Draw logo
	wxBoxSizer* pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

	pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0), 
                    0, wxALL, 5 );

	pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 ); // spacer
	pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("conv_logo"),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0),
                    0, wxALL, 5 );

	m_logoPanel->SetSizer( pLogoSizer );
	m_logoPanel->Layout();

	// Init widgets
   PopulateAvailableTracksList();
   
   //setup drag'n'drop
   auto availDataTarget = new Aurora::ConvolverTrackDropTarget(mAvailableList);
   mAvailableList->SetDropTarget(availDataTarget);
   
   auto audioDataTarget = new Aurora::ConvolverTrackDropTarget(mAudioDataList);
   mAudioDataList->SetDropTarget(audioDataTarget);
   
   auto filterTarget = new Aurora::ConvolverTrackDropTarget(mFilterList);
   mFilterList->SetDropTarget(filterTarget);
    
    wxSize sz = m_outputDescriptionLabel->GetSize();
    m_outputDescriptionLabel->Wrap(sz.GetWidth() - 10);
    CheckSetup();
}

//----------------------------------------------------------------------------
// Aurora::ConvolverDialog
//----------------------------------------------------------------------------

void Aurora::ConvolverDialog::OnOk( wxCommandEvent& event )
{
    EndModal(true);
}

void Aurora::ConvolverDialog::OnCancel( wxCommandEvent& event )
{
    EndModal(false);
}

void Aurora::ConvolverDialog::OnHelp( wxCommandEvent& event )
{
    wxString path;
    path << "Docs" << TRAILING_SLASH << "lin_conv.html";
    wxFileName help_path(path);
    help_path.MakeAbsolute();

    ::wxLaunchDefaultBrowser(help_path.GetFullPath());
}

void Aurora::ConvolverDialog::OnFullAutorangeCheck( wxCommandEvent& event )
{
   if(event.IsChecked())
   {
       mGainStext->Disable();
       mGainText->Disable();
       mdBStext->Disable();
       mAc->SetFullAutorange(true);
       
       if(mFBAutorangeCheck->IsChecked())
       {
          mFBAutorangeCheck->SetValue(false);
       }
   }
   else
   {
       mGainStext->Enable();
       mGainText->Enable();
       mdBStext->Enable();
       mAc->SetFullAutorange(false);
   }
}

void Aurora::ConvolverDialog::OnFirstBlockAutorangeCheck( wxCommandEvent& event )
{
   if(event.IsChecked())
   {
       mGainStext->Disable();
       mGainText->Disable();
       mdBStext->Disable();
       mAc->SetFirstBlockAutorange(true);
       
       if(mFullAutorangeCheck->IsChecked())
       {
          mFullAutorangeCheck->SetValue(false);
       }
   } 
   else 
   {
       mGainStext->Enable();
       mGainText->Enable();
       mdBStext->Enable();
       mAc->SetFirstBlockAutorange(false);
   }
}

void Aurora::ConvolverDialog::OnGainText( wxCommandEvent& event )
{
   double d = ::TextPtrToDouble(mGainText);
   mAc->SetGain(d);
}

void Aurora::ConvolverDialog::OnTimeReverseIrCheck( wxCommandEvent& event )
{
    mAc->SetTimeReversedIr(event.IsChecked());
    mPlot->Refresh();
}

void Aurora::ConvolverDialog::OnPreserveLengthCheck( wxCommandEvent& event )
{
    mAc->SetPreserveLength(event.IsChecked());
}

void Aurora::ConvolverDialog::OnCrosstalkCancelCheck( wxCommandEvent& event )
{
    mAc->SetCancelCrossTalk(event.IsChecked());
}

// *** Ctors
Aurora::ConvolverDialog::ConvolverDialog(wxWindow* parent, 
                                         Aurora::ConvolverEffect *ac)
 : AFConvolverDlg( parent ),
   mAc(ac)
{
	// Set caption
	wxString title("Aurora for Audacity - Convolver - v.");
    title << Aurora::ModuleVersionString();
    SetTitle(title);

	// Draw logo
	wxBoxSizer* pLogoSizer;
	pLogoSizer = new wxBoxSizer( wxHORIZONTAL );

	pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("Aurora_logo"),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0),
                    0, wxALL, 5 );

	pLogoSizer->Add( 0, 0, 1, wxEXPAND, 5 ); // spacer
	pLogoSizer->Add(new wxStaticBitmap(m_logoPanel, wxID_ANY,
                                       Aurora::ArtProvider::GetBitmap("conv_logo"),
                                       wxDefaultPosition, 
                                       wxDefaultSize, 
                                       0), 
                    0, wxALL, 5 );

	m_logoPanel->SetSizer( pLogoSizer );
	m_logoPanel->Layout();

	// Init widgets
    wxString value;
    value.Printf("%.2f", mAc->GetGain());
    mGainText->SetValue(value);

    if(((mAc->FilterRows() != 2) && (mAc->FilterColumns() != 2)) || mAc->IsMatrixMode())
    {
        mCrossTalkCheck->Disable();
    }
    else
    {
        mCrossTalkCheck->SetValue(mAc->IsCancelCrossTalk());
    }

    //Set/unset checks
    mFullAutorangeCheck->SetValue(mAc->IsFullAutorange());
    mFBAutorangeCheck->SetValue(mAc->IsFirstBlockAutorange());
    mTimeReversedCheck->SetValue(mAc->IsTimeReversedIr());
    mPreserveLengthCheck->SetValue(mAc->IsLengthPreserved());

    if(mFullAutorangeCheck->IsChecked() || mFBAutorangeCheck->IsChecked())
    {
        mGainText->Disable();
        mGainStext->Disable();
        mdBStext->Disable();
    }
   
    // Update info static text
    wxString label;
    label.Printf("%.0f Hz/", mAc->GetSamplerate());
    value.Printf("%d channel", mAc->FilterColumns());
    label << value;
    label << (mAc->FilterColumns() > 1 ? "s/" 
                                       : "/");
    value.Printf("%ld samples", long(mAc->GetInputTrack(0).Length()));
    label << value;
    mAudioDataStext->SetLabel(label);

    label.Printf("%.0f Hz/", mAc->GetSamplerate());

    if(mAc->IsMatrixMode())
    {
        value.Printf("%d x %d Matrix/%ld samples",
                     mAc->FilterRows(),
                     mAc->FilterColumns(),
                     long(mAc->GetFilters()[0].Length()/ mAc->FilterColumns()));
    }
    else
    {
        int nFilters = 0;
        
        for (auto& f : mAc->GetFilters())
        {
            if (! f.IsNull())
            {
                ++nFilters;
            }
        }
        value.Printf("%d channel%s/%ld samples",
                     nFilters,
                     (nFilters > 1 ? "s" : " "),
                     long(mAc->GetFilters()[0].Length()));
    }
    label << value;
    mFilterStext->SetLabel(label);

    label.Printf("%ld samples", long(mAc->GetFFTLength()));
    mFftSizeStext->SetLabel(label);

    if (mAc->IsMatrixMode())
    {
        label.Printf("Matrix Convolution Product (%d x %d)", mAc->FilterRows(),
                     mAc->FilterColumns());
    }
    else
    {
        label.Printf("One-to-one Convolution Product (%d output tracks)",
                     mAc->FilterRows());
    }
    mModeStext->SetLabel(label);

    // Initialize plot
//    mPlot->SetTitle("RMS");
    mPlot->GetTimeAxis().SetTitle("Time [s]");
    mPlot->GetTimeAxis().SetFormat(Aurora::AxisBase::RF_REAL);
    mPlot->GetTimeAxis().ShowTitle(true);
    mPlot->GetTimeAxis().ShowGrid(true);

    mPlot->GetLevelAxis().SetTitle("RMS Level [dB]");
    mPlot->GetLevelAxis().SetRange(-120, 0, Aurora::AxisBase::RF_LINEARdB);
    mPlot->GetLevelAxis().ShowTitle(true);
    mPlot->GetLevelAxis().ShowGrid(true);

    wxColour col;
    Aurora::ConvolverTimeDataSerie* tds;

    for(int nCh = 0; nCh < mAc->FilterRows(); nCh++)
    {
        auto& data = mAc->GetFilters()[nCh];
        
        if (data.IsNull())
        {
            continue;
        }
        tds = new Aurora::ConvolverTimeDataSerie();
        tds->CopyTimeData(data);
        tds->SetRate(mAc->GetSamplerate());        
        tds->SetTimeReversed(mAc->IsTimeReversedIr());
        
        label.Printf("Ch %d", nCh+1);
        tds->SetLabel(label);
        
        Aurora::ChooseColour(col, nCh);
        tds->SetLineColour(col);
        
        mPlot->AddSerie((DataSerie*)tds);
    }
    mPlot->SetTopBorder(false);
    mPlot->SetRightBorder(false);    
    mPlot->ShowLegend(true);
    
    mPlot->Refresh();
}

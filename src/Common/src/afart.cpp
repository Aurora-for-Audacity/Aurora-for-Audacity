/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common libraries

  afart.cpp

  Simone Campanini

**********************************************************************/

#include <aurora.h>
#include "stickers.h"

//----------------------------------------------------------------------------
// AFArtProvider declaration
//----------------------------------------------------------------------------
wxBitmap Aurora::ArtProvider::CreateBitmap(const wxArtID& id, 
                                           const wxArtClient& client, 
                                           const wxSize& size)
{
    if(id == wxT("Aurora_logo")) return GetBitmapFromMemory(Aurora_logo);
    if(id == wxT("ap_logo"))     return GetBitmapFromMemory(ap_logo);
    if(id == wxT("conv_logo"))   return GetBitmapFromMemory(conv_logo);
    if(id == wxT("kirk_logo"))   return GetBitmapFromMemory(kirk_logo);
    if(id == wxT("ssg_logo"))    return GetBitmapFromMemory(ssg_logo);
    if(id == wxT("sti_logo"))    return GetBitmapFromMemory(sti_logo);
    if(id == wxT("tha_logo"))    return GetBitmapFromMemory(tha_logo);
    if(id == wxT("xfn_logo"))    return GetBitmapFromMemory(xfn_logo);
    if(id == wxT("arw_up"))      return GetBitmapFromMemory(arw_up);
    if(id == wxT("arw_right"))   return GetBitmapFromMemory(arw_right);
    if(id == wxT("arw_down"))    return GetBitmapFromMemory(arw_down);
    if(id == wxT("arw_left"))    return GetBitmapFromMemory(arw_left);

    return wxNullBitmap;
}

void InitArtProvider()
{
   wxArtProvider::Push(new Aurora::ArtProvider);
}


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

/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
                       plugin collection

  Common libraries

  afart.h

  Simone Campanini

**********************************************************************/

#ifndef __AURORA_ART_H__
#define __AURORA_ART_H__


/// Macro to embed graphics in the wxArtProvider way
#define GetBitmapFromMemory(name) _GetBitmapFromMemory(name ## _png, sizeof(name ## _png))

inline wxBitmap _GetBitmapFromMemory(const unsigned char *data, size_t length)
{
   wxMemoryInputStream is(data, length);
   return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1);
}

//----------------------------------------------------------------------------
// AFArtProvider declaration
/// \brief The bitmap manager for Aurora's GUI
/// This class act as a image manager optimizing memory usage: what the
/// user have to do is an InitArtProvider() call in some part of the code
/// then a GetBitmap() call is sufficient whenever a bitmap of the
/// provider is needed.
//----------------------------------------------------------------------------
namespace Aurora
{
  class ArtProvider : public wxArtProvider
  {
    protected:
  	 /// Allocates memory for a new bitmap. But the user can get rid of it.
       virtual wxBitmap CreateBitmap(const wxArtID& id, const wxArtClient& client, const wxSize& size);
  };
}
//----------------------------------------------------------------------------
// AFArtProvider initializer
/// \brief - Initialize AFArtProvider.
/// This call is mandatory before the usage of the art provider. You can
/// call it whenever you want, but before a GetBitmap call, obviously.
//----------------------------------------------------------------------------
void InitArtProvider();


#endif  // __AURORA_ART_H__

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

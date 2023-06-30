/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsArt.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_XFUNCTIONS_ART_H__
#define __AURORA_XFUNCTIONS_ART_H__

//
// XFunctionsArtProvider implementation
//
#include <commdefs.h>
#include <wx/artprov.h>
#include <wx/image.h>
#include <wx/mstream.h>

//
// XFunctionsArtProvider declaration 
//
namespace Aurora
{
    class XFunctionsArtProvider : public wxArtProvider
    {
    protected:
        virtual wxBitmap CreateBitmap(const wxArtID& id, 
                                      const wxArtClient& client, 
                                      const wxSize& size);
    };


    //
    // XFunctionsArtProvider initializer
    //
    void InitXFunctionsArtProvider();
}

#endif // __AURORA_XFUNCTIONS_ART_H__
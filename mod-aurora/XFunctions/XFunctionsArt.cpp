/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  XFunctions

  XFunctionsArt.cpp

  Simone Campanini

**********************************************************************/
#include <aurora.h>

#include "XFunctionsArt.h"
#include "equations.h"

//
// XFunctionsArtProvider implementation
//
wxBitmap Aurora::XFunctionsArtProvider::CreateBitmap(const wxArtID& id, 
                                                     const wxArtClient& client, 
                                                     const wxSize& size)
{
    if(id == "eq_autocorr")     return GetBitmapFromMemory(eq_autocorr);
    if(id == "eq_crosscorr")    return GetBitmapFromMemory(eq_crosscorr);
    if(id == "eq_whitened_csp") return GetBitmapFromMemory(eq_whitened_csp);
    if(id == "eq_crosspower")   return GetBitmapFromMemory(eq_crosspower);
    if(id == "eq_h1")           return GetBitmapFromMemory(eq_h1);
    if(id == "eq_h2")           return GetBitmapFromMemory(eq_h2);
    if(id == "eq_h3")           return GetBitmapFromMemory(eq_h3);
    if(id == "eq_alpha")        return GetBitmapFromMemory(eq_alpha);
    if(id == "eq_pu_swtube")    return GetBitmapFromMemory(eq_pu_swtube);
    if(id == "eq_pu_freefield") return GetBitmapFromMemory(eq_pu_freefield);

    return wxNullBitmap;
}


void Aurora::InitXFunctionsArtProvider()
{
   wxArtProvider::Push(new Aurora::XFunctionsArtProvider);
}


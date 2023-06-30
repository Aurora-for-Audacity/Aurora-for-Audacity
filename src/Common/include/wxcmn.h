/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Common libraries

  wxcmn.h

  Angelo Farina - Simone Campanini

**********************************************************************/
#ifndef __AURORA_WXCMN_H__
#define __AURORA_WXCMN_H__

#include <wx/wx.h>   // since 15/11/11 vc++ don't like it anymore...

#include <wx/image.h>
#include <wx/artprov.h>
#include <wx/mstream.h>
#include <wx/clipbrd.h>
#include <wx/config.h>
#include <wx/filedlg.h>
#include <wx/progdlg.h>
#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/colour.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/statline.h>

#if wxUSE_METAFILE
#  include <wx/metafile.h>
#endif

#include <gpfuncts.h>

#define wxAURORA_NAME wxT("Aurora")
#define wxAURORA_VERSION wxT("5.0")

#ifdef __WXWIN__
    // Windows use the backslash in path expressions...
    #define TRAILING_SLASH wxT("\\")
    #define TRAILING_SLASH_CH '\\'
#else
    // ...while Unixes system use the slash.
    #define TRAILING_SLASH wxT("/")
    #define TRAILING_SLASH_CH '/'
#endif

#ifdef __WXMSW__
   #define LINE_BREAK         _("\r\n")
   #define FILES_LINE_BREAK wxT("\r\n")
#else
   #define LINE_BREAK         _("\n")
   #define FILES_LINE_BREAK wxT("\n")
#endif


#endif //__AURORA_WXCMN_H__

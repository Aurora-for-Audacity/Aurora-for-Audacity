/**********************************************************************

  Aurora for Audacity: A Powerful multiplatform Acoustic calculations 
                       plugin collection
                       
  Time History Analyzer

  ThAnalyzerNbook.h

  Simone Campanini

**********************************************************************/
#ifndef __AURORA_ITUP56_NBOOK_H__
#define __AURORA_ITUP56_NBOOK_H__

/*
   This derivation was written to fix a color bug (I suppose)
   on MSW platform: when choosing wxNB_TOP as wxNotebook
   style, the background of notebook panels are drawn with
   a different, lightier, color than other widgets, so the
   appearance is bad. This doesn't happen when tabs are placed
   on left, right or bottom...
*/

#include <wx/notebook.h>

#ifdef __WXMSW__
    #define NB_STYLE wxNB_LEFT
#else
    #define NB_STYLE wxNB_TOP
#endif
namespace Aurora
{
    class ThAnalyzerNotebook : public wxNotebook
    {
      public:
        ThAnalyzerNotebook(wxWindow* parent, 
                           wxWindowID id, 
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxDefaultSize, 
                           long style = 0, 
                           const wxString& name = wxNotebookNameStr)
        : wxNotebook(parent, id, pos, size, NB_STYLE, name) 
        { }
    };
} // namespace
#endif // __AURORA_ITUP56_NBOOK_H__

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

/**********************************************************************
 *
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 *
 *  Common libraries
 *
 *  gpfuncts.h
 *
 *  Simone Campanini 2011
 *
 **********************************************************************/
#ifndef __GPFUNCTS_H__
#define __GPFUNCTS_H__

//#include <wx/wx.h>
#include <wx/colour.h>
#include <wx/grid.h>
#include <wx/string.h>
#include <wx/textctrl.h>


namespace Aurora
{
    wxString GetBandShortLabel(const float fcb,
                               const bool replaceDotsWithUnderscores = false);

    wxString GetBandLabel(const float fcb,
                          const bool replaceDotsWithUnderscores = false);

    float GetFcbFromLabel(const wxString& label);
    
    // Version number generator (yy.mm.dd)
    enum ReleaseType { ART_RELEASE = 0, ART_ALPHA, ART_BETA, ART_RC };
    wxString ModuleVersionNumber(const int nType = 0, const int nRc = 0);
}
//
// This file contains some general purpose functions prototypes
//

// wx-data conversion
int    TextPtrToInt(wxTextCtrl* pTc);
long   TextPtrToLong(wxTextCtrl* pTc);
double TextPtrToDouble(wxTextCtrl* pTc);
double StringToDouble(wxString& str);
long   StringToLong(wxString& str);
int    StringToInt(wxString& str);

namespace Aurora
{
    // math & dsp
    int NearTwoPower(const double dbValue);
    int NearTwoPowerExp(const double dbValue);

    enum WindowType {

            WT_RECTANGULAR = 0,
            WT_HANNING,
            WT_BLACKMANN_HARRIS,
            WT_FLAT_TOP,
            WT_TRIANGULAR,
            WT_HALF_SINE
    };
    void GenerateWindow(double* window, const size_t length, const int type);

    // aesthetic things...
    void ChooseColour(wxColour& colour, const int nCh);
    void ChooseColour(wxColour& colour, const unsigned char uchChnl);
    wxColour GetCanvasColour();
    void UpdateGridBackgroundColor(wxGrid* pwxGrid);
    void UpdateGridBackgroundColor(wxGrid* pwxGrid, const int nRowsCount, const int nColsCount);

    void GetAudacityChannelName(wxString& wxszDest, const int nChnl);
}
#endif //__GPFUNCTS_H__

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


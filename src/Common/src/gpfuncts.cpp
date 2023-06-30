/**********************************************************************
 *
 *  Aurora for Audacity: A Powerful multiplatform Acoustic calculations
 *                       plugin collection
 *
 *  Common libraries
 *
 *  gpfuncts.cpp
 *
 *  Simone Campanini 2011
 *
 **********************************************************************/
#include <aurora.h>

wxString Aurora::GetBandShortLabel(const float fcb,
                                   const bool replaceDotsWithUnderscores)
{
    assert(fcb > 0.0 && fcb <= 20000.0);
    
    wxString mult;
    float f = fcb;
            
    if (fcb > 1000.0)
    {
        f /= 1000.0;
        mult = "k";
    }
    float whole;
    float frac = std::modf(f, &whole);
    
    wxString s;
    
    s.Printf((frac > 0.0 ? "%.1f" : "%.0f"), f); 
    
    if (! mult.IsEmpty())
    {
        s << mult;
    }
    
    if (replaceDotsWithUnderscores)
    {
        s.Replace(".", "_");
    }
    return s;
}

wxString Aurora::GetBandLabel(const float fcb,
                              const bool replaceDotsWithUnderscores)
{
    assert(fcb > 0.0 && fcb <= 20000.0);
            
    float whole;
    float frac = std::modf(fcb, &whole);
    
    wxString s;
    
    s.Printf((frac > 0.0 ? "%.1f" : "%.0f"), fcb); 
        
    if (replaceDotsWithUnderscores)
    {
        s.Replace(".", "_");
    }
    return s;
}

float Aurora::GetFcbFromLabel(const wxString& label)
{
    std::string s = label.ToStdString();
    double value = 0.0;
    double mult  = 1.0;

    if (s.back() == 'k')
    {
        mult = 1000.0;
        s.pop_back();
    }
    value = std::stod(s);
    assert(value > 0.0);
    value *= mult;
    return (float)value;
}



// Generate Module version number
wxString Aurora::ModuleVersionNumber(const int nType, const int nRc)
{
    wxDateTime now;
    now.ParseDate(wxT(__DATE__));

    wxString version(AURORA_VERSION_String);
    version << "." << AURORA_BUILD_COUNT;

    switch(nType)
    {
        case ART_ALPHA: version << "-alpha";     break;
        case ART_BETA:  version << "-beta";      break;
        case ART_RC:    version << "-RC" << nRc; break;
    }
#if _DEBUG
    version << "-DBG";
#endif
    version << " (build of " << now.GetYear() << "-" << now.GetMonth()
            << "-" << now.GetDay() << ")";

    return version;
}

wxString Aurora::ModuleVersionString()
{
    return Aurora::ModuleVersionNumber(Aurora::ART_BETA, 0);
}

// Various utilities
long TextPtrToLong(wxTextCtrl* pTc)
{
    long l = 0;
    wxString str;
    
    if(!pTc->IsEmpty())
    {
        str = pTc->GetValue();
    }
    
    if(str.ToLong(&l)) 
    {
        return int(l);
    }
    return 0;
}

int TextPtrToInt(wxTextCtrl* pTc)
{
    return int(::TextPtrToLong(pTc));
}

double TextPtrToDouble(wxTextCtrl* pTc)
{
    double d = 0.0;
    wxString str;
    
    if(!pTc->IsEmpty())
    {
        str = pTc->GetValue();
    }
    
    if(str.ToCDouble(&d)) 
    {
        return d;
    }
    return 0.0;
}

double StringToDouble(wxString& str)
{
    double d = 0.0;
    
    if(str.ToCDouble(&d)) 
    {
        return d;
    }
    return 0.0;
}

long StringToLong(wxString& str)
{
   long l = 0;
   
   if(str.ToLong(&l)) 
   {
       return l;
   }
   return 0;
}

int StringToInt(wxString& str)
{
   long l = 0;
   
   if(str.ToLong(&l)) 
   {
       return int(l);
   }
   return 0;
}

int Aurora::NearTwoPower(const double dbValue)
{
    int nExp = 0;
    while(dbValue > pow(2.0, double(nExp++)));
    return int(pow(2.0, --nExp));
}

int Aurora::NearTwoPowerExp(const double dbValue)
{
    int nExp = 0;
    while(dbValue > pow(2.0, double(nExp++)));
    return --nExp;
}

void Aurora::GetAudacityChannelName(wxString& wxszDest, const int nChnl)
{
    switch(nChnl)
    {
        case 0: wxszDest = wxT("left");  break;
        case 1: wxszDest = wxT("right"); break;
        case 2: wxszDest = wxT("mono");  break;
        default: wxszDest.Printf(wxT("%d"), nChnl); 
    }
}


void Aurora::ChooseColour(wxColour& colour, const int nCh)
{
    switch(nCh) 
    {
        case 0:  colour.Set(  0,128,255); break;  // Aqua        
        case 1:  colour.Set(255,  0,128); break;  // Strawberry
        case 2:  colour.Set(  0,  0,128); break;  // Midnight
        case 3:  colour.Set(  0,128,  0); break;  // Clover
        case 4:  colour.Set(255,255,  0); break;  // Lemon
        case 5:  colour.Set(255,128,  0); break;  // Tangerine
        case 6:  colour.Set(255,  0,  0); break;  // Maraschino
        case 7:  colour.Set(  0,  0,255); break;  // Spring
            
        default: 
        {
            unsigned char uchCol = (unsigned char)(nCh*8);            
            colour.Set(uchCol, uchCol, uchCol); 
            break;
        }
    }
    
}

void Aurora::ChooseColour(wxColour& colour, const unsigned char uchChnl)
{
    unsigned char uchRed, uchGreen, uchBlue;
    
    switch(uchChnl)
    {
        case 0: uchRed =   0; uchGreen = 128; uchBlue = 255; break; // Aqua
        case 1: uchRed = 255; uchGreen =   0; uchBlue = 128; break; // Strawberry
        case 2: uchRed =   0; uchGreen =   0; uchBlue = 128; break; // Midnight
        case 3: uchRed =   0; uchGreen = 128; uchBlue =   0; break; // Clover
        case 4: uchRed = 255; uchGreen = 255; uchBlue =   0; break; // Lemon
        case 5: uchRed = 255; uchGreen = 128; uchBlue =   0; break; // Tangerine
        case 6: uchRed = 255; uchGreen =   0; uchBlue =   0; break; // Maraschino
        case 7: uchRed =   0; uchGreen =   0; uchBlue = 255; break; // Spring
            
        default:
            uchRed   = 255 * ((uchChnl & 4) >> 2) - 127 * ((uchChnl & 32) >> 5);
            uchGreen = 255 * ((uchChnl & 2) >> 1) - 127 * ((uchChnl & 16) >> 4);
            uchBlue  = 255 *  (uchChnl & 1)       - 127 * ((uchChnl &  8) >> 3);
            break;
    }
    
    colour.Set(uchRed, uchGreen, uchBlue);    
}

wxColour Aurora::GetCanvasColour()
{
    wxSystemColour syscol;
#if defined __WXOSX__
    syscol = wxSYS_COLOUR_WINDOWFRAME; 
#elif defined __WXMSW__
    syscol = wxSYS_COLOUR_WINDOW;
#else // GTK   
    syscol = wxSYS_COLOUR_WINDOWFRAME;
#endif
    return wxSystemSettings::GetColour(syscol);
}

void Aurora::UpdateGridBackgroundColor(wxGrid* pwxGrid)
{
    Aurora::UpdateGridBackgroundColor(pwxGrid, -1, -1);
}

void Aurora::UpdateGridBackgroundColor(wxGrid* grid, const int nRowsCount, const int nColsCount)
{
    // This is a dirty trick to solve the 'ugly white borders'
    // of the wxGrid: we have to default the grid's background
    // color to the window/panel color where the grid is
    // placed, then call this function that changes every
    // cell background color to white. It works!

    grid->SetDefaultCellBackgroundColour(Aurora::GetCanvasColour());
    
    int nRows = (nRowsCount < 0) ? grid->GetNumberRows() : nRowsCount;
    int nCols = (nColsCount < 0) ? grid->GetNumberCols() : nColsCount;
    
    int nRow, nCol;
    
    for(nRow = 0; nRow < nRows; nRow++)
    {
        for(nCol = 0; nCol < nCols; nCol++)
        {
            grid->SetCellBackgroundColour(nRow, nCol, *wxWHITE);
        }
    }
    grid->ForceRefresh();
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

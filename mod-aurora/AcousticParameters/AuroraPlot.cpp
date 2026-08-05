#include "AuroraPlot.h"

#include <algorithm>

#include <wx/setup.h> // for wxUSE_* macros

#include <wx/brush.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/font.h>
#include <wx/file.h>
#include <wx/frame.h>
#include <wx/scrolbar.h>
#include <wx/slider.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>

#include <wx/textctrl.h>
#include <wx/textfile.h>

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include <math.h>

#include "AColor.h"
#include "AllThemeResources.h"
#include "BasicUI.h"
#include "CommonCommandFlags.h"
#include "Decibels.h"
#include "FFT.h"
#include "PitchName.h"
#include "Prefs.h"
#include "Project.h"
#include "ProjectWindows.h"
#include "SelectFile.h"
#include "ShuttleGui.h"
#include "Theme.h"
#include "ViewInfo.h"

#include "FileNames.h"

#include "WaveTrack.h"

#include "AudacityMessageBox.h"
#include "HelpSystem.h"
#include "widgets/FreqGauge.h"
#include "widgets/LinearDBFormat.h"
#include "widgets/LinearUpdater.h"
#include "widgets/LogarithmicUpdater.h"
#include "widgets/RealFormat.h"
#include "widgets/RulerPanel.h"

#if wxUSE_ACCESSIBILITY
#include "WindowAccessible.h"
#endif

BEGIN_EVENT_TABLE(AuroraPlot, wxWindow)

EVT_ERASE_BACKGROUND(AuroraPlot::OnErase)
EVT_PAINT(AuroraPlot::OnPaint)
EVT_MOUSE_EVENTS(AuroraPlot::OnMouseEvent)

END_EVENT_TABLE()

AuroraPlot::AuroraPlot(wxWindow* parent,
                       wxWindowID id)
: wxWindow(parent, id), mMargins{40,20,30,35}
{
    
}

AuroraPlot::AuroraPlot(wxWindow *parent,
                       wxWindowID id,
                       Margins margins)
: wxWindow(parent, id), mMargins{mMargins}
{
    
}

void AuroraPlot::SetData(
                         const std::vector<double>& time,
                         const std::vector<double>& level)
{
    if(time.size() != level.size())
        return;
    
    
    mTime  = time;
    mLevel = level;
    
    
    if(!mTime.empty())
    {
        auto [tmin,tmax] =
        std::minmax_element(
                            mTime.begin(),
                            mTime.end()
                            );
        
        mMinTime = *tmin;
        mMaxTime = *tmax;
    }
    
    
    if(!mLevel.empty())
    {
        auto [lmin,lmax] =
        std::minmax_element(
                            mLevel.begin(),
                            mLevel.end()
                            );
        
        
        //        mMinLevel = *lmin;
        //        mMaxLevel = *lmax;
        
        mMinLevel = 0;
        mMaxLevel = 90;
        
        
        // Give some vertical headroom
        //        mMinLevel -= 5.0;
        //        mMaxLevel += 5.0;
    }
}




bool AuroraPlot::AcceptsFocus() const
{
    return false;
}


void AuroraPlot::OnErase(
                         wxEraseEvent& WXUNUSED(event))
{
    
}


void AuroraPlot::OnPaint(
                         wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    
    dc.Clear();
    
    {
        wxFont font = GetFont();
        const wxSize fontSize{12,12};
        font.SetPixelSize(fontSize);
        dc.SetFont(font);
        
        if(mTime.empty() || mLevel.empty())
            return;
        
        
        wxRect rect = GetClientRect();
        
        
        // Margins
        const int left   = mMargins.l;
        const int bottom = mMargins.b;
        const int top    = mMargins.t;
        const int right  = mMargins.r;
        
        
        wxRect plotArea(
                        left,
                        top,
                        rect.width-left-right,
                        rect.height-top-bottom
                        );
        
        
        //
        // Draw border
        //
        

        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.DrawRectangle(plotArea);
        
        
        //
        // Axis labels
        //
        
        dc.DrawText(
                    "Time [s]",
                    plotArea.x + plotArea.width/2,
                    rect.height-30
                    );
        
        wxString label = "Level [dB]";
        
        wxSize textSize = dc.GetTextExtent(label);
        
        int x = 0;
        
        // After rotation, the text width becomes the vertical height
        int y = plotArea.y +
        (plotArea.height + textSize.GetWidth()) / 2;
        
        
        dc.DrawRotatedText(
                           label,
                           x,
                           y,
                           90
                           );
        
        //
        // Convert data coordinates to pixels
        //
        
        auto ToX =
        [&](double t)
        {
            double ratio = (t-mMinTime) / (mMaxTime-mMinTime);
            return plotArea.x + (ratio * plotArea.width);
        };
        
        
        auto ToY =
        [&](double db)
        {
            double ratio = (db - mMinLevel) / (mMaxLevel - mMinLevel);
            return plotArea.y - (ratio * plotArea.height);
        };
        
    
        //
        // Draw Axis
        //
        
        dc.SetPen(*wxBLACK_PEN);
        dc.SetFont(font.Bold());
        
        auto lineWeight = dc.GetPen().GetWidth();
        double lStepSize = double(plotArea.height - lineWeight) / 90.0;            
        
        for(int db = 0; db <= 90; db += 6)
        {
            int ly = lineWeight + plotArea.y + int(lStepSize * double(db));
            
            
            dc.SetPen(*wxBLACK_PEN);
            
            dc.DrawLine(plotArea.x-5,
                        ly,
                        plotArea.x,
                        ly);
            
            
            
            auto yTickLabel = wxString::Format(( (db==0) ? "%d" : "-%d" ), db);
            
            dc.SetFont((db % 24 == 0) ? font.Bold() : font);
            
            dc.DrawText(yTickLabel,
                        mMargins.l-8-(yTickLabel.length()*6),
                        ly-6);
            
            if(db != 0 && db != 90)
            {
                
                dc.SetPen((db % 24 == 0) ? *wxGREY_PEN : *wxLIGHT_GREY);
                
                dc.DrawLine(plotArea.x,
                            ly,
                            plotArea.x+plotArea.width,
                            ly);
            }
            
        }
        
        //
        // Draw curve
        //
        
        dc.SetPen(
                  wxPen(
                        wxColour(0,80,200),
                        2
                        )
                  );
        
        
        for(size_t i = 1; i < mTime.size(); i++)
        {
            dc.DrawLine(
                        wxPoint(
                                ToX(mTime[i-1]),
                                ToY(mLevel[i-1])
                                ),
                        
                        wxPoint(
                                ToX(mTime[i]),
                                ToY(mLevel[i])
                                )
                        );
        }

    }
}

void AuroraPlot::OnMouseEvent(wxMouseEvent & WXUNUSED(event))
{
    std::cout << __func__ << '\n';
}

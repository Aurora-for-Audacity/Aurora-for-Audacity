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
:
wxWindow(parent, id)
{
    
}


//
//
//
//void AuroraPlot::SetData(
//        const std::vector<double>& time,
//        const std::vector<double>& level)
//{
//    if(time.size() != level.size())
//        return;
//
//
//    mTime  = time;
//    mLevel = level;
//
//
//    if(!mTime.empty())
//    {
//        auto [tmin,tmax] =
//            std::minmax_element(
//                mTime.begin(),
//                mTime.end()
//            );
//
//        mMinTime = *tmin;
//        mMaxTime = *tmax;
//    }
//
//
//    if(!mLevel.empty())
//    {
//        auto [lmin,lmax] =
//            std::minmax_element(
//                mLevel.begin(),
//                mLevel.end()
//            );
//
//
//        mMinLevel = *lmin;
//        mMaxLevel = *lmax;
//
//
//        // Give some vertical headroom
//        mMinLevel -= 5.0;
//        mMaxLevel += 5.0;
//    }
//}
//
//
//

bool AuroraPlot::AcceptsFocus() const
{
   return false;
}


void AuroraPlot::OnErase(
        wxEraseEvent& WXUNUSED(event))
{
 
}


void AuroraPlot::OnPaint(
        wxPaintEvent& WXUNUSED(event))
{
//    wxAutoBufferedPaintDC dc(this);
//
//    dc.Clear();
//
//    {
//        if(mTime.empty() || mLevel.empty())
//            return;
//
//
//        wxRect rect = GetClientRect();
//
//
//        const int left   = 60;
//        const int bottom = 40;
//        const int top    = 20;
//        const int right  = 20;
//
//
//        wxRect plotArea(
//            left,
//            top,
//            rect.width-left-right,
//            rect.height-top-bottom
//        );
//
//
//        //
//        // Draw border
//        //
//
//        dc.SetPen(*wxBLACK_PEN);
//        dc.SetBrush(*wxTRANSPARENT_BRUSH);
//
//        dc.DrawRectangle(plotArea);
//
//
//
//        //
//        // Axis labels
//        //
//
//        dc.DrawText(
//            "Time [s]",
//            plotArea.x + plotArea.width/2,
//            rect.height-30
//        );
//
//
//        dc.DrawText(
//            "dB",
//            10,
//            plotArea.y + plotArea.height/2
//        );
//
//
//
//        //
//        // Convert data coordinates to pixels
//        //
//
//        auto ToX =
//            [&](double t)
//            {
//                double ratio =
//                    (t-mMinTime) /
//                    (mMaxTime-mMinTime);
//
//
//                return plotArea.x +
//                    ratio*plotArea.width;
//            };
//
//
//        auto ToY =
//            [&](double db)
//            {
//                double ratio =
//                    (db-mMinLevel) /
//                    (mMaxLevel-mMinLevel);
//
//
//                return plotArea.y +
//                    plotArea.height -
//                    ratio*plotArea.height;
//            };
//
//
//
//        //
//        // Draw curve
//        //
//
//        dc.SetPen(
//            wxPen(
//                wxColour(0,80,200),
//                2
//            )
//        );
//
//
//        for(size_t i=1;i<mTime.size();i++)
//        {
//            dc.DrawLine(
//                wxPoint(
//                    ToX(mTime[i-1]),
//                    ToY(mLevel[i-1])
//                ),
//
//                wxPoint(
//                    ToX(mTime[i]),
//                    ToY(mLevel[i])
//                )
//            );
//        }
//
//
//
//        //
//        // Draw simple y-axis scale
//        //
//
//        dc.SetPen(*wxBLACK_PEN);
//
//
//        for(int db =
//                static_cast<int>(mMinLevel);
//            db <= static_cast<int>(mMaxLevel);
//            db += 10)
//        {
//
//            int y = ToY(db);
//
//            dc.DrawLine(
//                plotArea.x-5,
//                y,
//                plotArea.x,
//                y
//            );
//
//
//            dc.DrawText(
//                wxString::Format("%d",db),
//                20,
//                y-8
//            );
//        }
//    }
}

void AuroraPlot::OnMouseEvent(wxMouseEvent & WXUNUSED(event))
{
    
}

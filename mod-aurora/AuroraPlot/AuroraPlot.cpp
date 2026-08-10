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
: wxWindow(parent, id), mMargins{20,20,35,40}
{
    
}

AuroraPlot::AuroraPlot(wxWindow *parent,
                       wxWindowID id,
                       Margins margins)
: wxWindow(parent, id), mMargins{mMargins}
{
    
}

void AuroraPlot::SetData(const std::vector<double>& time,
                         const std::vector<double>& level)
{
    
    if(time.size() != level.size())
        return;
    
    mPlots.clear();
    mPlots.push_back({time, level});
    
    UpdateLimits();
}

void AuroraPlot::SetData(PlotData plotData){
    mPlots.clear();
    mPlots.push_back(plotData);
    UpdateLimits();
}

void AuroraPlot::SetData(std::vector<PlotData> plotData)
{
    mPlots.clear();
    for (auto data : plotData){
        mPlots.push_back(data);
    }
    UpdateLimits();
}


void AuroraPlot::AddPlotData(PlotData plotData)
{
    mPlots.push_back(plotData);
    UpdateLimits();
}
void AuroraPlot::AddPlotData(std::vector<PlotData> plotData)
{
    for (auto data : plotData){
        mPlots.push_back(data);
    }
    UpdateLimits();
}

void AuroraPlot::UpdateLimits()
{
    if(!mPlots.empty())
    {
        for (auto data : mPlots){
            // go through vector and find lowest and highest recordsfor x and y
            if(!data.x.empty())
            {
                auto [tmin,tmax] =
                std::minmax_element(data.x.begin(),
                                    data.x.end());
                mMinTime = *tmin;
                mMaxTime = *tmax;
            }
            
            if(!data.y.empty())
            {
                auto [lmin,lmax] =
                std::minmax_element(
                                    data.y.begin(),
                                    data.y.end()
                                    );
                mMinLevel = 0;
                mMaxLevel = 120.0;
            }
        }
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
        
        if(mPlots.empty())
            return;
        
        
        wxRect rect = GetClientRect();
        
        
        // Margins
        const int left   = mMargins.l;
        const int bottom = mMargins.b;
        const int top    = mMargins.t;
        const int right  = mMargins.r;
        
        
        mPlotArea = {
            left,
            top,
            rect.width-left-right,
            rect.height-top-bottom
        };
        
        auto y0 = mPlotArea.y;
        auto yh = mPlotArea.y + mPlotArea.height;
        auto x0 = mPlotArea.x;
        auto xw = mPlotArea.x + mPlotArea.width;
        
        //
        // Draw border
        //
        
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.DrawRectangle(mPlotArea);
        
        
        //
        // Axis labels
        //
        
        wxString xAxisLabel{"Time [s]"};
        dc.DrawText(xAxisLabel,
                    (xw / 2) - ((xAxisLabel.length() / 2) * dc.GetCharWidth()),
                    yh + ((dc.GetCharHeight()*3)/2));
        
        wxString label = "Level [dB]";
        
        wxSize textSize = dc.GetTextExtent(label);
        
        int x = 0;
        
        // After rotation, the text width becomes the vertical height
        int y = mPlotArea.y +
        (mPlotArea.height + textSize.GetWidth()) / 2;
        
        dc.DrawRotatedText( label, x, y, 90 );
        
        //
        // Convert data coordinates to pixels
        //
        
        auto ToX =
        [&](double t)
        {
            double ratio = (t-mMinTime) / (mMaxTime-mMinTime);
            return mPlotArea.x + (ratio * mPlotArea.width);
        };
        
        
        auto ToY =
        [&](double db)
        {
            double ratio = (db - mMinLevel) / (mMaxLevel - mMinLevel);
            return mPlotArea.y - (ratio * mPlotArea.height);
        };
        
        
        
        //
        // Draw y-Axis
        //
        
        dc.SetPen(*wxBLACK_PEN);
        dc.SetFont(font.Bold());
        
        auto lineWeight = dc.GetPen().GetWidth();
        double lStepSize = double(mPlotArea.height - (double(lineWeight) * 0.5)) / mMaxLevel;
        
        
        
        for(int db = 0; db <= mMaxLevel; db += 6)
        {
            int ly = mPlotArea.y + int(lStepSize * double(db));
            
            
            dc.SetPen(*wxBLACK_PEN);
            
            dc.DrawLine(x0-5,
                        ly,
                        x0,
                        ly);
            
            
            
            auto yTickLabel = wxString::Format(( (db==0) ? "%d" : "-%d" ), db);
            
            dc.SetFont((db % 24 == 0) ? font.Bold() : font);
            
            dc.DrawText(yTickLabel,
                        mMargins.l-8-(yTickLabel.length()*6),
                        ly-6);
            
            if(db != 0 && db != 90)
            {
                dc.SetPen((db % 24 == 0) ? *wxGREY_PEN : *wxLIGHT_GREY);
                dc.DrawLine(x0, ly, xw, ly);
            }
            
        }
        
        
        //
        // Draw X-axis
        //
        
        double duration = mMaxTime - mMinTime;
        // Find order of magnitude
        int order = static_cast<int>(std::floor(std::log10(duration)));
        double magnitude = std::pow(10, order);
        
        // Fraction of duration relative to its magnitude
        double fraction = duration / magnitude;
        
        // Choose tick step
        double step;
        
        if (fraction > 5.0) {
            step = 1.0 * magnitude;
        }
        else if (fraction > 2.5) {
            step = 0.5 * magnitude;
        }
        else if (fraction > 1.0) {
            step = 0.25 * magnitude;
        }
        else {
            step = 0.1 * magnitude;
        }
        
        
        const int numSteps = int(duration / step) + 1;
        
        std::cout << "step: " << step << '\n';
        std::cout << "fraction: " << fraction << '\n';
        std::cout << "magnitude: " << magnitude << '\n';
        std::cout << "numSteps: " << numSteps << '\n';
        std::cout << "duration: " << duration << '\n';
        std::cout << "mMinTime: " << mMinTime << '\n';
        std::cout << "mMaxTime: " << mMaxTime << '\n';
        
        auto durToX =
        [&](double t)
        {
            double ratio = (t) / (duration);
            return mPlotArea.x + (ratio * mPlotArea.width);
        };
        
        for (int i = 0; i < numSteps; i++) {
            double t = double(i) * step;
            
            dc.DrawLine(wxPoint(durToX(t),
                                y0),
                        wxPoint(durToX(t),
                                yh + 5));
            
            auto xTickLabel = wxString::Format(("%.3f"), t);
            auto xoffset = (xTickLabel.length()/2) * dc.GetCharWidth();
            dc.DrawText(xTickLabel, durToX(t)-xoffset, yh + 5);
        }
        
        //
        // Draw curve
        //
        
        dc.SetClippingRegion(mPlotArea);
        
        int lineID = 0;
        
        for (auto curve : mPlots)
        {
            dc.SetPen(wxPen(plotColours[lineID++], 2));
            
            for(size_t i = 1; i < curve.size(); i++)
            {
                dc.DrawLine(wxPoint(ToX(curve.x[i-1]),
                                    ToY(curve.y[i-1])),
                            wxPoint(ToX(curve.x[i]),
                                    ToY(curve.y[i])));
            }
        }
        
        //
        // Legend
        //
        
        const wxString legendText = "XXXXX";
        const int padding = 2;
        const int sampleWidth = 10;
        
        wxSize legendSize = dc.GetTextExtent(legendText);
        wxRect legendRect(
                          mPlotArea.x + mPlotArea.width - legendSize.GetWidth() - sampleWidth - (padding * 4),
                          mPlotArea.y + padding,
                          legendSize.GetWidth() + sampleWidth + padding*3,
                          (legendSize.GetHeight() + padding*2) * mPlots.size()
                          );
        
        // Background
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawRectangle(legendRect);
        
        int legendID = 0;
        
        for (auto curve : mPlots)
        {
            // Line sample
            int lineY = (legendSize.y+padding)*legendID + legendRect.y + legendRect.height / (mPlots.size() + 1);
            dc.SetPen(wxPen(plotColours[legendID], 2));
            
            dc.DrawLine(legendRect.x + padding,
                        lineY,
                        legendRect.x + padding + sampleWidth,
                        lineY);
            // Text
            dc.SetPen(*wxBLACK_PEN);
            dc.SetFont(font);
            
            dc.DrawText(curve.legendTitle,
                        legendRect.x + padding + sampleWidth + padding,
                        (legendRect.y + padding) + ((legendSize.y+padding)*legendID));
            
            legendID++;
        }
    }
}

void AuroraPlot::OnMouseEvent(wxMouseEvent & WXUNUSED(event))
{
//    std::cout << __func__ << '\n';
}

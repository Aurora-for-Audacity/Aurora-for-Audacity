//
//  AuroraPlot.hpp
//  mod-aurora
//
//  Created by admin on 29/07/2026.
//

#pragma once

#include <wx/wx.h>
#include <vector>
#include <wx/checkbox.h>
#include <wx/dcbuffer.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <widgets/RulerPanel.h>
#include <ShuttleGui.h>
#include <cmath>
#include <vector>

class AuroraPlot final : public wxWindow
{
    
public:
    
    struct Margins
    {
        int t;
        int r;
        int b;
        int l;
    };
    
    class PlotData
    {
    public:
        struct Point
        {
            double& x;
            double& y;
            
            Point& operator=(std::initializer_list<double> values)
            {
                auto it = values.begin();
                
                x = *it++;
                y = *it;
                
                return *this;
            }
        };
        
        PlotData(std::size_t n)
        : x(n), y(n)
        {}
        
        PlotData(std::vector<double> xValues,
                 std::vector<double> yValues)
        : x(std::move(xValues)),
        y(std::move(yValues))
        {
        }
        
        Point operator[](std::size_t i)
        {
            return {x[i], y[i]};
        }
        
        std::size_t size() const
        {
            return x.size();
        }
        
        std::vector<double> x;
        std::vector<double> y;
    };
    
    AuroraPlot(wxWindow *parent,
               wxWindowID id );
    
    AuroraPlot(wxWindow *parent,
               wxWindowID id,
               Margins margins);
    
    void SetData(const std::vector<double>& time,
                 const std::vector<double>& level);
    
    void SetData(PlotData plotData);
    
    void SetData(std::vector<PlotData> plotData);
    // We don't need or want to accept focus.
    bool AcceptsFocus() const;
    
    
private:
    void OnPaint(wxPaintEvent& event);
    void OnErase(wxEraseEvent & event);
    void OnMouseEvent(wxMouseEvent & event);
    
    const Margins mMargins;
    
    std::vector<double> mTime;
    std::vector<double> mLevel;
    
    double mMinTime;
    double mMaxTime;
    
    double mMinLevel;
    double mMaxLevel;
    
    std::vector<PlotData> mPlots{};
    
    DECLARE_EVENT_TABLE()
};


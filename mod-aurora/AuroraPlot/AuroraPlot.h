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
            
            Point(double& x, double& y)
                : x(x), y(y)
            {}

            Point& operator=(std::initializer_list<double> values)
            {
                assert(values.size() == 2);

                auto it = values.begin();
                x = *it++;
                y = *it;

                return *this;
            }
        };
        

        // Construct N points
        explicit PlotData(std::size_t n)
            : x(n), y(n)
        {}

        // Construct from x/y vectors
        PlotData(std::vector<double> xValues,
                 std::vector<double> yValues)
            : x(std::move(xValues)),
              y(std::move(yValues))
        {}

        // Construct from x/y vectors with legend
        PlotData(std::vector<double> xValues,
                 std::vector<double> yValues,
                 std::string legend)
            : x(std::move(xValues)),
              y(std::move(yValues)),
              legendTitle(std::move(legend))
        {}

        // Construct from existing PlotData with a new legend
        PlotData(const PlotData& other,
                 std::string legend)
            : x(other.x),
              y(other.y),
              legendTitle(std::move(legend))
        {}

        Point operator[](std::size_t i)
        {
            return { x[i], y[i] };
        }

        std::size_t size() const
        {
            return x.size();
        }

        class Iterator
        {
        public:
            Iterator(PlotData& data, std::size_t index)
                : mData(data), mIndex(index)
            {}

            Point operator*() const
            {
                return { mData.x[mIndex], mData.y[mIndex] };
            }

            Iterator& operator++()
            {
                ++mIndex;
                return *this;
            }

            bool operator!=(const Iterator& other) const
            {
                return mIndex != other.mIndex;
            }

        private:
            PlotData& mData;
            std::size_t mIndex;
        };

        Iterator begin()
        {
            return { *this, 0 };
        }

        Iterator end()
        {
            return { *this, size() };
        }

        std::vector<double> x;
        std::vector<double> y;
        std::string legendTitle;
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
    
    void AddPlotData(PlotData plotData);
    void AddPlotData(std::vector<PlotData> plotData);
    // We don't need or want to accept focus.
    bool AcceptsFocus() const;
    
    int GetPixelHeight(){ return GetClientRect().height; }
    int GetPixelWidth() { return GetClientRect().width;  }
private:
    void OnPaint(wxPaintEvent& event);
    void OnErase(wxEraseEvent & event);
    void OnMouseEvent(wxMouseEvent & event);
    
    void UpdateLimits();
    
    const Margins mMargins;
    
    double mMinTime;
    double mMaxTime;
    
    double mMinLevel;
    double mMaxLevel;
    
    std::vector<PlotData> mPlots{};
    
    wxRect mPlotArea;
    
    const std::array<wxColour, 8> plotColours{
        wxColour( 31, 119, 180), // blue
        wxColour(255, 127,  14), // orange
        wxColour( 44, 160,  44), // green
        wxColour(214,  39,  40), // red
        wxColour(148, 103, 189), // purple
        wxColour(140,  86,   75), // brown
        wxColour(227, 119, 194), // pink
        wxColour( 23, 190, 207)  // cyan
    };
    
    DECLARE_EVENT_TABLE()
};


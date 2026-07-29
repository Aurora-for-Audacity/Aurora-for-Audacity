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
#include "ShuttleGui.h"

class AuroraPlot final : public wxWindow
{
public:
    AuroraPlot(wxWindow *parent,
               wxWindowID id);
    
    void SetData(const std::vector<double>& time,
            const std::vector<double>& level);
    // We don't need or want to accept focus.
    bool AcceptsFocus() const;
    
private:
    void OnPaint(wxPaintEvent& event);
    void OnErase(wxEraseEvent & event);
    void OnMouseEvent(wxMouseEvent & event);
    
    std::vector<double> mTime;
    std::vector<double> mLevel;
    
    double mMinTime;
    double mMaxTime;
    
    double mMinLevel;
    double mMaxLevel;
    
    DECLARE_EVENT_TABLE()
};


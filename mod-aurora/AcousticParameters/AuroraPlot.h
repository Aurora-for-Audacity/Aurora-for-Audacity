//
//  AuroraPlot.hpp
//  mod-aurora
//
//  Created by admin on 29/07/2026.
//

#pragma once

#include <wx/wx.h>
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

    // We don't need or want to accept focus.
    bool AcceptsFocus() const;

private:
    void OnPaint(wxPaintEvent& event);
    void OnErase(wxEraseEvent & event);
    void OnMouseEvent(wxMouseEvent & event);


    DECLARE_EVENT_TABLE()
};

//class FreqPlot final : public wxWindow
//{
//public:
//   FreqPlot(wxWindow *parent, wxWindowID winid);
//
//   // We don't need or want to accept focus.
//   bool AcceptsFocus() const;
//
//private:
//   void OnPaint(wxPaintEvent & event);
//   void OnErase(wxEraseEvent & event);
//   void OnMouseEvent(wxMouseEvent & event);
//
//private:
//    FrequencyPlotDialog *freqWindow;
//
//    DECLARE_EVENT_TABLE()
//};

#pragma once

#include <wx/wx.h>
#include <wx/mstream.h>
#include <wx/image.h>

inline wxBitmap LoadPngBitmap(const unsigned char *data, size_t size)
{
    wxMemoryInputStream stream(data, size);

    wxImage image;
    if (!image.LoadFile(stream, wxBITMAP_TYPE_PNG))
        return wxBitmap();

    return wxBitmap(image);
}

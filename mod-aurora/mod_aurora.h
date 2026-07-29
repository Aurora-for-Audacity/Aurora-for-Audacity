#pragma once

inline wxBitmap LoadPngBitmap(const unsigned char *data, size_t size)
{
    wxMemoryInputStream stream(data, size);

    wxImage image;
    if (!image.LoadFile(stream, wxBITMAP_TYPE_PNG))
        return wxBitmap();

    return wxBitmap(image);
}

#ifndef PIXELOUTLINELIST_H
#define PIXELOUTLINELIST_H

#include "bitmap.h"
#include "color.h"
#include "pixeloutline.h"

#include <vector>

namespace AutoTrace {

class PixelOutlineList
{
public:
    PixelOutlineList();
    PixelOutlineList(Bitmap *bitmap, // This is equal to the 'find_centerline_pixels' function
                     Color bgColor);
    PixelOutlineList(Bitmap *bitmap, // This is equal to the find_outline_pixels
                     Color *bgColor);
    void appendPixelOutline(PixelOutline outline);

private:
    std::vector<PixelOutline> data;
};

}

#endif // PIXELOUTLINELIST_H

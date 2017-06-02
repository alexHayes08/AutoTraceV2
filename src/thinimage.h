#ifndef THINIMAGE_H
#define THINIMAGE_H

#include "bitmap.h"
#include "color.h"
#include "common.h"

#include <iostream>

namespace AutoTrace {

typedef unsigned char Pixel[3]; // RGB pixel data type


/* -------------------------------- ThinImage - Thin binary image. --------------------------- *
 *
 *    Description:
 *        Thins the supplied binary image using Rosenfeld's parallel
 *        thinning algorithm.
 *
 *    On Entry:
 *        image = Image to thin.
 *
 * -------------------------------------------------------------------------------------------- */
class ThinImage
{
public:
    static void thin3(Bitmap *image, Pixel color);
    static void thin1(Bitmap *image, unsigned char color);

    /* Direction masks:                  */
    /*   N     S     W        E            */
    static unsigned int masks[];

    /*    True if pixel neighbor map indicates the pixel is 8-simple and  */
    /*    not an end point and thus can be deleted.  The neighborhood     */
    /*    map is defined as an integer of bits abcdefghi with a non-zero  */
    /*    bit representing a non-zero pixel.  The bit assignment for the  */
    /*    neighborhood is:                                                */
    /*                                                                    */
    /*                            a b c                                   */
    /*                            d e f                                   */
    /*                            g h i                                   */
    static const unsigned char todelete[];

    static Color background;
    static void thinImage (Bitmap *image, const Color *bg);

private:
    static void SetPixel(Pixel &pixel1, Pixel &pixel2);
    static bool PixelEqual(Pixel &pixel1, Pixel &pixel2);
};

}

#endif // THINIMAGE_H

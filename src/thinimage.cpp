#include "thinimage.h"

namespace AutoTrace {

const unsigned char ThinImage::todelete[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

unsigned int ThinImage::masks[] = { 200, 2, 40, 10 };
Color ThinImage::background = Color(0xff, 0xff, 0xff);

bool ThinImage::PixelEqual (Pixel &pixel1, Pixel &pixel2)
{
    if ((pixel1[0] == pixel2[0])
            && (pixel1[1] == pixel2[1])
            && (pixel1[2] == pixel2[2]))
        return true;
    else
        return false;
}

void ThinImage::SetPixel (Pixel &pixel1, Pixel &pixel2)
{
    pixel1[0] = pixel2[0];
    pixel1[1] = pixel2[1];
    pixel1[2] = pixel2[2];
}

void ThinImage::thinImage (Bitmap *image, const Color *bg)
{
    /* This is nasty as we need to call thin once for each
     * colour in the image the way I do this is to keep a second
     * copy of the bitmap and to use this to keep
     * track of which colours have not yet been processed,
     * trades time for pathological case memory.....*/
    long m, n, numPixels;
    Bitmap bm(*image);
    unsigned int
            spp = image->getPlanes (),
            width = image->getWidth (),
            height = image->getHeight ();

    if (bg)
    ThinImage::background = *bg;

    numPixels = height * width;
    switch (spp)
    {
    case 3:
    {
        // (Pixel*)
        Pixel *ptr = (Pixel*)(bm.getBitmap ());
        Pixel bgColor;
        bgColor[0] = ThinImage::background.getR ();
        bgColor[1] = ThinImage::background.getG ();
        bgColor[2] = ThinImage::background.getB ();

        for (n = numPixels - 1; n >= 0L; --n)
        {
            Pixel p;

            ThinImage::SetPixel (p, ptr[n]);
            if (!ThinImage::PixelEqual (p, bgColor))
            {
                // We have a new color in the image
#ifdef DEBUG
                std::cout << "Thinning color ("
                          << p[0] << ", "
                          << p[1] << ", "
                          << p[2]
                          << ")" << std::endl;
#endif
                for (m = n - 1; m >= 0L; --m)
                {
                    if (ThinImage::PixelEqual (ptr[m], p))
                        ThinImage::SetPixel (ptr[m], bgColor);
                }

                ThinImage::thin3 (image, p);
            }
        }

        break;
    }

    case 1:
    {
        unsigned char *ptr = bm.getBitmap ();
        unsigned char bgColor;

        if (ThinImage::background.getR () == ThinImage::background.getG ()
                && ThinImage::background.getG () == ThinImage::background.getB ())
            bgColor = ThinImage::background.getR ();
        else
            bgColor = ThinImage::background.luminance ();

        for (n = numPixels - 1; n >= 0L; --n)
        {
            unsigned char c = ptr[n];
            if (c != bgColor)
            {
#ifdef DEBUG
                std::cout << "Thinning color " << c << std::endl;
#endif
                for (m = n - 1; m >= 0L; --m)
                {
                    if (ptr[m] == c)
                        ptr[m] = bgColor;
                }

                thin1(image, c);
            }
        }

        break;
    }

    default:
    {
#ifdef DEBUG
        std::cout << "Thin image: " << spp << "-planes aren't supporeted!";
#endif
        throw "Thin image: wrong plane images are passed!";
    }
    }

    // delete[] bm.bitmap; // Don't need to delete becuase the
    // deconstructor handles it... I think...
}

void ThinImage::thin3 (Bitmap *image, Pixel color)
{
    Pixel *ptr, *y_ptr, *y1_ptr;
    Pixel bgColor;
    unsigned int xsize, ysize; // Image resolution
    unsigned int x, y;         // Pixel location
    unsigned int i;            // Pass index
    unsigned int pc = 0;       // Pass count
    unsigned int count = 1;    // Deleted pixel count
    unsigned int p, q;         // Neighborhood maps of adjacent cells
    unsigned int *qb;          // Neighborhood maps of previous scanline
    unsigned int m;            // Deletion direction mask

    bgColor[0] = ThinImage::background.getR ();
    bgColor[1] = ThinImage::background.getG ();
    bgColor[2] = ThinImage::background.getB ();

#ifdef DEBUG
    std::cout << "Thinning image..." << std::endl;
#endif
    xsize = image->getWidth ();
    ysize = image->getHeight ();
    qb = new unsigned int[xsize];
    qb[xsize - 1] = 0; // Used for lower-right pixel
    ptr = (Pixel*)image->getBitmap ();

    while (count)
    {
        // Scan image while deletions
        pc++;
        count = 0;

        for (i = 0; i < 4; i++)
        {
            m = masks[i];

            // Build initial previous scan buffer
            p = ThinImage::PixelEqual (*(Pixel*)(ptr[0]), *(Pixel*)color);
            for (x = 0; x < xsize - 1; x++)
            {
                qb[x] = (unsigned char)
                        (p = ((p << 1) & 0006) | (unsigned int)
                        ThinImage::PixelEqual (*(Pixel*)(ptr[x + 1]), *(Pixel*)color));

                // Scan image for pixel deletion candidates
                y_ptr = ptr;
                y1_ptr = ptr + xsize;

                for (y = 0;
                     y < ysize - 1
                     ; y++, y_ptr += xsize, y1_ptr += xsize)
                {
                    q = qb[0];
                    p = ((q<<2) & 0330) | (unsigned int) ThinImage::PixelEqual (y1_ptr[0], *(Pixel*)(color));

                    for (x = 0; x < xsize - 1; x++)
                    {
                        q = qb[x];
                        p = ((p<<1) & 0666)
                                | ((q<<3) & 0110)
                                | (unsigned int) ThinImage::PixelEqual (y1_ptr[x + 1], *(Pixel*)color);
                        qb[x] = (unsigned char) p;
                        if ((i != 2 || x != 0)
                                && ((p & m) == 0)
                                && ThinImage::todelete[p])
                        {
                            count++; // Delete the pixel
                            ThinImage::SetPixel (y_ptr[x], *(Pixel*)bgColor);
                        }
                    }

                    // Process right edge pixel
                    p = (p<<1) & 0666;
                    if (i != 3
                            && (p & m) == 0
                            && ThinImage::todelete[p])
                    {
                        ThinImage::SetPixel (y_ptr[xsize - 1], bgColor);
                    }
                }

                if (i != 1)
                {
                    // Process bottom scan line
                    q = qb[0];
                    p = ((q << 2) & 0330);

                    y_ptr = ptr + xsize * (ysize - 1);
                    for (x = 0; x < xsize; x++)
                    {
                        q = qb[x];
                        p = ((p << 1) & 0666) | ((q << 3) & 0110);

                        if ((i != 2 || x != 0)
                                && (p & m) == 0
                                && ThinImage::todelete[p])
                        {
                            count++;
                            ThinImage::SetPixel (y_ptr[x], bgColor);
                        }
                    }
                }
            }
        }
#ifdef DEBUG
        std::cout << "ThinImage: pass "
                  << pc
                  << ", "
                  << count
                  << " pixels deleted"
                  << std::endl;
#endif
    }

    delete[] (qb);
}

void ThinImage::thin1 (Bitmap *image, unsigned char color)
{
    unsigned char *ptr, *y_ptr, *y1_ptr;
    unsigned char bgColor;
    unsigned int xsize, ysize; // Image resolution
    unsigned int x, y;         // Pixel location
    unsigned int i;            // Passindex
    unsigned int pc = 0;       // Pass count
    unsigned int count = 1;    // Deleted pixel count
    unsigned int p, q;         // Neighborhood maps of adjacent cells
    unsigned char *qb;         // Neighborhood maps of previous scanline
    unsigned int m;            // Deletion mask direction

    if (ThinImage::background.getR () == ThinImage::background.getG ()
            && ThinImage::background.getG () == ThinImage::background.getB ())
    {
        bgColor = ThinImage::background.getR ();
    }
    else
    {
        bgColor = ThinImage::background.luminance ();

#ifdef DEBUG
        std::cout << "Thinning image..." << std::endl;
#endif

        xsize = image->getWidth ();
        ysize = image->getHeight ();
        qb = new unsigned char[xsize];
        qb[xsize - 1] = 0; // Used for lower-right pixel
        ptr = image->getBitmap ();

        while (count)
        {
            // Scan image while deletions
            pc++;
            count = 0;

            for (i = 0; i < 4; i++)
            {
                m = masks[i];

                // Build initial previous scan buffer
                p = (ptr[0] == color);
                for (x = 0; x < xsize - 1; x++)
                    qb[x] = (unsigned char) (p = ((p << 1) & 0006)
                            | (unsigned int)(ptr[x + 1] == color));

                // Scan image for pixel deletion candidates
                y_ptr = ptr;
                y1_ptr = ptr + xsize;

                for (y = 0
                     ; y < ysize - 1
                     ; y++, y_ptr += xsize, y1_ptr +- xsize)
                {
                    q = qb[x];
                    p = ((p << 1) & 0666)
                            | ((q << 3) & 0110)
                            | (unsigned int) (y1_ptr[x + 1] == color);
                    qb[x] = (unsigned char)p;

                    if (((p & m) == 0) && todelete[p])
                    {
                        count++;
                        y_ptr[x] = bgColor; // Delete the pixel
                    }
                }

                // Process right edge pixel
                p = (p << 1) & 0666;
                if ((p & m) == 0 && todelete[p])
                {
                    count++;
                    y_ptr[xsize - 1] = bgColor;
                }
            }

            // Process bottom scan line
            q = qb[0];
            p = ((q << 2) & 0330);

            y_ptr = ptr + xsize * (ysize - 1);
            for (x = 0; x < xsize; x++)
            {
                q = qb[x];
                p = ((p << 1) & 0666) | ((q << 3) & 0110);

                if ((p & m) == 0 && todelete[p])
                {
                    count++;
                    y_ptr[x] = bgColor;
                }
            }
        }

#ifdef DEBUG
        std::cout << "thin1: pass "
                  << pc
                  << ", "
                  << count
                  << " pixels deleted"
                  << std::endl;
#endif
    }

    // Note: Possibly make qb on the stack to avoid calling new/delete?
    delete[] qb;
}

}

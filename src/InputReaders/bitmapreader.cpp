#include "bitmapreader.h"

namespace AutoTrace {

#define MAXCOLORS 256
#define Image long

#define BitSet(byte, bit) (((byte) & (bit)) == (bit))

#define ReadOK(file, buffer, len) (fread(buffer, len, 1, file) != 0)
//#define ReadOK(file, buffer, len) (file->read(buffer, len, 1) != 0)

std::string BitmapReader::fileExtension ()
{
    return "BMP";
}

Bitmap *BitmapReader::func(std::string filename,
                           AutoTrace::InputOptions *opts,
                           void* data)
{
    FILE *fd;
    unsigned char buffer[64];
    int ColormapSize, rowbytes, Maps, Grey;
    unsigned char ColorMap[256][3];
    Bitmap *image;
    unsigned char *imageStorage;
    BitmapFileHead bitmapFileHead;
    BitmapHead bitmapHead;

    try {
//        fd->open(filename, std::ios::binary);
      fd = fopen(filename.c_str(), "rb");

        if (!fd)
        {
            throw "Error: Cannot open input file!";
        }

        // It's a file, now is it a Bitmap? Read the shortest possible header.
        if (!ReadOK (fd, buffer, 18) || (strncmp ((const char *)buffer, "BM", 2)))
        {
            throw "Error: Input file wasn't a valid BMP file!";
        }

        // bring them to the right byteorder. Not too nice, but it should work
        bitmapFileHead.bfSize = ToL (&buffer[0x02]); // Size of BMP file
        bitmapFileHead.zzHotX = ToS (&buffer[0x06]); // Application specific
        bitmapFileHead.zzHotY = ToS (&buffer[0x08]); // Application specific
        bitmapFileHead.bf0ffs = ToL (&buffer[0x0a]); // Offset where the pixel array begins,
        bitmapFileHead.biSize = ToL (&buffer[0x0e]); // Number of bytes in the DIB header (from this point), aka the number of bytes required by the structure

        // What kind of Bitmap is it?
        if (bitmapFileHead.biSize == 12) // OS/2 1.x && Windows 2.0 or later
        {
            if (!ReadOK (fd, buffer, 8))
            {
                throw "Error reading BMP file header. Possible corrupted file?";
            }

            bitmapHead.biWidth = ToS (&buffer[0x00]);  // 12
            bitmapHead.biHeight = ToS (&buffer[0x02]); // 14
            bitmapHead.biPlanes = ToS (&buffer[0x04]); // 16
            bitmapHead.biBitCnt = ToS (&buffer[0x06]); // 18
            bitmapHead.biCompr = 0;
            bitmapHead.biSizeIm = 0;
            bitmapHead.biXPels = bitmapHead.biYpels = 0;
            bitmapHead.biClrUsed = 0;
            Maps = 3;
        }
        else if (bitmapFileHead.biSize == 40) // Windows 3.x or later, aka 24-bit bitmap
        {
            if (!ReadOK (fd, buffer, bitmapFileHead.biSize - 4))
            {
                throw "Error reading BMP file header!\n";
            }

            bitmapHead.biWidth = ToL (&buffer[0x00]);  // 12
            bitmapHead.biHeight = ToL (&buffer[0x04]); // 16
            bitmapHead.biPlanes = ToS (&buffer[0x08]); // 1A
            bitmapHead.biBitCnt = ToS (&buffer[0x0A]); // 1C
            bitmapHead.biCompr = ToL (&buffer[0x0C]);  // 1E
            bitmapHead.biSizeIm = ToL (&buffer[0x10]); // 22
            bitmapHead.biXPels = ToL (&buffer[0x14]);  // 26
            bitmapHead.biYpels = ToL (&buffer[0x18]);  // 2A
            bitmapHead.biClrUsed = ToL (&buffer[0x20]);// 2E
            bitmapHead.biClrImp = ToL (&buffer[0x20]); // 32
                                                       // 36
            Maps = 4;
        }
        else if (bitmapFileHead.biSize <= 64) // Probably OS/2 2.X
        {
            if (!ReadOK (fd, buffer, bitmapFileHead.biSize - 4))
            {
                throw "Error reading BMP file header!\n";
            }

            bitmapHead.biWidth = ToL (&buffer[0x00]);  // 12
            bitmapHead.biHeight = ToL (&buffer[0x04]); // 16
            bitmapHead.biPlanes = ToS (&buffer[0x08]); // 1A
            bitmapHead.biBitCnt = ToS (&buffer[0x0A]); // 1C
            bitmapHead.biCompr = ToL (&buffer[0x0C]);  // 1E
            bitmapHead.biSizeIm = ToL (&buffer[0x10]); // 22
            bitmapHead.biXPels = ToL (&buffer[0x14]);  // 26
            bitmapHead.biYpels = ToL (&buffer[0x18]);  // 2A
            bitmapHead.biClrUsed = ToL (&buffer[0x20]);// 2E
            bitmapHead.biClrImp = ToL (&buffer[0x20]); // 32
                                                       // 36
            Maps = 3;
        }
        else if (bitmapFileHead.biSize == 124)
        {
            if (!ReadOK (fd, buffer, bitmapFileHead.biSize - 4))
            {
                throw "Error reading BMP file header!\n";
            }

            bitmapHead.biWidth = ToL (&buffer[0x00]);  // 12
            bitmapHead.biHeight = ToL (&buffer[0x04]); // 16
            bitmapHead.biPlanes = ToS (&buffer[0x08]); // 1A
            bitmapHead.biBitCnt = ToS (&buffer[0x0A]); // 1C
            bitmapHead.biCompr = ToL (&buffer[0x0C]);  // 1E
            bitmapHead.biSizeIm = ToL (&buffer[0x10]); // 22
            bitmapHead.biXPels = ToL (&buffer[0x14]);  // 26
            bitmapHead.biYpels = ToL (&buffer[0x18]);  // 2A
            bitmapHead.biClrUsed = ToL (&buffer[0x20]);// 2E
            bitmapHead.biClrImp = ToL (&buffer[0x20]); // 32
                                                       // 36
            Maps = 3;
        }
        else
        {
            throw "Error reading BMP file header!\n";
        }

        /* Valid options 1, 4, 8, 16, 24, 32 */
        /* 16 is awful, we should probably shoot whoever invented it */

        /* There should be some colors used! */
        ColormapSize = (bitmapFileHead.bf0ffs - bitmapFileHead.biSize - 14) / Maps;

        if ((bitmapHead.biClrUsed == 0) && (bitmapHead.biBitCnt <= 8))
            bitmapHead.biClrUsed = ColormapSize;

        // Sanity Checks
        if ((bitmapHead.biHeight == 0 || bitmapHead.biWidth == 0)
                || (bitmapHead.biPlanes != 1)
                || (ColormapSize > 256 || bitmapHead.biClrUsed > 256))
        {
            throw "Error reading BMP file header!\n";
        }

        /* Windows and OS/2 declare filler so that rows are a multiple of
         * word length (32 bits == 4 bytes)
         */
        rowbytes = ((bitmapHead.biWidth * bitmapHead.biBitCnt - 1) / 32) * 4 + 4;

#ifdef DEBUG
        std::cout << "Size: " << bitmapFileHead.bfSize
                  << ", Colors: " << bitmapHead.biClrUsed
                  << ", Bits: " << bitmapHead.biBitCnt
                  << ", Width: " << bitmapHead.biWidth
                  << ", Height: " << bitmapHead.biHeight
                  << ", Comp: " << bitmapHead.biCompr
                  << ", Zeile: " << rowbytes
                  << std::endl;
#endif

        // Get the colormap
        ReadColorMap (fd, ColorMap, ColormapSize, Maps, &Grey);

#ifdef DEBUG
        std::cout << "Colormap read\n" << std::endl;
#endif

        // Get the image and return the ID of -1 on error
        imageStorage = ReadImage (fd,
                                  bitmapHead.biWidth,
                                  bitmapHead.biHeight,
                                  ColorMap,
                                  bitmapHead.biBitCnt,
                                  bitmapHead.biCompr,
                                  rowbytes,
                                  Grey);
        image = new Bitmap(imageStorage,
                           (unsigned short) bitmapHead.biWidth,
                           (unsigned short) bitmapHead.biHeight,
                           Grey ? 1 : 3);
    }
    catch (std::string e)
    {
        fclose (fd);
        throw e;
    }
    catch (...)
    {
        fclose (fd);
        throw "Error occurred while reading input file!";
    }

    fclose (fd);
    return image;
}

long BitmapReader::ToL (unsigned char *puffer)
{
    return (puffer[0]
            | puffer[1] << 8
            | puffer[2] << 16
            | puffer[3] << 24);
}

short BitmapReader::ToS (unsigned char *puffer)
{
    return ((short)(puffer[0] | puffer[1] << 8));
}

int BitmapReader::ReadColorMap (FILE *fd,
                                unsigned char buffer[256][3],
                                int number,
                                int size,
                                int *grey)
{
    int i;
    unsigned char rgb[4];

    *grey = (number > 2);
    for (i = 0; i < number; i++)
    {
        if (!ReadOK (fd, rgb, size))
        {
            throw "Bad colormap!/n";
        }

        // Bitmap save the colors in another order! But change only once!
        buffer[i][0] = rgb[2];
        buffer[i][1] = rgb[1];
        buffer[i][2] = rgb[0];
        *grey = ((*grey) && (rgb[0] == rgb[1]) && rgb[1] == rgb[2]);
    }

    return 0;
}

unsigned char* BitmapReader::ReadImage (FILE *fd,
                                        int width,
                                        int height,
                                        unsigned char cmap[256][3],
                                        int bpp,
                                        int compression,
                                        int rowbytes,
                                        int grey)
{
    unsigned char v, howmuch;
    int xpos = 0, ypos = 0;
    unsigned char *image;
    unsigned char *temp;//, *buffer;
    long rowstride, channels;
    unsigned short rgb;
    int i, j, notused;

    if (bpp >= 16) // color image
    {
        image = new unsigned char[width * height * 3 * sizeof(unsigned char)];
        channels = 3;
    }
    else if (grey) // grey image
    {
        image = new unsigned char[width * height * 1 * sizeof(unsigned char)];
        channels = 1;
    }
    else // indexed range
    {
        image = new unsigned char[width * height * 1 * sizeof(unsigned char)];
        channels = 1;
    }

    rowstride = width * channels;
//    unsigned char tempa[rowbytes] = { };
    unsigned char buffer[rowbytes];

    ypos = height - 1; // Bitmaps begin in the lower left corner
    switch (bpp)
    {
    case 32:
        while (ReadOK (fd, buffer, rowbytes))
        {
            temp = image + (ypos * rowstride);
            for (xpos = 0; xpos < width; ++xpos)
            {
                *(temp++) = buffer[xpos * 4 + 2];
                *(temp++) = buffer[xpos * 4 + 1];
                *(temp++) = buffer[xpos * 4];
            }
            --ypos; // next line
        }
        break;

    case 24:
        while (ReadOK (fd, buffer, rowbytes))
        {
            temp = image + (ypos * rowstride);
            for (xpos = 0; xpos < width; ++xpos)
            {
                *(temp++) = buffer[xpos * 3 + 2];
                *(temp++) = buffer[xpos * 3 + 1];
                *(temp++) = buffer[xpos * 3];
            }
            --ypos; // next line
        }
        break;

    case 16:
        while (ReadOK (fd, buffer, rowbytes))
        {
            temp = image + (ypos * rowstride);
            for (xpos = 0; xpos < width; ++xpos)
            {
                rgb = ToS(&buffer[xpos * 2]);
                *(temp++) = (unsigned char)(((rgb >> 10) & 0x1f) * 8);
                *(temp++) = (unsigned char)(((rgb >> 5) & 0x1f) * 8);
                *(temp++) = (unsigned char)(((rgb) & 0x1f) * 8);
            }
            --ypos; // next line
        }
        break;

    case 8:
    case 4:
    case 1:
        if (compression == 0)
        {
            while (ReadOK (fd, &v, 1))
            {
                for (i = 1; (i <= (8 / bpp)) && (xpos < width); i++, xpos++)
                {
                    temp = (unsigned char*) (image + (ypos * rowstride) + (xpos * channels));
                    *temp = (unsigned char) ((v & (((1 << bpp) - 1) << (8 - (i * bpp)))) >> (8 - (i *bpp)));
                }
                if (xpos == width)
                {
                    notused = ReadOK (fd, buffer, rowbytes - 1 - (width * bpp - 1) / 8);
                    ypos--;
                    xpos = 0;
                }

                if (ypos < 0)
                    break;
            }
            break;
        }
        else
        {
            while (ypos >= 0 && xpos <= width)
            {
                notused = ReadOK (fd, buffer, 2);
                if ((unsigned char) buffer[0] != 0)
                {
                    // Count + color - record
                    for (j = 0; ((unsigned char) j < (unsigned char) buffer[0]) && (xpos < width);)
                    {
#ifdef DEGUB
                        std::cout << xpos << " " << width << std::endl;
#endif
                        for (i = 1;
                             ((i <= (8 / bpp))
                              && (xpos < width)
                              && ((unsigned char) j < (unsigned char) buffer[0]));
                             i++, xpos++, j++)
                        {
                            temp = image + (ypos * rowstride) + (xpos * channels);
                            *temp = (unsigned char) ((buffer[1] & (((1 << bpp) -1) << (8 - (i * bpp)))) >> (8 - (i * bpp)));
                        }
                    }
                }
                if (((unsigned char) buffer[0] == 0) && ((unsigned char) buffer[1] > 2))
                {
                    // uncompressed record
                    howmuch = buffer[1];
                    for (j = 0; j < howmuch; j += (8 / bpp))
                    {
                        notused = ReadOK (fd, &v, 1);
                        i = 1;
                        while ((i <= (8 / bpp)) && (xpos < width))
                        {
                               temp = image + (ypos * rowstride) + (xpos * channels);
                               *temp = (unsigned char) ((v & (((1 << bpp) - 1) << (8 - (i * bpp)))) >> (8 - (i * bpp)));
                               i++;
                               xpos++;
                        }
                    }

                    if ((howmuch % 2) && (bpp == 4))
                        howmuch++;

                    if ((howmuch / (8 / bpp)) % 2)
                        notused = ReadOK (fd, &v, 1);
                    // if odd(x div (8 div bpp)) then blockread(f, z^, 1);
                }

                if (((unsigned char) buffer[0] == 0) && (unsigned char) buffer[1] == 0)
                {
                    // line end
                    ypos--;
                    xpos = 0;
                }

                if (((unsigned char) buffer[0] == 0) && ((unsigned char) buffer[1] == 1))
                {
                    // bitmap end
                    break;
                }

                if (((unsigned char) buffer[0] == 0) && ((unsigned char) buffer[1] == 2))
                {
                    // Deltarecord
                    notused = ReadOK (fd, buffer, 2);
                    xpos += (unsigned char) buffer[0];
                    ypos -= (unsigned char) buffer[1];
                }
            }

            break;
        }

    default:
        // This is very bad, we should not be here!
        throw "Error: Unknown error happened at bitmapreader.cpp in function ReadImage!";
    }

    if (bpp <= 8)
    {
        unsigned char *temp2, *temp3;
        unsigned char index;
        temp2 = temp = image;

        // cleanup whatever image was pointing to
        delete[] image;

        image = new unsigned char[width * height * 3 * sizeof(unsigned char)];
        temp3 = image;

        for (ypos = 0; ypos < height; ypos++)
        {
            index = *temp2++;
            *temp3++ = cmap[index][0];
            if (!grey)
            {
                *temp3++ = cmap[index][1];
                *temp3++ = cmap[index][2];
            }
        }

        delete[] temp;
    }

    // Delete pointers
    //delete[] buffer;
    return image;
}

}

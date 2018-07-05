#ifndef BITMAPREADER_H
#define BITMAPREADER_H

#include "../bitmap.h"
#include "../common.h"
#include "../color.h"
#include "../inputoptions.h"
#include "../inputreader.h"

#include <iostream>
#include <string>
#include <stdio.h>

namespace AutoTrace
{

// Forward declare Bitmap
class Bitmap;

struct BitmapFileHead
{
    char zzMagic[2];       // 00 "BM"
    unsigned long bfSize;  // 02
    unsigned short zzHotX; // 06
    unsigned short zzHotY; // 08
    unsigned long bf0ffs;  // 0A
    unsigned long biSize;  // 0E
};

struct BitmapHead
{
    unsigned long biWidth;   // 12
    unsigned long biHeight;  // 16
    unsigned short biPlanes; // 1A
    unsigned short biBitCnt; // 1C
    unsigned long biCompr;   // 1E
    unsigned long biSizeIm;  // 22
    unsigned long biXPels;   // 26
    unsigned long biYpels;   // 2A
    unsigned long biClrUsed; // 2E
    unsigned long biClrImp;  // 32
                             // 36?
};

class BitmapReader : public InputReader
{
public:
//    BitmapReader();
    Bitmap *func(std::string filename,
                 AutoTrace::InputOptions *opts,
                 void* data);
    std::string getFileExtension();
    void* data;
private:
    FILE *errorFile;
    std::string progname = "bmp";
    std::string filename;
    int interactiveBmp;
    static long ToL (unsigned char *puffer);
    static short ToS (unsigned char *puffer);
    static int ReadColorMap (FILE *fd,
                             unsigned char buffer[256][3],
                             int number,
                             int size,
                             int *grey);
    static std::vector<unsigned char> ReadImage(FILE *fd,
                                     int width,
                                     int height,
                                     unsigned char cmap[256][3],
                                     int bpp,
                                     int compression,
                                     int rowbytes,
                                     int grey);
};

}

#endif // BITMAPREADER_H

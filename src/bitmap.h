#ifndef BITMAP_H
#define BITMAP_H

#include "bitmapreader.h"
#include "color.h"
#include "common.h"
#include "inputoptions.h"
#include "inputreader.h"

#include <string.h>

namespace AutoTrace {

// The number of color planes of each pixel
#define AT_BITMAP_PLANES(b) ((b)->np)

/* The pixels, represented as an array of bytes (in contiguous storage).
   Each pixel is represented by np bytes.  */
#define AT_BITMAP_BITS(b) ((b)->bitmap)

// These are convenient abbreviations for geting inside the members.
#define AT_BITMAP_WIDTH(b) ((b)->width)
#define AT_BITMAP_HEIGHT(b) ((b)->height)

// This is the pixel at [ROW, COL]
#define AT_BITMAP_PIXEL(b, row, col)            \
    ((AT_BITMAP_BITS (b) + (row) * AT_BITMAP_PLANES (b)) * AT_BITMAP_WIDTH(b) \
    + (col) * AT_BITMAP_PLANES(b))

/* Threshold for binarizing a monochrome image */
#define GRAY_THRESHOLD 225

#define WHITE 0xff
#define BLACK 0

class Bitmap
{
public:
    Bitmap();
    Bitmap(unsigned char *area,
           unsigned short width,
           unsigned short height,
           unsigned int planes);
    Bitmap(unsigned short width,
           unsigned short height,
           unsigned int planes);
    Bitmap(InputReader *reader, // at_bitmap_read
           std::string filename,
           InputOptions *opts);
    Bitmap(const Bitmap &original);
    ~Bitmap();

    unsigned short getWidth();
    unsigned short getHeight();
    unsigned int getPlanes();
    unsigned char* getBitmap() const;
    Color getColor(unsigned int row, unsigned int col);
    bool equalColorAt(Color color, unsigned int row, unsigned int col);
    bool isOutlineEdge(Edge edge,
                       unsigned short row,
                       unsigned short col,
                       Color color);
    unsigned char& atPixel (unsigned int row, unsigned int col);
    bool validPixel(unsigned int row, unsigned int col);
    void Binarize();

private:
    void init(unsigned char *area,
         unsigned short width,
         unsigned short height,
         unsigned int planes);
    unsigned short height;
    unsigned short width;
    unsigned char *bitmap;
    unsigned int np;
};

}

#endif // BITMAP_H

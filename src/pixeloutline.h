#ifndef PIXELOUTLINE_H
#define PIXELOUTLINE_H

#include "bitmap.h"
#include "color.h"
#include "common.h"

#include <vector>

namespace AutoTrace {

class PixelOutline
{
public:
    PixelOutline();
    PixelOutline(Bitmap *bitmap,
                 Edge originalEdge,
                 unsigned short originalRow,
                 unsigned short originalCol,
                 Bitmap *marked,
                 bool clockwise,
                 bool ignore); // find_one_outline
    PixelOutline(Bitmap *bitmap,
                 Direction searchDir,
                 unsigned short orginalRow,
                 unsigned short originalCol,
                 Bitmap *marked); // find_one_centerline
    ~PixelOutline();
    std::vector<Coord> data;
    bool clockwise;
    Color color;
    bool open;
    void appendPixelOutline (Coord c);
    bool isUnmarkedOutlineEdge (unsigned short row,
                                unsigned short col,
                                Edge edge,
                                Bitmap *bitmap,
                                Bitmap *marked,
                                Color color);
    bool isOutlineEdge (Edge edge,
                        Bitmap *bitmap,
                        unsigned short row,
                        unsigned short col,
                        Color color);
    void markEdge (Edge edge,
                   unsigned short row,
                   unsigned short col,
                   Bitmap *marked);
    void markDirection (unsigned short row,
                        unsigned short col,
                        Direction dir,
                        Bitmap *marked);
    bool isValidDirection (unsigned short row,
                           unsigned short col,
                           Direction dir,
                           Bitmap *bitmap,
                           Bitmap *marked);
    bool isValid (unsigned short row,
                  unsigned short col,
                  Direction dir,
                  Bitmap *bitmap,
                  Bitmap *marked);
    int computeRowDelta(Direction dir);
    int computeColDelta(Direction dir);
    unsigned numberNeighbors (unsigned short row,
                              unsigned short col,
                              Bitmap *bitmap);
    void concatPixelOutline (const PixelOutline *outline);
private:
    Edge NumEdges = NO_EDGE;
    bool isMarkedEdge (Edge edge,
                       unsigned short row,
                       unsigned short col,
                       Bitmap *marked);
    bool isMarkedDirection (Direction edge, unsigned short row,
                            unsigned short col,
                            Bitmap *marked);
    bool isOtherDirectionMarked (unsigned short row,
                                 unsigned short col,
                                 Direction dir,
                                 Bitmap *marked);
    bool nextUnmarkedPixel(unsigned short *row,
                                unsigned short *col,
                                Direction *dir,
                                Bitmap *bitmap,
                                Bitmap *marked);
    Coord nextPoint (Bitmap *bitmap,
                     Edge *edge,
                     unsigned short *row,
                     unsigned short *col,
                     Color color,
                     bool clockwise,
                     Bitmap *marked);
};

}

#endif // PIXELOUTLINE_H

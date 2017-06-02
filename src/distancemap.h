#ifndef DISTANCEMAP_H
#define DISTANCEMAP_H

#include "bitmap.h"

namespace AutoTrace {

#ifndef M_SQRT2
#define M_SQRT2 1.41421356237
#endif

class DistanceMap
{
public:
    DistanceMap();
    DistanceMap(const DistanceMap &original);
    DistanceMap(Bitmap *bitmap,
                unsigned char targetValue,
                bool padded);
    ~DistanceMap();
    unsigned height, width;
    float **weight;
    float **d;
};

}

#endif // DISTANCEMAP_H

#ifndef REALCOORD_H
#define REALCOORD_H

#include <cmath>

namespace AutoTrace {

class RealCoord
{
public:
    RealCoord();
    RealCoord(const RealCoord &original);
    RealCoord(const RealCoord coord, const float r);
    RealCoord(const RealCoord coord1, const RealCoord coord2);
    float x,y,z;
    static float distance(RealCoord p1, RealCoord p2);
};

}

#endif // REALCOORD_H

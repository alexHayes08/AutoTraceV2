#ifndef REALCOORD_H
#define REALCOORD_H

namespace AutoTrace {

class RealCoord
{
public:
    RealCoord();
    RealCoord(const RealCoord &original);
    RealCoord(const RealCoord coord, const float r);
    RealCoord(const RealCoord coord1, const RealCoord coord2);
    float x,y,z;
};

}

#endif // REALCOORD_H

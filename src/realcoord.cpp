#include "realcoord.h"

namespace AutoTrace {

RealCoord::RealCoord()
{ }

RealCoord::RealCoord(const RealCoord &original)
{
    this->x = original.x;
    this->y = original.y;
    this->z = original.z;
}

RealCoord::RealCoord(const RealCoord coord, const float r)
{
    this->x = coord.x * r;
    this->y = coord.y * r;
    this->z = coord.z * r;
}

RealCoord::RealCoord(const RealCoord coord1, const RealCoord coord2)
{
    this->x = coord1.x + coord2.x;
    this->y = coord1.y + coord2.y;
    this->z = coord1.z + coord2.z;
}

}

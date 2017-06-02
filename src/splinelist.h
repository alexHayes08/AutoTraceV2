#ifndef SPLINELIST_H
#define SPLINELIST_H

#include "color.h"
#include "spline.h"

namespace AutoTrace {

class SplineList
{
public:
    SplineList();
    SplineList(const SplineList &original);
    Spline *data;
    unsigned length;
    bool clockwise;
    Color color;
    bool open;

    Spline &Elt(unsigned);
};

}

#endif // SPLINELIST_H

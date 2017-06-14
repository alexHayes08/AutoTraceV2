#ifndef CURVELIST_H
#define CURVELIST_H

#include "curve.h"

#include <vector>

namespace AutoTrace {

class CurveList
{
public:
    CurveList();
    CurveList(const CurveList &original);
    ~CurveList();
    void appendCurve(Curve *c);
    std::vector<Curve*> data;
    int length();
    Curve *elt(int index);
    Curve *lastElt();
    bool clockwise;
    bool open;

private:
};

}

#endif // CURVELIST_H

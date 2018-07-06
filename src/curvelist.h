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
    void appendCurve(std::shared_ptr<Curve> c);
    std::vector<std::shared_ptr<Curve>> data;
    int length();
    Curve *elt(int index);
    Curve first();
    Curve *last();
    bool clockwise;
    bool open;

private:
};

}

#endif // CURVELIST_H

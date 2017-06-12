#include "curvelist.h"

namespace AutoTrace {

CurveList::CurveList()
{ }

CurveList::CurveList(const CurveList &original)
{
    this->clockwise = original.clockwise;
    this->open = original.open;

    this->data = original.data;
//    for (auto el = original.data.begin(); el != original.data.end(); ++el)
//        this->data
}

CurveList::~CurveList()
{
    this->data.clear();
}

void CurveList::appendCurve(Curve *c)
{
    this->data.push_back(c);
}

int CurveList::length()
{
    return this->data.size();
}

Curve *CurveList::elt(int index)
{
    return this->data[index];
}

Curve *CurveList::lastElt()
{
    return this->data[this->data.size() - 1];
}

}

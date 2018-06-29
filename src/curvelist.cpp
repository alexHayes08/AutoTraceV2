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

void CurveList::appendCurve(std::shared_ptr<Curve> c)
{
    this->data.push_back(c);
}

int CurveList::length()
{
    return this->data.size();
}

Curve *CurveList::elt(int index)
{
    return this->data.at (index);
}

std::shared_ptr<Curve> CurveList::first()
{
    return this->data.front ();
}

Curve *CurveList::last()
{
    return this->data.at(this->data.size() - 1);
}

}

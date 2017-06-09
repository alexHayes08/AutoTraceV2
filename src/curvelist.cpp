#include "curvelist.h"

namespace AutoTrace {

CurveList::CurveList()
{ }

CurveList::~CurveList()
{
    this->data.clear();
}

void CurveList::appendCurve(Curve c)
{
    this->data.push_back(c);
}

int CurveList::length()
{
    return this->data.size();
}

Curve &CurveList::elt(int index)
{
    return this->data[index];
}

Curve &CurveList::lastElt()
{
    return this->data[this->data.size() - 1];
}

}

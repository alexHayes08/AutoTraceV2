#include "splinelist.h"

namespace AutoTrace {

SplineList::SplineList()
{ }

SplineList::SplineList(const SplineList &original)
{
    this->data = original.data;
    this->length = original.length;
    this->clockwise = original.clockwise;
    this->color = original.color;
    this->open = original.open;
}

Spline &SplineList::Elt (unsigned index)
{
    return this->data[index];
}

}

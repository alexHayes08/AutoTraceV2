#include "svgelement.h"

namespace QtAutoTraceV2
{

bool SvgElement::isLine(const QVector<QPoint> points, const int tolerance)
{
    // Can't have a line of less than two points.
    if (points.size() < 2)
    {
        return false;
    }

    auto firstPoint = points.first();
    auto lastPoint = points.last();
    auto generalAngle = (lastPoint.y() - firstPoint.y()) / (lastPoint.x() - firstPoint.x());

    throw NotImplementedException();
}

bool SvgElement::isCircle(const QVector<QPoint> points, const int tolerance)
{
    throw NotImplementedException();
}
}

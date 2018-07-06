#include "svgelement.h"

namespace QtAutoTraceV2
{

    std::unique_ptr<SvgElement> SvgElement::CreateElement(QVector<QPoint> points, int tolerance)
    {
        throw new std::runtime_error("Not implemented.");
    }

    bool SvgElement::isLine(QVector<QPoint> points, int tolerance)
    {
        // Can't have a line of less than two points.
        if (points.size() < 2)
        {
            return false;
        }

        auto firstPoint = points.first();
        auto lastPoint = points.last();
        auto generalAngle = (lastPoint.y() - firstPoint.y()) / (lastPoint.x() - firstPoint.x());

        throw std::runtime_error("Not implemented.");
    }
}

#pragma once

#include <exception>
#include <memory>

#include <QPoint>
#include <QVector>

#include "notimplementedexception.h"
#include "pixeldata.h"
#include "pixelgroup.h"
#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT SvgElement
{
public:
    QVector<QPoint> outlinePoints;
    QColor color;
    bool isClosed;

private:
    /**
     * @brief isLine determines if a group of points can be considered a line.
     * @param points
     * @param tolerance
     * @return
     */
    static bool isLine(const QVector<QPoint> points, const int tolerance);

    /**
     * @brief Determines if the set of points (which should be only the outline points)
     * form a cirle.
     * @param points
     * @param tolerance
     * @return
     */
    static bool isCircle(const QVector<QPoint> points, const int tolerance);
};

}

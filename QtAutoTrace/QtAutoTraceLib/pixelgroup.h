#pragma once

#include <algorithm>
#include <cmath>
#include <exception>

#include <QPoint>
#include <QRect>
#include <QVector>

#include "adjacentpixels.h"
#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT PixelGroup
{
public:
    /**
     * @briefDefault constructor.
     */
    PixelGroup() : PixelGroup(QVector<QPoint>()) {}

    /**
     * @param pixels All points should be connected (there should be one other
     * point one unit away from it).
     */
    PixelGroup(QVector<QPoint> pixels);

    /**
     * @brief Returns true if there are no pixels.
     */
    bool isEmpty();

    /**
     * Returns true if the outline (or part of the outline) can fully contain
     * an ellipse.
     */
    bool isClosed();
    QVector<QPoint> getPixels() const;
    QRect getBBox() const;
    QVector<QPoint> getOutline() const;
    bool isFilled();
    QVector<QPoint> getInflectionPoints() const;

private:
    QRect bbox;
    QVector<QPoint> pixels;
    QVector<QPoint*> outline;
    bool closed;
    bool filled;
};

}

#pragma once

#include <cmath>
#include <exception>
#include <limits>

#include <QPoint>
#include <QVector>

#include "pixeldata.h"
#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{
enum class AdjacentResult
{
    NOTADJACENT,
    NORTH,
    WEST,
    SOUTH,
    EAST
};

enum class CardinalDirection
{
    NORTH,
    NORTHWEST,
    WEST,
    SOUTHWEST,
    SOUTH,
    SOUTHEAST,
    EAST,
    NORTHEAST
};

/**
 * @brief The AdjacentPixels class is a utility class for working with pixels \
 * and determining how it relates to other pixels (how far away/at what angle). \
 * Cannot instantiate this class.
 */
class QTAUTOTRACELIBSHARED_EXPORT AdjacentPixels
{
public:
    /**
     * @brief ArePointsAdjacent checks that the points have one axis identical
     * (the x or y are the same for both points) and the other axis is only
     * larger/smaller by a value of one.
     * @param a
     * @param b
     * @return
     */
    static AdjacentResult ArePointsAdjacent(QPoint a, QPoint b);

    /**
     * @brief ArePixelsAdjacent will check if the pixels are of the same color
     * as well as the pixels share a common side.
     * @param a
     * @param b
     * @return
     */
    static AdjacentResult ArePixelsAdjacent(PixelData a, PixelData b);

    /**
     * Checks to see if point p has another point directly adjacent to it using
     * the points in otherPoints.
     * @param p
     * @param otherPoints
     * @return
     */
    static bool IsSurrounded(QPoint p, QVector<QPoint> otherPoints);

    /**
     * Determines if the outlinePoint has at LEAST two other points adjacent to
     * it but still has at LEAST one side without any adjacent point.
     * @param outlinePoint
     * @param otherOutlinePoints
     */
    static bool IsPointOutliningAndCyclic(QPoint point, QVector<QPoint> otherPoints);

    /**
     * @brief FindClosestPoints will return all that share the shortest distance
     * away from point.
     * @param point
     * @param otherPoints
     * @return
     */
    static QVector<QPoint*> FindClosestPoints(QPoint point, QVector<QPoint> otherPoints);

    /**
     * @brief GetDirectionOfPointRelativeTo is used to determine if 'point' is
     * north/north-east/east/south-east/etc... relative to the other point.
     * @param point
     * @param relativeTo
     * @return
     */
    static CardinalDirection GetDirectionOfPointRelativeTo(QPoint point, QPoint relativeTo);

    /**
     * @brief GetAngleBetweenPoints returns the angle in radians between two
     * points. Formula is: atan2( a.y - b.y, a.x - b.x).
     * @param a
     * @param b
     * @return
     */
    static double GetAngleBetweenPoints(QPoint a, QPoint b);

private:
    // This is to prevent instantition of the class.
    AdjacentPixels();
};

}

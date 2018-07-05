#include "adjacentpixels.h"

namespace QtAutoTraceV2
{

AdjacentResult AdjacentPixels::ArePointsAdjacent(QPoint a, QPoint b)
{
    // Check north.
    if (a.x() == b.x()
        && a.y() == b.y() - 1)
    {
        return AdjacentResult::NORTH;
    }
    // Check west.
    else if (a.x() == b.x() - 1
        && a.y() == b.y())
    {
        return AdjacentResult::WEST;
    }
    // Check south.
    else if (a.x() == b.x()
        && a.y() == b.y() + 1)
    {
        return AdjacentResult::SOUTH;
    }
    // Check east.
    else if (a.x() == b.x() - 1
        && a.y() == b.y())
    {
        return AdjacentResult::EAST;
    }
    // Not adjacent.
    else
    {
        return AdjacentResult::NOTADJACENT;
    }
}

AdjacentResult AdjacentPixels::ArePixelsAdjacent(PixelData a, PixelData b)
{
    // Return false if the colors aren't the same.
    if (a.color != b.color)
    {
        return AdjacentResult::NOTADJACENT;
    }

    return QtAutoTraceV2::ArePointsAdjacent(a.coord, b.coord);
}

bool AdjacentPixels::IsSurrounded(QPoint p, QVector<QPoint> otherPoints)
{
    bool north = false;
    bool west = false;
    bool south = false;
    bool east = false;

    for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
    {
        auto otherPoint = *it;
        auto result = ArePointsAdjacent(p, otherPoint);

        switch (result)
        {
        case AdjacentResult::NORTH:
            north = true;
            break;
        case AdjacentResult::WEST:
            west = true;
            break;
        case AdjacentResult::SOUTH:
            south = true;
            break;
        case AdjacentResult::EAST:
            east = true;
            break;
        }

        // Exit loop if done.
        if (north && west && south && east)
        {
            break;
        }
    }

    return north && west && south && east;
}

bool AdjacentPixels::IsPointOutliningAndCyclic(QPoint point,
    QVector<QPoint> otherPoints)
{
    int adjacentPoints = 0;

    for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
    {
        auto result = ArePointsAdjacent(point, *it);

        switch (result)
        {
        case AdjacentResult::NORTH:
        case AdjacentResult::WEST:
        case AdjacentResult::SOUTH:
        case AdjacentResult::EAST:
            adjacentPoints++;
            break;
        }

        if (adjacentPoints >= 4)
        {
            break;
        }
    }

    // Return true there are only two or three adjacent sides.
    return adjacentPoints == 2 || adjacentPoints == 3;
}

QVector<QPoint*> AdjacentPixels::FindClosestPoints(QPoint point,
    QVector<QPoint> otherPoints)
{
    auto result = QVector<QPoint*>();
    double shortestHyp = std::numeric_limits<double>::max();
    auto x1 = point.x();
    auto y1 = point.y();

    for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
    {
        auto otherPoint = *it;
        auto x2 = otherPoint.x();
        auto y2 = otherPoint.y();
        double hyp = std::hypot(std::abs(x1 - x2), std::abs(y1 - y2));

        if (hyp == shortestHyp)
        {
            result.append(&otherPoint);
        }
        else if (hyp < shortestHyp)
        {
            result.clear();
            result.append(&otherPoint);
        }
    }

    return result;
}

CardinalDirection AdjacentPixels::GetDirectionOfPointRelativeTo(QPoint point,
    QPoint relativeTo)
{
    auto dx = point.x() - relativeTo.x();
    auto dy = point.y() - relativeTo.y();

    // Verify the points aren't at the same position.
    if (dx == 0
        && dy == 0)
    {
        throw std::runtime_error("Points cannot be at identical postions.");
    }

    if (dx == 0 && dy < 0)
        return CardinalDirection::NORTH;
    else if (dx < 0 && dy < 0)
        return CardinalDirection::NORTHWEST;
    else if (dx < 0 && dy == 0)
        return CardinalDirection::WEST;
    else if (dx < 0 && dy > 0)
        return CardinalDirection::SOUTHWEST;
    else if (dx == 0 && dy > 0)
        return CardinalDirection::SOUTH;
    else if (dx > 0 && dy > 0)
        return CardinalDirection::SOUTHEAST;
    else if (dx > 0 && dy == 0)
        return CardinalDirection::EAST;
    else if (dx > 0 && dy < 0)
        return CardinalDirection::NORTHEAST;
    else
        throw std::runtime_error("Failed to determine the relative direction of the point.");
}

double AdjacentPixels::GetAngleBetweenPoints(QPoint a, QPoint b)
{
    return std::atan2(a.y() - b.y(), a.x() - b.x());
}

}

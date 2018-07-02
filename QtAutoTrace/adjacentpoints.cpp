#include "adjacentpoints.h"

namespace QtAutoTraceV2
{

AdjacentPoints::AdjacentPoints(QSize size, QVector<QPoint> points, QPoint point)
{
    this->point = &point;

    bool hasNorth = this->point->y() - 1 >= 0;
    bool hasWest = this->point->x() + 1 >= size.width();
    bool hasSouth = this->point->y() + 1 <= size.height();
    bool hasEast = this->point->x() - 1 >= 0;
    bool hasNorthWest = hasNorth && hasWest;
    bool hasSouthWest = hasSouth && hasWest;
    bool hasSouthEast = hasSouth && hasEast;
    bool hasNorthEast = hasNorth && hasEast;

    for (auto it = points.begin(); it != points.end(); it++)
    {
        QPoint _point = *it;

        // Check if all adjacent points have been found.
        if ((hasNorth && this->north != nullptr)
                && (hasNorthWest == (this->northWest != nullptr))
                && (hasWest == (this->west != nullptr))
                && (hasSouthWest == (this->southWest != nullptr))
                && (hasSouth == (this->south != nullptr))
                && (hasSouthEast == (this->southEast != nullptr))
                && (hasEast == (this->east != nullptr))
                && (hasNorthEast == (this->northEast != nullptr)))
        {
            // All points found, exit loop.
            break;
        }

        auto result = AdjacentPoints::ArePointsAdjacent(point, _point);
        switch (result)
        {
        case AdjacentDirection::NORTH:
            if (hasNorth)
                this->north = &point;
            break;
        case AdjacentDirection::NORTHWEST:
            if (hasNorthWest)
                this->northWest = &point;
            break;
        case AdjacentDirection::WEST:
            if (hasWest)
                this->west = &point;
            break;
        case AdjacentDirection::SOUTHWEST:
            if (hasSouthWest)
                this->southWest = &point;
            break;
        case AdjacentDirection::SOUTH:
            if (hasSouth)
                this->south = &point;
            break;
        case AdjacentDirection::SOUTHEAST:
            if (hasSouthEast)
                this->southEast = &point;
            break;
        case AdjacentDirection::EAST:
            if (hasEast)
                this->east = &point;
            break;
        case AdjacentDirection::NORTHEAST:
            if (hasNorthEast)
                this->northEast = &point;
        default:
            break;
        }
    }
}

bool AdjacentPoints::isEmpty()
{
    return (this->north == nullptr
        && this->northWest == nullptr
        && this->west == nullptr
        && this->southWest == nullptr
        && this->south == nullptr
        && this->southEast == nullptr
        && this->east == nullptr
        && this->northEast == nullptr);
}

/**
 * @brief AdjacentPoints::ArePointsAdjacent Checks if points a and b are within
 * one pixel (including diagnals) of each other.
 * @param a
 * @param b
 * @return
 */
AdjacentDirection AdjacentPoints::ArePointsAdjacent(
        const QPoint a,
        const QPoint b)
{
    auto a_x = a.x();
    auto a_y = a.y();
    auto b_x = b.x();
    auto b_y = b.y();

    // Check if point is north.
    if (a_x == b_x
            && a_y == a_y + 1)
    {
        return AdjacentDirection::NORTH;
    }

    // Check if north-west.
    else if (a_x == b_x + 1
            && a_y == b_y + 1)
    {
        return AdjacentDirection::NORTHWEST;
    }

    // Check if west.
    else if (a_x == b_x + 1
            && a_y == b_y)
    {
        return AdjacentDirection::WEST;
    }

    // Check if south-west.
    else if (a_x == b_x
            && a_y == b_y)
    {
        return AdjacentDirection::SOUTHWEST;
    }

    // Check if south.
    else if (a_x == b_x
            && a_y == b_y - 1)
    {
        return AdjacentDirection::SOUTH;
    }

    // Check if south-east.
    else if (a_x == b_x - 1
             && a_y == b_y - 1)
    {
        return AdjacentDirection::SOUTHEAST;
    }

    // Check if east.
    else if (a_x == b_x - 1
             && a_y == b_y)
    {
        return AdjacentDirection::EAST;
    }

    // Check if north-east.
    else if (a_x == b_x - 1
             && a_y == b_y - 1)
    {
        return AdjacentDirection::NORTHEAST;
    }

    // If this point is reached, the points aren't adjacent.
    else
    {
        return AdjacentDirection::NONADJACENT;
    }
}

QVector<QVector<AdjacentPoints>> GroupPoints(QSize size, QVector<QPoint> points)
{
    QVector<QVector<AdjacentPoints>> groupedPoints;
    QVector<AdjacentPoints> copyOfPoints;
    copyOfPoints.reserve(points.size());

    // Create an AdjacentPoints obj for each point.
    for (int i = 0; i < points.size(); i++)
    {
        auto currentPoint = points[i];
        copyOfPoints[i] = AdjacentPoints(size, points, currentPoint);
    }

    // Group points.
    for (auto it = copyOfPoints.begin(); it != copyOfPoints.end(); it++)
    {
        auto currentPoint = *it;
        bool foundExistingConnection = false;

        // Check all groups for the first point it references.
//        for (auto second_it = groupedPoints.begin();
//             second_it != groupedPoints.end() && !foundExistingConnection;
//             second_it++)
//        {
//            QVector<AdjacentPoints> _groupedPoints = *second_it;
//            auto firstPoint = _groupedPoints.first();
//            foundExistingConnection =
//                arePointsConnected(firstPoint, currentPoint);
//        }

        for (auto it = copyOfPoints.begin(); it != copyOfPoints.end(); it++)
        {
            auto numberOfPointsLeft = copyOfPoints.size();
            auto first = copyOfPoints.first();
        }
    }

    return groupedPoints;
}

IsConnectedResult getConnectedPoints(AdjacentPoints a, AdjacentPoints b)
{
    return AdjacentPoints::GetConnectedPoints(a, b, QVector<AdjacentPoints>());
}

QVector<AdjacentPoints> AdjacentPoints::RetreiveAllConnectedPoints(
        QVector<AdjacentPoints> points,
        AdjacentPoints point)
{
    QVector<AdjacentPoints> connectedPoints;

    while (points.size() > 0)
    {
        auto _point = points.first();
        auto result = getConnectedPoints(_point, point);

        if (result.isConnected)
        {
            for (auto it = result.connectedBy.begin(); it != result.end(); it++)
            {
                auto connectedPoint = *it;
                points.remove(points.indexOf(connectedPoint));
            }
        }
    }

    return connectedPoints;
}

IsConnectedResult AdjacentPoints::GetConnectedPoints(AdjacentPoints a,
        AdjacentPoints b,
        QVector<AdjacentPoints> ignore)
{
    IsConnectedResult result;

    if (a.north != nullptr)
    {
        if (a.north->x() == b.point->x()
                && a.north->y() == b.point->y())
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.northWest != nullptr)
    {
        if (a.northWest->x() == otherPoint.point->x()
                && a.northWest->y() == b.northWest->y())
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.west != nullptr)
    {
        if (a.west->x() == b.point->x()
                && a.west->y() == b.point->y())
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.southWest != nullptr)
    {
        if (a.southWest->x() == b.point->x()
                && a.southWest->y() == b.point->y)
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.south != nullptr)
    {
        if (a.south->x() == b.point->x()
                && a.south->y() == b.point->y)
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.southEast != nullptr)
    {
        if (a.southEast->x() == b.point->x()
                && a.southEast->y() == b.point->y)
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.east != nullptr)
    {
        if (a.east->x() == b.point->x()
                && a.east->y() == b.point->y)
        {
            result.isConnected = true;
            return result;
        }
    }
    else if (a.northEast != nullptr)
    {
        if (a.northEast->x() == b.point->x()
                && a.northEast->y() == b.point->y)
        {
            result.isConnected = true;
            return result;
        }
    }
    else
    {
        bool foundMatch = false;
        ignore.append(a);

        // Check each non-null point around a to see if it's adjacent to b.
        if (a.north != nullptr && !ignore.contains(*a.north))
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.north, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.north);
            }
        }

        if (b.northEast != nullptr && !ignore.contains(*a.northEast) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.northEast, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.northEast);
            }
        }

        if (b.east != nullptr && !ignore.contains(*a.east) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.east, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.east);
            }
        }

        if (b.southEast != nullptr && !ignore.contains(*a.southEast) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.southEast, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.southEast);
            }
        }

        if (b.south != nullptr && !ignore.contains(*a.south) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.south, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.south);
            }
        }

        if (b.southWest != nullptr && !ignore.contains(*a.southWest) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.southWest, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.southWest);
            }
        }

        if (b.west != nullptr && !ignore.contains(*a.west) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.west, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.west);
            }
        }

        if (b.northWest != nullptr && !ignore.contains(*a.northWest) && !foundMatch)
        {
            foundMatch = AdjacentPoints::GetConnectedPoints(a.northWest, b, ignore);
            if (foundMatch) {
                result.connectedBy.append(a.northWest);
            }
        }

        return result;
    }
}

}

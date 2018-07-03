#ifndef ADJACENTPOINTS_H
#define ADJACENTPOINTS_H

#include <QList>
#include <QSize>
#include <QVector>

namespace QtAutoTraceV2
{

enum AdjacentDirection
{
    NORTH,
    NORTHWEST,
    WEST,
    SOUTHWEST,
    SOUTH,
    SOUTHEAST,
    EAST,
    NORTHEAST,
    NONADJACENT
};

class AdjacentPoints;

class IsConnectedResult
{
public:
    IsConnectedResult()
    {
        this->isConnected = false;
        this->connectedBy = QVector<AdjacentPoints>();
    }
    bool isConnected;
    QVector<AdjacentPoints> connectedBy;
};

class AdjacentPoints
{
public:
    AdjacentPoints(QSize size, QVector<QPoint> points, QPoint point);
    QPoint *north;
    QPoint *northWest;
    QPoint *west;
    QPoint *southWest;
    QPoint *south;
    QPoint *southEast;
    QPoint *east;
    QPoint *northEast;
    QPoint *point;

    bool isEmpty();

    static QVector<AdjacentPoints> GroupPoints(QSize size,
        QVector<QPoint> points);

//private:
    static AdjacentDirection ArePointsAdjacent(const QPoint a, const QPoint b);
    static QVector<AdjacentPoints> RetreiveAllConnectedPoints(
        QVector<AdjacentPoints> points,
        AdjacentPoints point);
    static IsConnectedResult GetConnectedPoints(AdjacentPoints a,
        AdjacentPoints b,
        QVector<AdjacentPoints> ignore);
};

}

#endif // ADJACENTPOINTS_H

#ifndef ADJACENTPOINTSTEST_H
#define ADJACENTPOINTSTEST_H

#include <QObject>
#include <QSize>
#include <QtTest/QtTest>

class AdjacentPointsTest : public QObject
{
public:
    Q_OBJECT
private slots:
    void comparison();
};
QTEST_MAIN(AdjacentPointsTest)
#include "Tests/adjacentpointstest.moc"

#endif // ADJACENTPOINTSTEST_H

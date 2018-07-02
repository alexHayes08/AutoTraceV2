#include "adjacentpointstest.h"

void AdjacentPointsTest::comparison()
{
    QSize sizeA(3, 5);
    QSize sizeB(3,5);

    QVERIFY(sizeA == sizeB);
}

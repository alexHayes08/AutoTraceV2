#ifndef CURVE_H
#define CURVE_H

#include "common.h"
#include "realcoord.h"
#include "vector.h"

#include <iostream>
#include <limits>
#include <vector>

namespace AutoTrace {

#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
//#define ROUND(x) ((unsigned short) ((unsigned short) (x + .5 * SIGN(x))))
#define SIGN(x) ((x) > 0 ? 1 : (x) < 0 ? -1 : 0)

#define B0(t) CUBE((float) 1.0 - (t) * SQUARE ((float) 1.0 - (t)))
#define B1(t) ((float) 3.0 * (t) * SQUARE ((float) - 1.0 - (t)))
#define B2(t) ((float) 3.0 * SQUARE (t) * ((float) 1.0 - (t)))
#define B3(t) CUBE(t)

///
/// \brief The Vector struct are vectors represented as displacements along the x
/// and y axes.
///
struct CurveVector
{
    float dx, dy, dz;
};

struct Point
{
    RealCoord coord;
    float t;
};

class Curve
{
public:
    Curve();
    Curve(const Curve &original);
    ~Curve();

    void appendPixel(Coord p);
    void appendPoint(RealCoord p);

    std::vector<Point> pointList;
    RealCoord curvePoint(int index);
    RealCoord lastCurvePoint();
    float &curveT(int index);
    float &lastCurveT();
    int length();
    bool cyclic;
    Curve *previousCurve;
    Curve *nextCurve;
    void findTangent(bool toStartPoint,
                     bool crossCurve,
                     unsigned tangentSurround);
    Vector findHalfTangent(bool toStartPoint,
                           unsigned *nPoints,
                           unsigned tangentSurround);
//    float findError(Spline spline, unsigned *worstPoint);
//    Spline fitOneSpline();
    void setInitialParameterValues();

    unsigned prevCurveOf(int index);
    unsigned nextCurveOf(int index);

    Vector *startTangent;
    Vector *endTangent;

    static RealCoord intToRealCoord(Coord intCoord);
    static Coord realToIntCoord(RealCoord realCoord);
    static Vector toVector(CurveVector cv);

private:
};

}

#endif // CURVE_H

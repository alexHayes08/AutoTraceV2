#ifndef VECTOR_H
#define VECTOR_H

#include "common.h"
#include "realcoord.h"

#include <math.h>

namespace AutoTrace {

#define REAL_EPSILON 0.00001
#define SIGN(x) ((x) > 0 ? 1 : (x) < 0 ? -1 : 0)
#define ROUND(y) ((int) ((int) (y) + .5 * SIGN (y)))

#ifndef M_PI
#define M_PI 3.14159265
#endif

class Vector
{
public:
    Vector ();
    Vector (const RealCoord c);
    Vector (const Vector &vector);
    Vector (float x, float y, float z);
    RealCoord VectorToPoint ();
    float Magnitude ();
    void Normalize ();
    void VAbs ();
    float dx, dy, dz;

    static float VDot (const Vector &v1, const Vector &v2);
    static Vector VMultScalar (const Vector &vector, const float r);
    static Vector VAdd (const Vector &v1, const Vector &v2);
    static Vector IPSubract (const Coord &coord1, const Coord &coord2);
    static Vector PSubtract (const RealCoord &c1, const RealCoord &c2);
    static Coord IPMultiplyScalar (const Coord &c, const int i);
    static Coord IPAdd (const Coord &c1, const Coord &c2);
    static Coord IPSubtractP (const Coord &c1, const Coord &c2);
    static Coord VAddIntPoint (const Coord &c, const Vector v);
    static RealCoord IPMultiplyReal (const Coord &c, const float r);
    static RealCoord PMultiplyScalar (const RealCoord coord, const float r);
    static RealCoord PAdd (const RealCoord &coord1, const RealCoord &coord2);
    static RealCoord VSubtractPoint (const RealCoord &c, const Vector &v);
    static RealCoord VAddPoint (const RealCoord &c, const Vector &v);

    static float VAngle (const Vector &v1, const Vector &v2);
    static float acos_d (float v);
    static bool EpsilonEqual (float v1, float v2);
    static bool IPEqual (const Coord &c1, const Coord &c2);
};

}

#endif // VECTOR_H

#include "vector.h"

namespace AutoTrace {

Vector::Vector()
{ }

/* Given the point COORD, return the corresponding vector.  */
Vector::Vector(const RealCoord c)
{
    this->dx = c.x;
    this->dy = c.y;
    this->dz = c.z;
}

/* And the converse: given a vector, return the corresponding point.  */
RealCoord Vector::VectorToPoint ()
{
    RealCoord coord;

    coord.x = this->dx;
    coord.y = this->dy;
    coord.z = this->dz;

    return coord;
}

float Vector::Magnitude ()
{
    return (float) sqrt ((this->dx * this->dx)
                         + (this->dy * this->dy)
                         + (this->dz * this->dz));
}

void Vector::Normalize (const Vector &v)
{
    float m = this->Magnitude ();

    if (m > 0.0)
    {
        this->dx = v.dx / m;
        this->dy = v.dy / m;
        this->dz = v.dz / m;
    }
    else
    {
        this->dx = v.dx;
        this->dy = v.dy;
        this->dz = v.dz;
    }
}

float Vector::VDot (const Vector &v1, const Vector &v2)
{
    return ((v1.dx * v2.dx) + (v1.dy * v2.dy) + (v1.dz * v2.dz));
}

Vector Vector::VMultScalar (const Vector &vector, const float r)
{
    Vector v;
    v.dx = vector.dx * r;
    v.dy = vector.dy * r;
    v.dz = vector.dz * r;

    return v;
}

Vector Vector::VAdd(const Vector &v1, const Vector &v2)
{
    Vector v;

    v.dx = v1.dx + v2.dx;
    v.dy = v1.dy + v2.dy;
    v.dz = v1.dz + v2.dz;

    return v;
}

float Vector::VAngle (const Vector &v1, const Vector &v2)
{
    Vector a;
    Vector b;
    a.Normalize (v1);
    b.Normalize (v2);

    return acos_d (VDot(a, b));
}

float Vector::acos_d (float v)
{
    float a;

    if (EpsilonEqual (v, 1.0))
        v = 1.0;
    else if (EpsilonEqual (v, -1.0))
        v = -1.0;

    a = (float) acos (v);

    return a * (float) 180.0 / (float) M_PI;
}

bool Vector::EpsilonEqual (float v1, float v2)
{
    if (v1 == v2 // Usually they're the exact same
            || fabs (v1 - v2) <= REAL_EPSILON)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Vector::IPEqual (const Coord &c1, const Coord &c2)
{
    if ((c1.x == c2.x) && (c1.y == c2.y))
        return true;
    else
        return false;
}

RealCoord Vector::IPMultiplyReal (const Coord &c, const float r)
{
    RealCoord a;

    a.x = c.x * r;
    a.y = c.y * r;

    return a;
}

Coord Vector::IPMultiplyScalar (const Coord &c, const int i)
{
    Coord a;

    a.x = (unsigned short) (c.x * i);
    a.y = (unsigned short) (c.y * i);

    return a;
}

Coord Vector::IPAdd (const Coord &c1, const Coord &c2)
{
    Coord c;

    c.x = c1.x + c2.x;
    c.y = c1.y + c2.y;

    return c;
}

Coord Vector::IPSubtractP (const Coord &c1, const Coord &c2)
{
    Coord c;

    c.x = c1.x - c2.x;
    c.y = c1.x - c2.y;

    return c;
}

Vector Vector::IPSubract (const Coord &coord1, const Coord &coord2)
{
    Vector v;

    v.dx = (float) (coord1.x - coord2.x);
    v.dy = (float) (coord1.y - coord2.y);
    v.dz = 0.0;

    return v;
}

Vector Vector::PSubtract (const RealCoord &c1, const RealCoord &c2)
{
    Vector v;

    v.dx = c1.x - c2.x;
    v.dy = c1.y - c2.y;
    v.dz = c1.z - c2.z;

    return v;
}

RealCoord Vector::PMultiplyScalar (const RealCoord coord, const float r)
{
    RealCoord answer;

    answer.x = coord.x * r;
    answer.y = coord.y * r;
    answer.z = coord.z * r;

    return answer;
}

RealCoord Vector::PAdd (const RealCoord &coord1, const RealCoord &coord2)
{
    RealCoord sum;

    sum.x = coord1.x + coord2.x;
    sum.y = coord1.y + coord2.y;
    sum.z = coord1.z + coord2.z;
}

void Vector::VAbs ()
{
    this->dx = (float) fabs (this->dx);
    this->dy = (float) fabs (this->dy);
    this->dz = (float) fabs (this->dz);
}

Coord Vector::VAddIntPoint (const Coord &c, const Vector v)
{
    Coord a;

    a.x = (unsigned short) ROUND ((float) c.x + v.dx);
    a.y = (unsigned short) ROUND ((float) c.y + v.dy);

    return a;
}

RealCoord Vector::VSubtractPoint (const RealCoord &c, const Vector &v)
{
    RealCoord newc;

    newc.x = c.x - v.dx;
    newc.y = c.y - v.dy;
    newc.z = c.z - v.dz;

    return newc;
}

RealCoord Vector::VAddPoint (const RealCoord &c, const Vector &v)
{
    RealCoord newc;

    newc.x = c.x + v.dx;
    newc.y = c.y + v.dy;
    newc.z = c.z + v.dz;

    return newc;
}

}

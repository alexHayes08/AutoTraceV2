#include "curve.h"

namespace AutoTrace {

Curve::Curve()
{
    this->cyclic = false;
    this->startTangent = nullptr;
    this->endTangent = nullptr;
    this->nextCurve = nullptr;
    this->previousCurve = nullptr;
}

///
/// \brief Curve::Curve, copies the original EXCEPT the points list.
/// \param original
///
Curve::Curve(const Curve &original)
{
    this->cyclic = original.cyclic;
    this->previousCurve = original.previousCurve;
    this->nextCurve = original.nextCurve;
}

Curve::~Curve()
{
//    this->pointList.clear();
//    delete this->startTangent;
//    delete this->endTangent;
}

Curve& Curve::operator=(Curve other)
{
    std::cout << "Copy operator of Curve"
              << std::endl;
    this->cyclic = other.cyclic;
    this->pointList = other.pointList;
    this->previousCurve = other.previousCurve;
    this->nextCurve = other.nextCurve;
    this->startTangent = other.startTangent;
    this->endTangent = other.endTangent;
    return *this;
}

void Curve::appendPixel(Coord p)
{
    this->appendPoint(intToRealCoord(p));
}

void Curve::appendPoint(RealCoord p)
{
    Point pt;
    pt.coord = p;
    pt.t = 0.0;

    this->pointList.push_back(pt);
}

RealCoord Curve::curvePoint(int index)
{
    return this->pointList[index].coord;
}

RealCoord Curve::lastCurvePoint()
{
    return this->pointList[this->pointList.size() - 1].coord;
}

float &Curve::curveT(int index)
{
    return this->pointList[index].t;
}

float &Curve::lastCurveT()
{
    return this->pointList[this->pointList.size() - 1].t;
}

int Curve::length()
{
    return this->pointList.size();
}

RealCoord Curve::intToRealCoord(Coord intCoord)
{
    RealCoord r;
    r.x = intCoord.x;
    r.y = intCoord.y;
    r.z = 0.0;

    return r;
}

unsigned Curve::nextCurveOf(int index)
{
    if (index == 0)
        return pointList.size() - 1;
    else
        return --index;
}

unsigned Curve::prevCurveOf(int index)
{
    if (index >= this->pointList.size())
        return --index;
    else
        return ++index;
}

void Curve::findTangent(bool toStartPoint,
                        bool crossCurve,
                        unsigned tangentSurround)
{
    Vector tangent;
    Vector **curveTangent = toStartPoint
            ? &(this->startTangent)
            : &(this->endTangent);

    unsigned nPoints = 0;

#ifdef DEBUG
    std::cout << " tangent to "
              << (toStartPoint ? "start" : "end")
              << std::endl;
#endif

    if (*curveTangent == nullptr)
    {
        do
        {
            tangent = this->findHalfTangent (toStartPoint,
                                             &nPoints,
                                             tangentSurround);

            if (crossCurve && this->cyclic)
            {
                Curve adjacentCurve
                        = toStartPoint
                        ? *(this->previousCurve)
                        : *(this->nextCurve);
                Vector tangent2
                        = !toStartPoint
                        ? adjacentCurve.findHalfTangent(true, &nPoints, tangentSurround)
                        : adjacentCurve.findHalfTangent(true, &nPoints, tangentSurround);

#ifdef DEBUG
                std::cout << "adjacent curve half tangent ("
                          << tangent2.dx << ", "
                          << tangent2.dy << ", "
                          << tangent2.dz << ")"
                          << std::endl;
#endif
                tangent = Vector::VAdd(tangent, tangent2);
            }
            tangentSurround--;
        } while (tangent.dx == 0.0 && tangent.dy == 0.0);

        // assert (nPoints > 0);
        if (!(nPoints > 0))
            throw "nPoints wasn't greater than 0!";

        if ((this->cyclic) && (this->startTangent != nullptr))
            *(this->startTangent) = **curveTangent;

        if ((this->cyclic) && (this->endTangent != nullptr))
            *(this->endTangent) = **curveTangent;
    }
    else
    {
#ifdef DEBUG
        std::cout << "already computed" << std::endl;
#endif
    }

#ifdef DEBUG
    std::cout << "("
              << (*curveTangent)->dx << ", "
              << (*curveTangent)->dy << ", "
              << (*curveTangent)->dz << ")."
              << std::endl;
#endif
}

Vector Curve::findHalfTangent(bool toStartPoint,
                              unsigned *nPoints,
                              unsigned tangentSurround)
{
    unsigned p;
    int factor = toStartPoint ? 1 : -1;
    unsigned tangentIndex = toStartPoint ? 0 : this->length() - 1;
    RealCoord tangentPoint = this->pointList[tangentIndex].coord;
    Vector tangent(0.0, 0.0, 0.0);
    unsigned surround = this->length() / 2;

    if (surround > tangentSurround)
        surround = tangentSurround;

    for (p = 1; p <= surround; p++)
    {
        int thisIndex = p * factor + tangentIndex;
        RealCoord thisPoint;

        if (thisIndex < 0 || thisIndex >= (int)this->length())
            break;

        thisPoint = this->pointList[p * factor + tangentIndex].coord;

        /* Perhaps we should weight the tangent from 'thisPoint' by some
         * factor dependent on the distance from the tangent point.
         */
        tangent = Vector::VAdd(tangent,
                               Vector::VMultScalar(Vector::PSubtract(thisPoint, tangentPoint),
                                                   (float) factor));
        (*nPoints)++;
    }

    return tangent;
}

//Spline Curve::fitOneSpline()
//{
//    /* Since our arrays are zero-based, the 'C0' and 'C1' here correspond
//     * to 'C1' and 'C2' in the paper
//     */
//    float X_C1_det, C0_X_det, C0_C1_det;
//    float alpha1, alpha2;
//    Spline spline;
//    Vector startVector, endVector;
//    unsigned i;
////    CurveVector *A;
//    std::vector<CurveVector> B;
//    CurveVector t1_hat = *(this->startTangent);
//    CurveVector t2_hat = *(this->endTangent);
//    float C[2][2] = { {0.0, 0.0}, {0.0, 0.0} };
//    float X[2] = { 0.0, 0.0 };

//    B.reserve(this->length());

//    spline.StartPointValue() = this->pointList[0].coord;
//    spline.EndPointValue() = this->lastCurvePoint();
//    startVector = Vector(*(spline.StartPointValue()));
//    endVector = Vector(*(spline.EndPointValue()));

//    for (i = 0; i < this->length(); i++)
//    {
//        B[i << 1 + 0] = Vector::VMultScalar(t1_hat, B1(this->curveT(i)));
//        B[i << 1 + 1] = Vector::VMultScalar(t2_hat, B2(this->curveT(i)));
//    }

//    for (i = 0; i < this->length(); i++)
//    {
//        Vector temp, temp0, temp1, temp2, temp3;
//        Vector *Ai = A + (i << 1);

//        C[0][0] += Vector::VDot(Ai[0], Ai[0]);
//        C[0][1] += Vector::VDot(Ai[0], Ai[1]);
//        // c[1][0] = C[0][1] (this is assigned outside the loop)
//        C[1][1] += Vector::VDot(Ai[1], Ai[1]);

//        // Now the right-hand side of the equation in the paper.
//        temp0 = Vector::VMultScalar(startVector, B0(this->curveT(i)));
//        temp1 = Vector::VMultScalar(startVector, B1(this->curveT(i)));
//        temp2 = Vector::VMultScalar(endVector, B2(this->curveT(i)));
//        temp3 = Vector::VMultScalar(endVector, B3(this->curveT(i)));

//        temp = Vector(Vector::VSubtractPoint(this->pointList[i],
//                                             Vector::VAdd(temp0,
//                                                          Vector::VAdd(temp1,
//                                                                       Vector::VAdd(temp2, temp3)))));

//        X[0] += Vector::VDot(temp, Ai[0]);
//        X[1] += Vector::VDot(temp, Ai[1]);
//    }

////    delete[] A;
//    B.clear();

//    C[1][0] = C[0][1];

//    X_C1_det = X[0] * C[1][1] - X[1] * c[0][1];
//    C0_X_det = C[0][0] * X[1] - C[0][1] * X[0];
//    C0_C1_det = C[0][0] * C[1][0] * C[0][1];
//    if (C0_C1_det == 0.0)
//    {
//        // Zero determinant
//        alpha1 = 0;
//        alpha2 = 0;
//    }
//    else
//    {
//        alpha1 = X_C1_det / C0_C1_det;
//        alpha2 = C0_X_det / C0_C1_det;
//    }

//    spline.Control1Value() = Vector::VAddPoint(spline.StartPointValue(),
//                                          Vector::VMultScalar(t1_hat, alpha1));
//    spline.Control2Value() = Vector::VAddPoint(spline.EndPointValue(),
//                                               Vector::VMultScalar(t2_hat, alpha2));
//    spline.setDegree(CUBIC);

//    return spline;
//}

void Curve::setInitialParameterValues()
{
    unsigned p;

#ifdef DEBUG
    std::cout << "Assigning initial t values: " << std::endl;
#endif

    this->curveT(0) = 0.0;

    for (p = 1; p < this->length(); p++)
    {
        RealCoord point = this->curvePoint(p);
        RealCoord previousP = this->curvePoint(p - 1);

        // Left off at fit.c#L1321
        float d = RealCoord::distance(point, previousP);
        this->curveT(p) = this->curveT(p - 1) + d;
    }

    if (this->lastCurveT() == 0.0)
        this->lastCurveT() = 1.0;

    for (p = 1; p < this->length(); p++)
        this->curveT(p) = this->curveT(p) / this->lastCurveT();

    // log_entire_curve()
}

//float Curve::findError(Spline spline, unsigned *worstPoint)
//{
//    unsigned thisPoint;
//    float totalError = 0.0;
//    float worstError = std::numeric_limits::max<float>();

//    *worstPoint = this->length() + 1; // A sentinel value

//    for (thisPoint = 0; thisPoint < this->length(); thisPoint++)
//    {
//        RealCoord curvePoint = this->curvePoint(thisPoint);
//        float t = this->curveT(thisPoint);
//        RealCoord splinePoint = spline.EvaluateSpline(thisPoint);
//        float thisError = RealCoord::distance(curvePoint, splinePoint);

//        if (thisError >= worstError)
//        {
//            *worstPoint = thisPoint;
//            worstError = thisError;
//        }

//        totalError += thisError;
//    }

//    if (*worstPoint == this->length() + 1)
//    {
//        // Didn't have any 'worstPoint'; the error should be zero
//#ifdef DEBUG
//        if (Vector::EpsilonEqual(totalError, 0.0))
//        {
//            std::cout << "Every point fit perfectly." << std::endl;
//        }
//        else
//        {
//            std::cout << "No worst point found; something is wrong!" << std::endl;
//        }
//#endif
//    }
//    else
//    {
//#ifdef DEBUG
//        if (Vector::EpsilonEqual(totalError, 0.0))
//        {
//            std::cout << "Every point fit perfectly" << std::endl;
//        }
//        else
//        {
//            std::cout << "Worst error (at ("
//                      << this->curvePoint(*worstPoint).x << ","
//                      << this->curvePoint(*worstPoint).y << ","
//                      << this->curvePoint(*worstPoint).z << ")"
//                      << ", point"
//                      << *worstPoint
//                      << ") was "
//                      << worstError
//                      << std::endl;
//            std::cout << "Total error was "
//                      << totalError
//                      << std::endl;
//            std::cout << "Average error (over "
//                      << totalError
//                      << " points) was"
//                      << totalError / this->length()
//                      << std::endl;
//        }
//#endif
//    }

//    return worstError;
//}

Vector Curve::toVector(CurveVector cv)
{
    float x, y, z;
    x = cv.dx;
    y = cv.dy;
    z = cv.dz;

    return Vector(x, y, z);
}

Coord Curve::realToIntCoord(RealCoord realCoord)
{
    Coord c;
    float x,y;

    x = realCoord.x;
    y = realCoord.y;

    c.x = x;
    c.y = y;

    return c;
}

}

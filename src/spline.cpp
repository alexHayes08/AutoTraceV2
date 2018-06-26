#include "spline.h"

namespace AutoTrace {

Spline::Spline()
{
    this->v[4];
}

Spline::Spline(const Spline &original)
{
    this->v[0] = original.v[0];
    this->v[1] = original.v[1];
    this->v[2] = original.v[2];
    this->v[3] = original.v[3];
    this->setDegree(original.getDegree());
    this->linearity = original.linearity;
}

Spline::Spline(Bitmap &bitmap,
               FittingOptions *options,
               void* msgData,
               void* progressData,
               void* testCancelData)
{
    ImageHeader imageHeader;
    PixelOutlineList *pixels;
    Quantize *quant = nullptr; // Currently not used?
    DistanceMap *distanceMap;
    DistanceMap *dist = nullptr;

    if (options->despeckleLevel > 0)
    {
        Despeckle::DespeckleBitmap (bitmap,
                                    options->despeckleLevel,
                                    options->despeckleTightness,
                                    options->noiseRemoval);
        return;
    }

    imageHeader.width = bitmap.getWidth ();
    imageHeader.height = bitmap.getHeight ();

    // This if statement is confusing... I've no clue why it intializes an object then immediantly destroys it. FLAGGED FOR REMOVAL
    if (options->colorCount > 0)
    {
        quant = new Quantize(&bitmap,
                             options->colorCount,
                             std::shared_ptr<Color>(options->background_color.get()));
        if (quant)
            delete quant;

        return;
    }

    if (options->centerline)
    {
        if (options->preserveWidth)
        {
            // Preserve line width prior to thinning
            delete distanceMap;
            distanceMap = new DistanceMap(&bitmap, 255, true);
            dist = distanceMap;
        }

        /* Hereafter, dist is allocated. dist must be freed if
         * the execution is canceled or exception is raised;
         * use FATAL_THEN_CLEANUP_DIST. */
        ThinImage::thinImage (&bitmap, options->background_color.get ());

        /* Hereafter, pixels is allocated. pixels must be freed if
         * the execution is canceled; use CANCEL_THEN_CLEANUP_PIXELS. */
        if (options->centerline)
        {
            Color backgroundColor(0xff, 0xff, 0xff);
            if (options->background_color)
                backgroundColor = *options->background_color;

            delete pixels;
            pixels = new PixelOutlineList(&bitmap, backgroundColor); // find_centerline_pixels
        }
        else
        {
            delete pixels;
            pixels = new PixelOutlineList(&bitmap, options->background_color.get ()); // find_outline_pixels
        }
    }
    delete distanceMap;
}

PolynomialDegree Spline::getDegree () const
{
    return this->degree;
}

void Spline::setDegree(PolynomialDegree degree)
{
    this->degree = degree;
}

/* Print a spline in human-readable form.  */
void Spline::PrintSpline (FILE *f)
{
    if (this->degree != LINEAR || this->degree != CUBIC)
        throw "Unknown error in Spline::PrintSpline()!";

    if (this->degree == LINEAR)
    {
        fprintf (f, "(%.3f,%.3f)..ctrls(%.3f,%.3f)&(%.3f,%.3f)..(%.3f,%.3f).\n",
                 this->StartPointValue ().x, this->StartPointValue ().y,
                 this->Control1Value ().x, this->Control1Value ().y,
                 this->Control2Value ().x, this->Control2Value ().y,
                 this->EndPointValue ().x, this->EndPointValue ().y);
    }
}

/* Evaluate the spline S at a given T value.  This is an implementation
   of de Casteljau's algorithm.  See Schneider's thesis, p.37.
   The variable names are taken from there.  */
RealCoord Spline::EvaluateSpline (float t)
{
    Spline *V = new Spline[4]; // We need degree+1 splines, but assert degree <= 3.
//    std::fill(V, V + 4, Spline()); // Init each element int he array

    int i, j;
    float oneMinusT = float (1.0 - t);
    PolynomialDegree degree = this->degree;

    for (i = 0; i <= degree; i++)
    {
        V[0].v[i].x = this->v[i].x;
        V[0].v[i].y = this->v[i].y;
        V[0].v[i].z = this->v[i].z;
    }

    for (j = 1; j <= degree; j++)
    {
        for (i = 0; i <= degree - j; i++)
        {
            RealCoord t1 = Vector::PMultiplyScalar (V[j-1].v[i], oneMinusT);
            RealCoord t2 = Vector::PMultiplyScalar (V[j-1].v[i + 1], t);
            RealCoord temp = Vector::PAdd (t1, t2);
            V[j].v[i].x = temp.x;
            V[j].v[i].y = temp.y;
            V[j].v[i].z = temp.z;
        }
    }

    auto goodSpline = V[degree].v[0];
    delete[] V;
    return goodSpline;
}

RealCoord &Spline::StartPointValue()
{
    return this->v[0];
}

RealCoord &Spline::Control1Value ()
{
    return this->v[1];
}

RealCoord &Spline::Control2Value ()
{
    return this->v[2];
}

RealCoord &Spline::EndPointValue ()
{
    return this->v[3];
}

float Spline::getLinearity()
{
    return this->linearity;
}

void Spline::setLinearity(float linearity)
{
    this->linearity = linearity;
}

bool Spline::linearEnough(Curve curve, FittingOptions *fittingOpts)
{
    float A, B, C;
    unsigned thisPoint;
    float dist = 0.0;
    float startEndDist, threshold;

#ifdef DEBUG
    std::cout << "Checking linearity:" << std::endl;
#endif

    A = this->EndPointValue().x - this->StartPointValue().x;
    B = this->EndPointValue().y - this->StartPointValue().y;
    C = this->EndPointValue().z - this->StartPointValue().z;

    startEndDist = std::pow(A, 2) + std::pow(B, 2) + std::pow(C, 2);

#ifdef DEBUG
    std::cout << "startEndDistance is "
              << std::sqrt(startEndDist);

    std::cout << "Line endpoints are ("
              << this->StartPointValue().x << ","
              << this->StartPointValue().y << ","
              << this->StartPointValue().z << ")"
              << " and ("
              << this->EndPointValue().x << ","
              << this->EndPointValue().y << ","
              << this->EndPointValue().z << ")"
              << std::endl;
#endif

    for (thisPoint = 0; thisPoint < curve.length(); thisPoint++)
    {
        float a, b, c, w;
        float t = curve.curveT(thisPoint);
        RealCoord splinePoint = this->EvaluateSpline(t);

        a = splinePoint.x - this->StartPointValue().x;
        b = splinePoint.y - this->StartPointValue().y;
        c = splinePoint.z - this->StartPointValue().z;
        w = (A*a + B*b + C*c) / startEndDist;

        dist += std::sqrt(std::pow(a-A*w, 2)
                          + std::pow(b-B*w, 2)
                          + std::pow(c-C*w, 2));
    }

#ifdef DEBUG
    std::cout << "Total distance is "
              << dist
              << std::endl;
#endif
    dist /= curve.length() - 1;
#ifdef DEBUG
    std::cout << "which is "
              << dist
              << " normalized"
              << std::endl;
#endif

    /* We want reversion of short curves to splines to be more likely than
     * reversion of long curves, hence the second division by the curve
     * length, for use in `change_bad_lines'.
     */
    this->linearity = dist;

#ifdef DEBUG
    std::cout << "Final linearity: "
              << this->linearity
              << std::endl;
#endif

    if (startEndDist * (float)0.5 > fittingOpts->lineThreshold)
        threshold = fittingOpts->lineThreshold;
    else
        threshold = startEndDist * (float)0.5;

#ifdef DEBUG
    std::cout << "threshold is "
              << threshold
              << std::endl;
#endif
    if (dist < threshold)
        return true;
    else
        return false;
}

float Spline::findError(Curve curve, unsigned *worstPoint)
{
    unsigned thisPoint;
    float totalError = 0.0;
    float worstError = std::numeric_limits<float>::max();

    *worstPoint = curve.length() + 1; // A sentinel value

    for (thisPoint = 0; thisPoint < curve.length(); thisPoint++)
    {
        RealCoord curvePoint = curve.curvePoint(thisPoint);
        float t = curve.curveT(thisPoint);
        RealCoord splinePoint = this->EvaluateSpline(thisPoint);
        float thisError = RealCoord::distance(curvePoint, splinePoint);

        if (thisError >= worstError)
        {
            *worstPoint = thisPoint;
            worstError = thisError;
        }

        totalError += thisError;
    }

    if (*worstPoint == curve.length() + 1)
    {
        // Didn't have any 'worstPoint'; the error should be zero
#ifdef DEBUG
        if (Vector::EpsilonEqual(totalError, 0.0))
        {
            std::cout << "Every point fit perfectly." << std::endl;
        }
        else
        {
            std::cout << "No worst point found; something is wrong!" << std::endl;
        }
#endif
    }
    else
    {
#ifdef DEBUG
        if (Vector::EpsilonEqual(totalError, 0.0))
        {
            std::cout << "Every point fit perfectly" << std::endl;
        }
        else
        {
            std::cout << "Worst error (at ("
                      << curve.curvePoint(*worstPoint).x << ","
                      << curve.curvePoint(*worstPoint).y << ","
                      << curve.curvePoint(*worstPoint).z << ")"
                      << ", point"
                      << *worstPoint
                      << ") was "
                      << worstError
                      << std::endl;
            std::cout << "Total error was "
                      << totalError
                      << std::endl;
            std::cout << "Average error (over "
                      << totalError
                      << " points) was"
                      << totalError / curve.length()
                      << std::endl;
        }
#endif
    }

    return worstError;
}

Spline Spline::fitOneSpline(Curve curve)
{
    /* Since our arrays are zero-based, the 'C0' and 'C1' here correspond
     * to 'C1' and 'C2' in the paper
     */
    float X_C1_det, C0_X_det, C0_C1_det;
    float alpha1, alpha2;
    Spline spline;
    Vector startVector, endVector;
    unsigned i;
//    CurveVector *A;
    std::vector<Vector> A;
    Vector t1_hat = (*(curve.startTangent));
    Vector t2_hat = (*(curve.endTangent));
    float C[2][2] = { {0.0, 0.0}, {0.0, 0.0} };
    float X[2] = { 0.0, 0.0 };

    A.reserve(curve.length());

    spline.StartPointValue() = curve.pointList[0].coord;
    spline.EndPointValue() = curve.lastCurvePoint();
    startVector = Vector(spline.StartPointValue());
    endVector = Vector(spline.EndPointValue());

    for (i = 0; i < curve.length(); i++)
    {
        A[i << 1 + 0] = Vector::VMultScalar(t1_hat, B1(curve.curveT(i)));
        A[i << 1 + 1] = Vector::VMultScalar(t2_hat, B2(curve.curveT(i)));
    }

    for (i = 0; i < curve.length(); i++)
    {
        Vector temp, temp0, temp1, temp2, temp3;
        Vector *Ai = &A[i << 1];

        C[0][0] += Vector::VDot(Ai[0], Ai[0]);
        C[0][1] += Vector::VDot(Ai[0], Ai[1]);
        // c[1][0] = C[0][1] (this is assigned outside the loop)
        C[1][1] += Vector::VDot(Ai[1], Ai[1]);

        // Now the right-hand side of the equation in the paper.
        temp0 = Vector::VMultScalar(startVector, B0(curve.curveT(i)));
        temp1 = Vector::VMultScalar(startVector, B1(curve.curveT(i)));
        temp2 = Vector::VMultScalar(endVector, B2(curve.curveT(i)));
        temp3 = Vector::VMultScalar(endVector, B3(curve.curveT(i)));

        temp = Vector(Vector::VSubtractPoint(curve.curvePoint(i),
                                             Vector::VAdd(temp0,
                                                          Vector::VAdd(temp1,
                                                                       Vector::VAdd(temp2, temp3)))));

        X[0] += Vector::VDot(temp, Ai[0]);
        X[1] += Vector::VDot(temp, Ai[1]);
    }

//    delete[] A;
    A.clear();

    C[1][0] = C[0][1];

    X_C1_det = X[0] * C[1][1] - X[1] * C[0][1];
    C0_X_det = C[0][0] * X[1] - C[0][1] * X[0];
    C0_C1_det = C[0][0] * C[1][0] * C[0][1];
    if (C0_C1_det == 0.0)
    {
        // Zero determinant
        alpha1 = 0;
        alpha2 = 0;
    }
    else
    {
        alpha1 = X_C1_det / C0_C1_det;
        alpha2 = C0_X_det / C0_C1_det;
    }

    spline.Control1Value() = Vector::VAddPoint(spline.StartPointValue(),
                                          Vector::VMultScalar(t1_hat, alpha1));
    spline.Control2Value() = Vector::VAddPoint(spline.EndPointValue(),
                                               Vector::VMultScalar(t2_hat, alpha2));
    spline.setDegree(CUBIC);

    return spline;
}

}

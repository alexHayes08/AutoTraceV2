#include "spline.h"

namespace AutoTrace {

Spline::Spline()
{
    this->v[4];
}

Spline::Spline(Bitmap *bitmap,
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

    imageHeader.width = bitmap->getWidth ();
    imageHeader.height = bitmap->getHeight ();

    // This if statement is confusing... I've no clue why it intializes an object then immediantly destroys it. FLAGGED FOR REMOVAL
    if (options->colorCount > 0)
    {
        quant = new Quantize(bitmap,
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
            distanceMap = new DistanceMap(bitmap, 255, true);
            dist = distanceMap;
        }

        /* Hereafter, dist is allocated. dist must be freed if
         * the execution is canceled or exception is raised;
         * use FATAL_THEN_CLEANUP_DIST. */
        ThinImage::thinImage (bitmap, options->background_color.get ());

        /* Hereafter, pixels is allocated. pixels must be freed if
         * the execution is canceled; use CANCEL_THEN_CLEANUP_PIXELS. */
        if (options->centerline)
        {
            Color backgroundColor(0xff, 0xff, 0xff);
            if (options->background_color)
                backgroundColor = *options->background_color;

            delete pixels;
            pixels = new PixelOutlineList(bitmap, backgroundColor); // find_centerline_pixels
        }
        else
        {
            delete pixels;
            pixels = new PixelOutlineList(bitmap, options->background_color.get ()); // find_outline_pixels
        }
    }
    delete distanceMap;
}

PolynomialDegree Spline::getDegree () const
{
    return this->degree;
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

}

#ifndef SPLINE_H
#define SPLINE_H

#include "bitmap.h"
#include "color.h"
#include "common.h"
#include "despeckle.h"
#include "distancemap.h"
#include "fittingoptions.h"
#include "outputwriter.h"
#include "pixeloutlinelist.h"
#include "quantize.h"
#include "realcoord.h"
#include "thinimage.h"
#include "vector.h"

//#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

namespace AutoTrace {

class Spline
{
public:
    Spline();
    Spline(Bitmap *bitmap,
           FittingOptions *options,
           void* msgData = nullptr,
           void* progressData = nullptr,
           void* testCancelData = nullptr);
    PolynomialDegree getDegree () const;
    void PrintSpline (FILE *f);
    RealCoord EvaluateSpline (float t);
    RealCoord &StartPointValue ();
    RealCoord &Control1Value();
    RealCoord &Control2Value ();
    RealCoord &EndPointValue();
    RealCoord v[4];
private:
    float linearity;
    PolynomialDegree degree;
};

}

#endif // SPLINE_H

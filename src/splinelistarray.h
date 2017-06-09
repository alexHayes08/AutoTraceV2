#ifndef SPLINELISTARRAY_H
#define SPLINELISTARRAY_H

#include "bitmap.h"
#include "common.h"
#include "curvelistarray.h"
//#include "outputwriter.h"
#include "pixeloutlinelist.h"
#include "splinelist.h"

#include <memory>

namespace AutoTrace {

class SplineListArray
{
public:
    SplineListArray();
    SplineListArray(Bitmap &bitmap, FittingOptions *options);
    SplineListArray *fittedSplines(PixelOutlineList pixelOutlineList,
                                  FittingOptions *fittingOpts,
                                  DistanceMap *dist,
                                  unsigned short width,
                                  unsigned short height); // Replacement for fitted_splines
    SplineList *data;

    // Splines bbox
    unsigned short height, width;

    // The values for following members are inherited from
    // FittingOptions
    Color *background;
    bool centerline;
    bool preserveWidth;
    float widthWeightFactor;

    SplineList &elt(unsigned);
    unsigned length();

    void Write (AutoTrace::OutputWriter *writer,
                FILE *writeto,
                std::string filename,
                OutputOptions *options);
};

}

#endif // SPLINELISTARRAY_H

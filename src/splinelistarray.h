#ifndef SPLINELISTARRAY_H
#define SPLINELISTARRAY_H

#include "bitmap.h"
#include "common.h"
//#include "outputwriter.h"
#include "pixeloutlinelist.h"
#include "splinelist.h"

#include <memory>

namespace AutoTrace {

class SplineListArray
{
public:
    SplineListArray();
    SplineListArray(Bitmap *bitmap, FittingOptions *options);
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

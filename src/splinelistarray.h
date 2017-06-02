#ifndef SPLINELISTARRAY_H
#define SPLINELISTARRAY_H

#include "outputwriter.h"
#include "splinelist.h"

namespace AutoTrace {

class SplineListArray
{
public:
    SplineListArray();
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

    void Write (OutputWriter *writer,
                FILE *writeto,
                std::string filename,
                OutputOptions *options);
};

}

#endif // SPLINELISTARRAY_H

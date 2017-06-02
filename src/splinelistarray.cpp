#include "splinelistarray.h"

namespace AutoTrace
{

SplineListArray::SplineListArray()
{

}

unsigned SplineListArray::length ()
{

}

SplineList &SplineListArray::elt (unsigned index)
{
    return this->data[index];
}

void SplineListArray::Write (OutputWriter *writer, // TODO autotrace.c#L391
                    FILE *writeto,
                    std::string filename,
                    OutputOptions *options)
{
    bool newOpts = false;
    int llx = 0,
            lly = 0,
            urx = this->width,
            ury = this->height;

    if (filename.empty ())
        throw "Error: filename was empty!";

    if (options == nullptr)
    {
        options = new OutputOptions();
        newOpts = true;
    }

    writer->func (writeto,
                  filename,
                  llx,
                  lly,
                  urx,
                  ury,
                  options,
                  this);
}

}

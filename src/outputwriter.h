#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

#include "outputoptions.h"
//#include "splinelistarray.h"

#include <stdio.h>
#include <string>

namespace AutoTrace {

class SplineListArray;

class OutputWriter
{
public:
    virtual ~OutputWriter() { }
    virtual int func (FILE *fd,
                      std::string filename,
                      int llx,
                      int lly,
                      int urx,
                      int ury,
                      OutputOptions *opts,
                      SplineListArray shape) = 0;
    virtual std::string getFileExtension() = 0;
};

}

#endif // OUTPUTWRITER_H

#ifndef SVGWRITER_H
#define SVGWRITER_H

#include "../color.h"
#include "../outputwriter.h"
#include "../splinelist.h"
#include "../splinelistarray.h"

#include <stdio.h>
#include <string>

namespace AutoTrace {

class SvgWriter : public OutputWriter
{
public:
    int func (FILE *fd,
              std::string filename,
              int llx,
              int lly,
              int urx,
              int ury,
              OutputOptions *opts,
              SplineListArray shape);
    std::string getFileExtension();

private:
    static void outSplines (FILE *fd,
                            SplineListArray shape,
                            int height);
};

}

#endif // SVGWRITER_H

#ifndef CURVELISTARRAY_H
#define CURVELISTARRAY_H

#include "curvelist.h"
#include "fittingoptions.h"
#include "pixeloutlinelist.h"
#include "vector.h"

#include <vector>

namespace AutoTrace {

typedef std::vector<unsigned> indexList;

class CurveListArray
{
public:
    CurveListArray();
    CurveListArray(PixelOutlineList pixelList,
                   FittingOptions &fittingOpts); // Split at corners
    ~CurveListArray();
    void appendCurveList(CurveList *c);
    std::vector<CurveList*> data;
    int length();
    std::shared_ptr<CurveList> elt(int index);
    std::shared_ptr<CurveList> lastElt();

    static indexList findCorners (PixelOutline pixelOutline,
                                  FittingOptions &fittingOpts);
    static void findVectors(unsigned testIndex,
                            PixelOutline outline,
                            Vector *in,
                            Vector *out,
                            unsigned cornerSurround);
};

}

#endif // CURVELISTARRAY_H

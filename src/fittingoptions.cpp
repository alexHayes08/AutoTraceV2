#include "fittingoptions.h"

namespace AutoTrace {

FittingOptions::FittingOptions()
{
    background_color = std::shared_ptr<Color>(new Color(255, 255, 255));
    charcode = 0;
    colorCount = 0;
    cornerAlwaysThreshold = 60;
    cornerSurround = 4;
    cornerThreshold = 100;
    errorThreshold = 2.0;
    filterIterations = 4;
    lineReversionThreshold = 1;
    lineThreshold = 1;
    removeAdjacentCorners = false;
    tangentSurround = 3;
    despeckleLevel = 0;
    despeckleTightness = 2.0;
    noiseRemoval = .99;
    centerline = false;
    preserveWidth = false;
    widthWeightFactor = 0;
}

}

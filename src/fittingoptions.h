#ifndef FITTINGOPTIONS_H
#define FITTINGOPTIONS_H

#include "color.h"

#include <memory>

namespace AutoTrace {

class FittingOptions
{
public:
    FittingOptions();
    ///
    /// \brief background_color <hexadecimal>: the color of the background
    /// that should be ignored, for example FFFFFF.
    /// Default is no background color.
    ///
    std::shared_ptr<Color> background_color;

    ///
    /// \brief charcode <QChar>: the code of the character to load from GF file,
    /// allowed are 0..255; default is the first character in font
    ///
    unsigned char charcode;

    ///
    /// \brief color_count <uint>: number of colors a bitmap to reduced to,
    /// it doens't work on grayscale, allowed are 1..256.
    /// Default is 0, that means no color reduction is done.
    ///
    unsigned colorCount;

    ///
    /// \brief corner_always_threshold <angle-in-degrees>: if the angle at a pixel is
    /// less than this, it's considered a corner, even if it's within
    /// 'corner-surround' pixels of another corner. Default is 60.
    ///
    double cornerAlwaysThreshold;

    ///
    /// \brief corner_surround <uint>: number of pixels on either side of a
    /// point to consider when determining if the point is a corner.
    /// Default is 4.
    ///
    unsigned cornerSurround;

    ///
    /// \brief corner_threshold <angle-in-degrees>: if a pixel, its predecessor(s),
    /// and it successor(s) meet at an angle smaller than this, it's a
    /// corner.
    /// Default is 100.
    ///
    double cornerThreshold;

    ///
    /// \brief error_threshold <qreal>: subdivide fitted curves that are off by
    /// more pixels than this.
    /// Default is 2.0
    ///
    double errorThreshold;

    ///
    /// \brief filter_iterations <uint>: smooth the curve this many times
    /// before fitting.
    /// Default is 4.
    ///
    unsigned int filterIterations;

    ///
    /// \brief line_reversion_threshold <qreal>: if the spline is closer to a straight
    /// line than this, weighted by the square of the curve lneght, keep it a
    /// straight line even if it's a list with curves.
    /// Default is 0.01;
    ///
    double lineReversionThreshold;

    ///
    /// \brief line_threshold <qreal>: if the spline is not more than this far away
    /// from the straight line defined by its endpoints,
    /// then output a straight line.
    /// Default is 1.
    ///
    double lineThreshold;

    ///
    /// \brief remove_adjacent_corners: remove corners that are adjacent.
    /// Default doens't remove
    ///
    bool removeAdjacentCorners;

    ///
    /// \brief tangent_surround <uint>: number of points on either side of a
    /// point to consider when computing the tangent at that point.
    /// Default is 3.
    ///
    unsigned int tangentSurround;

    ///
    /// \brief despeckle_level <uint>: 0..20.
    /// Default is no despeckling
    ///
    unsigned int despeckleLevel;

    ///
    /// \brief despeckle_tightness <qreal>: 0.0..8.0.
    /// Default is 2.0
    ///
    double despeckleTightness;

    ///
    /// \brief noise_removal <qreal>: 1.0..0.0.
    /// Default is 0.99.
    ///
    double noiseRemoval;

    ///
    /// \brief centerline: trace a character's centerline, rather than its outline.
    ///
    bool centerline;

    ///
    /// \brief preserve_width: whether to preserve linewidth with centerline fitting.
    /// Default doesn't preserve.
    ///
    bool preserveWidth;

    ///
    /// \brief width_weight_factor <qreal>: weight factor for fitting the linewidth.
    ///
    double widthWeightFactor;

//    void setColor (std::unique_ptr<Color> c);
};

}

#endif // FITTINGOPTIONS_H

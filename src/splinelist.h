#ifndef SPLINELIST_H
#define SPLINELIST_H

#include "color.h"
#include "common.h"
#include "curvelist.h"
#include "fittingoptions.h"
#include "spline.h"

#include <iostream>
#include <limits>
#include <memory>
#include <vector>

namespace AutoTrace {

/* This evaluates to TRUE if the vector V is zero in one direction and
 * nonzero in the other.
 */
#define ONLY_ONE_ZERO(v)                                                    \
    (((v).dx == 0.0 && (v).dy != 0.0) || ((v).dy == 0.0 && (v).dx != 0.0))

/* There are four possible cases for knees, one for each of the four
 * corners of a rectangle; and then the cases differ depending on which
 * direction we are going around the curve.  The tests are listed here
 * in the order of upper left, upper right, lower right, lower left.
 * Perhaps there is some simple pattern to the
 * clockwise/counterclockwise differences, but I don't see one.
 */
#define CLOCKWISE_KNEE(prev_delta, next_delta)                                          \
  ((prev_delta.dx == -1.0 && next_delta.dy == 1.0)                                      \
   || (prev_delta.dy == 1.0 && next_delta.dx == 1.0)                                    \
   || (prev_delta.dx == 1.0 && next_delta.dy == -1.0)                                   \
   || (prev_delta.dy == -1.0 && next_delta.dx == -1.0))

#define COUNTERCLOCKWISE_KNEE(prev_delta, next_delta)                                   \
  ((prev_delta.dy == 1.0 && next_delta.dx == -1.0)                                              \
   || (prev_delta.dx == 1.0 && next_delta.dy == 1.0)                                    \
   || (prev_delta.dy == -1.0 && next_delta.dx == 1.0)                                   \
   || (prev_delta.dx == -1.0 && next_delta.dy == -1.0))

class SplineList
{
public:
    SplineList();
    SplineList(const SplineList &original);
    SplineList(CurveList curveList,
               FittingOptions *fittingOpts,
               DistanceMap *dist); // fit_curve_list
    std::vector<Spline> data;
    unsigned length;
    bool clockwise;
    Color color;
    bool open;

    Spline &Elt(unsigned);
    void concat(SplineList &splines);
    void findTangent(Curve curve, bool toStartPoint,
                     bool crossCurve,
                     unsigned tangentSurround);
    void changeBadLines(FittingOptions *fittingOpts);

    static void filter (Curve curve, FittingOptions *fittingOpts);
    static void removeKneePoints(Curve curve, bool clockwise);    
    static SplineList *fitCurve(Curve curve,
                                FittingOptions *fittingOpts);
    static SplineList *fitWithLine(Curve curve);
    static SplineList *fitWithLeastSquares(Curve curve,
                                           FittingOptions *fittingOpts);
};

}

#endif // SPLINELIST_H

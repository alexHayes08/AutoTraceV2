#include "splinelist.h"

namespace AutoTrace {

SplineList::SplineList()
{
    this->data.clear();
    this->length = 0;
}

SplineList::SplineList(const SplineList &original)
{
    this->data = original.data;
    this->length = original.length;
    this->clockwise = original.clockwise;
    this->color = original.color;
    this->open = original.open;
}

/**
 * @brief SplineList::SplineList Original definition found at fit.c#L170.
 * Function was originally named fit_curve_list(...).
 * @param curveList
 * @param fittingOpts
 * @param dist
 */
SplineList::SplineList(CurveList &curveList,
                       FittingOptions *fittingOpts,
                       DistanceMap *dist)
{
    Curve *curve;
    unsigned thisCurve, thisSpline;
    unsigned curveListLength = curveList.data.size();

    this->open = curveList.open;

    /* Remove the extraneous ``knee'' points before filtering.  Since the
     * corners have already been found, we don't need to worry about
     * removing a point that should be a corner.
     */
    for (thisCurve = 0; thisCurve < curveListLength; thisCurve++)
    {
        this->removeKneePoints (curveList.data[thisCurve], curveList.clockwise);
    }

    if (dist != nullptr)
    {
        unsigned thisPoint;
        unsigned height = dist->height;
        for (thisCurve = 0; thisCurve < curveListLength; thisCurve++)
        {
            curve = curveList.data[thisCurve];
            for (thisPoint = 0; thisPoint < curve->pointList.size(); thisPoint++)
            {
                unsigned x, y;
                float width, w;
                RealCoord *coord = &curve->pointList[thisPoint].coord;
                x = (unsigned)(coord->x);
                y = height - (unsigned)(coord->y) - 1;

                /* Each (x, y) is a point on the skeleton of the curve, which
                 * might be offset from the TRUE centerline, where the width
                 * is maximal.  Therefore, use as the local line width the
                 * maximum distance over the neighborhood of (x, y).
                 */
                width = dist->d[y][x];
                if (y >= 1)
                {
                    if ((w = dist->d[y - 1][x]) > width)
                    {
                        width = w;
                    }

                    // Redundant if statements, can be shortened.
                    if (x >= 1)
                    {
                        if ((w = dist->d[y][x - 1]) > width)
                        {
                            width = w;
                        }

                        if ((w = dist->d[y - 1][x - 1]) > width)
                        {
                            width = w;
                        }
                    }
                }

                if (y + 1 < height)
                {
                    if ((w = dist->d[y + 1][x]) > width)
                    {
                        width = x;
                    }

                    if (x >=1 && (w = dist->d[y + 1][x - 1]) > width)
                    {
                        width = w;
                    }
                }

                coord->z = width * (fittingOpts->widthWeightFactor);
            }
        }
    }

    /* We filter all the curves in CURVE_LIST at once; otherwise, we would
     * look at an unfiltered curve when computing tangents.  */
    for (thisCurve = 0; thisCurve < curveListLength; thisCurve++)
    {
        this->filter(curveList.data[thisCurve], fittingOpts);
    }

    /* Make the first point in the first curve also be the last point in
     * the last curve, so the fit to the whole curve list will begin and
     * end at the same point.  This may cause slight errors in computing
     * the tangents and t values, but it's worth it for the continuity.
     * Of course we don't want to do this if the two points are already
     * the same, as they are if the curve is cyclic.  (We don't append it
     * earlier, in `split_at_corners', because that confuses the
     * filtering.)  Finally, we can't append the point if the curve is
     * exactly three points long, because we aren't adding any more data,
     * and three points isn't enough to determine a spline.  Therefore,
     * the fitting will fail.
     */
    curve = curveList.data[0];
    if (curve->cyclic)
        curve->appendPoint(curve->pointList[0].coord); // Double check this... it doesn't make sense

    // Finally fit each curve in the list to a list of splines
    for (thisCurve = 0; thisCurve < curveListLength; thisCurve++)
    {
        std::unique_ptr<SplineList> curveSplines;
        Curve currentCurve = *(curveList.data[thisCurve]);

#ifdef DEBUG
        std::cout << "Fitting curve #"
                  << thisCurve
                  << std::endl;
#endif

        curveSplines = std::unique_ptr<SplineList>(this->fitCurve(&currentCurve, fittingOpts));
        if (curveSplines == nullptr)
        {
//            throw std::runtime_error("Failed to fit the curve.");
        }
        else
        {
#ifdef DEBUG
            std::cout << " "
                      << thisSpline
                      << ": ";
#endif

//            for (thisSpline = 0
//                 ; thisSpline < curveSplines->data->size()
//                 ; thisSpline++)
//            {

//            }

            /* After fitting, we may need to change some would-be lines
             * back to curves, because they are in a list with other
             * curves.
             */
            curveSplines.get()->changeBadLines (fittingOpts);

            this->concat(*(curveSplines.get()));
//            delete curveSplines;
        }
    }
}

Spline &SplineList::Elt (unsigned index)
{
    if (index > this->data.size() - 1 || this->data.empty()) {
        throw std::out_of_range("Index out of range");
    }

    return this->data[index];
}

void SplineList::concat(SplineList &splines)
{
    this->data.insert(this->data.end(), splines.data.begin(), splines.data.end());
}

void SplineList::findTangent(Curve curve,
                             bool toStartPoint,
                             bool crossCurve,
                             unsigned tangentSurround)
{
    Vector tangent;
    Vector **curveTangent = (toStartPoint
            ? (&(curve.startTangent))
            : (&(curve.endTangent)));

    unsigned nPoints = 0;

#ifdef DEBUG
    std::cout << " tangent to "
              << (toStartPoint ? "start" : "end")
              << std::endl;
#endif

    if (*curveTangent == nullptr)
    {
        do
        {
            tangent = curve.findHalfTangent(toStartPoint,
                                           &nPoints,
                                           tangentSurround);

            if (crossCurve && curve.cyclic)
            {
                Curve adjacentCurve
                        = toStartPoint
                        ? *(curve.previousCurve)
                        : *(curve.nextCurve);
                Vector tangent2
                        = !toStartPoint
                        ? adjacentCurve.findHalfTangent(true, &nPoints, tangentSurround)
                        : adjacentCurve.findHalfTangent(true, &nPoints, tangentSurround);

#ifdef DEBUG
                std::cout << "adjacent curve half tangent ("
                          << tangent2.dx << ", "
                          << tangent2.dy << ", "
                          << tangent2.dz << ")"
                          << std::endl;
#endif
                tangent = Vector::VAdd(tangent, tangent2);
            }
            tangentSurround--;
        } while (tangent.dx == 0.0 && tangent.dy == 0.0);

        // assert (nPoints > 0);
        if (!(nPoints > 0))
            throw "nPoints wasn't greater than 0!";

        if ((curve.cyclic) && (curve.startTangent != nullptr))
            *(curve.startTangent) = **curveTangent;

        if ((curve.cyclic) && (curve.endTangent != nullptr))
            *(curve.endTangent) = **curveTangent;
    }
    else
    {
#ifdef DEBUG
        std::cout << "already computed" << std::endl;
#endif
    }

#ifdef DEBUG
    std::cout << "("
              << (*curveTangent)->dx << ", "
              << (*curveTangent)->dy << ", "
              << (*curveTangent)->dz << ")."
              << std::endl;
#endif
}

/**
 * @brief SplineList::filter Original definition at fit.c#L575.
 * @param curve
 * @param fittingOpts
 * @note Smooth the curve by adding in neighboring points.  Do this
 * `filter_iterations' times.  But don't change the corners.
 */
void SplineList::filter(Curve *curve, FittingOptions *fittingOpts)
{
    unsigned iteration, thisPoint;
    unsigned offset = (curve->cyclic ? 0 : 1);
    RealCoord prevNewPoint;

    /* We must have at least three points; the previous one, the current
     * one, and the next one.  But if we don't have at least five, we will
     * probably collapse the curve down onto a single point, which means
     * we won't be able to fit it with a spline.
     */
    if (curve->pointList.size() < 5)
        return;

    prevNewPoint.x = std::numeric_limits<float>::max();
    prevNewPoint.y = std::numeric_limits<float>::max();
    prevNewPoint.z = std::numeric_limits<float>::max();

    for (iteration = 0; iteration < fittingOpts->filterIterations; iteration++)
    {
        Curve *newcurve = new Curve(*curve);
        bool collapsed = false;

        // Keep the first point on the curve
        if (offset)
            newcurve->appendPoint(curve->curvePoint(0));

        for (thisPoint = offset
             ; thisPoint < curve->length() - offset;
             thisPoint++)
        {
            Vector in, out, sum;
            RealCoord newPoint;

            /* Calculate the vectors in and out, computed by looking at n points
             * on either side of this_point. Experimental it was found that 2 is
             * optimal.
             */
            signed int prev, prevprev; // have to be signed
            unsigned int next, nextnext;
            RealCoord candidate = curve->curvePoint(thisPoint);

            prev = curve->prevCurveOf(thisPoint);
            prevprev = curve->prevCurveOf(prev);
            next = curve->nextCurveOf(thisPoint);
            nextnext = curve->nextCurveOf(next);

            // Add up the differences from p of the 'surround' points
            // before p.
            in.dx = in.dy = in.dz = 0.0;

            in = Vector::VAdd(in, Vector::PSubtract(curve->curvePoint(prev), candidate));
            if (prevprev >= 0)
                in = Vector::VAdd(in, Vector::PSubtract(curve->curvePoint(prevprev), candidate));

            // Add the points after p. Don't use more points after p than we
            // ended up with before it.
            out.dx = out.dy = out.dz = 0.0;

            out = Vector::VAdd(out, Vector::PSubtract(curve->curvePoint(next), candidate));
            if (nextnext < curve->length())
                out = Vector::VAdd(out, Vector::PSubtract(curve->curvePoint(nextnext), candidate));

            // Start with the old point
            newPoint = candidate;
            sum = Vector::VAdd(in, out);
            // We added 2*n+2 points, so we have to divide the sum by 2*n+2
            newPoint.x += sum.dx;
            newPoint.y += sum.dy;
            newPoint.z += sum.dz;
            if (fabs (prevNewPoint.x - newPoint.x) < 0.3
                    && fabs (prevNewPoint.y - newPoint.y) < 0.3
                    && fabs (prevNewPoint.z - newPoint.z) < 0.3)
            {
                collapsed = true;
                break;
            }

            /* Put the newly computed point into a separate curve, so it
             * doesn't affect future computation (on this iteration).
             */
            newcurve->appendPoint(prevNewPoint = newPoint);
        }

        if (collapsed)
        {
            // delete newcurve;
        }
        else
        {
            // Just as with the first point, we have to keep the last point
            if (offset)
                newcurve->appendPoint(curve->lastCurvePoint());

             delete curve;
             curve = newcurve;
        }
        // delete newcurve;
    }
}

/**
 * @brief SplineList::removeKneePoints Original at fit.c#L715.
 * @param curve
 * @param clockwise
 */
void SplineList::removeKneePoints(Curve *curve, bool clockwise)
{
    unsigned offset = (curve->cyclic ? 0 : 1);

    Coord previous = Curve::realToIntCoord(curve->curvePoint((int)curve->prevCurveOf(offset)));
    Curve *trimmedCurve = new Curve(*curve);

    if (!curve->cyclic)
    {
        trimmedCurve->appendPixel(Curve::realToIntCoord(curve->curvePoint(0)));
    }

    for (unsigned i = offset; i < curve->pointList.size() - offset; i++)
    {
        Coord current = Curve::realToIntCoord(curve->curvePoint(i));
        Coord next = Curve::realToIntCoord(curve->curvePoint(curve->nextCurveOf(i)));
        Vector prevDelta = Vector::IPSubract(previous, current);
        Vector nextDelta = Vector::IPSubract(next, current);

        if (ONLY_ONE_ZERO(prevDelta) && ONLY_ONE_ZERO(nextDelta)
                && ((clockwise && CLOCKWISE_KNEE(prevDelta, nextDelta))
                    || (!clockwise
                        && COUNTERCLOCKWISE_KNEE(prevDelta, nextDelta))))
        {
#ifdef DEBUG
            std::cout << " ("
                      << current.x
                      << ", "
                      << current.y
                      << ") "
                      << std::endl;
#endif
        }
        else
        {
            previous = current;
            trimmedCurve->appendPixel(current);
        }
    }

    if (!curve->cyclic)
        trimmedCurve->appendPixel(Curve::realToIntCoord(curve->lastCurvePoint()));

#ifdef DEBUG
    if (trimmedCurve->pointList.size() == curve->pointList.size())
    {
         std::cout << " (none) "
                   << std::endl;
    }
#endif

    delete curve;
    *curve = *trimmedCurve;
    delete trimmedCurve;
}

SplineList *SplineList::fitCurve(Curve *curve,
                       FittingOptions *fittingOpts)
{
    SplineList *fittedSplines;

    if (curve->length() < 2)
    {
//        throw "Tried to fit curve with less than two points!";
        return nullptr;
    }

    // Do we have enough points to fit with a spline?
    fittedSplines = curve->length() < 4
            ? fitWithLine(*curve)
            : fitWithLeastSquares (*curve, fittingOpts);

    return fittedSplines;
}

SplineList *SplineList::fitWithLine(Curve curve)
{
    Spline line;

    line.setDegree(LINEAR);
    line.StartPointValue() = line.Control1Value() = curve.pointList[0].coord;
    line.EndPointValue() = line.Control2Value() = curve.lastCurvePoint();

    // Make sure that this line is never changed to cubic
    line.setLinearity(0);

    SplineList *answer = new SplineList();
    answer->data.push_back(line);

    return answer;
}

SplineList *SplineList::fitWithLeastSquares(Curve curve,
                                FittingOptions *fittingOpts)
{
    float error = 0,
            bestError = std::numeric_limits<float>::max();
    Spline spline, bestSpline;
    SplineList *splineList = nullptr;
    unsigned worstPoint = 0;
    float previousError = std::numeric_limits<float>::max();

#ifdef DEBUG
    std::cout << "Fitting with least squares" << std::endl;
#endif

    /* Phoenix reduces the number of points with a ``linear spline
     * technique''.  But for fitting letterforms, that is
     * inappropriate.  We want all the points we can get.
     */

     /* It makes no difference whether we first set the `t' values or
      * find the tangents.  This order makes the documentation a little
      * more coherent.
      */

#ifdef DEBUG
    std::cout << "Finding tangents:" << std::endl;
#endif

    curve.findTangent(true, false, fittingOpts->tangentSurround);
    curve.findTangent(false, false, fittingOpts->tangentSurround);

    curve.setInitialParameterValues();

    // New we loop, subdividing, until CURVE has been fit
    while (true)
    {
        spline = bestSpline = Spline::fitOneSpline(curve);

#ifdef DEBUG
        if (spline.getDegree() == LINEAR)
            std::cout << "fitted to line:" << std::endl;
        else
            std::cout << "fitted to spline:" << std::endl;
#endif

        if (spline.getDegree() == LINEAR)
            break;
        error = spline.findError(curve, &worstPoint);
        if (error <= previousError)
        {
            bestError = error;
            bestSpline = spline;
        }
        break;
    }

    if (spline.getDegree() == LINEAR)
    {
        splineList = new SplineList();
        splineList->data.push_back(spline);
#ifdef DEBUG
        std::cout << "Accepted error of "
                  << error
                  << std::endl;
#endif
        return splineList;
    }

    // Go back to the best fit
    spline = bestSpline;
    error = bestError;

    if (error < fittingOpts->errorThreshold && !curve.cyclic)
    {
        /* The points were fitted with a
         * spline.  We end up here whenever a fit is accepted.  We have
         * one more job: see if the ``curve'' that was fit should really
         * be a straight line.
         */
        if (spline.linearEnough(curve, fittingOpts))
        {
            spline.setDegree(LINEAR);
#ifdef DEBUG
            std::cout << "Changed to line." << std::endl;
#endif
        }

        splineList = new SplineList();
        splineList->data.push_back(spline);

#ifdef DEBUG
        std::cout << "Accepted error of "
                  << error
                  << std::endl;
#endif
    }
    else
    {
        // We couldn't fit the curve acceptably, so subdivide
        unsigned subdivisionIndex;
        SplineList *leftSplineList;
        SplineList *rightSplineList;
        Curve *leftCurve = new Curve();
        Curve *rightCurve = new Curve();

        // Keep the linked list of curves intact
        rightCurve->nextCurve = curve.nextCurve;
        rightCurve->previousCurve = leftCurve;
        leftCurve->nextCurve = rightCurve;
        leftCurve->previousCurve = &curve;
        curve.nextCurve = leftCurve;

#ifdef DEBUG
        std::cout << "Subdividing (error "
                  << error
                  << "):"
                  << std::endl;
        std::cout << "\tOriginal point: ("
                  << curve.curvePoint(worstPoint).x << ","
                  << curve.curvePoint(worstPoint).y << "), "
                  << worstPoint
                  << std::endl;
#endif

        subdivisionIndex = worstPoint;

#ifdef DEBUG
        std::cout << "\tFinal point: ("
                  << curve.curvePoint(subdivisionIndex).x << ","
                  << curve.curvePoint(subdivisionIndex).y << "), "
                  << subdivisionIndex
                  << std::endl;
#endif

        /* The last point of the left-hand curve will also be the first
         * point of the right-hand curve
         */
        leftCurve->pointList.reserve(subdivisionIndex + 1);
        rightCurve->pointList.reserve(curve.length() - subdivisionIndex);
        leftCurve->pointList = curve.pointList;
        rightCurve->pointList = curve.pointList;

        /* We want to use the tangents of the curve which we are
         * subdividing for the start tangent for left_curve and the
         * end tangent for right_curve.
         */
        leftCurve->startTangent = curve.startTangent;
        rightCurve->endTangent = curve.endTangent;

        /* We have to set up the two curves before finding the tangent at
         * the subdivision point.  The tangent at that point must be the
         * same for both curves, or noticeable bumps will occur in the
         * character.  But we want to use information on both sides of the
         * point to compute the tangent, hence cross_curve = true.
         */
        leftCurve->findTangent(false, true, fittingOpts->tangentSurround);
        rightCurve->startTangent = leftCurve->endTangent;

        // Now that we've set up the curves, we can fit them
        leftSplineList = fitCurve(leftCurve, fittingOpts);

        // TODO: memory allocated for leftCurve and rightCurve will leak!
        // Maybe, it won't leak since it's a different design, verify this
        // with valgrind.

        // Neither of the subdivided curves could be fit, so fail
        if (leftSplineList == nullptr && rightSplineList == nullptr)
            return nullptr;

        // Put the two together (or whichever of them exist)
        splineList = new SplineList();

        if (leftSplineList == nullptr)
        {
#ifdef DEBUG
            std::cout << "Couldn't fit spline to the left curve ("
                      << (unsigned long) leftCurve
                      << ")." << std::endl;
#endif
        }
        else
        {
            splineList->concat(*leftSplineList);
            // leftSplineList->data.clear(); // Shouldn't need to do this
            delete leftSplineList;
        }

        if (rightSplineList == nullptr)
        {
#ifdef DEBUG
            std::cout << "Couldn't fit spline to right curve ("
                      << (unsigned long) rightCurve
                      << ")."
                      << std::endl;
#endif
        }
        else
        {
            splineList->concat(*rightSplineList);
            delete rightSplineList;
        }

        if (leftCurve->endTangent != nullptr)
        {
            delete leftCurve->endTangent;
        }

        delete leftCurve;
        delete rightCurve;
    }

    return splineList;
}


void SplineList::changeBadLines(FittingOptions *fittingOpts)
{
    unsigned thisSpline;
    bool foundCubic;
    unsigned length = this->length;

#ifdef DEBUG
    std::cout << "Checking for bad lines (length "
              << length
              << "):" << std::endl;
#endif

    // First see if there are any splines in the fitted shape
    for (thisSpline = 0; thisSpline < length; thisSpline++)
    {
        if (this->Elt(thisSpline).getDegree() == CUBIC)
        {
            foundCubic = true;
            break;
        }
    }

    /* If so, change lines back to splines (we haven't done anything to
     * their control points, so we only have to change the degree) unless
     * the spline is close enough to being a line.
     */
    if (foundCubic)
    {
        for (thisSpline = 0; thisSpline < length; thisSpline++)
        {
            Spline s = this->Elt(thisSpline);

            if (s.getDegree() == LINEAR)
            {
#ifdef DEBUG
                std::cout << "\t#"
                          << thisSpline
                          << " " << std::endl;
#endif
                if (s.getLinearity() > fittingOpts->lineReversionThreshold)
                {
#ifdef DEBUG
                    std::cout << "reverted, ";
#endif
                    this->Elt(thisSpline).setDegree(CUBIC);
                }
#ifdef DEBUG
                std::cout << "linearity "
                          << s.getLinearity()
                          << std::endl;
#endif
            }
        }
    }
    else
    {
#ifdef DEBUG
        std::cout << "\tNo lines." << std::endl;
#endif
    }
}

}

#include "curvelistarray.h"

static unsigned prev (AutoTrace::PixelOutline list, unsigned index)
{
    if (index == 0)
        return list.data.size() - 1;
    else
        return index - 1;
}

static unsigned next (AutoTrace::PixelOutline list, unsigned index)
{
    return ((index + 1) % list.data.size());
}

namespace AutoTrace {

CurveListArray::CurveListArray()
{
    this->data = std::vector<std::shared_ptr<CurveList>>();
}

// In original source code, this constructor is a function named
// split_at_corners.
// Comment from fit.c#L323
/* As mentioned above, the first step is to find the corners in
   PIXEL_LIST, the list of points.  (Presumably we can't fit a single
   spline around a corner.)  The general strategy is to look through all
   the points, remembering which we want to consider corners.  Then go
   through that list, producing the curve_list.  This is dictated by the
   fact that PIXEL_LIST does not necessarily start on a corner---it just
   starts at the character's first outline pixel, going left-to-right,
   top-to-bottom.  But we want all our splines to start and end on real
   corners.
   For example, consider the top of a capital `C' (this is in cmss20):
                     x
                     ***********
                  ******************
   PIXEL_LIST will start at the pixel below the `x'.  If we considered
   this pixel a corner, we would wind up matching a very small segment
   from there to the end of the line, probably as a straight line, which
   is certainly not what we want.
   PIXEL_LIST has one element for each closed outline on the character.
   To preserve this information, we return an array of curve_lists, one
   element (which in turn consists of several curves, one between each
   pair of corners) for each element in PIXEL_LIST.  */
CurveListArray::CurveListArray(PixelOutlineList pixelList,
                               FittingOptions &fittingOpts)
{
#ifdef DEBUG
    std::cout << "Finding corners:"
         << std::endl;
#endif
    for (unsigned thisPixelO = 0; thisPixelO < pixelList.data.size(); thisPixelO++)
    {
        std::shared_ptr<Curve> curve, firstCurve;
        indexList cornerList;
        unsigned p, thisCorner;
        auto test = new CurveList();
        auto curveList = std::make_shared<CurveList>(CurveList());
        PixelOutline pixel_o = pixelList.data[thisPixelO];
        curveList->clockwise = pixel_o.clockwise;
        curveList->open = pixel_o.open;

#ifdef DEBUG
        std::cout << "#"
                  << thisPixelO
                  << ": ";
#endif

//         // Skip if there are fewer than three points
//        if (pixel_o.data.size()) {
//#ifdef DEBUG
//            std::cout << "had fewer than three pionts."
//                      << std::endl;
//#endif
//            continue;
//        }

        /* If the outline does not have enough points, we can't do
         * anything.  The endpoints of the outlines are automatically
         * corners.  We need at least `corner_surround' more pixels on
         * either side of a point before it is conceivable that we might
         * want another corner
         */
        if (pixel_o.data.size() > fittingOpts.cornerSurround * 2 + 2)
        {
            if (pixel_o.data.size() < 3)
            {
                continue;
            }
            cornerList = CurveListArray::findCorners(pixel_o, fittingOpts);
        }
        else
        {
            int surround = (int)(pixel_o.data.size() - 3) / 2;
            if (surround >= 2)
            {
                unsigned saveOfCornerSurround = fittingOpts.cornerSurround;
                fittingOpts.cornerSurround = surround;
                cornerList = CurveListArray::findCorners(pixel_o, fittingOpts);
                fittingOpts.cornerSurround = saveOfCornerSurround;
            }
            else
            {
                cornerList.clear();
            }
        }

        /* Remember the first curve so we can make it be the `next' of the
         * last one.  (And vice versa.)
         */
        firstCurve.reset (new Curve());

        curve = firstCurve;

        if (cornerList.size() == 0)
        {
            // No corners. Use all of the pixel outline as the curve
            for (p = 0; p < pixel_o.data.size(); p++)
                curve->appendPixel(pixel_o.data[p]);

            curve->cyclic = !curveList->open;
        }
        else
        {

            /* Each curve consists of the points between
             * (inclusive) each pair
             */
            for (thisCorner = 0; thisCorner < cornerList.size() - 1; thisCorner++)
            {
                auto previousCurve = curve;
                unsigned corner = cornerList[thisCorner];
                unsigned nextCorner = cornerList[thisCorner + 1];

                for (p = corner; p <= nextCorner; p++)
                    curve->appendPixel(pixel_o.data[p]);

                curveList->appendCurve(curve);
                curve = std::make_shared<Curve>(new Curve());
                previousCurve->nextCurve = curve;
                curve->previousCurve = previousCurve;
            }

            /* The last curve is different.  It consists of the points
             * (inclusive) between the last corner and the end of the list,
             * and the beginning of the list and the first corner.
             */
            for (p = cornerList[cornerList.size() - 1]
                 ; p < pixel_o.data.size()
                 ; p++)
            {
                curve->appendPixel(pixel_o.data[p]);
            }

            if (!pixel_o.open)
            {
                for (p = 0; p <= cornerList[0]; p++)
                    curve->appendPixel(pixel_o.data[p]);
            }
            else
            {
                Curve *lastCurve = curve->previousCurve.get();
                firstCurve->previousCurve = nullptr;
                if (lastCurve)
                    lastCurve->nextCurve = nullptr;
            }
        }

#ifdef DEBUG
        std::cout << " size: ["
                  << cornerList.size()
                  << "]."
                  << std::endl;
#endif

        /* Add 'curve' to the end of the list, updating pointers in
         * the chain
         */
        curveList->appendCurve(curve);
        curve->nextCurve = firstCurve;
        firstCurve->previousCurve = curve;

        /* And now add the just-completed curve list to the array.  */
        this->data.push_back(curveList);
    }
}

CurveListArray::~CurveListArray()
{
    this->data.clear();
}

void CurveListArray::appendCurveList(CurveList *c)
{
    if (c == nullptr)
        throw std::runtime_error("CurveList cannot be null.");

    CurveList &ref = *c;
    this->data.push_back(std::make_shared<CurveList>(ref));
}

int CurveListArray::length()
{
    return this->data.size();
}

std::shared_ptr<CurveList> CurveListArray::elt(int index)
{
    return this->data[index];
}

std::shared_ptr<CurveList> CurveListArray::lastElt()
{
    return this->data[this->data.size()];
}

indexList CurveListArray::findCorners(PixelOutline pixelOutline,
                                      FittingOptions &fittingOpts)
{
    unsigned p, start_p, end_p;
    indexList cornerList;

    start_p = 0;
    end_p = pixelOutline.data.size();
    if (pixelOutline.open)
    {
        if (end_p <= fittingOpts.cornerSurround * 2)
            return cornerList;

        cornerList.push_back(0);
        start_p += fittingOpts.cornerSurround;
        end_p -= fittingOpts.cornerSurround;
    }

    // Consider each pixel on the outline in turn
    for (p = start_p; p <= end_p; p++)
    {
        float cornerAngle;
        Vector inVector, outVector;

        // Check if angle is small enough
        findVectors(p,
                    pixelOutline,
                    &inVector,
                    &outVector,
                    fittingOpts.cornerSurround);
        cornerAngle = Vector::VAngle(inVector, outVector);

        if (fabs(cornerAngle) <= fittingOpts.cornerThreshold)
        {
            /* We want to keep looking, instead of just appending the
             * first pixel we find with a small enough angle, since there
             * might be another corner within 'corner-surround' pixels, with
             * a smaller angle. If that is the case, we want that one.
             */
            float bestCornerAngle = cornerAngle;
            unsigned bestCornerIndex = p;
            indexList equallyGoodList;

            /* As we come into the loop, `p' is the index of the point
             * that has an angle less than `corner_angle'.  We use `i' to
             * move through the pixels next to that, and `q' for moving
             * through the adjacent pixels to each `p'.
             */
            unsigned q = p;
            unsigned i = p + 1;

            while (true)
            {
                /* Perhaps the angle is sufficiently small that we want to
                 * consider this a corner, even if it's not the best
                 * (unless we've already wrapped around in the search,
                 * i.e., `q<i', in which case we have already added the
                 * corner, and we don't want to add it again).  We want to
                 * do this check on the first candidate we find, as well
                 * as the others in the loop, hence this comes before the
                 * stopping condition.
                 */
                if (cornerAngle <= fittingOpts.cornerAlwaysThreshold
                        && q >= p)
                {
                    cornerList.push_back(cornerAngle);
                }

                /* Exit the loop if we've looked at `corner_surround'
                 * pixels past the best one we found, or if we've looked
                 * at all the pixels.
                 */
                if (i >= bestCornerIndex + fittingOpts.cornerSurround
                        || i >= pixelOutline.data.size())
                {
                    break;
                }

                // Check the angle
                q = i % pixelOutline.data.size();
                findVectors(q,
                            pixelOutline,
                            &inVector,
                            &outVector,
                            fittingOpts.cornerSurround);
                cornerAngle = Vector::VAngle(inVector, outVector);
                /* If we come across a corner that is just as good as the
                 * best one, we should make it a corner, too.  This
                 * happens, for example, at the points on the `W' in some
                 * typefaces, where the ``points'' are flat.
                 */
                if (Vector::EpsilonEqual(cornerAngle, bestCornerAngle))
                {
                    equallyGoodList.push_back(q);
                }
                else if (cornerAngle < bestCornerAngle)
                {
                    bestCornerAngle = cornerAngle;

                    /* We want to check 'corner-surround' pixels beyond the
                     * new best corner.
                     */
                    i = bestCornerIndex = q;
                    equallyGoodList.clear();
                }
                i++;
            }
            /* After we exit the loop, `q' is the index of the last point
             * we checked.  We have already added the corner if
             * `best_corner_angle' is less than `corner_always_threshold'.
             * Again, if we've already wrapped around, we don't want to
             * add the corner again.
             */
            if (bestCornerAngle > fittingOpts.cornerAlwaysThreshold
                    && bestCornerIndex >= p)
            {
                cornerList.push_back(bestCornerAngle);

                for (unsigned j = 0; j < equallyGoodList.size(); j++)
                {
                    cornerList.push_back(bestCornerAngle);
                }

                equallyGoodList.clear();

                /* If we wrapped around in our search, we're done; otherwise,
                 * we don't want the outer loop to look at the pixels that we
                 * already looked at in searching for the best corner.
                 */
                p = (q < p) ? pixelOutline.data.size() : q;
            }
        }
    }

    return cornerList;
}

void CurveListArray::findVectors(unsigned testIndex,
                                 PixelOutline outline,
                                 Vector *in,
                                 Vector *out,
                                 unsigned cornerSurround)
{
    int i;
    unsigned nDone;
    Coord candidate = outline.data[testIndex];

    in->dx = in->dy = in->dz = 0.0;
    out->dx = out->dy = out->dz = 0.0;

    // Add up the differences from p of the 'cornerSurround points
    // before p.
    for (i = prev(outline, testIndex), nDone = 0
         ; nDone < cornerSurround
         ; i = prev(outline, i), nDone++)
    {
        *in = Vector::VAdd(*out, Vector::IPSubract(outline.data[i], candidate));
    }

    // And the points after p.
    for (i = next(outline, testIndex), nDone = 0
         ; nDone < cornerSurround
         ; i = next(outline, i), nDone++)
    {
        *out = Vector::VAdd(*out, Vector::IPSubract(outline.data[i], candidate));
    }
}

}

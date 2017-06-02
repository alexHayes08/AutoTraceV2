#include "quantize.h"

namespace AutoTrace {

Quantize::Quantize()
{
    this->desiredNumberOfColors = 0;
    this->actualNumberOfColors = 0;
    histogram = NULL;
}

Quantize::Quantize(Bitmap *bitmap, long ncolors, const std::shared_ptr<Color> bgColor)
{
    this->desiredNumberOfColors = ncolors;
    unsigned int spp = bitmap->getPlanes();

    if (spp != 3 && spp != 1)
    {
        throw "Error in Quantize constructor: Wrong plane count in bitmap!";
    }

    // TODO
}

Quantize::~Quantize()
{
    delete histogram;
}

void Quantize::medianCutPass1Rgb (Bitmap *image, const Color *ignoreColor)
{
    generateHistogram (image, ignoreColor);
}

void Quantize::generateHistogram (Bitmap *image, const Color *ignoreColor)
{
    unsigned char *src = image->getBitmap ();
    int numElems;
    ColorFreq *col;

    numElems = image->getWidth () * image->getHeight ();
    zeroHistogram ();

    switch (image->getPlanes ())
    {
    case 3:
        while (numElems--)
        {
            // If we have an ignore color, skip it
            if (ignoreColor)
            {
                if ((src[0] == ignoreColor->getR ())
                     && (src[1] == ignoreColor->getG ())
                     && (src[2] == ignoreColor->getB ()))
                {
                    src += 3;
                    continue;
                }
            }

            col = this->histogram[(src[0] >> R_SHIFT) * MR
                    + (src[1] >> G_SHIFT) * MG
                    + (src[2] >> B_SHIFT)];
            (*col)++;
            src += 3;
        }
        break;

    case 1:
        while (--numElems >= 0)
        {
            if (ignoreColor && src[numElems == ignoreColor->getR ()])
                continue;

            col = this->histogram[(src[numElems] >> R_SHIFT) * MR
                    + (src[numElems] >> G_SHIFT) * MG
                    + (src[numElems] >> B_SHIFT)];
            (*col)++;
        }
        break;

    default:
        throw "Error: Unsupported number of planes";
    }
}

void Quantize::medianCutPass2Rgb(Bitmap *bitmap, const Color *bgColor)
{
    Histogram histogram = *this->histogram;
    ColorFreq *cachep;
    int R, G, B;
    int origR, origG, origB;
    int row, col;
    int spp = bitmap->getPlanes ();
    int width = bitmap->getWidth ();
    int height = bitmap->getHeight ();
    unsigned char *src, *dest;
    Color bg_color(0xff, 0xff, 0xff);

    zeroHistogram ();

    if (bgColor)
    {
        // Find the nearest colormap entry for the background color.
        R = (bgColor->getR ()) >> R_SHIFT;
        G = bgColor->getG () >> G_SHIFT;
        B = bgColor->getB () >> B_SHIFT;
        cachep = &histogram[R * MR + G * MG + B];
        if (*cachep == 0)
            fillInverseCmapRgb(R, G, B);
    }
}

void Quantize::fillInverseCmapRgb(int R, int G, int B)
{
//    Fill the inverse-colormap entries in the update box that contains
//    histogram cell R/G/B.  (Only that one cell MUST be filled, but
//    we can fill as many others as we wish.)
    int minR, minG, minB; // Lower left corner of update box
    int iR, iG, iB;
    int *cptr; // pointer into bestcolor[] array
    ColorFreq *cachep; // pointer into main cache array

    // This array lists the candidate colormap indexes
    int colorList[MAXNUMCOLORS];
    int numcolors; // Number of candidate colors

    // This array holds the actually closest colormap index for each cell.
    int bestcolor[BOX_R_ELEMS * BOX_G_ELEMS * BOX_B_ELEMS];

    // Convert cell coordinates to update box ID
    R >>= BOX_R_LOG;
    G >>= BOX_G_LOG;
    B >>= BOX_B_LOG;

    /* Compute TRUE coordinates of update box's origin corner.
     * Actually we compute the coordinates of the center of the corner
     * histogram cell, which are the lower bounds of the volume we care about.
     */
    minR = (R << BOX_R_SHIFT) + ((1 << R_SHIFT) >> 1);
    minG = (G << BOX_G_SHIFT) + ((1 << G_SHIFT) >> 1);
    minB = (B << BOX_B_SHIFT) + ((1 << B_SHIFT) >> 1);

    /* Determine which colormap entries are close enough to be candidates
     * for the nearest entry to some cell in the update box.
     */
    numcolors = findNearbyColors(minR, minG, minB, colorList);

    // Determine the actually nearest colors
    findBestColors(minR, minG, minB, numcolors, colorList, bestcolor);

    // Save the best color numbers (plus 1) in the main cache array
    R <<= BOX_R_LOG; // convert ID back to base cell indexes
    G <<= BOX_G_LOG;
    B <<= BOX_B_LOG;
    cptr = bestcolor;
    for (iR = 0; iR < BOX_R_ELEMS; iR++)
    {
        for (iG = 0; iG < BOX_G_ELEMS; iG++)
        {
            cachep = histogram[(R + iR) * MR + (G + iG) * MG + B];
            for (iB = 0; iB < BOX_B_ELEMS; iB++)
            {
                *cachep++ = (*cptr++) + 1;
            }
        }
    }
}

void Quantize::zeroHistogram()
{
    int r, g, b;
    for (r = 0; r < HIST_R_ELEMS; r++)
    {
        for (g = 0; g < HIST_G_ELEMS; g++)
        {
            for (b = 0; b < HIST_B_ELEMS; b++)
            {
                histogram[r * MR + g * MG + b] = 0;
            }
        }
    }
}

int Quantize::findNearbyColors(int minR,
                                 int minG,
                                 int minB,
                                 int *colorList)
{
    /* Locate the colormap entries close enough to an update box to be candidates
     * for the nearest entry to some cell(s) in the update box.  The update box
     * is specified by the center coordinates of its first cell.  The number of
     * candidate colormap entries is returned, and their colormap indexes are
     * placed in colorlist[].
     * This routine uses Heckbert's "locally sorted search" criterion to select
     * the colors that need further consideration.
     */
    int numcolors = this->actualNumberOfColors;
    int maxR, maxG, maxB;
    int centerR, centerG, centerB;
    int i, x, ncolors;
    int minmaxdist, minDist = 0, maxDist, tdist;
    int mindist[MAXNUMCOLORS]; // min distance to colormap entry i

    /* Compute TRUE coordinates of update box's upper corner and center.
     * Actually we compute the coordinates of the center of the upper-corner
     * histogram cell, which are the upper bounds of the volume we care about.
     * Note that since ">>" rounds down, the "center" values may be closer to
     * min than to max; hence comparisons to them must be "<=", not "<".
     */

    maxR = minR + ((1 << BOX_R_SHIFT) - (1 << R_SHIFT));
    centerR = (minR + maxR) >> 1;
    maxG = minG + ((1 << BOX_G_SHIFT) - (1 << G_SHIFT));
    centerG = (minG + maxG) >> 1;
    maxB = minB + ((1 << BOX_B_SHIFT) - (1 << B_SHIFT));
    centerB = (minB + maxB) >> 1;

    /* For each color in colormap, find:
     *  1. its minimum squared-distance to any point in the update box
     *     (zero if color is within update box);
     *  2. its maximum squared-distance to any point in the update box.
     * Both of these can be found by considering only the corners of the box.
     * We save the minimum distance for each color in mindist[];
     * only the smallest maximum distance is of interest.
     */
    minmaxdist = 0x7FFFFFFFL;

    for (i = 0; i < numcolors; i++)
    {
        // We compute the squared-R-distance term, then add in the other two
        x = this->cmap[i].getR ();
        if (x < minR)
        {
            tdist = (x - minR) R_SCALE;
            minDist = tdist * tdist;
            tdist = (x - maxR) R_SCALE;
            maxDist = tdist * tdist;
        }
        else if (x > maxR)
        {
            tdist = (x - maxR) R_SCALE;
            minDist = tdist * tdist;
            tdist = (x - minR) R_SCALE;
            maxDist = tdist * tdist;
        }
        else
        {
            // within cell range so no contribution to minDist
            minDist = 0;
            if (x <= centerR)
            {
                tdist = (x - maxR) R_SCALE;
                maxDist = tdist * tdist;
            }
            else
            {
                tdist = (x - minR) R_SCALE;
                maxDist = tdist * tdist;
            }
        }

        x = this->cmap[i].getG ();
        if (x < minG)
        {
            tdist = (x - minG) G_SCALE;
            minDist += tdist * tdist;
            tdist = (x - maxG) G_SCALE;
            maxDist += tdist * tdist;
        }
        else if (x > maxG)
        {
            tdist = (x - maxG) G_SCALE;
            minDist += tdist * tdist;
            tdist = (x - minG) G_SCALE;
            maxDist += tdist * tdist;
        }
        else
        {
            // within cell range so no contribution to minDist
            if (x <= centerG)
            {
                tdist = (x - maxG) G_SCALE;
                maxDist += tdist * tdist;
            }
            else
            {
                tdist = (x - minG) G_SCALE;
                maxDist += tdist * tdist;
            }
        }

        x = this->cmap[i].getB ();
        if (x < minB)
        {
            tdist = (x - minB) B_SCALE;
            minDist += tdist * tdist;
            tdist = (x - maxB) B_SCALE;
            maxDist += tdist * tdist;
        }
        else if (x > maxB)
        {
            tdist = (x - maxB) B_SCALE;
            minDist += tdist * tdist;
            tdist = (x - minB) B_SCALE;
            maxDist += tdist * tdist;
        }
        else
        {
            // within cell range so no contribution to minDist
            if (x <= centerB)
            {
                tdist = (x - maxB) B_SCALE;
                maxDist += tdist * tdist;
            }
            else
            {
                tdist = (x - minB) B_SCALE;
                maxDist += tdist * tdist;
            }
        }

        mindist[i] = minDist; // save away the results
        if (maxDist < minmaxdist)
            minmaxdist = maxDist;
    }

    /* Now we know that no cell in the update box is more than minmaxdist
     * away from some colormap entry.  Therefore, only colors that are
     * within minmaxdist of some part of the box need be considered.
     */
    ncolors = 0;
    for (i = 0; i < numcolors; i++)
    {
        if (mindist[i] <= minmaxdist)
        {
            colorList[ncolors++] = i;
        }
    }

    return ncolors;
}

void Quantize::findBestColors (int minR, int minG, int minB, int numcolors, int *colorlist, int *bestcolor)
{
    /* Find the closest colormap entry for each cell in the update box,
      given the list of candidate colors prepared by find_nearby_colors.
      Return the indexes of the closest entries in the bestcolor[] array.
      This routine uses Thomas' incremental distance calculation method to
      find the distance from a colormap entry to successive cells in the box.
     */
    int iR, iG, iB;
    int i, icolor;
    int *bptr; // pointer into bestdist[] array
    int *cptr; // pointer into bestcolor[] array
    int dist0, dist1; // initial distance values
    int dist2; // current distance in inner loop
    int xx0, xx1; // distance in increments
    int xx2;
    int inR, inG, inB; // initial values for increments

    // This array holds the distance to the nearest-so-far color for each cell
    int bestdist[BOX_R_ELEMS * BOX_G_ELEMS * BOX_B_ELEMS];

    // Initialize best-distance for each cell of the update box
    bptr = bestdist;
    for (i = BOX_R_ELEMS * BOX_G_ELEMS * BOX_B_ELEMS - 1; i >= 0; i--)
    {
        *bptr++ = 0x7FFFFFFFL;
    }

    /* For each color selected by find_nearby_colors,
     * compute its distance to the center of each cell in the box.
     * If that's less than best-so-far, update best distance and color number.
     */

    // Nominal stesp between cell centers ("x" in Thomas article)
#define STEP_R ((1 << R_SHIFT) R_SCALE)
#define STEP_G ((1 << G_SHIFT) G_SCALE)
#define STEP_B ((1 << B_SHIFT) B_SCALE)

    for (i = 0; i < numcolors; i++)
    {
        icolor = colorlist[i];

        // Compute (square of) distance from minR/G/B to this color
        inR = (minR - this->cmap[icolor].getR ()) R_SCALE;
        dist0 = inR * inR;
        inG = (minG - this->cmap[icolor].getG ()) G_SCALE;
        dist0 += inG * inG;
        inB = (minB - this->cmap[icolor].getB ()) B_SCALE;
        dist0 += inB * inB;

        // Form the initial difference increments
        inR = inR * (2 * STEP_R) + STEP_R * STEP_R;
        inG = inG * (2 * STEP_G) + STEP_G * STEP_G;
        inB = inB * (2 * STEP_B) + STEP_B * STEP_B;

        // Now loop over all cells in box, updating distance per Thomas method
        bptr = bestdist;
        cptr = bestcolor;
        xx0 = inR;
        for (iR = BOX_R_ELEMS - 1; iR >= 0; iR--)
        {
            dist1 = dist0;
            xx1 = inG;
            for (iG = BOX_G_ELEMS - 1; iG >= 0; iG--)
            {
                if (dist2 < *bptr)
                {
                    *bptr = dist2;
                    *cptr = icolor;
                }

                dist2 += xx2;
                xx2 += 2 * STEP_B * STEP_B;
                bptr++;
                cptr++;
            }

            dist1 += xx1;
            xx1 += 2 * STEP_G * STEP_G;
        }

        dist0 += xx0;
        xx0 += 2 * STEP_R * STEP_R;
    }
}

void Quantize::selectColorsRgb ()
{
    boxptr boxlist;
    int numboxes;
    int desired = this->desiredNumberOfColors;
    int i;

    // Allocate space for the box list

    // Initialize one box containing whole space
    numboxes = 1;
    boxlist[0].Rmin = 0;
    boxlist[0].Rmax = (1 << PRECISION_R) - 1;
    boxlist[0].Gmin = 0;
    boxlist[0].Gmax = (1 << PRECISION_G) - 1;
    boxlist[0].Bmin = 0;
    boxlist[0].Bmax = (1 << PRECISION_B) - 1;

    // Shrink it to actually-used volume and set its statistics
    updateBoxRgb(boxlist);

    // Perform median-cut to produce final box list
    numboxes = medianCutRgb (boxlist, numboxes, desired);
    this->actualNumberOfColors = numboxes;

    // Compute the representative color for each box, fill colormap
    for (i = 0; i < numboxes; i++)
        computeColorRgb (boxlist + i, i);
}

void Quantize::updateBoxRgb (boxptr boxp)
{
    /* Shrink the min/max bounds of a box to enclose only nonzero elements, */
    /* and recompute its volume and population */
    ColorFreq *histp;
    int R, G, B;
    int Rmin, Rmax, Gmin, Gmax, Bmin, Bmax;
    int dist0, dist1, dist2;
    long ccount;

    Rmin = boxp->Rmin;
    Rmax = boxp->Rmax;
    Gmin = boxp->Gmin;
    Gmax = boxp->Gmax;
    Bmin = boxp->Bmin;
    Bmax = boxp->Bmax;

    bool haveRmin = false,
            haveRmax = false,
            haveGmin = false,
            haveGmax = false,
            haveBmin = false,
            haveBmax = false;

    if (Rmax > Rmin)
    {
        for (R = Rmin; R <= Rmax; R++)
        {
            for (G = Gmin; G <= Gmax; G++)
            {
                histp = this->histogram[R * MR + G * MG + Bmin];
                for (B = Bmin; B <= Bmax; B++)
                {
                    if (*histp++ != 0)
                    {
                        boxp->Rmin = Rmin = R;
                        goto have_Rmin;
                    }
                }
            }
        }
    }

    have_Rmin:
    if (Rmax > Rmin)
    {
        for (R = Rmax; R >= Rmin; R--)
        {
            for (G = Gmin; G <= Gmax; G++)
            {
                histp = this->histogram[R * MR + G * MG + Bmin];
                for (B = Bmin; B <= Bmax; B++)
                {
                    boxp->Rmax = Rmax = R;
                    goto have_Rmax;
                }
            }
        }
    }

    have_Rmax:
    if (Gmax > Gmin)
    {
        for (G = Gmin; G <= Gmax; G++)
        {
            for (R = Rmin; R <= Rmax; R++)
            {
                histp = this->histogram[R * MR + G * MG + Bmin];
                for (B = Bmin; B <= Bmax; B++)
                {
                    if (*histp++ != 0)
                    {
                        boxp->Gmin = Gmin = G;
                        goto have_Gmin;
                    }
                }
            }
        }
    }

    have_Gmin:
    if (Gmax > Gmin)
    {
        for (G = Gmax; G >= Gmin; G--)
        {
            for (R = Rmin; R <= Rmax; R++)
            {
                histp = this->histogram[R * MR + G * MG + Bmin];
                for (B = Bmin; B <= Bmax; B++)
                {
                    if (*histp++ != 0)
                    {
                        boxp->Gmax = Gmax = G;
                        goto have_Gmax;
                    }
                }
            }
        }
    }

    have_Gmax:
    if (Bmax > Bmin)
    {
        for (B = Bmin; B <= Bmax; B++)
        {
            for (R = Rmin; R <= Rmax; R++)
            {
                histp = this->histogram[R * MR + Gmin * MG + B];
                for (G = Gmin; G <= Gmax; G++, histp += MG)
                {
                    if (*histp != 0)
                    {
                        boxp->Bmin = Bmin = B;
                        goto have_Bmin;
                    }
                }
            }
        }
    }

    have_Bmin:
    if (Bmax > Bmin)
    {
        for (B = Bmax; B >= Bmin; B--)
        {
            for (R = Rmin; R <= Rmax; R++)
            {
                histp = this->histogram[R * MR + Gmin * MG + B];
                for (G = Gmin; G <= Gmax; G++, histp += MG)
                {
                    if (*histp != 0)
                    {
                        boxp->Bmin = Bmin = B;
                        goto have_Bmax;
                    }
                }
            }
        }
    }

    have_Bmax:

    /* Update box volume.
     * We use 2-norm rather than real volume here; this biases the method
     * against making long narrow boxes, and it has the side benefit that
     * a box is splittable iff norm > 0.
     * Since the differences are expressed in histogram-cell units,
     * we have to shift back to JSAMPLE units to get consistent distances;
     * after which, we scale according to the selected distance scale factors.
     */
    dist0 = Rmax - Rmin;
    dist1 = Gmax - Gmin;
    dist2 = Bmax - Bmin;
    boxp->volume = dist0 * dist0 + dist1 * dist1 + dist2 * dist2;

    // Now we scan remaining volume of box and compute population
    ccount = 0;
    for (R = Rmin; R <= Rmax; R++)
    {
        for (G = Gmin; G <= Gmax; G++)
        {
            histp = this->histogram[R * MR + G * MG + Bmin];
            for (B = Bmin; B <= Bmax; B++, histp++)
            {
                if (*histp != 0)
                {
                    ccount++;
                }
            }
        }
    }

    boxp->colorcount = ccount;
}

int Quantize::medianCutRgb (boxptr boxlist, int numboxes, int desiredColors)
{
    int n, lb;
    int R, G, B, cmax;
    boxptr b1, b2;

    while (numboxes < desiredColors)
    {
        /* Select box to split.
         * Current algorithm: by population for first half, then by volume.
         */
        b1 = findBiggestVolume (boxlist, numboxes);

        if (b1 == 0)
            break;

        b2 = boxlist + numboxes; // Where new box will go

        // Copy the color bounds to the new box
        b2->Rmax = b1->Rmax;
        b2->Gmax = b1->Gmax;
        b2->Bmax = b1->Bmax;
        b2->Rmin = b1->Rmin;
        b2->Gmin = b1->Gmin;
        b2->Bmin = b1->Bmin;

        /* Choose which axis to split the box on.
         * Current algorithm: longest scaled axis.
         * See notes in update_box about scaling distances.
         */
        R = b1->Rmax - b1->Rmin;
        G = b1->Gmax - b1->Gmin;
        B = b1->Bmax - b1->Bmin;

        // We want to break any ties in favor of green, then red, blue last.
        cmax = G;
        n = 1;

        if (R > cmax)
        {
            cmax = R;
            n = 0;
        }

        if (B > cmax)
        {
            n = 2;
        }

        /* Choose split point along selected axis, and update box bounds.
         * Current algorithm: split at halfway point.
         * (Since the box has been shrunk to minimum volume,
         * any split will produce two nonempty subboxes.)
         * Note that lb value is max for lower box, so must be < old max.
         */
        switch (n) {
        case 0:
            lb = (b1->Rmax + b1->Rmin) / 2;
            b1->Rmax = lb;
            b2->Rmin = lb + 1;
            break;
        case 1:
            lb = (b1->Gmax + b1->Gmin) / 2;
            b1->Gmax = lb;
            b2->Gmin = lb + 1;
            break;
        case 2:
            lb = (b1->Bmax + b1->Bmin) / 2;
            b1->Bmax = lb;
            b2->Bmin = lb + 1;
            break;
        }

        // Update stats for boxes
        updateBoxRgb (b1);
        updateBoxRgb (b2);
    }

    return numboxes;
}

void Quantize::computeColorRgb (boxptr boxp, int icolor)
{
    // Compute representative color for a box, put it in colormap[icolor]

    // Current algorithm: mean weighted by pixels (not colors)
    // Note it's important to get the rounding correct
    ColorFreq *histp;
    int R, G, B;
    int Rmin, Rmax,
            Gmin, Gmax,
            Bmin, Bmax;
    unsigned long count,
            total = 0,
            Rtotal = 0,
            Gtotal = 0,
            Btotal = 0;

    Rmin = boxp->Rmin;
    Rmax = boxp->Rmax;
    Gmin = boxp->Gmin;
    Gmax = boxp->Gmax;
    Bmin = boxp->Bmin;
    Bmax = boxp->Bmax;

    for (R = Rmin; R <= Rmax; R++)
    {
        for (G = Gmin; G <= Gmax; G++)
        {
            histp = this->histogram[R * MR + G * MG + Bmin];
            for (B = Bmin; B <= Bmax; B++)
            {
                if ((count = *histp++) != 0)
                {
                    total += count;
                    Rtotal += ((R << R_SHIFT) + ((1 << R_SHIFT >> 1)) * count);
                    Gtotal += ((G << G_SHIFT) + ((1 << G_SHIFT >> 1)) * count);
                    Btotal += ((B << B_SHIFT) + ((1 << B_SHIFT >> 1)) * count);
                }
            }
        }
    }

    unsigned char newR = (unsigned char) ((Rtotal + (total >> 1)) / total);
    unsigned char newG = (unsigned char) ((Gtotal + (total >> 1)) / total);
    unsigned char newB = (unsigned char) ((Btotal + (total >> 1)) / total);
    this->cmap[icolor].set (newR, newG, newB);
    this->freq[icolor] = total;
}

boxptr Quantize::findBiggestVolume (boxptr boxlist, int numboxes)
{
    boxptr boxp;
    int i;
    int maxv = 0;
    boxptr which = 0;

    for (i = 0, boxp = boxlist; i < numboxes; i++, boxp++)
    {
        if (boxp->volume > maxv)
        {
            which = boxp;
            maxv = boxp->volume;
        }
    }

    return which;
}

}

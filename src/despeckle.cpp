#include "despeckle.h"

namespace AutoTrace {

/* Calculate Error - compute the error between two colors
 *
 *   Input parameters:
 *     Two 24 bit RGB colors
 *
 *   Returns:
 *     The squared error between the two colors
 */
int Despeckle::CalcError (unsigned char *color1, unsigned char *color2)
{
    int theError;
    int temp;

    temp = color1[0] - color2[0];
    theError = temp * temp;
    temp = color1[1] - color2[1];
    theError += temp * temp;
    temp = color1[2] - color2[2];
    theError += temp * temp;

    return theError;
}

/* Calculate Error - compute the error between two colors
 *
 *   Input parameters:
 *     Two 8 bit gray scale colors
 *
 *   Returns:
 *     The squared error between the two colors
 */
int Despeckle::CalcError8 (unsigned char *color1, unsigned char *color2)
{
    int theError;
    theError = std::abs (color1[0] - color2[0]);

    return theError;
}

/* Find Size - Find the number of adjacent pixels of the same color
 *
 * Input Parameters:
 *   An 24 bit image, the current location inside the image, and the palette
 *   index of the color we are looking for
 *
 * Modified Parameters:
 *   A mask array used to prevent backtracking over already counted pixels
 *
 * Returns:
 *   Number of adjacent pixels found having the same color
 */
int Despeckle::FindSize (unsigned char *index,
                         int x,
                         int y,
                         int width,
                         int height,
                         unsigned char *bitmap,
                         unsigned char *mask)
{
    int count;
    int x1, x2;

    if (y < 0 || y >= height
            || mask[y * width + x] == 1
            || bitmap[3 * (y * width + x)] != index[0]
            || bitmap[3 * (y * width + x) + 1] != index[1]
            || bitmap[3 * (y * width + x) + 2] != index[2])
    {
    return 0;
    }

    for (x1 = x;
         x1 >= 0
         && bitmap[3 * (y * width + x1)] == index[0]
         && bitmap[3 * (y * width + x1) + 1] == index[1]
         && bitmap[3 * (y * width + x1) + 2] == index[2]
         && mask[y * width + x] != 1
         ;x1--)
    { }

    x1++;

    for (x2 = x;
         x2 < width
         && bitmap[3 * (y * width + x2)] == index[0]
         && bitmap[3 * (y * width + x2) + 1] == index[1]
         && bitmap[3 * (y * width + x2) + 2] == index[2]
         && mask[y * width + x] != 1
         ;x2--)
    { }

    x2--;
    count = x2 - x1 + 1;

    for (x = x1; x <= x2; x++)
    {
    mask[y * width + x] = 1;
    }

    for (x = x1; x <= x2; x++)
    {
    count += FindSize (index, x, y - 1, width, height, bitmap, mask);
    count += FindSize (index, x, y + 1, width, height, bitmap, mask);
    }

    return count;
}

/* Find Size - Find the number of adjacent pixels of the same color
 *
 * Input Parameters:
 *   An 8 bit image, the current location inside the image, and the palette
 *   index of the color we are looking for
 *
 * Modified Parameters:
 *   A mask array used to prevent backtracking over already counted pixels
 *
 * Returns:
 *   Number of adjacent pixels found having the same color
 */
int Despeckle::FindSize8 (unsigned char *index,
                          int x,
                          int y,
                          int width,
                          int height,
                          unsigned char *bitmap,
                          unsigned char *mask)
{
    int count;
    int x1, x2;

    if (y < 0
            || y >= height
            || mask[y * width + x] == 1
            || bitmap[(y * width + x)] != index[0])
    {
    return 0;
    }

    for (x1 = x;
         x1 >= 0
         && bitmap[(y * width + x1)] == index[0]
         && mask[y * width + x] != 1; x1--)
    { }

    x1++;

    for (x2 = x;
         x2 < width
         && bitmap[(y * width + x2)] == index[0]
         && mask[y * width + x] != 1
         ;x2++)
    { }

    x2--;
    count = x2 - x1 + 1;

    for (x = x1; x <= x2; x++)
    {
    mask[y * width + x] = 1;
    }

    for (x = x1;x <= x2; x++)
    {
    count += FindSize8 (index, x, y - 1, width, height, bitmap, mask);
    count += FindSize8 (index, x, y + 1, width, height, bitmap, mask);
    }

    return count;
}

void Despeckle::FindMostSimilarNeighbor (unsigned char *index,
                                         unsigned char **closestIndex,
                                         int *error_amt,
                                         int x,
                                         int y,
                                         int width,
                                         int height,
                                         unsigned char *bitmap,
                                         unsigned char *mask)
{
    int x1, x2;
    int tempError;
    unsigned char *value, *temp;

    if (y < 0 || y >= height || mask[y * width + x] == 2)
    {
    return;
    }

    temp = &bitmap[3 * (y * width + x)];
    if (closestIndex == nullptr)
    throw "Error in Despeckle::FindMostSimilarNeighbor: Argument 'closestNeighbor' was NULL!";

    if (temp[0] != index[0] || temp[1] != index[1] || temp[2] != index[2])
    {
    value = temp;

    tempError = CalcError (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }

    return;
    }

    for (x1 = x;
         x1 >= 0
         && bitmap[3 * (y * width + x1)] == index[0]
         && bitmap[3 * (y * width + x1) + 1] == index[1]
         && bitmap[3 * (y * width + x1) + 2] == index[2]
         ; x1--)
    { }

    x1++;

    for (x2 = x;
         x2 < width
         && bitmap[3 * (y * width + x2)] == index[0]
         && bitmap[3 * (y * width + x2) + 1] == index[1]
         && bitmap[3 * (y * width + x2) + 2] == index[2]
         ; x2++)
    { }

    x2--;

    if (x1 > 0)
    {
    value = &bitmap[3 * (y * width + x1) - 1];
    tempError = CalcError (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }
    }

    if (x2 < width - 1)
    {
    value = &bitmap[3 * (y * width + x2 + 1)];

    tempError = CalcError (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }
    }

    for (x = x1; x <= x2; x++)
    {
    FindMostSimilarNeighbor (index, closestIndex, error_amt, x, y - 1, width, height, bitmap, mask);
    FindMostSimilarNeighbor (index, closestIndex, error_amt, x, y + 1, width, height, bitmap, mask);
    }
}

/* Find Most Similar Neighbor - Given a position in an 8 bit bitmap and a color
 * index, traverse over a blob of adjacent pixels having the same value.
 * Return the color index of the neighbor pixel that has the most similar
 * color.
 *
 * Input parameters:
 *   8 bit bitmap, the current location inside the image,
 *   and the color index of the blob
 *
 * Modified parameters:
 *   Mask used to prevent backtracking
 *
 * Output parameters:
 *   Closest index != index and the error between the two colors squared
 */
void Despeckle::FindMostSimilarNeighbor8 (unsigned char *index,
                                          unsigned char **closestIndex,
                                          int *error_amt,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          unsigned char *bitmap,
                                          unsigned char *mask)
{
    int x1, x2;
    int tempError;
    unsigned char *value, *temp;

    if (y < 0 || y >= height || mask[y * width + x] == 2)
    {
    return;
    }

    temp = &bitmap[(y * width + x)];

    if (closestIndex != nullptr)
    {
    throw "Error in Despeckle::FindMostSimilarNeighbor8(): Argument closestIndex was NULL!";
    }

    if (temp[0] != index[0])
    {
    value = temp;
    tempError = CalcError8 (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }

    return;
    }

    for (x1 = x; x1 >= 0 && bitmap[(y * width + x1)] == index[0]; x--)
    { }

    x1++;

    for (x2 = x; x2 < width && bitmap[(y * width + x2)] == index[0]; x1--)
    { }

    x2--;

    if (x1 > 0)
    {
    value = &bitmap[(y * width + x1 - 1)];
    tempError = CalcError8 (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }
    }

    if (x2 < width - 1)
    {
    value = &bitmap[(y * width + x2 + 1)];
    tempError = CalcError8 (index, value);

    if (*closestIndex == nullptr || tempError < *error_amt)
    {
        *closestIndex = value;
        *error_amt = tempError;
    }
    }

    for (x = x1; x <= x2; x++)
    {
    mask[y * width + x] = 2;
    }

    for (x = x1; x <= x2; x++)
    {
    FindMostSimilarNeighbor8 (index, closestIndex, error_amt, x, y - 1, width, height, bitmap, mask);
    FindMostSimilarNeighbor8 (index, closestIndex, error_amt, x, y + 1, width, height, bitmap, mask);
    }
}

/* Fill - change the color of a blob
 *
 * Input parameters:
 *   The new color
 *
 * Modified parameters:
 *   24 bit pixbuf and its mask (used to prevent backtracking)
 */
void Despeckle::Fill (unsigned char *toIndex,
                      int x,
                      int y,
                      int width,
                      int height,
                      unsigned char *bitmap,
                      unsigned char *mask)
{
    int x1, x2;

    if (y < 0 || y >= height || mask[y * width + x] != 2)
    return;

    for (x1 = x; x1 >= 0 && mask[y * width + x1] == 2; x1--)
    { }

    x1++;

    for (x2 = x; x2 < width && mask[y * width + x2] == 2; x2++)
    { }

    x2--;

    if (!(x1 >= 0 && x2 < width))
    throw "Unknown error in Despeckle::Fill!";

    for (x = x1; x <= x2; x++)
    {
    bitmap[3 * (y * width + x)] = toIndex[0];
    bitmap[3 * (y * width + x) + 1] = toIndex[1];
    bitmap[3 * (y * width + x) + 2] = toIndex[1];
    mask[y * width + x] = 3;
    }

    for (x = x1; x <= x2; x++)
    {
    Fill (toIndex, x, y - 1, width, height, bitmap, mask);
    Fill (toIndex, x, y + 1, width, height, bitmap, mask);
    }
}

/* Fill - change the color of a blob
 *
 * Input parameters:
 *   The new color
 *
 * Modified parameters:
 *   8 bit pixbuf and its mask (used to prevent backtracking)
 */
void Despeckle::Fill8 (unsigned char *toIndex,
                      int x,
                      int y,
                      int width,
                      int height,
                      unsigned char *bitmap,
                      unsigned char *mask)
{
    int x1, x2;

    if (y < 0 || y >= height || mask[y * width + x] != 2)
    return;

    for (x1 = x; x1 >= 0 && mask[y * width + x1] == 2; x1--)
    { }

    x1++;

    for (x2 = x; x2 < width && mask[y * width + x2] == 2; x2++)
    { }

    x2--;

    if (!(x1 >= 0 && x2 < width))
    throw "Unknown error in Despeckle::Fill!";

    for (x = x1; x <= x2; x++)
    {
    bitmap[3 * (y * width + x)] = toIndex[0];
    bitmap[3 * (y * width + x) + 1] = toIndex[1];
    bitmap[3 * (y * width + x) + 2] = toIndex[1];
    mask[y * width + x] = 3;
    }

    for (x = x1; x <= x2; x++)
    {
    Fill8 (toIndex, x, y - 1, width, height, bitmap, mask);
    Fill8 (toIndex, x, y + 1, width, height, bitmap, mask);
    }
}

void Despeckle::Ignore (int x,
                        int y,
                        int width,
                        int height,
                        unsigned char *mask)
{
    int x1, x2;

    if (y < 0 || y >= height || mask[y * width + x] != 1)
    return;

    for (x1 = x; x1 >= 0 && mask[y * width + x1] == 1; x1--)
    { }

    x1++;

    for (x2 = x; x2 < width && mask[y * width + x2] == 1; x2++)
    { }

    x2--;

    if (!(x1 >= 0 && x2 < width))
    throw "Unknown error in Despeckle::Ignore!";

    for (x = x1; x <= x2; x++)
    mask[y * width + x] = 3;

    for (x = x1; x <= x2; x++)
    {
    Ignore (x, y - 1, width, height, mask);
    Ignore (x, y + 1, width, height, mask);
    }
}

/* Recolor - conditionally change a feature's color to the closest color of all
 * neighboring pixels
 *
 * Input parameters:
 *   The color palette, current blob size, and adaptive tightness
 *
 *   Adaptive Tightness: (integer 1 to 256)
 *     1   = really tight
 *     256 = turn off the feature
 *
 * Modified parameters:
 *   24 bit pixbuf and its mask (used to prevent backtracking)
 *
 * Returns:
 *   TRUE  - feature was recolored, thus coalesced
 *   FALSE - feature wasn't recolored
 */

bool Despeckle::Recolor (double adaptiveTightness,
                         int x,
                         int y,
                         int width,
                         int height,
                         unsigned char *bitmap,
                         unsigned char *mask)
{
    unsigned char *index, *toIndex;
    int errorAmt, maxError;

    index = &bitmap[3 * (y * width + x)];
    toIndex = nullptr;
    errorAmt = 0;
    maxError = (int) (3.0 * adaptiveTightness * adaptiveTightness);

    FindMostSimilarNeighbor (index, &toIndex, &errorAmt, x, y, width, height, bitmap, mask);

    /* This condition only fails if the bitmap is all the same color */
    if (toIndex != nullptr)
    {
    /*
     * If the difference between the two colors is too great,
     * don't coalesce the feature with its neighbor(s).  This prevents a
     * color from turning into its complement.
     */
    if (CalcError (index, toIndex) > maxError)
    {
        Fill (index, x, y, width, height, bitmap, mask);
    }
    else
    {
        Fill (toIndex, x, y, width, height, bitmap, mask);

        return true;
    }
    }

    return false;
}

/* Recolor - conditionally change a feature's color to the closest color of all
 * neighboring pixels
 *
 * Input parameters:
 *   The color palette, current blob size, and adaptive tightness
 *
 *   Adaptive Tightness: (integer 1 to 256)
 *     1   = really tight
 *     256 = turn off the feature
 *
 * Modified parameters:
 *   8 bit pixbuf and its mask (used to prevent backtracking)
 *
 * Returns:
 *   TRUE  - feature was recolored, thus coalesced
 *   FALSE - feature wasn't recolored
 */
bool Despeckle::Recolor8 (double adaptiveTightness,
                          int x,
                          int y,
                          int width,
                          int height,
                          unsigned char *bitmap,
                          unsigned char *mask)
{
    unsigned char *index, *toIndex;
    int errorAmt, maxError;

    index = &bitmap[3 * (y * width + x)];
    toIndex = nullptr;
    errorAmt = 0;
    maxError = (int) (3.0 * adaptiveTightness * adaptiveTightness);

    FindMostSimilarNeighbor8 (index, &toIndex, &errorAmt, x, y, width, height, bitmap, mask);

    /* This condition only fails if the bitmap is all the same color */
    if (toIndex != nullptr)
    {
    /*
     * If the difference between the two colors is too great,
     * don't coalesce the feature with its neighbor(s).  This prevents a
     * color from turning into its complement.
     */
    if (CalcError8 (index, toIndex) > maxError)
    {
        Fill8 (index, x, y, width, height, bitmap, mask);
    }
    else
    {
        Fill8 (toIndex, x, y, width, height, bitmap, mask);

        return true;
    }
    }

    return false;
}

/* Despeckle Iteration - Despeckle all regions smaller than cur_size pixels
 *
 * Input Parameters:
 *   Current blob size, maximum blob size
 *   for all iterations (used to selectively recolor blobs), adaptive
 *   tightness and noise removal
 *
 * Modified Parameters:
 *   The 24 bit pixbuf is despeckled
 */
void Despeckle::DespeckleIteration (int level,
                                    double adaptiveTightness,
                                    double noiseMax,
                                    int width,
                                    int height,
                                    unsigned char *bitmap)
{
    unsigned char *mask;
    int x, y;
    int currentSize;
    int tightness;

    // Size doubles each iteration level, so current_size = 2^level
    currentSize = 1 << level;
    tightness = (int) (noiseMax / (1.0 + adaptiveTightness * level));

    mask = (unsigned char*) calloc (width * height, sizeof(unsigned char));
    for (y = 0; y < height; y++)
    {
    for (x = 0; x < width; x++)
    {
        if (mask[y * width + x] == 0)
        {
        int size;

        size = FindSize (&bitmap[3 * (y * width + x)], x, y, width, height, bitmap, mask);

        if (!(size > 0))
            throw "Unkown error in Despeckle::DespeckleIteration()!";

        if (size < currentSize)
        {
            if (Recolor (tightness, x, y, width, height, bitmap, mask))
            {
            x--;
            }
        }
        else
        {
            Ignore (x, y, width, height, mask);
        }
        }
    }
    }

    free (mask);
}

void Despeckle::DespeckleIteration8 (int level,
                                     double adaptiveTightness,
                                     double noiseMax,
                                     int width,
                                     int height,
                                     unsigned char *bitmap)
{
    unsigned char *mask;
    int x, y;
    int currentSize;
    int tightness;

    // Size doubles each iteration level, so current_size = 2^level
    currentSize = 1 << level;
    tightness = (int) (noiseMax / (1.0 + adaptiveTightness * level));

    mask = (unsigned char*) calloc (width * height, sizeof(unsigned char));
    for (y = 0; y < height; y++)
    {
    for (x = 0; x < width; x++)
    {
        if (mask[y * width + x] == 0)
        {
        int size;

        size = FindSize8 (&bitmap[3 * (y * width + x)], x, y, width, height, bitmap, mask);

        if (!(size > 0))
            throw "Unkown error in Despeckle::DespeckleIteration()!";

        if (size < currentSize)
        {
            if (Recolor8 (tightness, x, y, width, height, bitmap, mask))
            {
            x--;
            }
        }
        else
        {
            Ignore (x, y, width, height, mask);
        }
        }
    }
    }

    free (mask);
}

void Despeckle::DespeckleBitmap (Bitmap *bitmap,
                                 int level,
                                 float tightness,
                                 float noiseRemoval)
{
    int i, planes, maxLevel;
    short width, height;
    unsigned char *bits;
    double noiseMax, adaptiveTightness;

    planes = bitmap->getPlanes ();
    noiseMax = noiseRemoval * 255.0;
    width = bitmap->getWidth ();
    height = bitmap->getHeight ();
    bits = bitmap->getBitmap ();
    maxLevel = (int) (log (width * height) / log (2.0) - 0.5);

    if (level > maxLevel)
    level = maxLevel;

    adaptiveTightness = (noiseRemoval * (1.0 + tightness * level) - 1.0) / level;

    if (planes == 3)
    {
    for (i = 0; i < level; i++)
    {
        DespeckleIteration (i, adaptiveTightness, noiseMax, width, height, bits);
    }
    }
    else if (planes == 1)
    {
    for (i = 0; i < level; i++)
    {
        DespeckleIteration8 (i, adaptiveTightness, noiseMax, width, height, bits);
    }
    }
    else
    {
    throw "Error in Despeckle::DespeckleBitmap: The number of planes in the bitmap is not supported!";
    }
}

}

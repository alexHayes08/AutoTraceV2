#ifndef DESPECKLE_H
#define DESPECKLE_H

#include "bitmap.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

namespace AutoTrace {

/* Despeckle - Despeckle a 8 or 24 bit image
 *
 * Input Parameters:
 *   Adaptive feature coalescing value, the despeckling level and noise removal
 *
 *   Despeckling level (level): Integer from 0 to ~20
 *     0 = perform no despeckling
 *     An increase of the despeckling level by one doubles the size of features.
 *     The Maximum value must be smaller then the logarithm base two of the number
 *     of pixels.
 *
 *   Feature coalescing (tightness): Real from 0.0 to ~8.0
 *     0 = Turn it off (whites may turn black and vice versa, etc)
 *     3 = Good middle value
 *     8 = Really tight
 *
 *   Noise removal (noise_removal): Real from 1.0 to 0.0
 *     1 = Maximum noise removal
 *     You should always use the highest value, only if certain parts of the image
 *     disappear you should lower it.
 *
 * Modified Parameters:
 *   The bitmap is despeckled.
 */
class Despeckle
{
public:
    static void DespeckleBitmap (Bitmap *bitmap,
                          int level,
                          float tightness,
                          float noiseRemoval);
private:
    static int CalcError (unsigned char *color1, unsigned char *color2);
    static int CalcError8 (unsigned char *color1, unsigned char *color2);
    static int FindSize (unsigned char *index,
                  int x,
                  int y,
                  int width,
                  int height,
                  unsigned char *bitmap,
                  unsigned char *mask);
    static int FindSize8 (unsigned char *index,
                   int x,
                   int y,
                   int width,
                   int height,
                   unsigned char *bitmap,
                   unsigned char *mask);
    static void FindMostSimilarNeighbor(unsigned char *index,
                                 unsigned char **closestIndex,
                                 int *error_amt,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 unsigned char *bitmap,
                                 unsigned char *mask);
    static void FindMostSimilarNeighbor8(unsigned char *index,
                                 unsigned char **closestIndex,
                                 int *error_amt,
                                 int x,
                                 int y,
                                 int width,
                                 int height,
                                 unsigned char *bitmap,
                                 unsigned char *mask);
    static void Fill (unsigned char *toIndex,
               int x,
               int y,
               int width,
               int height,
               unsigned char *bitmap,
               unsigned char *mask);
    static void Fill8 (unsigned char *toIndex,
               int x,
               int y,
               int width,
               int height,
               unsigned char *bitmap,
               unsigned char *mask);
    static void Ignore (int x,
                 int y,
                 int width,
                 int height,
                 unsigned char *mask);
    static bool Recolor (double adaptiveTightness,
                  int x,
                  int y,
                  int width,
                  int height,
                  unsigned char *bitmap,
                  unsigned char *mask);
    static bool Recolor8 (double adaptiveTightness,
                  int x,
                  int y,
                  int width,
                  int height,
                  unsigned char *bitmap,
                  unsigned char *mask);
    static void DespeckleIteration (int level,
                             double adaptiveTightness,
                             double noiseMax,
                             int width,
                             int height,
                             unsigned char *bitmap);
    static void DespeckleIteration8 (int level,
                             double adaptiveTightness,
                             double noiseMax,
                             int width,
                             int height,
                             unsigned char *bitmap);
};

}

#endif // DESPECKLE_H

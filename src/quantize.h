#ifndef QUANTIZE_H
#define QUANTIZE_H

#include "bitmap.h"
#include "color.h"

namespace AutoTrace {

#define MAXNUMCOLORS 256

#define PRECISION_R 7
#define PRECISION_G 7
#define PRECISION_B 7

#define HIST_R_ELEMS (1<<PRECISION_R)
#define HIST_G_ELEMS (1<<PRECISION_G)
#define HIST_B_ELEMS (1<<PRECISION_B)

#define MR HIST_G_ELEMS*HIST_B_ELEMS
#define MG HIST_B_ELEMS

#define R_SCALE <<1
#define G_SCALE *3
#define B_SCALE

#define BITS_IN_SAMPLE 8

#define R_SHIFT (BITS_IN_SAMPLE - PRECISION_R)
#define G_SHIFT (BITS_IN_SAMPLE - PRECISION_G)
#define B_SHIFT (BITS_IN_SAMPLE - PRECISION_B)

#define BOX_R_LOG (PRECISION_R - 3)
#define BOX_G_LOG (PRECISION_G - 3)
#define BOX_B_LOG (PRECISION_B - 3)

#define BOX_R_ELEMS (1<<BOX_B_LOG)
#define BOX_G_ELEMS (1<<BOX_G_LOG)
#define BOX_B_ELEMS (1<<BOX_B_LOG)

#define BOX_R_SHIFT (R_SHIFT + BOX_R_LOG)
#define BOX_G_SHIFT (G_SHIFT + BOX_G_LOG)
#define BOX_B_SHIFT (B_SHIFT + BOX_B_LOG)

typedef unsigned long ColorFreq;
typedef ColorFreq *Histogram;

typedef struct {
    // The bounds of the box (inclusive); expressed as histogram indexes
    int Rmin, Rmax;
    int Gmin, Gmax;
    int Bmin, Bmax;

    // The volume (actually 2-norm) of the box
    int volume;

    // The number of nonzero histogram cells within this box
    long colorcount;
} box, *boxptr;

class Quantize
{
public:
    Quantize();
    Quantize(Bitmap *bitmap,
             long ncolors,
             const std::shared_ptr<Color> bgColor);
    ~Quantize();
    int desiredNumberOfColors;
    int actualNumberOfColors;
    Color cmap[256];
    ColorFreq freq[256];
    Histogram *histogram;
    void medianCutPass1Rgb(Bitmap *image, const Color *ignoreColor);
    void medianCutPass2Rgb(Bitmap *bitmap, const Color *bgColor);

private:
    void computeColorRgb (boxptr boxp,
                          int icolor);
    void fillInverseCmapRgb (int R, int G, int B);
    int findNearbyColors (int minR, int minG, int minB, int *colorList);
    void findBestColors (int minR,
                         int minG,
                         int minB,
                         int numcolors,
                         int *colorlist,
                         int *bestcolor);
    boxptr findBiggestVolume(boxptr boxlist, int numboxes);
    void generateHistogram (Bitmap *image, const Color *ignoreColor);
    int medianCutRgb (boxptr boxlist,
                      int numboxes,
                      int desiredColors);
    void selectColorsRgb ();
    void updateBoxRgb (boxptr boxp);
    void zeroHistogram ();
};

}

#endif // QUANTIZE_H

#include "distancemap.h"

namespace AutoTrace {

DistanceMap::DistanceMap(const DistanceMap &original)
{
    this->width = original.width;
    this->height = original.height;
    this->weight = original.weight;
    this->d = original.d;
}

/**
 * @brief DistanceMap::DistanceMap Original definition at
 * image-proc.c#L53. Orginally named new_distance_map.
 * @note Allocate storage for a new distance map with the same dimensions
 * as BITMAP and initialize it so that pixels in BITMAP with value
 * TARGET_VALUE are at distance zero and all other pixels are at
 * distance infinity.  Then compute the gray-weighted distance from
 * every non-target point to the nearest target point.
 * @param bitmap
 * @param targetValue
 * @param padded
 */
DistanceMap::DistanceMap(Bitmap *bitmap,
                         unsigned char targetValue,
                         bool padded)
{
    int x, y;
    float d, min;
    unsigned char *b = bitmap->getBitmap ();
    unsigned char w = bitmap->getWidth ();
    unsigned char h = bitmap->getHeight ();
    unsigned spp = bitmap->getPlanes ();

    this->height = h;
    this->width = w;

//    for (y = 0; y < (int)h; y++)
//    {
//	this->d[y] = w * sizeof(float);
//	this->weight[y] = w * sizeof(float);
//    }

    if (spp == 3)
    {
        for (y = 0; y < (int) h; y++)
        {
            for (x = 0; x < (int) w; x++, b += spp)
            {
                int gray;
                float fgray;
                gray = (int)(Color(b[0], b[1], b[0]).luminance ());
                this->d[y][x] = (gray == targetValue ? 0.0F : 1.0e10F);
                fgray = gray * 0.0039215686F; // = gray / 255.0F
                this->weight[y][x] = 1.0F - fgray;
            }
        }
    }
    else
    {
        for (y = 0; y < (int) h; y++)
        {
            for (x = 0; x < (int) w; x++, b += spp)
            {
                int gray;
                int fgray;
                gray = b[0];
                this->d[y][x] = (gray == targetValue ? 0.0F : 1.0e10F);
                fgray = gray * 0.0039215686F; // = gray / 255.0F
                this->weight[y][x] = 1.0F - fgray;
            }
        }
    }

    /* If the image is padded then border points are all at most
       one unit away from the nearest target point. */
    if (padded)
    {
        for (y = 0; y < (int) h; y++)
        {
            if (this->d[y][0] > this->weight[y][0])
            this->d[y][0] = this->weight[y][0];

            if (this->d[y][w - 1] > this->weight[y][w - 1])
            this->d[y][w - 1] = this->weight[y][w - 1];
        }

        for (x = 0; x < (int) w; x++)
        {
            if (this->d[0][x] > this->weight[0][x])
            this->d[0][x] = this->weight[0][x];

            if (this->d[h - 1][x] > this->weight[h - 1][x])
            this->d[h - 1][x] = this->weight[h - 1][x];
        }
    }

    /* Scan the image from left to right, top to bottom.
       Examine the already-visited neighbors of each point (those
       situated above or to the left of it).  Each neighbor knows
       the distance to its nearest target point; add to this distance
       the distance from the central point to the neighbor (either
       sqrt(2) or one) multiplied by the central point's weight
       (derived from its gray level).  Replace the distance already
       stored at the central point if the new distance is smaller. */
    for (y = 1; y < (int) h; y++)
    {
        for (x = 1; x < (int) w; x++)
        {
            if (this->d[y][x] == 0.0F)
            continue;

            min = this->d[y][x];

            // Upper-left neighbor
            d = this->d[y - 1][x - 1] + (float) M_SQRT2 * this->weight[y][x];

            if (d < min)
            min = this->d[y][x] = d;

            // Upper neighbor
            d = this->d[y - 1][x] + this->weight[y][x];
            if (d < min)
            min = this->d[y][x] = d;

            // Left neighbor
            d = this->d[y][x - 1] + this->weight[y][x];
            if (d < min)
            min = this->d[y][x] = d;

            // Upper-right neighbor (except at the last column)
            if (x + 1 < (int) w)
            {
            d = this->d[y - 1][x + 1] + (float) M_SQRT2 * this->weight[y][x];
            if (d < min)
                min = this->d[y][x] = d;
            }
        }
    }

    // Same as above, but now scanning right to left, bottom to top.
    for (y = h - 2; y >= 0; y--)
    {
        for (x = w - 2; x >= 0; x--)
        {
            min = this->d[y][x];

            // Lower-right neighbor
            d = this->d[y + 1][x + 1] + (float) M_SQRT2 * this->weight[y][x];
            if (d < min)
            min = this->d[y][x] = d;

            // Lower neighbor
            d = this->d[y + 1][x] + this->weight[y][x];
            if (d < min)
            min = this->d[y][x] = d;

            // Right neighbor
            d = this->d[y][x + 1] + this->weight[y][x];
            if (d < min)
            min = this->d[y][x] = d;

            // Lower-left neighbor (except at the first column
            if (x - 1 >= 0)
            {
            d = this->d[y + 1][x - 1] + (float) M_SQRT2 * this->weight[y][x];
            if (d < min)
                min = this->d[y][x] = d;
            }
        }
    }
}

DistanceMap::~DistanceMap ()
{
//    delete[] weight; // Having issues deleting crap...
//    delete[] d;
}

}

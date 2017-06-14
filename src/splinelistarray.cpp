#include "splinelistarray.h"

namespace AutoTrace
{

SplineListArray::SplineListArray()
{ }

// NOTE: SET DATA!
SplineListArray::SplineListArray(Bitmap &bitmap, FittingOptions *options)
{
    ImageHeader imageHeader;
    PixelOutlineList *pixels;
    Quantize *quant; // Currently not used?
    DistanceMap *distanceMap,
            *dist = nullptr;

    if (options->despeckleLevel > 0)
    {
        Despeckle::DespeckleBitmap(bitmap,
                                   options->despeckleLevel,
                                   options->despeckleTightness,
                                   options->noiseRemoval);
        return;
    }

    imageHeader.width = bitmap.getWidth ();
    imageHeader.height = bitmap.getHeight ();

    // This if statement is confusing... I've no clue why it intializes an object then immediantly destroys it. FLAGGED FOR REMOVAL
    if (options->colorCount > 0)
    {
        quant = new Quantize(&bitmap,
                             options->colorCount,
                             std::shared_ptr<Color>(options->background_color.get()));
        if (quant)
            delete quant;

        return;
    }

    if (options->centerline)
    {
        if (options->preserveWidth)
        {
            // Preserve line width prior to thinning
            delete distanceMap;
            distanceMap = new DistanceMap(&bitmap, 255, true);
            dist = distanceMap;
        }

        /* Hereafter, dist is allocated. dist must be freed if
         * the execution is canceled or exception is raised;
         * use FATAL_THEN_CLEANUP_DIST. */
        ThinImage::thinImage (&bitmap, options->background_color.get ());
    }

    /* Hereafter, pixels is allocated. pixels must be freed if
     * the execution is canceled; use CANCEL_THEN_CLEANUP_PIXELS. */
    if (options->centerline)
    {
        Color backgroundColor(0xff, 0xff, 0xff);
        if (options->background_color)
            backgroundColor = *options->background_color;

        pixels = new PixelOutlineList(&bitmap, backgroundColor); // find_centerline_pixels
    }
    else
    {
        pixels = new PixelOutlineList(&bitmap, options->background_color.get ()); // find_outline_pixels
    }

    auto splineListArray = fittedSplines (*pixels, options, dist, imageHeader.width, imageHeader.height);
    this->background = splineListArray->background;
    this->centerline = splineListArray->centerline;
    this->height = splineListArray->height;
    this->width = splineListArray->width;
    this->preserveWidth = splineListArray->preserveWidth;
    this->widthWeightFactor = splineListArray->widthWeightFactor;

    data.insert (data.end (), splineListArray->data.begin (), splineListArray->data.end ());

    if (splineListArray != nullptr)
        delete splineListArray;
//    if (distanceMap != nullptr)
//        delete distanceMap;
}

// TODO
SplineListArray *SplineListArray::fittedSplines(PixelOutlineList pixelOutlineList,
                                               FittingOptions *fittingOpts,
                                               DistanceMap *dist,
                                               unsigned short width,
                                               unsigned short height)
{
    unsigned thisList;

    SplineListArray *charSplines = new SplineListArray();
    CurveListArray curveArray(pixelOutlineList, fittingOpts);

    charSplines->centerline = fittingOpts->centerline;
    charSplines->preserveWidth = fittingOpts->preserveWidth;
    charSplines->widthWeightFactor = (bool)fittingOpts->widthWeightFactor;

    if (fittingOpts->background_color == nullptr)
        charSplines->background = fittingOpts->background_color.get();
    else
        charSplines->background = nullptr;

    // Set dummy values. Real value is set in upper context
    charSplines->width = width;
    charSplines->height = height;

    for (thisList = 0; thisList < curveArray.data.size(); thisList++)
    {
        SplineList *curveListSplines;
        CurveList *curves = curveArray.data[thisList];

        curveListSplines = new SplineList(*curves, fittingOpts, dist);
        curveListSplines->clockwise = curves->clockwise;
        curveListSplines->color = pixelOutlineList.data.at(thisList).color;

        charSplines->data.push_back (*curveListSplines);
    }

    curveArray.data.clear ();
    return charSplines;
}

SplineList &SplineListArray::elt (unsigned index)
{
    return this->data[index];
}

void SplineListArray::Write (OutputWriter *writer, // TODO autotrace.c#L391
                             FILE *writeto,
                             std::string filename,
                             OutputOptions *options)
{
    bool newOpts = false;
    int llx = 0,
            lly = 0,
            urx = this->width,
            ury = this->height;

    if (filename.empty ())
        throw "Error: filename was empty!";

    if (options == nullptr)
    {
        options = new OutputOptions();
        newOpts = true;
    }

    writer->func (writeto,
                  filename,
                  llx,
                  lly,
                  urx,
                  ury,
                  options,
                  *this);
}

unsigned SplineListArray::length()
{
    return this->width * this->height;
}

}

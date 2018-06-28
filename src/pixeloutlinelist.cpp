#include "pixeloutlinelist.h"

namespace AutoTrace {

PixelOutlineList::PixelOutlineList()
{ }

PixelOutlineList::PixelOutlineList(Bitmap *bitmap, Color bgColor)
{
    signed short row, col;
    Bitmap *marked = new Bitmap(bitmap->getWidth (), bitmap->getHeight (), 1);
    unsigned int maxProgress = bitmap->getHeight () * bitmap->getWidth ();

    this->data.clear ();

    auto height = bitmap->getHeight ();
    auto width = bitmap->getWidth ();
    for (row = 0; row < height; row++)
    {
        for (col = 0; col < width; )
        {
            Direction dir = EAST;
            bool clockwise = false;

            PixelOutline outline(bitmap, dir, row, col, marked);

            if (bitmap->equalColorAt (bgColor, row, col))
            {
                col++;
                continue;
            }

//            if (pixelOutline.isValidDirection (row, col, dir, bitmap, marked)
//                    || (!pixelOutline.isValidDirection (pixelOutline.computeRowDelta (dir) + row,
//                                                        pixelOutline.computeColDelta (dir) + col,
//                                                        dir,
//                                                        bitmap,
//                                                        marked)
//                        && pixelOutline.numberNeighbors (row, col, bitmap) > 2)
//                    || pixelOutline.numberNeighbors (row, col, bitmap) > 2
//                    || pixelOutline.numberNeighbors (pixelOutline.computeRowDelta (dir) + row,
//                                                     pixelOutline.computeColDelta (dir) + col,
//                                                     bitmap) > 4
//                    || (pixelOutline.isOtherDirectionMarked (row, col, dir, marked)
//                        && pixelOutline.isOtherDirectionMarked (row + pixelOutline.computeRowDelta (dir),
//                                                                col + pixelOutline.computeColDelta (dir),
//                                                                dir,
//                                                                marked)))
//            {
//                dir = SOUTHEAST;
//                if (pixelOutline.isValidDirection (row, col, dir, bitmap, marked)
//                        || (!pixelOutline.isValidDirection (pixelOutline.computeRowDelta (dir) + row,
//                                                            pixelOutline.computeColDelta (dir) + col,
//                                                            dir,
//                                                            bitmap,
//                                                            marked)
//                            && pixelOutline.numberNeighbors (row, col, bitmap) > 2)
//                        || pixelOutline.numberNeighbors (row, col, bitmap) > 4
//                        || pixelOutline.numberNeighbors (pixelOutline.computeRowDelta (dir) + row,
//                                                         pixelOutline.computeColDelta (dir) + col,
//                                                         bitmap) > 4
//                        || (pixelOutline.isOtherDirectionMarked (row, col, dir, marked)
//                            && pixelOutline.isOtherDirectionMarked (row + pixelOutline.computeRowDelta (dir),
//                                                                    col + pixelOutline.computeColDelta (dir),
//                                                                    dir,
//                                                                    marked)))
//                {
//                    dir = SOUTH;
//                }
//            }
            if (outline.isValid (row, col, dir, bitmap, marked))
            {
                dir = SOUTHEAST;
                if (outline.isValid (row, col, dir, bitmap, marked))
                {
                    dir = SOUTH;
                    if (outline.isValid (row, col, dir, bitmap, marked))
                    {
                        dir = SOUTHWEST;
                        if (outline.isValid (row, col, dir, bitmap, marked))
                        {
                            col++;
                            continue;
                        }
                    }
                }
            }

#ifdef DEBUG
            std::cout << "#"
                      << this->data.size ()
                      << ": ("
                      << (clockwise ? "" : "counter")
                      << "clockwise)"
                      << std::endl;
#endif

            outline = PixelOutline(bitmap, dir, row, col, marked);
            if (outline.open)
            {
                PixelOutline partialOutline;
                bool okay = false;

                if (dir == EAST)
                {
                    dir = SOUTH;
                    if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                    {
                        dir = SOUTHWEST;
                        if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                        {
                            dir = SOUTHEAST;
                            okay = outline.isValidDirection (row, col, dir, bitmap, marked);
                        }
                        else if (dir == SOUTHEAST)
                        {
                            dir = SOUTHWEST;
                            if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                            {
                                dir = EAST;
                                if(!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                                {
                                    dir = SOUTH;
                                    okay = outline.isValidDirection (row, col, dir, bitmap, marked);
                                }
                            }
                        }
                        else if (dir == SOUTH)
                        {
                            dir = EAST;
                            if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                            {
                                dir = SOUTHEAST;
                                if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                                {
                                    dir = SOUTHWEST;
                                    okay = outline.isValidDirection (row, col, dir, bitmap, marked);
                                }
                            }
                        }
                        else if (dir == SOUTHWEST)
                        {
                            dir = SOUTHEAST;
                            if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                            {
                                dir = EAST;
                                if (!(okay = outline.isValidDirection (row, col, dir, bitmap, marked)))
                                {
                                    dir = SOUTH;
                                    okay = outline.isValidDirection (row, col, dir, bitmap, marked);
                                }
                            }
                        }

                        if (okay)
                        {
                            partialOutline = PixelOutline(bitmap, dir, row, col, marked);
                            outline.concatPixelOutline (&partialOutline);

                            if (partialOutline.data.size () > 0)
                            {
                                partialOutline.data.clear ();
                            }
                        }
                        else
                        {
                            col++;
                        }
                    }
                    /* Outside outlines will start at a top edge, and move
                     * counterclockwise, and inside outlines will start at a
                     * bottom edge, and move clockwise.  This happens because of
                     * the order in which we look at the edges. */
                    outline.clockwise = clockwise;

                    if (outline.data.size () > 1)
                        outline.concatPixelOutline(&partialOutline); // Replace appendPixelOutline with concatPixelOutline

#ifdef DEBUG
                    std::cout << "("
                              << (outline.open ? " open:" : "closed")
                              << ")";
                    std::cout << " ["
                              << outline.data.size ()
                              << "]."
                              << std::endl;
#endif
                    if (outline.data.size () == 1)
                        outline.data.clear ();
                }
            }
        }
    }

    // TODO: If test cancel... pxl-outline.c#L420
    delete marked;
}

/**
 * @brief PixelOutlineList::PixelOutlineList Original definition found at
 * pxl-outline.c#L85. Originally named find_outline_pixels.
 * @param bitmap
 * @param bgColor
 */
PixelOutlineList::PixelOutlineList(Bitmap *bitmap, Color *bgColor)
{
    unsigned short row, col;
    Bitmap *marked = new Bitmap (bitmap->getWidth (), bitmap->getHeight (), 1);
//    unsigned int maxProgress = bitmap->getWidth () * bitmap->getHeight ();

    this->data.clear ();

    for (row = 0; row < bitmap->getHeight (); row++)
    {
        for (col = 0; col < bitmap->getWidth (); col++)
        {
            Edge edge;
            Color color;
            bool isBackground;
            PixelOutline *outline = new PixelOutline();

            // TODO: if (notify_progress) pxl-outline.c#L118

            /* A valid edge can be TOP for an outside outline.
             * Outside outlines are traced counterclockwise
             */
            color = bitmap->getColor (row, col);
            if (!(isBackground = (bool)(bgColor && color.equal (bgColor)))
                    && outline->isUnmarkedOutlineEdge (row, col, edge = TOP, bitmap, marked, color));
            {
                delete outline;
                outline = new PixelOutline(bitmap, edge, row, col, marked, false, false);
                this->appendPixelOutline (*outline);
#ifdef DEBUG
                std::cout << "@("
                          << row
                          << ", "
                          << col
                          << ") { data.size: "
                          << outline->data.size()
                          << ", counterclockwise: "
                          << "true"
                          << "}"
                          << std::endl;
#endif
            }

            /* A valid edge can be BOTTOM for an inside outline.
             * Inside outlines are traced clockwise */
            if (row != 0)
            {
                color = bitmap->getColor (row - 1, col);
                if (!(bgColor && color.equal (bgColor))
                        && outline->isUnmarkedOutlineEdge (row - 1, col, edge = BOTTOM, bitmap, marked, color))
                {
                    // These lines are for debugging only!
                    if (isBackground)
                    {
#ifdef DEBUG
                        std::cout << "#"
                                  << this->data.size ()
                                  << ": (clockwise)"
                                  << std::endl;
#endif
                        outline->clockwise = true;

#ifdef DEBUG
                        this->appendPixelOutline (*outline);
                        std::cout << " ["
                                  << outline->data.size()
                                  << "]."
                                  << std::endl;
#endif
                    }
                    else
                    {
                        delete outline;
                        outline = new PixelOutline(bitmap,
                                               edge,
                                               row,
                                               col,
                                               marked,
                                               true,
                                               true);
                    }
                }
//                else
//                {
//                    CHECK_FATAL ();
//                }
            }
            // TODO: If test cancel... pxl-outline.c#L183
            delete outline;
        }
    }

    // cleanup: // goto label
    if (marked != nullptr)
    {
        delete marked;
    }
}

void PixelOutlineList::appendPixelOutline(PixelOutline outline)
{
    data.push_back(outline);
}

}

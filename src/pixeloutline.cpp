#include "pixeloutline.h"

namespace AutoTrace {

PixelOutline::PixelOutline()
{
    open = false;
}

PixelOutline::PixelOutline(Bitmap *bitmap,
                           Direction searchDir,
                           unsigned short originalRow,
                           unsigned short originalCol,
                           Bitmap *marked)
{
    open = false;
    Direction originalDir = searchDir;
    unsigned short row = originalRow,
            col = originalCol;
    unsigned short prev_row, prev_col;
    Coord pos;

    this->color = bitmap->getColor(row, col);

    pos.x = col;
    pos.y = bitmap->getHeight() - row - 1;

    appendPixelOutline(pos);

    for ( ; ; )
    {
        prev_row = row;
        prev_col = col;

        /* If there is no adjacent, unmarked pixel, we can't proceed
           any further, so return an open outline. */
        if (!nextUnmarkedPixel(&row, &col, &searchDir, bitmap, marked))
        {
            open = true;
            break;
        }

        /* If we've moved to a new pixel, mark all edges of the previous
           pixel so that it won't be revisited. */
        if (!(prev_row == originalRow
              && prev_col == originalCol))
            markDirection(prev_row, prev_col, searchDir, marked);

        markDirection(row, col, (Direction)((searchDir + 4) % 8), marked);

        // If we've returned to the starting pixel, we're done
        if (row == originalRow && col == originalCol)
            break;

        // Add the new pixel to the output list
        pos.x = col;
        pos.y = bitmap->getHeight() - row - 1;
        appendPixelOutline(pos);
    }

    markDirection(originalRow, originalCol, originalDir, marked);
}

PixelOutline::PixelOutline(Bitmap *bitmap,
                           Edge originalEdge,
                           unsigned short originalRow,
                           unsigned short originalCol,
                           Bitmap *marked,
                           bool clockwise,
                           bool ignore)
{
    unsigned short row = originalRow,
            col = originalCol;
    Edge edge = originalEdge;
    Coord pos;

    pos.x = col + ((edge == RIGHT) || (edge == BOTTOM) ? 1 : 0);
    pos.y = bitmap->getHeight() - row - 1 +
            ((edge == TOP || (edge == RIGHT) ? 1 : 0));

    if (!ignore)
        open = false;

    this->color = bitmap->getColor(row, col);

    do
    {
        // Put this edge into the output list
        if (!ignore)
        {
            appendPixelOutline(pos);
        }

        markEdge(edge, row, col, marked);
        pos = nextPoint(bitmap, &edge, &row, &col, this->color, clockwise, marked);
    } while (edge != NO_EDGE);
}

PixelOutline::~PixelOutline ()
{}

void PixelOutline::appendPixelOutline(Coord c)
{
    this->data.push_back(c);
}

bool PixelOutline::isUnmarkedOutlineEdge(unsigned short row,
                                         unsigned short col,
                                         Edge edge,
                                         Bitmap *bitmap,
                                         Bitmap *marked,
                                         Color color)
{
    return (!isOutlineEdge(edge, bitmap, row, col, color)
                  && isMarkedEdge(edge, row, col, marked));
}

bool PixelOutline::isOutlineEdge(Edge edge,
                                 Bitmap *bitmap,
                                 unsigned short row,
                                 unsigned short col,
                                 Color color)
{
    // If this pixel isn't of the same color, it's not part of the outline
    if (!bitmap->equalColorAt(color, row, col))
        return false;

    switch (edge)
    {
    case LEFT:
        return (col == 0
                      || !bitmap->equalColorAt(color, row, col - 1));
    case TOP:
        return (row == 0
                      || !bitmap->equalColorAt(color, row - 1, col));
    case RIGHT:
        return (col == bitmap->getWidth () - 1
                      || !bitmap->equalColorAt(color, row, col + 1));
    case BOTTOM:
        return (row == bitmap->getHeight () - 1
                      || !bitmap->equalColorAt(color, row + 1, col));
    case NO_EDGE:
        throw "Error in isOutlineEdge: edge was NO_EDGE!";
    default:
        throw "Error in isOutlineEdge: edge wasn't of supported type!";
    }

//    return false; // This shouldn't be reached!
}

bool PixelOutline::isMarkedEdge(Edge edge,
                                unsigned short row,
                                unsigned short col,
                                Bitmap *marked)
{
    return (bool)(edge == NO_EDGE
                  ? false : (marked->atPixel(row, col) & (1 << edge)) != 0);
}

bool PixelOutline::isMarkedDirection(Direction dir,
                                     unsigned short row,
                                     unsigned short col,
                                     Bitmap *marked)
{
    return (bool) ((marked->atPixel(row, col) & 1 << dir) != 0);
}

bool PixelOutline::nextUnmarkedPixel(unsigned short *row,
                                     unsigned short *col,
                                     Direction *dir,
                                     Bitmap *bitmap,
                                     Bitmap *marked)
{
    unsigned short originalRow = *row;
    unsigned short originalCol = *col;
    Direction originalDirection = *dir;
    Direction testDirection = *dir;

    do
    {
        if (isValidDirection(originalRow,
                             originalCol,
                             testDirection,
                             bitmap,
                             marked))
        {
            *row = originalRow + computeRowDelta(testDirection);
            *col = originalCol + computeColDelta(testDirection);
            *dir = testDirection;
            break;
        }

        if (originalDirection == testDirection)
            testDirection = (Direction)((originalDirection + 2) % 8);
        else if ((originalDirection + 2) % 8 == testDirection)
            testDirection = (Direction)((originalDirection + 6) % 8);
        else if ((originalDirection + 6) % 8 == testDirection)
            testDirection = (Direction)((originalDirection + 1) % 8);
        else if ((originalDirection + 1) % 8 == testDirection)
            testDirection = (Direction)((originalDirection + 7) % 8);
        else if ((originalDirection + 7) % 8 == testDirection)
            testDirection = (Direction)((originalDirection + 3) % 8);
        else if ((originalDirection + 3) % 8 == testDirection)
            testDirection = (Direction)((originalDirection + 5) % 8);
        else if ((originalDirection + 5) % 8 == testDirection)
            break;
    } while(1);

    if ((*row != originalRow || *col != originalCol)
            && (!(isOtherDirectionMarked (originalRow,
                                         originalCol,
                                         testDirection,
                                         marked)))
            && isOtherDirectionMarked (originalRow + computeRowDelta(testDirection),
                                      originalCol + computeColDelta(testDirection),
                                      testDirection,
                                      marked))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PixelOutline::isOtherDirectionMarked (unsigned short row, unsigned short col, Direction dir, Bitmap *marked)
{
    return (bool)((marked->atPixel(row, col) & (255 - (1 << dir) - (1 << (dir + 4) % 8))) != 0);
}

bool PixelOutline::isValidDirection(unsigned short row,
                                    unsigned short col,
                                    Direction dir,
                                    Bitmap *bitmap,
                                    Bitmap *marked)
{
    auto c = bitmap->getColor(this->computeRowDelta(dir) + row, computeColDelta(dir) + col);
    return ((bool)(!isMarkedDirection(dir, row, col, marked)
                   && computeRowDelta(dir) + row > 0
                   && computeColDelta(dir) + col > 0
                   && bitmap->validPixel(row, col)
                   && bitmap->equalColorAt(c, row, col)));
}

/* If EDGE is not already marked, we mark it; otherwise, it's a fatal error.
   The position ROW and COL should be inside the bitmap MARKED. EDGE can be
   NO_EDGE. */
void PixelOutline::markEdge(Edge edge,
                            unsigned short row,
                            unsigned short col,
                            Bitmap *marked)
{
    marked->atPixel(row, col) |= 1 << edge;
}

// Mark the direction of the pixel ROW/COL in MARKED.
void PixelOutline::markDirection(unsigned short row,
                                 unsigned short col,
                                 Direction dir,
                                 Bitmap *marked)
{
    marked->atPixel(row, col) |= 1 << dir;
}

/* Concatenate two pixel lists. The two lists are assumed to have the
   same starting pixel and to proceed in opposite directions therefrom. */
void PixelOutline::concatPixelOutline(const PixelOutline *outline)
{
    int src, dst;
    unsigned o1_length, o2_length;

    if (this->data.size() <= 0 || !outline || outline->data.size () <= 0)
        return;

    o1_length = this->data.size();
    o2_length = outline->data.size();

    /* Resize o1 to the sum of the lengths of o1 and o2 minus one (because
     the two lists are assumed to share the same starting pixel). */

//    Shift the contents of o1 to the end of the new array to make room
//         to prepend o2.
    for (src = int (o1_length - 1), dst = int (this->data.size() - 1); src >= 0; src--, dst--)
    {
        this->data[dst] = outline->data[src];
    }

    for (src = o2_length - 1, dst = 0; src > 0; src--, dst++)
    {
        this->data[dst] = outline->data[src];
    }
}

// Return the number of pixels adjacent to pixel ROW/COL that are black.
unsigned PixelOutline::numberNeighbors(unsigned short row,
                                       unsigned short col,
                                       Bitmap *bitmap)
{
    unsigned dir, count = 0;
    Color color = bitmap->getColor(row, col);

    for (dir = NORTH; dir <= NORTHEAST; dir++)
    {
        Direction d = static_cast<Direction>(dir);

        int delta_r = computeRowDelta(d);
        int delta_c = computeColDelta(d);

        unsigned int testRow = row + delta_r;
        unsigned int testCol = col + delta_c;

        if (bitmap->validPixel(testRow, testCol)
                && bitmap->equalColorAt(color, testRow, testCol))
        {
            ++count;
        }
    }

    return count;
}

Coord PixelOutline::nextPoint(Bitmap *bitmap,
                              Edge *edge,
                              unsigned short *row,
                              unsigned short *col,
                              Color color,
                              bool clockwise,
                              Bitmap *marked)
{
    Coord pos = {0, 0};

    if (!clockwise)
    {
        switch(*edge)
        {
        case TOP:
            // WEST
            if ((*col >= 1
                 && !isMarkedEdge(TOP, *row, *col - 1, marked)
                 && isOutlineEdge(TOP, bitmap, *row, *col - 1, color)))
            {
                // edge = TOP
                (*col)--;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }
            // NORTHWEST
            else if ((*col >= 1 && *row >= 1
                      && !isMarkedEdge(RIGHT, *row - 1, *col - 1, marked)
                      && isOutlineEdge(RIGHT, bitmap, *row - 1, *col, color))
                     && !(isMarkedEdge(LEFT, *row - 1, *col, marked)
                          && (isMarkedEdge(TOP, *row, *col - 1, marked)))
                     && !(isMarkedEdge(BOTTOM, *row - 1, *col, marked)
                          && (isMarkedEdge(RIGHT, *row, *col - 1, marked))))
            {
                *edge = RIGHT;
                (*col)--;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }
            else if ((!isMarkedEdge(LEFT, *row, *col, marked)
                      && isOutlineEdge(LEFT, bitmap, *row, *col, color)))
            {
                *edge = LEFT;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            *edge = NO_EDGE;
            break;

        case RIGHT:
            // NORTH
            if ((*row >= 1
                 && !isMarkedEdge (RIGHT, *row - 1, *col, marked)
                 && isOutlineEdge(RIGHT, bitmap, *row - 1, *col, color)))
            {
                // *edge = RIGHT;
                (*row)--;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row;
                break;
            }
            // NORTHEAST
            else if ((*col + 1 < marked->getWidth() && *row >= 1
                 && !isMarkedEdge(BOTTOM, *row - 1, *col + 1, marked)
                 && isOutlineEdge(BOTTOM, bitmap, *row - 1, *col + 1, color))
                    && !(isMarkedEdge(LEFT, *row, *col + 1, marked)
                         && isMarkedEdge(BOTTOM, *row - 1, *col, marked))
                    && !(isMarkedEdge(TOP, *row, *col + 1, marked)
                         && isMarkedEdge(RIGHT, *row - 1, *col, marked)))
            {
                *edge = BOTTOM;
                (*col)++;
                (*row)--;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            else if ((!isMarkedEdge(TOP, *row, *col, marked)
                 && isOutlineEdge(TOP, bitmap, *row, *col, color)))
            {
                *edge = TOP;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            *edge = NO_EDGE;
            break;
        case BOTTOM:
            // EAST
            if ((*col + 1 < marked->getWidth())
                    && !isMarkedEdge(BOTTOM, *row, *col + 1, marked)
                    && isOutlineEdge(BOTTOM, bitmap, *row, *col + 1, color))
            {
                // *edge = BOTTOM;
                (*col)++;
                pos.x = *col+1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }
            // SOUTHEAST
            else if ((*col + 1 < marked->getWidth()
                 && *row + 1 < marked->getHeight()
                 && !isMarkedEdge(LEFT, *row + 1, *col + 1, marked)
                 && isOutlineEdge(LEFT, bitmap, *row + 1, *col + 1, color))
                    && !(isMarkedEdge(TOP, *row + 1, *col, marked)
                         && isMarkedEdge(LEFT, *row, *col + 1, marked))
                    && !(isMarkedEdge(RIGHT, *row + 1, *col, marked)
                         && isMarkedEdge(BOTTOM, *row, *col + 1, marked)))
            {
                *edge = LEFT;
                (*col)++;
                (*row)++;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            else if ((!isMarkedEdge(RIGHT, *row, *col, marked)
                 && isOutlineEdge(RIGHT, bitmap, *row, *col, color)))
            {
                *edge = RIGHT;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight()- *row;
                break;
            }

            *edge = NO_EDGE;

        case LEFT:
            // SOUTH
            if ((*row + 1 < marked->getHeight()
                 && !isMarkedEdge(LEFT, *row + 1, *col, marked)
                 && isOutlineEdge (LEFT, bitmap, *row + 1, *col, color)))
            {
                // *edge = LEFT;
                (*row)++;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            // SOUTHWEST
            else if ((*col >= 1 && *row + 1 < marked->getHeight()
                 && !isMarkedEdge(TOP, *row + 1, *col - 1, marked)
                 && isOutlineEdge(TOP, bitmap, *row + 1, *col - 1, color))
                    && !(isMarkedEdge(RIGHT, *row, *col - 1, marked)
                         && isMarkedEdge(TOP, *row + 1, *col, marked))
                    && !(isMarkedEdge(BOTTOM, *row, *col - 1, marked)
                         && isMarkedEdge(LEFT, *row + 1, *col, marked)))
            {
                *edge = TOP;
                (*col)--;
                (*row)++;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            else if ((!isMarkedEdge(BOTTOM, *row, *col, marked)
                 && isOutlineEdge(BOTTOM, bitmap, *row, *col, color)))
            {
                *edge = BOTTOM;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

        case NO_EDGE:
        default:
            *edge = NO_EDGE;
            break;
        }
    }
    else
    {
        switch (*edge)
        {
        case TOP:
            if ((!isMarkedEdge(LEFT, *row, *col, marked)
                 && isOutlineEdge(LEFT, bitmap, *row, *col, color)))
            {
                *edge = LEFT;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            // WEST
            if ((*col >= 1
                 && !isMarkedEdge(TOP, *row, *col - 1, marked)
                 && isOutlineEdge(TOP, bitmap, *row, *col - 1, color)))
            {
                // *edge = TOP;
                (*col)--;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            // NORTHWEST
            if ((*col >= 1&& *row >= 1
                 && !isMarkedEdge(RIGHT, *row - 1, *col - 1, marked)
                 && isOutlineEdge(RIGHT, bitmap, *row - 1, *col - 1, color)))
            {
                *edge = RIGHT;
                (*col)--;
                (*row)--;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            *edge = NO_EDGE;
            break;

        case RIGHT:
            if ((!isMarkedEdge(TOP, *row, *col, marked)
                 && isOutlineEdge(TOP, bitmap, *row, *col, color)))
            {
                *edge = TOP;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            // NORTH
            if ((*row >= 1
                 && !isMarkedEdge(RIGHT, *row - 1, *col, marked)
                 && !isOutlineEdge(RIGHT, bitmap, *row - 1, *col, color)))
            {
                // *edge = RIGHT
                (*row)--;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            // NORTHEAST
            if ((*row + 1 < marked->getWidth() && *row >= 1
                 && !isMarkedEdge(BOTTOM, *row - 1, *col + 1, marked)
                 && isOutlineEdge(BOTTOM, bitmap, *row - 1, *col + 1, color)))
            {
                *edge = BOTTOM;
                (*col)++;
                (*row)--;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            *edge = NO_EDGE;
            break;

        case BOTTOM:
            if ((!isMarkedEdge(RIGHT, *row, *col, marked)
                 && isOutlineEdge(RIGHT, bitmap, *row, *col, color)))
            {
                *edge = RIGHT;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

            // EAST
            if ((*col + 1 < marked->getWidth()
                 && !isMarkedEdge(BOTTOM, *row, *col + 1, marked)
                 && isOutlineEdge(BOTTOM, bitmap, *row, *col + 1, color)))
            {
                // *edge = BOTTOM;
                (*col)++;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            // SOUTHEAST
            if ((*col + 1 < marked->getWidth()
                 && *row + 1 < marked->getHeight()
                 && !isMarkedEdge(LEFT, *row + 1, *col + 1, marked)
                 && isOutlineEdge(LEFT, bitmap, *row + 1, *col + 1, color)))
            {
                *edge = LEFT;
                (*col)++;
                (*row)++;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            *edge = NO_EDGE;
            break;

        case LEFT:
            if ((!isMarkedEdge(BOTTOM, *row, *col, marked)
                 && isOutlineEdge(BOTTOM, bitmap, *row, *col, color)))
            {
                *edge = BOTTOM;
                pos.x = *col + 1;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            // SOUTH
            if ((*row + 1 < marked->getHeight()
                 && !isMarkedEdge(LEFT, *row + 1, *col, marked)
                 && isOutlineEdge(LEFT, bitmap, *row + 1, *col, color)))
            {
                // *edge = LEFT;
                (*row)++;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row - 1;
                break;
            }

            // SOUTHWEST
            if ((*col >= 1 && *row + 1 < marked->getHeight()
                 && !isMarkedEdge(TOP, *row + 1, *col - 1, marked)
                 && isOutlineEdge(TOP, bitmap, *row + 1, *col - 1, color)))
            {
                *edge = TOP;
                (*col)--;
                (*row)++;
                pos.x = *col;
                pos.y = bitmap->getHeight() - *row;
                break;
            }

        case NO_EDGE:
        default:
            *edge = NO_EDGE;
            break;
        }
    }
    return pos;
}

int PixelOutline::computeRowDelta(Direction dir)
{
  if (dir == NORTH)
    return -1;
  else if (dir == SOUTH)
    return 1;
  else
    return 0;
}

int PixelOutline::computeColDelta(Direction dir)
{
  if (dir == WEST)
    return -1;
  else if (dir == EAST)
    return 1;
  else
    return 0;
}

bool PixelOutline::isValid (unsigned short row,
                            unsigned short col,
                            Direction dir,
                            Bitmap *bitmap,
                            Bitmap *marked)
{
    if (!isValidDirection (row, col, dir, bitmap, marked)
            || (!isValidDirection (computeRowDelta (dir) + row,
                                                computeColDelta (dir) + col,
                                                dir,
                                                bitmap,
                                                marked)
                && numberNeighbors (row, col, bitmap) > 2)
            || numberNeighbors (row, col, bitmap) > 4
            || numberNeighbors (computeRowDelta (dir) + row,
                                             computeColDelta (dir) + col,
                                             bitmap) > 4
            || (isOtherDirectionMarked (row, col, dir, marked)
                && isOtherDirectionMarked (row + computeRowDelta (dir),
                                                        col + computeColDelta (dir),
                                                        dir,
                                                        marked)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

}

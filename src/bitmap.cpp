#include "bitmap.h"

namespace AutoTrace {

Bitmap::Bitmap(const std::vector<unsigned char> area,
               unsigned short width,
               unsigned short height,
               unsigned int planes)
{
    this->bitmap = area;

    if (0 == (width * height))
        this->bitmap.clear();
    else if (bitmap.size != width * height)
        this->bitmap.reserve (width * height * np);

    this->width = width;
    this->height = height;
    this->np = planes;
    unsigned int row = 0;
    unsigned int col = 0;

    for (auto it = area.begin (); it != area.end();)
    {
        Color color;

        // Increment
        if (this->np == 1)
        {
            auto r = *it++;
            auto g = *it++;
            auto b = *it++;
            color = color(r, g, b);
        }
        else
        {
            auto m = *it++;
            color = m == 0 ? Color(0, 0, 0) : Color(255, 255, 255);
        }

        auto coord = Coord { x = col, y = row };

        this->colors.insert (coord, color);

        // Increment col/row.
        if (col == this->width)
        {
            col = 0;
            row++;
        }
        else
        {
            col++;
        }
    }

    switch(this->np)
    {
    case 1:
    {
        for (auto it = area.begin (); it != area.end();)
        {
            auto isBlack = *it++;

            auto color = isBlack == 0 ? Color(0, 0, 0) : Color(255, 255, 255);
            auto coord = Coord { x = col, y = row };

            this->colors.insert (coord, color);

            // Increment col/row.
            if (col == this->width)
            {
                col = 0;
                row++;
            }
            else
            {
                col++;
            }
        }
        break;
    }
    case 3:
    {

        break;
    }
    }
}

Bitmap::Bitmap(const Bitmap &original)
{
    this->width = original.width;
    this->height = original.height;
    this->np = original.np;
    this->bitmap = original.bitmap;
}

Color Bitmap::getColor (unsigned int row, unsigned int col)
{
    unsigned char *p;
    Color color;

    p = this->bitmap + row * this->np * this->width + col * this->np;

    if (this->np >= 3)
        color.set (p[0], p[1], p[2]);
    else
        color.set (p[0], p[0], p[0]);

    return color;
}

bool Bitmap::equalColorAt (Color color, unsigned int row, unsigned int col)
{
    Color c(this->getColor (row, col));
    return c.equal (&color);
}

unsigned short Bitmap::getHeight ()
{
    return this->height;
}

unsigned short Bitmap::getWidth ()
{
    return this->width;
}

unsigned int Bitmap::getPlanes ()
{
    return this->np;
}

std::vector<unsigned char> Bitmap::getBitmap () const
{
    return this->bitmap;
}

bool Bitmap::isOutlineEdge(Edge edge, unsigned short row, unsigned short col, Color color)
{
    // If this pixel isn't of the same color, it's not part of the outline
    if (!this->equalColorAt(color, row, col))
        return false;

    switch (edge)
    {
    case LEFT:
        return (bool)(col == 0 || !this->equalColorAt(color, row - 1, col));
    case TOP:
        return (bool)(row == 0 || !this->equalColorAt(color, row, col));
    case RIGHT:
        return (bool)(col == this->width - 1 || this->equalColorAt(color, row, col + 1));
    case BOTTOM:
        return (bool)(row == this->height - 1 || !this->equalColorAt(color, row + 1, col));
    case NO_EDGE:
        throw "Error in isOutlineEdge: the edge was of type NO_EDGE!";
    default:
        throw "Error in isOutlineEdge: the edge was not of any acceptable types!";
    }

    return false; // This shouldn't be reached!
}

unsigned char& Bitmap::atPixel(unsigned int row, unsigned int col)
{
    return this->bitmap[row * this->np * this->width
            + col * this->np];

//    return ((*bitmap) + (row) * (np) * (width)
//            + (col) * (np));
}

bool Bitmap::validPixel(unsigned int row, unsigned int col)
{
    return bool (((row) < this->height) && ((col) < this->width));
}

void Bitmap::Binarize ()
{
    unsigned i, npixels, spp;
    unsigned char *b;

    if (this->bitmap == nullptr)
        throw "Error: Cannot binarize NULL bitmap!";

    b = this->bitmap;
    spp = this->np;
    npixels = this->height * this->width;

    if (spp == 1)
    {
        for (i = 0; i < npixels; i++)
        {
            b[i] = (b[i] > GRAY_THRESHOLD ? WHITE : BLACK);
        }
    }
    else if (spp == 3)
    {
        unsigned char *rgb = b;
        for (i = 0; i < npixels; i++, rgb += 3)
        {
            b[i] = (Color(rgb[0], rgb[1], rgb[2]).luminance () > GRAY_THRESHOLD
                    ? WHITE : BLACK);
        }

        this->np = 1;
    }
    else
    {
        throw "Error in Bitmap::Binarize(): The image had an unsupported number of planes!";
    }
}

}

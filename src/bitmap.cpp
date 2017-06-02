#include "bitmap.h"

namespace AutoTrace {

Bitmap::Bitmap()
{
    init(nullptr, 0,0,0);
}

Bitmap::Bitmap(unsigned char *area,
               unsigned short width,
               unsigned short height,
               unsigned int planes)
{
    init(area, width, height, planes);
}

Bitmap::Bitmap(unsigned short width,
               unsigned short height,
               unsigned int planes)
{
    init (nullptr, width, height, planes);
}

Bitmap::Bitmap(InputReader *reader,
               std::string filename,
               InputOptions *opts)
{
    bool newOptions = false;

    if (opts == nullptr)
    {
        opts = new InputOptions();
        newOptions = true;
    }

    // FIXME!
    this->bitmap = reader->func (filename, opts, reader->data);

    if (newOptions)
    delete opts;
}

Bitmap::Bitmap(const Bitmap &original)
{
    this->width = original.width;
    this->height = original.height;
    this->np = original.np;
    this->bitmap = original.bitmap;
}

Bitmap::~Bitmap ()
{
    delete bitmap;
    bitmap = nullptr; // Set pointer to null after deleting it... this is good practice?
}

void Bitmap::init(unsigned char *area,
                  unsigned short width,
                  unsigned short height,
                  unsigned int planes)
{
//    if (bitmap != nullptr)
//        delete bitmap;

    if (area)
    {
        this->bitmap = area;
    }
    else
    {
        if (0 == (width * height))
            this->bitmap = nullptr;
        else
            this->bitmap = new unsigned char[width * height * planes * sizeof(unsigned char)];
    }

    this->width = width;
    this->height = height;
    this->np = planes;
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

unsigned char* Bitmap::getBitmap () const
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

#include "color.h"

#include <stdio.h>
#include <string.h>
namespace AutoTrace {

Color::Color()
{
    this->set (0,0,0);
}

Color::Color(unsigned char r,
             unsigned char g,
             unsigned char b)
{
    this->set (r, g, b);
}

Color::Color(const std::string string)
{
//    if (string.size() !)
//        throw "Error in Color constructor: const char *string was NULL";
    if (string.size() != 6)
        throw "Error in Color constructor: const char *string didn't have exactly SIX characters in it!";

    unsigned char c[6];
    int i;

    for (i = 0; i < 6; i++)
    {
    c[i] = Color::hctoi (string[i]);
    }

    this->set((unsigned char)(16 * c[0] + c[1]),
              (unsigned char)(16 * c[0] + c[1]),
              (unsigned char)(16 * c[0] + c[1]));
}

Color::Color(const Color &original)
{
    this->set(original.r,
              original.g,
              original.b);
}

Color::~Color ()
{ }

bool Color::equal (const Color *color)
{
    if (this == color ||
            (this->r == color->r &&
             this->g == color->g &&
             this->b == color->b))
        return true;
    else
        return false;
}

void Color::set (unsigned char r, unsigned char g, unsigned char b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

unsigned char Color::getR () const
{
    return this->r;
}

unsigned char Color::getG () const
{
    return this->g;
}

unsigned char Color::getB () const
{
    return this->b;
}

unsigned char Color::luminance ()
{
    return ((unsigned char) ((this->r) * 0.30 + (this->g) * 0.59 + (this->b) * 0.11 + 0.5));
}

unsigned int Color::hctoi (const char &c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'a':
    case 'A':
        return 10;
    case 'b':
    case 'B':
        return 11;
    case 'c':
    case 'C':
        return 12;
    case 'd':
    case 'D':
        return 13;
    case 'e':
    case 'E':
        return 14;
    case 'f':
    case 'F':
        return 15;
    default:
        throw "Error in Color::hctoi(char c): c wasn't in the range of 0..15!";
    }
}

}

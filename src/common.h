#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <stdio.h>
#include <vector>

namespace AutoTrace {

struct Coord
{
    unsigned short x,y;
};

struct ImageHeader
{
    unsigned short hres, vres;
    unsigned short width, height;
    unsigned short depth;
    unsigned format;
};

struct FileName
{
    std::string FullName;
    std::string Name;
    std::string Suffix;
    std::string Path;
    FILE *File;
};

enum Edge : unsigned
{
    TOP = 1,
    LEFT = 2,
    BOTTOM = 3,
    RIGHT = 0,
    NO_EDGE = 4
};

enum Direction : unsigned
{
    NORTH = 0,
    NORTHWEST = 1,
    WEST = 2,
    SOUTHWEST = 3,
    SOUTH = 4,
    SOUTHEAST = 5,
    EAST = 6,
    NORTHEAST = 7
};

enum PolynomialDegree
{
    LINEAR = 1,
    QUADRATIC = 2,
    CUBIC = 3,
    PARALLELELLIPSE = 4,
    ELLIPSE = 5,
    CIRCLE = 6
};

}

#endif // COMMON_H

#ifndef PIXELDATA_H
#define PIXELDATA_H

#include <QColor>
#include <QPoint>

struct PixelData
{
    QColor color;
    QPoint coord;

    // Might remove these fields...
    bool curveStart;
    bool curveEnd;
};

#endif // PIXELDATA_H

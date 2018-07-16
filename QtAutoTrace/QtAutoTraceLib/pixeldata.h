#pragma once

#include <QColor>
#include <QPoint>

#include "qtautotracelib_global.h"

struct QTAUTOTRACELIBSHARED_EXPORT PixelData
{
    QColor color;
    QPoint coord;

    // Might remove these fields...
    bool curveStart;
    bool curveEnd;
};

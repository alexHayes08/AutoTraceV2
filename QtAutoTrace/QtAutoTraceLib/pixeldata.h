#pragma once

#include <QColor>
#include <QPoint>

#include "qtautotracelib_global.h"

struct QTAUTOTRACELIBSHARED_EXPORT PixelData
{
    QColor color;
    QPoint coord;
};

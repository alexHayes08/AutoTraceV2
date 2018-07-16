#pragma once

#include "notimplementedexception.h"
#include "pixelgroup.h"
#include "qtautotracelib_global.h"
#include "svgelement.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT SvgElementFactory
{
public:
    SvgElementFactory();
    SvgElement *createElement(PixelGroup pixels) const;

private:
    int tolerance;
};

}

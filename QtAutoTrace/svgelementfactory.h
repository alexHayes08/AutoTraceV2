#ifndef SVGELEMENTFACTORY_H
#define SVGELEMENTFACTORY_H

#include "pixelgroup.h"
#include "svgelement.h"

namespace QtAutoTraceV2
{

class SvgElementFactory
{
public:
    SvgElementFactory();
    SvgElement createElement(PixelGroup pixels) const;

private:
    int tolerance;
};

}

#endif // SVGELEMENTFACTORY_H

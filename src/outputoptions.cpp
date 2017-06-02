#include "outputoptions.h"

namespace AutoTrace {

OutputOptions::OutputOptions()
{
    this->dpi = AT_DEFAULT_DPI;
}

OutputOptions::OutputOptions(const OutputOptions &original)
{
    this->dpi = original.dpi;
}

OutputOptions::~OutputOptions ()
{

}

}

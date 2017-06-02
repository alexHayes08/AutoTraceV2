#ifndef INPUTOPTIONS_H
#define INPUTOPTIONS_H

#include "color.h"

#include <memory>

namespace AutoTrace {

class InputOptions
{
public:
    InputOptions();
    InputOptions(const InputOptions &original);
    ~InputOptions();
    std::shared_ptr<Color> backgroundColor;
    unsigned char charchode;
};

}

#endif // INPUTOPTIONS_H

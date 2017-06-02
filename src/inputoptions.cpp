#include "inputoptions.h"

namespace AutoTrace {

InputOptions::InputOptions()
{
    this->backgroundColor = nullptr;
    this->charchode = 0;
}

InputOptions::InputOptions(const InputOptions &original)
{
    this->backgroundColor.reset (original.backgroundColor.get ());
    this->charchode = original.charchode;
}

InputOptions::~InputOptions ()
{
    // Can't delete this pointer because it's not initiallized by this class
//    delete backgroundColor;
}

}

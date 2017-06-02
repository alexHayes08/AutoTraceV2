#ifndef OUTPUTOPTIONS_H
#define OUTPUTOPTIONS_H

#define AT_DEFAULT_DPI 72

namespace AutoTrace {

class OutputOptions
{
public:
    OutputOptions();
    OutputOptions(const OutputOptions &original);
    ~OutputOptions();
    int dpi;
};

}

#endif // OUTPUTOPTIONS_H

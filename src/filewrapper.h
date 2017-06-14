#ifndef FILEWRAPPER_H
#define FILEWRAPPER_H

#include <stdio.h>
#include <string>

namespace AutoTrace
{

class FileWrapper
{
public:
    FileWrapper(std::string inputFile);
    std::string fullName();
    std::string name();
    std::string suffix();
    FILE *file;
private:
    std::string _fullName;
    std::string _name;
    std::string _suffix;
};

}

#endif // FILEWRAPPER_H

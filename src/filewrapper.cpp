#include "filewrapper.h"

namespace AutoTrace {

FileWrapper::FileWrapper(std::string inputFile)
{
    // FULL NAME
    this->_fullName = inputFile;

    auto posOfFirstDot = inputFile.find_first_of('.');
    auto posOfLastDot = inputFile.find_last_of('.');
    if (posOfFirstDot != std::string::npos
            || posOfLastDot != std::string::npos)
    {
    // NAME
    // Handle paths
    if (inputFile.find_first_of('\\') != std::string::npos
            || inputFile.find_first_of('/') != std::string::npos)
    {
        auto lastSlash =
                (inputFile.find_last_of('/') == std::string::npos
                 ? inputFile.find_last_of('\\')
                 : inputFile.find_last_of('/'));
        posOfFirstDot = inputFile.substr(lastSlash + 1, inputFile.size())
                .find_first_of('.');
        this->_name = inputFile.substr(lastSlash + 1, posOfFirstDot);
    }
    else
    {
        // Not path
        this->_name = inputFile.substr(0, posOfFirstDot);
    }

    // SUFFIX
    this->_suffix = inputFile.substr(posOfLastDot + 1, inputFile.size());
//    ToLowerCase(this->_suffix);
    }
    else
    {
    this->_name = inputFile;
    this->_suffix = "";
    }
}

std::string FileWrapper::fullName()
{
    return this->_fullName;
}

std::string FileWrapper::name()
{
    return this->_name;
}

std::string FileWrapper::suffix()
{
    return this->_suffix;
}

}

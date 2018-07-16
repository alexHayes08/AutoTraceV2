#pragma once

#include <QException>

#include "qtautotracelib_global.h"
namespace QtAutoTraceV2
{

/**
 * @brief The FileReadException class is thrown when the file exists but the
 * reader failed to interpret the files' contents.
 */
class QTAUTOTRACELIBSHARED_EXPORT FileReadException : public QException
{
public:
    void raise() const { throw *this; }
    FileReadException *clone() const
    {
        return new FileReadException(*this);
    }
    const char* what() const throw() {
        return "Error occurred trying to read the file.";
    }
};

}

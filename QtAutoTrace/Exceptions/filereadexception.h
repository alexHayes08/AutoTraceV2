#ifndef FILEREADEXCEPTION_H
#define FILEREADEXCEPTION_H

#include <QException>

namespace QtAutoTraceV2
{

/**
 * @brief The FileReadException class is thrown when the file exists but the
 * reader failed to interpret the files' contents.
 */
class FileReadException : public QException
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

#endif // FILEREADEXCEPTION_H

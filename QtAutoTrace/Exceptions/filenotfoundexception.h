#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <QException>

namespace QtAutoTraceV2
{

class FileNotFoundException : public QException
{
public:
    void raise() const { throw *this; }
    FileNotFoundException *clone() const
    {
        return new FileNotFoundException(*this);
    }
};

#endif // FILENOTFOUNDEXCEPTION_H

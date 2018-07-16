#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <QException>

#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

/**
 * @brief The FileNotFoundException class is similar to the C#
 * FileNotFoundException class.
 */
class QTAUTOTRACELIBSHARED_EXPORT FileNotFoundException : public QException
{
public:
    void raise() const { throw *this; }
    FileNotFoundException *clone() const
    {
        return new FileNotFoundException(*this);
    }
};

#endif // FILENOTFOUNDEXCEPTION_H

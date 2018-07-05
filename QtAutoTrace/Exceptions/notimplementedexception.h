#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define NOTIMPLEMENTEDEXCEPTION_H

#include <QException>

namespace QtAutoTraceV2
{

/**
 * @brief The NotImplementedException class is similar to C#'s
 * NotImplementedException class.
 */
class NotImplementedException : public QException
{
public:
    void raise() const { throw *this; }
    NotImplementedException *clone() const
    {
        return new NotImplementedException(*this);
    }
};

}

#endif // NOTIMPLEMENTEDEXCEPTION_H

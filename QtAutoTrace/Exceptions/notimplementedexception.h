#ifndef NOTIMPLEMENTEDEXCEPTION_H
#define NOTIMPLEMENTEDEXCEPTION_H

#include <QException>

namespace QtAutoTraceV2
{

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

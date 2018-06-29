#ifndef SIGNALEXCEPTION_H
#define SIGNALEXCEPTION_H

#include <QException>
#include <QObject>

namespace QtAutoTraceV2
{

class SignalException : public QObject
{
    Q_OBJECT
public:
    explicit SignalException(QObject *parent = nullptr);

signals:
    void error(QException exc);

public slots:
};

}

#endif // SIGNALEXCEPTION_H

#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <QDebug>
#include <QException>
#include <QFileInfo>
#include <QObject>

#include "Exceptions/filereadexception.h"
#include "Exceptions/notimplementedexception.h"
#include "inputoptions.h"

namespace QtAutoTraceV2
{

class InputParser : public QObject
{
    Q_OBJECT
public:
    explicit InputParser(InputOptions &inputOptions, QObject *parent = nullptr);

signals:
    void finished(InputOptions &options);
    void error(QException &exc);

public slots:
    void run();

private:
    InputOptions inputOptions;
};

}

#endif // INPUTPARSER_H

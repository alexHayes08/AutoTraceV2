#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <QDebug>
#include <QException>
#include <QFileInfo>
#include <QObject>

#include "filereadexception.h"
#include "notimplementedexception.h"
#include "inputoptions.h"
#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT InputParser : public QObject
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

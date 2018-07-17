#pragma once

#include <QDebug>
#include <QException>
#include <QFileInfo>
#include <QImage>
#include <QMap>
#include <QObject>
#include <QVector>

#include "generictinputreader.h"
#include "inputoptions.h"
#include "inputparser.h"
#include "qtautotracelib_global.h"
#include "splinelistarray.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT QtAutoTraceLib : public QObject
{
    Q_OBJECT
public:
    explicit QtAutoTraceLib(QObject *parent = nullptr);
    void generateSvg(InputOptions &inputOptions);

private:
    InputParser inputParser;
    GenerictInputReader inputReader;
    SplineListArray splineListArray;

signals:
    void readingImage(int percentDone);
    void generatingSplines(int percentDone);
    void finished();
    void error(QException &exception);
};

}

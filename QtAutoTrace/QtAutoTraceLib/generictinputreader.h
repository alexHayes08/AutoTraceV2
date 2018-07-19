#pragma once

#include <QException>
#include <QDebug>
#include <QFileInfo>
#include <QImage>
#include <QObject>

#include "inputoptions.h"
#include "filereadexception.h"
#include "qtautotracelib_global.h"

#define MEGABYTE 1000000

namespace QtAutoTraceV2
{

/**
 * In the original AutoTrace program they have multiple input readers for
 * various types of files, however since Qt can handle all of them this class
 * will most likely be merged into the spline list array class.
 */
class QTAUTOTRACELIBSHARED_EXPORT GenerictInputReader : public QObject
{
    Q_OBJECT
public:
    explicit GenerictInputReader(QObject *parent = nullptr);

private:
//    const int BufferSize = MEGABYTE;
//    char buffer[BufferSize];

signals:
    void finishedReadingImage(QImage image, InputOptions &inputOptions);
    void error(QException &exc);

public slots:
    void readImage(InputOptions &inputOptions);
};

}

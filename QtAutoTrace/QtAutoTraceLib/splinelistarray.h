#pragma once

#include <QDebug>
#include <QException>
#include <QHash>
#include <QImage>
#include <QObject>
#include <QVector>

#include "adjacentpixels.h"
#include "inputoptions.h"
#include "pixeldata.h"
#include "qtautotracelib_global.h"
#include "svgelement.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT SplineListArray : public QObject
{
    Q_OBJECT
public:
    explicit SplineListArray(QObject *parent = nullptr);

signals:
    void finished(QVector<SvgElement> &elements,
        QRect &size,
        InputOptions &inputOptions);
    void error(QException &exception);
    void generatingSplines(int percentDone);

public slots:
    void run(QImage image, InputOptions &inputOptions);
};

}

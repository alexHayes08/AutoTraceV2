#pragma once

#include <QException>
#include <QObject>
#include <QPainter>
#include <QRect>
#include <QSvgGenerator>
#include <QVector>

#include "inputoptions.h"
#include "qtautotracelib_global.h"
#include "svgelement.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT OutputWriter : public QObject
{
    Q_OBJECT
public:
    explicit OutputWriter(QObject *parent = nullptr);

private:
    QSvgGenerator svgGenerator;

signals:
    void error(QException &exc);
    void finished();

public slots:
    void generate(QVector<SvgElement> &elements,
        QRect &size,
        InputOptions &inputoptions);
};

}

#ifndef SPLINELISTARRAY_H
#define SPLINELISTARRAY_H

#include <QDebug>
#include <QHash>
#include <QImage>
#include <QList>
#include <QObject>

#include "inputoptions.h"

namespace QtAutoTraceV2
{

class SplineListArray : public QObject
{
    Q_OBJECT
public:
    explicit SplineListArray(QObject *parent = nullptr);

signals:
    void finished();

public slots:
    void run(QImage image, InputOptions inputOptions);
};

}

#endif // SPLINELISTARRAY_H

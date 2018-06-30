#ifndef SPLINELISTARRAY_H
#define SPLINELISTARRAY_H

#include <QObject>
#include <QMap>

#include "inputoptions.h"

namespace QtAutoTraceV2
{

class SplineListArray : public QObject
{
    Q_OBJECT
public:
    explicit SplineListArray(QObject *parent = nullptr);

signals:

public slots:
    void run(QImage image, InputOptions inputOptions);
};

}

#endif // SPLINELISTARRAY_H

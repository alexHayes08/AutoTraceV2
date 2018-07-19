#pragma once

#include <QObject>
#include <QString>

#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

class QTAUTOTRACELIBSHARED_EXPORT InputOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool showProgress MEMBER showProgress)
    Q_PROPERTY(bool override MEMBER override)
    Q_PROPERTY(bool displayNCursesInterface MEMBER displayNCursesInterface)
    Q_PROPERTY(QString inputFileName MEMBER inputFileName)
    Q_PROPERTY(QString inputFileFormat MEMBER inputFileFormat)
    Q_PROPERTY(QString outputFileName MEMBER outputFileName)
    Q_PROPERTY(QString outputFileFormat MEMBER outputFileFormat)
    Q_PROPERTY(QString backgroundColor MEMBER backgroundColor)
    Q_PROPERTY(QString centerline MEMBER centerline)
    Q_PROPERTY(qint8 colorCount MEMBER colorCount)
    Q_PROPERTY(qint8 cornerThreshold MEMBER cornerThreshold)
    Q_PROPERTY(qint8 despeckleLevel MEMBER despeckleLevel)
    Q_PROPERTY(qint8 despeckleTightness MEMBER despeckleTightness)
    Q_PROPERTY(quint8 dpi MEMBER dpi)
    Q_PROPERTY(quint8 filterIterations MEMBER filterIterations)
    Q_PROPERTY(qreal lineReversionThreshold MEMBER lineReversionThreshold)
    Q_PROPERTY(qreal lineThreshold MEMBER lineThreshold)
    Q_PROPERTY(bool preserveWidth MEMBER preserveWidth)
    Q_PROPERTY(bool removeAdjacentCorners MEMBER removeAdjacentCorners)
    Q_PROPERTY(quint8 tangentSurround MEMBER tangentSurround)
public:
    explicit InputOptions(QObject *parent = nullptr);
    bool showProgress;
    bool override;
    bool displayNCursesInterface;
    QString inputFileName;
    QString inputFileFormat;
    QString outputFileName;
    QString outputFileFormat;
    QString backgroundColor;
    QString centerline;
    qint8 colorCount;
    qint8 cornerThreshold;
    qint8 despeckleLevel;
    qint8 despeckleTightness;
    quint8 dpi;
    quint8 filterIterations;
    qreal lineReversionThreshold;
    qreal lineThreshold;
    bool preserveWidth;
    bool removeAdjacentCorners;
    quint8 tangentSurround;
};

}

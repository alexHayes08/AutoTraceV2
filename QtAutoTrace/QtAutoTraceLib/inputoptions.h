#pragma once

#include <QObject>
#include <QString>
#include <QUrl>

#include "qtautotracelib_global.h"

namespace QtAutoTraceV2
{

struct QTAUTOTRACELIBSHARED_EXPORT InputOptions
{
    Q_GADGET
    Q_PROPERTY(bool showProgress MEMBER showProgress)
    Q_PROPERTY(bool override MEMBER override)
    Q_PROPERTY(bool displayNCursesInterface MEMBER displayNCursesInterface)
    Q_PROPERTY(bool preserveWidth MEMBER preserveWidth)
    Q_PROPERTY(bool removeAdjacentCorners MEMBER removeAdjacentCorners)
    Q_PROPERTY(QUrl inputFile MEMBER inputFile)
    Q_PROPERTY(QUrl outputFile MEMBER outputFile)
    Q_PROPERTY(QString backgroundColor MEMBER backgroundColor)
    Q_PROPERTY(QString centerline MEMBER centerline)
    Q_PROPERTY(qint8 colorCount MEMBER colorCount)
    Q_PROPERTY(qint8 cornerThreshold MEMBER cornerThreshold)
    Q_PROPERTY(qint8 despeckleLevel MEMBER despeckleLevel)
    Q_PROPERTY(qint8 despeckleTightness MEMBER despeckleTightness)
    Q_PROPERTY(quint8 dpi MEMBER dpi)
    Q_PROPERTY(quint8 filterIterations MEMBER filterIterations)
    Q_PROPERTY(quint8 tangentSurround MEMBER tangentSurround)
    Q_PROPERTY(qreal lineReversionThreshold MEMBER lineReversionThreshold)
    Q_PROPERTY(qreal lineThreshold MEMBER lineThreshold)

public:
//    explicit InputOptions(QObject *parent = nullptr);
    bool showProgress;
    bool override;
    bool displayNCursesInterface;
    bool preserveWidth;
    bool removeAdjacentCorners;
    QUrl inputFile;
    QUrl outputFile;
    QString backgroundColor;
    QString centerline;
    qint8 colorCount;
    qint8 cornerThreshold;
    qint8 despeckleLevel;
    qint8 despeckleTightness;
    quint8 dpi;
    quint8 filterIterations;
    quint8 tangentSurround;
    qreal lineReversionThreshold;
    qreal lineThreshold;
};

}

Q_DECLARE_METATYPE(QtAutoTraceV2::InputOptions)

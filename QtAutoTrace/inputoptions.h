#ifndef INPUTOPTIONS_H
#define INPUTOPTIONS_H

#include <QString>

namespace QtAutoTraceV2
{

class InputOptions
{
public:
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
    bool removeAdajentCorners;
    quint8 tangentSurround;
};

}

#endif // INPUTOPTIONS_H

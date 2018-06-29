#ifndef GENERICTINPUTREADER_H
#define GENERICTINPUTREADER_H

#include <QException>
#include <QDebug>
#include <QImage>
#include <QObject>

#include "inputoptions.h"
#include "Exceptions/filereadexception.h"

namespace QtAutoTraceV2
{

class GenerictInputReader : public QObject
{
    Q_OBJECT
public:
    explicit GenerictInputReader(QObject *parent = nullptr);

signals:
    void finishedReadingImage(QImage image);
    void error(QException &exc);

public slots:
    void readImage(InputOptions &inputOptions);
};

}

#endif // GENERICTINPUTREADER_H

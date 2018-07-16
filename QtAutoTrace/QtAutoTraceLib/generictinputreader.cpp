#include "generictinputreader.h"

namespace QtAutoTraceV2
{

GenerictInputReader::GenerictInputReader(QObject *parent) : QObject(parent)
{ }

void GenerictInputReader::readImage(InputOptions &inputOptions)
{
    QImage image;
    if (!image.load(inputOptions.inputFileName))
    {
        // Error occurred.
        QException &exc = *(new FileReadException());
        this->error(exc);
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() << "Finished loading the image (without errors)."
                 << endl;
        qDebug() << "Image size: "
                 << image.size()
                 << endl;
#endif
        this->finishedReadingImage(image, inputOptions);
    }
}

}

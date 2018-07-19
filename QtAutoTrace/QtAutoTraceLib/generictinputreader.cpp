#include "generictinputreader.h"

namespace QtAutoTraceV2
{

GenerictInputReader::GenerictInputReader(QObject *parent) : QObject(parent)
{ }

void GenerictInputReader::readImage(InputOptions &inputOptions)
{
    // Get file info.
    QFileInfo info(inputOptions.inputFileName);
    if (!info.exists())
    {
        QException exc;
        emit this->error(exc);
        return;
    }

    // If the image is greater than 10 MB
//    if (info.size() > 10 * MEGABYTE)
//    {
//        // Read image as bytes.

//    }
    // Load image in one go.
    QImage image;
    if (!image.load(inputOptions.inputFileName))
    {
        // Error occurred.
        QException exc;
        emit this->error(exc);
        return;
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
        emit this->finishedReadingImage(image, inputOptions);
    }
}

}

#include "splinelistarray.h"

namespace QtAutoTraceV2
{

SplineListArray::SplineListArray(QObject *parent) : QObject(parent)
{ }

void SplineListArray::run(QImage image, InputOptions inputOptions)
{
    // * Get most common colors
    // * Sort by most to least common
    QHash<QRgb, QPoint> colorMap;

    auto width = image.height();
    auto height = image.width();

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            QColor color = image.pixelColor(col, row);
            QPoint point = QPoint(col, row);
            colorMap.insertMulti(color.rgb(), point);
        }
    }

    // Retrieve all unique colors.
    auto uniqueKeys = colorMap.uniqueKeys ();

    for (auto it = uniqueKeys.begin(); it != uniqueKeys.end(); it++)
    {
#ifdef QT_DEBUG
        qDebug() << "Color: "
                 << QColor(*it)
                 << endl
                 << "\tNumber of Points: "
                 << colorMap.values(*it).size()
                 << endl
                 << "\t% of image: "
                 << double(colorMap.values(*it).size()) / double((width * height)) * 100
                 << "%"
                 << endl
                 << "\tnumber of entries: "
                 << colorMap.values(*it).size()
                 << endl;
#endif

        // Seperate the pixel coords for each color into their bounding boxes.
        auto pixels = colorMap.values(*it);
        QList<QList<QPoint>> boundingBoxes;
    }

    this->finished();
}

}

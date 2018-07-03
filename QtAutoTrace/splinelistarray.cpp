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
        QVector<QVector<AdjacentPoints>> boundingBoxes;
        QVector<AdjacentPoints> adjacentPoints;
        int currentIndex = 0;

        // Create an adjacent poins object for each pixel.
        for (auto it = pixels.begin(); it != pixels.end(); it++)
        {
            QVector<QPoint> currentList = boundingBoxes.value(currentIndex, QVector<QPoint>());
            auto pixel = *it;

            // Check for a pixel of the same color in the eight surrounding
            // pixels.
            adjacentPoints.append(AdjacentPoints(
                QSize(image.width(), image.height()),
                currentList,
                pixel));
        }

        // Group the adjacent pixels if they contain a reference to the other
        // pixel.

#ifdef QT_DEBUG
        // Print out all bounding boxes.
        for (auto it = boundingBoxes.begin(); it != boundingBoxes.end(); it++)
        {
            auto bbox = *it;
            qDebug() << "bbox size: "
                     << bbox.size()
                     << endl;
        }
#endif
    }

    this->finished();
}

}

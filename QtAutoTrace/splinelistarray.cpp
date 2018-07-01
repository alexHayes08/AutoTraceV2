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

#ifdef QT_DEBUG
    for (auto it = colorMap.begin(); it != colorMap.end(); it++)
    {
        qDebug() << "Color: "
                 << it.key()
                 << endl
                 << "\tNumber of Points: "
                 << colorMap.values(it.key()).size()
                 << endl
                 << "\t% of image: "
                 << colorMap.values(it.key()).size() / (width * height)
                 << endl
                 << "\tnumber of entries: "
                 << colorMap.values(it.key()).size()
                 << endl;
    }
#endif

    this->finished();
}

}

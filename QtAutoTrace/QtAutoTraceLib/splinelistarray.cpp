#include "splinelistarray.h"

namespace QtAutoTraceV2
{

SplineListArray::SplineListArray(QObject *parent) : QObject(parent)
{ }

void SplineListArray::run(QImage image, InputOptions inputOptions)
{
    // * Get most common colors.
    // * Sort by most to least common.
    QHash<QRgb, QPoint> colorMap;
    auto width = image.height();
    auto height = image.width();
    auto numberOfPixels = height * width;
    auto numberOfMarkedPixels = 0;
    QVector<PixelData> unmarkedPixels;
    QVector<QVector<PixelData>> pixelGroups;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            QColor color = image.pixelColor(col, row);
            QPoint point = QPoint(col, row);
            PixelData pixelData;
            pixelData.color = color;
            pixelData.coord = point;
            unmarkedPixels.append(pixelData);
            colorMap.insertMulti(color.rgb(), point);
        }
    }

    while (unmarkedPixels.size() > 0)
    {
        PixelData pixelData = unmarkedPixels.takeFirst();
        QVector<PixelData> connectedPixels = { pixelData };

        for (qint64 i = 0; i < unmarkedPixels.size(); ++i)
        {
            auto otherPixel = unmarkedPixels[i];

            for (int ii = 0; ii < connectedPixels.size(); ii++)
            {
                auto connectedPixel = connectedPixels[ii];
                if (AdjacentPixels::ArePixelsAdjacent(connectedPixel, otherPixel)
                    != AdjacentResult::NOTADJACENT)
                {
                    // Found match, append to group.
                    connectedPixels.append(unmarkedPixels.takeAt(i));

                    // Restart the loop.
                    i = -1;
                    break;
                }
            }
        }

        // Append to list of groups.
        pixelGroups.append(connectedPixels);
    }

    // Retrieve all unique colors.
    auto uniqueKeys = colorMap.uniqueKeys ();
    for (auto it = uniqueKeys.begin(); it != uniqueKeys.end(); it++)
    {
        if (inputOptions.showProgress)
        {
            qInfo() << "Color: "
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
        }
    }

    if (inputOptions.showProgress)
    {
        qDebug() << "Shapes detected: "
            << pixelGroups.size()
            << endl;

        // Print out all bounding boxes.
        for (int i = 0; i < pixelGroups.size(); i++)
        {
            auto pixelGroup = pixelGroups[i];
            qDebug() << "Shape #"
                << i
                << "size: "
                << pixelGroup.size();
        }
    }

    this->finished();
}

}

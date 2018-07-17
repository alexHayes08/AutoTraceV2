#include "qtautotracelib.h"

namespace QtAutoTraceV2
{
QtAutoTraceLib::QtAutoTraceLib(QObject *parent) : QObject(parent)
{
    this->inputParser = InputParser(this);
    this->inputReader = GenerictInputReader(this);
    this->splineListArray = SplineListArray(this);

    QObject::connect(&this->inputParser, &InputParser::error,
        &this, QtAutoTraceLib::error);
    QObject::connect(&this->inputParser, &InputParser::finished,
        &this->inputReader, &GenerictInputReader::readImage);

    QObject::connect(&this->inputReader, &GenerictInputReader::error,
        &this, QtAutoTraceLib::error);
    QObject::connect(&this->inputReader, &GenerictInputReader::finishedReadingImage,
        &this->splineListArray, SplineListArray::run);

    QObject::connect(&this->splineListArray, &SplineListArray::error,
        &this, &QtAutoTraceLib::error);
    QObject::connect(&this->splineListArray, &SplineListArray::finished,
        &this, &this->finished);
}

void QtAutoTraceLib::generateSvg(InputOptions &inputOptions)
{
    // First validate inputOptions.
    this->parseInputs(inputOptions);

    this->inputReader.readImage(inputOptions);
    // Read the input file.
    QImage image;
    if (!image.load(inputOptions.inputFileName))
    {
        // Error occurred.
        QException &exc = *(new FileReadException());
        this->error(exc);
    }

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
}

void QtAutoTraceLib::parseInputs(InputOptions &inputOptions)
{
    // Verify the required arguments are being passed in.
    if (this->inputOptions.inputFileName.isEmpty())
    {
        qCritical() << "No input file specified.";
        QException &exc = *(new FileReadException());
        this->error(exc);
    }

    // Populate all unset options to defaults.

    // Default dpi is 72.
    if (inputOptions.dpi == 0)
    {
        inputOptions.dpi = 72;
    }

    // Default output format is svg.
    if (inputOptions.outputFileFormat.isEmpty())
    {
        inputOptions.outputFileFormat = "svg";
    }

    // Default output filename is the input file name with the suffix replaced
    // with svg.
    if (inputOptions.outputFileName.isEmpty())
    {
        QFileInfo inputFileInfo(inputOptions.inputFileName);
        inputOptions.outputFileName = inputFileInfo.completeBaseName()
            + "." + inputOptions.outputFileFormat;
    }

    // Background color defaults to an empty string.
    // Centerline defaults

#ifdef QT_DEBUG
    qDebug() << "Input Options:";
    qDebug() << "\tShow progress: " << inputOptions.showProgress;
    qDebug() << "\tOverride output file: " << inputOptions.override;
    qDebug() << "\tInput format: " << inputOptions.inputFileFormat;
    qDebug() << "\tInput file: " << inputOptions.inputFileName;
    qDebug() << "\tOutput format: " << inputOptions.outputFileFormat;
    qDebug() << "\tOutput file: " << inputOptions.outputFileName;
    qDebug() << endl;
#endif
}
}

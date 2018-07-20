#include "qtautotracelib.h"

namespace QtAutoTraceV2
{

QtAutoTraceLib::QtAutoTraceLib(QObject *parent) : QObject(parent)
{
    this->inputParser.setParent(this);
    this->inputReader.setParent(this);
    this->splineListArray.setParent(this);
    this->outputWriter.setParent(this);

    QObject::connect(&this->inputParser, &InputParser::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->inputParser, &InputParser::finished,
        &this->inputReader, &GenerictInputReader::readImage);

    QObject::connect(&this->inputReader, &GenerictInputReader::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->inputReader, &GenerictInputReader::finishedReadingImage,
        &this->splineListArray, &SplineListArray::run);
    QObject::connect(&this->inputReader, &GenerictInputReader::readingImage,
        this, &QtAutoTraceLib::readingImage);

    QObject::connect(&this->splineListArray, &SplineListArray::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->splineListArray, &SplineListArray::finished,
        &this->outputWriter, &OutputWriter::generate);
    QObject::connect(&this->splineListArray, &SplineListArray::generatingSplines,
        this, &QtAutoTraceLib::generatingSplines);

    QObject::connect(&this->outputWriter, &OutputWriter::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->outputWriter, &OutputWriter::finished,
        this, &QtAutoTraceLib::finished);
}

void QtAutoTraceLib::generateSvg(InputOptions inputOptions)
{
    // First validate inputOptions.
    this->inputParser.run(inputOptions);
}

void QtAutoTraceLib::generateSvg(QVariant data)
{
    QMap<QString, QVariant> map = data.toMap();
    InputOptions inOpts;

    inOpts.inputFileName = map["inputFileName"].toString();
    inOpts.outputFileName = map["outputFileName"].toString();
    inOpts.colorCount = map["colorCount"].value<qint8>();

    this->inputParser.run(inOpts);
}

}

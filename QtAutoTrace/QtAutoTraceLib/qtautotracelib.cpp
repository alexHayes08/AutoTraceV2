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

    QObject::connect(&this->splineListArray, &SplineListArray::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->splineListArray, &SplineListArray::finished,
        &this->outputWriter, &OutputWriter::generate);

    QObject::connect(&this->outputWriter, &OutputWriter::error,
        this, &QtAutoTraceLib::error);
    QObject::connect(&this->outputWriter, &OutputWriter::finished,
        this, &QtAutoTraceLib::finished);
}

void QtAutoTraceLib::generateSvg(InputOptions &inputOptions)
{
    // First validate inputOptions.
    this->inputParser.run(inputOptions);
}

}

#include "inputparser.h"

namespace QtAutoTraceV2
{

InputParser::InputParser(InputOptions &inputOptions, QObject *parent)
    : QObject(parent)
{ }

void InputParser::run(InputOptions &inputOptions)
{
    // Verify the required arguments are being passed in.
    if (inputOptions.inputFileName.isEmpty())
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

    this->finished(this->inputOptions);
}

}

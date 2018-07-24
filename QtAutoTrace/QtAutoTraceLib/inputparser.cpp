#include "inputparser.h"

namespace QtAutoTraceV2
{

InputParser::InputParser(QObject *parent) : QObject(parent)
{ }

void InputParser::run(InputOptions &inputOptions)
{
    // Verify the required arguments are being passed in.
    if (inputOptions.inputFile.isEmpty())
    {
        qCritical() << "No input file specified.";
        QException &exc = *(new FileReadException());
        emit this->error(exc);
        return;
    }

    // Populate all unset options to defaults.

    // Default dpi is 72.
    if (inputOptions.dpi == 0)
    {
        inputOptions.dpi = 72;
    }

    // Default output filename is the input file name with the suffix replaced
    // with svg.
    if (inputOptions.outputFile.isEmpty())
    {
        QFileInfo inputFileInfo(inputOptions.inputFile.path ());
        inputOptions.outputFile= inputFileInfo.completeBaseName()
            + ".svg";
    }

    // Background color defaults to an empty string.
    // Centerline defaults

#ifdef QT_DEBUG
    qDebug() << "Input Options:";
    qDebug() << "\tShow progress: " << inputOptions.showProgress;
    qDebug() << "\tOverride output file: " << inputOptions.override;
    qDebug() << "\tInput file: " << inputOptions.inputFile;
    qDebug() << "\tOutput file: " << inputOptions.outputFile;
    qDebug() << endl;
#endif

    emit this->finished(inputOptions);
}

}

#include "inputparser.h"

namespace QtAutoTraceV2
{

InputParser::InputParser(InputOptions &inputOptions, QObject *parent)
    : QObject(parent)
{
    this->inputOptions = inputOptions;
}

void InputParser::run()
{
    // Verify the required arguments are being passed in.
    if (this->inputOptions.inputFileName.isEmpty())
    {
        qCritical() << "No input file specified.";
        QException &exc = *(new FileReadException());
        this->error(exc);
    }

#ifdef QT_DEBUG
    qDebug() << "Finished parsing inputs." << endl;
#endif
    this->finished(this->inputOptions);
}

}

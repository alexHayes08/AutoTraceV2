#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QPoint>

#include "generictinputreader.h"
#include "inputparser.h"
#include "splinelistarray.h"

/**
 * @brief main - Run with the following arguments:
 * 1) -p --input-format="bmp" --input-file="../testG.bmp" --output-format="svg"
 * --input-format="bmp"
 * 2) -h
 * 3) -v
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("QtAutoTraceV2");
    QCoreApplication::setApplicationVersion("0.1");

    QtAutoTraceV2::InputOptions inputOptions;
    QtAutoTraceV2::GenerictInputReader inputReader;
    QtAutoTraceV2::SplineListArray splineListArr;

    QCommandLineParser parser;
    parser.setApplicationDescription("QtAutoTraceV2 is a utility for"
        "converting bitmap into vector graphics.");

    // Add options to the command line parser.
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption showProgressOption(QStringList() << "p" << "progress",
        QCoreApplication::translate("main",
            "Show progress."));
    parser.addOption(showProgressOption);

    QCommandLineOption forceOption(QStringList() << "f" << "force",
        QCoreApplication::translate("main",
            "Override output file."));
    parser.addOption(forceOption);

    QCommandLineOption outputFileOption("output-file",
        QCoreApplication::translate("main",
            "Write to <filename>."),
        "file");
    parser.addOption(outputFileOption);

    QCommandLineOption outputFormatOption("output-format",
        QCoreApplication::translate("main",
          "Use format <format> for the output file."),
        "format");
    parser.addOption(outputFormatOption);

    QCommandLineOption inputFileOption("input-file",
        QCoreApplication::translate("main",
            "The bitmap file location to transform into a vector file."),
        "file");
    parser.addOption(inputFileOption);

    QCommandLineOption inputFormatOption("input-format",
        QCoreApplication::translate("main",
            "Can be one of the following: png, tga, pbm, pnm, pgm, ppm, or"
            " bmp."),
        "format");
    parser.addOption(inputFormatOption);

	QCommandLineOption infoOption("info",
		QCoreApplication::translate("main",
			"Retrieves info about the image such as the ten most common colors"
			" and the percentage of the image they cover."));

    // Process arguments.
    parser.process(app);

    // If this point is reached then the user isn't retrieving info on this
    // program but is trying to convert an image.

    // Retrieve arguments.
    inputOptions.showProgress = parser.isSet(showProgressOption);
    inputOptions.override = parser.isSet(forceOption);
	inputOptions.getInfo = parser.isSet(infoOption);

    if (parser.isSet(inputFileOption))
        inputOptions.inputFileName = parser.value(inputFileOption);

    if (parser.isSet(inputFormatOption))
        inputOptions.inputFileFormat = parser.value(inputFormatOption);

    if (parser.isSet(outputFileOption))
        inputOptions.outputFileFormat = parser.value(outputFormatOption);

    if (parser.isSet(outputFormatOption))
        inputOptions.outputFileFormat = parser.value(outputFormatOption);

#ifdef QT_DEBUG
    qDebug() << "Running from: "
             << app.applicationFilePath()
             << endl;
#endif

    // Create the input parser.
    QtAutoTraceV2::InputParser inputParser(inputOptions);

    auto onError = [=](QException &exception) {
        qDebug() << exception.what() << endl;
        QCoreApplication::exit(1);
    };

    auto programDone = [=]() {
        qDebug() << "Finished executing program.";
        QCoreApplication::exit();
    };

    // Hook up all signal/slots here.

    // Parse input args.
    QObject::connect(&inputParser, &QtAutoTraceV2::InputParser::error,
        onError);
    QObject::connect(&inputParser, &QtAutoTraceV2::InputParser::finished,
        &inputReader, &QtAutoTraceV2::GenerictInputReader::readImage);

    // Load image as QImage.
    QObject::connect(&inputReader, &QtAutoTraceV2::GenerictInputReader::error,
        onError);
    QObject::connect(&inputReader,
        &QtAutoTraceV2::GenerictInputReader::finishedReadingImage,
        &splineListArr, &QtAutoTraceV2::SplineListArray::run);

	// TODO: Create splines from QImage.
	QObject::connect(&splineListArr, &QtAutoTraceV2::SplineListArray::error,
		onError);
    QObject::connect(&splineListArr, &QtAutoTraceV2::SplineListArray::finished,
        programDone);

    // TODO: Convert splines into svg elements (such as path).

    // TODO: Write svg elements to the output file.

    // Can't call function directly on the inputReader before the event loop
    // starts else we won't have access to the event loop.
    QMetaObject::invokeMethod(&inputParser, "run", Qt::QueuedConnection);

    return app.exec();
}

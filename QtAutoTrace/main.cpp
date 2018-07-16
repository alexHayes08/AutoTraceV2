#include <iostream>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QPoint>

#include "generictinputreader.h"
#include "inputparser.h"
#include "splinelistarray.h"
#include "terminalhelper.h"

void draw(Terminal::TerminalHelper &terminal)
{
    // Title bar.
    Terminal::OutputOptions titleBarStyle;
    titleBarStyle.bg = Terminal::VT100Color::CYAN;
    titleBarStyle.fg = Terminal::VT100Color::WHITE;
    titleBarStyle.maxHeightOfOutput = 1;
    titleBarStyle.maxHeightOfOutputAsPercent = false;
    titleBarStyle.textWrap = Terminal::TextWrap::NO_WRAP;
    titleBarStyle.textOverflow = Terminal::TextOverflow::ELLIPSIS_RIGHT;

    // Print status bar > Text message.
    Terminal::OutputOptions statusBar_textMessage;
    statusBar_textMessage.bg = Terminal::VT100Color::CYAN;
    statusBar_textMessage.fg = Terminal::VT100Color::WHITE;
    statusBar_textMessage.maxHeightOfOutput = 1;
    statusBar_textMessage.maxHeightOfOutputAsPercent = false;
    statusBar_textMessage.maxWidthOfOutput = 50;
    statusBar_textMessage.maxWidthOfOutputAsPercent = true;
    statusBar_textMessage.textWrap = Terminal::TextWrap::NO_WRAP;
    statusBar_textMessage.textOverflow = Terminal::TextOverflow::ELLIPSIS_RIGHT;

    // Print status bar > progress bar.
    Terminal::OutputOptions statusBar_progressBar;
    statusBar_progressBar.bg = Terminal::VT100Color::CYAN;
    statusBar_progressBar.fg = Terminal::VT100Color::WHITE;
    statusBar_progressBar.maxHeightOfOutput = 1;
    statusBar_progressBar.maxHeightOfOutputAsPercent = false;
    statusBar_progressBar.maxWidthOfOutput = 50;
    statusBar_progressBar.maxWidthOfOutputAsPercent = true;
}

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
    parser.setApplicationDescription("QtAutoTraceV2 is a utility for \
        converting bitmap into vector graphics.");

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

    QCommandLineOption interfaceOption(QStringList() << "i" << "interactive",
        QCoreApplication::translate("main",
            "Display ncurses like interface prompting the user for the required input."));
    parser.addOption(interfaceOption);

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
            "Can be one of the following: png, tga, pbm, pnm, pgm, ppm, or "
            "bmp."),
        "format");
    parser.addOption(inputFormatOption);

    // Process arguments.
    parser.process(app);

    // If this point is reached then the user isn't retrieving info on this
    // program but is trying to convert an image.

    // Retrieve arguments.
    inputOptions.showProgress = parser.isSet(showProgressOption);
    inputOptions.override = parser.isSet(forceOption);
    inputOptions.displayNCursesInterface = parser.isSet(interfaceOption);

    if (parser.isSet(inputFileOption))
        inputOptions.inputFileName = parser.value(inputFileOption);

    if (parser.isSet(inputFormatOption))
        inputOptions.inputFileFormat = parser.value(inputFormatOption);

    if (parser.isSet(outputFileOption))
        inputOptions.outputFileName = parser.value(outputFormatOption);

    if (parser.isSet(outputFormatOption))
        inputOptions.outputFileFormat = parser.value(outputFormatOption);

    // Create the output handler.
    Terminal::TerminalHelper terminal;

    // Check if terminal supports VT100.
    if (!terminal.supportsVT100() && inputOptions.displayNCursesInterface)
    {
        terminal.print("Your terminal does not support the VT100 interface. "
            "Ignoring flag '--interface'.\n");
#ifdef Q_OS_WIN
        terminal.print("Windows detected: Currently 'Command Prompt' and "
            "'PowerShell' both work with Windows 10 Anniversary Update and "
            "later.\n");
#endif
    }
    else if (inputOptions.displayNCursesInterface)
    {
        // Switch to alternative buffer.
        terminal.alternativeBuffer();
    }

#ifdef QT_DEBUG
    auto message = "Running from: " + app.applicationFilePath().toStdString();
    terminal.print(message, true);
#endif

    // Create the input parser.
    QtAutoTraceV2::InputParser inputParser(inputOptions);

    auto onError = [=](QException &exception) {
        qDebug() << exception.what() << endl;
        QCoreApplication::exit(1);
    };

    auto programDone = [=]() {
#ifdef QT_DEBUG
        qDebug() << "Finished executing program.";
#endif
        QCoreApplication::exit();
    };

    auto onBeforeShowProgress = [=](QString progressBarTitle) {
        std::cout << std::endl;

        if (!progressBarTitle.isEmpty())
        {
            std::cout << progressBarTitle.toStdString() << std::endl;
        }
    };

    auto onShowProgress = [=](double amountDone, double total) {
        if (amountDone == total) {
            std::cout << std::endl;
        } else {
            double percentage = amountDone / total;
            double increment = 1;
        }
    };

    auto onAfterShowProgress = [inputOptions, terminal]() {
        if (inputOptions.displayNCursesInterface)
        {
            terminal;
        }
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

    terminal
        .alternativeBuffer()
        .print("Testing 1 2 3")
        .fillLine(Terminal::TerminalShape::TOP_LEFT_CORNER,
            Terminal::TerminalShape::HORIZONTAL_LINE,
            Terminal::TerminalShape::TOP_RIGHT_CORNER)
        .fillLine(Terminal::TerminalShape::BOT_LEFT_CORNER,
            Terminal::TerminalShape::HORIZONTAL_LINE,
            Terminal::TerminalShape::BOT_RIGHT_CORNER);
    terminal.clearCurrentLine();
    terminal
        .clear()
        .print("Testing", true)
        .print("RED FG", Terminal::VT100Color::RED, true)
        .print("GREEN FG", Terminal::VT100Color::GREEN, true)
        .print("BLUE FG", Terminal::VT100Color::BLUE, true)
        .print("YELLOW FG", Terminal::VT100Color::YELLOW, true)
        .print("CYAN FG", Terminal::VT100Color::CYAN, true)
        .print("MAGENTA FG", Terminal::VT100Color::MAGENTA, true)
        .print("WHITE FG", Terminal::VT100Color::WHITE, true)
        .print("BLACK FG", Terminal::VT100Color::BLACK, true)
        .print("RED BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::RED, true)
        .print("GREEN BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::GREEN, true)
        .print("BLUE BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::BLUE, true)
        .print("YELLOW BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::YELLOW, true)
        .print("CYAN BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::CYAN, true)
        .print("MAGENTA BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::MAGENTA, true)
        .print("WHITE BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::WHITE, true)
        .print("BLACK BG", Terminal::VT100Color::INHERIT, Terminal::VT100Color::BLACK, true);
    terminal
        .print("Test_ _ _ _a", false)
        .backspace()
        .newLine()
        .down(3)
        .print("Second Test_")
        .left(12)
        .print("Third Test_")
        .backspace();

    auto terminalDimensions = terminal.getTerminalDimensions();
    terminal.print(QString("Terminal Size: { x: %1, y: %2 }")
            .arg(terminalDimensions.width())
            .arg(terminalDimensions.height())
        .toStdString()
        , true);

    terminal.mainBuffer();
    return 0;

//    return app.exec();
}

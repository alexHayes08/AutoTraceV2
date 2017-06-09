#include "bitmap.h"
#include "InputReaders/bitmapreader.h"
#include "color.h"
#include "common.h"
#include "commandhandler.h"
#include "filewrapper.h"
#include "fittingoptions.h"
#include "inputoptions.h"
#include "inputreader.h"
#include "outputoptions.h"
#include "outputwriter.h"
#include "splinelistarray.h"
#include "OutputWriters/svgwriter.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <stdio.h>
#include <string>

static void ToLowerCase (std::string &input)
{
    std::transform(input.begin (), input.end(), input.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

namespace AutoTrace {

static void Init(std::string argZero);

static std::map<std::string, std::shared_ptr<InputReader>> InputReaders;
static std::map<std::string, std::shared_ptr<OutputWriter>> OutputWriters;
}

int main(int argc, char *argv[])
{
    // Init InputReaders
    AutoTrace::InputReaders["bmp"] =
            std::shared_ptr<AutoTrace::BitmapReader>(new AutoTrace::BitmapReader());

    // Init OutputWriters
    AutoTrace::OutputWriters["svg"] =
            std::shared_ptr<AutoTrace::SvgWriter>(new AutoTrace::SvgWriter());

//    AutoTrace::Init(argv[0]);
//    std::vector<std::string> arguments(argv + 1, argv + argc);

//    auto fittingOpts = std::make_unique<AutoTrace::FittingOptions>(new AutoTrace::FittingOpts());
//    auto inputOptions = std::make_unique<AutoTrace::InputOptions>(new AutoTrace::InputOptions());
//    auto outputOptions = std::make_unique<AutoTrace::OutputOptions>(new AutoTrace::OutputOptions());
    AutoTrace::FittingOptions *fittingOpts = new AutoTrace::FittingOptions();
    AutoTrace::InputOptions *inputOpts = new AutoTrace::InputOptions();
    AutoTrace::OutputOptions *outputOpts = new AutoTrace::OutputOptions();

    AutoTrace::FileWrapper *inputFile = nullptr;
    AutoTrace::FileWrapper *outputFile = nullptr;

//    std::string outputName = "",
//            outputSuffix = "";
    std::string logfileName;
//    std::unique_ptr<AutoTrace::Spline> splines;
    std::shared_ptr<AutoTrace::SplineListArray> splines;
    std::shared_ptr<AutoTrace::Bitmap> bitmap;

    std::shared_ptr<AutoTrace::InputReader> inputReader;
    std::shared_ptr<AutoTrace::OutputWriter> outputWriter;

//    FILE *outputFile;
    FILE *dumpFile;

//    bool inputFile = false;
    bool getInformation = false;
    bool correctInputs = true;

    for (int i = 1; i < argc; i++)
    {
        auto command = AutoTrace::CommandHandler::parseInputForCommand (argv[i]);
        auto argument = AutoTrace::CommandHandler::parseInputForArgument (argv[i]);
//        std::cout << "Command: "
//                  << command
//                  << (argument.size () > 0 ? " Argument: " + argument: "")
//                  << std::endl;

        if (command == "help")
        {
            getInformation = true;
        }
        else if (command == "background-color")
        {
            fittingOpts->background_color = std::shared_ptr<AutoTrace::Color>(new AutoTrace::Color(argument));
            inputOpts->backgroundColor = fittingOpts->background_color;
#ifdef DEBUG
            std::cout << "Set the background-color to: " << argument << std::endl;
#endif
        }
        else if (command == "charcode")
        {
            fittingOpts->charcode = (unsigned char) (std::stoi(argument));
            inputOpts->charchode = fittingOpts->charcode;
#ifdef DEBUG
            std::cout << "Charcode: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "color-count")
        {
            fittingOpts->colorCount = static_cast<unsigned int>(std::stoul (argument));
#ifdef DEBUG
            std::cout << "Set color count to: "
                      << fittingOpts->colorCount
                      << std::endl;
#endif
        }
        else if (command == "corner-always-threshold")
        {
            fittingOpts->cornerAlwaysThreshold = std::stod (argument);
#ifdef DEBUG
            std::cout << "Set corner-always-threshold to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "corner-surround")
        {
            fittingOpts->cornerSurround = static_cast<unsigned int>(std::stoul (argument));
#ifdef DEBUG
            std::cout << "Set corner-surround to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "corner-threshold")
        {
            fittingOpts->cornerThreshold = std::stod (argument);
#ifdef DEBUG
            std::cout << "Set corner-threshold to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "dpi")
        {
            outputOpts->dpi = std::stoi (argument);

#ifdef DEBUG
            std::cout << "Set dpi to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "error-threshold")
        {
            fittingOpts->errorThreshold = std::stod (argument);
#ifdef DEBUG
            std::cout << "Set error-threshold to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "filter-iterations")
        {
            fittingOpts->filterIterations = static_cast<unsigned int>(std::stoul(argument));
#ifdef DEBUG
            std::cout << "Set filterIterations to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "line-reversion-theshold")
        {
            fittingOpts->lineReversionThreshold = std::stod(argument);
#ifdef DEBUG
            std::cout << "Set line-reversion-threshold to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "line-threshold")
        {
            fittingOpts->lineThreshold = std::stod(argument);
#ifdef DEBUG
            std::cout << "Set line-threshold to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "output-format")
        {
            ToLowerCase (argument);
            outputWriter = AutoTrace::OutputWriters[argument];
        }
        else if (command == "remove-adjacent-corners")
        {
            ToLowerCase (argument);

            if (argument.empty () || argument == "true")
                fittingOpts->removeAdjacentCorners = true;
            else if (argument == "false")
                fittingOpts->removeAdjacentCorners = false;
            else
                throw "Error: Could not parse argument!";

#ifdef DEBUG
            std::cout << "Set remove-adjacent-corners to: "
                      << (fittingOpts->removeAdjacentCorners ? "true" : "false")
                      << std::endl;
#endif
        }
        else if (command == "tangent-surround")
        {
            fittingOpts->tangentSurround = static_cast<unsigned int>(std::stoul(argument));
#ifdef DEBUG
            std::cout << "Set tangent-surround to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "despeckle-level")
        {
            fittingOpts->despeckleLevel = static_cast<unsigned int>(std::stoul(argument));
#ifdef DEBUG
            std::cout << "Set despeckle level to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "despeckle-tightness")
        {
            fittingOpts->despeckleTightness = std::stod(argument);
#ifdef DEBUG
            std::cout << "Set despeckle-tightness to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "input-format")
        {
            ToLowerCase (argument);
            inputReader = AutoTrace::InputReaders[argument];
        }
        else if (command == "noise-removal")
        {
            fittingOpts->noiseRemoval = std::stod(argument);
#ifdef DEBUG
            std::cout << "Set noise-removal to: "
                      << argument
                      << std::endl;
#endif
        }
        else if (command == "centerline")
        {
            ToLowerCase (argument);

            if (argument.empty () || argument == "true")
                fittingOpts->centerline = true;
            else if (argument == "false")
                fittingOpts->centerline = false;
            else
                throw "Error: Couldn't parse argument!";

#ifdef DEBUG
            std::cout << "Set centerline to: "
                      << (fittingOpts->centerline ? "true" : "false")
                      << std::endl;
#endif
        }
        else if (command == "preserve-width")
        {
            ToLowerCase (argument);

            if (argument.empty () || argument == "true")
                fittingOpts->preserveWidth = true;
            else if (argument == "false")
                fittingOpts->preserveWidth = false;
            else
                throw "Error: Couldn't parse argument!";

#ifdef DEBUG
            std::cout << "Set preserve-width to: "
                      << (fittingOpts->preserveWidth ? "true" : "false")
                      << std::endl;
#endif
        }
        else if (command == "width-weight-factor")
        {
            ToLowerCase (argument);

            if (argument.empty () || argument == "true")
                fittingOpts->widthWeightFactor = true;
            else if (argument == "false")
                fittingOpts->widthWeightFactor = false;
            else
                throw "Error: Couldn't parse argument!";

#ifdef DEBUG
            std::cout << "Set width-weight-factor to: "
                      << (fittingOpts->widthWeightFactor ? "true" : "false")
                      << std::endl;
#endif
        }
        else if (!command.empty())
        {
            // Check to if it's a filename
            inputFile = new AutoTrace::FileWrapper(command);

#ifdef DEBUG
                std::cout << "Full filename is: " << inputFile->fullName()
                          << std::endl
                          << "Filename is: " << inputFile->name()
                          << std::endl
                          << "File suffix is: " << inputFile->suffix()
                          << std::endl;
#endif
        }
        else if (command.empty())
        { }
        else
        {
            correctInputs = false;
        }
    }

    // Then check that all non-specified settings are populated to default
    if (inputReader.get () == nullptr)
        inputReader = AutoTrace::InputReaders[inputFile->suffix()];

    if (outputWriter.get () == nullptr)
        outputWriter = AutoTrace::OutputWriters["svg"];

    if (outputFile == nullptr)
    {
        outputFile = new AutoTrace::FileWrapper(inputFile->name() + "." + "svg");
        std::cout << "Output file name is now: " << outputFile->fullName() << std::endl;
    }

    // This program runs in two modes:
    //  * Information - Help, version, etc...
    //  * Converts raster image into vector image
    if (getInformation)
    {
        // TODO: Print help msg
        std::cout << "Print help msg... TODO" << std::endl;
    }
    else
    {
        bitmap = std::shared_ptr<AutoTrace::Bitmap>
                (inputReader.get()->func(inputFile->fullName(), inputOpts, nullptr));
//                (new AutoTrace::Bitmap(inputReader.get(),
//                                       inputFile->fullName(),
//                                       inputOpts));
        splines = std::make_shared<AutoTrace::SplineListArray>(*bitmap.get (), fittingOpts);

        // Dump loaded bitmap if needed
        // @ main.c#L166 - This block of code may be removed, it doesn't
        // seem needed anymore

        splines.get()->Write(outputWriter.get (),
                             outputFile->file,
                             outputFile->fullName(),
                             outputOpts); // TODO
    }

    delete fittingOpts;
    delete inputOpts;
    delete outputOpts;
    delete outputFile;
    delete dumpFile;

    return 0;
}

namespace AutoTrace {

void Init(std::string argZero)
{
    std::cout << "Arg0: " << argZero << std::endl;
    // Setup crap...
}

}

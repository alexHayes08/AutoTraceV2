#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "command.h"
#include "common.h"
#include "fittingoptions.h"
#include "inputoptions.h"
#include "outputoptions.h"

#include <locale>
#include <iostream>
#include <string>
#include <vector>

namespace AutoTrace {

class CommandHandler
{
public:
//    static void readCommandLine(std::vector<std::string> arguments,
//                                FittingOptions &fittingOpts,
//                                InputOptions &inputOpts,
//                                OutputOptions &outputOpts,
//                                FileName &inputFile,
//                                FileName &outputFile);
    static std::string parseInputForCommand (std::string input);
    static std::string parseInputForArgument (std::string input);
};

}

#endif // COMMANDHANDLER_H

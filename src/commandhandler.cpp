#include "commandhandler.h"

namespace AutoTrace {

std::string CommandHandler::parseInputForCommand(std::string input)
{
    int commandNameStart = 0;
    for (unsigned int i = 0; i < input.size (); i++)
    {
        if (input.at (i) == '-')
            commandNameStart++;
        else
            break;
    }

    int commandNameEnd = input.find_first_of ('=');

    if (commandNameEnd == std::string::npos)
        commandNameEnd = input.size ();
    commandNameEnd = commandNameEnd - commandNameStart;

    std::locale loc;
    auto command = input.substr (commandNameStart, commandNameEnd);
    for (std::string::size_type i = 0; i < command.size(); ++i)
    {
        std::tolower(command[i], loc);
    }

    return command;
}

std::string CommandHandler::parseInputForArgument (std::string input)
{
    int argumentNameStart = input.find_first_of ('=');
    if (argumentNameStart == std::string::npos)
    {
        return "";
    }
    else
    {
        int argumentNameEnd = input.size () - argumentNameStart;
        return input.substr (argumentNameStart + 1, argumentNameEnd);
    }
}

//void CommandHandler::readCommandLine(std::vector<std::string> arguments,
//                                     FittingOptions &fittingOpts,
//                                     InputOptions &inputOpts,
//                                     OutputOptions &outputOpts,
//                                     FileName &inputFile,
//                                     FileName &outputFile)
//{
//    Command::init(fittingOpts, inputOpts, outputOpts, inputFile, outputFile);

//    for (auto arg = arguments.begin(); arg != arguments.end(); ++arg)
//    {
//        auto command = CommandHandler::parseInputForCommand(arg);
//        auto argument = CommandHandler::parseInputForArgument(arg);

//        // Check if the map contains the command
//        if (Command::availableCommands::find(command) == Command::availableCommands::end())
//        {
//            std::string errorMsg = "Error: command ("
//                    + command + ") isn't a valid command!";
//            std::cout << errorMsg
//                      << std::endl;
//            throw errorMsg;
//        }

//        if (command != "help" && command != "version")
//            Command::availableCommands[command].handler(argument);
//    }
//}

}

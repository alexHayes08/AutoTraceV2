//#ifndef COMMAND_H
//#define COMMAND_H

//#include "common.h"
//#include "fittingoptions.h"
//#include "inputoptions.h"
//#include "outputoptions.h"

//#include <map>
//#include <string>

//namespace AutoTrace {

//class Command{
//public:
//    Command(std::string commandName, bool _takesArgument, void *(*handler)(std::string));
//    std::string commandName();
//    void (*handler)(std::string input);

//    static void init(FittingOptions &fittingOpts,
//                     InputOptions &inputOpts,
//                     OutputOptions &outputOpts,
//                     FileName &inputFile,
//                     FileName &outputOpts);
//    static std::map<std::string, Command> availableCommands;
//    static void registerCommand(Command command);

//private:
//    std::string _commandName;
//    bool _wasTriggered;
//    bool _takesArgument;
//    void _badArgument();
//    void _badCommand();

//    void helpCommand(std::string argument);
//    void backgroundColorCommand(std::string argument);

//    static FittingOptions _fittingOpts;
//    static InputOptions _inputOpts;
//    static OutputOptions _outputOpts;
//    static FileName _inputFile;
//    static FileName _outptuFile;
//};

//}

//#endif // COMMAND_H

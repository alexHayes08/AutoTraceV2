//#include "command.h"

//namespace AutoTrace {

//Command::init(FittingOptions &fittingOpts,
//              InputOptions &inputOpts,
//              OutputOptions &outputOpts,
//              FileName &inputFile,
//              FileName &outputFile)
//{
//    this->_fittingOpts = fittingOpts;
//    this->_inputOpts = inputOpts;
//    this->_outputOpts = outputOpts;
//    this->_inputFile = inputFile;
//    this->_outputOpts = outputFile;

//    new Command("help", true, &helpCommand);
//    new Command("background-color", true, &backgroundColorCommand);
//    new Command("charcode", true, );
//}

//Command::Command(std::string commandName,
//                 bool takesArgument,
//                 void *(*handler)(std::string))
//{
//    this->_commandName = commandName;
//    this->_takesArgument = takesArgument;
//    this->_wasTriggered = false;
//    this->handler = &takesArgument;

//    Command::availableCommands[commandName, *this];
//}

//// Help command
//void Command::helpCommand (std::string argument)
//{
//    std::cout << "Usage: autotrace [options] <input_name>.\n"
//              << "Options:<input_name> should be a supported image.\n"
//              << "\tYou can use `--' or `-' to start an option.\n"
//              << "\tYou can use any unambiguous abbreviation for an option name.\n"
//              << "\tYou can separate option names and values with `=' or ` '.\n"
//              << "background-color <hexadezimal>: the color of the background that\n"
//              << "\tshould be ignored, for example FFFFFF;\n"
//              << "\tdefault is no background color\n."
//              << "centerline: trace a character's centerline, rather than its outline.\n"
//              << "color-count <unsigned>: number of colors a color bitmap is reduced to,\n"
//              << "\tit does not work on grayscale, allowed are 1..256;\n"
//              << "\tdefault is 0, that means not color reduction is done.\n"
//              << "corner-always-threshold <angle-in-degrees>: if the angle at a pixel is\n"
//              << "\tless than this, it is considered a corner, even if it is within\n"
//              << "\t`corner-surround' pixels of another corner; default is 60.\n"
//              << "corner-surround <unsigned>: number of pixels on either side of a\n"
//              << "\tpoint to consider when determining if that point is a corner;\n"
//              << "\tdefault is 4.\n"
//              << "corner-threshold <angle-in-degrees>: if a pixel, its predecessor(s),\n"
//              << "\tand its successor(s) meet at an angle smaller than this, it's a\n"
//              << "\tcorner; default is 100.\n"
//              << "despeckle-level <unsigned>: 0..20; default is no despeckling.\n"
//              << "despeckle-tightness <real>: 0.0..8.0; default is 2.0.\n"
//              << "dpi <unsigned>: The dots per inch value in the input image, affects scaling\n"
//              << "\tof mif output image\n"
//              << "error-threshold <real>: subdivide fitted curves that are off by\n"
//              << "\tmore pixels than this; default is 2.0.\n"
//              << "filter-iterations <unsigned>: smooth the curve this many times\n"
//              << "\tbefore fitting; default is 4.\n"
//              << "input-format:  PNG, TGA, PBM, PNM, PGM, PPM or BMP.\n"
//              << "help: print this message.\n"
//              << "line-reversion-threshold <real>: if a spline is closer to a straight\n"
//              << "\tline than this, weighted by the square of the curve length, keep it a\n"
//              << "\tstraight line even if it is a list with curves; default is .01.\n"
//              << "line-threshold <real>: if the spline is not more than this far away\n"
//              << "\tfrom the straight line defined by its endpoints,\n"
//              << "\tthen output a straight line; default is 1.\n"
//              << "list-output-formats: print a list of support output formats to stderr.\n"
//              << "list-input-formats:  print a list of support input formats to stderr.\n"
//              << "log: write detailed progress reports to <input_name>.log.\n"
//              << "output-file <filename>: write to <filename>\n"
//              << "output-format <format>: use format <format> for the output file\n"
//              << "\teps, ai, p2e, sk, svg, fig, emf, mif, er, dxf, epd, pdf, cgm, meta, gmfa, meta, gmfb, plot, pnm, plot-pnm, pcl, plot-pcl, hpgl, plot-hpgl, tek, plot-tek, ..., magick, gcode, c, cairo, lwo, rib, rpl, java, java1, java2, kil, txt, text, mp, mpost, asy, m, mma, tex, latex2e, xml, noixml, pic, hpgl, pcl, dat, pcbi, pcb, pcbfill, gschem, cfdg, tk, vtk, svm, gnuplot, obj, tgif, idraw or dr2d can be used.\n"
//              << "preserve-width: whether to preserve line width prior to thinning.\n"
//              << "remove-adjacent-corners: remove corners that are adjacent.\n"
//              << "tangent-surround <unsigned>: number of points on either side of a\n"
//              << "\tpoint to consider when computing the tangent at that point; default is 3.\n"
//              << "report-progress: report tracing status in real time.\n"
//              << "debug-arch: print the type of cpu.\n"
//              << "debug-bitmap: dump loaded bitmap to <input_name>.bitmap.\n"
//              << "version: print the version number of this program.\n"
//              << "width-weight-factor <real>: weight factor for fitting the linewidth.\n\n"
//              << "You can get the source code of autotrace from\n"
//              << "http://autotrace.sourceforge.net;\n"
//              << std::endl;
//}

//void Command::backgroundColorCommand (std::string argument)
//{
//    this->_fittingOpts.background_color = std::shared_ptr<AutoTrace
//}

//}

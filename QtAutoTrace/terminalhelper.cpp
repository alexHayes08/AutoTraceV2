#include "terminalhelper.h"

/*
 * NOTE: List of terminal escape sequences found here:
 * http://ascii-table.com/ansi-escape-sequences-vt-100.php
 */

namespace Terminal
{
#pragma region Helpers

std::string getColor(VT100Color color, bool forForeground = true)
{
    switch (color)
    {
    case VT100Color::RED:
        return forForeground ? "41" : "31";
    case VT100Color::GREEN:
        return forForeground ? "42" : "32";
    case VT100Color::BLUE:
        return forForeground ? "44" : "34";
    case VT100Color::YELLOW:
        return forForeground ? "43" : "33";
    case VT100Color::CYAN:
        return forForeground ? "46" : "36";
    case VT100Color::MAGENTA:
        return forForeground ? "45" : "35";
    case VT100Color::WHITE:
        return forForeground ? "47" : "37";
    case VT100Color::BLACK:
        return forForeground ? "40" : "30";
    case VT100Color::INHERIT:
    default:
        throw std::runtime_error("Passed in color not supported yet.");
    }
}

char getDecChar(TerminalShape shape)
{
    switch (shape)
    {
    case TerminalShape::BOT_LEFT_CORNER:
        return 'm';
    case TerminalShape::BOT_RIGHT_CORNER:
        return 'j';
    case TerminalShape::BOT_TEE_INTERSECTION:
        return 'w';
    case TerminalShape::CROSS_INTERSECTION:
        return 'n';
    case TerminalShape::HORIZONTAL_LINE:
        return 'q';
    case TerminalShape::LEFT_TEE_INTERSECTION:
        return 'u';
    case TerminalShape::RIGHT_TEE_INTERSECTION:
        return 't';
    case TerminalShape::TOP_LEFT_CORNER:
        return 'l';
    case TerminalShape::TOP_RIGHT_CORNER:
        return 'k';
    case TerminalShape::TOP_TEE_INTERSECTION:
        return 'v';
    case TerminalShape::VERTICAL_LINE:
        return 'x';
    }
}

void setColors(VT100Color fg, VT100Color bg)
{
    if (fg != VT100Color::INHERIT)
        std::cout << CSI + getColor(fg, false) + "m";

    if (bg != VT100Color::INHERIT && bg != VT100Color::BLACK)
        std::cout << CSI + getColor(bg, true) + "m";
}

void resetColors(VT100Color fg, VT100Color bg)
{
    if (fg != VT100Color::INHERIT)
        std::cout << CSI "39m";

    if (bg != VT100Color::INHERIT)
        std::cout << CSI "49m";
}

#pragma endregion

TerminalHelper::TerminalHelper()
{
#ifdef Q_OS_WIN

    // Get output handle.
    this->hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (this->hStdout == INVALID_HANDLE_VALUE)
    {
        throw std::runtime_error("Failed to retreive handle to stdout.");
    }

    this->originalOutputMode = 0;

    GetConsoleMode(this->hStdout, &this->originalOutputMode);

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;

    DWORD dwOutMode = this->originalOutputMode | dwRequestedOutModes;
    this->setOutputMode = SetConsoleMode(this->hStdout, dwOutMode);
#endif
}

TerminalHelper::~TerminalHelper()
{
#ifdef Q_OS_WIN
    if (this->setOutputMode)
        SetConsoleMode(this->hStdout, this->originalOutputMode);
#endif
}

const TerminalHelper &TerminalHelper::backspace(int numberOfCharsToRemove) const
{
    for (auto i = 0; i < numberOfCharsToRemove; i++)
    {
        this->left()
            .print(" ")
            .left();
    }

    return *this;
}

const TerminalHelper &TerminalHelper::print(std::string message, bool newLine) const
{
    return this->print(message, VT100Color::INHERIT, VT100Color::INHERIT, newLine);
}

const TerminalHelper &TerminalHelper::print(std::string message, VT100Color fg, bool newLine) const
{
    return this->print(message, fg, VT100Color::INHERIT, newLine);
}

const TerminalHelper &TerminalHelper::print(std::string message, VT100Color fg, VT100Color bg, bool newLine) const
{
    // Set new colors.
    setColors(fg, bg);

    std::cout << message;

	if (newLine)
		this->newLine();

    // Reset colors
    resetColors(fg, bg);

    return *this;
}

const TerminalHelper &TerminalHelper::print(std::string message,
	OutputOptions options,
	bool newLine) const
{
	auto terminalDimensions = this->getTerminalDimensions();
	auto cursorPosition = this->getCursorPosition();
	auto charactersToPrint = message.size();

	// Check that there is room for the box.
	if (!options.maxWidthOfOutputAsPercent)
	{
		auto remainingWidth = terminalDimensions.width() - cursorPosition.x();
		if (remainingWidth < options.maxWidthOfOutput)
		{
			// Can't fit content on remaining area, move to it to a new line.
			this->newLine();
		}
	}

	// Set new colors.
	setColors(options.fg, options.bg);

	// Print message.

	if (newLine)
		this->newLine();

	// Reset colors.
	resetColors(options.fg, options.bg);
}

const TerminalHelper &TerminalHelper::up(int numberOfRows) const
{
    for (auto i = 0; i < numberOfRows; i++) {
        std::cout << CSI "A";
    }
    return *this;
}

const TerminalHelper &TerminalHelper::down(int numberOfRows) const
{
    for (auto i = 0; i < numberOfRows; i++) {
        std::cout << CSI "B";
    }
    return *this;
}

const TerminalHelper &TerminalHelper::left(int numberOfCols) const
{
    for (auto i = 0; i < numberOfCols; i++) {
        std::cout << CSI "D";
    }
    return *this;
}

const TerminalHelper &TerminalHelper::right(int numberOfCols) const
{
    for (auto i = 0; i < numberOfCols; i++) {
        std::cout << CSI "C";
    }
    return *this;
}

const TerminalHelper &TerminalHelper::clear() const
{
    // Clear screen.
    std::cout << CSI "2J";

    // Move cursor to upper-left hand corner.
    std::cout << CSI "0;0f";

    return *this;
}

const TerminalHelper &TerminalHelper::clearArea(QRect &area) const
{
    this->createRestorePoint();
    auto currentPosition = this->getCursorPosition();

    int dx = area.x() - currentPosition.x();
    int dy = area.y() - currentPosition.y();

    if (dx > 0)
    {
        this->left(uint(dx));
    }
    else
    {
        this->right(uint(dx * -1));
    }

    if (dy > 0)
    {
        this->up(uint(dy));
    }
    else
    {
        this->down(uint(dy * -1));
    }

    for (int row = 0; row < area.height(); row++)
    {
        for (int col = 0; col < area.width(); col++)
        {
            std::cout << ' ';
            this->left();
        }

        this->down();
    }

    // Restore point should be deconstructed right after this point restoring
    // the cursor to it's previous position.
    return *this;
}

const TerminalHelper &TerminalHelper::clearCurrentLine() const
{
    std::cout << CSI "2K";
    std::cout << '\r';

    return *this;
}

const TerminalHelper &TerminalHelper::reset() const
{
    std::cout << ESC "c";
    return *this;
}

const TerminalHelper &TerminalHelper::newLine() const
{
    std::cout << std::endl;
    return *this;
}

const TerminalHelper &TerminalHelper::alternativeBuffer() const
{
    std::cout << CSI "?1049h";
    return *this;
}

const TerminalHelper &TerminalHelper::mainBuffer() const
{
    std::cout << CSI "?1049l";
    return *this;
}

const TerminalHelper &TerminalHelper::blinkCursor(bool enableBlinking) const
{
    std::cout << enableBlinking ? CSI "?12h" : CSI "?12l";
    return *this;
}

const TerminalHelper &TerminalHelper::displayCursor(bool showCursor) const
{
    std::cout << showCursor ? CSI "?25h" : CSI "25l";
    return *this;
}

const TerminalHelper &TerminalHelper::fillLine(TerminalShape shape) const
{
    return this->fillLine(shape, shape, shape);
}

const TerminalHelper &TerminalHelper::fillLine(TerminalShape startingShape,
    TerminalShape midShape,
    TerminalShape endShape) const
{
    return this->fillLine(startingShape,
        midShape,
        endShape,
        VT100Color::INHERIT,
        VT100Color::INHERIT);
}

const TerminalHelper &TerminalHelper::fillLine(
    TerminalShape startingShape,
    TerminalShape midShape,
    TerminalShape endShape,
    VT100Color fg,
    VT100Color bg) const
{
    // Switch to the DEC character set.
    // NOTE: this will be disposed at function exit.
    auto lock = this->createDecCharLock();

    // Set colors.
    setColors(fg, bg);

    // Set the cursor position to the start of the line.
    std::cout << "\r";

    // Print out first shape.
    std::cout << getDecChar(startingShape);

    auto terminalSize = this->getTerminalDimensions();
    int widthOfMidChars = terminalSize.width() - 2;
    for (int i = 0; i < widthOfMidChars; i++)
    {
        std::cout << getDecChar(midShape);
    }

    // Print out last shape.
    std::cout << getDecChar(endShape);

    // Reset colors
    resetColors(fg, bg);

    return *this;
}

const TerminalHelper &TerminalHelper::printShape(TerminalShape shape,
    int numberToPrint) const
{
    for (int i = 0; i < numberToPrint; i++)
        std::cout << getDecChar(shape);

    return *this;
}

QRect TerminalHelper::getTerminalDimensions() const
{
#ifdef Q_OS_WIN
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(this->hStdout, &csbiInfo);

    return QRect(0, 0, csbiInfo.dwSize.X, csbiInfo.dwSize.Y);
#else
    return QRect(0, 0, 100, 100);
#endif
}

QPoint TerminalHelper::getCursorPosition() const
{
#ifdef Q_OS_WIN
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(this->hStdout, &csbiInfo);

    int x = csbiInfo.dwCursorPosition.X;
    int y = csbiInfo.dwCursorPosition.Y;
    return QPoint(x, y);
#else
    std::cout << CSI + "[6n";

    return QPoint(1,1);
#endif
}

bool TerminalHelper::supportsVT100()
{
#ifdef Q_OS_WIN
    return this->setOutputMode;
#else
    return true;
#endif
}

std::unique_ptr<TerminalPositionRestore> TerminalHelper::createRestorePoint() const
{
    return std::make_unique<TerminalPositionRestore>();
}

std::unique_ptr<DEC_CharacterSetLock> TerminalHelper::createDecCharLock() const
{
    return std::make_unique<DEC_CharacterSetLock>();
}
}

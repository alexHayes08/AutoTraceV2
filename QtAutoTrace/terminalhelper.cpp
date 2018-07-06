#include "terminalhelper.h"

/*
 * NOTE: List of terminal escape sequences found here:
 * http://ascii-table.com/ansi-escape-sequences-vt-100.php
 */

namespace QtAutoTraceV2
{
TerminalHelper::TerminalHelper()
{
#ifdef Q_OS_WIN

    // Get output handle.
    this->hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (this->hStdout == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to retreive handle to stdout.");

    // Get input handle.
    this->hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (this->hStdin == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Failed to retrieve handle to stdin.");

    DWORD dwOriginalOutputMode = 0;
    DWORD dwOriginalInputMode = 0;

    if (!GetConsoleMode(this->hStdout, &dwOriginalOutputMode))
        throw std::runtime_error("Error retrieving the input mode for stdout.");

    if (!GetConsoleMode(this->hStdin, &dwOriginalInputMode))
        throw std::runtime_error("Error retrieving the input mode for stdin.");

    DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
//    DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

    DWORD dwOutMode = dwOriginalOutputMode | dwRequestedOutModes;
    if (!SetConsoleMode(this->hStdout, dwOutMode))
    {
        // We failed to set both modes, try to step down mode gracefully.
        dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        dwOutMode = dwOriginalOutputMode | dwRequestedOutModes;
        if (!SetConsoleMode(this->hStdout, dwOutMode))
        {
            // Failed to set any VT mode, can't do anything here.
            throw std::runtime_error("Failed to set any VT mode. Please use a VT compliant terminal.");
        }
    }

    DWORD dwInMode = dwOriginalInputMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(this->hStdin, dwInMode))
    {
        // Failed to set VT input mode, can't do anything here.
        throw std::runtime_error("Failed to set VT input mode. Please use a VT compliant terminal.");
    }
#endif
}

TerminalHelper TerminalHelper::backspace(int numberOfCharsToRemove) const
{
    for (auto i = 0; i < numberOfCharsToRemove; i++)
    {
        this->right();
        std::cout << ' ';
        this->right();
    }

    return *this;
}

TerminalHelper TerminalHelper::print(std::string message, bool newLine) const
{
    return this->print(message, QColor(), QColor(), newLine);
}

TerminalHelper TerminalHelper::print(std::string message, QColor fg, bool newLine) const
{
    this->print(message, fg, QColor(), newLine);
    return *this;
}

TerminalHelper TerminalHelper::print(std::string message, QColor fg, QColor bg, bool newLine) const
{
    // Retrieve current colors.
#ifdef Q_OS_WIN
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    WORD wOldColorAttrs;
    if (!GetConsoleScreenBufferInfo(this->hStdout, &csbiInfo))
    {
        throw std::runtime_error("Failed to retrieve the terminals info.");
    }

    wOldColorAttrs = csbiInfo.wAttributes;

    // Set new colors.
    if (fg.isValid())
    {
        if (!SetConsoleTextAttribute(this->hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY))
        {
            throw std::runtime_error("Failed to set foreground color in terminal.");
        }
    }

    if (bg.isValid())
    {
        if (!SetConsoleTextAttribute(this->hStdout, BACKGROUND_BLUE | BACKGROUND_INTENSITY))
        {
            throw std::runtime_error("Failed to set background color in terminal.");
        }
    }

    // Print message.
    std::cout << message;

    if (newLine)
        std::cout << std::endl;

    // Set colors to their original. Don't need to check this one if the first
    // call to SetConsoleTextAttribute succeeded.
    SetConsoleTextAttribute(this->hStdout, wOldColorAttrs);
#endif

    return *this;
}

TerminalHelper TerminalHelper::up(int numberOfRows) const
{
    for (auto i = 0; i < numberOfRows; i++) {
        std::cout << EscSeq << "A";
    }
    return *this;
}

TerminalHelper TerminalHelper::down(int numberOfRows) const
{
    for (auto i = 0; i < numberOfRows; i++) {
        std::cout << EscSeq + "B";
    }
    return *this;
}

TerminalHelper TerminalHelper::left(int numberOfCols) const
{
    for (auto i = 0; i < numberOfCols; i++) {
        std::cout << EscSeq + "D";
    }
    return *this;
}

TerminalHelper TerminalHelper::right(int numberOfCols) const
{
    for (auto i = 0; i < numberOfCols; i++) {
        std::cout << EscSeq + "C";
    }
    return *this;
}

TerminalHelper TerminalHelper::clear() const
{
    // Clear screen.
    std::cout << EscSeq + "2J";

    // Move cursor to upper-left hand corner.
    std::cout << EscSeq + ";f";

    return *this;
}

TerminalHelper TerminalHelper::clearArea(QRect &area) const
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

TerminalHelper TerminalHelper::clearCurrentLine() const
{
    std::cout << EscSeq + "2K";
    std::cout << '\r';

    return *this;
}

TerminalHelper TerminalHelper::reset() const
{
    std::cout << EscSeq + "c";
    return *this;
}

TerminalHelper TerminalHelper::newLine() const
{
    std::cout << std::endl;
	return *this;
}

QRect TerminalHelper::getTerminalDimensions() const
{
    return QRect(0, 0, 100, 100);
}

QPoint TerminalHelper::getCursorPosition() const
{
    std::cout << EscSeq + "[6n";

    return QPoint(1,1);
}

std::unique_ptr<TerminalPositionRestore> TerminalHelper::createRestorePoint() const
{
    return std::make_unique<TerminalPositionRestore>();
}
}

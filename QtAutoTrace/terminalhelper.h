#ifndef TERMINALHELPER_H
#define TERMINALHELPER_H

#include <iostream>
#include <memory>
#include <string>

#include <QPoint>
#include <QRect>

#include "terminalsequences.h"
#include "outputoptions.h"

#ifdef Q_OS_WIN

#include "stdio.h"
#include "wchar.h"
#include "windows.h"

// Helper in case ENABLE_VIRTUAL_TERMINAL_PROCESSING isn't defined.
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

#elif Q_OS_LINUX

#include <ioctl>

#endif

/**
 * TODO: Move all files in namespace 'Terminal' to their own project.
 */
namespace Terminal
{

#pragma region Enums

enum class TerminalShape
{
    BOT_RIGHT_CORNER,
    TOP_RIGHT_CORNER,
    TOP_LEFT_CORNER,
    BOT_LEFT_CORNER,
    CROSS_INTERSECTION,
    HORIZONTAL_LINE,
    VERTICAL_LINE,
    RIGHT_TEE_INTERSECTION,
    LEFT_TEE_INTERSECTION,
    TOP_TEE_INTERSECTION,
    BOT_TEE_INTERSECTION
};

#pragma endregion

// Forward declarations.
class OutputOptions;
class TerminalPositionRestore;
class DEC_CharacterSetLock;

/**
 * @brief The TerminalHelper class allows for more advanced usage when printing
 * to a terminal. This class uses std::cout and std::cin for the inputs/outputs.
 * Requires the terminal to be VT100 compliant as it uses ANSI escape sequences
 * internally. If used on Windows requires Windows 10 with the Anniversary
 * Update or later.
 */
class TerminalHelper
{
public:
    TerminalHelper();
    ~TerminalHelper();

    /**
     * @brief Will remove n(defaults to one) characters from the current line.
     * @param numberOfCharsToRemove
     * @return
     */
    const TerminalHelper &backspace(int numberOfCharsToRemove = 1) const;

    /**
     * @brief Clears the terminal of all characters.
     * @return
     */
    const TerminalHelper &clear() const;

    /**
     * @brief Removes a rectangular region in the terminal of all characters. If
     * there are characters on the right side of the region that area
     * intersecting that line will be replaced with spaces.
     * @param area
     * @return
     */
    const TerminalHelper &clearArea(QRect &area) const;

    /**
     * @brief Sets the cursor to the first column and clears the line of all
     * characters.
     * @return
     */
    const TerminalHelper &clearCurrentLine() const;

    /**
     * @brief Inserts a new line under the curor and moves the cursor to it.
     * @return
     */
    const TerminalHelper &newLine() const;

    /**
     * @brief Prints a string the terminal.
     * @param message
     * @param newLine
     * @return
     */
    const TerminalHelper &print(std::string message,
        bool newLine = false) const;

    /**
     * @brief Prints a string to the terminal.
     * @param message
     * @param fg The color of the text.
     * @param newLine
     * @return
     */
    const TerminalHelper &print(std::string message,
        VT100Color fg,
        bool newLine = false) const;

    /**
     * @brief Prints a string to the terminal.
     * @param message
     * @param fg The color of the text.
     * @param bg The color of the text background.
     * @param newLine
     * @return
     */
    const TerminalHelper &print(std::string message,
        VT100Color fg,
        VT100Color bg,
        bool newLine = false) const;

    /**
     * @brief Prints the string to the console according to the styling provided
     * in options.
     * @param message
     * @param options
     * @param newLine
     * @return
     */
    const TerminalHelper &print(std::string message,
        OutputOptions options,
        bool newLine = false) const;

    /**
     * @brief Moves the cursor up.
     * @param numberOfRows The number of rows to move up.
     * @return
     */
    const TerminalHelper &up(int numberOfRows = 1) const;

    /**
     * @brief Moves the cursor down.
     * @param numberOfRows
     * @return
     */
    const TerminalHelper &down(int numberOfRows = 1) const;

    /**
     * @brief Moves the cursor left.
     * @param numberOfCols
     * @return
     */
    const TerminalHelper &left(int numberOfCols = 1) const;

    /**
     * @brief Moves the cursor right.
     * @param numberOfCols
     * @return
     */
    const TerminalHelper &right(int numberOfCols = 1) const;

    /**
     * @brief Resets the console of all attribute changes such as resetting the
     * colors back to default.
     * @return
     */
    const TerminalHelper &reset() const;

    /**
     * @brief Switches the terminal to an alternate buffer. Calling again will
     * destroy the existing alternate buffer and replace it with a new one.
     * There is no way to undo or bring back a destroyed buffer. Switching to
     * the main buffer will not clear nor destory the alternate buffer.
     * @return
     */
    const TerminalHelper &alternativeBuffer() const;

    /**
     * @brief Switches the terminal to the main buffer.
     * @return
     */
    const TerminalHelper &mainBuffer() const;

    /**
     * @brief Causes the cursor to start/stop blinking.
     * @param enableBlinking
     * @return
     */
    const TerminalHelper &blinkCursor(bool enableBlinking) const;

    /**
     * @brief Displays/hides the cursor.
     * @param showCursor
     * @return
     */
    const TerminalHelper &displayCursor(bool showCursor) const;

    /**
     * @brief Fills the current line (from the first column to the last column)
     * with a shape. This does NOT create a new line.
     * @param shape
     * @return
     */
    const TerminalHelper &fillLine(TerminalShape shape) const;

    /**
     * @brief On the current line sets the first column to the startingShape,
     * the last column to the endShape, and everything inbetween is set to the
     * midShape. This does NOT create a new line.
     * @param startingShape
     * @param midShape
     * @param endShape
     * @return
     */
    const TerminalHelper &fillLine(TerminalShape startingShape,
        TerminalShape midShape,
        TerminalShape endShape) const;

    /**
     * @brief On the current line sets the first column to the startingShape,
     * the last column to the endShape, and everything inbetween is set to the
     * midShape. This does NOT create a new line.
     * @param startingShape
     * @param midShape
     * @param endShape
     * @param fg
     * @param bg
     * @return
     */
    const TerminalHelper &fillLine(TerminalShape startingShape,
        TerminalShape midShape,
        TerminalShape endShape,
        VT100Color fg,
        VT100Color bg) const;

    /**
     * @brief Prints a shape x(defaults to 1) amount of times on the current
     * line.
     * @param shape
     * @param numberToPrint
     * @return
     */
    const TerminalHelper &printShape(TerminalShape shape,
        int numberToPrint = 1) const;

    /**
     * @brief Retrieves the current dimensions of the terminal. Units are in
     * character width/heights.
     * @return
     */
    QRect getTerminalDimensions() const;

    /**
     * @brief Retrieves the current position of the cursor. Units are in
     * character width/heights.
     * @return
     */
    QPoint getCursorPosition() const;

    /**
     * @brief Returns true/false if the current terminal supports the VT100
     * interface.
     * @return
     */
    bool supportsVT100();

private:
    std::unique_ptr<TerminalPositionRestore> createRestorePoint() const;
    std::unique_ptr<DEC_CharacterSetLock> createDecCharLock() const;

#ifdef Q_OS_WIN
    HANDLE hStdout;
    DWORD originalOutputMode;
    bool setOutputMode;
#endif
};

class DEC_CharacterSetLock
{
public:
    DEC_CharacterSetLock()
    {
        // Switch to DEC character set.
        std::cout << ESC "(0";
    }
    ~DEC_CharacterSetLock()
    {
        // Switch to ASCII character set.
        std::cout << ESC "(B";
    }
};

class TerminalPositionRestore
{
public:
    TerminalPositionRestore()
    {
        // Store cursor postion + attributes.
        std::cout << ESC "7";
    }
    ~TerminalPositionRestore()
    {
        // Restore cursor position.
        std::cout << ESC "8";
    }
};

}

#endif // TERMINALHELPER_H

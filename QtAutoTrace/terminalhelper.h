#ifndef TERMINALHELPER_H
#define TERMINALHELPER_H

#include <iostream>
#include <memory>
#include <string>

#include <QColor>
#include <QPoint>
#include <QRect>

#ifdef Q_OS_WIN

#include "stdio.h"
#include "wchar.h"
#include "windows.h"

// Helper in case ENABLE_VIRTUAL_TERMINAL_PROCESSING isn't defined.
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#elif Q_OS_LINUX

#include <ioctl>

#endif

namespace QtAutoTraceV2
{

// Forward declaration of TerminalPositionRestore.
class TerminalPositionRestore;

class TerminalHelper
{
public:
    TerminalHelper();
    TerminalHelper backspace(int numberOfCharsToRemove = 0) const;
    TerminalHelper clear() const;
    TerminalHelper clearArea(QRect &area) const;
    TerminalHelper clearCurrentLine() const;
    TerminalHelper newLine() const;
    TerminalHelper print(std::string message, bool newLine = false) const;
    TerminalHelper print(std::string message, QColor fg, bool newLine = false) const;
    TerminalHelper print(std::string message, QColor fg, QColor bg, bool newLine = false) const;
    TerminalHelper up(int numberOfRows = 1) const;
    TerminalHelper down(int numberOfRows = 1) const;
    TerminalHelper left(int numberOfCols = 1) const;
    TerminalHelper right(int numberOfCols = 1) const;
    TerminalHelper reset() const;
    QRect getTerminalDimensions() const;
    QPoint getCursorPosition() const;

    static constexpr char EscSeq = 0x1b;

private:
    std::unique_ptr<TerminalPositionRestore> createRestorePoint() const;

#ifdef Q_OS_WIN
    HANDLE hStdin;
    HANDLE hStdout;
#endif
};

class TerminalPositionRestore
{
public:
    TerminalPositionRestore()
    {
        // Store cursor postion + attributes.
        std::cout << TerminalHelper::EscSeq + "7";
    }
    ~TerminalPositionRestore()
    {
        // Restore cursor position.
        std::cout << TerminalHelper::EscSeq + "8";
    }
};

}

#endif // TERMINALHELPER_H

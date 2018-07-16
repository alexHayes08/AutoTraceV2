#ifndef OUTPUTOPTIONS_H
#define OUTPUTOPTIONS_H

#include "qtautotracelib_global.h"

namespace Terminal
{

#pragma region Enums

enum class Alignment
{
    NONE,
    LEFT,
    CENTER,
    RIGHT
};

enum class Display
{
    NONE,
    BLOCK,
    INLINE,
    FLEX,
    INLINE_FLEX
};

enum class TextWrap
{
    NO_WRAP,
    WRAP,
    BREAK_WORD
};

enum class TextOverflow
{
    HIDDEN,
    ELLIPSIS_LEFT,
    ELLIPSIS_CENTER,
    ELLIPSIS_RIGHT
};

enum class VT100Color {
    INHERIT,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
    BLACK
};

#pragma endregion

#pragma region Structs

struct QTAUTOTRACELIBSHARED_EXPORT Spacing
{
    int top;
    bool topAsPercent;

    int bottom;
    bool bottomAsPercent;

    int left;
    bool leftAsPercent;

    int right;
    bool rightAsPercent;
};

#pragma endregion

class QTAUTOTRACELIBSHARED_EXPORT OutputOptions
{
public:
    OutputOptions();
    Display display;
    int maxWidthOfOutput;
    bool maxWidthOfOutputAsPercent;
    int maxHeightOfOutput;
    bool maxHeightOfOutputAsPercent;
    Alignment horizontalAlign;
    Alignment verticalAlign;
    Spacing margins;
    Spacing padding;
    TextWrap textWrap;
    TextOverflow textOverflow;
    VT100Color fg;
    VT100Color bg;
    int height;
    int width;
};

}

#endif // OUTPUTOPTIONS_H

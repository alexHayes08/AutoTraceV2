#include "outputoptions.h"

namespace Terminal
{

OutputOptions::OutputOptions()
{
	this->display = Display::BLOCK;
	this->bg = VT100Color::INHERIT;
	this->fg = VT100Color::INHERIT;
	this->maxWidthOfOutput = 100;
	this->maxWidthOfOutputAsPercent = true;
	this->maxHeightOfOutput = 100;
	this->maxHeightOfOutputAsPercent = true;
	this->horizontalAlign = Alignment::NONE;
	this->horizontalAlign = Alignment::NONE;
	this->margins = Spacing { 0,false, 0,false, 0,false, 0,false };
	this->padding = Spacing { 0,false, 0,false, 0,false, 0,false };
	this->textWrap = TextWrap::WRAP;
	this->textOverflow = TextOverflow::HIDDEN;
}

}

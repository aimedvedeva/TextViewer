#include "font.h"

HFONT GetFixedFont(Font myFont)
{
	static LOGFONT font;

	GetObject(GetStockObject(ANSI_FIXED_FONT), sizeof(LOGFONT), &font);

	font.lfHeight = myFont.letterHeight;
	font.lfWidth = myFont.letterWidth;

	return CreateFont(font.lfHeight, font.lfWidth,
		font.lfEscapement, font.lfOrientation, font.lfWeight,
		font.lfItalic, font.lfUnderline, font.lfStrikeOut,
		font.lfCharSet, font.lfOutPrecision, font.lfClipPrecision,
		font.lfQuality, font.lfPitchAndFamily, font.lfFaceName);
}



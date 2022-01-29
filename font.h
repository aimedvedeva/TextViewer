#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <windows.h>

typedef struct{
   int letterHeight;
   int letterWidth;
}Font;


/**
  PARAM[IN]:  initialized Font variable
  PARAM[OUT]: ID of the monospaced font
*/
HFONT GetFixedFont(Font myFont);

#endif // FONT_H_INCLUDED

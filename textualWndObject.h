#ifndef TEXTUALWNDOBJECT_H_INCLUDED
#define TEXTUALWNDOBJECT_H_INCLUDED

#include <windows.h>
#include "fileProxy.h"
#include "wrappedText.h"
#include "textViewerMode.h"
#include "font.h"

typedef struct{
  TEXT_VIEWER_MODES textViewerMode;

  WrappedText wrappedText;
  WrappedText unwrappedText;

  Font font;
  int displayedLinesNumber;  //the number of lines in work space of the window
  int textPosition;          //the index of line in lines array from that painting starts in UNWRAP mode
  char *textPos;             //the pointer to character from that painting starts in WRAP mode
  int textOffset;
}TextualWndObject;



void initializeTextualWndObject(TextualWndObject *textualWndObject, char *fileName, HWND hwnd);
void destroyTextualWndObject(TextualWndObject *textualWndObject);

void createTextualWndObject(TextualWndObject *textualWndObject);

/**
  PARAM[IN]:  pointer to  TextualWndObject structure,
              client window height

  PARAM[OUT]: number of lines in work space of window
*/
int  getDisplayedLinesNumber(TextualWndObject *textualWndObject, int clientWindowHeight);
void displayTextualWndObject(TextualWndObject *textualWndObject, HDC hdc);
void initTextPosition(TextualWndObject *TextualWndObject);

#endif // TEXTUALWNDOBJECT_H_INCLUDED

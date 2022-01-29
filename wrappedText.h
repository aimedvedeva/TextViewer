#ifndef WRAPPEDTEXT_H_INCLUDED
#define WRAPPEDTEXT_H_INCLUDED

#include "text.h"
#include "textViewerMode.h"
#include "font.h"


typedef struct{
    Text *text;              //pointer to the structure of original data
    char **linesArray;       //array of pointer to wrapped/unwrapped lines
    int linesArraySize;
}WrappedText;


/**
  PARAM[IN]:  the pointer to WrappedText structure,
              the pointer to Text structure,

  PARAM[OUT]:

  The function initializes the filed of WrappedText by pointer to Text structure.
*/
void initializeWrappedText(WrappedText *wrappedText, Text *text);

/**
  PARAM[IN]:  the pointer to WrappedText structure,

  PARAM[OUT]:

  The function cleans allocated memory for field linesArray of WrappedText structure.
*/
void destroyWrappedText(WrappedText *wrappedText);

/**
  PARAM[IN]:  the pointer to WrappedText structure that contains unwrapped text

  PARAM[OUT]:

  The function initializes the filed - linesArray of WrappedText structure for UNWRAP mode.
*/
void unwrapText(WrappedText *wrappedText);


/**
  PARAM[IN]:  the pointer to WrappedText structure that contains wrapped text,
              the pointer to the character from that wrapping starts
              the number of lines in work space of application window
              client window width
              Font variable

  PARAM[OUT]:

  The function initializes the field - linesArray of WrappedText structure for WRAP mode.
*/
void wrapText(WrappedText *wrappedText, char *textPos, int displayedLinesNumber, int clientWindowWidth, Font font);

/**
  PARAM[IN]:  the pointer to WrappedText structure that contains wrapped text,
              HDC structure
              the number of lines in work space of application window
              the index if line in linesArray from that painting starts
              text offset in UNWRAP mode
              Font variable
              mode

  PARAM[OUT]:
*/
void displayWrappedText(WrappedText *wrappedText, HDC hdc, int displayedLinesNumber, int textPosition, int textOffset, Font font, TEXT_VIEWER_MODES mode);

#endif // WRAPPEDTEXT_H_INCLUDED

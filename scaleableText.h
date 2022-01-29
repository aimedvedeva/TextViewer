#ifndef SCALEABLETEXT_H_INCLUDED
#define SCALEABLETEXT_H_INCLUDED

#include <windows.h>
#include "font.h"
#include "text.h"
#include "line.h"


typedef struct{
    Text *text;
    Line *linesArray;
    int linesArraySize;
    int lineSize;
}ScaleableText;

typedef struct{
    Text *text;
    char **linesArray;
    int linesArraySize;
}ScaleableText;

void initializeScaleableText(ScaleableText *scaleableText, char *textData, int textDataSize);
void destroyScaleableText(ScaleableText *scaleableText);
void setLineSize(ScaleableText *scaleableText, int clientWindowWidth, Font font);
void scaleText(ScaleableText *scaleableText);
void displayScaleableText(HDC hdc, ScaleableText *scaleableText, int displayedLinesNumber, int textPosition, Font font);

#endif // SCALEABLETEXT_H_INCLUDED

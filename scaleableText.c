#include "scaleableText.h"

void displayScaleableText(HDC hdc, ScaleableText *scaleableText, int displayedLinesNumber, int textPosition, Font font){
    int X = 0;
    int Y = 0;

    int paintedLinesNum = min(displayedLinesNumber, scaleableText->linesArraySize);

    for(int line = textPosition; line < (paintedLinesNum - 1) + textPosition; line++){
            TextOutA(hdc, X, Y, scaleableText->linesArray[line].line, scaleableText->lineSize);
            Y += font.letterHeight;

        int restDataLen = scaleableText->text->textSize - ((paintedLinesNum - 1) * scaleableText->lineSize);
        if(restDataLen  < scaleableText->lineSize){
            TextOut(hdc, X, Y, scaleableText->linesArray[paintedLinesNum - 1 + (textPosition - 1)].line, restDataLen);
        }
        else if(restDataLen  == scaleableText->lineSize){
            TextOut(hdc, X, Y, scaleableText->linesArray[paintedLinesNum - 1 + (textPosition - 1)].line, scaleableText->lineSize);
        }
    }
}

int getScaleableTextLinesNumber(int textSize, int lineLength){
  int linesNumber = textSize / lineLength;

  if(textSize - (linesNumber * lineLength) > 0){
    linesNumber += 1;
  }
  return linesNumber;
}

void scaleText(ScaleableText *scaleableText){
  scaleableText->linesArraySize = getScaleableTextLinesNumber(scaleableText->text->textSize, scaleableText->lineSize);

  if(scaleableText->linesArray == NULL){
    scaleableText->linesArray = malloc(scaleableText->linesArraySize * sizeof(Line));
  }

  void *dataPosition = scaleableText->text->textData;

  for(int line = 0; line < scaleableText->linesArraySize; line++){
    scaleableText->linesArray[line].line = dataPosition;

    int restDataLength = scaleableText->text->textSize - (line * scaleableText->lineSize);
    if(restDataLength > scaleableText->lineSize){
        dataPosition += scaleableText->lineSize;
    }
    else if(0 < restDataLength && restDataLength < scaleableText->lineSize){
        dataPosition += restDataLength;
    }
    else if(restDataLength <= 0){
        break;
    }
  }
  return;
}

void destroyScaleableText(ScaleableText *scaleableText){
    free(scaleableText->linesArray);
}

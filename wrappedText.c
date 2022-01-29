#include "wrappedText.h"

void initializeWrappedText(WrappedText *wrappedText, Text *text){
    wrappedText->text = text;
}

int getWrappedTextLinesNumber(int textSize, int lineLength){
  int linesNumber = textSize / lineLength;

  if(textSize - (linesNumber * lineLength) > 0){
    linesNumber += 1;
  }
  return linesNumber;
}

void wrapText(WrappedText *wrappedText, char *textPos, int displayedLinesNumber, int clientWindowWidth, Font font){
    int lineSize = clientWindowWidth / font.letterWidth;
    int symbolsNumber = lineSize * displayedLinesNumber;
    int restTextSize = wrappedText->text->textSize - (textPos - wrappedText->text->textData);

    if(restTextSize >= symbolsNumber){
      wrappedText->linesArraySize =  displayedLinesNumber;
    }
    else {
      wrappedText->linesArraySize = restTextSize / lineSize;
      if(wrappedText->linesArraySize * lineSize < restTextSize){
        wrappedText->linesArraySize += 1;
      }
    }

    free(wrappedText->linesArray);
    wrappedText->linesArray = malloc(wrappedText->linesArraySize * sizeof(char*) + 1);


    void *dataPosition = textPos;

    for(int line = 0; line < wrappedText->linesArraySize; line++){
      wrappedText->linesArray[line] = dataPosition;

      int restDataLength = restTextSize - ((line) * lineSize);

      if(restDataLength > lineSize){
          dataPosition += lineSize;
      }
      else if(0 < restDataLength && restDataLength < lineSize){
          dataPosition += restDataLength;
      }
      else if(restDataLength <= 0){
          break;
      }
    }
    return;
}

void unwrapText(WrappedText *wrappedText){
    wrappedText->linesArray = malloc(sizeof(char*) * wrappedText->text->textSize);

    wrappedText->text->maxLineSize = 0;
    int curLineLen  = 0;
    int charCounter = 0;
    int curLine     = 0;
    int pointerShift = 0;
    int prevLineSize = 0;
    for(; charCounter <= wrappedText->text->textSize; charCounter++, curLineLen++){
        if(wrappedText->text->textData[charCounter] == '\n'){

            if(curLine != 0){
               pointerShift += prevLineSize + 1;
            }

            wrappedText->linesArray[curLine] =  wrappedText->text->textData + pointerShift;
            prevLineSize = curLineLen;

            if(curLineLen > wrappedText->text->maxLineSize){
                wrappedText->text->maxLineSize = curLineLen;
            }

            charCounter += 1;
            curLineLen = 0;
            curLine += 1;
        }
        if(wrappedText->text->textData[charCounter] == '\0'){
             if(curLine != 0){
               pointerShift += prevLineSize + 1;
            }

            wrappedText->linesArray[curLine] =  wrappedText->text->textData + pointerShift;
            prevLineSize = curLineLen;

            if(curLineLen > wrappedText->text->maxLineSize){
                wrappedText->text->maxLineSize = curLineLen;
            }

            curLineLen = 0;
            curLine += 1;
        }
        if(wrappedText->text->textData[charCounter] == '\0'){
            wrappedText->linesArraySize = curLine;
            break;
        }
    }
}

void displayWrappedText(WrappedText *wrappedText, HDC hdc, int displayedLinesNumber, int textPosition, int textOffset, Font font, TEXT_VIEWER_MODES mode){
   int X = 0;
   int Y = 0;

   int curLinesNum = min(displayedLinesNumber, wrappedText->linesArraySize);
   int line;

   if(mode == TEXT_VIEWER_MODES_WRAP){
     textPosition = 0;
   }
   for(line = textPosition; line < displayedLinesNumber - 1 + textPosition && line < wrappedText->linesArraySize - 1; line++){
      int curLineLen = wrappedText->linesArray[line + 1] - wrappedText->linesArray[line];
      TextOutA(hdc, X, Y, wrappedText->linesArray[line] + sizeof(char) * textOffset, curLineLen - textOffset);
      Y += font.letterHeight;
   }

   int lastLineLen = 0;
   if(line == wrappedText->linesArraySize - 1){
     lastLineLen = (wrappedText->text->textData + wrappedText->text->textSize * sizeof(char)) - wrappedText->linesArray[wrappedText->linesArraySize - 1];
   }
   else if(line < wrappedText->linesArraySize - 1){
     lastLineLen = wrappedText->linesArray[line] - wrappedText->linesArray[line - 1];
   }
   else{
      return;
   }

   TextOutA(hdc, X, Y, wrappedText->linesArray[line] + sizeof(char) * textOffset, lastLineLen - textOffset);
}

void destroyWrappedText(WrappedText *wrappedText){
    free(wrappedText->linesArray);
}

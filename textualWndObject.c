#include <stdio.h>
#include "textualWndObject.h"

int getDisplayedLinesNumber(TextualWndObject *textualWndObject, int clientWindowHeight){
    return clientWindowHeight / textualWndObject->font.letterHeight + 1;
}

void displayTextualWndObject(TextualWndObject *textualWndObject, HDC hdc){
    if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
      displayWrappedText(&textualWndObject->unwrappedText, hdc, textualWndObject->displayedLinesNumber, textualWndObject->textPosition, textualWndObject->textOffset, textualWndObject->font, textualWndObject->textViewerMode);
    }
    else if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_WRAP){
      displayWrappedText(&textualWndObject->wrappedText, hdc, textualWndObject->displayedLinesNumber, textualWndObject->textPosition, textualWndObject->textOffset, textualWndObject->font, textualWndObject->textViewerMode);
    }
}

void destroyTextualWndObject(TextualWndObject *textualWndObject){
    destroyText(textualWndObject->wrappedText.text);
    destroyWrappedText(&textualWndObject->unwrappedText);
    destroyWrappedText(&textualWndObject->wrappedText);
    textualWndObject->wrappedText.text = NULL;
    textualWndObject->unwrappedText.linesArray = NULL;
    textualWndObject->wrappedText.linesArray = NULL;
}

void initializeTextualWndObject(TextualWndObject *textualWndObject, char *fileName, HWND hwnd){
    FILE *file = NULL;

    file = fopen(fileName, "rb");
    if(isFileOpened(file)){
       if(!isFileEmpty(file)){
               Text *text = createText();
               text->textSize = getFileSize(file);
               text->textData = getFileData(file, text->textSize);

               initializeWrappedText(&textualWndObject->unwrappedText, text);
               initializeWrappedText(&textualWndObject->wrappedText, text);
               textualWndObject->unwrappedText.linesArray = NULL;
               textualWndObject->wrappedText.linesArray = NULL;
           }
       else{
           errno = ERROR_FILE_INVALID;
           perror("An error has occurred: ");
           return;
       }
   }

   RECT clientWindowRect;
   GetClientRect(hwnd, &clientWindowRect);
   int clientWindowHeight = clientWindowRect.bottom - clientWindowRect.top;

   textualWndObject->textPosition = 0;
   textualWndObject->textOffset = 0;
   textualWndObject->displayedLinesNumber = getDisplayedLinesNumber(textualWndObject, clientWindowHeight);

   unwrapText(&textualWndObject->unwrappedText);

   textualWndObject->textPos = textualWndObject->wrappedText.text->textData;
}

void initTextPosition(TextualWndObject *textualWndObject){
    char *symbol = textualWndObject->textPos;
    int midIndex;
    int min = 0;
    int max = textualWndObject->unwrappedText.linesArraySize;

    while(min <= max){
        midIndex = min + (max - min) / 2;

        int lineSize;
        if(midIndex != textualWndObject->unwrappedText.linesArraySize){
            lineSize = textualWndObject->unwrappedText.linesArray[midIndex + 1] - textualWndObject->unwrappedText.linesArray[midIndex];
        }
        else if(midIndex == textualWndObject->unwrappedText.linesArraySize && symbol >= textualWndObject->unwrappedText.linesArray[midIndex]){
            textualWndObject->textPosition = midIndex;
            return;
        }

        if(symbol <= (textualWndObject->unwrappedText.linesArray[midIndex] + sizeof(char) * (lineSize - 1)) && symbol >= textualWndObject->unwrappedText.linesArray[midIndex]){
            textualWndObject->textPosition = midIndex;
            return;
        }
        else if(symbol > (textualWndObject->unwrappedText.linesArray[midIndex] + sizeof(char) * (lineSize - 1))){
           min = midIndex + 1;
        }
        else if(symbol < textualWndObject->unwrappedText.linesArray[midIndex]){
           max = midIndex - 1;
        }
    }
}

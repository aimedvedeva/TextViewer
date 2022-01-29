#include "vertTextScrollProxy.h"


void initVertTextScrollRange(HWND hwnd, TextualWndObject *textualWndObject, ScrollInfo *scroll, int clientWindowWidth){
    int curLinesNumber;
    if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
        curLinesNumber = textualWndObject->unwrappedText.linesArraySize;
    }
    else if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_WRAP){
        int lineSize = clientWindowWidth / textualWndObject->font.letterWidth;
        curLinesNumber = textualWndObject->wrappedText.text->textSize / lineSize + 1;
    }


    if(curLinesNumber <= MAX_INT_VALUE){
       SetScrollRange(hwnd, SB_VERT, 0, curLinesNumber, TRUE);
    }
    else{
     SetScrollRange(hwnd, SB_VERT, 0, MAX_INT_VALUE, TRUE);
    }
}

void setVertTextScrollShift(TextualWndObject *textualWndObject, ScrollInfo *scroll, int clientWindowWidth){
    int curLinesNumber;
    if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
        curLinesNumber = textualWndObject->unwrappedText.linesArraySize;
    }
    else if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_WRAP){
         int lineSize = clientWindowWidth / textualWndObject->font.letterWidth;
        curLinesNumber = textualWndObject->wrappedText.text->textSize / lineSize + 1;
    }

    if(curLinesNumber <= MAX_INT_VALUE){
       scroll->scrollShiftProportion = 1;
    }
    else{
       scroll->scrollShiftProportion = curLinesNumber / MAX_INT_VALUE + 1;
    }
}

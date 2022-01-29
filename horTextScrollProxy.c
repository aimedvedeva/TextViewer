#include "horTextScrollProxy.h"
#define MAX_INT_VALUE 32767

void initHorTextScrollRange(HWND hwnd, TextualWndObject *textualWndObject, ScrollInfo *scroll){
    int curLettersNumber;
    if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
        if(textualWndObject->unwrappedText.text != NULL){
            curLettersNumber = textualWndObject->unwrappedText.text->maxLineSize;
        }

    }
    else{
        return;
    }

    if(curLettersNumber <= MAX_INT_VALUE){
       SetScrollRange(hwnd, SB_HORZ, 0, curLettersNumber, TRUE);
    }
    else{
       SetScrollRange(hwnd, SB_HORZ, 0, MAX_INT_VALUE, TRUE);
    }
}

void setHorTextScrollShift(TextualWndObject *textualWndObject, ScrollInfo *scroll){
    int curLettersNumber;
    if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
            if(textualWndObject->unwrappedText.text != NULL){
                curLettersNumber = textualWndObject->unwrappedText.text->maxLineSize;
            }
    }
    else if(textualWndObject->textViewerMode == TEXT_VIEWER_MODES_WRAP){
        return;
    }

    if(curLettersNumber <= MAX_INT_VALUE){
       scroll->scrollShiftProportion = 1;
    }
    else{
       scroll->scrollShiftProportion = curLettersNumber / MAX_INT_VALUE;
    }
}

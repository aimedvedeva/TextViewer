#include <stdlib.h>
#include "text.h"

Text *createText(){
    Text *text = malloc(sizeof(Text));
    return text;
}

void destroyText(Text *text){
    if(text != NULL){
     free(text->textData);
    }
    free(text);
}

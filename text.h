#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

typedef struct{
    char *textData;   //pointer to original text data
    int textSize;
    int maxLineSize;  //the length of the longest line in the original text
}Text;


/**
  PARAM[IN]:
  PARAM[OUT]: the pointer to Text structure

  The function allocates memory for Text structure.
*/
Text *createText();

/**
  PARAM[IN]:  the pointer to Text structure
  PARAM[OUT]:

  The function cleans memory for Text structure.
*/
void destroyText(Text *text);

#endif // TEXT_H_INCLUDED

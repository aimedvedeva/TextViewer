#ifndef _FILE_PROXY_H
#define _FILE_PROXY_H

#include <stdio.h>
#include "booleanType.h"

/**
  PARAM[IN]:  initialized pointer to the structure of FILE
  PARAM[OUT]: number of characters in text file
*/
int getFileSize(FILE *file);


/**
  PARAM[IN]:  pointer to the structure of FILE
  PARAM[OUT]: myBool variable that is true if file can be opened and vice versa
*/
myBool isFileOpened(FILE *file);

/**
  PARAM[IN]:  pointer to the structure of FILE,
              size of file
  PARAM[OUT]: pointer to the text data
*/
void *getFileData(FILE *file, int fileSize);

/**
  PARAM[IN]:  pointer to the structure of FILE
  PARAM[OUT]: myBool variable that is true if file is empty and vice versa
*/
myBool isFileEmpty(FILE *file);

#endif /*_FILE_PROXY_H*/

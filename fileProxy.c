#include <stdlib.h>
#include "fileProxy.h"

int getFileSize(FILE *file){
    fseek(file, 0, SEEK_END);
    int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    return fileSize;
}

myBool isFileOpened(FILE *file){
    if(file == NULL){
            return myBool_false;
    }
    return myBool_true;
}

void *getFileData(FILE *file, int fileSize){
    char* fileData;

    if((fileData = calloc(fileSize + 1, sizeof(char))) == NULL){
        return NULL;
    }

    fread(fileData, sizeof(char), fileSize, file);
    fileData[fileSize] = '\0';
    return fileData;
}

myBool isFileEmpty(FILE *file){
    int fileSize = getFileSize(file);
    if(fileSize > 0){
        return myBool_false;
    }
    return myBool_true;
}


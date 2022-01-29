#include "myMenuOpen.h"

void initOpenedFileName(OPENFILENAMEA *openFileName, HWND hwnd, char *fileName){
    openFileName->lStructSize = (DWORD)sizeof(*openFileName);
    openFileName->hwndOwner = hwnd;
    openFileName->lpstrTitle = NULL;
    openFileName->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    openFileName->lpstrFilter =
        "Text Files(*.txt)\0*.txt\0" \
        "All Files(*.*)\0*.*\0";
    openFileName->nFilterIndex = 1;
    openFileName->lpstrFile = fileName;
    openFileName->nMaxFile = sizeof(fileName);
}

#ifndef HORTEXTSCROLLPROXY_H_INCLUDED
#define HORTEXTSCROLLPROXY_H_INCLUDED

#include <windows.h>
#include "scroll.h"
#include "textualWndObject.h"

/**
  PARAM[IN]:  ID of the application window,
              pointer to the TextualWndObject variable,
              pointer to the ScrollInfo variable
  PARAM[OUT]: void

  The function sets scroll range relating to the current application mode that TextualWndObject variable contains.
*/
void initHorTextScrollRange(HWND hwnd, TextualWndObject *textualWndObject, ScrollInfo *scroll);

/**
  PARAM[IN]:  pointer to the TextualWndObject variable,
              pointer to the ScrollInfo variable
  PARAM[OUT]: void

  The function sets scroll proportion relating to the current application mode that TextualWndObject variable contains.
*/
void setHorTextScrollShift(TextualWndObject *textualWndObject, ScrollInfo *scroll);

#endif // HORTEXTSCROLLPROXY_H_INCLUDED

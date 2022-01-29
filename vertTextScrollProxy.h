#ifndef SCROLLPROXY_H
#define SCROLLPROXY_H

#include <windows.h>
#include "scroll.h"
#include "textualWndObject.h"

#define MAX_INT_VALUE 32767

void initVertTextScrollRange(HWND hwnd, TextualWndObject *textualWndObject, ScrollInfo *scroll, int clientWindowWidth);
void setVertTextScrollShift(TextualWndObject *textualWndObject, ScrollInfo *scroll, int clientWindowWidth);

#endif // SCROLLPROXY_H

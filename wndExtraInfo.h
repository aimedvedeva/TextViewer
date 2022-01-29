#ifndef WNDEXTRAINFO_H_INCLUDED
#define WNDEXTRAINFO_H_INCLUDED

#include <windows.h>
#include "textualWndObject.h"

typedef struct {
   HDC hdc;
   TextualWndObject textualWndObject;
}WndExtraInfo;



#endif // WNDEXTRAINFO_H_INCLUDED

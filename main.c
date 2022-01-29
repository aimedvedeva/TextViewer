#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <stdio.h>
#include "myMenu.h"
#include "wrappedText.h"
#include "textualWndObject.h"
#include "vertTextScrollProxy.h"
#include "horTextScrollProxy.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS | CS_CLASSDC;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "MyMenu";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Text viewer"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument         /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static HDC hdc;
   static TextualWndObject textualObject = {0};
   static ScrollInfo vertScroll = {0};
   static ScrollInfo horScroll = {0};
   PAINTSTRUCT paintStruct;


   switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
        {
            char *fileName = (char*)(((LPCREATESTRUCTA)lParam)->lpCreateParams);

            //font initialization
            {
                hdc = GetDC(hwnd);

                textualObject.font.letterHeight = 20;
                textualObject.font.letterWidth = 7;
                HFONT hFont = GetFixedFont(textualObject.font);
                SelectObject(hdc, hFont);

                ReleaseDC(hwnd, hdc);
            }

            //textualObject initialization
            initializeTextualWndObject(&textualObject, fileName, hwnd);
            textualObject.textViewerMode = TEXT_VIEWER_MODES_UNWRAP;

            //vertical scroll initialization
            RECT clientWindowRect;
            GetClientRect(hwnd, &clientWindowRect);
            int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;

            initVertTextScrollRange(hwnd, &textualObject, &vertScroll, clientWondowWidth);
            setVertTextScrollShift(&textualObject, &vertScroll, clientWondowWidth);

            //horizontal scroll initialization
            initHorTextScrollRange(hwnd, &textualObject, &horScroll);
            setHorTextScrollShift(&textualObject, &horScroll);
        }
        break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &paintStruct);

            displayTextualWndObject(&textualObject, hdc);

            EndPaint(hwnd, &paintStruct);
            break;
        case WM_SIZE:
            {
                RECT clientWindowRect;
                GetClientRect(hwnd, &clientWindowRect);
                int clientWindowWidth = clientWindowRect.right - clientWindowRect.left;
                int clientWindowHeight = clientWindowRect.bottom - clientWindowRect.top;

                if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
                  textualObject.displayedLinesNumber = getDisplayedLinesNumber(&textualObject, clientWindowHeight);

                  InvalidateRect(hwnd, &clientWindowRect, TRUE);

                  wrapText(&textualObject.wrappedText, textualObject.textPos, textualObject.displayedLinesNumber, clientWindowWidth, textualObject.font);

                  initVertTextScrollRange(hwnd, &textualObject, &vertScroll, clientWindowWidth);
                  setVertTextScrollShift(&textualObject, &vertScroll, clientWindowWidth);
                }
                else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                    InvalidateRect(hwnd, &clientWindowRect, TRUE);
                    textualObject.displayedLinesNumber = getDisplayedLinesNumber(&textualObject, clientWindowHeight);
                }
            }
            break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_EXIT:
                    destroyTextualWndObject(&textualObject);
                    DestroyWindow(hwnd);
                    break;

                case IDM_OPEN:
                {
                    char newFileName[MAX_PATH] = {0};
                    OPENFILENAMEA openFileName = {0};
                    openFileName.lStructSize = sizeof(OPENFILENAMEA);
                    openFileName.hwndOwner = hwnd;
                    openFileName.lpstrTitle = NULL;
                    openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    openFileName.lpstrFilter =
                        "Text Files(*.txt)\0*.txt\0" \
                        "All Files(*.*)\0*.*\0";
                    openFileName.nFilterIndex = 1;
                    openFileName.lpstrFile = newFileName;
                    openFileName.nMaxFile = MAX_PATH;

                    if(GetOpenFileNameA(&openFileName)){
                        FILE *file;
                        file = fopen(newFileName, "r");

                        if(isFileOpened(file)){
                          destroyTextualWndObject(&textualObject);
                          initializeTextualWndObject(&textualObject, newFileName, hwnd);
                        }
                        if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
                          RECT clientWindowRect;
                          GetClientRect(hwnd, &clientWindowRect);
                          int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;

                          textualObject.textPos = textualObject.wrappedText.text->textData;
                          wrapText(&textualObject.wrappedText, textualObject.textPos, textualObject.displayedLinesNumber, clientWondowWidth, textualObject.font);
                        }
                        RECT clientWindowRect;
                        GetClientRect(hwnd, &clientWindowRect);
                        InvalidateRect(hwnd, &clientWindowRect, TRUE);

                        //vertical scroll initialization
                        int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;

                        initVertTextScrollRange(hwnd, &textualObject, &vertScroll, clientWondowWidth);
                        setVertTextScrollShift(&textualObject, &vertScroll, clientWondowWidth);

                        //horizontal scroll initialization
                        initHorTextScrollRange(hwnd, &textualObject, &horScroll);
                        setHorTextScrollShift(&textualObject, &horScroll);
                    }
                }
                    break;
                case IDM_WRAP:
                    {
                        HMENU hMenu = GetMenu(hwnd);
                        if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                            textualObject.textViewerMode = TEXT_VIEWER_MODES_WRAP;

                            RECT clientWindowRect;
                            GetClientRect(hwnd, &clientWindowRect);
                            int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;
                            int clientWondowHeight = clientWindowRect.bottom - clientWindowRect.top;

                            textualObject.displayedLinesNumber = getDisplayedLinesNumber(&textualObject, clientWondowHeight);
                            textualObject.textPos = textualObject.unwrappedText.linesArray[textualObject.textPosition];

                            wrapText(&textualObject.wrappedText, textualObject.textPos, textualObject.displayedLinesNumber, clientWondowWidth, textualObject.font);

                            InvalidateRect(hwnd, &clientWindowRect, TRUE);
                            CheckMenuItem(hMenu,IDM_WRAP, MF_CHECKED);

                            EnableScrollBar(hwnd, SB_HORZ, ESB_DISABLE_BOTH);
                        }
                        else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
                            textualObject.textViewerMode = TEXT_VIEWER_MODES_UNWRAP;

                            RECT clientWindowRect;
                            GetClientRect(hwnd, &clientWindowRect);
                            InvalidateRect(hwnd, &clientWindowRect, TRUE);

                            initTextPosition(&textualObject);

                            CheckMenuItem(hMenu,IDM_WRAP, MF_UNCHECKED);

                            EnableScrollBar(hwnd, SB_HORZ, ESB_ENABLE_BOTH);
                        }
                        RECT clientWindowRect;
                        GetClientRect(hwnd, &clientWindowRect);
                        int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;

                        //vertical scroll initialization
                        initVertTextScrollRange(hwnd, &textualObject, &vertScroll, clientWondowWidth);
                        setVertTextScrollShift(&textualObject, &vertScroll, clientWondowWidth);

                        if(textualObject.textViewerMode != TEXT_VIEWER_MODES_WRAP){
                             //horizontal scroll initialization
                            initHorTextScrollRange(hwnd, &textualObject, &horScroll);
                            setHorTextScrollShift(&textualObject, &horScroll);
                        }
                    }
                    break;
            }
            break;
        case WM_VSCROLL:
            {
                 int scrollPosDelta = 0;
                 switch(LOWORD(wParam))
                 {
                    case SB_LINEUP:
                        vertScroll.scrollPosition -= 1;
                        scrollPosDelta = -1;
                        break;
                    case SB_LINEDOWN:
                        vertScroll.scrollPosition += 1;
                        scrollPosDelta = 1;
                        break;
                    case SB_THUMBPOSITION:
                        scrollPosDelta = vertScroll.scrollPosition;
                        vertScroll.scrollPosition = HIWORD(wParam);
                        scrollPosDelta = vertScroll.scrollPosition - scrollPosDelta;
                        break;
                    default:
                        break;
                }

                if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                    if(textualObject.unwrappedText.linesArraySize <= MAX_INT_VALUE){
                        vertScroll.scrollPosition = max(0, min(vertScroll.scrollPosition, textualObject.unwrappedText.linesArraySize));
                    }
                    else{
                        vertScroll.scrollPosition = max(0, min(vertScroll.scrollPosition, MAX_INT_VALUE + 1));
                    }
                }
                else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
                    RECT clientWindowRect;
                    GetClientRect(hwnd, &clientWindowRect);
                    int clientWondowWidth = clientWindowRect.right - clientWindowRect.left;

                    int lineSize = clientWondowWidth / textualObject.font.letterWidth;
                    int linesNumber = textualObject.wrappedText.text->textSize / lineSize + 1;


                    if(linesNumber <= MAX_INT_VALUE){
                        vertScroll.scrollPosition = max(0, min(vertScroll.scrollPosition, linesNumber));
                    }
                    else{
                        vertScroll.scrollPosition = max(0, min(vertScroll.scrollPosition, MAX_INT_VALUE + 1));
                    }
                }



                if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                    textualObject.textPosition += vertScroll.scrollShiftProportion * scrollPosDelta;
                    textualObject.textPosition = max(0, min(textualObject.textPosition, textualObject.unwrappedText.linesArraySize));
                }

                else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
                    RECT clientWindowRect;
                    GetClientRect(hwnd, &clientWindowRect);
                    int clientWindowWindth = clientWindowRect.right - clientWindowRect.left;

                    textualObject.textPos +=  (vertScroll.scrollShiftProportion * scrollPosDelta) * (clientWindowWindth / textualObject.font.letterWidth);

                    char *lastPointerTextBoard = textualObject.wrappedText.text->textData + textualObject.wrappedText.text->textSize * sizeof(char);
                    textualObject.textPos = max(textualObject.wrappedText.text->textData, min(textualObject.textPos, lastPointerTextBoard));

                    wrapText(&textualObject.wrappedText, textualObject.textPos, textualObject.displayedLinesNumber, clientWindowWindth, textualObject.font);
                }

                if (vertScroll.scrollPosition != GetScrollPos(hwnd, SB_VERT))
                {
                    SetScrollPos(hwnd, SB_VERT, vertScroll.scrollPosition, TRUE);
                    RECT clientWindowRect;
                    GetClientRect(hwnd, &clientWindowRect);
                    InvalidateRect(hwnd, &clientWindowRect, TRUE);
                }
                return 0;
            }
            break;
        case WM_KEYDOWN:
            switch (wParam)
            {
            case VK_ESCAPE:
                destroyTextualWndObject(&textualObject);
                DestroyWindow(hwnd);
                break;
            case VK_UP:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
                break;
            case VK_DOWN:
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
                break;
            case VK_LEFT:
                if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                   SendMessage(hwnd, WM_HSCROLL, SB_LINELEFT, 0);
                }
                break;
            case VK_RIGHT:
                if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
                   SendMessage(hwnd, WM_HSCROLL, SB_LINERIGHT, 0);
                }
                break;
            }
            break;
        case WM_HSCROLL:
        {
            int scrollPosDelta = 0;
            switch(LOWORD(wParam))
            {
               case SB_LINERIGHT:
                   horScroll.scrollPosition += 1;
                   scrollPosDelta = 1;
                   break;
               case SB_LINELEFT:
                   horScroll.scrollPosition -= 1;
                   scrollPosDelta = -1;
                   break;
               case SB_THUMBPOSITION:
                   scrollPosDelta = horScroll.scrollPosition;
                   horScroll.scrollPosition = HIWORD(wParam);
                   scrollPosDelta = horScroll.scrollPosition - scrollPosDelta;
                    break;
               default:
                   break;
           }
           if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
               horScroll.scrollPosition = max(0, min(horScroll.scrollPosition, textualObject.unwrappedText.text->maxLineSize));
           }
           else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
               int lineSize = textualObject.wrappedText.text->textSize / (textualObject.wrappedText.linesArraySize - 1);
               horScroll.scrollPosition = max(0, min(horScroll.scrollPosition, lineSize));
           }


           textualObject.textOffset += horScroll.scrollShiftProportion * scrollPosDelta;
           if(textualObject.textViewerMode == TEXT_VIEWER_MODES_UNWRAP){
               textualObject.textOffset = max(0, min(textualObject.textOffset, textualObject.unwrappedText.text->maxLineSize));
           }
           else if(textualObject.textViewerMode == TEXT_VIEWER_MODES_WRAP){
               int lineSize = textualObject.wrappedText.text->textSize / (textualObject.wrappedText.linesArraySize - 1);
               textualObject.textOffset = max(0, min(textualObject.textOffset, lineSize));
           }

            if (horScroll.scrollPosition != GetScrollPos(hwnd, SB_HORZ))
           {
               SetScrollPos(hwnd, SB_HORZ, horScroll.scrollPosition, TRUE);
               RECT clientWindowRect;
               GetClientRect(hwnd, &clientWindowRect);
               InvalidateRect(hwnd, &clientWindowRect, TRUE);
           }
           return 0;
        }
            break;
        case WM_CLOSE:
            destroyTextualWndObject(&textualObject);
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

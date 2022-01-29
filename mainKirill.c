#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <Windows.h>
#include <limits.h>
#include <commdlg.h>
#include "fileProxy.h"
#include "text.h"
#include "menu1.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
HFONT GetFixedFont(LONG fontHeight, LONG fontWidth);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");



int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    //HMENU hMenu,hMenu1,hMenu2;
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
    wincl.lpszMenuName = ("MyMenu");                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Text editor"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           500,                 /* The programs width */
           500,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /*hMenu = CreateMenu();
    hMenu1 = CreateMenu();
    AppendMenu(hMenu, MF_STRING|MF_POPUP,(UINT)hMenu1,"Menu");
        AppendMenu(hMenu1, MF_STRING,1,"File Open");
        AppendMenu(hMenu1, MF_STRING,2,"File Close");
    SetMenu(hwnd,hMenu);
    SetMenu(hwnd,hMenu1);
    SetMenu(hwnd,hMenu2);*/
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
    HMENU hMenu;

    static int iSelection = IDM_UNFORMAT;
    HDC hdc;
    PAINTSTRUCT paintStruct;
    static Text text;
    static RECT rect;
    static Scroll scroll;
    static Pointer pointer;
    static int i, cxChar, cxCaps, cyChar, cyClient, iVscrollPos, iHscrollPos, iHscrollInc ;

    switch (message)                  /* handle the messages */
    {

            case WM_CLOSE :
            DestroyWindow(hwnd);
            return 0;
        case WM_COMMAND :
        {


            hMenu = GetMenu(hwnd);
            switch(LOWORD(wParam))
            {
                /*case IDM_OPEN :
                {

                    if(PopFileOpenDlg(hwnd, szFileName, szTitleName))
                    {
                        if(!PopFileRead(hwndEdit, szFileName))
                        {
                            OkMessage(hwnd, "Could not read file %s!",
                            szTitleName);
                            szFileName[0] = '\0';
                            szTitleName[0] = '\0';
                        }
                    }
                    return 0;

                }*/
                case IDM_UNFORMAT:
                case IDM_FORMAT:

                CheckMenuItem(hMenu,iSelection,MF_UNCHECKED);
                iSelection = LOWORD(wParam);
                CheckMenuItem(hMenu,iSelection,MF_CHECKED);
                return 0;

                case IDM_EXIT :
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    return 0;
            }

        return 0;
        }



        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE:
        {
            hdc = GetDC(hwnd);          // РИТА, ТЕБЕ НУЖНО ЭТО!!!
            text.info.font.height = 20; // поля структуры, которые содержат ширину и высоту
            text.info.font.width = 7;
            HFONT hFont = GetFixedFont(text.info.font.height, text.info.font.width);
            SelectObject(hdc, hFont);

            ReleaseDC(hwnd, hdc);



            char *fileName = (char*)(((LPCREATESTRUCTA)lParam)->lpCreateParams);
            FILE *file;

            file = fopen(fileName, "r");
            if(isFileOpened(file)){
                text.size = getFileSize(file);

            if(text.size > 0)
                text.data = getFileData(file, text.size);
            else{
                errno = ERROR_FILE_INVALID;
                perror("An error has occurred: ");
                return 0;
            }


            if(GetClientRect(hwnd, &rect))
            {
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;

                text.info.paintInfo.observedLinesNumber = getObservedLinesNumber(height, text);
                formatText(text, width);
                //formatTextH(text);
                printf("dlina! %d %d\n", text.maxline, text.numline);

                //if (getLinesNumber(text.size,height,text) <= 32767)
                    //SetScrollRange(hwnd, SB_VERT, 0, getLinesNumber(text.size,height,text), FALSE);
                    SetScrollRange(hwnd, SB_VERT, 0, 1173, FALSE);

                //if (&text.maxline <= 32767){
                    printf("maxline %d",&text.maxline);
                    SetScrollRange(hwnd, SB_HORZ, 0, 812, FALSE);
                //}
                /*else{
                    SetScrollRange(hwnd, SB_VERT, 0, 32767, FALSE);
                    SetScrollRange(hwnd, SB_HORZ, 0, 32767, FALSE);
                }*/
            }

            }
            else{
                errno = ERROR_FILE_NOT_FOUND;
                perror("An error has occurred: ");
                return 0;
            }
        }
        break;

        case WM_VSCROLL :
        {
            RECT rect;
            if(GetClientRect(hwnd, &rect))
            {
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;

                switch(LOWORD(wParam))
                {
                    case SB_LINEUP :
                        iVscrollPos -= 1;
                        break;
                    case SB_LINEDOWN :
                        iVscrollPos += 1;
                        break;
                    case SB_THUMBPOSITION :
                        iVscrollPos = HIWORD(wParam);
                        break;
                    case SB_THUMBTRACK :
                        iVscrollPos = HIWORD(wParam);
                        break;
                    case SB_PAGEUP :
                        iVscrollPos -= height / (text.info.font.width);
                        break;
                    case SB_PAGEDOWN :
                        iVscrollPos += height / (text.info.font.width);
                        break;


                    default :
                    break;
                }

                //int maxRange = getLinesNumber(text.size,height,text);
                int maxRange1 = text.numline;
                //iVscrollPos = max(0, min(iVscrollPos, maxRange));
                iVscrollPos = max(0, min(iVscrollPos, 1170)); //ÈÑÏÐÀÂÈÒÜ
                scroll.vscrollpos = iVscrollPos;
                }


            //if (iVscrollPos != GetScrollPos(hwnd, SB_VERT))
            //{
                SetScrollPos(hwnd, SB_VERT, iVscrollPos, FALSE);
                InvalidateRect(hwnd, NULL, TRUE);
            //}DM_OPEN
        }
            break;

        case WM_HSCROLL :
        {
            switch(LOWORD(wParam))
            {
                case SB_LINEUP :
                    iHscrollInc -= 1;
                    break;
                case SB_LINEDOWN :
                    iHscrollInc += 1;
                    break;
                case SB_THUMBPOSITION :
                    iHscrollInc = HIWORD(wParam);
                    break;
                case SB_THUMBTRACK :
                    iHscrollInc = HIWORD(wParam);
                    break;
                default :
                    break;
            }
            iHscrollInc = max( 0, min(iHscrollInc, 812));
            scroll.hscrollpos = iHscrollInc;

            SetScrollPos(hwnd, SB_HORZ, iHscrollInc, FALSE);
            InvalidateRect(hwnd, NULL, TRUE);

        return 0;
        }




        case WM_PAINT:
            {
                hdc = BeginPaint(hwnd, &paintStruct);
                PaintText(hdc, text, scroll);
                EndPaint(hwnd, &paintStruct);
            }
            break;
        case WM_SIZE:
            {
                int currentWidth = LOWORD(lParam);
                int currentHeight = HIWORD(lParam);

                formatText(text, currentWidth);
                //formatTextH(text);

                InvalidateRect(hwnd,NULL,TRUE);

                //if (getLinesNumber(text.size,currentHeight,text) <= 32767)
                    //SetScrollRange(hwnd, SB_VERT, 0, getLinesNumber(text.size,currentHeight,text), FALSE);
                    SetScrollRange(hwnd, SB_VERT, 0, 1173, FALSE);
                //if (&text.maxline <= 32767){
                    //printf("maxline %d",&text.maxline);
                    SetScrollRange(hwnd, SB_HORZ, 0, 812, FALSE);
                //}
                /*else{
                    SetScrollRange(hwnd, SB_VERT, 0, 32767, FALSE);
                    SetScrollRange(hwnd, SB_HORZ, 0, 32767, FALSE);
                }*/

                SetScrollPos (hwnd, SB_VERT, iVscrollPos, TRUE);

                text.info.paintInfo.observedLinesNumber = getObservedLinesNumber(currentHeight, text);




            }
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


HFONT GetFixedFont(LONG fontHeight, LONG fontWidth)   // РИТА, ТЕБЕ НУЖНО ЭТО!!!
{
	static LOGFONT font;

	GetObject(GetStockObject(ANSI_FIXED_FONT), sizeof(LOGFONT), &font);

	font.lfHeight = fontHeight;
	font.lfWidth = fontWidth;

	return CreateFont(font.lfHeight, font.lfWidth,
		font.lfEscapement, font.lfOrientation, font.lfWeight,
		font.lfItalic, font.lfUnderline, font.lfStrikeOut,
		font.lfCharSet, font.lfOutPrecision, font.lfClipPrecision,
		font.lfQuality, font.lfPitchAndFamily, font.lfFaceName);
}

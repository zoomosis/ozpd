/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  simpss - the simplest windows screen saver                       */
/*                                                                   */
/*  You need to create a .def file with                              */
/*  description 'SCRNSAVE : Simple Screen Saver'                     */
/*                                                                   */
/*  in order to use this as a screen saver.  Left mouse exits.       */
/*                                                                   */
/*  Also you need to rename the executable to simpss.scr and         */
/*  put it in your windows directory.                                */
/*                                                                   */
/*********************************************************************/

#include <windows.h>

LONG FAR PASCAL simpss(HWND hwnd, WORD msg, WORD wparam, LONG lparam);

int PASCAL WinMain(HANDLE inst, 
                   HANDLE previnst, 
                   LPSTR cmd, 
                   int cmdshow)
{
    WNDCLASS wndClass = { 0, (WNDPROC)simpss, 0, 0, 0, NULL, NULL, 
                          (HBRUSH)(COLOR_WINDOW+1), "simpss", 
                          "simpss" };
    HWND hwnd;            
    MSG  msg;              
 
    (void)previnst;
    (void)cmd;       
    wndClass.hInstance = inst;
    RegisterClass(&wndClass);
    hwnd = CreateWindow("simpss", "Simple Screen Saver", 
                        WS_BORDER,
                        CW_USEDEFAULT, CW_USEDEFAULT, 
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        NULL, NULL, inst, NULL );                 
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (msg.wParam);
}

LONG FAR PASCAL simpss(HWND hwnd, WORD msg, WORD wparam, LONG lparam)
{
    int calldefault = 1;
    LONG ret = 0;
    
    switch (msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            RECT rect;

            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);
            DrawText(hdc, "How simple can you get?", -1, &rect, 
                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(hwnd, &ps); 
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_DESTROY:
            PostQuitMessage(0);
            calldefault = 0;
            break;
    }
    if (calldefault)
    {
        ret = DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return (ret);
}
            

/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  simpwin - the simplest windows program (roughly speaking)        */
/*                                                                   */
/*********************************************************************/

#include <windows.h>

LONG FAR PASCAL simpwin(HWND hwnd, WORD msg, WORD wparam, LONG lparam);

int PASCAL WinMain(HANDLE inst, 
                   HANDLE previnst, 
                   LPSTR cmd, 
                   int cmdshow)
{
    WNDCLASS wndClass = { 0, (WNDPROC)simpwin, 0, 0, 0, NULL, NULL, 
                          (HBRUSH)(COLOR_WINDOW+1), "simpwin", "simpwin" };
    HWND hwnd;            
    MSG  msg;              
 
    (void)previnst;
    (void)cmd;       
    wndClass.hInstance = inst;
    RegisterClass(&wndClass);
    hwnd = CreateWindow("simpwin", "Simple Windows", 
                        WS_SYSMENU,
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

LONG FAR PASCAL simpwin(HWND hwnd, WORD msg, WORD wparam, LONG lparam)
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
            DrawText(hdc, "Hello There", -1, &rect, 
                     DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint(hwnd, &ps); 
            break;
        }
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
            

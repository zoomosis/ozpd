/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Modified instructions by Andrew Clarke.                          */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  simppm - the simplest PM program (roughly speaking)              */
/*                                                                   */
/*  To compile this program, make sure you have a "simppm.def" file  */
/*  with "name simppm windowapi" in it, otherwise the linker won't   */
/*  generate a Presentation Manager executable.  WATCOM C does not   */
/*  use this .def file, instead compile simppm using                 */
/*  "wcl386 -l=os2v2_pm simppm.c" to tell the WATCOM linker to       */
/*  generate a PM executable.                                        */
/*                                                                   */
/*********************************************************************/

#define INCL_WIN
#include <os2.h>

MRESULT EXPENTRY simppm(HWND hwnd, ULONG msgType, MPARAM mp1, MPARAM mp2);

int main(void)
{
    HAB hab;
    HMQ hmq;
    QMSG qmsg;
    HWND hwndClient;
    ULONG createFlags = FCF_SYSMENU | FCF_TITLEBAR | FCF_TASKLIST
                            | FCF_SIZEBORDER | FCF_SHELLPOSITION;
    
    hab = WinInitialize((ULONG)0);
    hmq = WinCreateMsgQueue(hab, (LONG)0);
    WinRegisterClass(hab, "simppm", simppm, (ULONG)0, (USHORT)0);
    WinCreateStdWindow(HWND_DESKTOP,
                       WS_VISIBLE,
                       &createFlags,
                       (PSZ)"simppm",
                       (PSZ)"simple program",
                       (ULONG)0,
                       NULLHANDLE,
                       (ULONG)0,
                       &hwndClient);
    while (WinGetMsg(hab, &qmsg, NULLHANDLE, (ULONG)0, (ULONG)0) == TRUE)
    {
        WinDispatchMsg(hab, &qmsg);
    }
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    return (0);
}

MRESULT EXPENTRY simppm(HWND hwnd, ULONG msgType, MPARAM mp1, MPARAM mp2)
{
    int callDefault = 0;
    MRESULT ret = (MRESULT)0;
    RECTL rectl;

    switch (msgType)
    {
        case WM_PAINT:
            WinBeginPaint(hwnd, NULLHANDLE, NULL);
            WinQueryWindowRect(hwnd, &rectl);
            WinDrawText(WinGetPS(hwnd),
                        (LONG)-1, 
                        "Hello There", 
                        &rectl, 
                        (LONG)0, 
                        (LONG)0, 
                        DT_CENTER | DT_VCENTER
                            | DT_ERASERECT | DT_TEXTATTRS); 
            WinEndPaint(hwnd);
            break;
        default:
            callDefault = 1;
            break;
    }
    if (callDefault)
    {
        ret = WinDefWindowProc(hwnd, msgType, mp1, mp2);
    }
    return (ret);
}

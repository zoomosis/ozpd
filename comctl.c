/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  comctl - A demonstration of a program that accesses the COM      */
/*  port using OS/2.                                                 */
/*                                                                   */
/*  This program was tested using CSET++/2 and Borland C++ 1.0.      */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>

#define INCL_DOS
#define INCL_BSE
#define INCL_DOSDEVIOCTL
#define INCL_EXE
#include <os2.h>

int main(void)
{
    APIRET rc;
    HFILE  hfile;
    ULONG  action;
    ULONG  newsize = 0;
    ULONG  fileAttr = 0;
    ULONG  openAction = OPEN_ACTION_OPEN_IF_EXISTS;
    ULONG  openMode = OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE;
    char   fileName[FILENAME_MAX];
    ULONG  actualWritten;
    unsigned long baudRate;
    
    strcpy(fileName, "COM3");
    rc = DosOpen((unsigned char *)fileName,
                 &hfile, 
                 &action, 
                 newsize, 
                 fileAttr,
                 openAction, 
                 openMode, 
                 NULL);
    printf("rc from DosOpen is %lu\n", (unsigned long)rc);
    if (rc == 0)
    {
        baudRate = 19200L;
        rc = DosDevIOCtl(hfile, 
                         IOCTL_ASYNC,
                         0x43, /* set extended bit rate */
                         &baudRate,
                         sizeof baudRate,
                         NULL,
                         NULL,
                         0,
                         NULL);
        if (rc != 0)
        {
            printf("rc from DosDevIoCtl is %lu\n", (unsigned long)rc);
        }
        else
        {
            rc = DosWrite(hfile, "ATDT4361785\r\n", 13, &actualWritten);
            if (rc != 0)
            {
                printf("rc from DosWrite is %lu\n", (unsigned long)rc);
            }
            printf("actual written is %lu\n", actualWritten);
            DosSleep(300);
        }
        rc = DosClose(hfile);
        printf("rc from DosClose is %lu\n", (unsigned long)rc);
    }
    return (0);
}                 

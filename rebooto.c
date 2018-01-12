/* Written by Peter Fitzsimmons and released to the public domain */

#include <stdio.h>

#define INCL_DOS
#include <os2.h>

int main(void)
{
    HFILE hf;
    APIRET rc;
    ULONG dummy;

    rc = DosOpen("DOS$", &hf, &dummy, 0L, FILE_NORMAL, FILE_OPEN,
                 OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE |
                 OPEN_FLAGS_FAIL_ON_ERROR, 0L);
    if (!rc)
    {
        DosShutdown(0L);        /* can't use stdin/stdout after this! */
        /* 16 bit: DosDevIOCtl(NULL, NULL, 0xab, 0xd5, hf); *//* reboot */
        rc = DosDevIOCtl(hf, 0xd5, 0xab,
                         NULL, 0, NULL,
                         NULL, 0, NULL);
        /* we shouldn't actually make it this far */
        printf("ioctl returned %d\n", rc);
        DosClose(hf);
    }
    else
        printf("DOS.SYS not installed (sys%04u)\n", rc);

    return (0);
}

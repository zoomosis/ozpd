/* written by Peter Fitzsimmons and released to the public domain */

#define INCL_NOPM
#define INCL_DOS
#include <os2.h>
#include <process.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("FH - Increase File Handles for program.\n");
        printf("USAGE  : FH [/b | /f] <program name and args>\n");
        printf("Options:  /b run at background priority\n");
        printf("          /f run at foreground priority\n");
    }
    if (argc > 2 && !stricmp(argv[1], "/b"))
    {
        DosSetPrty(PRTYS_PROCESSTREE, PRTYC_IDLETIME, 1, 0);
        argv++;
    }
    else if (argc > 2 && !stricmp(argv[1], "/f"))
    {
        DosSetPrty(PRTYS_PROCESSTREE, PRTYC_FOREGROUNDSERVER, 1, 0);
        argv++;
    }
    DosSetMaxFH(50);
    return (spawnvp(P_WAIT, argv[1], &argv[1]));
}

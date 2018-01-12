/* Test maximum number of files that can be opened */
/* using DosOpen under OS/2 */
/* Written by Paul Edwards and released to the public domain */

#include <stdio.h>

#include <os2.h>

int main(void)
{
    int x;
    char fnm[FILENAME_MAX];
    int ret;
    HFILE hfile;
    FILE *fq;
#ifdef OS216
    USHORT action;
#else        
    ULONG action;
#endif    
    ULONG newsize = 0;
#ifdef OS216    
    USHORT fileAttr = 0;
#else        
    ULONG fileAttr = 0;
#endif    
#ifdef SETMAX
    ULONG MaxFH;
    LONG ReqFH;
#endif
    
    printf("ISO guarantees at least %d\n", FOPEN_MAX - 3);    
#ifdef SETMAX
#ifdef OS216
    DosSetMaxFH(60);
#else    
    ReqFH = 0;
    DosSetRelMaxFH(&ReqFH, &MaxFH);
    printf("MaxFH is %d\n", (int)MaxFH);
    ReqFH = 50;
    DosSetRelMaxFH(&ReqFH, &MaxFH);
    printf("MaxFH is %d\n", (int)MaxFH);
#endif    
#endif    
    for (x = 1; x <= 200; x++)
    {
        sprintf(fnm, "maxxx.%d", x);
#ifdef USE_FOPEN
        fq = fopen(fnm, "wb");
        ret = (fq == NULL);
#else                
        ret = DosOpen(fnm, 
                      &hfile,
                      &action,
                      newsize,
                      fileAttr,
                      OPEN_ACTION_CREATE_IF_NEW
                      | OPEN_ACTION_REPLACE_IF_EXISTS,
                      OPEN_SHARE_DENYWRITE
                      | OPEN_ACCESS_WRITEONLY,
                      0);
#endif
        if (ret != 0)
        {
            printf("max is %d\n", x - 1);
            printf("ret is %d\n", ret);
            return (0);
        }
    }
    printf("max is at least %d\n", x - 1);
    return (0);
}

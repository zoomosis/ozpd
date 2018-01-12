/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  zfill - fill up disk quickly!                                    */
/*                                                                   */
/*********************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef SIZEBUF
#define SIZEBUF 6144
#endif
static char buf[SIZEBUF];

int main(int argc, char **argv)
{
    FILE *fq;
    unsigned long max = 0;
    int infinite = 0;
    size_t cnt;

    if (argc < 2)
    {
        printf("enter filename and number of bytes to generate\n");
        printf("leave number of bytes blank or zero for infinity\n");
        return (EXIT_FAILURE);
    }
    assert((fq = fopen(*(argv+1),"wb")) != NULL);
    setvbuf(fq, NULL, _IOFBF, SIZEBUF);
    if (argc > 2) max = strtoul(*(argv+2), NULL, 0);
    if (max == 0) infinite = 1;
    
    cnt = SIZEBUF;
    memset(buf, '\xff', cnt);
    while ((max > 0) || infinite)
    {
        if ((max < SIZEBUF) && !infinite)
        {
            cnt = max;
        }
        fwrite(buf, cnt, 1, fq);
        max -= cnt;
    }
    fclose(fq);
    return (EXIT_SUCCESS);
}

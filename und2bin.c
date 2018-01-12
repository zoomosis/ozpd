/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  und2bin - convert an "undefined" MVS file into a general         */
/*  purpose output binary file (normally FB).                        */
/*                                                                   */
/*  This is for use on mainframe environments where transfer of      */
/*  record-oriented files to PCs is hazardous.                       */
/*                                                                   */
/*  The binary file consists of a series of length/data pairs, with  */
/*  the length being a short, with -1 representing EOF.              */
/*                                                                   */
/*  Note that you nearly always need to specify the DCB info on the  */
/*  output dataset.                                                  */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

static char buf[33000];
static short len;
static FILE *fp;
static FILE *fq;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage: und2bin <host file> <binary file>\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv + 1), "rb,type=record");
    if (fp == NULL)
    {
        printf("failed to open %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }
    fq = fopen(*(argv + 2), "wb");
    if (fq == NULL)
    {
        printf("failed to open %s\n", *(argv + 2));
        fclose(fp);
        return (EXIT_FAILURE);
    }
    for (;;)
    {
        len = fread(buf, 1, sizeof buf, fp);
        if (feof(fp)) break;
        if (ferror(fp))
        {
            printf("read error\n");
            return (EXIT_FAILURE);
        }
        fwrite(&len, 1, sizeof len, fq);
        fwrite(buf, 1, len, fq);
        if (ferror(fq))
        {
            printf("write error\n");
            return (EXIT_FAILURE);
        }
    }
    len = -1;
    fwrite(&len, 1, sizeof len, fq);
    if (ferror(fq))
    {
        printf("write error\n");
        return (EXIT_FAILURE);
    }
    fclose(fp);
    fclose(fq);
    return (EXIT_SUCCESS);
}

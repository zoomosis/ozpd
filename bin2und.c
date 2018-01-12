/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  bin2und - convert a binary file containing encoded records into  */
/*  a record-oriented file.                                          */
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
static size_t ret;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage: bin2und <binary file> <host file>\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv + 1), "rb");
    if (fp == NULL)
    {
        printf("failed to open %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }
    fq = fopen(*(argv + 2), "wb,type=record");
    if (fq == NULL)
    {
        printf("failed to open %s\n", *(argv + 2));
        fclose(fp);
        return (EXIT_FAILURE);
    }
    for (;;)
    {
        ret = fread(&len, sizeof len, 1, fp);
        if ((ret == 1) && (len != -1))
        {
            ret = fread(buf, len, 1, fp);
        }
        if (ret != 1)
        {
            printf("read error\n");
            return (EXIT_FAILURE);
        }
        if (len == -1)
        {
            break;
        }
        ret = fwrite(buf, len, 1, fq);
        if (ret != 1)
        {
            printf("write error\n");
            return (EXIT_FAILURE);
        }
    }
    fclose(fp);
    fclose(fq);
    return (EXIT_SUCCESS);
}

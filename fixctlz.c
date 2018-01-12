/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  fixctlz - delete ctrl-z from a file                              */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int c;
    FILE *fp;
    FILE *fq;
    
    if (argc < 3)
    {
        printf("usage: fixctlz <input file> <output file>\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv+1), "rb");
    if (fp != NULL)
    {
        fq = fopen(*(argv+2), "wb");
        if (fq != NULL)
        {
            while ((c = fgetc(fp)) != EOF)
            {
                if (c != 0x1a)
                {
                    fputc(c, fq);
                }
            }
            fclose(fq);
        }
        fclose(fp);
    }
    return (0);
}

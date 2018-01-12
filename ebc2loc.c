/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program converts an ebcdic file into the local character    */
/*  set.                                                             */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

#include "febc.c"

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    int c;
    int x = 0;
    
    if (argc <= 2)
    {
        printf("usage: ebc2loc infile outfile\n");
        return (0);
    }
    
    fp = fopen(*(argv + 1), "rb");
    fq = fopen(*(argv + 2), "wb");
    
    while ((c = fgetc(fp)) != EOF)
    {
        x++;
        c = febc(c);
        fputc(c, fq);
        if (x == 1000)
        {
            fputc('\n', fq);
            x = 0;
        }
    }
    return (0);
}

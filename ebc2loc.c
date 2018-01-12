/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program converts an ebcdic file into the local character    */
/*  set. It will insert line breaks at the end of a record (default  */
/*  80) but does not trim spaces.                                    */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "febc.h"

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    int c;
    int x = 0;
    int lrecl = 80;
    long count = 0;
    
    if (argc <= 2)
    {
        printf("usage: ebc2loc infile outfile [record length]\n");
        return (EXIT_FAILURE);
    }
    
    fp = fopen(*(argv + 1), "rb");
    fq = fopen(*(argv + 2), "w");
    if (argc > 3)
    {
        lrecl = atoi(*(argv + 3));
    }
    
    while ((c = fgetc(fp)) != EOF)
    {
        int origc;
        
        x++;
        origc = c;
        c = febc(c);
        if (c == 0)
        {
            printf("converting unknown character x'%02X' at offset %ld"
                   " to space\n", origc, count);
            c = ' ';
        }
        count++;
        fputc(c, fq);
        if (x == lrecl)
        {
            fputc('\n', fq);
            x = 0;
        }
    }
    return (0);
}

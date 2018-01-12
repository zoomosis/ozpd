/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program converts a text file in the local character set to  */
/*  EBCDIC fixed length records, default 80 character wide, silently */
/*  truncating any long records and noisily changing any             */
/*  unrecognized characters into spaces.                             */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "tebc.h"

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    int c;
    int x = 0;
    int lrecl = 80;
    
    if (argc <= 2)
    {
        printf("usage: loc2ebc infile outfile [record length]\n");
        return (EXIT_FAILURE);
    }
    
    fp = fopen(*(argv + 1), "r");
    fq = fopen(*(argv + 2), "wb");
    
    if (argc > 3)
    {
        lrecl = atoi(*(argv + 3));
    }
    
    while ((c = fgetc(fp)) != EOF)
    {
        int origc;
        
        if (c == '\n')
        {
            for (; x < lrecl; x++)
            {
                fputc(tebc(' '), fq);
            }
            x = 0;
        }
        else
        {
            x++;
            origc = c;
            c = tebc(c);
            if (c == 0)
            {
                printf("converting unknown character x'%2X' to space\n",
                       origc);
                c = ' ';
            }
            fputc(c, fq);
        }
    }
    return (0);
}

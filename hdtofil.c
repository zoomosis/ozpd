/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, avon@matra.com.au.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  hdtofil - convert hexdump output to a file.                      */
/*  will happily skip anything you blank out.                        */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[100];
    char hex[3] = "  ";
    int ch;
    int x;
    int y;
    char *p;
    FILE *fq;
    
    if (argc < 2)
    {
        printf("usage: hdtofil <output filename>\n");
        printf("will read dump file from standard input\n");
        return (EXIT_FAILURE);
    }
    fq = fopen(*(argv + 1), "wb");
    if (fq == NULL)
    {
        printf("unable to open file %s for writing\n", *(argv + 1));
        return (EXIT_FAILURE);
    }
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        p = buf + 8;
        for (x = 0; x < 4; x++)
        {
            for (y = 0; y < 4; y++)
            {
                if (*p != ' ')
                {
                    memcpy(hex, p, 2);
                    ch = strtol(hex, NULL, 16);
                    putc(ch, fq);
                }
                p += 2;
            }
            p++;
        }
    }
    fclose(fq);
    return (EXIT_SUCCESS);
}

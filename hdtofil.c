/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards                             */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  hdtofil - convert hexdump output to a file.                      */
/*  will happily skip anything you blank out.                        */
/*                                                                   */
/*  It will also handle mvsendec and idcams hex dumps as well.       */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[150];
    char hex[3] = "  ";
    int ch;
    int x;
    int y;
    char *p;
    FILE *fq;
    size_t len;
    size_t pad;
    int mvsendec_format = 0;
    int idcams_format = 0;
    int hexdump_format = 0;
    int sets;
    int sizeset;
    
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
        len = strlen(buf) - 1;
        pad = sizeof buf - len - 1;
        memset(buf + len, ' ', pad);
        buf[sizeof buf - 1] = '\0';

        /* If the file is in mvsendec format, there is no offset to skip */
        p = buf;
        if (memcmp(buf + 6, "  ", 2) == 0)
        {
            p += 8;
            if (len > 70)
            {
                idcams_format = 1;
            }
            else
            {
                hexdump_format = 1;
            }
        }
        else
        {
            mvsendec_format = 1;
        }
        
        sets = 4;
        if (idcams_format)
        {
            sets = 8;
        }

        sizeset = 4;
        if (mvsendec_format)
        {
            sizeset = 80;
            sets = 1;
        }

        for (x = 0; x < sets; x++)
        {
            for (y = 0; y < sizeset; y++)
            {
                if (*p != ' ')
                {
                    memcpy(hex, p, 2);
                    ch = strtol(hex, NULL, 16);
                    putc(ch, fq);
                    p += 2;
                }
                else
                {
                    /* allow mvsendec to have blanks anywhere */
                    if (mvsendec_format)
                    {
                        p++;
                    }
                    else
                    {
                        p += 2;
                    }
                }
            }            
            p++;
            
            /* idcams format has extra spaces in the middle */
            if (idcams_format && (x == 3))
            {
                p += 2;
            }
        }
    }
    fclose(fq);
    return (EXIT_SUCCESS);
}

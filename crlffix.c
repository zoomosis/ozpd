/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  crlffix - fix up cr + lf                                         */
/*                                                                   */
/*  convert <cr><lf> to just <lf> etc.                               */
/*  You need to specify whether you want the output to be crlf or    */
/*  just lf.  E.g. crlffix crlf *.c *.d                              */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trav.h"
#include "reform.h"

static char *inpat;
static char *outpat;
static int opt;

static int convert(char *filename, void *ucb);
static int convert2(char *one, char *two);
static int convert3(FILE *fp, FILE *fq);
static void writeOne(FILE *fq);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("usage: crlffix <option> <input file> <output file>\n");
        return (EXIT_FAILURE);
    }
    if (strcmp(*(argv+1), "crlf") == 0)
    {
        opt = 1;
    }
    else if (strcmp(*(argv+1), "lf") == 0)
    {
        opt = 2;
    }
    else
    {
        printf("option must be either crlf or lf\n");
        return (EXIT_FAILURE);
    }
    inpat = *(argv+2);
    outpat = *(argv+3);
    trav(inpat, NULL, convert, NULL);
    return (EXIT_SUCCESS);
}

static int convert(char *filename, void *ucb)
{
    char *filename2;
    
    (void)ucb;
    filename2 = reform(filename, inpat, outpat);
    if (filename2 == NULL)
    {
        printf("some problem with file %s\n", filename);
        return (0);
    }
    return (convert2(filename, filename2));
}

static int convert2(char *one, char *two)
{
    FILE *fp;
    FILE *fq;
    int ret = 0;
    
    fp = fopen(one, "rb");
    if (fp == NULL)
    {
        printf("failed to open %s\n", one);
    }
    else
    {
        fq = fopen(two, "wb");
        if (fq == NULL)  
        {
            printf("failed to open %s\n", two);
        }
        else
        {
            ret = convert3(fp, fq);
            fclose(fq);
        }
        fclose(fp);
    }
    return (ret);
}

static int convert3(FILE *fp, FILE *fq)
{
    int c;
    int lastCR = 0;
    
    while ((c = fgetc(fp)) != EOF)
    {
        if ((c != 0x0d) && (c != 0x0a))
        {
            if (lastCR)
            {
                writeOne(fq);
                lastCR = 0;
            }
            fputc(c, fq);
        }
        else
        {
            if (c == 0x0a)
            {
                writeOne(fq);
                lastCR = 0;
            }
            else
            {
                if (lastCR)
                {
                    writeOne(fq);
                }
                else
                {
                    lastCR = 1;
                }
            }
        }
    }
    if (lastCR)
    {
        writeOne(fq);
    }
    if (ferror(fp) || ferror(fq)) return (0);
    else return (1);
}

static void writeOne(FILE *fq)
{
    if (opt == 1)
    {
        fputc(0x0d, fq);
        fputc(0x0a, fq);
    }
    else if (opt == 2)
    {
        fputc(0x0a, fq);
    }
    return;
}


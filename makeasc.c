/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  makeasc - make a clean ASCII file (only tabs and spaces and      */
/*  newlines plus ASCII characters) by converting all non-ASCII      */
/*  characters to a "."                                              */
/*                                                                   */
/*********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "reform.h"
#include "trav.h"
#include "stritr.h"
#include "strinl.h"
#include "unused.h"

#define MAXBUF 500

static int doit(char *fnm, void *ucb);

static char *oldpat;
static char *newpat;

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        printf("usage: makeasc <infile> <outfile>\n");
        return (EXIT_FAILURE);
    }
    oldpat = *(argv + 1);
    newpat = *(argv + 2);
    trav(oldpat, NULL, doit, NULL);

    return (0);
}


static int doit(char *fnm, void *ucb)
{
    char *p;
    FILE *fp;
    FILE *fq;
    char buf[MAXBUF];
    
    unused(ucb);
    fp = fopen(fnm, "r");
    if (fp == NULL)
    {
        printf("failed to open %s\n", fnm);
        return (0);
    }
    p = reform(fnm, oldpat, newpat);
    fq = fopen(p, "w");
    if (fq == NULL)
    {
        printf("failed to open %s\n", p);
        fclose(fp);
        return (0);
    }
    while (fgets(buf, sizeof buf, fp) != NULL)
    {
        stritr(strinl(buf));
        p = buf;
        while (*p != '\0')
        {
            if (!isprint((unsigned char)*p)
                && (*p != '\t'))
            {
                *p = '.';
            }
            p++;
        }
        fputs(buf, fq);
        fputc('\n', fq);
    }
    fclose(fq);
    fclose(fp);
    return (1);
}

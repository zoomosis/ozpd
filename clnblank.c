/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  clnblank - clean blanks from the end of lines in a file.         */
/*                                                                   */
/*********************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "reform.h"
#include "trav.h"
#include "stritr.h"
#include "strinl.h"
#include "unused.h"

#define MAXBUF 500

static int doit(char *fnm, void *ucb);

static char *oldpat;
static char *newpat;
static int zap = 0;

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        printf("usage: clnblank <infile> <outfile>\n");
        return (EXIT_FAILURE);
    }
    oldpat = *(argv + 1);
    newpat = *(argv + 2);
    if (argc > 3)
    {
        zap = atoi(*(argv + 3));
    }
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
        if (zap > 0)
        {
            buf[zap] = '\0';
        }
        stritr(strinl(buf));
        fputs(buf, fq);
        fputc('\n', fq);
    }
    fclose(fq);
    fclose(fp);
    return (1);
}

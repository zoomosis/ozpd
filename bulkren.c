/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  bulkren - rename files in bulk                                   */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "reform.h"
#include "trav.h"
#include "unused.h"

static int doit(char *fnm, void *ucb);

static char *oldpat;
static char *newpat;

int main(int argc, char **argv)
{    
    if (argc < 3)
    {
        printf("usage: bulkren <oldname> <newname>\n");
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
    
    unused(ucb);
    p = reform(fnm, oldpat, newpat);
    rename(fnm, p);
    return (1);
}

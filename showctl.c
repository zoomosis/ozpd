/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  showctl - show control characters in a file                      */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "trav.h"

int ufunc(char *fnm, void *ucb);
int goodch(int c);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: showctl <filename>\n");
        return (EXIT_FAILURE);
    }
    trav(*(argv+1), NULL, ufunc, NULL);
    return (0);
}

int ufunc(char *fnm, void *ucb)
{
    FILE *fp;
    static char buf[300];
    char *p;
    
    fp = fopen(fnm, "r");
    while (fgets(buf, sizeof buf, fp) != NULL)
    {
        p = buf;
        while (*p != '\0')
        {
            if (!goodch(*p))
            {
                printf("%s: %s", fnm, buf);
                break;
            }
            p++;
        }
    }
    fclose(fp);
    return (1);
}

#define MYSTR "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
              "abcdefghijklmnopqrstuvwxyz" \
              "0123456789" \
              "!\"#%&'()*+,-./:" \
              ";<=>?[\\]^_{|}~" \
              " \t\v\f\n"

int goodch(int c)
{
    if (strchr(MYSTR, c) == NULL)
    {
        return (0);
    }
    else
    {
        return (1);
    }
}

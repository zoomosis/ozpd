/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  procfcmd - process files by executing a command                  */
/*  This program takes a wildcard to be used as a filename pattern,  */
/*  and then executes the command that follows.                      */
/*                                                                   */
/*  e.g. procfcmd *.c del %s                                         */
/*                                                                   */
/*  This would delete every C file in your directory.                */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trav.h"

static char outline[2000];

static int process(char *fileName, void *parm);

int main(int argc, char **argv)
{
    int x;
    
    if (argc < 3)
    {
        printf("usage: procfcmd <filename> <command>\n");
        return (EXIT_FAILURE);
    }
    strcpy(outline, "");
    for (x=2; x<argc; x++)
    {
        strcat(outline, *(argv+x));
        strcat(outline, " ");
    }
    trav(*(argv+1), NULL, process, NULL);
    return (EXIT_SUCCESS);
}

static int process(char *fileName, void *parm)
{
    char cmd[2000];
    
    (void)parm;
    sprintf(cmd, outline, fileName);
    printf("executing command %s\n", cmd);
    system(cmd);
    return (1);
}

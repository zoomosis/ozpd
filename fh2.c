/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/

/* This program increases the maximum number of file handles, and
   then closes file handles 3-19 in case they are open (this gets round
   a bug in C6), and then executes the program specified */

/* This program is written for OS/2 1.x */

#include <stdio.h>
#include <stdlib.h>

#define INCL_DOS
#include <os2.h>

#include <process.h>

int main(int argc, char **argv)
{
    int x;
    int ret;

    if (argc < 2)
    {
        printf("usage: fh2 <program> <parameters>\n");
        return (EXIT_FAILURE);
    }
    DosSetMaxFH(60);
    for (x = 3; x < 20; x++)
    {
        DosClose(x);
    }
    ret = execvp(argv[1], &argv[1]);
    return (ret);
}

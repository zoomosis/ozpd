/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  delzero - delete zero-length files                               */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "trav.h"

static int delzero(char *file, void *ucb);

int main(int argc, char **argv)
{
    int ret;
    
    if (argc < 2)
    {
        printf("usage: delzero <filespec>\n");
        return (EXIT_FAILURE);
    }
    ret = trav(*(argv+1), NULL, delzero, NULL);
    if (!ret)
    {
        ret = EXIT_FAILURE;
    }
    else
    {
        ret = EXIT_SUCCESS;
    }
    return (ret);
}

static int delzero(char *file, void *ucb)
{
    FILE *fp;
    
    (void)ucb;
    fp = fopen(file, "rb");
    if (fp == NULL)
    {
        printf("failed to open file %s - aborting\n", file);
        return (0);
    }
    fgetc(fp);
    if (ferror(fp))
    {
        printf("file error on file %s - aborting\n", file);
        return (0);
    }
    if (feof(fp))
    {
        fclose(fp);
        if (remove(file) != 0)
        {
            printf("failed to remove file %s - aborting\n", file);
            return (0);
        }
    }
    else
    {
        fclose(fp);
    }
    return (1);
}

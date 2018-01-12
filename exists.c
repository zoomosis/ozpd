/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the Public Domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  exists - test to see if a file exists                            */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

int exists(char *fnm)
{
    FILE *fp;
    int exsts = 0;
    
    if ((fp = fopen(fnm, "rb")) != NULL)
    {
        exsts = 1;
        fclose(fp);
    }
    return (exsts);
}

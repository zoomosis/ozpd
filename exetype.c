/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  exetype - determine whether an OS/2 executable is 16 or 32 bit.  */
/*                                                                   */
/*  You must enter the name of the executable as a parameter.        */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *fp;
    long offset;
    int c;
    char str[2];
    
    if (argc < 2)
    {
        printf("usage: exetype <fnm>\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv+1), "rb");
    if (fp == NULL)
    {
        printf("failed to open file %s\n", *(argv+1));
        return (EXIT_FAILURE);
    }
    offset = 0;
    fseek(fp, 0x3C, SEEK_SET);
    c = fgetc(fp);
    offset |= c;
    c = fgetc(fp);
    offset |= (c << 8);
    c = fgetc(fp);
    offset |= (c << 16);
    c = fgetc(fp);
    offset |= (c << 24);
    fseek(fp, offset, SEEK_SET);
    fread(str, 2, 1, fp);
    if (memcmp(str, "LX", 2) == 0)
    {
        printf("32 bit\n");
    }
    else if (memcmp(str, "NE", 2) == 0)
    {
        printf("16 bit\n");
    }
    else printf("unknown executable type\n");
    return (EXIT_SUCCESS);
}

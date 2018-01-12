/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  zpedump - do a hex dump of whatever file is specified as         */
/*  parameter 1.                                                     */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    FILE *fp;
    int   c;
    int   seq = 0;
    
    if (argc < 2)
    {
        printf("first parameter must be file name\n");
        return (EXIT_FAILURE);
    }
    fp = fopen(*(argv+1),"rb");
    if (fp == NULL)
    {
        printf("failed to open %s\n",*(argv+1));
        return (EXIT_FAILURE);
    }
    printf("hexseq decseq ch hex dec\n");
    while ((c = getc(fp)) != EOF)
    {
        seq++;
        printf("%6x %6d %2c %3.2x %3d\n", 
               seq, 
               seq, 
               isprint(c) ? c : ' ', 
               c, 
               c) ;
    }
    return (EXIT_SUCCESS) ;
}

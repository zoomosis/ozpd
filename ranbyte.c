/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  ranbyte - create a file with random bytes in it.                 */
/*                                                                   */
/*  This program takes two parameters - the name of the file you     */
/*  want to create, and the number of bytes you want to put in       */
/*  the file.                                                        */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "stdsed.h"

int main(int argc, char **argv)
{
    long total;
    long cnt;
    FILE *fq;
    int c;
    
    if (argc < 3)
    {
        printf("usage: ranbyte <outfile> <bytes>\n");
        return (EXIT_FAILURE);
    }
    stdsed();
    fq = fopen(*(argv + 1), "wb");
    if (fq == NULL)
    {
        printf("failed to open output file\n");
        return (EXIT_FAILURE);
    }
    total = atol(*(argv + 2));
    for (cnt = 0; cnt < total; cnt++)
    {
        c = rand()%256;
        putc(c, fq);
    }
    fclose(fq);
    return (0);
}

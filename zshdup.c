/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  zshdup - show duplicate lines in a file.                         */
/*  This program takes the following parameters:                     */
/*                                                                   */
/*  1) Name of the input dataset.                                    */
/*  2) Name of the output dataset.                                   */
/*  3) If supplied, the starting position to be compared.            */
/*  4) If supplied, the ending position of the compare.              */
/*                                                                   */
/*  This program looks at each line in the input file, and compares  */
/*  it to the last.  If it is the same, then both lines are output.  */
/*  Any further lines the same are also output.  If you supply a     */
/*  starting and ending position, then only data inbetween these     */
/*  positions is considered for the compare.  If you only specify a  */
/*  starting position, then the ending position is taken as the end  */
/*  of the record.  The file must be sorted in order for this        */
/*  program to function correctly.                                   */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAX 5000

int main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    static char buf1[MAX];
    static char buf2[MAX];
    char *old;
    char *new;
    char *temp;
    size_t start;
    size_t fin;
    size_t dis;
    int donefirst = 0;

    if (argc < 3)
    {
        printf("usage: zshdup <infile> <outfile> [start] [end]\n");
        return (EXIT_FAILURE);
    }
    if (argc >= 4)
    {
        start = (size_t)atoi(*(argv + 3));
    }
    else
    {
        start = 0;
    }
    if (argc >= 5)
    {
        fin = (size_t)atoi(*(argv + 4));
    }
    else
    {
        fin = MAX;
    }
    fp = fopen(*(argv + 1), "r");
    if (fp == NULL)
    {
        printf("failed to open %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }
    fq = fopen(*(argv + 2), "w");
    if (fq == NULL)
    {
        printf("failed to open %s\n", *(argv + 2));
        return (EXIT_FAILURE);
    }
    dis = fin - start;
    old = buf1;
    new = buf2;
    old[0] = '\0';
    old[start] = '\0';
    while (fgets(new, MAX, fp) != NULL)
    {
        if (strncmp(old + start, new + start, dis) == 0)
        {
            if (!donefirst)
            {
                fputs(old, fq);
            }
            fputs(new, fq);
            donefirst = 1;
        }
        else
        {
            donefirst = 0;
        }
        temp = old;
        old = new;
        new = temp;
    }
    return (EXIT_SUCCESS);
}

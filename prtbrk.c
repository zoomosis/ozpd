/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  prtbrk - put breaks in a printout                                */
/*                                                                   */
/*  This program was born of my frustration with printers not        */
/*  being easily able to be set up to skip over the perforation.     */
/*  At least, not my computer on the operating system I use.         */
/*                                                                   */
/*  Example of use: prtbrk infile.txt outfile.txt 5 50 5             */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

static int top;
static int middle;
static int bottom;

static char buf[1000];
static FILE *infile, *outfile;
static char *lastret;

int main(int argc, char **argv)
{
    int stage, x;
    
    if (argc < 6)
    {
        printf("usage: prtbrk <infile> <outfile> top middle bottom\n");
        return (EXIT_FAILURE);
    }
    infile = fopen(*(argv+1), "r");
    outfile = fopen(*(argv+2), "w");
    top = atoi(*(argv+3));
    middle = atoi(*(argv+4));
    bottom = atoi(*(argv+5));
    lastret = fgets(buf, sizeof buf, infile);
    while (lastret != NULL)
    {
        if (stage == 0)
        {
            for (x = 0; x < top; x++)
            {
                fputs("\n", outfile);
            }
            stage = 1;
            x = 0;
        }
        if (stage == 1)
        {
            x++;
            if (x > middle)
            {
                stage = 0;
                for (x = 0; x < bottom; x++)
                {
                    fputs("\n", outfile);
                }
            }
            else
            {
                fputs(buf, outfile);
                lastret = fgets(buf, sizeof buf, infile);
            }
        }
    }
    return (EXIT_SUCCESS);
}

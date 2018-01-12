/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  hex2dec - convert a hex number into decimal                      */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    long x;
    
    if (argc < 2)
    {
        printf("usage: hex2dec <n>\n");
        return (EXIT_FAILURE);
    }
    x = strtol(*(argv+1), NULL, 16);
    printf("%ld\n", x);
    return (EXIT_SUCCESS);
}

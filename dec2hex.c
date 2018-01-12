/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  dec2hex - convert a decimal number into hex                      */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    long x;
    
    if (argc < 2)
    {
        printf("usage: dec2hex <n>\n");
        return (EXIT_FAILURE);
    }
    x = strtol(*(argv+1), NULL, 10);
    printf("%lx\n", x);
    return (EXIT_SUCCESS);
}

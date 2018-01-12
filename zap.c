/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the public domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  zap - modify a file by changing a byte at an offset              */
/*  e.g. zap temp.txt 3 13  (put a 13 at offset 3 [0-based]).        */
/*                                                                   */
/*  You can use hex digits instead of decimal by prefixing the       */
/*  number with "0x" or "0X".                                        */
/*                                                                   */
/*  You can use octal digits instead of decimal by prefixing the     */
/*  number with "0".                                                 */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fu;

    if (argc < 4)
    {
        printf("usage: zap <fnm> <offs> <dec>\n");
        return (EXIT_FAILURE);
    }
    fu = fopen(*(argv+1), "r+b");
    if (fu == NULL)
    {
        printf("unable to open file %s\n", *(argv+1));
        return (EXIT_FAILURE);
    }
    fseek(fu, strtol(*(argv+2), NULL, 0), SEEK_SET);
    fputc((int)strtol(*(argv+3), NULL, 0), fu);
    fclose(fu);
    return (EXIT_SUCCESS);
}

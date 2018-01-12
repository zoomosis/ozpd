/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  stritr - strip trailing blanks immediately from passed string.   */
/*                                                                   */
/*  This function returns a pointer to the passed string.            */
/*                                                                   */
/*********************************************************************/

#include <string.h>

char *stritr(char *in)
{
    register int x, y;

    y = strlen(in);
    for (x = y - 1; x >= 0 && *(in + x) == ' '; x--) ;
    *(in + x + 1) = '\0';
    return (in);
}

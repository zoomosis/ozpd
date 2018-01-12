/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  strinl:                                                          */
/*  This program checks the last character in a string.  If it is a  */
/*  '\n' then it is replaced with a '\0'.  It returns the same       */
/*  string passed to it (except now it has been modified).           */
/*                                                                   */
/*********************************************************************/

#include <string.h>

char *strinl(char *work)
{
    int i;

    i = strlen(work);
    if (i > 0)
    {
        if (*(work + i - 1) == '\n')
        {
            *(work + i - 1) = '\0';
        }
    }
    return (work);
}

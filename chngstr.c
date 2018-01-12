/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  chngstr - change one string to another within another.           */
/*                                                                   */
/*  e.g. chngstr(buf, sizeof buf, "abc", "defg");                    */
/*                                                                   */
/*  Notes:                                                           */
/*                                                                   */
/*  1. buf must point to a valid string which is less than "max"     */
/*     in length.                                                    */
/*  2. The function returns 0 if successful, non-zero if some error  */
/*     was detected, e.g. attempted buffer overflow                  */
/*  3. If attempted buffer overflow occurs, the string will still be */
/*     valid, and include as many changes as could be done.          */
/*                                                                   */
/*********************************************************************/

#include <string.h>

int chngstr(char *m, size_t max, char *x, char *y)
{
    size_t lenx;
    size_t leny;
    size_t lenm;
    size_t sdiff;
    size_t used = 0;
    size_t avail;
    int ybig;
    char *p;
    int ret = 0;
    
    lenm = strlen(m);
    lenx = strlen(x);
    leny = strlen(y);
    avail = max - lenm;
    if (leny > lenx)
    {
        ybig = 1;
        sdiff = leny - lenx;
    }
    else
    {
        ybig = 0;
        sdiff = lenx - leny;
    }
    p = m;
    while ((p = strstr(p, x)) != NULL)
    {
        if (ybig)
        {
            if ((avail - used) < sdiff)
            {
                ret = 1;
                break;
            }
            used += sdiff;
        }
        if (sdiff != 0)
        {
            memmove(p + leny, p + lenx, strlen(p + lenx) + 1);
        }
        memcpy(p, y, leny);
        p += leny;
    }
    return (0);
}

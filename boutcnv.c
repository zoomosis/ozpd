/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  boutcnv - convert binkley outbound from and to integer.          */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char *alphabet2 = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *alphabet3 = "0123456789abcdefghijklmnopqrstuvwxyz_-!@%";

unsigned int bout2int(char *mask)
{
    unsigned int value;
    char *p1;
    char *p2;
    char *p3;

    if (isxdigit(mask[0]) && isxdigit(mask[1]) && isxdigit(mask[2]))
    {
        value = (unsigned int)strtoul(mask, NULL, 16);
    }
    else if (((p1 = strchr(alphabet2, tolower(mask[0]))) != NULL)
        && ((p2 = strchr(alphabet2, tolower(mask[1]))) != NULL)
        && ((p3 = strchr(alphabet2, tolower(mask[2]))) != NULL))
    {
        value = 4096 + (unsigned)(p1 - &alphabet2[16]) * 36 * 36
                + (p2 - alphabet2) * 36 
                + (p3 - alphabet2);
    }
    else if (((p1 = strchr(alphabet3, tolower(mask[0]))) != NULL)
        && ((p2 = strchr(alphabet3, tolower(mask[1]))) != NULL)
        && ((p3 = strchr(alphabet3, tolower(mask[2]))) != NULL))
    {
        value = 46656U + (unsigned)(p1 - &alphabet3[36]) * 41 * 41
                + (p2 - alphabet3) * 41 
                + (p3 - alphabet3);
    }
    else
    {
        value = 0;
    }
    return (value);
}

char *int2bout(unsigned int x)
{
    static char mask[4];
    
    if (x < 4096)
    {
        sprintf(mask, "%03x", x);
    }
    else if (x < 46656U)
    {
        x -= 4096;
        mask[0] = alphabet2[16 + x/(36*36)];
        x = x % (36 * 36);
        mask[1] = alphabet2[x/36];
        x = x % 36;
        mask[2] = alphabet2[x];
    }
    else if (x <= 65535U)
    {
        x -= 46656U;
        mask[0] = alphabet3[36 + x/(41*41)];
        x = x % (41 * 41);
        mask[1] = alphabet3[x/41];
        x = x % 41;
        mask[2] = alphabet3[x];
    }
    else
    {
        strcpy(mask, "000");
    }
    return (mask);
}

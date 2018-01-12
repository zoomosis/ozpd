/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program is used to map times from one time zone to another. */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int timediff;
    int x;
    int y;
    int z;
    char *from = "FROM";
    char *to = "TO";
    char *dayoff;
    
    if (argc < 2)
    {
        printf("usage: maptime <time difference> [from] [to]\n");
        printf("e.g. maptime -17 Sydney St_Louis\n");
        return (EXIT_FAILURE);
    }
    
    if (argc >= 4)
    {
        from = *(argv + 2);
        to = *(argv + 3);
    }
    
    timediff = atoi(*(argv + 1));
    
    printf("%s", from);
    for (y = (int)strlen(from); y < 5; y++)
    {
        printf("-");
    }
    printf("->%s (%+d)\n\n", to, timediff);
    for (x = 0; x < 24; x++)
    {
        printf("%02d:00-", x);
        for (y = 5; y < (int)strlen(from); y++)
        {
            printf("-");
        }
        z = x + timediff;
        if (z >= 24)
        {
            dayoff = " (+1)";
            z = z % 24;
        }
        else if (z < 0)
        {
            dayoff = " (-1)";
            z = (z % 24) + 24;
        }
        else
        {
            dayoff = "";
        }
        printf(">%02d:00%s\n", z, dayoff);
    }    
    return (0);
}

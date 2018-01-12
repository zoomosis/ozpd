/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  freqlog - produce list of FREQ'ed files                          */
/*                                                                   */
/*  This program searches the binkley.log file, and produces a       */
/*  list of all files that have been FREQed, in the format:          */
/*  filename   address                                               */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char buf[1000];
static char addr[50];
static int state;
static char *p, *q, *pbuf;

int main(void)
{
    state = 0;
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        if (*buf == '>') continue;
        pbuf = buf + 23;
        if ((strncmp(pbuf, "CONNECT", 7) == 0) 
            || (strncmp(pbuf, "Connect", 7) == 0))
        {
            state = 1;
        }
        else if ((state == 1) 
                 && ((p = strrchr(pbuf, '(')) != NULL)
                 && ((q = strchr(p, ')')) != NULL))
        {
            p++;            
            memcpy(addr, p, q - p);
            addr[(size_t)(q - p)] = '\0';
            state = 2;
        }
        else if ((state == 2) && (strncmp(pbuf, "File Request", 12) == 0))
        {
            state = 3;
        }
        else if ((state == 3) && (strncmp(pbuf, "Sent", 4) == 0))
        {
            p = strrchr(pbuf, '\\');
            if (p != NULL)
            {
                q = strchr(p, '\n');
                if (q != NULL)
                {
                    *q = '\0';
                    p++;
                    printf("%-15s %s\n", p, addr);
                }
            }
        }
        else if ((state == 3) && (strncmp(pbuf, "End of", 6) == 0))
        {
            state = 0;
        }
        else if ((state == 3) && (strncmp(pbuf, "Session with", 12) == 0))
        {
            state = 0;
        }
    }
    return (0);
}

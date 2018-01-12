/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program finds out the longest line in a file.               */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

int main(void)
{
    int linenum = 0;
    int cnt = 0;
    int maxline = 0;
    int max = 0;
    int c;
    
    while ((c = getc(stdin)) != EOF)
    {
        if (c == '\n')
        {
            linenum++;
            if (cnt > max)
            {
                maxline = linenum;
                max = cnt;
            }
            cnt = 0;
        }
        else
        {
            cnt++;
        }
    }

    if (cnt > 0)
    {
        printf("last line not new-line terminated\n");
    }

    if (linenum > 0)
    {
        printf("longest line was number %d which was %d long\n",
               maxline, max);
    }
    else
    {
        printf("no data input\n");
    }
    
    return (0);    
}

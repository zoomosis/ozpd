/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program reads in a file, already sorted with case           */
/*  insensitive, and prints out all the names that will conflict     */
/*  if this program is compiled on an MVS system, which only has     */
/*  8-character uniqueness and no case sensitivity.                  */
/*                                                                   */
/*  You can create such an input file by doing the following:        */
/*  copy *.s temp.txt                                                */
/*  grep "^\* X\-" temp.txt | awk "{print $3}" | sort -f >temp2.txt  */
/*  mvsdupx <temp2.txt >temp3.txt                                    */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXBUF 100

static int compare(char *buf1, char *buf2);
static void prtone(char *buf);

int main(void)
{
    char buf1[MAXBUF];
    char buf2[MAXBUF];
    int inseq = 0;
    
    strcpy(buf1, "");
    while (fgets(buf2, sizeof buf2, stdin) != NULL)
    {
        if (compare(buf1, buf2) == 0)
        {
            if (!inseq)
            {
                inseq = 1;
                prtone(buf1);
            }
            prtone(buf2);
        }
        else
        {
            inseq = 0;
        }
        strcpy(buf1, buf2);
    }
    return (0);
}

static int compare(char *buf1, char *buf2)
{
    int x;
    
    for (x = 0; x < 8; x++)
    {
        if (buf1[x] == buf2[x])
        {
            if (buf1[x] == '\0') break;
        }
        else
        {
            if (toupper((unsigned char)buf1[x]) 
                == toupper((unsigned char)buf2[x])) continue;
            return (-1);
        }
    }
    return (0);
}

static void prtone(char *buf)
{
    char *p;
    
    p = strchr(buf, '\n');
    if (p == NULL)
    {
        p = strchr(buf, '\0');
    }
    printf("#define ");
    fwrite(buf, p - buf, 1, stdout);
    printf(" %s", buf);
    return;
}

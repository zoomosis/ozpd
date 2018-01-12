/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  m2x - give the hex representation of some memory.                */
/*                                                                   */
/*  This program takes the following parameters:                     */
/*  1) Pointer to memory area.                                       */
/*  2) Length of memory to be translated.                            */
/*                                                                   */
/*  e.g. printf("%s\n",m2x("ABC",3));                                */
/*                                                                   */
/*    gives c1c2c3 if executed on an EBCDIC machine.                 */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

char *m2x(void *p,int len)
{
    static char *p1 = NULL;
    int i,reslen;
  
    reslen = len * 2;
    p1 = (char *)realloc(p1,reslen+1);
    *p1 = '\0';
    for (i=0;i<reslen;i+=2,p=(char *)p+1)
    {
        sprintf(p1+i,"%0.2x",*(unsigned char *)p);
    }
    return (p1);
}

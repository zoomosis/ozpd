/* Written by Joshua Marshall, 3:640/452.35@fidonet */
/* Released to the public domain */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "fsf.h"
#include "trav.h"

char xtras[100][80];
int  xtrapos;
int  in_comment,in_quotes,in_quote,in_bracket,touchem;

int main(int argc, char **argv)
{
  int i,localpos;
  
  in_comment=in_quotes=in_quote=in_bracket=touchem=localpos=0;
  
  if (argc < 2)
  {
    printf("%s version 1.0 compiled %s.\n", argv[0], __DATE__);
    printf("Useage: %s <string> [switches] <filename>\n",argv[0]);
    printf("Where [switches] are:\n");
    printf("-T   Touch all files containing string\n");
    printf("-Cx  Comments\n");
    printf("-Dx  Double Quotes\n");
    printf("-Sx  Single Quotes\n");
    printf("-Bx  Brackets\n");
    printf("\n");
    printf("And x is:\n");
    printf("  I = Search inside\n");
    printf("  O = Search outside\n");
    exit(1);
  }

  for (i=1;i<argc;i++)
  {
    if (argv[i][0]=='-')
    {
      if (toupper(argv[i][1])=='T')
        touchem=1;
      if (toupper(argv[i][1])=='C')
        if (toupper(argv[i][2])=='I')
          in_comment=1;
        else if (toupper(argv[i][2])=='O')
          in_comment=2;
      if (toupper(argv[i][1])=='D')
        if (toupper(argv[i][2])=='I')
          in_quotes=1;
        else if (toupper(argv[i][2])=='O')
          in_quotes=2;
      if (toupper(argv[i][1])=='S')
        if (toupper(argv[i][2])=='I')
          in_quote=1;
        else if (toupper(argv[i][2])=='O')
          in_quote=2;
      if (toupper(argv[i][1])=='B')
        if (toupper(argv[i][2])=='I')
          in_bracket=1;
        else if (toupper(argv[i][2])=='O')
          in_bracket=2;
    }
  }
  strcpy(xtras[0], argv[1]);
  xtrapos=1;
  while (xtrapos>localpos)
  {
    trav((localpos==0?argv[i-1]:"*.?"), NULL, find_string_format, 
xtras[localpos++]);
  }
  return (0);
}

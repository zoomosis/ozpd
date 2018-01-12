/* Written by Joshua Marshall, 3:640/452.35@fidonet */
/* Released to the public domain */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fsf.h"

char in[1000];

int find_string_format(char *filename, void *srchstr)
{
  FILE *infile;
  int i;
  int bracket=0;
  int comment=0;
  int quotes;
  int quote;
  int sp;
  int linenum=0;
  int inside;
  int already=0;
  char temp[80];
  
  infile = fopen(filename,"r");
  if (infile == NULL)
  {
    printf("Unable to open %s\n", filename);
    exit(1);
  }
  
  for (;;)
  {
    fgets(in, 1000, infile);
    linenum++;
    if (feof(infile))
      break;
    quote=quotes=sp=i=inside=0;
    if (strstr(in, srchstr)!=NULL)
      inside=1;
    while (in[i]!=0)
    {
      if ((quote==0)&&(quotes==0)&&(comment==0)&&(in[i]=='(')) bracket++;
      if ((quote==0)&&(quotes==0)&&(comment==0)&&(in[i]==')')) bracket--;
      if ((comment==0)&&(in[i]=='\'')
         &&((in[(i==0?0:i-1)]!='\\')||((in[(i==0?0:i-1)]=='\\')&&(in[(i==0?0:i-
2)]=='\\'))))
        quote=(quote==0?1:0);
      if ((comment==0)&&(in[i]=='\"')
         &&((in[(i==0?0:i-1)]!='\\')||((in[(i==0?0:i-1)]=='\\')&&(in[(i==0?0:i-
2)]=='\\'))))
        quotes=(quotes==0?1:0);
      if ((quote==0)&&(quotes==0)&&(in[i]=='/')&&(in[i+1]=='*')) comment++;
      if ((quote==0)&&(quotes==0)&&(in[i]=='*')&&(in[i+1]=='/')) comment--;
      if (inside==1)
        if (strncmp(&in[i], srchstr, strlen(srchstr))==0)
          if (((in_quote==1)&&(quote!=0))||((in_quote==2)&&(quote==0))||(in_quote==0))
            if (((in_quotes==1)&&(quotes!=0))||((in_quotes==2)&&(quotes==0))||(
in_quotes==0))
              if (((in_comment==1)&&(comment!=0))||((in_comment==2)&&(comment==
0))||(in_comment==0))
                if (((in_bracket==1)&&(bracket!=0))||((in_bracket==2)&&(bracket
==0))||(in_bracket==0))
                {
                  if (already==0)
                  {
                    if (touchem)
                      printf("Touching %s\n", filename);
                    else
                      printf("%s:\n", filename);
                    already=1;
                    if ((strstr(filename, ".h"))&&touchem)
                    {
                      strcpy(xtras[xtrapos++], filename);
                    }
                    if (touchem)
                    {
                      sprintf(temp, "touch %s", filename);
                      system(temp);
                    }
                  }
                  if (!touchem)
                    printf("%5d:%s", linenum, in);
                  break;
                }
      i++;
      }
   }
 fclose(infile);
 return 1;
}

void prname(char *name)
{
  printf("%s\n", name);
}

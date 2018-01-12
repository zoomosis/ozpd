/* qwk2txt - convert the messages from a QWK mail packet into a
   text file.  */
/* Written by Paul Edwards and released to the Public Domain */

#include <stdio.h>
#include <stdlib.h>

static void gen_mess(FILE *fp, FILE *fq);

int main(void)
{
  FILE *fp, *fq;

  fp = fopen("messages.dat","rb");
  if (fp != NULL)
  {
    fq = fopen("messages.txt","w");
    if (fq != NULL)
    {
      gen_mess(fp,fq);
    }
  }
  return (0);
}

static void gen_mess(FILE *fp, FILE *fq)
{
  char tdate[9];
  char ttime[5];
  char tsubj[25];
  char tfrom[25];
  char tto[25];
  char tnumlin[7];
  long numlin, x;
  int c, y;

  fseek(fp,128,SEEK_CUR); /* skip file header */
  while (!feof(fp))
  {
    fseek(fp,1,SEEK_CUR); /* skip private flag */
    fseek(fp,7,SEEK_CUR); /* skip message number */
    if (fread(tdate,8,1,fp) != 1) break;
    fprintf(fq,"Date: %.8s\n",tdate);
    fread(ttime,5,1,fp);
    fprintf(fq,"Time: %.5s\n",ttime); 
    fread(tto,25,1,fp);
    fprintf(fq,"To: %.25s\n",tto);
    fread(tfrom,25,1,fp);
    fprintf(fq,"From: %.25s\n",tfrom);
    fread(tsubj,25,1,fp);
    fprintf(fq,"Subj: %.25s\n",tsubj);
    fseek(fp,12,SEEK_CUR); /* skip password */
    fseek(fp,8,SEEK_CUR); /* skip reference number */
    fread(tnumlin,6,1,fp);
    *(tnumlin+6) = '\0';
    numlin = atol(tnumlin)-1;
    fseek(fp,1,SEEK_CUR); /* skip message active flag */
    fseek(fp,2,SEEK_CUR); /* skip conference number */
    fseek(fp,3,SEEK_CUR); /* skip junk */
    for (x = 0L; x < numlin; x++)
    { 
      for (y = 0; y < 128; y++)
      {
        c = fgetc(fp);
        if (c == 0xe3) fputc('\n',fq);
        else if (c != '\0') fputc(c,fq);
      }
    }
    fputc('\n',fq);
    fputc('\n',fq);
  }
}

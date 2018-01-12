/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards                             */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  mvsunzip - unzip files for use by pdpclib                        */
/*  e.g. mvsunzip pdpclib.czip pdpclib.c                             */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 500000

static int onefile(FILE *infile);
static char *ascii2l(char *buf);
static char *outn;

int main(int argc, char **argv)
{
    FILE *infile;
    int c;

    outn = *(argv+2);
    infile = fopen(*(argv+1), "rb");
    while (onefile(infile)) ;
    return (0);
}

static int onefile(FILE *infile)
{
    int c;
    int x;
    long size;
    int fnmlen;
    char fnm[FILENAME_MAX];
    static char *buf = NULL;
    char newfnm[FILENAME_MAX];
    FILE *newf;

    if (buf == NULL)
    {
        buf = malloc(MAXBUF);
    }
    if (fgetc(infile) != 0x50)
    {
        return (0);
    }
    if (fgetc(infile) != 0x4b)
    {
        return (0);
    }
    for (x = 0; x < 16; x++)
    {
        fgetc(infile);
    }
    c = fgetc(infile);
    size = c;
    c = fgetc(infile);
    size = (c << 8) | size;
    c = fgetc(infile);
    size = (c << 16) | size;
    c = fgetc(infile);
    size = (c << 24) | size;
    if (size > MAXBUF)
    {
        return (0);
    }
    for (x = 0; x < 4; x++)
    {
        fgetc(infile);
    }
    c = fgetc(infile);
    fnmlen = c;
    c = fgetc(infile);
    fnmlen = (c << 8) | fnmlen;
    fgetc(infile);
    fgetc(infile);
    fread(fnm, fnmlen, 1, infile);
    fnm[fnmlen] = '\0';
    ascii2l(fnm);
    printf("fnm is %s\n", fnm);
    fread(buf, size, 1, infile);
    buf[size] = '\0';
    ascii2l(buf);
    if (strchr(fnm, '.') != NULL) *strchr(fnm, '.') = '\0';
    while (strchr(fnm, '-') != NULL) *strchr(fnm, '-') = '@';
    while (strchr(fnm, '_') != NULL) *strchr(fnm, '_') = '@';
    sprintf(newfnm, "%s(%s)", outn, fnm);
    newf = fopen(newfnm, "w");
    fwrite(buf, size, 1, newf);
    fclose(newf);
    return (1);
}

static char *ascii2l(char *buf)
{
    char *p;

    p = buf;
    while (*p != '\0')
    {
        *p = fasc(*p);
        p++;
    }
    return (buf);
}

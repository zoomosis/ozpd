/* Written by Paul Edwards */
/* Released to the public domain */
/* assemble - join cut files together */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNKSZ 30720

static char *outfn;
static FILE *outf;
static FILE *inf;
static char infn[FILENAME_MAX];
static char buf[CHUNKSZ];
static char *p;
static int upto = 0;

static void process(void);
static int openfile(void);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage: assemble <filename>\n");
        return (EXIT_FAILURE);
    }
    
    outfn = *(argv + 1);
    process();
    return (0);
}

static void process(void)
{
    size_t x;
    
    outf = fopen(outfn, "wb");
    if (outf == NULL)
    {
        printf("failed to open %s for writing\n", outfn);
        exit(EXIT_FAILURE);
    }
    printf("ASSEMBLE: Opened file %s as output.\n", outfn);
    setvbuf(outf, NULL, _IOFBF, CHUNKSZ);
    strcpy(infn, outfn);
    p = strchr(infn, '.');
    if (p == NULL)
    {
        p = infn + strlen(infn);
        *p = '.';
    }
    p++;
    while (openfile())
    {
        while ((x = fread(buf, 1, sizeof buf, inf)) > 0)
        {
            fwrite(buf, 1, x, outf);
        }
        if (ferror(inf))
        {
            printf("an error occurred reading file %s\n", infn);
            exit(EXIT_FAILURE);
        }
        fclose(inf);
    }
    if (ferror(outf))
    {
        printf("an error occurred writing file %s\n", outfn);
        exit(EXIT_FAILURE);
    }
    if (fclose(outf) != 0)
    {
        printf("error closing file %s\n", outfn);
        exit(EXIT_FAILURE);
    }
    printf("ASSEMBLE: No more files.  Exiting normally.\n");
    return;
}

static int openfile(void)
{
    upto++;
    sprintf(p, "%03d", upto);
    inf = fopen(infn, "rb");
    if (inf == NULL)
    {
        return (0);
    }
    else
    {
        printf("ASSEMBLE: Reading file %s.\n", infn);
        setvbuf(inf, NULL, _IOFBF, CHUNKSZ);
        return (1);
    }
}

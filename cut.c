/* Written by Paul Edwards */
/* Released to the public domain */
/* cut - cut a file into parts */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNKSZ 30720

static char outfn[FILENAME_MAX];
static FILE *outf;
static FILE *inf;
static char *infn;
static char buf[CHUNKSZ];
static char *p;
static int upto = 0;
static unsigned long limit;
static unsigned long cnt;
static int c;

static void process(void);
static void openfile(void);

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("usage: cut <filename> <chunk size>\n");
        return (EXIT_FAILURE);
    }
    
    infn = *(argv + 1);
    limit = strtoul(*(argv + 2), NULL, 0);
    process();
    return (0);
}

static void process(void)
{
    size_t x;
    
    inf = fopen(infn, "rb");
    if (inf == NULL)
    {
        printf("failed to open %s for reading\n", infn);
        exit(EXIT_FAILURE);
    }
    printf("CUT: Opened file %s as input.\n", infn);
    setvbuf(inf, NULL, _IOFBF, CHUNKSZ);
    strcpy(outfn, infn);
    p = strchr(outfn, '.');
    if (p == NULL)
    {
        p = outfn + strlen(outfn);
        *p = '.';
    }
    p++;
    c = getc(inf); /* force EOF if empty file */
    cnt = 0;
    while (!feof(inf))
    {
        ungetc(c, inf);
        openfile();
        
        /* handle big chunks */
        while ((cnt + CHUNKSZ) < limit)
        {
            x = fread(buf, 1, sizeof buf, inf);
            if (fwrite(buf, 1, x, outf) != x)
            {
                printf("write error\n");
                exit(EXIT_FAILURE);
            }
            cnt += CHUNKSZ;
        }
        
        /* handle small cases */
        x = fread(buf, 1, (size_t)(limit - cnt), inf);
        if (fwrite(buf, 1, x, outf) != x)
        {
            printf("write error=n");
            exit(EXIT_FAILURE);
        }
        
        if (ferror(inf))
        {
            printf("read error\n");
            exit(EXIT_FAILURE);
        }
        
        if (fclose(outf) != 0)
        {
            printf("error closing file %s\n", outfn);
            exit(EXIT_FAILURE);
        }
        
        c = getc(inf); /* force EOF if nothing left */
        cnt = 0;
    }
    printf("CUT: exiting normally\n");
    return;
}

static void openfile(void)
{
    upto++;
    sprintf(p, "%03d", upto);
    outf = fopen(outfn, "wb");
    if (outf == NULL)
    {
        printf("failed to open output file %s\n", outfn);
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("CUT: Writing file %s.\n", outfn);
        setvbuf(outf, NULL, _IOFBF, CHUNKSZ);
    }
    return;
}

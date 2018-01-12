/* Written by Raymond Zhong */
/* Released to the public domain */
/* modified 1995-02-05 by Paul Edwards to make it zero-based and
   not prompt for overwrite of output file */
/* modified 1995-07-23 by Paul Edwards to make it use longs when
   accessing the file.  Also fixed a couple of bugs. */
/* modified 1999-08-10 by Paul Edwards to fix zero-based
   processing and other cosmetic changes. */
/* modified 1999-08-19 by Paul Edwards to use more flexible
   strtol instead of atol. */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int in_ch;
    long startch, endch, count = 0;
    FILE *infile,*outfile;
    
    if (argc != 5)
    {
        printf("Usage: %s infile outfile startch endch\n", 
               argv[0]);
        return(EXIT_FAILURE);
    }

    if ((infile = fopen(argv[1],"rb")) == NULL)
    {
        printf("Unable to open infile '%s'\n", argv[2]);
        return(EXIT_FAILURE);
    }

    if ((outfile = fopen(argv[2], "wb")) == NULL)
    {
        printf("Unable to open outfile '%s'\n", argv[3]);
        return (EXIT_FAILURE);
    }

    startch = strtol(argv[3], NULL, 0);
    endch = strtol(argv[4], NULL, 0);
    
    if (startch > endch)
    {
        printf("Start position can not be greater "
                "than end position.\n");
        return(EXIT_FAILURE);
    }

    /* skip leading characters */
    while ((count < startch) && ((in_ch = getc(infile)) != EOF))
    {
        count++;
    }
    
    if (in_ch == EOF)
    {
        printf("End of file before startch.\n");
        return(EXIT_FAILURE);
    }
    
    /* copy remaining characters */
    while ((count <= endch) && ((in_ch = getc(infile)) != EOF))
    {
        if (putc(in_ch, outfile) == EOF)
        {
            printf("write error\n");
            return (EXIT_FAILURE);
        }
        count++;
    }

    if (ferror(infile))
    {
        printf("error occurred reading file\n");
        return (EXIT_FAILURE);
    }
    
    fclose(infile);
    
    if (fclose(outfile) == EOF)
    {
        printf("failed to close output file\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

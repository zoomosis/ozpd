/* Written by Xie Yi Zhong */
/* Released to the public domain */
/* modified 1995-02-05 by Paul Edwards to make it zero-based and
   not prompt for overwrite of output file */
/* modified 1995-07-23 by Paul Edwards to make it use longs when
   accessing the file.  Also fixed a couple of bugs. */   

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int in_ch;
    long startch, endch, count = 0;
    FILE *infile,*outfile;
    
    if (argc != 5)
    {
        fprintf(stderr, 
                "\n\aUsage: %s infile outfile startch endch \n\n", 
                argv[0]);
        return(EXIT_FAILURE);
    }

    if ((infile = fopen(argv[1],"rb")) == NULL)
    {
        fprintf(stderr,"\n\aUnable to open infile '%s'\n\n", argv[2]);
        return(EXIT_FAILURE);
    }

    outfile = fopen(argv[2], "wb");

    startch = atol(argv[3]);        
    endch = atol(argv[4]);
    if (startch > endch)
    {
        printf("\n\aStart position can not be greater "
                "than end position.");
        return(EXIT_FAILURE);
    }

    if ((in_ch = getc(infile)) != EOF)
    {
        count++;
        while ((count < startch) && (in_ch != EOF))
        {
            in_ch = getc(infile);
            count++;
        }
        if (in_ch == EOF)
        {
            printf("\n\aEnd of file before startch.");
            return(EXIT_FAILURE);
        }
        while ((count <= endch) && (in_ch != EOF))
        {
            putc(in_ch, outfile);
            in_ch = getc(infile);
            count++;
        }
        if (in_ch == EOF)
        {
            printf("\n\aEnd of file before endch.");
            return(EXIT_FAILURE);
        }        
    }

    fclose(infile);
    fclose(outfile);

    return(0);
}

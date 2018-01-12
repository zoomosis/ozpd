/***************************************************************************/
/* arctype.c                                                               */
/*                                                                         */
/* Written by Paul Markham, Jan 1993.                                      */
/*                                                                         */
/* this program is released to the public domain                           */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/****************************************************************************
    Given a pointer to a file name, return a pointer to a string identifying
    the archive type, or NULL if an unknown archive.

    Improvements:
        1. the archive table could be read from a configuration file to allow
           extra archive types to be added easily.

        2. the signature field may contain imbeded nulls so it should not be
           a string. You would then need to include the signature length
           in the structure.

        3. the amount of data that needs be read from the file should be
           determined from the length of the signature and the offset. This
           code just hopes that the first 256 bytes are good enough!

        4. the handling of an error from fopen is not very nice. But then
           again you should have checked if the file exists before calling
           this routine :-)
*****************************************************************************/

char *arctype(char *fname)
    {
    static struct
        {
        char *type;
        char *sig;
        int  offset;
        } arc[] =
            {"ZIP",   "PK",       0,
             "LHARC", "-lh",      2,
             "ZOO",   "ZOO",      0,
             "ARJ",   "\x60\xea", 0,
             "ARC",   "\x1a\x08", 0};

    #define NUM_ARC (sizeof(arc) / sizeof(arc[0]))

    int  i;
    char hdr[256];
    char *ret_type = NULL;
    FILE *fp;

    assert ((fp = fopen(fname, "rb")) != NULL);
    fread(hdr, sizeof(hdr), 1, fp);
    fclose(fp);
    for (i = 0; i < NUM_ARC; i++)
        {
        if (memcmp(hdr + arc[i].offset, arc[i].sig, strlen(arc[i].sig)) == 0)
            {
            ret_type = arc[i].type;
            break;
            }
        }
    return(ret_type);
    }

/*---------- Test program for arctype() ----------*/
/*int main(int argc, char **argv)
    {
    char *t;
    int  i;

    if (argc == 1)
        {
        printf("format: ARCTYPE filename\n");
        exit(EXIT_FAILURE);
        }

    if ((t = arctype(argv[1])) == NULL)
        printf("Unknown archive type\n");
    else
        printf("Archive type is %s\n", t);
    return(EXIT_SUCCESS);
    }
*/

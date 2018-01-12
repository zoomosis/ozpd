/*****************************************************************************/
/*                                                                           */
/* This program Written by Joshua Marshall                                   */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/* Sort - sort a text file on a certain column and datatype                  */
/*                                                                           */
/*                                                                           */
/* This program is released to the public domain.                            */
/*                                                                           */
/* Written December 1996.                                                    */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Local includes                                                            */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*****************************************************************************/
/* Local defines                                                             */
/*****************************************************************************/

#define ALPHA_TYPE   1
#define NUMBER_TYPE  2

#define UP_TYPE      0
#define DOWN_TYPE    1

/*****************************************************************************/
/* Local structures                                                          */
/*****************************************************************************/

typedef struct LINE
{
    char *text;
    struct LINE *nextline;
}
line;

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/

char *filein = NULL;
char *fileout = NULL;
int field = 0;
int datatype = 0;
int direction = 0;
line firstline;
int numlines = 0;

/*****************************************************************************/
/* Local variables                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Local functions                                                           */
/*****************************************************************************/

static int compare(char *s1, char *s2)
{
    char *tmp1;
    char *tmp2;
    char *tok1;
    char *tok2;
    int i;

    if (field > 0)
    {
        tmp1 = (char *)(malloc(strlen(s1) + 1));
        tmp2 = (char *)(malloc(strlen(s2) + 1));

        strcpy(tmp1, s1);
        strcpy(tmp2, s2);
        tok1 = strtok(tmp1, " ");
        for (i = 1; i < field; i++)
        {
            tok1 = strtok(NULL, " ");
        }
        free(tmp1);
        if (!tok1)
            return 0;           /* Leave everything if field not found */

        tok2 = strtok(tmp2, " ");
        for (i = 1; i < field; i++)
            tok2 = strtok(NULL, " ");
        free(tmp2);
        if (!tok2)
            return 0;           /* Leave everything if field not found */
    }
    else
    {
        tok1 = s1;
        tok2 = s2;
    }

    if (datatype == NUMBER_TYPE)
    {
        double i1;
        double i2;

        i1 = atof(tok1);
        i2 = atof(tok2);
        if (i1 < i2)
            return -1;
        if (i1 > i2)
            return 1;
        return 0;
    }

    return strcmp(tok1, tok2);
}

static void read_list(void)
{
    FILE *in;

    line *list;
    line *lastlist = &firstline;
    char s[500];

    in = fopen(filein, "r");

    if (in)
    {
        while (fgets(s, 500, in))
        {
            list = (line *) (malloc(sizeof(line)));
            if (list)
            {
                lastlist->nextline = list;
                list->text = (char *)(malloc(strlen(s) + 1));
                if (list->text)
                {
                    numlines++;
                    strcpy(list->text, s);
                }
                list->nextline = NULL;
                lastlist = list;
            }
        }
        fclose(in);
    }
}

static void sort_list(void)
{
    struct LINE *t1;
    struct LINE *t2;
    long max;
    long tmp;

    t1 = &firstline;

    while (t1->nextline)
    {
        t2 = t1->nextline;
        max = (long)(t1->nextline);

#ifdef DEBUG
        printf(".");
        fflush(stdout);
#endif

        while (t2->nextline)
        {
            if (((direction == DOWN_TYPE) && (compare(((struct LINE *)(max))->text,
                                              (t2->nextline)->text) < 0))
                || ((direction == UP_TYPE) && (compare(((struct LINE *)(max))->text,
                                             (t2->nextline)->text) > 0)))
            {
                max = (long)(t2->nextline);
            }
            t2 = t2->nextline;
        }

        tmp = (long)(((struct LINE *)(max))->text);
        ((struct LINE *)(max))->text = (t1->nextline)->text;
        (t1->nextline)->text = (char *)(tmp);

        t1 = t1->nextline;
    }
}

static void write_list(void)
{
    FILE *out;
    line *list;
    line *tmp;

    out = fopen(fileout, "w");

    if (out)
    {
        list = firstline.nextline;
        while (list)
        {
            fprintf(out, "%s", list->text);
            free(list->text);
            tmp = list;
            list = list->nextline;
            free(tmp);
        }
        fclose(out);
    }
}

/*****************************************************************************/
/* Global functions                                                          */
/*****************************************************************************/

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("Sort by Joshua Marshall, compiled" __DATE__ "\n\n");
        printf("This program released to the public domain\n");
        printf("Usage: sort <in-file> <out-file> <field #> <datatype>"
               " < direction > \n ");
        printf("Where field is the column number (default 0 - whole line)\n");
        printf("And datatype is\n");
        printf("  ALPHA (default)  - alphanumeric\n");
        printf("  NUMBER           - numeric\n");
        printf("And direction is\n");
        printf("  UP (default)     - smallest at the top\n");
        printf("  DOWN             - largest at the top\n");
    }

    if (argc > 1)
        filein = argv[1];
    if (argc > 2)
        fileout = argv[2];
    if (argc > 3)
    {
        int i = 2;

        while (++i < argc)
        {
            if (strcmp(argv[i], "ALPHA") == 0)
                datatype = ALPHA_TYPE;
            if (strcmp(argv[i], "NUMBER") == 0)
                datatype = NUMBER_TYPE;
            if (strcmp(argv[i], "UP") == 0)
                direction = UP_TYPE;
            if (strcmp(argv[i], "DOWN") == 0)
                direction = DOWN_TYPE;
            if (isdigit(*argv[i]))
                field = atoi(argv[i]);
        }
    }

    read_list();

    sort_list();

    write_list();

    return 0;
}

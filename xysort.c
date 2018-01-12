/* XYSORT version 2.1
 * Written by Xie Yi Zhong
 * Released to the public domain
 * This program is a replacement for the dos "sort" function
 * Usage: XYSORT [+n] [/r] < infile [> outfile]
 * Slight mods by Paul Edwards on 1995-10-22
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int sort_funct(const void *a, const void *b);

#define MAXBUF 9000

struct table
{
    char *buff;
    struct table *next;
};

int pos;
static char in_buff[MAXBUF];

int main(int argc, char *argv[])
{
    struct table *first;
    struct table *node;
    struct table *new;
    static char **tab;
    char *in;
    char buff[30];
    size_t count = 0;
    size_t x;
    int i;

    /* Initialize the first record of the linked list */
    if ((first = (struct table *)
         malloc(sizeof(struct table))) == NULL)
    {
        fprintf(stderr, "\a\n\tUnable to obtain "
                "memory!\n");
        return (EXIT_FAILURE);
    }
    node = first;
    node->next = NULL;

    /* Read input into the linked list */
    in = gets(in_buff);
    while (in)
    {
        node->buff = malloc(strlen(in_buff) + 1); /* mod PE */
        if (node->buff == NULL)
        {
            fprintf(stderr, "\a\n\tUnable to obtain new "
                    "memory!\n");
            return (EXIT_FAILURE);
        }
        memcpy(node->buff, in_buff, strlen(in_buff) + 1);
        count++;
        if ((new = (struct table *)
             malloc(sizeof(struct table))) == NULL)
        {
            fprintf(stderr, "\a\n\tUnable to obtain new"
                    "memory!\n");
            return (EXIT_FAILURE);
        }
        in = gets(in_buff);
        if (!in)
            node->next = NULL;
        else
        {
            node->next = new;
            node = new;
        }
    }

    /* Find out which column the sort should accord to */
    if (argc > 1 && argv[1][0] == '+')
    {
        for (i = 1; argv[1][i]; i++)
        {
            if (!isdigit(argv[1][i]))
            {
                printf("\nUnexpected parameter format!\n");
                return (EXIT_FAILURE);
            }
            buff[i - 1] = argv[1][i];
        }
        sscanf(buff, "%d", &pos);
    }
    else
    {
        pos = 1;
    }

    /* Construct a table for sort */
    tab = (char **) malloc(count * sizeof(char *));
    node = first;
    for (x = 0; x < count; x++)
    {
        tab[x] = node->buff;
        node = node->next;
    }

    /* Sort the table */
    qsort((void *) tab, count, sizeof(tab[0]), sort_funct);

    /* Print output */
    if (argc > 1 && (strcmp(argv[1], "/r") == 0 || strcmp(argv[1], "/R") == 0 ||
                  strcmp(argv[2], "/r") == 0 || strcmp(argv[2], "/R") == 0))
    {
        for (x = count - 1; x + 1 > 0; x--)
        {
            puts(tab[x]);
        }
    }
    else
    {
        for (x = 0; x < count; x++)
        {
            puts(tab[x]);
        }
    }
    return (EXIT_SUCCESS);
}

/* Comparison function */
int sort_funct(const void *a, const void *b)
{
    int i;
    char *tmp1,
    *tmp2;

    tmp1 = *(char **) a;
    tmp2 = *(char **) b;
    for (i = 1; i < pos; i++)
    {
        tmp1++;
        tmp2++;
    }

    return (strcmp(tmp1, tmp2));
}

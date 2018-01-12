/* Written by Xie Yi Zhong */
/* Released to the public domain */
/* This program is a replacement for the dos "sort" function */
/* modified 1995-02-05 by Paul Edwards to fix a bug */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sort_funct (const void *a, const void *b);

#define MAXBUF 9000

struct table
{
	char *buff;
	struct table *next;
};

static char in_buff[MAXBUF];

int main(void)
{
	struct table *first, *node, *new;
	static char ** tab;
	char *in;
	size_t count=0, x;

	/* Allocate memory */
	if((first = (struct table *)
		malloc(sizeof(struct table)))==NULL)
	{
		fprintf(stderr, "\a\n\tUnable to obtain "
		"memory!\n");
		return (EXIT_FAILURE);
	}
	node=first;
	node->next=NULL;
	
	/* Read input */
	in = gets(in_buff);
	while(in)
	{
        node->buff = malloc(strlen(in_buff) + 1); /* mod PE 1995-02-05 */
        if (node->buff == NULL)
        {
			fprintf(stderr, "\a\n\tUnable to obtain new"
			"memory!\n");
			return (EXIT_FAILURE);
        }
		memcpy(node->buff, in_buff, strlen(in_buff)+1);
		count++;
		if((new=(struct table *)
			malloc(sizeof(struct table)))==NULL)
		{
			fprintf(stderr, "\a\n\tUnable to obtain new"
			"memory!\n");
			return (EXIT_FAILURE);
		}
		in = gets(in_buff);
		if(!in)
			node->next = NULL;
		else
		{
			node->next=new;
			node=new;
		}
	}
	
	/* Sort table */
	tab = (char **) malloc(count * sizeof(char *));
	node = first;
	for (x=0; x<count; x++)
	{
		tab[x] = node->buff;
		
		node = node->next;
	}

	qsort((void *)tab, count, sizeof(tab[0]), sort_funct);

	/* Print output */
	for (x=0; x < count; x++)
	{
		puts(tab[x]);
		
		
	}
	return (EXIT_SUCCESS);
}


int sort_funct(const void *a, const void *b)
{
	return ( strcmp(*(char **)a,*(char **)b) );
}

/* seen-by processing */
/* Written by Paul Edwards, 1993.7.3 */
/* Released to the Public Domain */

#ifndef SEENBY
#define SEENBY

#include <stddef.h>

typedef struct {
    int net;
    int node;
  } seenby_entry;

typedef struct 
{
  seenby_entry *entry;
  size_t numelem;
  size_t maxelem;
  size_t curelem;
  int lastnet;
} seenby;

int seenby_init(seenby *snby, size_t maxelem);
int seenby_term(seenby *snby);
int seenby_reset(seenby *snby);
int seenby_addline(seenby *snby, const unsigned char *snbyline);
int seenby_sort(seenby *snby);
int seenby_extract(seenby *snby, unsigned char *extract, size_t max);

#endif


/* seen-by processing */
/* Written by Paul Edwards, 1993-07-03 */
/* Released to the Public Domain */
/* This set of routines is designed to allow seen-by lines to be added
to an internal buffer, and then extracted. */


#include <stdlib.h>


#include "seenby.h"


static int seenby_compare(const void *l, const void *r);
static int seenby_extract_one(int first, int neednet, int net,
    int node, unsigned char *buf, size_t avail);
static int seenby_write_num(unsigned char *buf, int *used,
    size_t *avail, int num);
    

/* init - must supply the maximum number of elements that can be stored
in the seenby buffer. */

int seenby_init(seenby *snby, size_t maxelem)
{
  snby->maxelem = maxelem;
  snby->entry = malloc(maxelem * sizeof(seenby_entry));
  if (snby->entry == NULL) return (1);
  return (0);
}


/* term - terminate and free up resources */

int seenby_term(seenby *snby)
{
  free(snby->entry);
  return (0);
}


/* reset - prepare to read seen-by lines */

int seenby_reset(seenby *snby)
{
  snby->numelem = 0;
  snby->lastnet = 0;
  snby->curelem = 0;
  return (0);
}


/* addline - add a new line to the pool */

int seenby_addline(seenby *snby, const unsigned char *snbyline)
{
  int ch;
  int num;
  
  num = 0;
  for (;;snbyline++)
  {
    ch = *snbyline;
    if ((ch >= 0x30) && (ch <= 0x39))  /* between '0' and '9' */
    {
      num = num * 10 + (ch - 0x30);
    }
    else if (ch == 0x2f)  /* '/' */
    {
      snby->lastnet = num;
      num = 0;
    }
    else 
    {
      snby->entry[snby->numelem].net = snby->lastnet;
      snby->entry[snby->numelem].node = num;
      snby->numelem++;
      num = 0;
      if (ch == '\0') break;
    }
  }
  return (0);
}

  
/* sort - sort array of seen-by lines */

int seenby_sort(seenby *snby)
{
  qsort(snby->entry,
        snby->numelem,
        sizeof(seenby_entry),
        seenby_compare);
  return (0);
}


/* compare - comparison function used by sort */

static int seenby_compare(const void *l, const void *r)
{
  const seenby_entry *left = l;
  const seenby_entry *right = r;
  int ret;
      
  if (left->net == right->net)
  {
    if (left->node == right->node) ret = 0;
    else if (left->node < right->node) ret = -1;
    else ret = 1;
  }
  else if (left->net < right->net) ret = -1;
  else ret = 1;
  return (ret);
}


/* extract - extract a seen-by line */

int seenby_extract(seenby *snby, unsigned char *extract, size_t max)
{
  size_t remain = max - 1;
  int used;
  size_t upto = 0;
  int first = 1;
  int neednet;

  if (snby->curelem >= snby->numelem)
  {
    snby->curelem = 0;
    *(extract+upto) = '\0';
    return (1);
  }
  if (snby->curelem == 0) snby->lastnet = 0;
  else snby->lastnet = snby->entry[snby->curelem - 1].net;
  while (1)
  {
    if (snby->entry[snby->curelem].net != snby->lastnet)
    {
      snby->lastnet = snby->entry[snby->curelem].net;
      neednet = 1;
    }
    else neednet = 0;
    used = seenby_extract_one(first, 
                              neednet, 
                              snby->lastnet,
                              snby->entry[snby->curelem].node,
                              extract+upto,
  	                      remain);
    if (used == 0)
    {
      *(extract+upto) = '\0';
      return (0);
    }
    else
    {
      remain -= used;
      upto += used;
      snby->curelem++;
      if (snby->curelem >= snby->numelem) 
      {
        *(extract+upto) = '\0';
	return (0);
      }
    }
    first = 0;
  }   
}


/* extract_one - create one seen-by entry */

static int seenby_extract_one(int first, int neednet, int net,
    int node, unsigned char *buf, size_t avail)
{
  int used=0;
  
  if (!first)
  {
    if (avail < 1) return (0);
    *(buf+used) = 0x20;    /* ' ' */
    used++;
    avail--;
  }
  if (neednet)
  {
    if (seenby_write_num(buf+used, &used, &avail, net) != 0) return (0);
    if (avail < 1) return (0);
    *(buf+used) = 0x2f;     /* '/' */
    used++;
    avail--;
  }
  if (seenby_write_num(buf+used, &used, &avail, node) != 0) return (0);
  return (used);
}  


/* write_num - write out a single number to a buffer */

static int seenby_write_num(unsigned char *buf, int *used,
    size_t *avail, int num)
{
  unsigned char tempbuf[100];
  int x, y;
  
  x = 0;
  tempbuf[x] = 0x30 + num % 10;
  num /= 10;
  while (num > 0)
  {
    x++;
    tempbuf[x] = 0x30 + num % 10;
    num /= 10;
    if (x == 99) break;
  }
  if (*avail < (x+1)) return (1);
  for (y=0; y <= x; y++)
  {
    buf[y] = tempbuf[x-y];
  }
  *avail -= (x+1); 
  *used += (x+1);
  return (0);
}

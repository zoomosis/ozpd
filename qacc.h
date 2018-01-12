/* quick access */
/* Written by Paul Edwards, 1993.1.24 */
/* Released to the Public Domain */

#ifndef QACC
#define QACC

#include <stdio.h>

typedef struct 
{
  FILE *inf;    /* in file */
  unsigned char *origbuf; /* original buffer */
  unsigned char *buf;
  unsigned char *finnul;  /* final nul */
  unsigned char *snul;    /* small nul */
  size_t szbuf;  /* size of buffer */
  size_t hszbuf; /* half size of buffer */
  fpos_t pos;
  int noted;     /* did you do a note? */
  unsigned char *msgstart;  /* start of message */
  long numhalves;  /* number of half buffers traversed */
  unsigned char *endorig; /* end of origin line */
} qacc;


int qacc_init(qacc *qck, FILE *fp, size_t bfsz);
int qacc_term(qacc *qck);
int qacc_zoom(qacc *qck);
int qacc_half(qacc *qck);
int qacc_note(qacc *qck);
int qacc_resynch(qacc *qck);
int qacc_read(qacc *qck, void *vbuf, size_t num);
int qacc_readstr(qacc *qck, void *vbuf, size_t max);
int qacc_skipton(qacc *qck);
int qacc_readln(qacc *qck, void *vbuf, size_t sz);
int qacc_getkl(qacc *qck, void *vbuf, size_t max);
int qacc_prepare(qacc *qck);
int qacc_restore(qacc *qck);
int qacc_getorigin(qacc *qck, void *vbuf, size_t max);
int qacc_getseenby(qacc *qck, void *snbyln, size_t max);
int qacc_writemsg(qacc *qck, FILE *fq);

    
#endif

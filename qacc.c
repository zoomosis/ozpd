/* quick access */
/* Written by Paul Edwards, 1993.1.24 */
/* Released to the Public Domain */
/* This set of routines is designed to allow fast reading of
a file, by storing everything in a buffer which is directly
accessible from your program */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qacc.h"

static const unsigned char origin_str[] =
    "\x0d\x20\x2a\x20\x4f\x72\x69\x67\x69\x6e\x3a\x20";
static const unsigned char seenbystr[] =
    "\x53\x45\x45\x4e\x2d\x42\x59\x3a\x20";

/* init - must supply pointer to file to access, and how much
buffer you have available.  This should be sufficient to store
a terminating NUL character as well, so an odd number here is
usual. */

int qacc_init(qacc *qck, FILE *fp, size_t bfsz)
{
  size_t remain;

  qck->inf = fp;
  if (bfsz < 2) return (1);
  bfsz--;
  if (bfsz%2 != 0) bfsz--;
  qck->buf = (unsigned char *)malloc(bfsz+1);
  if (qck->buf == NULL) return (1);
  qck->origbuf = qck->buf;
  qck->finnul = qck->buf+bfsz;
  *qck->finnul = '\0';
  qck->szbuf = bfsz;
  qck->hszbuf = bfsz/2;
  remain = fread(qck->buf,1,qck->szbuf,qck->inf);
  qck->snul = qck->buf+remain;
  *qck->snul = '\0';
  return (0);
}


/* term - terminate and free up resources */

int qacc_term(qacc *qck)
{
  free(qck->origbuf);
  return (0);
}


/* zoom - make "buf" the first thing in the buffer, to make
maximum use of the buffer */

int qacc_zoom(qacc *qck)
{
  size_t remain;

  memmove(qck->origbuf,qck->buf,(size_t)(qck->snul-qck->buf+1));
  remain = (size_t)(qck->snul-qck->buf);
  qck->buf = qck->origbuf;
  qck->snul = qck->buf+remain;
  remain = fread(qck->snul,1,(size_t)(qck->finnul-qck->snul),qck->inf);
  qck->snul = qck->buf+remain;
  *qck->snul = '\0';
  return (0);
}


/* half - should only be used after a zoom.  Moves half the
buffer up. */

int qacc_half(qacc *qck)
{
  size_t remain;

  if (qck->buf != qck->origbuf) return (1);
  memcpy(qck->buf,qck->buf+qck->hszbuf,qck->hszbuf+1);
  *(qck->buf+qck->hszbuf+1) = '\0';
  qck->snul = qck->buf+qck->hszbuf;
  remain = fread(qck->snul,1,(size_t)(qck->finnul-qck->snul),qck->inf);
  qck->snul = qck->buf+remain;
  *qck->snul = '\0';
  return (0);
}


/* should only be used after a zoom - takes note of the
file position in order to return here when a "resynch" is
issued. */

int qacc_note(qacc *qck)
{
  fpos_t pos2;
  int ret;

  if (qck->buf != qck->origbuf) return (1);
  ret = fgetpos(qck->inf,&pos2);
  if (ret != 0) return (1);
  ret = fseek(qck->inf,qck->buf-qck->snul,SEEK_CUR);
  if (ret != 0) return (1);
  ret = fgetpos(qck->inf,&qck->pos);
  if (ret != 0) return (1);
  ret = fsetpos(qck->inf,&pos2);
  if (ret != 0) return (1);
  return (0);
}


/* return to point where "note" was issued */

int qacc_resynch(qacc *qck)
{
  int ret;

  ret = fsetpos(qck->inf,&qck->pos);
  if (ret != 0) return (1);
  qck->buf = qck->origbuf;
  return (0);
}


/* read - read specified number of bytes, which should be
no more than the size of your buffer (this restriction will
have to go sometime) */

int qacc_read(qacc *qck, void *vbuf, size_t num)
{
  size_t remain;
  unsigned char *buf;

  buf = (unsigned char *)vbuf;
  remain = (size_t)(qck->snul-qck->buf);
  if (remain < num)
  {
    qacc_zoom(qck);
    remain = (size_t)(qck->snul-qck->buf);
    if (remain < num) return (1);
  }
  memcpy(buf,qck->buf,num);
  qck->buf+=num;
  return (0);
}  


/* readstr - read a string, must find a terminating nul else
error - cannot read string bigger than buffer (temporary
limitation). */

int qacc_readstr(qacc *qck, void *vbuf, size_t max)
{
  size_t now;
  unsigned char *buf;

  buf = (unsigned char *)vbuf;
  now = strlen((char *)qck->buf);
  if (now < max)
  {
    if (qck->buf+now != qck->snul)
    {
      memcpy(buf,qck->buf,now+1);
      qck->buf+=(now+1);
      return (0);
    }
    else 
    {
      qacc_zoom(qck);
      now = strlen((char *)qck);
      if (now < max)
      {
        if (qck->buf+now == qck->snul) return (1);
        memcpy(buf,qck->buf,now+1);
        qck->buf+=(now+1);
        return (0);
      }
      else return (1);
    }
  }
  else return (1);
}


/* skipton - skip to next NUL character */

int qacc_skipton(qacc *qck)
{
  size_t now;

  now = strlen((char *)qck->buf);
  if (qck->buf+now != qck->snul)
  {
    qck->buf+=(now+1);
    return (0);
  }
  while (1)
  {
    qacc_zoom(qck);
    now = strlen((char *)qck->buf);
    if (qck->buf+now != qck->snul)
    {
      qck->buf+=(now+1);    
      return (0);
    }
    if (now == 0) return (1);
    qck->buf = qck->snul;
  }
}


/* readln - read upto a CR */

int qacc_readln(qacc *qck, void *vbuf, size_t sz)
{
  unsigned char *found;
  size_t len;

  found = (unsigned char *)strchr((char *)qck->buf,0x0d);
  if (found)
  {
    len = (size_t)(found-qck->buf);
    if (len <= sz)
    {
      memcpy(vbuf,qck->buf,len);
      *((char *)vbuf+len) = '\0';
      qck->buf = found+1;
      return (0);
    }
    else return (1);
  }
  qacc_zoom(qck);
  found = (unsigned char *)strchr((char *)qck->buf,0x0d);
  if (found)
  {
    len = (size_t)(found-qck->buf);
    if (len <= sz)
    {
      memcpy(vbuf,qck->buf,len);
      *((char *)vbuf+len) = '\0';
      qck->buf = found+1;
      return (0);
    }
    else return (1);
  }
  return (1);
}


/* getkl - get kludge line */

int qacc_getkl(qacc *qck, void *vbuf, size_t max)
{
  unsigned char *buf;
  int rc;

  buf = (unsigned char *)vbuf;
  if (*qck->buf == '\x01')
  {
    qck->buf++;
    rc = qacc_readln(qck,buf,max);
    return (rc);
  }
  if (*qck->buf != '\0') return (1);
  if (qck->buf != qck->snul) return (1);
  qacc_zoom(qck);
  if (*qck->buf == '\x01')
  {
    qck->buf++;
    rc = qacc_readln(qck,buf,max);
    return (rc);
  }
  return (1);
}


/* prepare - get ready to process a message */

int qacc_prepare(qacc *qck)
{
  size_t len;

  qck->noted = 0;
  len = strlen((char *)qck->buf);
  if ((qck->buf+len) == qck->snul)
  {
    qacc_zoom(qck);
    len = strlen((char *)qck->buf);
    if ((qck->buf+len) == qck->snul)
    {
      qacc_note(qck);
      qck->noted = 1;
      qck->numhalves = 0;
      while (((qck->buf+len) == qck->snul) && (qck->snul == qck->finnul))
      {
        qacc_half(qck);
        qck->numhalves++;
        len = strlen((char *)qck->buf);
      }
      if ((qck->buf+len) == qck->snul) return (1);
    }
  }
  if (qck->noted == 0) qck->msgstart = qck->buf;
  return (0);
}


/* restore - go back to previous "prepare" */

int qacc_restore(qacc *qck)
{
  if (qck->noted) qacc_resynch(qck);
  else qck->buf = qck->msgstart;
  return (0);
}


/* getorigin - retrieve the origin line */

int qacc_getorigin(qacc *qck, void *vbuf, size_t max)
{
  char *found1;
  char *found2;
  size_t len;

  found1 = strstr((char *)qck->buf,(char *)origin_str);
  if (found1 == NULL) return (1);
  while ((found2 = strstr(++found1,(char *)origin_str)) != NULL)
      found1 = found2;
  found2 = strchr(found1,0x0d);
  if (found2 == NULL) return (1);
  len = (size_t)(found2 - found1);
  if (len >= max) return (1);
  memcpy(vbuf,found1,len);
  *((char *)vbuf+len) = '\0';
  qck->buf = (unsigned char *)(found1+len+1);
  if (*qck->buf == 0x0a) qck->buf++;
  qck->endorig = qck->buf;
  return (0);
}


/* getseenby - read the next SEEN-BY line */

int qacc_getseenby(qacc *qck, void *snbyln, size_t max)
{
  char *found;
  size_t len;

  if (strncmp((char *)qck->buf,(char *)seenbystr,sizeof seenbystr - 1) == 0)
  {
    found = strchr((char *)qck->buf,0x0d);
    if (found == NULL) return (1);
    len = (size_t)(found - (char *)qck->buf);
    if (len >= max) return (1);
    memcpy(snbyln,qck->buf,len);
    *((char *)snbyln+len) = '\0';
    qck->buf+=(len+1);
    return (0);
  }
  return (1);
}


/* writemsg - write out the guts of the message.  Do not move
   "buf" pointer */

int qacc_writemsg(qacc *qck, FILE *fq)
{
  size_t rc, extra;
  long x,numbufs;

  if (qck->noted == 0)
  {
    rc = fwrite(qck->buf,(size_t)(qck->endorig-qck->buf),1,fq);
    if (rc == 1) return (0);
    else return (1);
  }
  else
  {
    numbufs = qck->numhalves/2;
    for (x=0L;x<numbufs;x++)
    {
      rc = fwrite(qck->buf,qck->szbuf,1,fq);
      if (rc != 1) return (1);
      rc = fread(qck->buf,1,qck->szbuf,qck->inf);
      if (rc != qck->szbuf) return (1);
    }
    if (qck->numhalves%2 == 1) extra = qck->hszbuf;
    else extra = 0;
    fwrite(qck->buf,extra+(size_t)(qck->endorig-qck->buf),1,fq);
    qacc_restore(qck);
  }
  return (1);
}

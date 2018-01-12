/* written by Paul Edwards */
/* released to the Public Domain */
/* see slidwin.c for documentation */

#ifndef SLIDWIN_INCLUDED
#define SLIDWIN_INCLUDED

#include "repo.h"

typedef struct {
    char *buf;
    size_t szbuf;
    size_t half;
    REPO *repo;
    long loc;
    size_t valbytes;
} SLIDWIN;

#define slidwinLoc(slidwin) ((slidwin)->loc)
#define slidwinVal(slidwin) ((slidwin)->valbytes)

void slidwinDefaults(SLIDWIN *slidwin);
void slidwinSetRepo(SLIDWIN *slidwin, REPO *repo);
void slidwinFinRepo(SLIDWIN *slidwin);
void slidwinSetBuf(SLIDWIN *slidwin, void *buf, size_t szbuf);
void slidwinInit(SLIDWIN *slidwin);
void slidwinTerm(SLIDWIN *slidwin);
void slidwinForward(SLIDWIN *slidwin, long amount);
void slidwinHalfF(SLIDWIN *slidwin);
void slidwinBackward(SLIDWIN *slidwin, long amount);
void slidwinHalfB(SLIDWIN *slidwin);
void slidwinSeek(SLIDWIN *slidwin, long offset);

#endif

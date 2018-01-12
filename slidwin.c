/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  slidwin - sliding window                                         */
/*                                                                   */
/*  note that it is not a good idea to play silly buggers with       */
/*  these routines.  Specifically going forward when you've already  */
/*  hit the end, going "forward" by negative amounts, etc.           */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>

#include "slidwin.h"
#include "error.h"
#include "unused.h"

void slidwinDefaults(SLIDWIN *slidwin)
{
    unused(slidwin);
    return;
}

void slidwinSetBuf(SLIDWIN *slidwin, void *buf, size_t szbuf)
{
    slidwin->buf = buf;
    slidwin->szbuf = szbuf;
    slidwin->half = szbuf / 2;
    return;
}

void slidwinInit(SLIDWIN *slidwin)
{
    unused(slidwin);
    return;
}

void slidwinTerm(SLIDWIN *slidwin)
{
    unused(slidwin);
    return;
}

void slidwinSetRepo(SLIDWIN *slidwin, REPO *repo)
{
    slidwin->repo = repo;
    repoOpen(slidwin->repo);
    if (ALLOK)
    {
        slidwin->valbytes = repoRead(slidwin->repo, 
                                     slidwin->buf, 
                                     slidwin->szbuf);
    }
    slidwin->loc = 0;
}

void slidwinFinRepo(SLIDWIN *slidwin)
{
    repoClose(slidwin->repo);
    slidwin->loc = 0;
}

void slidwinForward(SLIDWIN *slidwin, long amount)
{
    if (amount > slidwin->szbuf)
    {
        slidwin->loc = slidwin->loc + amount;
        repoSeek(slidwin->repo, slidwin->loc);
        if (errorCompare(REPO_INVLOC))
        {
            errorClear();
            slidwin->valbytes = 0;
        }
        else
        {
            slidwin->valbytes = repoRead(slidwin->repo, 
                                         slidwin->buf, 
                                         slidwin->szbuf);
        }
    }
    else if (amount == slidwin->szbuf)
    {
        slidwin->loc += amount;
        slidwin->valbytes = repoRead(slidwin->repo, 
                                     slidwin->buf, 
                                     slidwin->szbuf);
    }
    else if (amount < slidwin->szbuf)
    {
        slidwin->loc += amount;
        slidwin->valbytes = slidwin->szbuf - (size_t)amount;
        memmove(slidwin->buf, 
                slidwin->buf + (size_t)amount, 
                slidwin->valbytes);
        slidwin->valbytes += repoRead(slidwin->repo,
                                      slidwin->buf + slidwin->valbytes,
                                      (size_t)amount);
    }
    return;
}

void slidwinHalfF(SLIDWIN *slidwin)
{
    slidwinForward(slidwin, slidwin->half);
    return;
}

void slidwinBackward(SLIDWIN *slidwin, long amount)
{
    slidwin->loc = slidwin->loc - amount;
    repoSeek(slidwin->repo, slidwin->loc);
    if (errorCompare(REPO_INVLOC))
    {
        errorClear();
        slidwin->valbytes = 0;
    }
    else
    {
        slidwin->valbytes = repoRead(slidwin->repo, 
                                     slidwin->buf, 
                                     slidwin->szbuf);
    }
    return;
}

void slidwinHalfB(SLIDWIN *slidwin)
{
    slidwinBackward(slidwin, slidwin->half);
    return;
}

void slidwinSeek(SLIDWIN *slidwin, long offset)
{
    slidwin->loc = offset;
    repoSeek(slidwin->repo, slidwin->loc);
    if (errorCompare(REPO_INVLOC))
    {
        errorClear();
        slidwin->valbytes = 0;
    }
    else
    {
        slidwin->valbytes = repoRead(slidwin->repo, 
                                     slidwin->buf, 
                                     slidwin->szbuf);
    }
    return;
}

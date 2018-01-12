/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  repo - repositioning functions                                   */
/*                                                                   */
/*********************************************************************/

#include "repo.h"
#include "unused.h"

/* The following functions are designed to be used by applications */

void repoOpen(REPO *repo)
{
    repo->openFnc(repo->vstr);
    return;
}

void repoClose(REPO *repo)
{
    repo->closeFnc(repo->vstr);
    return;
}

void repoSeek(REPO *repo, long offset)
{
    repo->seekFnc(repo->vstr, offset);
    return;
}

size_t repoRead(REPO *repo, char *buf, size_t amount)
{
    size_t bytes;
    
    bytes = repo->readFnc(repo->vstr, buf, amount);
    return (bytes);
}

/* The following functions are meant to be used by the setup
   code */
   
void repoDefaults(REPO *repo)
{
    unused(repo);   
    return;
}

void repoInit(REPO *repo)
{
    unused(repo);
    return;
}
   
void repoTerm(REPO *repo)
{
    unused(repo);
    return;
}

void repoSetV(REPO *repo, void *vstr)
{
    repo->vstr = vstr;
    return;
}
   
void repoSetOpen(REPO *repo, void (*openFnc)(void *vstr))
{
    repo->openFnc = openFnc;
    return;
}

void repoSetClose(REPO *repo, void (*closeFnc)(void *vstr))
{
    repo->closeFnc = closeFnc;
    return;
}

void repoSetSeek(REPO *repo, void (*seekFnc)(void *vstr, long offset))
{
    repo->seekFnc = seekFnc;
    return;
}

void repoSetRead(REPO *repo, size_t (*readFnc)(void *, char *, size_t))
{
    repo->readFnc = readFnc;
    return;
}

/* written by Paul Edwards */
/* released to the Public Domain */
/* see repo.c for documentation */

#ifndef REPO_INCLUDED
#define REPO_INCLUDED

#include <stddef.h>

typedef struct {
    void *vstr;
    void (*openFnc)(void *vstr);
    void (*closeFnc)(void *vstr);
    void (*seekFnc)(void *vstr, long offset);
    size_t (*readFnc)(void *vstr, char *buf, size_t szbuf);
} REPO;

#define REPO_INVLOC "REP001 invalid location to seek to (%ld)\n"

void repoOpen(REPO *repo);
void repoClose(REPO *repo);
void repoSeek(REPO *repo, long offset);
size_t repoRead(REPO *repo, char *buf, size_t amount);
void repoDefaults(REPO *repo);
void repoInit(REPO *repo);
void repoTerm(REPO *repo);
void repoSetV(REPO *repo, void *vstr);
void repoSetOpen(REPO *repo, void (*openFnc)(void *vstr));
void repoSetClose(REPO *repo, void (*closeFnc)(void *vstr));
void repoSetSeek(REPO *repo, void (*seekFnc)(void *vstr, long offset));
void repoSetRead(REPO *repo, size_t (*readFnc)(void *, char *, size_t));

#endif

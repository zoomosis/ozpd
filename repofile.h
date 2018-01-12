/* written by Paul Edwards */
/* released to the Public Domain */
/* see repofile.c for documentation */

#ifndef REPOFILE_INCLUDED
#define REPOFILE_INCLUDED

#include <stddef.h>

#include "estdio.h"
#include "repo.h"

typedef struct {
    REPO repo;
    EFILE fp;
    char *filename;
} REPOFILE;

#define repofileGetRepo(repofile) (&((repofile)->repo))

void repofileDefaults(REPOFILE *repofile);
void repofileInit(REPOFILE *repofile);
void repofileTerm(REPOFILE *repofile);
void repofileSetFile(REPOFILE *repofile, char *filename);
void repofileOpen(void *vstr);
void repofileClose(void *vstr);
void repofileSeek(void *vstr, long offset);
size_t repofileRead(void *vstr, char *buf, size_t szbuf);

#endif

/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  repofile - repositioning functions for a standard file           */
/*                                                                   */
/*********************************************************************/

#include "repofile.h"
#include "repo.h"
#include "unused.h"
#include "error.h"

void repofileDefaults(REPOFILE *repofile)
{
    repoDefaults(&repofile->repo);
    return;
}

void repofileInit(REPOFILE *repofile)
{
    repoInit(&repofile->repo);
    if (ALLOK)
    {
        repoSetV(&repofile->repo, (void *)repofile);
        repoSetOpen(&repofile->repo, repofileOpen);
        repoSetClose(&repofile->repo, repofileClose);
        repoSetSeek(&repofile->repo, repofileSeek);
        repoSetRead(&repofile->repo, repofileRead);
    }
    return;
}

void repofileTerm(REPOFILE *repofile)
{
    repoTerm(&repofile->repo);
    return;
}

void repofileSetFile(REPOFILE *repofile, char *filename)
{
    repofile->filename = filename;
    return;
}

void repofileOpen(void *vstr)
{
    REPOFILE *repofile = (REPOFILE *)vstr;
    
    efileOpen(&repofile->fp, repofile->filename, "rb");
    return;
}

void repofileClose(void *vstr)
{
    REPOFILE *repofile = (REPOFILE *)vstr;
    
    efileClose(&repofile->fp);
    return;
}

void repofileSeek(void *vstr, long offset)
{
    REPOFILE *repofile = (REPOFILE *)vstr;
    
    efileSeek(&repofile->fp, offset, SEEK_SET);
    if (errorCompare(EFILE_SEEK))
    {
        errorClear();
        errorSet(REPO_INVLOC, offset);
    }
    return;
}

size_t repofileRead(void *vstr, char *buf, size_t szbuf)
{
    REPOFILE *repofile = (REPOFILE *)vstr;
    size_t bytes;
    
    bytes = efileRead(&repofile->fp, buf, 1, szbuf);
    return (bytes);
}

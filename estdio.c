/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*  Written Jan 1994                                                 */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  estdio.c - stdio replacement functions                           */
/*                                                                   */
/*  Refer to estdio.h for documentation.                             */
/*                                                                   */
/*********************************************************************/

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "error.h"
#include "estdio.h"

void efileOpen(EFILE *efile, const char *fileName, const char *mode)
{
    if (strlen(fileName) > FILENAME_MAX)
    {
        errorSet(EFILE_FNM_TOO_LONG, fileName);
    }
    else
    {
        strcpy(efile->fileName, fileName);
        errno = 0;
        efile->filePtr = fopen(efile->fileName, mode);
        if (efile->filePtr == NULL)
        {
            errorSet(EFILE_OPEN, 
                     efile->fileName, 
                     mode, 
                     errno, 
                     strerror(errno));
        }
    }
    return;
}
        
void efileClose(EFILE *efile)
{
    int ret;
    
    errno = 0;
    ret = fclose(efile->filePtr);
    if (ret != 0)
    {
        errorSet(EFILE_CLOSE, efile->fileName, errno, strerror(errno));
    }
    return;
}

void efileReopen(EFILE *newFile, 
                 const char *newName, 
                 const char *mode, 
                 EFILE *oldFile)
{
    efileClose(oldFile);
    if (ALLOK)
    {
        efileOpen(newFile, newName, mode);
    }
    return;
}

void efileSetVbuf(EFILE *efile, char *buf, int mode, size_t szbuf)
{
    errno = 0;
    if (setvbuf(efile->filePtr, buf, mode, szbuf) != 0)
    {
        errorSet(EFILE_SETVBUF, 
                 efile->fileName, 
                 (unsigned long)szbuf, 
                 errno,
                 strerror(errno));
    }
    return;
}

int efilePrintf(EFILE *efile, const char *fmt, ...)
{
    va_list va;
    int ret;
    
    va_start(va, fmt);
    ret = vfprintf(efile->filePtr, fmt, va);
    if (ret < 0)
    {
        errorSet(EFILE_WRITE, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}

int efileVprintf(EFILE *efile, const char *fmt, va_list va)
{
    int ret;
    
    errno = 0;
    ret = vfprintf(efile->filePtr, fmt, va);
    if (ret < 0)
    {
        errorSet(EFILE_WRITE, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}

int efileFgetc(EFILE *efile)
{
    int ret;
    
    errno = 0;
    ret = fgetc(efile->filePtr);
    if (ret == EOF)
    {
        if (ferror(efile->filePtr))
        {
            errorSet(EFILE_READ, efile->fileName, errno, strerror(errno));
        }
    }
    return (ret);
}

void efileFlush(EFILE *efile)
{
    errno = 0;
    if (fflush(efile->filePtr) != 0)
    {
        errorSet(EFILE_FLUSH, efile->fileName, errno, strerror(errno));
    }
    return;
}
    
char *efileGets(EFILE *efile, char *buf, int szbuf)
{
    char *ptr;
    
    errno = 0;
    ptr = fgets(buf, szbuf, efile->filePtr);
    if (ptr == NULL)
    {
        if (ferror(efile->filePtr))
        {
            errorSet(EFILE_READ, efile->fileName, errno, strerror(errno));
        }
    }
    return (ptr);
}
        
int efileFputc(EFILE *efile, int ch)
{
    int ret;
    
    errno = 0;
    ret = fputc(ch, efile->filePtr);
    if (ret == EOF)
    {
        errorSet(EFILE_WRITE, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}

void efilePuts(EFILE *efile, const char *buf)
{
    errno = 0;
    if (fputs(buf, efile->filePtr) == EOF)
    {
        errorSet(EFILE_WRITE, efile->fileName, errno, strerror(errno));
    }
    return;
}
    
int efileUngetc(EFILE *efile, int ch)
{
    int ret;
    
    ret = fputc(ch, efile->filePtr);
    if (ret == EOF)
    {
        errorSet(EFILE_UNGET, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}

size_t efileRead(EFILE *efile, void *buf, size_t sz, size_t numelem)
{
    size_t ret;
    
    errno = 0;
    ret = fread(buf, sz, numelem, efile->filePtr);
    if (ferror(efile->filePtr))
    {
        errorSet(EFILE_READ, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}

size_t efileWrite(EFILE *efile, const void *buf, size_t sz, size_t numelem)
{
    size_t ret;

    errno = 0;    
    ret = fwrite(buf, sz, numelem, efile->filePtr);
    if (ret < numelem)
    {
        errorSet(EFILE_WRITE, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}
    
void efileGetPos(EFILE *efile, fpos_t *pos)
{
    errno = 0;
    if (fgetpos(efile->filePtr, pos) != 0)
    {
        errorSet(EFILE_GPOS, efile->fileName, errno, strerror(errno));
    }
    return;
}

void efileSeek(EFILE *efile, long offset, int type)
{
    errno = 0;
    if (fseek(efile->filePtr, offset, type) != 0)
    {
        errorSet(EFILE_SEEK, efile->fileName, errno, strerror(errno));
    }
    return;
}

void efileSetPos(EFILE *efile, const fpos_t *pos)
{
    errno = 0;
    if (fsetpos(efile->filePtr, pos) != 0)
    {
        errorSet(EFILE_SPOS, efile->fileName, errno, strerror(errno));
    }
    return;
}

long efileTell(EFILE *efile)
{
    long ret;
    
    errno = 0;
    ret = ftell(efile->filePtr);
    if (ret == -1L)
    {
        errorSet(EFILE_TELL, efile->fileName, errno, strerror(errno));
    }
    return (ret);
}
    
void efileRewind(EFILE *efile)
{
    rewind(efile->filePtr);
}

int efileEOF(EFILE *efile)
{
    return (feof(efile->filePtr));
}

void eremove(const char *fileName)
{
    errno = 0;
    if (remove(fileName) != 0)
    {
        errorSet(EREMOVE, fileName, errno, strerror(errno));
    }
    return;
}

void erename(const char *fileName1, const char *fileName2)
{
    errno = 0;
    if (rename(fileName1, fileName2) != 0)
    {
        errorSet(ERENAME, fileName1, fileName2, errno, strerror(errno));
    }
    return;
}

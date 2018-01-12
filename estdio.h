/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet          */
/*  Released to the Public Domain                                    */
/*  Written Jan 1994                                                 */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  estdio.h - functions from stdio with error handling              */
/*                                                                   */
/*  The routines seen here are replacements for the ones in          */
/*  ISO/IEC 9899:1990.  The difference is that these all set error   */
/*  codes, using the error handling strategy (see error.h), and also */
/*  the object (e.g. EFILE) always comes first.                      */
/*  The use of these functions is obvious to anyone familiar with    */
/*  the ISO C standard.                                              */
/*                                                                   */
/*********************************************************************/

#ifndef ESTDIO_INCLUDED
#define ESTDIO_INCLUDED

#include <stdio.h>
#include <stdarg.h>

#define EFILE_OPEN "EFL001 Failed to open file %s in mode %s - errno " \
                   "is %d, error string is %s\n"
#define EFILE_FNM_TOO_LONG "EFL002 Filename %s is too long\n"                   
#define EFILE_EOF "EFL003 Unexpected end-of-file on %s\n"
#define EFILE_CLOSE "EFL004 Failed to close file %s - errno is %d, " \
                    "error string is %s\n"
#define EFILE_WRITE "EFL005 Failed to write to file %s - errno is %d, " \
                    "error string is %s\n"
#define EFILE_READ "EFL006 Failed to read from file %s - errno is %d, " \
                    "error string is %s\n"
#define EFILE_SETVBUF "EFL007 Failed to do allocate file buffers for " \
                      "file %s with allocation of %lu bytes - errno " \
                      "is %d, error string is %s\n"
#define EFILE_UNGET "EFL008 Failed to unget character on file %s - " \
                    "errno is %d, error string is %s\n"
#define EFILE_GPOS "EFL009 Failed to determine position of file %s - " \
                   "errno is %d, error string is %s\n"                    
#define EFILE_SEEK "EFL010 Failed to position file %s - " \
                   "errno is %d, error string is %s\n"                   
#define EFILE_SPOS "EFL011 Failed to position file %s - " \
                   "errno is %d, error string is %s\n"                    
#define EFILE_TELL "EFL012 Failed to determine position of file %s - " \
                   "errno is %d, error string is %s\n"
#define EFILE_FLUSH "EFL013 Failed to flush file %s - errno is %d " \
                    "error string is %s\n"
#define EREMOVE "SDI001 Failed to remove file %s - errno is %d " \
                "error string is %s\n"
#define ERENAME "SDI002 Failed to rename file %s to %s - errno is %d " \
                "error string is %s\n"

typedef struct {
    char fileName[FILENAME_MAX];
    FILE *filePtr;
} EFILE;

#define efileFileName(x) x->fileName
#define efileFilePtr(x) x->filePtr
void efileOpen(EFILE *efile, const char *fileName, const char *mode);
void efileClose(EFILE *efile);
void efileReopen(EFILE *newFile, 
                 const char *newName, 
                 const char *mode, 
                 EFILE *oldFile);
void efileSetVbuf(EFILE *efile, char *buf, int mode, size_t szbuf);
int efilePrintf(EFILE *efile, const char *fmt, ...);
int efileVprintf(EFILE *efile, const char *fmt, va_list va);
int efileFgetc(EFILE *efile);
void efileFlush(EFILE *efile);
char *efileGets(EFILE *efile, char *buf, int szbuf);
int efileFputc(EFILE *efile, int ch);
void efilePuts(EFILE *efile, const char *buf);
int efileUngetc(EFILE *efile, int ch);
size_t efileRead(EFILE *efile, void *buf, size_t sz, size_t numelem);
size_t efileWrite(EFILE *efile, const void *buf, size_t sz, size_t numelem);
void efileGetPos(EFILE *efile, fpos_t *pos);
void efileSeek(EFILE *efile, long offset, int type);
void efileSetPos(EFILE *efile, const fpos_t *pos);
long efileTell(EFILE *efile);
void efileRewind(EFILE *efile);
int efileEOF(EFILE *efile);
void eremove(const char *fileName);
void erename(const char *fileName1, const char *fileName2);

#endif


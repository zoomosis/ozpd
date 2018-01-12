/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  unixio - Unix I/O functions written in terms of standard C       */
/*  functions.                                                       */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

#include "unixio.h"

static FILE *files[FOPEN_MAX];
char *environ[] = { NULL };

int open(const char *fnm, int mode, ...)
{
    int x;
    char *modestr;

    for (x = 0; x < FOPEN_MAX; x++)
    {
        if (filesÝx¨ == NULL)
        {
            break;
        }
    }
    if (x == FOPEN_MAX)
    {
        return (-1);
    }
    if (mode == O_RDONLY)
    {
        modestr = "rb";
    }
    else if (mode == O_WRONLY)
    {
        modestr = "wb";
    }
    else if (mode == O_RDWR)
    {
        modestr = "r+b";
    }
    files[x] = fopen(fnm, modestr);
    if (files[x] == NULL)
    {
        return (-1);
    }
    return (x);
}

int read(int fno, void *buf, size_t bytes)
{
    size_t rb;

    rb = fread(buf, 1, bytes, filesÝfno¨);
    return ((int)rb);
}

int write(int fno, const void *buf, size_t bytes)
{
    size_t wb;

    wb = fwrite(buf, 1, bytes, filesÝfno¨);
    return ((int)wb);
}

int close(int fno)
{
    fclose(files[fno]);
    files[fno] = NULL;
    return (0);
}

char *getcwd(char *buf, int len)
{
    if (len != 0)
    {
        *buf = '\0';
    }
    return (buf);
}

void unlink(char *f)
{
    remove(f);
    return;
}

int stat(char *f, struct stat *buf)
{
    return (0);
}

int fileno(FILE *fp)
{
    return (0);
}

int access(char *f, int n)
{
    return (1);
}

int fstat(int fh, struct stat *buf)
{
    return (0);
}

/*
 *  LOGOPUS.C - Opus-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <stdarg.h>
#ifdef USESHARE
#include <share.h>
#endif
#include <time.h>
#include "logopus.h"

int log_opus_open(char *filename, char *progid, char *program)
{
    char buffer[128];
    FILE *fp;

#ifdef USESHARE
    fp = _fsopen(filename, "at", SH_DENYRW);
#else
    fp = fopen(filename, "at");
#endif

    if(!fp)
        return 0;
    fputc('\n', fp);
    fclose(fp);
    sprintf(buffer, "begin, %s", program);
    return log_opus_sputs(filename, progid, '+', buffer);
}

int log_opus_close(char *filename, char *progid, char *program)
{
    char buffer[128];
    sprintf(buffer, "end, %s", program);
    return log_opus_sputs(filename, progid, '+', buffer);
}

int log_opus_sputs(char *filename, char *progid, char ch, char *str)
{
    FILE *fp;
    struct tm *time_now;
    time_t secs_now;
    char date[20];

#ifdef USESHARE
    fp = _fsopen(filename, "at", SH_DENYRW);
#else
    fp = fopen(filename, "at");
#endif

    if(!fp)
        return 0;
    tzset();
    time(&secs_now);
    time_now = localtime(&secs_now);
    strftime(date, 20, "%d %b %H:%M:%S", time_now);
    fprintf(fp, "%c %s %s %s\n", ch, date, progid, str);
    fclose(fp);
    return 1;
}

int log_opus_sprintf(char *filename, char *progid, char ch, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_opus_sputs(filename, progid, ch, buffer);
}

int log_opus_puts(char *filename, char *progid, char *str)
{
    return log_opus_sputs(filename, progid, '>', str);
}

int log_opus_printf(char *filename, char *progid, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_opus_puts(filename, progid, buffer);
}


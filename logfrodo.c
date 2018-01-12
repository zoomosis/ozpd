/*
 *  LOGFRODO.C - FrontDoor-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <stdarg.h>
#ifdef USESHARE
#include <share.h>
#endif
#include <time.h>
#include "logfrodo.h"

int log_frodo_open(char *filename, char *program)
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
    strftime(date, 20, "%a %d %b %y", time_now);
    fprintf(fp, "\n----------  %s, %s\n", date, program);
    fclose(fp);
    return 1;
}

int log_frodo_sputs(char *filename, char ch, char *str)
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
    strftime(date, 20, "%H:%M:%S", time_now);
    fprintf(fp, "%c %s  %s\n", ch, date, str);
    fclose(fp);
    return 1;
}

int log_frodo_sprintf(char *filename, char ch, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_frodo_sputs(filename, ch, buffer);
}

int log_frodo_puts(char *filename, char *str)
{
    return log_frodo_sputs(filename, '>', str);
}

int log_frodo_printf(char *filename, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_frodo_puts(filename, buffer);
}


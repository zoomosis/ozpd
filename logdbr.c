/*
 *  LOGDBR.C - D'Bridge-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <stdarg.h>
#ifdef USESHARE
#include <share.h>
#endif
#include <time.h>
#include "logdbr.h"

int log_dbr_puts(char *filename, char *str)
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
    strftime(date, 20, "%m/%d/%y %H:%M", time_now);
    fprintf(fp, "%s  %s\n", date, str);
    fclose(fp);
    return 1;
}

int log_dbr_printf(char *filename, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_dbr_puts(filename, buffer);
}


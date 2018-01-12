/*
 *  LOGSTAT.C - Status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <stdarg.h>
#include "logdbr.h"
#include "logfrodo.h"
#include "logopus.h"
#include "logstat.h"
#include "unused.h"

int log_stat_open(char *filename, int style, char *prod_short, char *prod_long)
{
    switch (style) {
    case LOG_STY_DBR:
        return 1;
    case LOG_STY_FRODO:
        return log_frodo_open(filename, prod_long);
    case LOG_STY_OPUS:
        return log_opus_open(filename, prod_short, prod_long);
    default:
        return -1;
    }
}

int log_stat_close(char *filename, int style, char *prod_short, char *prod_long)
{
    switch (style) {
    case LOG_STY_DBR:
    case LOG_STY_FRODO:
        return 1;
    case LOG_STY_OPUS:
        return log_opus_close(filename, prod_short, prod_long);
    default:
        return -1;
    }
}

int log_stat_puts(char *filename, int style, char *prod_short, char *prod_long, char *str)
{
    unused(prod_long);
    switch (style) {
    case LOG_STY_DBR:
        return log_dbr_puts(filename, str + 1);
    case LOG_STY_FRODO:
        return log_frodo_sputs(filename, *str, str + 1);
    case LOG_STY_OPUS:
        return log_opus_sputs(filename, prod_short, *str, str + 1);
    default:
        return -1;
    }
}

int log_stat_printf(char *filename, int style, char *prod_short, char *prod_long, const char *format,...)
{
    va_list args;
    char buffer[512];
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    return log_stat_puts(filename, style, prod_short, prod_long, buffer);
}

#ifdef TEST_LOGSTAT

void munch(int style)
{
    log_stat_open("logstat.log", style, "LOGSTAT", "Logstat");
    log_stat_printf("logstat.log", style, "LOGSTAT", "Logstat",
      "!User ate nice juicy carrot");
    log_stat_close("logstat.log", style, "LOGSTAT", "Logstat");
}

int main(void)
{
    munch(LOG_STY_DBR);
    munch(LOG_STY_FRODO);
    munch(LOG_STY_OPUS);
    return 0;
}

#endif


/*
 *  LOGSTAT.H - Status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#ifndef __LOGSTAT_H__
#define __LOGSTAT_H__

#define LOG_STY_DBR    1
#define LOG_STY_FRODO  2
#define LOG_STY_OPUS   3

int log_stat_open(char *filename, int style, char *prod_short, char *prod_long);
int log_stat_close(char *filename, int style, char *prod_short, char *prod_long);
int log_stat_puts(char *filename, int style, char *prod_short, char *prod_long, char *str);
int log_stat_printf(char *filename, int style, char *prod_short, char *prod_long, const char *format,...);

#endif


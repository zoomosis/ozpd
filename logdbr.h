/*
 *  LOGDBR.H - D'Bridge-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#ifndef __LOGDBR_H__
#define __LOGDBR_H__

int log_dbr_puts(char *filename, char *str);
int log_dbr_printf(char *filename, const char *format,...);

#endif


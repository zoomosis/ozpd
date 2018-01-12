/*
 *  LOGOPUS.H - Opus-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#ifndef __LOGOPUS_H__
#define __LOGOPUS_H__

int log_opus_open(char *filename, char *progid, char *program);
int log_opus_close(char *filename, char *progid, char *program);
int log_opus_sputs(char *filename, char *progid, char ch, char *str);
int log_opus_sprintf(char *filename, char *progid, char ch, const char *format,...);
int log_opus_puts(char *filename, char *progid, char *str);
int log_opus_printf(char *filename, char *progid, const char *format,...);

#endif


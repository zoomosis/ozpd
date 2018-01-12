/*
 *  LOGFRODO.H - FrontDoor-style status logging routines.
 *  Written 1994,95,96 by Andrew Clarke and released to the public domain.
 */

#ifndef __LOGFRODO_H__
#define __LOGFRODO_H__

int log_frodo_open(char *filename, char *program);
int log_frodo_sputs(char *filename, char ch, char *str);
int log_frodo_sprintf(char *filename, char ch, const char *format,...);
int log_frodo_puts(char *filename, char *str);
int log_frodo_printf(char *filename, const char *format,...);

#endif


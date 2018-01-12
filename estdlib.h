/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet          */
/*  Released to the Public Domain                                    */
/*  Written Jan 1994                                                 */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  estdlib.h - functions from stdlib with error handling            */
/*                                                                   */
/*  The routines seen here are replacements for the ones in          */
/*  ISO/IEC 9899:1990.  The difference is that these all set error   */
/*  codes, using the error handling strategy (see error.h).          */
/*  The use of these functions is obvious to anyone familiar with    */
/*  the ISO C standard.                                              */
/*                                                                   */
/*********************************************************************/

#ifndef ESTDLIB_INCLUDED
#define ESTDLIB_INCLUDED

#include <stddef.h>

#define ERROR_MEMORY "ESL001 Insufficient memory allocating %lu bytes\n"

void *emalloc(size_t sz);
void efree(void *buf);

#endif


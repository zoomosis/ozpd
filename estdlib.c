/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*  Written Jan 1994                                                 */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  estdlib.c - stdlib replacement functions                         */
/*                                                                   */
/*  Refer to estdlib.h for documentation.                            */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>

#include "error.h"
#include "estdlib.h"

void *emalloc(size_t sz)
{
    void *ptr;
    
    ptr = malloc(sz);
    if ((ptr == NULL) && (sz != 0))
    {
        errorSet(ERROR_MEMORY, (unsigned long)sz);
    }
    return (ptr);
}

void efree(void *buf)
{
    free(buf);
    return;
}

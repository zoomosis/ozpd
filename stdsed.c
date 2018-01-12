/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program will seed the random number generator with a value  */
/*  worked out using the time.                                       */
/*                                                                   */
/*********************************************************************/

#include <time.h>
#include <limits.h>
#include <stdlib.h>

#include "stdsed.h"

int stdsed(void)
{
    time_t tt;
    struct tm *tms;
    long seed;
    
    time(&tt);
    tms = localtime(&tt);
    seed = tms->tm_yday;
    seed = seed * 24 + tms->tm_hour;
    seed = seed * 60 + tms->tm_min;
    seed = seed * 60 + tms->tm_sec;
    srand((unsigned int)(seed%UINT_MAX));
    return (0);
}

/*
 *  UNIXTIME.C
 *
 *  Returns the number of seconds since midnight on January 1, 1970.
 *  Written in October 1996 by Andrew Clarke.
 *  Released to the public domain.
 */

#include <stdio.h>
#include <time.h>

int isleap(int year)
{
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

#define UNIX_EPOCH 1970

unsigned long unixtime(const struct tm *tm)
{
    int year, i;
    unsigned long result;

    result = 0UL;
    year = tm->tm_year + 1900;

    /* Traverse through each year */
    for (i = UNIX_EPOCH; i < year; i++)
    {
        result += 31536000UL;  /* 60s * 60m * 24h * 365d = 31536000s */
        if (isleap(i))
        {
            /* It was a leap year; add a day's worth of seconds */
            result += 86400UL;  /* 60s * 60m * 24h = 86400s */
        }
    }

    /* Traverse through each day of the year, adding a day's worth
     * of seconds each time. */
    for (i = 0; i < tm->tm_yday; i++)
    {
        result += 86400UL;  /* 60s * 60m * 24h = 86400s */
    }

    /* Now add the number of seconds remaining */
    result += 3600UL * tm->tm_hour;
    result += 60UL * tm->tm_min;
    result += (unsigned long) tm->tm_sec;

    return result;
}

/* Written by Roy McNeill */
/* Released to the public domain */
/* Modified by Paul Edwards */
/* Set i to the number you want the reciprocal of */

#define PLACES 501

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    long i, j, top;
    int place;
    char ans[PLACES + 1], buf[40];

    i = 300;
    top = 1;
    for (place = 0; place < PLACES; place++)
    {
        j = top / i;            /* divide */
        sprintf(buf, "%d", j);  /* convert to string. Answer should be 1
                                   digit. */
        ans[place] = *buf;      /* add digit to big answer */
        top = top - j * i;      /* get remainder */
        top *= 10;              /* shift one decimal place */
    }
    ans[PLACES] = '\0';
    printf("ans is %s\n", ans);
    return 0;
}

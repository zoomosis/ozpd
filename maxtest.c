/* Test maximum number of files that can be opened */
/* Written by Paul Edwards and released to the public domain */

#include <stdio.h>

int main(void)
{
    int x;
    char fnm[FILENAME_MAX];
    FILE *fq;

    printf("ISO guarantees at least %d\n", FOPEN_MAX - 3);    
    for (x = 1; x <= 200; x++)
    {
        sprintf(fnm, "maxxx.%d", x);
        fq = fopen(fnm, "w");
        if (fq == NULL)
        {
            printf("max is %d\n", x - 1);
            return (0);
        }
    }
    printf("max is at least %d\n", x - 1);
    return (0);
}


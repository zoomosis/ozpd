/* This code is released to the public domain */
/* Written by Paul Markham, fidonet 3:711/934.1 */
/* With slight mods by Paul Edwards */
/* You must remove all hyphens etc when using this routine */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE    1
#define FALSE   0

char ISBN_compute(char *isbn);
int  ISBN_validate(char *isbn);
int  ISBN_sum(char *isbn, int len);

int main(void)
    {
    char isbn[10];

    strcpy(isbn, "188357725");
    printf("%s%c\n", isbn, ISBN_compute(isbn)); /* should be X */
    printf("188357725X - %d\n", ISBN_validate("188357725X")); /* true */
    printf("1883577251 - %d\n", ISBN_validate("1883577251")); /* false */

    strcpy(isbn, "000648019");
    printf("%s%c\n", isbn, ISBN_compute(isbn)); /* should be 5 */
    printf("0006480195 - %d\n", ISBN_validate("0006480195")); /* true */
    printf("000648019X - %d\n", ISBN_validate("000648019X")); /* false */

    return EXIT_SUCCESS;
    }


char ISBN_compute(char *isbn)
    {
    int     digit;
    char    rc;

    digit = 10 - ((ISBN_sum(isbn, strlen(isbn)) - 1) % 11);
    if (digit == 10)
        rc = 'X';
    else
        rc = (char)('0' + digit);

    return rc;
    }

int ISBN_validate(char *isbn)
    {
    int     rc;
    int     sum;
    int     len;
    char    digit;

    len = strlen(isbn);

    sum = ISBN_sum(isbn, len - 1);
    digit = isbn[len - 1];
    if (digit == 'X')
        sum += 10;
    else
        sum += digit - '0';

    if (sum % 11 == 0)
        rc = TRUE;
    else
        rc = FALSE;

    return rc;
    }

int ISBN_sum(char *isbn, int len)
    {
    int digit;
    int sum;
    int i;

    sum = 0;
    for (i = 0; i < len; i++)
        {
        digit = isbn[i] - '0';
        sum += digit * (len - i + 1);
        }
    return sum;
    }

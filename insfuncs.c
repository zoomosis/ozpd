/*
   ** int Ins_Word(char*,char*,char*,size_t);
   ** int Ins_Str(char*,char*,char*,size_t);
   **
   ** Public domain by Frank Adam, 1996.
   **
   ** Ins_Str will insert a string into a string at a given location
   ** Ins_Word will normalize a word or string insertion, by adding
   ** (or not) necessary spaces.
   ** In both cases the caller is responsible to provide adequate space
   ** for the string and the inserted string, and the maximum allowed length
   ** of the resulting string must be passed on to the functions.
   ** With InsWord to have an insertion start or end with space 2 spaces
   ** are required. Eg.: "  Hello" | "Hello  ". Or just use StrIns().
   ** Both functions return success as a positive number ( strlen(text) ).
   ** 0 if encountered a NULL, and a negative number indicating the required
   ** number of bytes needed, if the buffer was not large enough.
   ** InsChar returns TRUE or FALSE, and assumes there is room for 1 byte.
 */


#include<string.h>
#include<stdlib.h>

#include "insfuncs.h"

#ifdef TEST

#include<stdio.h>

main()
{
    char *test1 = (char *)malloc(12);
    char test2[32];
    char *number = "5";
    int bytes;

    strcpy(test1, "123467890");
    printf("\n%s  ---> %d bytes", test1, strlen(test1));
    bytes = InsStr(test1, &test1[4], number, 11);
    printf("\n%s ---> %d returned", test1, bytes);
    free(test1);

    strcpy(test2, "Will the Windies win ?");
    printf("\n%s      ---> %d bytes", test2, strlen(test2));
    bytes = InsWord(test2, strstr(test2, "win"), "ever", strlen(test2) + 1 + 4 + 1 + 1);
    printf("\n%s ---> %d returned", test2, bytes);  /* sp + 4chars + sp +
                                                       \0 */

    InsChar(&test2[17], 'n');
    printf("\n%s ---> %d bytes", test2, ++bytes);

    return 0;
}
#endif

int InsChar(char *point, char ch)
{
    if (!point || !*point)
        return 0;
    memmove(point + 1, point, strlen(point) + 1);
    *point = ch;
    return 1;
}

int InsStr(char *txt, char *point, char *str, size_t maxlen)
{
    int txtlen, wordlen;
    char *ptr;
    if (NULL == str || !*str)
        return 0;
    wordlen = strlen(str);
    txtlen = strlen(txt) + wordlen + 1;
    if (txtlen > maxlen)
        return -(txtlen);
    ptr = point + wordlen;
    memmove(ptr, point, strlen(point) + 1);
    memcpy(point, str, wordlen);
    return (txtlen - 1);
}

int InsWord(char *txt, char *point, char *word, size_t maxlen)
{

    char *ptr;
    int txtlen, spflag = 0;

    if ((txtlen = InsStr(txt, point, word, maxlen - 2)) <= 0)
        return txtlen == 0 ? 0 : (txtlen + -2);

    if (*word != ' ')
    {
        if (*point != ' ' && (*(point - 1) != ' ' && (point) != txt))
        {
            memmove(point + 1, point, strlen(point) + 1);
            *point = ' ';
            spflag++;
        }
        ptr = point + (strlen(word));
    }

    else
    {
        if (*point == ' ')
            memmove(point - 1, point, strlen(point) + 1);
        ptr = point + (strlen(word) - 1);
        spflag--;
    }

    if (word[strlen(word) - 1] != ' ')
    {
        if (*(ptr + 1) && *ptr != ' ' && *ptr - 1 != ' ' && *ptr + 1 != ' ')
        {
            memmove(ptr + 1, ptr, strlen(ptr) + 1);
            *ptr = ' ';
            spflag++;
        }
    }

    return (txtlen + spflag);
}

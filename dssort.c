/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  dssort - an O(N) sort program.                                   */
/*                                                                   */
/*  Last modification - 1995-04-21                                   */
/*                                                                   */
/*  This program takes the following parameters:                     */
/*  1) base - A pointer to the start of your data                    */
/*  2) n - number of elements to be sorted                           */
/*  3) size - size of elements being sorted                          */
/*  4) offset - number of bytes into the record                      */
/*                                                                   */
/*  Designed to be much like C's builtin qsort, this program sorts   */
/*  into ascending order an array base[0]...base[n-1] of objects     */
/*  size size.  (not a typo).                                        */
/*                                                                   */
/*  History - originally I read in a book that radix sort was the    */
/*  best O(N) sort available, but it was very slow and you needed    */
/*  a lot of records before the O(N) started beating the O(NlogN)    */
/*  that quicksort achieved.  Hence I attempted to create a new      */
/*  O(N) algorithm that was faster than radix sort.  I achieved      */
/*  this, and called it "death sort".  It turned out that this had   */
/*  already been invented, and was called "radix exchange sort".     */
/*  IOW, the book was full of shit.  I then went out and tried to    */
/*  invent another sort algorithm, and did so, and called this one   */
/*  "licorice all sort".  This in turn turned out to be an           */
/*  implementation of "distribution sort".  After that I decided to  */
/*  become a Tibetan monk, and I've never looked back.               */
/*                                                                   */
/*                                                                   */
/*  Here is how my implementation of distribution sort works:        */
/*                                                                   */
/*  To sort an array of character strings, you first start off       */
/*  looking at the first byte.  You create a table of statistics     */
/*  (UCHAR_MAX entries) so that you know how many A's ther are etc.  */
/*                                                                   */
/*  It then once again traverses the list, knowing where to put      */
/*  each element that is out of order.                               */
/*                                                                   */
/*  I am contactable via netmail to 3:711/934.                       */
/*                                                                   */
/*  This program was originally written on 1989-10-03                */
/*                                                                   */
/*                                                                   */
/*  An enhancement was made so that when you have less than or equal */
/*  to RETIME records to sort, use Radix Exchange Sort instead.      */
/*  When you have less than or equal to INTIME records to sort, use  */
/*  Insertion Sort instead.                                          */
/*                                                                   */
/*********************************************************************/

/* RETIME, INTIME are machine dependant, find values that suit you   */
/* best.  List of possible optimum times for different machines:     */
/* IBM XT compatible: RETIME 46, INTIME 7                            */

#define RETIME 46               /* when radix is better than licorice */
#define INTIME 7                /* when insertion sort is better than resort */
#define MAX 300                 /* maximum length of strings to be sorted */
static char temp[MAX];          /* temporary storage for swap */

#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>


/*********************************************************************/
/*                                                                   */
/*  insertion sort - start from the bottom element (which is a       */
/*  sorted array of length 1), then every time you move up one       */
/*  element, you move it down (swapping with all elements below it)  */
/*  until it is in its sorted position.                              */
/*                                                                   */
/*********************************************************************/

void insort(void *base, int n, int size, int offset)
{
    register int j;
    register int rc;
    register int i;

    for (i = 0; i < n; i++)     /* moving up one element */
    {
        for (j = i; j > 0; j--) /* move down until in sorted order */
        {
            rc = memcmp((unsigned char *) base + j * size + offset,
            (unsigned char *) base + (j - 1) * size + offset, size - offset);
            if (rc < 0)
            {
                /* swapping elements along the way */
                memcpy(temp, (unsigned char *) base + j * size, size);
                memcpy((unsigned char *) base + j * size,
                       (unsigned char *) base + (j - 1) * size, size);
                memcpy((unsigned char *) base + (j - 1) * size, temp, size);
            }
            else
            {
                break;
            }
        }
    }
    return;
}


/*********************************************************************/
/*                                                                   */
/*  radix exchange sort.  view one bit at a time, getting all '1'    */
/*  bits up the top, and all the '0' bits down the bottom.           */
/*                                                                   */
/*********************************************************************/

void resort(void *base, int n, int size, int offset)
{
    int bytesin;                /* no of bytes into the string */
    int bitsin;                 /* no of bits into the byte */
    int p;                      /* bottom element number */
    int q;                      /* top element number */
    register unsigned char mask = 0x80; /* bit we are interested in */
    
    if (n <= 1)
        return;                 /* we have finished this sub-sort if we have
                                 * one or less elements to sort */
    bytesin = offset / CHAR_BIT;/* work out how many bytes we are into the
                                 * element by dividing by the number of bits
                                 * we are into the element by the number of
                                 * bits in a byte */
    if (bytesin == size)
        return;                 /* we have just passed the length of the data */
    bitsin = offset % CHAR_BIT; /* no of bits into the byte */
    mask = (unsigned char)(mask >> bitsin); /* mask masks the bit we are 
                                               interested in */
    p = 0;                      /* low element starts at 0 */
    q = n - 1;                  /* high element starts at n-1 */
    while (p <= q)              /* until all swaps have been done */
    {
        while (((*((unsigned char *) base + p * size + bytesin) & mask) == 0)
               && (p < n))
            p++;                /* p will move up until it finds a bit that
                                 * is not 0 (or until it reaches the top) */
        /* q moves down until if finds a bit that is 0 (or until it reaches
         * the bottom) */
        while ((q >= 0) &&
             ((*((unsigned char *) base + q * size + bytesin) & mask) != 0))
        {
            q--;
        }
        if (p < q)              /* p has a 1 bit and q has a 0 bit, so swap */
        {
            memcpy(temp, (unsigned char *) base + p * size, size);
            memcpy((unsigned char *) base + p * size,
                   (unsigned char *) base + q * size, size);
            memcpy((unsigned char *) base + q * size, temp, size);
            p++;                /* p now points to a 0 bit, so move up one
                                 * more */
            q--;                /* q now points to a 1 bit, so move down one
                                 * more */
        }
    }
    /* sort lower numbers */
    if (p <= INTIME)
    {
        insort(base, p, size, (offset + 1) / CHAR_BIT);
    }
    else
    {
        resort((unsigned char *) base, p, size, offset + 1);
    }
    /* sort higher numbers */
    if ((n - p) <= INTIME)
    {
        insort((unsigned char *) base + p * size, 
               n - p, 
               size, 
               (offset + 1) / CHAR_BIT);
    }
    else
    {
        resort((unsigned char *) base + p * size, n - p, size, offset + 1);
    }
    return;
}


/* distribution sort */

void dssort(void *base, int n, int size, int offset)
{
    int *stats;                 /* ptr to table of statistics (how many A's
                                 * etc) */
    unsigned char **subp;       /* table of pointers to top of characters */
    unsigned char **orgp;       /* table of original pointers */
    register int i;             /* used for many things */
    register unsigned char critic;  /* character to be sorted */
    register unsigned char *wereat; /* pointer to element we're up to */
    
    assert((stats = malloc((UCHAR_MAX + 1) * sizeof(*stats))) != NULL);
    assert((subp = malloc((UCHAR_MAX + 1) * sizeof(*subp))) != NULL);
    assert((orgp = malloc((UCHAR_MAX + 1) * sizeof(*orgp))) != NULL);
    for (i = 0; i < (UCHAR_MAX + 1); i++)
    {
        stats[i] = 0;           /* reset statistics */
    }
    for (i = 0; i < n; i++)     /* create stats */
    {
        stats[*((unsigned char *) base + i * size + offset)]++;
    }
    orgp[0] = subp[0] = base;
    /* create table of original pointers, and subpointers */
    for (i = 0; i < UCHAR_MAX; i++)
    {
        orgp[i + 1] = subp[i + 1] = subp[i] + size * stats[i];
    }
    for (i = 0; i < n;)
    {
        /* start at bottom of table */
        wereat = (unsigned char *) base + i * size;
        critic = *(wereat + offset);  /* byte to put in sorted order */
        if (wereat >= orgp[critic])
        {
            i++;                /* is it already sorted? */
        }
        else
        {
            memcpy(temp, wereat, size); /* copy the record into temp */
            memcpy(wereat, subp[critic], size); /* get unsorted back here */
            memcpy(subp[critic], temp, size); /* put temp into sorted order */
            subp[critic] += size; /* next pointer will be one record up */
        }
    }
    free(subp);                 /* don't need table of subpointers any more */
    if (++offset < size)        /* if there are still more bytes in record */
    {
        for (i = 0; i < (UCHAR_MAX + 1); i++)
        {
            /* if there are sub-records of a character, sort them */
            if (stats[i] > 1)
            {
                /* if there are less than INTIME records, use insort */
                if (stats[i] <= INTIME)
                    insort(orgp[i], stats[i], size, offset);
                /* else if there are less than RETIME records, use resort */
                else if (stats[i] <= RETIME)
                    resort(orgp[i], stats[i], size, offset * 8);
                else
                    dssort(orgp[i], stats[i], size, offset);
            }
        }
    }
    free(stats);                /* don't need table of statistics anymore */
    free(orgp);                 /* don't need table of original pointers
                                 * either */
    return;
}


/* example of program that calls distribution sort */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

main(int argc, char **argv)
{
    FILE *fp;
    FILE *fq;
    time_t t1;
    time_t t2;
    int numrec;
    int lenrec;
    char *myarr;

    if (argc < 5)
    {
        printf("you must enter input file name, output file name\n");
        printf("number of elements to read, length of elements\n");
        printf("without making any mistakes\n");
        return (EXIT_FAILURE);
    }
    assert((fp = fopen(*(argv + 1), "rb")) != NULL);
    assert((fq = fopen(*(argv + 2), "wb")) != NULL);
    assert((numrec = atoi(*(argv + 3))) != 0);
    assert((lenrec = atoi(*(argv + 4))) != 0);
    assert((myarr = malloc(lenrec * numrec)) != NULL);
    numrec = fread(myarr, lenrec, numrec, fp);
    printf("%d records read in to licorice all sort\n", numrec);
    time(&t1);
    dssort((unsigned char *) myarr, numrec, lenrec, 0);
    time(&t2);
    printf("distribution sort took %d secs to process %d records\n",
           (int) difftime(t2, t1), numrec);
    fwrite(myarr, lenrec, numrec, fq);
    return (EXIT_SUCCESS);
}

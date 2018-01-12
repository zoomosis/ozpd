/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This program is given 4 parameters: the low number in the range, */
/*  the high number in the range, how many numbers you want          */
/*  selected from the range, and an array where you want all the     */
/*  numbers to be stored.  It puts random numbers from within the    */
/*  range into the area you specify.  This routine comes in useful   */
/*  when working out your lotto numbers for instance.                */
/*  ransel(1,40,6,arr);   will make arr contain your numbers.        */
/*                                                                   */
/*  You should call stdsed() e.g. yourself to seed the random number */
/*  generator.                                                       */
/*                                                                   */
/*********************************************************************/

#include <stddef.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ransel.h"

int ransel(int low, int high, int no, int nos[])
{
    int x,i,dist;
    int *arr;
  
    dist = high - low + 1;
    arr = malloc(dist*sizeof(int));
    if (arr == NULL) 
    {
        return (-1);
    }
    for (x=low; x<=high; x++)
    {
        arr[x-low] = x;
    }
    for (i=1; i<=no; i++)
    {
        int temp;
        
        x = (float)rand()/(((float)RAND_MAX)+1.0) * (dist-i+1);
        temp = arr[x];
        arr[x] = arr[dist-i];
        arr[dist-i] = temp;
    }
    for (i=0; i<no; i++)
    {
        nos[i] = arr[dist-no+i];
    }
    free(arr);
    return (0);
}

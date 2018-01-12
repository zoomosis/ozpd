/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  pincode - encode a pin number.                                   */
/*                                                                   */
/*  This program allows you to encode all your individual pin        */
/*  numbers with a single master pin number.  You enter both         */
/*  your master pin number and the new pin number, and it            */
/*  generates a sequence of random numbers, then substitutes         */
/*  in the new pin, which still leaves the numbers random for        */
/*  anyone without the master pin.                                   */
/*                                                                   */
/*  If you forget the master pin, you can always reverse the         */
/*  process to find the master, if you know one of the real pins.    */
/*  This also has the disadvantage in that if one of your pins is    */
/*  compromised, all of them are.                                    */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "stdsed.h"
#include "ransel.h"

int main(int argc, char **argv)
{
    int arr[10];
    int cnt = 0;
    int pos1;
    int pos2;
    int enc;
    int temp;
    char *m;
    char *e;
    int x;
    
    if (argc < 3)
    {
        printf("usage: pincode <master pin> <encode pin>\n");
        return (EXIT_FAILURE);
    }
    stdsed();
    m = *(argv + 1);
    e = *(argv + 2);
    if (strlen(e) > strlen(m))
    {
        printf("pin to encode is longer than master\n");
        return (EXIT_FAILURE);
    }
    printf("0 1 2 3 4 5 6 7 8 9\n\n");
    while (e[cnt] != '\0')
    {
        ransel(0, 9, 10, arr);
        enc = e[cnt] - '0';
        pos1 = m[cnt] - '0';
        for (pos2 = 0; pos2 < 10; pos2++)
        {
            if (arr[pos2] == enc)
            {
                break;
            }
        }
        arr[pos2] = arr[pos1];
        arr[pos1] = enc;
        for (x = 0; x < 10; x++)
        {
            printf("%d ", arr[x]);
        }
        printf("\n");
        cnt++;
    }
    return (0);
}

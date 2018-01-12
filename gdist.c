/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  gdist - calculate distance between two GPS points in degrees     */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "gccalc.h"

int main(int argc, char **argv)
{
    double lat1, lon1, lat2, lon2, dist;
    
    if (argc < 5)
    {
        printf("usage: <lat1> <lon1> <lat2> <lon2>\n");
        return (EXIT_FAILURE);
    }

    lat1 = strtod(*(argv + 1), NULL);
    lon1 = strtod(*(argv + 2), NULL);
    lat2 = strtod(*(argv + 3), NULL);
    lon2 = strtod(*(argv + 4), NULL);
    dist = degdist(lat1, lon1, lat2, lon2);
    printf("%lf\n", dist);
    return (0);
}

/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  gccalc - great circle calculations                               */
/*                                                                   */
/*  degdist - calculate distance for degrees (DDD.DDDD) to metres.   */
/*  raddist - calculate distance for radians to radians.             */
/*  d2r - convert degrees to radians.                                */
/*  r2m - convert radians to metres.                                 */
/*                                                                   */
/*********************************************************************/

#include <math.h>

#include "gccalc.h"

#define pi 3.14159

double d2r(double deg)
{
    double r;
    
    r = (2 * pi) * (deg / 360.0);
    return (r);
}

double r2m(double rad)
{
    double m;
    
    m = rad * 180 * 60 / pi * 1852.0;
    return (m);
}

double degdist(double lat1, double lon1, double lat2, double lon2)
{
    double d;
    
    d = raddist(d2r(lat1), d2r(lon1), d2r(lat2), d2r(lon2));
    d = r2m(d);
    return (d);
}

double raddist(double lat1, double lon1, double lat2, double lon2)
{
    double d;
    
    d = 2 
      * asin(
             sqrt(
                  pow(
                      sin(
                          ((lat1-lat2)/2)
                         ), 2
                     )
                  + cos(lat1)
                    * cos(lat2)
                    * pow(
                          sin(
                              ((lon1-lon2)/2)
                             ), 2
                         )
                 )
            );
    return (d);
}

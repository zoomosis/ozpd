/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  This function calculates the greatest common divisor of          */
/*  two integers, using Euclid's algorithm                           */
/*                                                                   */
/*********************************************************************/

long gcd(long a, long b)
{
  long temp;
  long rem;

  if (a < b)
  {
    temp = a;
    a = b;
    b = temp;
  }
  rem = a % b;
  while (rem != 0)
  {
    a = b;
    b = rem;
    rem = a % b;
  }
  return (b);
}

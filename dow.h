/* dow - written by Paul Edwards, 1993.1.31 */
/* Released to the Public Domain */
/* This routine will work over the range 1.1.1 to 32767.12.31.
   It assumes the current calendar system has always been in
   place in that time.  If you pass 0 or negative years, then
   it produces results on the assumption that there is a year
   0.  It should always produce a value in the range of 0..6
   if a valid month and day have been passed, no matter what
   the year is.  However, it has not been tested for negative
   years, because the results are meaningless anyway.  It is
   mainly to stop people playing silly buggers and causing
   the macro to crash on negative years. */
/* it returns the day of the week, 0..6 for Monday..Sunday,
   according to ISO's recommendation */

#define dow(y,m,d) \
  ((((((m)+9)%12+1)<<4)%27 + (d) + \
  ((y)%400+400) + ((y)%400+400)/4 - ((y)%400+400)/100 + \
  (((m)<=2) ? ( \
  (((((y)%4)==0) && (((y)%100)!=0)) || (((y)%400)==0)) \
  ? 5 : 6) : 0)) % 7)
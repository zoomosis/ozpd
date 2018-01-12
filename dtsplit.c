 /**************************** dt_split () ******************************
 * Fido format - string, 20 bytes                                       *
 *   DayOfMonth " " Month " " Year " " " " HH ":" MM ":" SS Null        *
 *                                  ^---^ note *2* spaces!              *
 *   *Note: Fido 0 fills DayOfMonth.                                    *
 *                                                                      *
 * Seadog format - string, 20 bytes                                     *
 *   DayOfWeek " " DayOfMonth " " Month " " Year " " HH ":" MM Null     *
 *   *Note: Seadog blank fills DayOfMonth.                              *
 *                                                                      *
 *                                                                      *
 *                   BYTE POSITION                                      *
 *                           1 1 1 1 1 1 1 1 1 1                        *
 *       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9                        *
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       *
 * FIDO |0|1| |J|a|n| |8|6| | |0|1|:|2|3|:|5|6| | <-NULL terminated.    *
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       *
 * SEA  |M|o|n| | |1| |J|a|n| |8|6| |0|2|:|3|4| | <-NULL terminated.    *
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       *
 *               ^_NOT_ zero filled!                                    *
 *                                                                      *
 * Note: _double_ space in Fido date string between year and hour.      *
 *                                                                      *
 * Options for testing:                                                 *
 *                                                                      *
 *   1. Test byte 0.  If numerical, then Fido, else if Uppercase Alpha, *
 *      then Seadog, else Error.  This is the option chosen.            *
 *                                                                      *
 *   2. Test byte 9.  If blank, then Fido, else if Lowercase Alpha,     *
 *      then Seadog, else Error.                                        *
 *                                                                      *
 *   3. Test both bytes.  More complex.  Don't need this complexity.    *
 *                                                                      *
 *                                                                      *
 *          THIS CODE IS RELEASED TO THE PUBLIC DOMAIN.                 *
 *                                                                      *
 *                                                                      *
 * (Delete the following NOTE when it no longer applies.  It is aimed   *
 *  mainly at those writing and testing sdt* and fdt*, and serves no    *
 *  other purpose than to give suggestion.)                             *
 *                                                                      *
 * NOTE: the functions sdt* and fdt* should be different only           *
 * in the byte positions declared for the constituent parts.  Extraction*
 * of these parts should be handled by common functions!  The defines   *
 * for the minimum and maximum values for year, month, day, hour, and   *
 * minute should be moved here for access by both sdt* and fdt*.        *
 * Benjamin and Paul, your code has to pass through this code.  I       *
 * have restated the defines here so that you should not need to        *
 * redeclare them in your own code.  I am assuming that the code from   *
 * the three of us will need to be combined into one file because of    *
 * the static declarations!                                             *
 ************************************************************************/ 

 /*************************** INCLUDES AND DEFINES **********************
  * (These are mostly copied directly from Benjamin Lin, except for the *
  *  error code defines.)                                               */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 
 #include "dtsplit.h"
 
                    /*****************************************************/
 #define ymax 99    /* highest year                                      */
 #define ymin 0     /* lowest year                                       */
 #define dmax 31    /* maximum days                                      */
 #define dmin 1     /* minimum days                                      */
 #define hmax 23    /* 24 hrs to a day, from 00.00 to 23.59.             */
 #define hmin 0     /* minimum hours                                     */
 #define mmax 59    /* 60 mins in an hour, from 00 to 59                 */
 #define mmin 0     /* minimum minutes                                   */
 #define smax 59    /* 60 mins in an hour, from 00 to 59                 */
 #define smin 0     /* minimum minutes                                   */
                    /* further defines can be placed here to indicate    */
                    /* other failures in date processing in fdt* and sdt**/
                    /* according to need.  These error codes would need  */
                    /* to be defined even earlier in the program if the  */
                    /* modules which call this function need to know the */
                    /* reason for failure, which if my memory serves me, */
                    /* is a distinct possibility.                        */
                    /*****************************************************/

 /************************************************************************
  * The following months of the year are in hex because the standard for *
  * Fidonet is ASCII, not EBCDIC (etc) and this program is designed to   *
  * be character set independent                                         *
  ************************************************************************/

 static unsigned char * Month[] = { (unsigned char *)"", 
                             (unsigned char *)"\x4a\x61\x6e",  /* Jan */
		             (unsigned char *)"\x46\x65\x62",  /* Feb */
                             (unsigned char *)"\x4d\x61\x72",  /* Mar */
                             (unsigned char *)"\x41\x70\x72",  /* Apr */
                             (unsigned char *)"\x4d\x61\x79",  /* May */
			     (unsigned char *)"\x4a\x75\x6e",  /* Jun */
                             (unsigned char *)"\x4a\x75\x6c",  /* Jul */
			     (unsigned char *)"\x41\x75\x67",  /* Aug */
                             (unsigned char *)"\x53\x65\x70",  /* Sep */
			     (unsigned char *)"\x4f\x63\x74",  /* Oct */
                             (unsigned char *)"\x4e\x6f\x76",  /* Nov */
			     (unsigned char *)"\x44\x65\x63"}; /* Dec */
 

 /*********************** FUNCTION DECLARATIONS **************************/
 static int sdt_split (const unsigned char *sdt, int *year, int *month, 
                       int *day, int *hour, int *minute);
 static int fdt_split (const unsigned char *fdt, int *year, int *month, 
                       int *day, int *hour, int *minute, int *sec);

 /*************************** DT_SPLIT (the function) *******************
 * Purpose        : Test for Seadog or Fido format, pass to appropriate *
 *                  function.  No need to test validity of string, as   *
 *                  function being called is to test each part of the   *
 *                  string.                                             *
 * Preconditions  : A date string has been passed in.                   *
 * Postconditions : The date string has been parsed, and the results    *
 *                  passed back to the calling module.  Results will    *
 *                  be the valid parts of a date, else an error is      *
 *                  returned.  Error is assumed till otherwise defined  *
 *                  to be coded as 0, to indicate a Fido or a Seadog    *
 *                  date, else 1, to indicate an unrecognized date      *
 *                  format (ie not Fido or Seadog).                     *
 * Uses                                                                 *
 * System defined :                                                     *
 * Programmer defined : sdt_split() (by Benjamin Lin)                   *
 *                      fdt_split() (by Paul Edwards)                   *
 * Programmer     : Romane Takkenberg                                   *
 * First version  : 1993.02.05                                          *
 * Comments       : The 0 in the if statements are possibly not needed, *
 *                  (*(dt) already points to 0), but are kept for       *
 *                  readability.                                        *
 ************************************************************************/
 int dt_split (const unsigned char *dt, int *year, int *month, int *day, 
               int *hour, int *minute, int *sec)
 {
   int error = GOOD_DATE;                        /* assume no problems */ 

   if ((*dt >= 0x41) && (*dt <= 0x5a))    /* Seadog format ('A' to 'Z') */
   {
     *sec = 0;
     error = sdt_split (dt, year, month, day, hour, minute);
   }
   else
   {
     if ((*dt >= '0') && (*dt <= '9'))    /* Fido format ? ('0' to '9') */
       error = fdt_split (dt, year, month, day, hour, minute, sec);
     else
       error = UNRECOGNIZED_DATE;                /* not Fido or Seadog */
   }
   return (error);
 } /* dt_split() */



/************************************************************************** 
 *    Module - sdtsplit 
 *    Author: Benjamin Lin 
 *    Description: 
 *        The function sdt_split() takes a date string in the SEAdog date 
 *        format and breaks it down to its components. 
 *        Tested using Borland C++ 3.1 under ANSI C mode. 
 *    Revision History: 
 *        1992.12.10 - First working model. 
 *        1993.01.10 - Second revision. 
 *    Released into Public Domain by Benjamin Lin, 1993. 
 **************************************************************************/ 
 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
 
/* comment out this if not debugging. */ 
/*#define DEBUG_SDT_SPLIT*/
 
/************************************************************************** 
 *    Function - asctoi() 
 *    Description: 
 *        Same as standard atoi() except ASCII char set is always used. 
 *    Revision History: 
 *        1993.01.10 - First working model. 
 *    Released into Public Domain by Benjamin Lin, 1993. 
 **************************************************************************/ 
 
int asctoi(const unsigned char *s) 
{ 
  int N1;                               /* a counter */ 
  int neg;                              /* Is is negative? */ 
  int retval; 
  N1 = 0; 
  neg = 0; 
  retval = 0; 
  while ((s[N1] == '\x20')) {           /* skip leading spaces */ 
    N1++; 
  } 
  if (s[N1] == '\x2d') {                /* check if it is negative */ 
    neg = 1; 
    N1++; 
  } 
  else if (s[N1] == '\x2b') {           /* ignore the positive sign if 
                                           present */ 
    N1++; 
  } 
  for (; N1 < strlen((char *)s); N1++) { 
    if ((s[N1] >= '\x30') && (s[N1] <= '\x39')) { 
                                        /* accumulate the result if a 
                                           digit is found */ 
      if (!neg) 
        retval = (retval * 10) + (s[N1] - 0x30); 
      else 
        retval = (retval * 10) - (s[N1] - 0x30); 
    } 
    else {                              /* exit the function if any 
                                           unrecognised char found */ 
      return retval; 
    } 
  } 
  return retval;                        /* finish process, exit */ 
} 
 
 
/************************************************************************** 
 *    Function - get_numerical_token() 
 *    Description: 
 *        Parse a numerical token in a string into a int. Only two digits 
 *        are parsed in this version. 
 *    Parameter: 
 *        tokval - pointer to store the parsed integer. 
 *        s      - source string. 
 *        pos    - poition to get the token. 
 *        maxval - maximum legal value for the token. 
 *        minval - minimum legal value for the token. 
 *    Revision History: 
 *        1993.01.10 - First working model based on get_time() written by 
 *                      Romane Takkenberg. 
 *    Released into Public Domain by Benjamin Lin, 1993. 
 **************************************************************************/ 
 
int get_numerical_token( 
  int *tokval,
  const unsigned char* s, 
  const int pos, 
  const int maxval, 
  const int minval) 
{ 
  int Result; 
  static unsigned char Buf[3] = "00"; 
 
  strncpy((char *)Buf, (char *)s + pos, 2); 
  *tokval = asctoi(Buf); 
  Result = ((*tokval > maxval) || (*tokval < minval)) ? 0 : 1; 
  return Result; 
} 
 
 
/************************************************************************** 
 *    Function - sdt_split() 
 *    Description: 
 *        Parse a SEAdog date format string into its components. 
 *    Parameter: 
 *        sdt    - date string in SEAdog format. 
 *    Revision History: 
 *        1992.12.10 - First working model. 
 *        1993.01.10 - Code to parse year, day, hour, min is now in 
 *                      get_numerical_token(). 
 *                    - Modified to use ANSI C time function call time() 
 *                      and localtime(). 
 *                    - Local variable Timer is a now data object instead 
 *                      of a pointer. 
 *    Released into Public Domain by Benjamin Lin, 1993. 
 **************************************************************************/ 
 
static int sdt_split( 
  const unsigned char *sdt, 
  int *year, 
  int *month, 
  int *day, 
  int *hour, 
  int *min) 
{ 
  time_t Timer; 
  struct tm* pTM; 
 
  int iMonth; 
 
  /* Year */ 
  if (!get_numerical_token(year, sdt, 11, ymax, ymin)) 
    return (1); 
 
  time(&Timer); 
  pTM = localtime(&Timer); 
  if (((2000+*year) - pTM->tm_year) <= (pTM->tm_year - (1900+*year))) 
    *year = *year + 2000; 
  else 
    *year = *year + 1900; 
 
  /* Month */ 
  for (iMonth=1; (memcmp((sdt+7),Month[iMonth],3) != 0) && (iMonth < 13); 
       iMonth++) { 
  } 
  if (iMonth == 13) 
    return (1); 
  *month = iMonth; 
 
  /* Day */ 
  if (!get_numerical_token(day, sdt, 4, dmax, dmin)) 
    return (1); 
 
  /* Hour */ 
  if (!get_numerical_token(hour, sdt, 14, hmax, hmin)) 
    return (1); 
 
  /* Min */ 
  if (!get_numerical_token(min, sdt, 17, mmax, mmin)) 
    return (1); 
 
  return (0); 
} 
 
 
/************************************************************************** 
 *    Function - fdt_split() 
 *    Description: 
 *        Parse a Fido date format string into its components. 
 *    Parameter: 
 *        fdt    - date string in Fido format. 
 *    Revision History: 
 *        1993.07.03 - First working model, based on sdt_split(). 
 *
 *    Released into Public Domain by Paul Edwards, 1993. 
 **************************************************************************/ 
 
static int fdt_split( 
  const unsigned char *fdt, 
  int *year, 
  int *month, 
  int *day, 
  int *hour, 
  int *min,
  int *sec)
{ 
  time_t Timer; 
  struct tm* pTM; 
 
  int iMonth; 
 
  /* Year */ 
  if (!get_numerical_token(year, fdt, 7, ymax, ymin)) 
    return (1); 
 
  time(&Timer); 
  pTM = localtime(&Timer); 
  if (((2000+*year) - pTM->tm_year) <= (pTM->tm_year - (1900+*year))) 
    *year = *year + 2000; 
  else 
    *year = *year + 1900; 
 
  /* Month */ 
  for (iMonth=1; (memcmp((fdt+3),Month[iMonth],3) != 0) && (iMonth < 13); 
       iMonth++) { 
  } 
  if (iMonth == 13) 
    return (1); 
  *month = iMonth; 
 
  /* Day */ 
  if (!get_numerical_token(day, fdt, 0, dmax, dmin)) 
    return (1); 
 
  /* Hour */ 
  if (!get_numerical_token(hour, fdt, 11, hmax, hmin)) 
    return (1); 
 
  /* Min */ 
  if (!get_numerical_token(min, fdt, 14, mmax, mmin)) 
    return (1); 

    /* Sec */ 
  if (!get_numerical_token(sec, fdt, 17, smax, smin)) 
    return (1); 
 
  return (0); 
} 
 


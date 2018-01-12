/***********************************************************************
**                                                                    **
**   Function    :  Reform                                            **
**                                                                    **
**   Purpose     :  Pattern Changing                                  **
**                                                                    **
**   Usage       :  Pass three strings as parameters.The first being  **
**                  the string you want transformed,the second being  **
**                  the pattern the string matches & the third being  **
**                  the pattern you want the resulting string to      **
**                  match.                                            **
**                  This function returns a pointer to a reformatted  **
**                  string.This pointer will be NULL if there is an   **
**                  error in the call.The error can be because the    **
**                  first string does not match the pattern passed.   **
**                                                                    **
**                  e.g   reform("abcdef","a*de?","de*?a") will       **
**                        return a string pointer to "debcfa".        **
**                                                                    **
**                        reform("abcdef","a*de?","*ccx?") will       **
**                        return a string pointer to "bcccxf".        **
**                                                                    **
**   Author      :  Sreenath Chary              Apr 10 1989           **
**                                                                    **
**   Logic       :  Take each character & if it is a *,then get all   **
**                  the character's it stands for & replace the       **
**                  first *'s position in the second pattern with     **
**                  these characters,similaryly with the ? until      **
**                  there are no wild cards in the first pattern.     **
**                  This should result in no wild cards being left    **
**                  in the second pattern normally.                   **
**                                                                    **
**   N.B         :  If the second pattern has less wild cards than    **
**                  the first pattern the substitutuion may not be    **
**                  what you want.If there are more wild cards in the **
**                  result pattern,then the leftover wild cards in    **
**                  the result will all be to the left of the string. **
**                  Anyway navigate with care!!!!!                    **
**                                                                    **
**   Changes:                                                         **
**   1992/09/28: Modified by Paul Edwards.  There was a bug in        **
**               subst so I fixed that as well as changing it to use  **
**               some ANSI provided functions                         **
**                                                                    **
**   This program is dedicated to the public domain.                  **
**                                                                    **
***********************************************************************/
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static int process(char *raw,char *pat,char *rslt);
static void subst(char which,char what,char *in,char *with,int howmany);
int patmat(char *raw,char *pat);

char *reform(char *raw,char *pat1,char *pat2)
{
   char *rfrm;

   if (patmat(raw,pat1) == 0)               /* if string does not    */
     return( NULL );                        /* match pattern         */

   rfrm = (char *)malloc(strlen(raw)+strlen(pat2)+2);
                                            /* Max length of result  */
   strcpy(rfrm,pat2);                       /* this will be clobbered*/

   process(raw,pat1,rfrm);

   return( rfrm ) ;
 }
static int process(char *raw,char *pat,char *rslt)
/***********************************************************************
**                                                                    **
**   Function    :  process                                           **
**                                                                    **
**   Purpose     :  This is the backbone of reform.It receives the    **
**                  the raw string,the pattern the raw string matches **
**                  & the result string with the pattern you want the **
**                  result to match as parameters.It then proceeds to **
**                  match the wild cards in pattern1 with the chara-  **
**                  cters they represent.These representations are    **
**                  then replaced in the result string where the      **
**                  wild card appears.Because the pattern matching is **
**                  done in a recursive fashion,and hence the chara-  **
**                  cters which match each wild card are retrieved    **
**                  in reverse,the replacement in the result string   **
**                  is also done in reverse.                          **
**                                                                    **
***********************************************************************/
 {  int  i;

   if ((*pat == '\0') && (*raw == '\0'))
     return( 1 );

   if (*pat == '*')
    { if (*(pat+1) == '\0')
       { subst('l','*',rslt,raw,strlen(raw));
         return( 1 );
       }
      for(i=0;i<=strlen(raw);i++)
      {
        if ((*(raw+i) == *(pat+1)) || (*(pat+1) == '?'))
         {
           if (*(pat+1) == '?')
            {  subst('f','?',rslt,raw+i,1);
            }
           if (process(raw+i+1,pat+2,rslt) == 1)
            {  subst('l','*',rslt,raw,i);
               return( 1 ) ;
            }
         }
      }
    }
   else
    {
      if ((*pat == '?') || (*pat == *raw))
       { if (*pat == '?')
           {  subst('f','?',rslt,raw,1);
           }
         if (process(raw+1,pat+1,rslt) == 1)
           return( 1 ) ;
       }
    }
   return( 0 ) ;
}

static void subst(char which,char what,char *in,char *with,int howmany)
/***********************************************************************
**                                                                    **
**   Function    :  subst  short for substitute.                      **
**                                                                    **
**   Purpose     :  This function substitutes a particular occurence  **
**                  (which = 'l'=>last,'f'=>first) of character (what)**
**                  in string (in) with a certain number of characters**
**                  (howmany) of another string(with).                **
**                                                                    **
**                  e.g subst('l','*',"abcde*efg*h","xxx",2) will     **
**                                                                    **
**                  result in "abcde*efgxxh" in the in string.        **
**                                                                    **
**   Note        : in string must be allocated long enough for        **
**                 the substitute to take place.                      **
**                                                                    **
**                                                                    **
***********************************************************************/
{
  char *temp;

  if (which == 'l') temp = strrchr(in,what);
  else temp = strchr(in,what);
  if (temp == NULL) return;
  memmove(temp+howmany,temp+1,strlen(temp));
  memcpy(temp,with,howmany);
 }

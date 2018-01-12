/***********************************************************************
**                                                                    **
**   Program  :  Calculator                                           **
**                                                                    **
**   Purpose  :  Has the functions of a desk calculator.It allows     **
**               variables ( A max of 28 ) & allows displaying        **
**               variable values.                                     **
**                                                                    **
**   Author   :  Sreenath Chary     Dec 30 1988                       **
**                                                                    **
**                                                                    **
**   Adding Functions  : New functions may be added to the calculator **
**                       by repeating the last entry in the struct    **
**                       fun_det.                                     **
**                                                                    **
**   Adding Constants  : Commonly used constants can be entered by    **
**                       declaring them in the structure const_struct **
**                       by just repeating the line { "pi",..},       **
**                                                                    **
**   Nota Bene  :        Only those functions which take in one       **
**   ---------           argument in double float format & return a   **
**                       double float can be entered.                 **
**                       Function names must be composed of only      **
**                       alpha numeric characters & must start of     **
**                       with an alphabetic character.The length of   **
**                       the name can be between 1 to 30 characters.  **
**                                                                    **
**                       Constant names must be composed of only      **
**                       alpha numeric characters & must start with   **
**                       an alphabetic character & the name length    **
**                       must be more than 1 character.               **
**                                                                    **
**                       Constant & Function names may be the same    **
**                       and they are evaluated only based on the     **
**                       way they are used.                           **
**                                                                    **
**   Various mods by Paul Edwards to fix bugs, improve code and       **
**   add support for calculation string to be done as a command line  **
**   parameter.                                                       **
**                                                                    **
**   This Program is Dedicated to the Public Domain                   **
**                                                                    **
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define  MAX  (sizeof fun_arr/sizeof(struct fun_det))
#define  CONSTS (sizeof consts/sizeof(struct const_struct))
#define entry(a)  {#a,a}

static void prthlp(char *c);
static void elimbl(char *inc);
static void getinp(char *inc);
static void prtvar(char *inc);
static void catchr(char *c,char *c1);
static double compute(char *inc);
static double evaluate(char *c, int delim);
static char *sttoup(char *x);
static double cfact(double n);

static void hsin(void),hsinh(void),hasin(void),
    hcos(void),hcosh(void),hacos(void),
    htan(void),htanh(void),hatan(void),
    hlog(void),hlog10(void),
    hfloor(void),
    hceil(void),hsqrt(void),hcfact(void),
    hfabs(void),hexp(void);

static double  vars[257];
static char    dcl_vars[257];
static char    temp = 0x01;
static struct fun_det { char   *fname   ;
                 double (*fun_ptr)(double);
               } fun_arr[] = { entry(sin),
                                   entry(sinh),
                                   entry(cos),
                                   entry(cosh),
                                   entry(tan),
                                   entry(tanh),
                                   entry(exp),
                                   entry(fabs),
                                   entry(log),
                                   entry(log10),
                                   entry(asin),
                                   entry(acos),
                                   entry(atan),
                                   entry(ceil),
                                   entry(floor),
                                   entry(sqrt),
                                   entry(cfact),
                                 };
static struct const_struct {  char  *name;
                              double value;
                           } consts[] = {
                               { "pi",3.14159 },
                                   };

int main(int argc, char **argv)
{
      char   *c;
      int x;

      c = (char *)malloc(255);

      if (argc < 2)
      {
        printf("Welcome to the Calculator.Enter help at ");
        printf("any stage to get instructions.\n\n")     ;
        printf("      Builtin Functions Available  \n\n");
        printf("      sin,cos,tan,log (base e),log10 \n");
        printf("      asin,acos,atan,floor,ceil,sqrt \n");
        printf("      sinh,cosh,tanh,exp,fabs,cfact\n");
        printf("      power raising is indicated by **\n");
        printf("       e.g   2**3  will print 8.   \n\n");
        printf("      Constants available          \n\n");
        printf("        pi  = 3.14159              \n\n");

        printf("Enter an expression\n");
      }

      for(;;printf("Enter another expression\n"))
      {
        if (argc > 1)
        {
          strcpy(c,*(argv+1));
          for (x=2;x<argc;x++) strcat(c,*(argv+x));
        }
        else getinp( c )     ;   /*  get the expression            */

        temp = 0x01; /* mod PE 1997-08-01 to prevent overflows of
                        static arrays */
                        
        strcpy(c,sttoup(c))  ;   /*  convert input to upper case   */

        elimbl( c );             /*  remove embedded blanks        */

        if ((strcmp(c,"QUIT") == 0)  ||
            (*c == '\0') ||
            (*c == ' ') ) break;
        if (strncmp(c,"HELP",4) == 0) /*  was help requested?       */
          prthlp( c );
        else
         if (*(c+1) == '=')       /*  assignment statement          */
            (void)evaluate(c,'\0');  /*  ignore returned value  */
         else
           if (*c == '?')         /*  print statement               */
             prtvar( c );
           else
              printf("Calculated Value is %-15.6f\n",evaluate(c,'\0'));
        if (argc > 1) break;
      }
      printf("Thank you for using the calculator\n");
      free( c )   ;
      return (0);
}

/***********************************************************************
**                                                                    **
**  Function  :  getinp                                               **
**                                                                    **
**  Purpose   :  The function reads data from the terminal & copies   **
**               it as a string into the pointer specified.           **
**                                                                    **
**                                                                    **
***********************************************************************/

static void getinp(char *inc)
{
    if (fgets(inc,255,stdin) != NULL) inc[strlen(inc)-1] = '\0';
    else
    {
      printf("end of file encountered on terminal input\n");
      *inc = '\0';
    }
}

/***********************************************************************
**                                                                    **
**  Function  :  prthlp                                               **
**                                                                    **
**  Purpose   :  The function prints the help for using the calculat- **
**               or.It checks if a function name has been entered     **
**               along with help & if so,it calls that function's     **
**               help by calling the function pointer in the          **
**               function array's hlp_ptr.                            **
**                                                                    **
**               As new functions are added,put a help function for   **
**               it with the name h+the function name & declare it    **
**               below & also put in its entry using ENTRY1 into      **
**               the array hlp_arr.                                   **
**                                                                    **
***********************************************************************/

#define  MAX2 (sizeof hlp_arr/sizeof(struct hlp_det))
#define  ENTRY1(a)  {#a,h##a}

static void prthlp(char *c)
{  int i;

   struct hlp_det { char   *fname   ;   /* function name           */
                    void  (*hlp_ptr)(void);
                               /* function's help function*/
                  } hlp_arr[] = { ENTRY1(sin),
                                      ENTRY1(sinh),
                                      ENTRY1(cos),
                                      ENTRY1(cosh),
                                      ENTRY1(tan),
                                      ENTRY1(tanh),
                                      ENTRY1(exp),
                                      ENTRY1(fabs),
                                      ENTRY1(log),
                                      ENTRY1(log10),
                                      ENTRY1(asin),
                                      ENTRY1(acos),
                                      ENTRY1(atan),
                                      ENTRY1(ceil),
                                      ENTRY1(floor),
                                      ENTRY1(sqrt),
                                      ENTRY1(cfact),
                                      };
/***********************************************************************
**  If a function name was entered along with help,then check if help **
**  was setup in the hlp_arr,If it was,then call the help function ptr**
**  for that help to print the help.                                  **
***********************************************************************/
      if (strlen(c) > 4)
       { for(i=0;i<MAX2;i++)
           if (strcmp(c+4,sttoup(hlp_arr[i].fname))==0)
             break;
         if (i>=MAX2)
           printf("Unknown function or Help not available for %s\n",
                                                              c+4);
         else
           hlp_arr[i].hlp_ptr();
         return;
       }

      printf("           Help for using the calculator     \n\n\n");
      printf("  The calculator allows you to evaluate expressions\n");
      printf("  It allows defining variables & displaying their  \n");
      printf("  values.\n\n");
      printf("  The following shows the syntax for each command\n\n");
      printf("           Defining a Variable\n\n");
      printf("  Only one character variables are allowed & they  \n");
      printf("  are defined when their value is equated to an    \n");
      printf("  expression or value like in the examples below.  \n");
      printf("  Note all variables converted to capital letters\n\n");
      printf("    a=25       will define a variable A & assign 25\n");
      printf("               to it.                              \n");
      printf("    a=12.4*(34.345-12/34)+145  will define A & set \n");
      printf("               the value the expression evaluates  \n");
      printf("               to it.                              \n");
      printf("    b=a+(a/12)-a*3+a   will define B & evaluate the\n");
      printf("               expression & set that value.Note no \n");
      printf("               check is done to determine if A has \n");
      printf("               been defined before.              \n\n");
      printf("    Just entering expression will evaluate it and\n\n");
      printf("    print the value.                             \n\n");
      printf("    e.g    12*2.3    will print 27.6               \n");
      printf("           a+b       will print the value of A+B \n\n");
      printf(" Note: See that there are matching parantheses &   \n");
      printf("       the syntax of the expression is correct,else\n");
      printf("       the program may goof.                     \n\n");
      printf("       Printing the variable values              \n\n");
      printf("  Just enter ?a,b,c..   This will print the values \n");
      printf("                        of A,B & C                 \n");
      printf("  Entering a ? will print all variables defined  \n\n");
      printf("       Exiting the Calculator                    \n\n");
      printf(" Just type in quit or hit ENTER without any data \n\n");
      printf(" Entering help along with a function name will give\n");
      printf(" help on that function.                          \n\n");
      printf("      Builtin Functions Available  \n\n");
      printf("      sin,cos,tan,log (base e),log10 \n");
      printf("      asin,acos,atan,floor,ceil,sqrt \n");
      printf("      sinh,cosh,tanh,exp,fabs,cfact\n\n");
      printf("      power raising is indicated by **\n");
      printf("       e.g   2**3  will print 8.   \n\n");
      printf("      Constants available          \n\n");
      printf("      pi  =  3.14159               \n\n");
}

static void hsin(void)
{     printf("SIN(x)  evaluates the SINE of x,where x is \n");
      printf("        in radians.To convert radians to \n");
      printf("        degrees use x * (180/(355/113))  \n");
}

static void hasin(void)
{     printf("ASIN(x)  evaluates the arc sine of x,the  \n");
      printf("        result is in radians.To convert  \n");
      printf("        radians to degrees multiply the  \n");
      printf("        result by (180/(355/113)).       \n");
}

static void hsinh(void)
{     printf("SINH(x) evaluates the hyperbolic SINE of x \n");
}

static void hcos(void)
{     printf("COS(x)  evaluates the COSINE of x,where x is \n");
      printf("        in radians.To convert radians to \n");
      printf("        degrees use x * (180/(355/113))  \n");
}

static void hacos(void)
{     printf("ACOS(x)  evaluates the arc cosine of x,the  \n");
      printf("        result is in radians.To convert  \n");
      printf("        radians to degrees multiply the  \n");
      printf("        result by (180/(355/113)).       \n");
}

static void hcosh(void)
{     printf("SINH(x) evaluates the hyperbolic COSINE of x \n");
}

static void htan(void)
{     printf("TAN(x)  evaluates the TANGENT of x,where x is \n");
      printf("        in radians.To convert radians to \n");
      printf("        degrees use x * (180/(355/113))  \n");
}

static void hatan(void)
{     printf("ATAS(x)  evaluates the arc tangent of x,the  \n");
      printf("        result is in radians.To convert  \n");
      printf("        radians to degrees multiply the  \n");
      printf("        result by (180/(355/113)).       \n");
}

static void htanh(void)
{     printf("TANH(x) evaluates the hyperbolic TANGENT of x \n");
}

static void hlog(void)
{     printf("LOG(x)    gets the natural logarithm of x   \n");
}

static void hlog10(void)
{     printf("LOG10(x)  gets the logarithm to the base 10 of x\n");
}

static void hexp(void)
{     printf("EXP(x)    evaluates e**x where e = 2.718..\n");
}

static void hcfact(void)
{     printf("CFACT(x)  evaluates the factorial of x.x is \n");
      printf("        converted to the nearest integer first.\n");
}

static void hceil(void)
{     printf("CEIL(x)   gets the next higher integer value  \n");
      printf("          of x.E.G ceil(2.3) = 3.0            \n");
      printf("               E.G ceil(-2.3) = -2.0          \n");
}

static void hfloor(void)
{     printf("FLOOR(x)  gets the integer such that it is <= \n");
      printf("          x.   E.G floor(2.3)  = 2.0          \n");
      printf("               E.G floor(-2.3) = -3.0         \n");
}

static void hsqrt(void)
{     printf("SQRT(x)   gets the square root of x           \n");
}

static void hfabs(void)
{     printf("FABS(x)   gets the absolute value of x.       \n");
      printf("          of x.E.G fabs(2-3) = 1.0            \n");
}

/***********************************************************************
**                                                                    **
**  Function  :  elimbl                                               **
**                                                                    **
**  Purpose   :  This receives a string pointer & it eliminates all   **
**               blanks from it.                                      **
**                                                                    **
***********************************************************************/

static void elimbl(char *inc)
{  char  *tmp;

   for(tmp=inc;*inc != '\0';inc++)
    { if (*inc != ' ')
        *tmp++ = *inc;
    }
   *tmp = '\0';
}

/***********************************************************************
**                                                                    **
**  Function  :  prtvar                                               **
**                                                                    **
**  Purpose   :  This receives a string which has a statement of the  **
**               form "?A,B,C.." where A,B & C are variables,or just  **
**               a ? to signify dumping all variables declared.The    **
**               declared variables have in the variables position    **
**               of array dcl_vars a 'y'.                             **
**                                                                    **
**               This routine takes each variable & gets it's value   **
**               from the variable storage vars[257] & prints         **
**               the value on the terminal.                           **
**                                                                    **
**                                                                    **
***********************************************************************/

static void prtvar(char *inc)
{  char  *c,i;

      c  =  inc;

      for(;(*c != '\0');c++)
      {  if (isalpha(*c))
           if (dcl_vars[*c] != 'y')
             printf("%c is not defined\n",*c);
           else
             printf("    %c = %-15.6f \n",*c,vars[*c]);
      }
/***********************************************************************
**   If only a ? was entered,then check all variables in dcl_vars     **
**   & select only those which are defined & print the values.        **
***********************************************************************/
      if (strlen(inc)==1)
       { for(i='A';i<='Z';i++)
          if (dcl_vars[i]=='y')
             printf("    %c = %-15.6f \n",i,vars[i]);
       }
}

/***********************************************************************
**                                                                    **
**    Function   :  evaluate                                          **
**                                                                    **
**    Purpose    :  This function recursively calls itself to         **
**                  evaluate the value of an expression given         **
**                  to it until the delimiter.                        **
**                                                                    **
**    Logic      :  The expression is first converted to polish       **
**                  postfix notation & then evaluated.                **
**                  Each variable's value is held in the table        **
**                  vars in that variables position.                  **
**                                                                    **
**                  If a function was entered,then the function must  **
**                  be followed immediately by a '(' & it must have   **
**                  a corresponding ')'.Now as soon as a function is  **
**                  recognised (a string of an alphanumeric chars     **
**                  terminated by a '(' ), then a recursive           **
**                  call is made to first calculate the value         **
**                  in the brackets (logical.That's why a delimiter   **
**                  is passed as a parameter to this function.so      **
**                  an expression like LOG10(A+B*(D-E))+23 will       **
**                  first evaluate using recursion (A+B*(D-E)) &then  **
**                  evaluate the LOG10 for it & add 23 in the end.    **
**                  For keeping track of when to stop evaluating      **
**                  a variable j keeps track of how many open brackets**
**                  we have passed.We must encounter that many closed **
**                  brackets before the expression's value is retu-   **
**                  rned to evaluate the function.                    **
**                                                                    **
**                  Entering an invalid expression may crash the      **
**                  program.E.G  LOG(B*(LOG10(A+B) This has fewer     **
**                  parantheses & evaluates to log(0) which is minus  **
**                  infinity & hence invalid.                         **
**                                                                    **
**                  All constants & functions which are evaluated     **
**                  are kept track of as temporary variables in the   **
**                  same variable array vars.Temporary variables      **
**                  position's in the array range from x'01' upwards. **
**                  If you use about 190 constants or more in any     **
**                  expression you'll start overwriting legitimate    **
**                  variables entered by the user (which range from   **
**                  A thru Z,i.e 191 (x'C1' to x'E8' in EBCDIC)).     **
**                                                                    **
***********************************************************************/

static double evaluate(char *c, int delim)
{  char     save_var,outstr[255];
   char     stack[50],fun_name[30],*fun;
   int      stkval[50],val,i,j,k,stktop=-1;
   double   const_num;

      outstr[0] = '\0';
      j             = 0   ;
      if (*(c+1) == '=')          /*  If an assignment              */
       {  save_var  =  *c ;
          c++             ;      /*  By pass variable name         */
          c++             ;      /*  By pass '=' symbol            */
          vars[save_var] = evaluate(c,'\0');
          dcl_vars[save_var] = 'y'             ;
          printf("                %c = %-15.6f\n",save_var,
                                                 vars[save_var]);
          return( vars[save_var]);
       }
      else
       {  for(;*c != '\0';c++)    /*  take each character..         */
           {  if ((j == 0)  &&    /*  if end of evaluation..        */
                  (*c == delim))
                break;
              if (isalpha(*c))    /*  is char an alpha?             */
               { if ((isalpha(*(c+1))) ||   /* is it a function?    */
                     (isdigit(*(c+1))) ||   /* or a constant?       */
                     (*(c+1)=='('))
                  { fun        =  fun_name  ;
                    while(isalpha(*c) || isdigit(*c))
                       *fun++  = *c++       ;
                    *fun       =  '\0'      ;
                    fun        =  fun_name  ;
                    if (*c != '(')          /* it's a constant      */
                     { for(i=0;i<CONSTS;i++)
                        if (strcmp(fun,sttoup(consts[i].name))==0)
                           break;
                       if (i>=CONSTS)
                        { printf("Constant not found %s \n",fun);
                          const_num = 0;
                        }
                       else
                        const_num = consts[i].value;
                       vars[temp] = const_num;
                       catchr(outstr,&temp);
                       temp++             ;
                       c--                ;
                     }
                    else       /* it's a function     */
                     { const_num  =  evaluate(c,')');
                       for(i=0;i<MAX;i++)
                        if (strcmp(fun,sttoup(fun_arr[i].fname))
                                                   == 0)
                          break;
                       if (i >= MAX)
                        { printf("Invalid function =>%s<=\n",fun);
                          return( 0 )  ;
                        }
                       vars[temp] =
                         fun_arr[i].fun_ptr(const_num);
                       catchr(outstr,&temp);
                       temp++ ;
                       /*********************************************
                       ** Now bypass the function & it's parameters**
                       ** in the input string.                     **
                       *********************************************/
                       k  =  0;
                       for(;*c!= '\0';c++)
                        { if (*c == '(')
                            k++;
                          if (*c == ')')
                            k--;
                          if ((*c == ')') && (k == 0))
                            break;
                        }
                     }
                  }
                 else         /* not a function,just a variable    */
                   catchr(outstr,c);
               }
              else
               /*****************************************************
               **  check if a constant has been entered,if so      **
               **  get it's value & point c at the last digit.     **
               **  add temp to the polish postfix string, &        **
               **  update the tempoaray variable temp.             **
               *****************************************************/
               { if (isdigit(*c) || (*c == '.'))
                  { vars[temp] = strtod(c,&c);
                    if (*c == '.') c++; /* mod PE 1995-11-06 */
                    catchr(outstr,&temp);
                    temp++ ;
                    c--    ;
                  }
                 else
                 /***************************************************
                 ** So possibly an arithmetic operator has been    **
                 ** encountered.Stack it appropriately.            **
                 ***************************************************/
                  { switch( *c )
                    { case '(' : stack[++stktop] = *c;
                                 stkval[stktop]      = 0 ;
                                 j++  ;
                                 break;
                      case ')' : for(;stktop >= 0;--stktop)
                                  if (stack[stktop] == '(')
                                   break;
                                  else
                                   catchr(outstr,&stack[stktop]);
                                 j--;
                                 if ((j == 0) && (delim == ')'))
                                   c--;
                                 if (stktop < 0)
                                  { printf("Invalid parantheses\n");
                                    return( 0 );
                                  }
                                 stktop--;
                                 break   ;
                      default  : if ((*c == '+') || (*c == '-'))
                                   val  =  1 ;
                                 if ((*c == '*') || (*c == '/') ||
                                     (*c == '%'))
                                   val  =  2 ;
                                 /**************************************
                                 ** if ** was entered convert it to a **
                                 ** > & update the input string ptr c **
                                 ** to point to it accordingly.Also   **
                                 ** power raising has top priority.   **
                                 **************************************/
                                 if ((*c == '*') && (*(c+1) == '*'))
                                  { *(c+1) =  '>' ;
                                    c++           ;
                                    val    =  3   ;
                                  }
                                 for(;stktop>=0;--stktop)
                                   if (stkval[stktop] < val)
                                      break;
                                   else
                                    catchr(outstr,&stack[stktop]);
                                 stack[++stktop] = *c;
                                 stkval[stktop]      = val;
                                 break;
                    }
                  }
               }
           }
       }

      for(;stktop>=0;stktop--)
       catchr(outstr,&stack[stktop]);

      return( compute(outstr) );
}

/***********************************************************************
**                                                                    **
**  Function  :  catchr                                               **
**                                                                    **
**  Purpose   :  This receives a string pointer & an address of a     **
**               character.This function then concatenates the        **
**               character to the end of the string.                  **
**                                                                    **
***********************************************************************/

static void catchr(char *c,char *c1)
{ 
  c+=strlen(c);
  *c++ = *c1;
  *c = '\0';
}  

/***********************************************************************
**                                                                    **
**  Function  :  compute                                              **
**                                                                    **
**  Purpose   :  This receives a string which has an expression in    **
**               polish postfix format.It then evaluates the expr-    **
**               ession and returns the value as a double float       **
**               decimal.                                             **
**                                                                    **
**               Raising to the power is indicated by a '>'.          **
**                                                                    **
**                                                                    **
***********************************************************************/

static double compute(char *inc)
{  char   *c;
   double value,stack[50];
   int    stktop = -1,i1,i2;

      c  =  inc;
      for(;*c != '\0';c++)
       { switch( *c )
          {  case  '+' :  value = stack[stktop-1]      +
                                  stack[stktop]      ;
                          stack[--stktop] = value;
                          break;
             case  '-' :  value = stack[stktop-1] -
                                  stack[stktop]      ;
                          stack[--stktop] = value;
                          break;
             case  '/' :  if (stack[stktop] == 0)
                            { printf("Divide by zero attempted\n");
                              return( 0 );
                            }
                          value = stack[stktop-1] /
                                  stack[stktop]      ;
                          stack[--stktop] = value;
                          break;
             case  '%' :  if (stack[stktop] == 0)
                            { printf("Divide by zero attempted\n");
                              return( 0 );
                            }
                          i1    = stack[stktop-1];
                          i2    = stack[stktop]      ;

                          value = i1 % i2            ;

                          stack[--stktop] = value;
                          break;
             case  '*' :  value = stack[stktop-1] *
                                  stack[stktop]      ;
                          stack[--stktop] = value;
                          break;
             case  '>' :  value = pow(stack[stktop-1],
                                  stack[stktop])     ;
                          stack[--stktop] = value;
                          break;
             default   :  stack[++stktop] = vars[*c];
                          break;
          }
       }
      return( stack[0] );
}

static char *sttoup(char *x)
{
  static char *y = NULL;
  int i,len;

  len = strlen(x);
  y = (char *)realloc(y,len+1);
  for (i=0;i<=len;i++) y[i] = toupper(x[i]);
  return (y);
}

static double cfact(double n)
{
  double i,ans;

  if (n < 0) return (0.0);
  for (i=1,ans=1;i<=n;i++) ans*=i;
  return (ans);
}

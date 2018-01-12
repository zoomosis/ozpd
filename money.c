/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the public domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  Money - work out how to get rich quick                           */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "getopts.h"

int main(int argc, char **argv)
{
  static float netweek,intborr,intsave,houseinc,rent,borrow;
  static float totsave,totowe,houseval,interest,incnet,incrent;
  static float totalworth, difftot;
  static int   i,j,years,defs;

  opt_t opttable[]=
  {
    { "n", OPTFLOAT, &netweek },
    { "in", OPTFLOAT, &incnet },
    { "ib", OPTFLOAT, &intborr },
    { "is", OPTFLOAT, &intsave },
    { "hv", OPTFLOAT, &houseinc },
    { "r", OPTFLOAT, &rent },
    { "ir", OPTFLOAT, &incrent },
    { "b", OPTFLOAT, &borrow },
    { "y", OPTINT, &years },
    { "d", OPTBOOL, &defs },
    { NULL, 0, NULL }
  };

  netweek=500.0;   /* how much you can save a week after paying rent */
  incnet=5.0;	   /* how much net pay increases each year (%) */
  intborr=15.0;    /* how much you pay back on your borrowings */
  intsave=11.0;    /* how much interest you get after tax on savings */
  houseinc=10.0;   /*how much the value of houses goes up */
  rent=155.0;       /* how much rent you pay a week */
  incrent=5.0;     /* how much rent increases each year */
  borrow=100000.0;   /* how much you want to borrow */
  years=10;         /* how many years you want to display */
  defs=0;         /* take all defaults? no. */

  if (getopts(argc,argv,opttable) == 1)
  {
    printf("money - get rich quick!!!\n\n");
    printf("n = netweek - n500\n");
    printf("in = increase netweek each year -in5\n");
    printf("ib = interest on borrrowings -ib15\n");
    printf("is = interest on savings -is11\n");
    printf("hv = house value increment -hv10\n");
    printf("r = rent you pay in 1 week -r155\n");
    printf("ir = yearly increase in rent -ir5\n");
    printf("b = how much you want to borrow -b100000\n");
    printf("y = how many years you want to calculate -y10\n");
    printf("d = take all defaults?\n");
    return (EXIT_SUCCESS);
  }

  totsave=0;
  totowe=borrow;
  totalworth=0;
  difftot=0;
  houseval=borrow;
  printf("year     save     borrow      house"
      "   totalworth   difftot\n");
  printf("%2d %10.0f %10.0f %10.0f %10.0f %10.0f\n",
      0,totsave,totowe,houseval,totalworth,difftot);
  for (i=1;i<=years;i=i+1)
  {
    for (j=1;j<=12;j=j+1)
    {
      totsave=totsave+totsave*(intsave/100)/12+365.25/7.0/12.0*netweek;
      if (totowe > 0) interest=totowe*(intborr/100)/12;
      else interest=totowe*(intsave/100)/12;
      totowe=totowe+interest-(netweek+rent)*365.25/7.0/12.0;
    }
    houseval=houseval*(1+houseinc/100);
    totalworth = houseval - totowe;
    difftot = totalworth - totsave;
    rent=rent*(1+incrent/100);
    netweek=netweek*(1+incnet/100);
    printf("%2d %10.0f %10.0f %10.0f %10.0f %10.0f\n",
        i,totsave, totowe, houseval, totalworth, difftot);
  }
  return (EXIT_SUCCESS);
}

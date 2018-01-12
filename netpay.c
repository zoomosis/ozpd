/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Dedicated to the public domain.                                  */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  netpay - work out your net pay given gross yearly pay            */
/*  array of tax rates will need to be updated.                      */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NUMENT ((sizeof taxscale)/(sizeof(struct taxent)))

main(int argc, char **argv)
{
  struct taxent { float wage; float tax1; float rate; }
      taxscale[] = {
      { 95000.0 , 28200.0  , 47.0 },
      { 63000.0 , 14760.0  , 42.0 },
      { 21600.0 ,  2340.0  , 30.0 },
      {  6000.0 ,     0.0  , 15.0 },
      {     0.0 ,     0.0  ,  0.0 },
      };
  float medirate = 0.015;
  float grosspay, blocktax, remtax, fulltax, netpay;
  int x;

  if (argc < 2)
  {
    printf("netpay - Australian tax rates of 2006-07-01\n");
    printf("assuming medicare to be paid on entire gross wage\n");
    printf("what is your gross yearly pay?\n");
    scanf("%f",&grosspay);
  }
  else grosspay = atof(*(argv+1));
  for (x=0;x<NUMENT;x++)
  {
    if (grosspay >= taxscale[x].wage)
    {
      blocktax = taxscale[x].tax1;
      remtax = taxscale[x].rate *
          (grosspay - taxscale[x].wage)/100;
      break;
    }
  }
  fulltax = blocktax + remtax;
  fulltax += grosspay * medirate;
  netpay = grosspay - fulltax;
  printf("gross yearly pay %9.2f  yearly tax %9.2f  net yearly pay %9.2f\n",
      grosspay, fulltax, netpay);
  printf("gross weekly pay %9.2f  weekly tax %9.2f  net weekly pay %9.2f\n",
      grosspay/52, fulltax/52, netpay/52);
  return (EXIT_SUCCESS);
}

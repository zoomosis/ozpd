/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  timesht - produce a timesheet given raw values                   */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>

typedef struct {
    int hh;
    int mm;
} hhmm;

static void hhmminit(hhmm *x1);
static void addhhmm(hhmm *res, hhmm *x1, hhmm *x2);
static void subhhmm(hhmm *res, hhmm *x1, hhmm *x2);

#define MAXTIMES 16

static char buf[200];
static hhmm rawtime[MAXTIMES];

static hhmm period;
static hhmm day;
static hhmm lunch;
static hhmm work;

static hhmm totalday;
static hhmm totallunch;
static hhmm totalwork;

int main(void)
{
    int rc;
    int x;
    
    while (fgets(buf, sizeof buf, stdin) != NULL)
    {
        rc = sscanf(buf, 
                    "%d:%d %d:%d %d:%d %d:%d"
                    " %d:%d %d:%d %d:%d %d:%d"
                    " %d:%d %d:%d %d:%d %d:%d"
                    " %d:%d %d:%d %d:%d %d:%d",
                    &rawtime[0].hh,
                    &rawtime[0].mm,
                    &rawtime[1].hh,
                    &rawtime[1].mm,
                    &rawtime[2].hh,
                    &rawtime[2].mm,
                    &rawtime[3].hh,
                    &rawtime[3].mm,
                    &rawtime[4].hh,
                    &rawtime[4].mm,
                    &rawtime[5].hh,
                    &rawtime[5].mm,
                    &rawtime[6].hh,
                    &rawtime[6].mm,
                    &rawtime[7].hh,
                    &rawtime[7].mm,
                    &rawtime[8].hh,
                    &rawtime[8].mm,
                    &rawtime[9].hh,
                    &rawtime[9].mm,
                    &rawtime[10].hh,
                    &rawtime[10].mm,
                    &rawtime[11].hh,
                    &rawtime[11].mm,
                    &rawtime[12].hh,
                    &rawtime[12].mm,
                    &rawtime[13].hh,
                    &rawtime[13].mm,
                    &rawtime[14].hh,
                    &rawtime[14].mm,
                    &rawtime[15].hh,
                    &rawtime[15].mm);
                    
        if (rc < 4) continue;

        if (rc == (MAXTIMES * 2))
        {
            printf("WARNING!!! Need to update internal tables!\n");
        }

        hhmminit(&work);
        hhmminit(&lunch);
        
        subhhmm(&work, &rawtime[1], &rawtime[0]);

        for (x = 4; (x+2) <= rc; x += 2)
        {
            hhmminit(&period);
            subhhmm(&period, &rawtime[x/2], &rawtime[x/2-1]);
            if (((x/2) % 2) != 0)
            {
                addhhmm(&work, &work, &period);
            }
            else
            {
                addhhmm(&lunch, &lunch, &period);
            }   
        }
        
        addhhmm(&day, &work, &lunch);
        
        addhhmm(&totalday, &totalday, &day);
        addhhmm(&totallunch, &totallunch, &lunch);
        addhhmm(&totalwork, &totalwork, &work);
        
        printf("lunch %02d:%02d work %02d:%02d    (%f)\n",
               lunch.hh, lunch.mm, work.hh, work.mm, work.hh + work.mm/60.0);
    }
    printf("\ntotal work %02d:%02d    (%f)\n", totalwork.hh, totalwork.mm,
           totalwork.hh + totalwork.mm/60.0);
    return (0);
}


static void hhmminit(hhmm *x1)
{
    x1->hh = x1->mm = 0;
    return;
}


static void addhhmm(hhmm *res, hhmm *x1, hhmm *x2)
{
    res->mm = x1->mm + x2->mm;
    res->hh = x1->hh + x2->hh + res->mm / 60;
    res->mm %= 60;
    return;
}


static void subhhmm(hhmm *res, hhmm *x1, hhmm *x2)
{
    res->mm = x1->mm - x2->mm;
    res->hh = x1->hh - x2->hh;
    if (res->mm < 0)
    {
        res->hh -= (-res->mm / 60) + 1;
        res->mm = 60 - (-res->mm % 60);
    }
    if (res->hh < 0)
    {
        res->hh += 24;
    }
    return;
}



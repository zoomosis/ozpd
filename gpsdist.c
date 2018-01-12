/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  gpsdist - calculate gps averages from raw data                   */
/*                                                                   */
/*********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "gccalc.h"

#define pi 3.14159

typedef struct {
    double lat;
    double lon;
} entry;

static int nextload(void);
static void extract(entry *e, char *buf);
static void initcalcs(void);
static void docalcs(void);
static void finalcalcs(void);
static void average(entry *e, long secs, double *avglat, double *avglon);

static entry *chunk;
static double blat;
static double blon;
static long secs;
static long numcases;
static char buf[200];
static FILE *fp;

static double favg;
static double aavg;
static double totalf;
static double totala;

static long firstwins = 0;
static long avgwins = 0;

static double favgArea;
static double aavgArea;
static double ftotalArea;
static double atotalArea;

static double rms;
static double avgerror;
static double avgsrch;
static double maxerror;
static double maxsrch;

static double avgerrorTotal;
static double avgsrchTotal;

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        printf("usage: <infile> <base lat> <base lon> <secs>\n");
        return (EXIT_FAILURE);
    }

    blat = strtod(*(argv + 2), NULL);    
    blon = strtod(*(argv + 3), NULL);
    secs = strtol(*(argv + 4), NULL, 0);
    
    fp = fopen(*(argv + 1), "r");
    if (fp == NULL)
    {
        printf("failed to open file %s\n", *(argv + 1));
        return (EXIT_FAILURE);
    }

    initcalcs();
    numcases = 0;    
    while (nextload())
    {
        docalcs();
        numcases++;
    }
    finalcalcs();
    
    fclose(fp);

    /*    
    printf("numcases %ld first avgerror %lf, avg avgerror %lf\n", 
           numcases, favg, aavg);
    printf("first wins %ld, average wins %ld\n", firstwins, avgwins);
    printf("avg win%% is %lf\n", (double)avgwins/(firstwins + avgwins));
    printf("first avgarea %lf, average avgarea %lf\n",
           favgArea, aavgArea);
    */
    
    printf("%d %lf %lf %lf %lf %lf\n",
           secs, rms, avgerror, maxerror, avgsrch * pi, maxsrch * pi);
    return (0);
}

static int nextload(void)
{
    static int first = 1;
    long cnt;
    
    if (first)
    {
        first = 0;
        chunk = malloc(secs * sizeof(entry));
        assert(chunk != NULL);
        cnt = 0;
        while ((cnt < secs) && (fgets(buf, sizeof buf, fp) != NULL))
        {
            extract(&chunk[cnt], buf);
            cnt++;
        }
    }
    else
    {
        memmove(chunk, chunk + 1, (secs - 1) * sizeof(entry));
        fgets(buf, sizeof buf, fp);
        extract(&chunk[secs - 1], buf);
    }
    if (feof(fp)) return (0);
    return (1);
}

static void extract(entry *e, char *buf)
{
    char *p;
    
    e->lat = strtod(buf, &p) / 3600000.0;
    e->lon = strtod(p, NULL) / 3600000.0;
    return;
}

static void initcalcs(void)
{
    totalf = 0.0;
    totala = 0.0;
    ftotalArea = 0.0;
    atotalArea = 0.0;
    maxerror = 0.0;
    avgerrorTotal = 0.0;
    avgsrchTotal = 0.0;
    return;
}

static void docalcs(void)
{
    double avglat, avglon;
    double firstdist;
    double avgdist;
    
    average(chunk, secs, &avglat, &avglon);
    firstdist = degdist(blat, blon, chunk[0].lat, chunk[0].lon);
    avgdist = degdist(blat, blon, avglat, avglon);
    if (firstdist < avgdist) firstwins++;
    else if (avgdist < firstdist) avgwins++;
    totalf += firstdist;
    totala += avgdist;
    ftotalArea += firstdist * firstdist;
    atotalArea += avgdist * avgdist;
    avgerrorTotal += avgdist;
    avgsrchTotal += avgdist * avgdist;
    if (avgdist > maxerror)
    {
        maxerror = avgdist;
    }
    return;
}

static void finalcalcs(void)
{
    favg = totalf / numcases;
    aavg = totala / numcases;
    favgArea = ftotalArea / numcases;
    aavgArea = atotalArea / numcases;
    avgsrch = avgsrchTotal / numcases;
    avgerror = avgerrorTotal / numcases;
    maxsrch = maxerror * maxerror;
    rms = sqrt(avgsrch);
    return;
}

static void average(entry *e, long secs, double *avglat, double *avglon)
{
    double tlat = 0.0;
    double tlon = 0.0;
    long cnt;

    for (cnt = 0; cnt < secs; cnt++)
    {
        tlat = tlat + e[cnt].lat;
        tlon = tlon + e[cnt].lon;
    }
    *avglat = tlat / secs;
    *avglon = tlon / secs;
    return;
}

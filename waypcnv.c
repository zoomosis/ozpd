/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards.                            */
/*  Released to the public domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  waypcnv - convert waypoints and tracks from gardown format to    */
/*  waypoint+ format and vice versa.                                 */
/*                                                                   */
/*  waypoint+ format must be WGS84 and ddd mm.mmm                    */
/*                                                                   */
/*  The program reads from stdin and writes to stdout.               */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "strinl.h"
#include "dow.h"

static char buf[500];
static char *mtab[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static char *dtab[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static void wp2g_wp(char *buf);
static void latnp(double x, char *buf, int north);
static void datnp(char *from, char *to);
static void g2wp_wp(char *buf);
static void latpn(char *from, char *to, int north);
static void datpn(char *from, char *to);
static void wp2g_t(char *buf);
static void g2wp_t(char *buf);

int main(void)
{
    fgets(buf, sizeof buf, stdin);
    if (strncmp(buf, "Datum", 5) == 0)
    {
        fgets(buf, sizeof buf, stdin);
    }
    while (!feof(stdin))
    {
        strinl(buf);
        if (memcmp(buf, "WP", 2) == 0)
        {
            wp2g_wp(buf);
        }
        else if (memcmp(buf, "W", 1) == 0)
        {
            g2wp_wp(buf);
        }
        else if (memcmp(buf, "TP", 2) == 0)
        {
            wp2g_t(buf);
        }
        else if (memcmp(buf, "T", 1) == 0)
        {
            g2wp_t(buf);
        }
        printf("%s\n", buf);
        fgets(buf, sizeof buf, stdin);
    }
    return (0);
}

static void wp2g_wp(char *buf)
{
    char buf2[77];
    double x;
    char *p;
    
    memset(buf2, ' ', sizeof buf2);
    buf2[76] = '\0';
    buf2[0] = 'W';
    p = strchr(buf + 6, ',');
    memcpy(buf2 + 3, buf + 6, p - (buf + 6));
    p++;
    sscanf(p, "%lf", &x);
    p = strchr(p, ',') + 1;
    latnp(x, buf2 + 10, 1);
    sscanf(p, "%lf", &x);
    p = strchr(p, ',') + 1;
    latnp(x, buf2 + 22, 0);
    datnp(p, buf2 + 35);
    memcpy(buf2 + 60, p + 20, 16);
    strcpy(buf, buf2);
    return;
}

static void latnp(double x, char *buf, int north)
{
    char buf2[50];
    int y;
    
    if (x < 0.0)
    {
        buf2[0] = north ? 'S' : 'W';
        x = -x;
    }
    else
    {
        buf2[0] = north ? 'N' : 'E';
    }
    y = (int)x;
    if (north)
    {
        sprintf(buf2 + 1, "%02d", y);
    }
    else
    {
        sprintf(buf2 + 1, "%03d", y);
    }
    x -= y;
    x *= 100.0;
    y = (int)x;
    sprintf(buf2 + 4 - north, " %02d", y);
    x -= y;
    x *= 10000.0;
    x += 0.5;
    y = (int)x;
    sprintf(buf2 + 7 - north, ".%04d", y);
    memcpy(buf, buf2, strlen(buf2));
    return;
}

static void datnp(char *from, char *to)
{
    int dofw;
    int day;
    int month;
    int year;
    
    memset(to, ' ', 24);
    memcpy(to + 20, from + 6, 4);
    memcpy(to + 11, from + 11, 8);
    memcpy(to + 8, from + 3, 2);
    month = atoi(from);
    memcpy(to + 4, mtab[month - 1], 3);
    day = atoi(from + 3);
    year = atoi(from + 6);
    dofw = dow(year, month, day);
    memcpy(to, dtab[dofw], 3);
    return;
}

static void g2wp_wp(char *buf)
{
    char buf2[73];
    
    memset(buf2, ',', sizeof buf2);
    buf2[72] = '\0';
    memcpy(buf2, "WP,DM", 5);
    memcpy(buf2 + 6, buf + 3, 6);
    latpn(buf + 10, buf2 + 13, 1);
    latpn(buf + 22, buf2 + 24, 0);
    datpn(buf + 35, buf2 + 36);
    memcpy(buf2 + 56, buf + 60, 16);
    strcpy(buf, buf2);
    return;
}

static void latpn(char *from, char *to, int north)
{
    if ((*from == 'S') || (*from == 'W'))
    {
        to[0] = '-';
    }
    else
    {
        to[0] = ' ';
    }
    if (north)
    {
        memcpy(to + 1, from + 1, 2);
    }
    else
    {
        memcpy(to + 1, from + 1, 3);
    }
    to[4 - north] = '.';
    memcpy(to + 5 - north, from + 5 - north, 2);
    memcpy(to + 7 - north, from + 8 - north, 4);
    return;
}
   
static void datpn(char *from, char *to)
{
    int month;

    for (month = 0; month < 12; month++)
    {
        if (strncmp(mtab[month], from + 4, 3) == 0)
        {
            break;
        }
    }
    month++;
    sprintf(to, "%02d", month);
    to[2] = '/';
    memcpy(to + 3, from + 8, 2);
    to[5] = '/';
    memcpy(to + 6, from + 20, 4);
    memcpy(to + 11, from + 11, 8);
    return;
}

static void wp2g_t(char *buf)
{
    char buf2[55];
    double x;
    char *p;
    
    memset(buf2, ' ', sizeof buf2);
    buf2[54] = '\0';
    buf2[0] = 'T';
    p = buf + 6;
    sscanf(p, "%lf", &x);
    latnp(x, buf2 + 3, 1);
    p = strchr(p, ',') + 1;
    sscanf(p, "%lf", &x);
    latnp(x, buf2 + 15, 0);
    p = strchr(p, ',') + 1;
    datnp(p, buf2 + 28);
    buf2[53] = buf[strlen(buf) - 1];
    strcpy(buf, buf2);
    return;
}

static void g2wp_t(char *buf)
{
    char buf2[51];
    
    memset(buf2, ',', sizeof buf2);
    buf2[50] = '\0';
    memcpy(buf2, "TP,DM", 5);
    latpn(buf + 3, buf2 + 6, 1);
    latpn(buf + 15, buf2 + 17, 0);
    datpn(buf + 28, buf2 + 29);
    buf2[49] = '0';
    if (strlen(buf) >= 54)
    {
        buf2[49] = buf[53];
    }
    strcpy(buf, buf2);
    return;
}

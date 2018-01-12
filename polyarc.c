/*********************************************************************/
/*                                                                   */
/*  This Program Written by Paul Edwards, 3:711/934@fidonet.         */
/*  Released to the Public Domain                                    */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  polyarc - archive stuff using zip to binkley-style outbounds.    */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "trav.h"
#include "unused.h"
#include "strinl.h"
#include "stritr.h"
#include "boutcnv.h"

static char *pspec;
static char *oloc;
static int myZone;
static int myNet;
static int myNode;
static int myPoint;
static int defZone;

static int process_packet(char *fnm, void *ucb);
static void figure_arc(char *hold, char *bundle, char *pkt);
static void do_zip(char *newfile, char *pkt);
static void deleteOld(char *bundle, char *thisday);
static int deleteOne(char *file, void *ucb);

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("usage: polyarc <packet spec> <outbound root> "
               "<default zone>\n");
        return (EXIT_FAILURE);
    }
    pspec = *(argv + 1);
    oloc = *(argv + 2);
    defZone = atoi(*(argv + 3));
    trav(pspec, NULL, process_packet, NULL);
    return (0);
}

static int process_packet(char *fnm, void *ucb)
{
    unsigned char buf[58];
    char outshort[13];
    char outlong[13];
    char outdir[FILENAME_MAX];
    char bundle[9];
    char holdfile[9];
    char b[FILENAME_MAX];
    char h[FILENAME_MAX];
    FILE *fp;
    int zone, net, node, point;
    
    unused(ucb);
    fp = fopen(fnm, "rb");
    if (fp != NULL)
    {
        fread(buf, sizeof buf, 1, fp);
        fclose(fp);
        myZone = buf[34] | (buf[35] << 8);
        myNet = buf[20] | (buf[21] << 8);
        myNode = buf[0] | (buf[1] << 8);
        myPoint = buf[50] | (buf[51] << 8);
        zone = buf[36] | (buf[37] << 8);
        net = buf[22] | (buf[23] << 8);
        node = buf[2] | (buf[3] << 8);
        point = buf[52] | (buf[53] << 8);
        if (zone == defZone)
        {
            strcpy(outshort, "Outbound");
        }
        else
        {
            sprintf(outshort, "Outbound.%s", int2bout(zone));
        }
        if (point == 0)
        {
            sprintf(outlong, "");
            sprintf(bundle, 
                    "%04x%04x", 
                    ((unsigned)(myNet - net)) & 0xFFFF, 
                    ((unsigned)(myNode - node)) & 0xFFFF);
            sprintf(holdfile, "%04x%04x", net, node);
        }
        else
        {
            sprintf(outlong, "%04x%04x.pnt", net, node);
            sprintf(bundle, 
                    "%04x%04x", 
                    0, 
                    ((unsigned)(myPoint - point)) & 0xFFFF);
            sprintf(holdfile, "%04x%04x", 0, point);
        }
        strcpy(outdir, outshort);
        if (strcmp(outlong, "") != 0)
        {
            strcat(outdir, "\\");
        }
        strcat(outdir, outlong);
        
        strcpy(h, oloc);
        strcat(h, "\\");
        strcat(h, outdir);
        strcat(h, "\\");
        strcat(h, holdfile);
        
        strcpy(b, oloc);
        strcat(b, "\\");
        strcat(b, outdir);
        strcat(b, "\\");
        strcat(b, bundle);
        
        figure_arc(h, b, fnm);
    }    
    return (1);
}

static void figure_arc(char *hold, char *bundle, char *pkt)
{
    char buf[200];
    FILE *f;
    int max = 0;
    time_t tt;
    struct tm *tms;
    static char *days[] = { "su", "mo", "tu", "we", "th", "fr", "sa" };
    int x;
    FILE *tf;
    char newfile[FILENAME_MAX];
    int appendHold;
    char *thisday;
    
    strcat(hold, ".hlo");
    time(&tt);
    tms = localtime(&tt);
    thisday = days[tms->tm_wday];
    strcat(bundle, ".");
    deleteOld(bundle, thisday);
    strcat(bundle, thisday);
    f = fopen(hold, "r");
    if (f != NULL)
    {
        while (fgets(buf, sizeof buf, f) != NULL)
        {
            strinl(buf);
            stritr(buf);
            if (strncmp(buf + 1, bundle, strlen(bundle)) == 0)
            {
                x = atoi(buf + 1 + strlen(bundle));
                if (x > max)
                {
                    max = x;
                }
            }
        }
        fclose(f);
    }
        
    /* until we hit a max of 9, or get a non-existant file, or
       get a non-zero-length file, keep trying */
    appendHold = 0;
    while (max < 10)
    {
        sprintf(newfile, "%s%d", bundle, max);
        tf = fopen(newfile, "rb");
        if (tf == NULL)
        {
            appendHold = 1;
            break;
        }
        if (getc(tf) != EOF)
        {
            fclose(tf);
            break;
        }
        max++;
    }
        
    /* If we exceeded the maximum, I don't know what to do */
    if (max == 10)
    {
        max = 0;
        appendHold = 1;
    }
        
    if (appendHold)
    {
        sprintf(newfile, "%s%d", bundle, max);
        f = fopen(hold, "a");
        if (f != NULL)
        {
            fprintf(f, "#%s\n", newfile);
            fclose(f);
        }
    }
    do_zip(newfile, pkt);
    return;
}

static void do_zip(char *newfile, char *pkt)
{
    char s[FILENAME_MAX + FILENAME_MAX + 50];
    
    sprintf(s, "zip -9jmk %s %s", newfile, pkt);
    printf("%s\n", s);
    system(s);
    return;
}

static void deleteOld(char *bundle, char *thisday)
{
    char buf[FILENAME_MAX];
    
    strcpy(buf, bundle);
    strcat(buf, "*");
    trav(buf, NULL, deleteOne, thisday);
    return;
}

static int deleteOne(char *file, void *ucb)
{
    char *thisday = (char *)ucb;
    FILE *fp;
    
    fp = fopen(file, "rb");
    if (fp != NULL)
    {
        if (fgetc(fp) == EOF)
        {
            fclose(fp);
            if (strncmp(file + strlen(file) - 3, thisday, 2) != 0)
            {
                remove(file);
            }
        }
        else
        {
            fclose(fp);
        }
    }
    return (1);
}

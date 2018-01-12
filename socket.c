/***********************************************************
 * C Socket Interface.
 * -------------------
 * For use with the modified Hercules emulator
 *
 * (see: EZASOKET.ASM)
 *
 * Released to the public domain by Jason Paul Winter in 2009
 *
 ***********************************************************/

#include <string.h> /* for strlen, used by gethostbyname */
#include <stdlib.h> /* for atol, used by inet_aton */
#include <errno.h>  /* for thread safe definition of int errno */

#define gethostbyname gthstbyn  /* don't really use that long name */

#define u_short unsigned short  /* Obvious defines */
#define u_char  unsigned char
#define u_long  unsigned long

#define FIONBIO  1 /* set/clear non-blocking i/o */
#define FIONREAD 2 /* get # bytes to read */

long EZASOKET (void * a, void * b, void * c, void * d,
               void * e, void * f, void * g, void * h,
               void * i, void * j, void * k); /* External linkage */

/*****************************************************************************/

struct in_addr {  /* used by sockaddr_in below */
    union {
        struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { u_short s_w1,s_w2; } S_un_w;
        u_long S_addr;
    } S_un_a;
};

struct sockaddr_in {  /* Used to define what the socket should look like/for */
    short   sin_family;
    u_short sin_port;
    struct  in_addr sin_addr;
    char    sin_zero [8];
};

/*****************************************************************************/

static long inited = 0;                      /* C version doesn't need to INITAPI, */
                                             /* so this flags if it's beed done. */

static char ident [17] = "TCPIP           "; /* These three are just */
static char subtk [9]  = "0000001A";         /* dummy vars for EZA compatability */
static long maxsock = 51;                    /* I never understood why IBM did this? */

/*****************************************************************************/

long WSAGetLastError () {
    return (errno);
};

/*****************************************************************************/

unsigned long inet_addr (const char * s) {
    unsigned long i;
    const char  * c;

    i = (atol (s) << 24);
    c = strstr (s, ".");
    if (c) {
        i |= (atol (++c) << 16);
        c = strstr (c, ".");
        if (c) {
            i |= (atol (++c) << 8);
            c = strstr (c, ".");
            if (c)
                i |= atol (++c);
        };
    };

    if (i == 0) return (0xFFFFFFFF);

    return (i);
};

int inet_aton (const char * s, long * a) {
    long         i;
    const char * c;

    i = (atol (s) << 24);
    c = strstr (s, ".");
    if (c == NULL) c = strstr (s, ",");
    if (c) {
        i |= (atol (++c) << 16);
        c = strstr (c, ".");
        if (c == NULL) c = strstr (c, ",");
        if (c) {
            i |= (atol (++c) << 8);
            c = strstr (c, ".");
            if (c == NULL) c = strstr (c, ",");
            if (c)
                i |= atol (++c);
        };
    };

    *a = i;

    return (4);
};

/*****************************************************************************/

struct hostent * gethostbyname (char * name) {
    long retcd;
    long namelen;
    struct hostent * hp;

    namelen = strlen (name);

    EZASOKET ("GETHOSTBYNAME", &namelen, name, &hp, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    return (hp);
};

/*****************************************************************************/

long socket (long af, long type, long protocol) {
    long retcd;
    long maxsno;

    if (inited == 0) { /* Need a socket before anything else... */
        inited = 1;    /* so it's the best place to init. */
                       /* The Host PC has it's own trigger too! */

        EZASOKET ("INITAPI ", &maxsock, &ident, &subtk, &maxsno, &errno, &retcd,
                  NULL, NULL, NULL, NULL);
    };

    EZASOKET ("SOCKET  ", &af, &type, &protocol, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long bind (long ss, struct sockaddr_in * name, long length) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("BIND    ", &s, name, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long connect (long ss, struct sockaddr_in * name, long length) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("CONNECT ", &s, name, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long listen (long ss, long backlog) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("LISTEN  ", &s, &backlog, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long accept (long ss, struct sockaddr_in * name, long * length) {
    long retcd;
    struct sockaddr_in temp;
    short s = (short)ss;

    if (name == NULL) {
        name = &temp;
    };

    EZASOKET ("ACCEPT  ", &s, name, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    if (length != NULL) length [0] = sizeof (struct sockaddr_in);

    return (retcd);
};

/*****************************************************************************/

long send (long ss, char * buf, long len, long flags) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("SEND    ", &s, &flags, &len, buf, &errno, &retcd,
              NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long recv (long ss, char * buf, long len, long flags) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("RECV    ", &s, &flags, &len, buf, &errno, &retcd,
              NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long closesocket (long ss) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("CLOSE   ", &s, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

char * ascii2ebcdic (char * in, long len) {
    EZASOKET ("ASCII2EBCDIC", &len, in,
              NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    return (in);
};

/*****************************************************************************/

char * ebcdic2ascii (char * in, long len) {
    EZASOKET ("EBCDIC2ASCII", &len, in,
              NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    return (in);
};

/*****************************************************************************/

long ioctlsocket (long ss, long cmd, long * argp) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("IOCTL   ", &s, &cmd, argp, argp, &errno, &retcd,
              NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/

long hgetsockname (long ss, long * name, int * size) {
    long retcd;
    short s = (short)ss;

    EZASOKET ("GETSOCKNAME", &s, name, &errno, &retcd,
              NULL, NULL, NULL, NULL, NULL, NULL);

    return (retcd);
};

/*****************************************************************************/
/*****************************************************************************/

long select (long maxsock, long * r, long * w, long * e, long * t) {
    long retcd;

    EZASOKET ("SELECT  ", &maxsock, t, r, w, e, r, w, e, &errno, &retcd);

    return (retcd);
};

/*****************************************************************************/

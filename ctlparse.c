/*
 *  CTLPARSE.C - Parse a configuration control file.
 *  Written 1993,94,95,96 by Andrew Clarke and released to the public domain.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "ctlparse.h"

#define BUFFER_LEN  512

void ctl_parse(void (*ctl_proc) (char *arg, char *val, int status, int line), char *pArgs[], FILE * fp, int case_flag)
{
    int bArgFound, iArg, iLine = 1;
    char *p, *pEol, *pEnd, *pVal, szBuf[BUFFER_LEN];

    /* first open the config file */
    if (!fp) {
        ctl_proc(NULL, NULL, CTL_NOFILE, 0);
        return;
    }

    /* file is open so prepare for the scan */
    while (1) {
        *szBuf = '\0';

        /* read a line of the file into the buffer */
        fgets(szBuf, BUFFER_LEN, fp);
        if (!*szBuf) {   /* read was unsuccessful */
            if (feof(fp)) {     /* end-of-file marker encountered */
                ctl_proc(NULL, NULL, CTL_ENDOFFILE, iLine);
                return;
            } else {            /* general failure of some sort */
                ctl_proc(NULL, NULL, CTL_READFAILURE, iLine);
                return;
            }
        }

        p = szBuf;

        if (!strchr(p, '\n')) {
            ctl_proc(NULL, NULL, CTL_LINETOOLONG, iLine);
            return;
        }

        /* scan for comment character */
        pEol = strchr(p, ';');
        if (pEol)
            *pEol = '\0';          /* kill comment and point to end of line */
        else
            pEol = p + strlen(p);  /* point to end of line */

        /* skip leading whitespace */
        while (isspace(*p))
            ++p;

        /* remove trailing whitespace  */
        pEnd = p + strlen(p) - 1;
        while (isspace(*pEnd))
            *(pEnd--) = '\0';

        /* if it's a blank line then don't bother processing it */
        if (*p) {
            pVal = p;

            /* split the argument and parameter */
            while (*pVal && !isspace(*pVal))
                ++pVal;
            if (*pVal)
                *(pVal++) = '\0';

            /* skip leading whitespace in parameter */
            while (*pVal && isspace(*pVal))
                ++pVal;

            for (iArg = 0; *(pArgs + iArg); iArg++) {

                switch (case_flag) {
                case CTL_NOIGNORECASE:
                    bArgFound = (!strcmp(*(pArgs + iArg), p));
                    break;
                case CTL_IGNORECASE:
                    bArgFound = (!stricmp(*(pArgs + iArg), p));
                    break;
                default:
                    /* value of case_flag is invalid */
                    ctl_proc(NULL, NULL, CTL_INVCASEFLAG, iLine);
                    return;
                } /* end switch() */

                if (bArgFound) {
                    if (!(*pVal))  /* argument missing parameter */
                        ctl_proc(*(pArgs + iArg), NULL, CTL_MISSINGPARAM, iLine);
                    else           /* valid argument with parameter */
                        ctl_proc(*(pArgs + iArg), pVal, iArg + 1, iLine);
                    break;         /* break for() */
                } /* end if() */

            } /* end for() */

            if (!bArgFound)     /* illegal argument */
                ctl_proc(p, pVal, CTL_ILLEGALARG, iLine);

        } /* end if() */
        /* comment, white space, blank line, or line too long encountered */
        iLine++;
    } /* end while (1) */
}


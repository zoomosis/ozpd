/*
 *  FILTER.C - Filter to attempt to convert text with hard-returns to text in
 *  paragraph form.
 *
 *  For more information send FidoNet netmail to Andrew Clarke at 3:635/727.4
 *  (or in the Australian PUBLIC_DOMAIN conference) or Internet e-mail at
 *  randy@empire.bbs.net.au.
 *
 *  Written 1996 by Andrew Clarke in about 5 minutes and released to the
 *  public domain.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char quote[11], tmp[300];
    int addtext, retforce;

    addtext = retforce = 0;
    while (1) {
        if (fgets(tmp, 300, stdin) == NULL)
            break;
        if (*(tmp + strlen(tmp) - 1) == '\n')
            *(tmp + strlen(tmp) - 1) = '\0';
        if (!retforce) {
            if (strcmp(tmp, "~~") == 0) {
                retforce = 1;
                printf("\n");
            } else {
                if (addtext)
                    putchar(' ');
                strncpy(quote, tmp, 11);
                quote[10] = '\0';
                if (*tmp && !strchr(quote, '>') && strlen(tmp) > 60) {
                    printf("%s", tmp);
                    addtext = 1;
                } else {
                    printf("%s\n", tmp);
                    addtext = 0;
                }
            }
        } else {
            if (strcmp(tmp, "~~") == 0)
                retforce = 0;
            if (addtext) {
                printf("\n");
                addtext = 0;
            }
            if (strcmp(tmp, "~~") != 0)
                printf("%s\n", tmp);
        }
    }
    return 0;
}

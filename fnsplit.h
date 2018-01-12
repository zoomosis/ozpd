/* public domain by Frank Adam */

#ifndef __DIR_H

int fnsplit(const char *path, char *drive, char *dir, char *name,
char *ext);
void fnmerge(char *path, const char *drive, const char *dir,
const char *name, const char *ext);

 #define WILDCARDS 1
 #define EXTENSION 2
 #define FILENAME  4
 #define DIRECTORY 8
 #define DRIVE     16

 #define MAXDRIVE 3
 #define MAXPATH 80
 #define MAXDIR  66
 #define MAXFILE  9
 #define MAXEXT   5

/* end header */

#endif

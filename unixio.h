/* written by Paul Edwards */
/* released to the public domain */

#include <stddef.h>

#define S_IFMT 0
#define S_IFDIR 1
#define S_IFREG 2

#define ENOENT 100
#define ENOTDIR 101

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

struct stat {
  long st_size;
  long st_mode;
  long st_ino;
  long st_dev;
};

int open(const char *fnm, int mode, ...);
int read(int fno, void *buf, size_t bytes);
int write(int fno, const void *buf, size_t bytes);
int close(int fno);

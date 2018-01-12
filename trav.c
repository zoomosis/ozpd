/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*  Amiga mods by Michael Stapleton                                  */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  trav - traverse a directory                                      */
/*                                                                   */
/*  This function is designed to be highly machine-specific.  It     */
/*  is to get all the machine-specific directory processing into a   */
/*  single function.                                                 */
/*                                                                   */
/*  The first parameter is a filename.  This can be a simple         */
/*  filename such as "temp.c", or one that contains wildcards such   */
/*  as "temp.*" or "temp.c(*)".                                      */
/*                                                                   */
/*  The second parameter is a pointer to a control block.  This      */
/*  control block's structure is machine-specific, and contains      */
/*  information that trav will use such as whether or not to         */
/*  search subdirectories as well.                                   */
/*                                                                   */
/*  The third parameter is a pointer to a function which will get    */
/*  called for every file found.                                     */
/*                                                                   */
/*  The fourth parameter is a pointer to a control block which will  */
/*  get passed to the function you specified as parameter 3.         */
/*                                                                   */
/*  The function that trav calls takes two parameters:               */
/*  The first is a filename that trav has found.                     */
/*  The second is the fourth parameter you passed to trav.           */
/*                                                                   */
/*  example of use:                                                  */
/*                                                                   */
/*  int myfunc(char *fnm, void *ucb)                                 */
/*  {                                                                */
/*    printf("fnm is %s\n",fnm);                                     */
/*    return (1);                                                    */
/*  }                                                                */
/*                                                                   */
/*  main()                                                           */
/*  {                                                                */
/*    trav("dd:input(*)",NULL,myfunc,NULL);                          */
/*    return (0);                                                    */
/*  }                                                                */
/*                                                                   */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

#ifdef MVS
  #include "brkpds.h"
  #include "mkpdsn.h"
  #include "patmat.h"
  #include "stritr.h"
  #include "striupr.h"
#endif
#ifdef MSDOS
  #include <dos.h>
#ifdef __TURBOC__
  #include <dir.h>
#else
  #include <direct.h>
#endif
#endif
#ifdef UNIX
  #include <ftw.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include "patmat.h"
  static int (*gfunc)(const char *fnm, void *ucb);
  static char *gfile;
  static void *gcb;
  static int   grec;
  static char *gpat;
  static char gstem[FILENAME_MAX+200];
#endif
#ifdef VAXVMS
#include <descrip.h>
#include "patmat.h"
#endif
#ifdef AMIGA
  #include <dos.h>
  #include <proto/dos.h>
  #include "patmat.h"
#endif
#ifdef OS2
#include <os2.h>
#endif
#ifdef VM
#endif

/* suppress warning about unused parameter */

#ifndef unused
#define unused(x) ((void)(x))
#endif

int trav(char *filename, void *tcb,
    int (*ufunc)(const char *fnm, void *ucb), void *cb)
{
#ifdef MVS
  FILE *pds;
  char *first = NULL;
  char *sec = NULL;
  char buf[256];
  char mem[9];
  char ttr[3];
  int mcnt, scnt, x;

  /* tcb of 1 means that the user wants the TTR */
  if ((tcb != NULL) && (*(int *)tcb == 1)) cb = ttr;
  if (!brkpds(filename,&first,&sec))
      return(ufunc(filename,cb));
#ifdef C370
  pds = fopen(first,"rb,recfm=u");
#else
  pds = fopen(first,"rb");
#endif
  if (pds == NULL) return (0);
  striupr(sec);
  mem[8] = '\0';
  while (fread(buf,256,1,pds) == 1)
  {
    mcnt = *(short *)buf;
    for (x=2;x<mcnt;)
    {
      memcpy(mem,buf+x,8);
      if (memcmp(mem,"\xff\xff\xff\xff\xff\xff\xff\xff",8) == 0)
      {
        fclose(pds);
        return (1);
      }
      memcpy(ttr,buf+x+8,3);
      scnt = *(buf+x+11);
      scnt = scnt & 0x1f;
      x += 12 + scnt * 2;
      stritr(mem);
      if (!patmat(mem,sec)) continue;
      if (!ufunc(mkpdsn(first,mem),cb))
      {
        fclose(pds);
        return (0);
      }
    }
  }
  fclose(pds);
  return (1);
#elif defined(VM)
  FILE *fp;
  char buf[200];
  int allok = 1;

  sprintf(buf, "listfile %s (exec", filename);
  system(buf);
  fp = fopen("cms exec","r");
  if (fp == NULL) return (0);
  while ((fgets(buf,sizeof buf,fp) != NULL) && allok)
  {
    buf[27] = '\0';
    if (!ufunc(buf+7,cb)) allok = 0;
  }
  fclose(fp);
  remove("cms exec");
  return (allok);
#elif defined(MSDOS)
#ifdef __TURBOC__
  struct ffblk ffblk;
#else
  struct find_t ft;
#endif
  int x,done;
  int lower=0;

  unused(tcb);
  for (x=0;*(filename+x)!='\0';x++)
      if (islower(*(filename+x))) lower=1;
#ifdef __TURBOC__
  done = findfirst(filename,&ffblk,0);
  while (!done)
  {
    if (lower) for (x=0;*(ffblk.ff_name+x)!='\0';x++)
        *(ffblk.ff_name+x) = (char)tolower(*(ffblk.ff_name+x));
    if ((strchr(filename, '.') != NULL)
        && (strchr(ffblk.ff_name, '.') == NULL))
    {
        strcat(ffblk.ff_name, ".");
    }
    if (!ufunc(ffblk.ff_name,cb)) return (0);
    done = findnext(&ffblk);
  }
#else
  done = _dos_findfirst(filename, -1, &ft);
  while (!done)
  {
    if (lower) for (x=0;*(ft.name+x)!='\0';x++)
        *(ft.name+x) = (char)tolower(*(ft.name+x));
    if (!ufunc(ft.name,cb)) return (0);
    done = _dos_findnext(&ft);
  }
#endif
  return (1);
#elif defined(OS216)
  HDIR hdir;
  struct _FILEFINDBUF ffb;
  USHORT cnt;

  hdir = -1;
  cnt = 1;
  done = DosFindFirst(filename, &hdir, 0, &ffb, sizeof ffb, NULL, &cnt, 0);
  while (!done)
  {
    if (lower) for (x=0;*(ffb.achName+x)!='\0';x++)
        *(ffb.achName+x) = (char)tolower(*(ffb.achName+x));
    if (!ufunc(ffb.achName,cb)) return (0);
    cnt = 1;
    done = DosFindNext(&hdir, &ffb, sizeof ffb, &cnt);
  }
  return (1);
#elif defined(OS2)
  FILEFINDBUF3 fbuf;
  int x;
  int lower=0;
  unsigned int done;
  unsigned long count=1;
  unsigned long handle=0xFFFFFFFFUL;
  unsigned long lev = 1;
  char newfile[FILENAME_MAX];
  char prefix[FILENAME_MAX];
  char *temp;
  int lenpref;

  unused(tcb);
  temp = strrchr(filename,'\\');
  if (temp == NULL) temp = strrchr(filename, '/');
  if (temp == NULL) temp = strrchr(filename,':');
  if (temp == NULL) strcpy(prefix,"");
  else
  {
    memcpy(prefix,filename,temp-filename+1);
    *(prefix+(temp-filename+1)) = '\0';
  }
  lenpref = strlen(prefix);
  for (x=0;*(filename+x)!='\0';x++)
      if (islower(*(filename+x))) lower=1;
  if (lower) for (x=0;*(prefix+x)!='\0';x++)
      *(prefix+x)=(char)tolower(*(prefix+x));
  done = DosFindFirst((PSZ)filename,&handle,0x0000UL,
      &fbuf,sizeof fbuf,&count,lev);
  while (done==0)
  {
    strcpy(newfile,prefix);
    strcat(newfile,fbuf.achName);
    if (lower) for (x=0;*(newfile+lenpref+x)!='\0';x++)
        *(newfile+lenpref+x) = (char)tolower(*(newfile+lenpref+x));
    if (!ufunc(newfile,cb)) return (0);
    done = DosFindNext(handle, &fbuf, sizeof fbuf, &count);
  }
  if (done == 18) DosFindClose(handle);
  return (1);
#elif defined(UNIX)
  int utrav(const char *fn, const struct stat *mystat, int myint);
  char *pos;

  gcb = cb;
  gfunc = ufunc;
  gfile = filename;
  if ((tcb != NULL) && (*(int *)tcb == 1)) grec = 1;
  else grec = 0;
  strncpy(gstem, filename, sizeof gstem);
  gstem[sizeof gstem - 1] = '\0';
  pos = strrchr(gstem, '/');
  if (pos != NULL)
  {
    *pos = '\0';
    gpat = pos+1;
  }
  else
  {
    strcpy(gstem, ".");
    gpat = filename;
  }
  ftw(gstem, utrav, 1);
  return (1);
#elif defined(VAXVMS)
  unsigned long context=0;
  char found[80];
  char wildcard[80];
  int x,y,lower=0;
  $DESCRIPTOR(dfilename,"*.*");
  $DESCRIPTOR(dfound,found);

  for (x=0;*(filename+x)!='\0';x++)
      if (islower(*(filename+x))) lower=1;
  while ((LIB$FIND_FILE(&dfilename,&dfound,&context,0,0,0,0)
      & 1) == 1)
  {
    x = strlen(found);
    while (*(found+x) != ';') x--;
    *(found+x) = '\0';
    while (*(found+x) != ']') x--;
    if (lower) for (y=0;*(found+x+1+y)!='\0';y++)
        *(found+x+1+y) = (char)tolower(*(found+x+1+y));
   if (!ufunc(found+x+1,cb)) return (0);
  }
  LIB$FIND_FILE_END(&context);
  return (1);
#elif defined(AMIGA)
    struct FileInfoBlock __aligned fib;
    char newfile[FILENAME_MAX];
    char prefix[FILENAME_MAX];
    char *temp;
    BPTR fLock;
    LONG ioerr;

    strlwr(filename);   /* To make comparison case-insensitive */
    temp = strrchr(filename, '/');
    if (temp == NULL)
        temp = strrchr(filename, '\\');
    if (temp == NULL)
        temp = strrchr(filename, ':');

    if (temp == NULL)
        strcpy(prefix, "");
    else
    {
        memcpy(prefix, filename, temp - filename + 1);
        *(prefix + (temp - filename + 1)) = '\0';
    }

    if (fLock = Lock(prefix, SHARED_LOCK))
    {
        if (Examine(fLock, &fib))
        {
            if (fib.fib_DirEntryType > 0)
            {
                while (ExNext(fLock, &fib))
                {
                    if (fib.fib_DirEntryType < 0)
                    {
                        strcpy(newfile, prefix);
                        strcat(newfile, fib.fib_FileName);
                        strlwr(newfile);    /* To make comparison case-insensitive */
                        if (patmat(newfile, filename))
                        {
                            /* printf("%s\n", newfile); */
                            if (!ufunc(newfile, cb)) return (0);
                        }
                    }
                }
                if ((ioerr = IoErr()) != ERROR_NO_MORE_ENTRIES)
                    printf("DOS error %d\n", ioerr);
            }
        }
        UnLock(fLock);
    }
    return (1);
#else
  ufunc(filename,cb);
  return (1);
#endif
}

#ifdef UNIX
int utrav(const char *fn, const struct stat *mystat, int myint)
{
  const char *pos;
  const char *actfn;

  pos = strrchr(fn, '/');
  if (pos == NULL) pos = fn;
  else pos++;
  if (myint == FTW_F)
  {
    if (patmat(pos,gpat))
    {
        if (grec || (strchr(fn + strlen(gstem) + 1, '/') == NULL))
        {
            actfn = fn;
            if (strncmp(fn, "./", 2) == 0) actfn += 2;
            return (!gfunc(actfn,gcb));
        }
    }
  }
  return (0);
}
#endif

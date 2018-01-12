/* 
Public domain by Frank Adam 1996.
Can be used as a direct replacement of Borland's fnsplit() and fnmerge()
Especially fnmerge is better as you can chuck just about anything at it
and still comes up with some sort of valid path..I hope :) 
*/

/* #define TEST /* uncomment to test */

#ifdef TEST
 #include <stdio.h>
#endif

#include "fnsplit.h"
                      
int fnsplit(const char *path, char *drive, char *dir, char *name,char *ext)
{
#ifndef _STRING_H
 #include <string.h>
#endif

 const char *p1 = path,*p2=NULL,*p3=NULL,*p4=NULL,*p5=NULL,*pt = path;
 int flags = 0;
 *drive = '\0';*dir = '\0';*name = '\0';*ext = '\0';
 while(*pt && *pt == ' ') pt++;
 p1 = pt; /* strip leading spaces */

 if(!pt) return 0;

 while(*pt && !flags)
 {
  if(*pt == '*' || *pt == '?') flags |= WILDCARDS;
  pt++;
  }

 pt = path;
 p2 = strrchr(p1,'.');
 p3 = strchr(p1,'\\');
 p4 = strrchr(p1,'\\');
 p5 = strchr(p1,':');

 if(p2) {strcat(ext,p2);flags |= EXTENSION;}
 if(p3)
 {
  flags |= DIRECTORY;if(p4 && p4 == p3) p4 = NULL;
  }
 if(p5 == (pt + 1)) flags |= DRIVE;

 if(flags & DRIVE) strncat(drive,pt,2);
 if(flags & DIRECTORY)
 {
  if(p4) strncat(dir,p3,(size_t)((p4 - p3)+1));
   else strncat(dir,p3,1);
  }

 if(p4)
 {
  if(!*(p4+1) || *(p4+1) == '.') return flags;

  flags |= FILENAME;
  if(p2)
   strncat(name,p4+1,(size_t)(p2-p4)-1);
  else if(*(p4 + 1))
  {
   strncat(name,p4+1,MAXFILE);
   name[MAXFILE-1] = '\0';
   }

  }

 else if(p3)
 {
  if(!*(p3+1) || *(p3+1) == '.') return flags;
  flags |= FILENAME;
  if(p2)
   strncat(name,p3+1,(size_t)(p2-p3)-1);
  else
  {
   strncat(name,p3+1,MAXFILE);
   name[MAXFILE-1] = '\0';
   }
  }

 else if(p2 && p2 != pt)
 {
  flags |= FILENAME;
  strncat(name,pt,(size_t)(p2-pt));
  }

 else if(!flags && !(flags & WILDCARDS))
 {
  flags |= FILENAME;
  if(*pt)
  {
   strncat(name,pt,MAXFILE);
   name[MAXFILE-1] = '\0';
   }

  }

return flags;
}


void fnmerge(char *path, const char *drive, const char *dir,
const char *name, const char *ext)  /* no need to trim spaces off psegs */
{
 const char *p;char *p1 = path;
 int count,havedir = 0;

if(drive)
{
 p = drive;
 while(*p && *p == ' ')p++;
 if(*p)
 {
  *p1++ = *drive;
  *p1++ = ':';
  }
 }

if(dir)
{
  p = dir;
  count = 0;
 while(*p && *p == ' ')p++;   /* trim leading spaces if any */
 if(*p)
 {
  havedir = 1;
  if(p1 != path && *p != '\\') *p1++ = '\\';
  while(*p && *p != ' ' && count < (MAXDIR - 1))
  {
   *p1++ = *p++;
   count++;
   }
  if(*(p1 - 1) != '\\') *p1++ = '\\';
  }
}

if(name)
{
 p = name;
 if(*p && !havedir && *drive) *p1++ = '\\';
 count = 0;
 while(*p && *p == ' ')p++;
 if(*p) while(*p && *p != ' ' && count < (MAXFILE - 1))
 {
  *p1++ = *p++;
  count++;
  }

 }

if(ext)
{
 p = ext;
 count = 0;
 while(*p && *p == ' ') p++;
 if(*p != '.')
 {
  *p1++ = '.';count++;
  }
 if(*p && p) while(*p && *p != ' ' && count < (MAXEXT - 1))
 {
  *p1++ = *p++;
  count++;
  }
}

*p1 = '\0';
}

#ifdef TEST

main()
{
 char drive[MAXDRIVE];
 char dir[MAXDIR];
 char file[MAXFILE];
 char ext[MAXEXT];
 char path[MAXPATH];

 fnsplit("c:\\borlandc\\bin\\bcc.exe",drive,dir,file,ext);
 fnmerge(path,"c","mygreatdir","mygreatfile","myextension");

 printf("\n%s\n%s\n%s\n%s\n%s",drive,dir,file,ext,path);
 return 0;
 }

#endif
  

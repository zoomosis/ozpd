/*
** Fatread.c  Ver1.1 ..by Frank Adam.
** Released to the Public Domain on the 18th October 1996.
** It now works on older compilers as well, but still requires
** DOS 3.31 or higher.
** Should now compile in small too, but as of now, i have not tested that.
** What it does ?
** Reads the fat and writes it formatted to screen/file/printer.
** Or just reads and dumps it into a file, for later restore.
** Usage: fatread S|P|R drvletter. The switches can be reversed.
** S = Save, P = Print. Save dumps the file to boot%c.dat and fat%c.dat
** %c = drive of course and either creates or appends or overwrites 
** fatbak.dat, which is the control file. 
** 'R' (restore) is not yet implemented.           
** If your parameter is P it'll give you 3 options to print to.
** if you select file then you will be prompted for a filename.
** There is no error checking for the printer, so it'd better be on.:-)
** Neither is there error checking for the drive, but the Dpb routine
** at the start should have a hernia anyway and the program will abort.
** Anyway, why am i explaining this to programmers ?? :-)
** While it has been tested thoroughly on my PC and 3 drives i can not
** guarantee that the code will work on all systems, thus i will not take
** any responsibility for loss of data or damage caused by the use and
** or misuse of this code.
** There, you have been warned, but it's quite safe. Trust me ! :-)
*/

#if !defined (__LARGE__)
 #error Compile me in large model for now please.
#endif

#if defined (__BORLANDC__)  || defined (__TURBOC__)
 #define FAR far
#endif

#ifndef __TURBOC__
 #if (defined(_MSC_VER) && (_MSC_VER >= 700)) || (defined(__SC__))
  /* Make FP_xxx macros lvalues as in older versions */
  #undef FP_SEG
  #undef FP_OFF
  #define FP_SEG(fp)    ((unsigned)((unsigned long)(fp) >> 16))
  #define FP_OFF(fp)    ((unsigned)(fp && 0xffff))
 #endif
#ifndef MK_FP
 #define MK_FP(seg,off) ((void FAR *)(((long)(seg) << 16)|(unsigned)(off)))
#endif
#endif

#include <dos.h>
#include <string.h>  /* for memcpy */
#include <ctype.h>   /* for toupper */
#include <stdio.h>
#include <stdlib.h>

#define BYTE char
#define WORD unsigned int
#define DWORD unsigned long
#define UCHAR unsigned char

#define FATSHOW 0
#define FATSAVE 1
#define RESTORE 2


struct DriveParamBlock  /* from Ralph Brown's excellent Interrupt List */
{
 BYTE Driveno;  /*  drive number (00h = A:, 01h = B:, etc) */
 BYTE DevUnit;  /*  unit number within device driver */
 WORD Bpersec;  /*  bytes per sector */
 BYTE Hysect ;  /*  highest sector number within a cluster */
 BYTE Shft   ;  /*  shift count to convert clusters into sectors */
 WORD ResSecs;  /*  number of reserved sectors at beginning of drive */
 BYTE NumFats;  /*  number of FATs */
 WORD NumRoot;  /*  number of root directory entries */
 WORD UserFsc;  /*  number of first sector containing user data */
 WORD MaxClust; /*  highest cluster number (number of data clusters + 1) */
  /*  16-bit FAT if greater than 0FF6h, else 12-bit FAT */
 WORD SecPerFat;/*  number of sectors per FAT */
 WORD DirStart; /*  sector number of first directory sector */
 DWORD DevHead; /* address of device driver header */
 BYTE  MediaId; /*  media ID byte (see #0669) */
 BYTE  AccessB; /*  00h if disk accessed, FFh if not */
 DWORD NextDbp; /*  pointer to next DPB */
 WORD  NextClus; /* cluster at which to start search for free space */
          /*  when writing, usually the last cluster allocated */
 WORD  FreeClus; /* number of free clusters on drive, FFFFh = unknown */
};

struct DiskReadPacket   /* for DOS INT 25 absread */
{
 DWORD   sector;
 WORD    numsectors;
 DWORD   transferto;
 }drp;

struct BackupData  /* this holds the "where to put it back" info */
{
 BYTE drive;
 WORD fss; /* fat start sector. Always 1 but who knows.. */
 WORD bps; /* bytes per sector 512 on my drives but may vary */
 WORD spf; /* sectors per fat 251-2 on my drives again may vary */
 BYTE fats;  /* number of fats  (normally 2, a 0 here is not really good.
       /* the two fats are contiguous and the same, so one is enough
       /* to save, Norton's image.exe does the same.
       /* perhaps i should compare before saving, may do in future
       */
 }bupdat;     /* for test purposes at this stage */

struct DriveParamBlock FAR dpb;
unsigned linecount = 0;
FILE *stream;int PrintFlag;
char fname[80] = "";

void ShowMedia(void);
int GetDpb(int);
int GetFat(int,int,int);
int GetBoot(int,int);
void ShowSector(int,UCHAR*);
int ParseArg(char*[], int*,int*);
void SeekBackupPos(FILE *,int);
void FatShow( int );
void FatSave( int );

int GetKey(void);
int AbsRead(int,DWORD,WORD,UCHAR*);
int AbsWrite(int,DWORD,WORD,UCHAR*);


int main (int argc, char* argv[])
{
 int drive,swich,key;

 if(argc <= 2) { printf("Missing argument(s) !");return 0; }
 if(!ParseArg(argv, &drive,&swich)) return 0;

 if(GetDpb(drive)) printf("DPB Operation OK..");
  else {
 printf("DPB Operation Failed ! Probably CD-ROM or invalid drive.");
 return 0;
 }

  ShowMedia(); /* looking good if we got here */
  printf("\n\nAre the above correct ? Proceed (Y/n)");
  while(!(key = GetKey()));
  if(toupper(key) == 'N') return 0;
  key = 0;

switch(swich)
{
 case FATSHOW : FatShow(drive); break;
 case FATSAVE : FatSave(drive); break;
 case RESTORE : printf("\nNot yet implemented, waiting on feedback..");
 }

 return 0;
 }

void FatShow( int drive )
{
 int key;
 char printoname[64];
 char *p;

 printf("\nPrint to what ? ('p'rinter, 'f'ile, 'S'creen)");
  while(!(key = GetKey()));
  switch(toupper(key))
  {
   case 'P' : stream = stdprn;PrintFlag = 2;break;
   case 'F' : printf("\nEnter Filename:");
       fgets(printoname,63,stdin);
       p = printoname;
       if(*p == '\n') {printf("Wise guy !"); exit(0);}
       while(*p != '\n') p++;
       *p = '\0';
       if(NULL == (stream = fopen(printoname,"wt")))
       {
        printf("Bad filename !"); exit(0);
        }
       else printf("Excellent filename ! :-)");
       PrintFlag = 1;
       break;
   default  : stream = stdout;PrintFlag = 0;
   }

  if(!GetBoot(drive -1,FATSHOW))
    printf("Boot read failed !");
  if(!GetFat(drive - 1,1,FATSHOW))
    printf("Fat read failed !");

}


void FatSave( int drive )
{
 int newdat = 0;
 FILE *datfile;

 if(!GetBoot(drive -1,FATSAVE))
  printf("Boot read failed !");
 if(!GetFat(drive - 1,1,FATSAVE))
  printf("Fat read failed !");

   bupdat.drive = dpb.Driveno;
   bupdat.fss = 1;
   bupdat.bps = dpb.Bpersec;
   bupdat.spf = dpb.SecPerFat;
   bupdat.fats = dpb.NumFats;
   strcpy(fname,"fatbak.dat");
   if((datfile = fopen(fname,"rb+")) == NULL)
   {
    newdat = 1;
    if((datfile = fopen(fname,"wb")) == NULL)
     {
      printf("\n *** Could not save backup data !!! *** ");
      exit(1);
      }
    }

   if(!newdat) SeekBackupPos(datfile,dpb.Driveno);

   {
    fwrite(&bupdat,sizeof(struct BackupData),1,datfile);
    }
   printf("\nBackup completed. Control file's name FATBAK.DAT");
}


void SeekBackupPos(FILE *fh,int drive)
{
 struct BackupData temp;
 if(fseek(fh,0L,SEEK_SET)) return;

 while(!feof(fh))
 {
  fread(&temp,sizeof(struct BackupData),1,fh);
  if(temp.drive == drive)
  {
   fseek(fh,-(long)sizeof(struct BackupData),SEEK_CUR);
   return;
   }
  }
 fseek(fh,0L,SEEK_END);
}


int ParseArg(char* args[], int *drv,int *sw)
{
 char d,s;

  if(*args[1] == '/')
  {
   d = *args[2];
   s = args[1][1];
   }

   else if(*args[2] == '/')
   {
    d = *args[1];
    s = args[2][1];
    }
     else
    {
     printf("Incorrect argument(s) !");
     return 0;
     }

*drv = toupper(d) - 'A' + 1;

switch(toupper(s))
{
 case 'S' : *sw = FATSAVE;PrintFlag = 0; return 1;
 case 'R' : *sw = RESTORE;PrintFlag = 0; return 1;
 case 'P' : *sw = FATSHOW; return 1;
 }

return 0;
}

int GetDpb(int drive)
{
 union REGS inregs,outregs;struct SREGS sregs;
 char FAR* p;
 inregs.h.ah = 0x32;
 inregs.h.dl = drive; /* 0 default drv,1 = A, 2 = B, etc.. */
 intdosx(&inregs,&outregs,&sregs);
 if(outregs.x.cflag) return 0;
 if(outregs.h.al) return 0;
 p = (char FAR*) MK_FP((unsigned)sregs.ds,(unsigned)outregs.x.bx);
 printf("\nDPB Address = %p\n",p);
 memcpy(&dpb,p,sizeof(struct DriveParamBlock));
 return 1;
}

int GetBoot(int Gbdrive,int mode)
{
 UCHAR *boot;int j,p = 0;
 FILE *out;

 if(Gbdrive != dpb.Driveno)
 {
  printf ("Drive mismatch !");
  return 0;
  }

 boot = (UCHAR*)calloc(dpb.Bpersec,sizeof(UCHAR));
  if(NULL == boot) {printf("\nNot enough memory");return 0;}

 if(mode == FATSAVE)
 {
  sprintf(fname,"boot%c.dat\0",(dpb.Driveno + 'A'));
   out = fopen(fname,"wb");
   if(out == NULL) {perror("\nError opening file");return 0;}
  printf("\n\n");
  }
 else fprintf(stream,"\n Boot Sector..");

  if (AbsRead(dpb.Driveno,0,1 , boot) != 1)
  {perror("\nDisk read error"); return 0;}

 if(mode == FATSAVE || PrintFlag == 1)
  {
   printf("\rSector 0 Read OK....");

  if(!PrintFlag)
   for(j = 0; j < dpb.Bpersec;j++) p += fwrite(&boot[j],1,1,out);

   else if(PrintFlag == 1) p = dpb.Bpersec;
      printf("Written %03d bytes OK",p);
     p = 0;
    }

  if(mode == FATSHOW)
  {
   ShowSector(0,boot);
   }
 free(boot);

 if(mode == FATSAVE )
 {
  fclose(out);
  printf("\nBoot saved OK to %s.",fname);
  }

 else
  fprintf(stream,"\nEnd of boot sector.\n");

  if(!PrintFlag && mode != FATSAVE)
  {
   printf("\nPress any key to go on..");
   while(!GetKey());
   printf("\rFat starts here..       ");
   }
 return 1;
}


int GetFat(int drive,int fatnum,int mode)
{
 UCHAR *fat;
 int startsector,endsector,i, j , p = 0;
 FILE *out;

 if(drive != dpb.Driveno)
 {
  printf ("Drive mismatch !");
  return 0;
  }

 if(fatnum == 2 && dpb.NumFats == 1)
 {
  printf("\nOnly one FAT on this drive .");
  return 0;
  }

 startsector = ((fatnum -1) * dpb.SecPerFat) + 1;
 endsector = (dpb.SecPerFat * fatnum) + 1;
 fat = (UCHAR*)calloc(dpb.Bpersec,sizeof(UCHAR));
  if(NULL == fat) {printf("\nNot enough memory");return 0;}

 if(mode == FATSAVE)
 {
  sprintf(fname,"fat%c.dat\0",dpb.Driveno + 'A');
   out = fopen(fname,"wb");
   if(out == NULL) {perror("\nError opening file");return 0;}
  }
 printf("\n");

 for(i = startsector ;i < endsector;i++)
 {
  if (AbsRead(dpb.Driveno,i,1 , fat) != 1)
  {perror("\nDisk read error"); return 0;}

 if(mode == FATSAVE || PrintFlag == 1)
  {
   printf("\rSector %02d Read OK....",i);
   if(!PrintFlag)
    for(j = 0; j < dpb.Bpersec;j++) p += fwrite(&fat[j],1,1,out);

    printf("Written %03d sectors OK",i,p);
    p = 0;
    }

  if(mode == FATSHOW) ShowSector(i,fat);
 }
 free(fat);
 if(mode == FATSAVE )
 {
  fclose(out);
  printf("\nFat saved OK to %s.",fname);
  }
 else
  printf("\nEnd. Press any key to exit...");

 while(!GetKey());
 return 1; /* phew */
 }

void ShowMedia( void ) /* just a bit of show off :o) */
{
 printf("\nDrive %c media type = ",toupper(dpb.Driveno + 'A'));
 switch((int)dpb.MediaId)
 {
  case 0xfff0 : if(dpb.SecPerFat > 4 && dpb.SecPerFat < 10)
     printf("Probably 1.44M 3.5 floppy");
       else printf("Unrecognized device");
     break;
  case 0xfff8 : printf("Hard drive");break;
  case 0xfff9 : printf("DS 1.2M 5.25 floppy");break;
  case 0xfffa : printf("HP ROM device");break;
  case 0xfffc : printf("SS 9S/T 180K floppy");break;
  case 0xfffd : printf("DS 9S/T 360K floppy");break;
  case 0xfffe : printf("SS 8S/T 160K floppy");break;
  case 0xffff : printf("DS 8S/T 320K floppy");break;
  default   : printf("Unrecognized device");break;
  }
 printf("\nSectors per fat  = %d",dpb.SecPerFat);
 printf("\nBytes per sector = %d",dpb.Bpersec);
 printf("\nNumber of fats   = %d",dpb.NumFats);
 printf("\nFat System       = ");
 printf(dpb.MaxClust > 0xffb ? "16 Bit" : "12 Bit");
if((unsigned) dpb.MediaId == 0xfff8)
 printf("\nAvailable Space  = %ld bytes",
    (long) (dpb.FreeClus << dpb.Shft) * (long)dpb.Bpersec);
 }

/* Note: i found, a 1.4M can return 0xfff9 if disk/x-copied from a 1.2M */

int AbsRead(int AbsRdrive,DWORD sector,WORD numsecs,UCHAR *buf)
{
 union REGS inregs,outregs;
 struct SREGS sregs;
 drp.sector = sector;
 drp.numsectors = numsecs;
 drp.transferto = (DWORD)buf;
 inregs.h.al = AbsRdrive;
 inregs.x.cx = 0xffff;
 inregs.x.bx = FP_OFF(&drp);
 sregs.ds = FP_SEG(&drp);
 int86x(0x25,&inregs,&outregs,&sregs);
 if(outregs.x.cflag) return 0;
 return 1;
 }

/*
** !!! to newbies and not so sures, DO NOT muck around with
** this function unless you're sure you know what it does !!!
*/
int AbsWrite(int AbsWdrive,DWORD sector,WORD numsecs,UCHAR *buf)
{
 union REGS inregs,outregs;
 struct SREGS sregs;
 drp.sector = sector;
 drp.numsectors = numsecs;
 drp.transferto = (DWORD)buf;
 inregs.h.al = AbsWdrive;
 inregs.x.cx = 0xffff;
 inregs.x.bx = FP_OFF(&drp);
 sregs.ds = FP_SEG(&drp);
 int86x(0x26,&inregs,&outregs,&sregs);
 if(outregs.x.cflag) return 0;
 return 1;
}
/*!!!!!!!!!!!!!!!!!!!!*/


void ShowSector(int sector,UCHAR *fatstr)
{
  int i,j;
  UCHAR *p = fatstr;
  if(PrintFlag == 2) printf("\rPrinting Sector %03d",sector);

  fprintf(stream,"\nSector: %d\tDrive: %c\n",sector,dpb.Driveno + 'A');
  for (i = 0; i < dpb.Bpersec; i += 16)
   {
     if (linecount == 22 && !PrintFlag)
     {
      printf("Press any key... or ESC to leave");
      if(GetKey() == 27) exit(0);
      fprintf(stream,"\rSector: %d continued.            \n", sector);
      linecount = 0;
      }

    fprintf(stream,"%05lX: ", (long)(i + ((long)sector * (long)dpb.Bpersec)));
    for (j = 0; j < 16; j++) fprintf(stream,"%02X ", p[i+j]);
      fprintf(stream,"\t");
     for (j = 0; j < 16; j++)
      if (isprint(p[i+j]))
       fprintf(stream,"%c", p[i+j]);
       else fprintf(stream,".");
      fprintf(stream,"\n");
     linecount++;
   }

}


int GetKey()
{
 union REGS regs;
 regs.h.ah = 0x10;
 int86(0x16,&regs,&regs);
 return regs.h.al;
}

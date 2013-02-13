#include <stdio.h>
#include <gccore.h>		/*** Wrapper to include common libogc headers ***/
#include <ogcsys.h>		/*** Needed for console support ***/
#include <ogc/dvd.h>	
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <ogc/machine/processor.h>

#include "dvd.h"

char *dvdDiscTypeStr = NotInitStr;
//int dvdDiscTypeInt = 0;

file_handle *dir = NULL;
uint64_t offset;
int size;
u8 *dol;

/* Simple DVD functions */
int is_unicode,files;
static int last_current_dir = -1;
u32 inquiryBuf[2048] __attribute__((aligned(32)));
int num_entries = 0;
file_entries *DVDToc = NULL; //Dynamically allocate this
volatile unsigned long* dvd = (volatile unsigned long*)0xCC006000;


int dvd_read_id()
{
   dvd[0] = 0x2E;
   dvd[1] = 0;
   dvd[2] = 0xA8000040;
   dvd[3] = 0;
   dvd[4] = 0x20;
   dvd[5] = 0x80000000;
   dvd[6] = 0x20;
   dvd[7] = 3;

   while (dvd[7] & 1);
      if (dvd[0] & 0x4) return 1;
   return 0;
}

unsigned int dvd_get_error(void)
{
   dvd[2] = 0xE0000000;
   dvd[8] = 0;
   dvd[7] = 1;

   while (dvd[7] & 1);
   return dvd[8];
}

void dvd_motor_off()
{
   dvd[0] = 0x2E;
   dvd[1] = 0;
   dvd[2] = 0xe3000000;
   dvd[3] = 0;
   dvd[4] = 0;
   dvd[5] = 0;
   dvd[6] = 0;
   dvd[7] = 1;

   while (dvd[7] & 1);
}


/*
DVD_LowRead64(void* dst, unsigned int len, uint64_t offset)
  Read Raw, needs to be on sector boundaries
  Has 8,796,093,020,160 byte limit (8 TeraBytes)
  Synchronous function.
    return -1 if offset is out of range
*/
int DVD_LowRead64(void* dst, unsigned int len, uint64_t offset)
{
   volatile unsigned long* dvd = (volatile unsigned long*)0xCC006000;
   if(offset>>2 > 0xFFFFFFFF) return -1;

   if ((((int)dst) & 0xC0000000) == 0x80000000)
      dvd[0] = 0x2E;
      dvd[1] = 0;
      dvd[2] = 0xA8000000;
      dvd[3] = offset >> 2;
      dvd[4] = len;
      dvd[5] = (unsigned long)dst;
      dvd[6] = len;
      dvd[7] = 3;
      DCInvalidateRange(dst, len);

   while (dvd[7] & 1);
      if (dvd[0] & 0x4) return 1;
   return 0;
}

static char error_str[256];

char *dvd_error_str()
{
   unsigned int err = dvd_get_error();
   if(!err) return "OK";
  
   memset(&error_str[0],0,256);
   switch(err>>24) {
      case 0:
         break;
      case 1:
         strcpy(&error_str[0],"Lid open");
         break;
      case 2:
         strcpy(&error_str[0],"No disk/Disk changed");
         break;
      case 3:
         strcpy(&error_str[0],"No disk");
         break;  
      case 4:
         strcpy(&error_str[0],"Motor off");
         break;  
      case 5:
         strcpy(&error_str[0],"Disk not initialized");
         break;
   }
   switch(err&0xFFFFFF) {
      case 0:
         break;
      case 0x020400:
         strcat(&error_str[0]," Motor Stopped");
         break;
      case 0x020401:
         strcat(&error_str[0]," Disk ID not read");
         break;
      case 0x023A00:
         strcat(&error_str[0]," Medium not present / Cover opened");
         break;  
      case 0x030200:
         strcat(&error_str[0]," No Seek complete");
         break;  
      case 0x031100:
         strcat(&error_str[0]," UnRecoverd read error");
         break;
      case 0x040800:
         strcat(&error_str[0]," Transfer protocol error");
         break;
      case 0x052000:
         strcat(&error_str[0]," Invalid command operation code");
         break;
      case 0x052001:
         strcat(&error_str[0]," Audio Buffer not set");
         break;  
      case 0x052100:
         strcat(&error_str[0]," Logical block address out of range");
         break;  
      case 0x052400:
         strcat(&error_str[0]," Invalid Field in command packet");
         break;
      case 0x052401:
         strcat(&error_str[0]," Invalid audio command");
         break;
      case 0x052402:
         strcat(&error_str[0]," Configuration out of permitted period");
         break;
      case 0x053000:
         strcat(&error_str[0]," DVD-R"); //?
         break;
      case 0x053100:
         strcat(&error_str[0]," Wrong Read Type"); //?
         break;
      case 0x056300:
         strcat(&error_str[0]," End of user area encountered on this track");
         break;  
      case 0x062800:
         strcat(&error_str[0]," Medium may have changed");
         break;  
      case 0x0B5A01:
         strcat(&error_str[0]," Operator medium removal request");
         break;
   }

   if(!error_str[0])
      sprintf(&error_str[0],"Unknown %08X",err);
   return &error_str[0];
}


/* 
DVD_Read(void* dst, uint64_t offset, int len)
  Reads from any offset and handles alignment from device
  Synchronous function.
    return -1 if offset is out of range
*/
int DVD_Read(void* dst, uint64_t offset, int len)
{
   int ol = len;
   int ret = 0;
   char *sector_buffer = (char*)memalign(32,2048);

   while (len) {
      uint32_t sector = offset / 2048;
      ret |= DVD_LowRead64(sector_buffer, 2048, sector * 2048);
      uint32_t off = offset & 2047;

      int rl = 2048 - off;
      if (rl > len) rl = len;
      memcpy(dst, sector_buffer + off, rl);
      offset += rl;
      len -= rl;
      dst += rl;
   }
   free(sector_buffer);
   if(ret) return -1;

   return ol;
}

int read_sector(void* buffer, int numSectors, uint32_t sector){
   return DVD_Read(buffer, sector*2048, numSectors*2048);
}

int read_direntry(unsigned char* direntry)
{
   int nrb = *direntry++;
   ++direntry;

   int sector;

   direntry += 4;
   sector = (*direntry++) << 24;
   sector |= (*direntry++) << 16;
   sector |= (*direntry++) << 8;
   sector |= (*direntry++);

   int size;

   direntry += 4;

   size = (*direntry++) << 24;
   size |= (*direntry++) << 16;
   size |= (*direntry++) << 8;
   size |= (*direntry++);

   direntry += 7; // skip date

   int flags = *direntry++;
   ++direntry; ++direntry; direntry += 4;

   int nl = *direntry++;

   char* name = DVDToc->file[files].name;

   DVDToc->file[files].sector = sector;
   DVDToc->file[files].size = size;
   DVDToc->file[files].flags = flags;

   if ((nl == 1) && (direntry[0] == 1)) // ".."
   {
      DVDToc->file[files].name[0] = 0;
      if (last_current_dir != sector)
         files++;
   }
   else if ((nl == 1) && (direntry[0] == 0))
   {
      last_current_dir = sector;
   }
   else
   {
      if (is_unicode)
      {
         int i;
         for (i = 0; i < (nl / 2); ++i)
            name[i] = direntry[i * 2 + 1];
            name[i] = 0;
            nl = i;
      }
      else
      {
         memcpy(name, direntry, nl);
         name[nl] = 0;
      }

      if (!(flags & 2))
      {
         if (name[nl - 2] == ';')
            name[nl - 2] = 0;

            int i = nl;
            while (i >= 0)
               if (name[i] == '.')
                  break;
               else
                  --i;
                  ++i;
      }
      else
      {
         name[nl++] = '/';
         name[nl] = 0;
      }

      files++;
   }
   return nrb;
}


void read_directory(int sector, int len)
{
   int ptr = 0;
   unsigned char *sector_buffer = (unsigned char*)memalign(32,2048);
   read_sector(sector_buffer, 1, sector);
  
   files = 0;
   memset(DVDToc,0,sizeof(file_entries));
   while (len > 0)
   {
      ptr += read_direntry(sector_buffer + ptr);
      if (ptr >= 2048 || !sector_buffer[ptr])
      {
         len -= 2048;
         sector++;
         read_sector(sector_buffer, 1, sector);
         ptr = 0;
      }
   }
   free(sector_buffer);
}


int dvd_read_directoryentries(uint64_t offset, int size)
{
   int sector = 16;
   unsigned char *bufferDVD = (unsigned char*)memalign(32,2048);
   struct pvd_s* pvd = 0;
   struct pvd_s* svd = 0;

   if(DVDToc)
   {
      free(DVDToc);
      DVDToc = NULL;
   }
   DVDToc = memalign(32,sizeof(file_entries));

   while (sector < 32)
   {
      if (read_sector(bufferDVD, 1, sector)!=2048)
      {
         free(bufferDVD);
         free(DVDToc);
         DVDToc = NULL;
//       WriteFont(25+(0*116)+10,125, "Fatal_ERROR...\n");
//       sleep (5);
      }
      if (!memcmp(((struct pvd_s *)bufferDVD)->id, "\2CD001\1", 8))
      {
         svd = (void*)bufferDVD;
         break;
      }
      ++sector;
   }

   if (!svd)
   {
      sector = 16;
      while (sector < 32)
      {
         if (read_sector(bufferDVD, 1, sector)!=2048)
         {
            free(bufferDVD);
            free(DVDToc);
            DVDToc = NULL;
//          WriteFont(25+(0*116)+10,125, "Fatal_ERROR...\n");
//          sleep (5); 	  
         }
      
         if (!memcmp(((struct pvd_s *)bufferDVD)->id, "\1CD001\1", 8))
         {
            pvd = (void*)bufferDVD;
            break;
         }
         ++sector;
      }
   }
  
   if ((!pvd) && (!svd))
   {
      free(bufferDVD);
      free(DVDToc);
      DVDToc = NULL;
   }
  
   files = 0;
   if (svd)
   {
      is_unicode = 1;
      read_direntry(svd->root_direntry);
   }
   else
   {
      is_unicode = 0;
      read_direntry(pvd->root_direntry);
   }
  
   if((size + offset) == 0)  // enter root
      read_directory(DVDToc->file[0].sector, DVDToc->file[0].size);
   else
      read_directory(offset>>11, size);

   free(bufferDVD);
   if(files>0)
      num_entries = files;

   return num_entries;
}


int gettype_disc() {

   //Attempt to determine the disc type from the 32 byte header at 0x80000000
   const char *dvdid = (const char*)0x80000000;

   //"COBRAM" is Cobra MultiBoot
   if (strncmp(dvdid, "COBRAM", 6) == 0) {
      dvdDiscTypeStr = CobraStr;
      return COBRA_MULTIGAME_DISC;
   }

   //"RGCOSD" or "GGCOSD" is GCOS Single Layer MultiGame Disc
   if ((strncmp(dvdid, "GGCOSD", 6) == 0) || (strncmp(dvdid, "RGCOSD", 6) == 0)) {
      dvdDiscTypeStr = GCOSD5Str;
      return GCOSD5_MULTIGAME_DISC;
   }

   //"GCOPDVD9" is GCOS Dual Layer MultiGame Disc
   if (strncmp(dvdid, "GCOPDVD9", 8) == 0) {
      dvdDiscTypeStr = GCOSD9Str;
      return GCOSD9_MULTIGAME_DISC;
   }

   //Attempt to identify a ISO9660 disc
   char *checkBuf = (char*)memalign(32,32);
   DVD_Read((void*)checkBuf,32769,32);
   if (strncmp(checkBuf, "CD001", 5) == 0) {
      dvdDiscTypeStr = ISO9660Str;
      return ISO9660_DISC;  //"CD001" at 32769 is iso9660
   }
   free(checkBuf);

   if ((*(volatile unsigned int*)(0x8000001C)) == DVD_MAGIC) {
      if(dvdid[6]) {
      dvdDiscTypeStr = MDStr;
      return MULTIDISC_DISC;  //Gamecube 2 Disc (or more?) Game
      }
      else {
      dvdDiscTypeStr = GCDStr;
      return GAMECUBE_DISC;   //Gamecube Game
      }
   }

   dvdDiscTypeStr = UnkStr;
   return UNKNOWN_DISC;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void DVD_Initialize()
{
// DVD_Reset(DVD_RESETHARD);
   DVD_Init ();
   retDVD = DVD_Mount();
   dvd_read_id();

   if(!is_gamecube()) { return; }    // Is Not GC...too bad, so sad!!
   else {
      if((dvd_get_error()>>24) > 1) {
         DVD_Reset(DVD_RESETHARD);
         dvd_read_id();
      }
   //{ WriteFont(25+(0*116)+10,125, "Initialization Complete"); }
   }
   return;
}

void GCM()
{
   if(__wkfSpiReadId() != 0 && __wkfSpiReadId() != 0xFFFFFFFF) {
      DrawImage(TEX_DVDOUT, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(1.70*116)+10,125, "WKF DETECTED");
      WriteFont(25+(1.60*116)+10,300, "Not Supported");
   }
   else {
      int dvdDiscTypeInt = 0;
      DVD_Initialize();
      dvdDiscTypeInt = gettype_disc();

      //NOTE: if GCM is detected then just kill this program and reboot to load GCM :) 
      if((dvdDiscTypeInt == GAMECUBE_DISC) || (dvdDiscTypeInt == MULTIDISC_DISC)) { exit(1); }
      else {
         dvd_motor_off();
         DrawImage(TEX_DVDOUT, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         WriteFont(25+(2.25*116)+10,125, "DVD");
         WriteFont(25+(1.75*116)+10,300, "INVALID DISC");
      }
   }
   DrawFrameFinish();
   sleep(3);
}

void DVD_ISO9660(char *EmuName)
{
   // SAFETY CHECK TO PREVENT MOUNTING WIIKEY FUSION
   if(__wkfSpiReadId() != 0 && __wkfSpiReadId() != 0xFFFFFFFF) {
      DrawImage(TEX_DVDOUT, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(1.70*116)+10,125, "WKF DETECTED");
      WriteFont(25+(1.60*116)+10,300, "Not Supported");
   }
   else {
      int dvdDiscTypeInt = 0;
      DVD_Initialize();
      dvdDiscTypeInt = gettype_disc();

      if(dvdDiscTypeInt != ISO9660_DISC) {
         dvd_motor_off();
         DrawImage(TEX_DVDOUT, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         WriteFont(25+(2.25*116)+10,125, "DVD");
         WriteFont(25+(1.78*116)+10,300, "Not ISO9660");
      }
      else if(dvdDiscTypeInt == ISO9660_DISC) {
         int retDVD=0;
         int rootDVD=0;
         int rootDVD1=0;
//         int emusDVD=0;
//         int megaDVD=0;

         // NOTE: FOR SD SEARCH WE SHOOK THE TREE...NOW FOR DVD SEARCH, WERE GONNA CUT THE DAMN TREE DOWN
         while((retDVD==0)){ 
            retDVD = 1;
            num_entries = dvd_read_directoryentries(offset, size);	
            if(num_entries <= 0) { rootDVD = 1; }
            else {
               dir = memalign(32, num_entries * sizeof(file_handle));
               int i;
               // SEARCH ROOT DIR
               if (rootDVD == 0){
                  for(i=0; i<num_entries; ++i){
                     if (strcmp(DVDToc->file[i].name, EmuName) == 0){
                        (dir)[i].fileBase = (uint64_t)(((uint64_t)DVDToc->file[i].sector)*2048);
                        u8 *dol = (u8*) memalign(32,DVDToc->file[i].size);
                        DVD_Read(dol,(dir)[i].fileBase,DVDToc->file[i].size);
                        DOLtoARAM(dol);
                        return;
                     }
                  }
               } rootDVD1 = 1;
               // SEARCH EMUS DIR
 /*              if (emusDVD == 0){
                  for(i=0; i<num_entries; ++i){
                     if (strcmp(DVDToc->file[i].name, "emus/") == 0){
                        (dir)[i].fileBase = (uint64_t)(((uint64_t)DVDToc->file[i].sector)*2048);
                        num_entries = dvd_read_directoryentries((dir)[i].fileBase, DVDToc->file[i].size);
                        if(num_entries <= 0) { emusDVD=1; }
                        else {
                           dir = memalign(32, num_entries * sizeof(file_handle) );
                           for(i=0; i<num_entries; ++i){
                              (dir)[i].fileBase = (uint64_t)(((uint64_t)DVDToc->file[i].sector)*2048);
                              if (strcmp(DVDToc->file[i].name, EmuName) == 0){
                                 u8 *dol = (u8*) memalign(32,DVDToc->file[i].size);
                                 DVD_Read(dol,(dir)[i].fileBase,DVDToc->file[i].size);
                                 DOLtoARAM(dol);
                                 return;
                              }
                           } // emusDVD=1;
                        }
                     } emusDVD=1;
                  }
               }
               // SEARCH MEGALOADER DIR
               if (megaDVD == 0){
                  for(i=0; i<num_entries; ++i){
                     if (strcmp(DVDToc->file[i].name, "megaloader/") == 0){
                        (dir)[i].fileBase = (uint64_t)(((uint64_t)DVDToc->file[i].sector)*2048);
                        num_entries = dvd_read_directoryentries((dir)[i].fileBase, DVDToc->file[i].size);
                        if(num_entries <= 0) { megaDVD=1; }
                        else {
                           dir = memalign(32, num_entries * sizeof(file_handle) );
                           for(i=0; i<num_entries; ++i){
                              (dir)[i].fileBase = (uint64_t)(((uint64_t)DVDToc->file[i].sector)*2048);
                              if (strcmp(DVDToc->file[i].name, EmuName) == 0){
                                 u8 *dol = (u8*) memalign(32,DVDToc->file[i].size);
                                 DVD_Read(dol,(dir)[i].fileBase,DVDToc->file[i].size);
                                 DOLtoARAM(dol);
                                 return;
                              }
                           } // emusDVD=1;
                        }
                     } emusDVD=1;
                  }
               }
  */          }
         }
         // EXIT DVD SEARCH
         // DOL NOT FOUND ON DVD..
         dvd_motor_off();
         DrawImage(TEX_DVDIN, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         WriteFont(25+(2.25*116)+10,125, "DVD");
         WriteFont(25+(1.95*116)+10,150, "Searching...");
//         if ( (emusDVD == 1) && (megaDVD == 1) ) { WriteFont(25+(1.68*116)+10,300, "DOL Not Found"); }
//         else if ( (rootDVD = 1) ) { WriteFont(25+(1.68*116)+10,300, "No Data Found"); }
         if ( (rootDVD1 = 1) ) { WriteFont(25+(1.68*116)+10,300, "DOL Not Found"); }
         else if ( (rootDVD = 1) ) { WriteFont(25+(1.68*116)+10,300, "No Data Found"); }
      }
   }
   sprintf(Found,"Could Not Find: %s",EmuName);
   WriteFont(25+(0*116)+10,425, Found);
   DrawFrameFinish();
   sleep(8);
   return;
}
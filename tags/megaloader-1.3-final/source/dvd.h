#ifndef DVD_H
#define DVD_H

#include "aram/sidestep.h"
#include "FrameBufferMagic.h"
#include "IPLFontWrite.h"
#include "main.h"
#include "wkf.h"


#define DVD_MAGIC   0xC2339F3D

//Disc Types
#define CobraStr    "Cobra MultiGame"
#define GCOSD5Str   "GCOS MultiGame"
#define GCOSD9Str   "GCOS MultiGame"
#define MDStr       "2-Disc Gamecube"
#define GCDStr      "Gamecube"
#define ISO9660Str  "ISO9660"
#define UnkStr      "Unknown"
#define NotInitStr  "Un-Initialized"

#define MAXIMUM_ENTRIES_PER_DIR 2048
//#define FILE_BROWSER_MAX_PATH_LEN 128



//DVD Disc Type (enum)
enum dvdDiscTypes
{
	COBRA_MULTIGAME_DISC=0,     //Cobra Multigame Disc, games can be every 32Kb interval
	GCOSD5_MULTIGAME_DISC,      //GCOS Single Layer Disc, games can be every 128Kb interval
	GCOSD9_MULTIGAME_DISC,      //GCOS Single Dual Disc, games can be every 128Kb interval
	MULTIDISC_DISC,             //Multi Disc Gamecube Game
	GAMECUBE_DISC,              //Single Disc Gamecube Game
	ISO9660_DISC,               //ISO9660 Filesystem Disc
	UNKNOWN_DISC                //Unknown
};


typedef struct {
	char name[1024];            // File or Folder, absolute path goes here
	uint64_t fileBase;          // Raw sector on device
	unsigned int offset;        // Offset in the file
	unsigned int size;          // size of the file
	int fileAttrib;             // IS_FILE or IS_DIR
	int status;                 // is the device ok
	FILE *fp;                   // file pointer
} file_handle;



//File type
enum fileTypes
{
  IS_FILE=0,
  IS_DIR,
  IS_SPECIAL
};

/*
typedef struct {
	char name[FILE_BROWSER_MAX_PATH_LEN];
	uint64_t discoffset;        // Only necessary for DVD
	unsigned int offset;        // Keep track of our offset in the file
	unsigned int size;
	unsigned int attr;
} fileBrowser_file;
*/

typedef struct
{
	char name[128];
	int flags;
	int sector, size;
} file_entry; 


typedef struct
{
	file_entry file[MAXIMUM_ENTRIES_PER_DIR];
} file_entries; 


struct pvd_s
{
	char id[8];
	char system_id[32];
	char volume_id[32];
	char zero[8];
	unsigned long total_sector_le, total_sect_be;
	char zero2[32];
	unsigned long volume_set_size, volume_seq_nr;
	unsigned short sector_size_le, sector_size_be;
	unsigned long path_table_len_le, path_table_len_be;
	unsigned long path_table_le, path_table_2nd_le;
	unsigned long path_table_be, path_table_2nd_be;
	unsigned char root_direntry[34];
	char volume_set_id[128], publisher_id[128], data_preparer_id[128], application_id[128];
	char copyright_file_id[37], abstract_file_id[37], bibliographical_file_id[37];
	// some additional dates, but we don't care for them :)
}  __attribute__((packed));


extern file_entries *DVDToc;

void dvd_motor_off();
//unsigned int dvd_get_error(void);
//int dvd_read_directoryentries(uint64_t offset, int size);
//void read_directory(int sector, int len);
//int read_direntry(unsigned char* direntry);
//int read_sector(void* buffer, int numSectors, u32 sector);
//int dvd_read(void* dst,unsigned int len, unsigned int offset);
//int dvd_read_id();
//int DVD_LowRead64(void* dst, unsigned int len, uint64_t offset);

void DVD_Initialize();
int gettype_disc();
void GCM();
void DVD_ISO9660(char *EmuName);

#endif


/* main.h 
	- defines and externs
	by emu_kidid
 */

#ifndef MAIN_H
#define MAIN_H
#include <gccore.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <ogcsys.h>

#include "aram/sidestep.h"
#include "dvd.h"
#include "FrameBufferMagic.h"
#include "IPLFontWrite.h"
#include "wkf.h"

#define _VERSION "v1.3"

//Console Version Type Helpers
#define GC_CPU_VERSION01 0x00083214
#define GC_CPU_VERSION02 0x00083410
#ifndef mfpvr
#define mfpvr()  ({unsigned int rval; asm volatile("mfpvr %0" : "=r" (rval)); rval;})
#endif
#define is_gamecube() (((mfpvr() == GC_CPU_VERSION01)||((mfpvr() == GC_CPU_VERSION02))))


typedef struct {
  u8   mid;         //Manufacturer ID
  char oid[2];      //OEM/Application ID
  char pnm[5];      //Product Name
  u8   prv;         //product version 0001 0001 = 1.1
  u32  psn;         //product serial number
  u16  mdt;         //bottom 4 bits are month, 8 bits above is year since 2000
  u8   unk;
} CIDdata __attribute__((aligned(32)));


/*** 2D Video Globals ***/
extern GXRModeObj *vmode;	/*** Graphics Mode Object ***/
extern u32 *xfb[2];		/*** Framebuffers ***/
extern int whichfb;		/*** Frame buffer toggle ***/

int retDVD;
int rootDVD;
int emusDVD;
int megaDVD;
char EmuName[50];
char RunEmu[100];
char Found[100];

void __SYS_ReadROM(void *buf,u32 len,u32 offset);
char IPLInfo[256] __attribute__((aligned(32)));
#endif
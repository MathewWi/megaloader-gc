/* deviceHandler-wiikeyfusion.h
	- device implementation for Wiikey Fusion
	by emu_kidid
 */


#ifndef DEVICE_HANDLER_WKF_H
#define DEVICE_HANDLER_WKF_H

#include <gccore.h>
#include <ogc/disc_io.h>

#include "aram/sidestep.h"
#include "FrameBufferMagic.h"
#include "IPLFontWrite.h"
#include "main.h"

#define DEVICE_TYPE_GC_WKF	(('W'<<24)|('K'<<16)|('F'<<8)|'!')


//extern const DISC_INTERFACE __io_wkf;


void wkfWriteRam(int offset, int data);
void wkfWriteOffset(int offset);
int wkfSpiRead(unsigned char *buf, unsigned int addr, int len);
void wkfRead(void* dst, int len, u64 offset);
unsigned int __wkfSpiReadId();
void wkfReinit();



void WKF(char *EmuName);

#endif
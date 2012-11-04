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

//extern WKFfile_handle initial_WKF;
extern const DISC_INTERFACE __io_wkf;

//int deviceHandler_WKF_readDir(WKFfile_handle*, file_handle**, unsigned int);
//int deviceHandler_WKF_readFile(file_handle*, void*, unsigned int);
//int deviceHandler_WKF_seekFile(file_handle*, unsigned int, unsigned int);
//void deviceHandler_WKF_setupFile(file_handle* file, file_handle* file2);
//int deviceHandler_WKF_init(file_handle* file);
//int deviceHandler_WKF_deinit(file_handle* file);


void wkfWriteRam(int offset, int data);
void wkfWriteOffset(int offset);
int wkfSpiRead(unsigned char *buf, unsigned int addr, int len);
void wkfRead(void* dst, int len, u64 offset);
unsigned int __wkfSpiReadId();
void wkfReinit();



void WKF(char *EmuName);

#endif
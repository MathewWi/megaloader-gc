#include <stdio.h>
#include <gccore.h>		/*** Wrapper to include common libogc headers ***/
#include <ogcsys.h>		/*** Needed for console support ***/
#include <ogc/color.h>
#include <ogc/lwp.h>
#include <ogc/video_types.h>
#include <ogc/machine/processor.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sdcard/gcsd.h>
#include <fat.h>
#include <iso9660.h>

#include "aram/sidestep.h"
#include "main.h"
#include "FrameBufferMagic.h"
#include "IPLFontWrite.h"
#include "dvd.h"

#define DEFAULT_FIFO_SIZE    (256*1024)					//(64*1024) minimum
	
GXRModeObj *vmode = NULL;								//Graphics Mode Object
u32 *xfb[2] = { NULL, NULL };							//Framebuffers
int whichfb = 0;										//Frame buffer toggle
char *videoStr = NULL;

FILE *fp;
char EmuName[10];
int ret  = 0;
int ret1 = 0;
int ret2 = 0;
int x = 25;
int y = 85;
int curMenu = 0;

const DISC_INTERFACE* slotA = &__io_gcsda;
const DISC_INTERFACE* slotB = &__io_gcsdb;
const DISC_INTERFACE*  dvd1 = &__io_gcdvd;

char IPLInfo[256] __attribute__((aligned(32)));


static void ProperScanPADS()	{ 
	PAD_ScanPads(); 
}

void doBackdrop0()
{
	DrawFrameStart();
	DrawMenuLogos0() ;
	DrawMenuButtons();
}

void doBackdrop1()
{
	DrawFrameStart();
	DrawMenuLogos1() ;
	DrawMenuButtons();
}

void populateVideoStr(GXRModeObj *vmode) {
	if(vmode == &TVPal576ProgScale) {
		videoStr = Prog576Str;
	}
	else if(vmode == &TVNtsc480Prog) {
		videoStr = Prog480Str;
	}
	else if(vmode == &TVPal576IntDfScale) {
		videoStr = PALStr;
	}
	else if(vmode == &TVNtsc480IntDf) {
		videoStr = NTSCStr;
	}
	else {
		videoStr = UnkStr;
	}
}

/* Initialise Video, PAD, Font */
void* Initialise (void)
{
	VIDEO_Init ();
	PAD_Init (); 
	
	__SYS_ReadROM(IPLInfo,256,0);						// Read IPL tag

	// Wii has no IPL tags for "PAL" so let libOGC figure out the video mode
	if(!is_gamecube()) {
		vmode = VIDEO_GetPreferredMode(NULL); 			//Last mode used
	}
	else {	// Gamecube, determine based on IPL
		if(VIDEO_HaveComponentCable()) {
			if((strstr(IPLInfo,"PAL")!=NULL)) {
				vmode = &TVPal576ProgScale; 			//Progressive 576p
			}
			else {
				vmode = &TVNtsc480Prog; 				//Progressive 480p
			}
		}
		else {
			//try to use the IPL region
			if(strstr(IPLInfo,"PAL")!=NULL) {
				vmode = &TVPal576IntDfScale;         	//PAL
			}
			else if(strstr(IPLInfo,"NTSC")!=NULL) {
				vmode = &TVNtsc480IntDf;        		//NTSC
			}
			else {
				vmode = VIDEO_GetPreferredMode(NULL); 	//Last mode used
			}
		}
	}
	populateVideoStr(vmode);

	VIDEO_Configure (vmode);
	xfb[0] = (u32 *) MEM_K0_TO_K1 (SYS_AllocateFramebuffer (vmode));
	xfb[1] = (u32 *) MEM_K0_TO_K1 (SYS_AllocateFramebuffer (vmode));
	VIDEO_ClearFrameBuffer (vmode, xfb[0], COLOR_BLACK);
	VIDEO_ClearFrameBuffer (vmode, xfb[1], COLOR_BLACK);
	VIDEO_SetNextFramebuffer (xfb[0]);
	VIDEO_SetPostRetraceCallback (ProperScanPADS);
	VIDEO_SetBlack (0);
	VIDEO_Flush ();
	VIDEO_WaitVSync ();
	if (vmode->viTVMode & VI_NON_INTERLACE) {
		VIDEO_WaitVSync ();
	}
	
	// setup the fifo and then init GX
	void *gp_fifo = NULL;
	gp_fifo = MEM_K0_TO_K1 (memalign (32, DEFAULT_FIFO_SIZE));
	memset (gp_fifo, 0, DEFAULT_FIFO_SIZE);
	GX_Init (gp_fifo, DEFAULT_FIFO_SIZE);
	// clears the bg to color and clears the z buffer
	GX_SetCopyClear ((GXColor){0,0,0,255}, 0x00000000);
	// init viewport
	GX_SetViewport (0, 0, vmode->fbWidth, vmode->efbHeight, 0, 1);
	// Set the correct y scaling for efb->xfb copy operation
	GX_SetDispCopyYScale ((f32) vmode->xfbHeight / (f32) vmode->efbHeight);
	GX_SetDispCopyDst (vmode->fbWidth, vmode->xfbHeight);
	GX_SetCullMode (GX_CULL_NONE); // default in rsp init
	GX_CopyDisp (xfb[0], GX_TRUE); // This clears the efb
	GX_CopyDisp (xfb[0], GX_TRUE); // This clears the xfb

	init_font();
	init_textures();
	whichfb = 0;
	
	return xfb[0];
}


void RunDOL(char *EmuName) 
{
	char RunEmu[10];
	char Found[10];

	fatInitDefault ();
	DrawFrameStart();

	ret =  slotA->startup() && fatMountSimple("SDA1", slotA);
	ret1 = slotB->startup() && fatMountSimple("SDB1", slotB);	

//	CHECK WHICH SD CARDSLOT(S) ARE DETECTED AND DRAW
//	SD CARDSLOT A	
	if (ret <= 0) {
		DrawImage(TEX_SDOUT, 25+(0.50*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
		WriteFont(25+(0.60*116)+10,190, "SD CARD A");
		WriteFont(25+(0.60*116)+10,215, "No SD Card");
	}
	else {
		DrawImage(TEX_SDIN, 25+(0.50*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
		WriteFont(25+(0.60*116)+10,190, "SD CARD A");
	}
//	SD CARDSLOT B
	if (ret1 <= 0) {
		DrawImage(TEX_SDOUT, 25+(3*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
		WriteFont(25+(3.10*116)+10,190, "SD CARD B");
		WriteFont(25+(3.10*116)+10,215, "No SD Card");
	}
	else {
		DrawImage(TEX_SDIN, 25+(3*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
		WriteFont(25+(3.10*116)+10,190, "SD CARD B");
	}
	
//	SEARCH BOTH CARDSLOTS....SHAKE THE TREE AND SEE WHAT FALLS
	if (ret > 0 || ret1 > 0 )
	{
		if (ret > 0){ 
		WriteFont(25+(0.60*116)+10,215, "Searching..."); 
		}
		
		sprintf(RunEmu,"SDA1:/%s",EmuName);
		fp = fopen(RunEmu, "r");
		if (fp == NULL)
		{
			sprintf(RunEmu,"SDA1:/emus/%s",EmuName);
			fp = fopen(RunEmu, "r");
			if (fp == NULL)
			{
				sprintf(RunEmu,"SDA1:/megaloader/%s",EmuName);
				fp = fopen(RunEmu, "r");
				if (fp == NULL)
				{
					if (ret > 0) WriteFont(25+(0.60*116)+10,265, "Not Found"); 
					if (ret1 > 0) WriteFont(25+(3.10*116)+10,215, "Searching...");
					
					sprintf(RunEmu,"SDB1:/%s",EmuName);
					fp = fopen(RunEmu, "r");
					if (fp == NULL)
					{
						sprintf(RunEmu,"SDB1:/emus/%s",EmuName);
						fp = fopen(RunEmu, "r");
						if (fp == NULL)
						{
							sprintf(RunEmu,"SDB1:/megaloader/%s",EmuName);
							fp = fopen(RunEmu, "r");
							if (fp == NULL)
							{
								if (ret1 > 0) WriteFont(25+(3.10*116)+10,265, "Not Found");
								DrawFrameFinish();
							}
							else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0.75*116)+10,425, Found); DrawFrameFinish(); }
						}
						else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0.75*116)+10,425, Found); DrawFrameFinish(); }
					}
					else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0.75*116)+10,425, Found); DrawFrameFinish(); }
				}
				else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0.75*116)+10,425, Found); DrawFrameFinish(); }
			}
			else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0.75*116)+10,425, Found); DrawFrameFinish(); }
		}
		else {	sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,125, Found); DrawFrameFinish(); }
	}
	
//	IF Fp EXISTS THEN TRY DOLtoARAM
	if (fp != NULL)
	{	
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if ((size > 0) && (size < (AR_GetSize() - (64*1024)))) {
			u8 *dol = (u8*) memalign(32, size);
			if (dol) {
				fread(dol, 1, size, fp);
				DOLtoARAM(dol);
			}
		}
		fclose(fp);
		return;
	}

//	DOL NOT FOUND ON SD SLOT A/B...UNMOUNT CARD
	if (ret > 0)  fatUnmount("SDA1");
	if (ret1 > 0) fatUnmount("SDB1");	

//	DOL NOT FOUND ON SD SLOT A/B...SEARCH ON DISC
	DVD_ISO9660(EmuName);

	return;
}



void main_loop()
{ 
	//MAIN MENU
	VIDEO_WaitVSync();
	PAD_ScanPads();

	// DISPLAY MENU BASED ON curMenu
	if (curMenu == 0) {
		doBackdrop0();
		DrawMenuSelector(x,y);
		DrawFrameFinish(); 
	}
	if (curMenu == 1) {
		doBackdrop1(); 
		DrawMenuSelector(x,y);
		DrawFrameFinish(); 
	}
	
	while(1){
		// SWAP MENU
		if (PAD_ButtonsDown(0) & PAD_TRIGGER_R) {
			if (curMenu == 0) { curMenu = 1; return; }
			if (curMenu == 1) { curMenu = 0; return; }
		}

		//EXIT TO SWISS	
		if (PAD_ButtonsDown(0) & PAD_TRIGGER_Z) {
			sprintf(EmuName, "swiss.dol");
			RunDOL(EmuName);
		}
		
		//MENU SELECTOR
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_LEFT) && curMenu == 0) { 	
			if (x <= 460 && x > 25) { x = x - 145; } doBackdrop0(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT) && curMenu == 0)  { 	
			if (x >= 25 && x < 460) { x = x + 145; } doBackdrop0(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_UP) && curMenu == 0)  { 	
			if (y > 85 && y <= 285) { y = y - 100; } doBackdrop0(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_DOWN) && curMenu == 0)  { 
			if (y < 285 && y >= 85) { y = y + 100; } doBackdrop0(); DrawMenuSelector(x,y); DrawFrameFinish();

		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_LEFT) && curMenu == 1) { 	
			if (x <= 460 && x > 25) { x = x - 145; } doBackdrop1(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT) && curMenu == 1)  { 	
			if (x >= 25 && x < 460) { x = x + 145; } doBackdrop1(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_UP) && curMenu == 1)  { 	
			if (y > 85 && y <= 285) { y = y - 100; } doBackdrop1(); DrawMenuSelector(x,y); DrawFrameFinish();
		}
		if ((PAD_ButtonsDown(0) & PAD_BUTTON_DOWN) && curMenu == 1)  { 
			if (y < 285 && y >= 85) { y = y + 100; } doBackdrop1(); DrawMenuSelector(x,y); DrawFrameFinish();

		}
		//ENTER CONFIRMATION MENU 0 OR MENU 1
		if (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
			if (curMenu == 0) { DrawConfirm0(x,y); return; }
			if (curMenu == 1) { DrawConfirm1(x,y); return; }
		}
	}
}	


/****************************************************************************
* Main
****************************************************************************/
int main () 
{
	void *fb;
	
	fb = Initialise();
	if(!fb) {
		return -1;
	}
	

	while(1) {
		main_loop();
	}
	return 0;
}
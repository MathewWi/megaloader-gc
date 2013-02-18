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

#include "main.h"

#define DEFAULT_FIFO_SIZE    (256*1024)     //(64*1024) minimum
int easteregg = 0;
GXRModeObj *vmode = NULL;                   //Graphics Mode Object
u32 *xfb[2] = { NULL, NULL };               //Framebuffers
int whichfb = 0;                            //Frame buffer toggle

FILE *fp;
int x = 25;
int y = 85;
int curMenu = 0;

const DISC_INTERFACE* slotA = &__io_gcsda;
const DISC_INTERFACE* slotB = &__io_gcsdb;
const DISC_INTERFACE*  dvd1 = &__io_gcdvd;


static void ProperScanPADS() { 
   PAD_ScanPads(); 
}

// MAIN MENU BUTTONS
void DrawMenuButtons()
{
   DrawImage(TEX_BTNA, 40+(0*116), 420, BTNA_WIDTH,BTNA_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_BTNZ, 40+(4*116), 420, BTNZ_WIDTH,BTNZ_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   WriteFont(110,430, "SELECT");
   WriteFont(400,430, "SWISS");
}

// DRAW SELECTION BOX BASED ON X/Y POSITION
void DrawMenuSelector(x,y)
{
   DrawImage(TEX_LOGOBORDER, x, y, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
}

//DRAW ICONS BASED ON CURRENT WINDOW
void DrawMenuLogos()
{
   if (curMenu==0){     
      DrawImage(TEX_LOGONES,  40+(0*116),    100, LOGONES_WIDTH,LOGONES_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOSNES, 40+(1.25*116), 100, LOGOSNES_WIDTH,LOGOSNES_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGON64,  40+(2.5*116),  100, LOGON64_WIDTH,LOGON64_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOGBA,  40+(3.75*116), 100, LOGOGBA_WIDTH,LOGOGBA_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);

      DrawImage(TEX_LOGOGENESIS, 40+(0*116), 200, LOGOGENESIS_WIDTH,LOGOGENESIS_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOTGX,  40+(1.25*116), 200, LOGOTGX_WIDTH,LOGOTGX_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGONGCD, 40+(2.50*116), 200, LOGONGCD_WIDTH,LOGONGCD_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGONGP,  40+(3.75*116), 200, LOGONGP_WIDTH,LOGONGP_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);

      DrawImage(TEX_LOGOPSX,  40+(0*116),    300, LOGOPSX_WIDTH,LOGOPSX_HEIGHT,   0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOVECTREX,40+(1.25*116), 300, LOGOVECTREX_WIDTH, LOGOVECTREX_HEIGHT,0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOCOLECO, 40+(2.50*116), 300, LOGOCOLECO_WIDTH, LOGOCOLECO_HEIGHT,  0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOGC,	40+(3.75*116), 300, LOGOGC_WIDTH, LOGOGC_HEIGHT,    0, 0.0f, 1.0f, 0.0f, 1.0f);
     // DrawImage(TEX_Z, 40+(3.75*116), 300, Z_WIDTH, Z_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   }
   if (curMenu==1){
      DrawImage(TEX_LOGOSCUMMVM, 40+(0*116), 100, LOGOSCUMMVM_WIDTH,LOGOSCUMMVM_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOCHIP8, 40+(1.25*116),100, LOGOCHIP8_WIDTH,LOGOCHIP8_HEIGHT,0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOPONG,  40+(2.5*116), 100, LOGOPONG_WIDTH,LOGOPONG_HEIGHT,  0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOQUAKE, 40+(3.75*116),100, LOGOQUAKE_WIDTH,LOGOQUAKE_HEIGHT,0, 0.0f, 1.0f, 0.0f, 1.0f);

      DrawImage(TEX_LOGOFRUIT, 40+(0*116),   200, LOGOFRUIT_WIDTH,LOGOFRUIT_HEIGHT,0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOTETRIS,40+(1.25*116),200, LOGOTETRIS_WIDTH,LOGOTETRIS_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOAF,    40+(2.50*116),200, LOGOAF_WIDTH,LOGOAF_HEIGHT,      0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOBREAK, 40+(3.75*116),200, LOGOBREAK_WIDTH,LOGOBREAK_HEIGHT,0, 0.0f, 1.0f, 0.0f, 1.0f);

      DrawImage(TEX_LOGOSNOW,  40+(0*116),   300, LOGOSNOW_WIDTH,LOGOSNOW_HEIGHT,  0, 0.0f, 1.0f, 0.0f, 1.0f);
      DrawImage(TEX_LOGOMETH,  40+(1.25*116),300, LOGOMETH_WIDTH,LOGOMETH_HEIGHT,  0, 0.0f, 1.0f, 0.0f, 1.0f);
     // DrawImage(TEX_Y, 40+(2.50*116), 300, Y_WIDTH, Y_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      if (easteregg == 1) 
         DrawImage(TEX_LOGOQBOX, 40+(3.75*116), 300, LOGOQBOX_WIDTH,LOGOQBOX_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   }
}

//DRAW BACKGROUND AND BUTTONS
void doBackdrop()
{
   DrawFrameStart();
   DrawMenuLogos();
   DrawMenuButtons();
}

//DOL HAS BEEN FOUND, LOAD THE DOL
void RunDOL()
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
}


//SEARCH MEDIA LOCATION FOR DOL
void FindDOL(char *EmuName) 
{
   int ret = 0;
   int ret1 = 0;

   DrawFrameStart();
      fatInitDefault ();
//   ret =  slotA->startup() && slotA->isInserted();
//   ret1 = slotB->startup() && slotB->isInserted();
   ret =  slotA->startup() && fatMountSimple("SDA1", slotA);
   ret1 = slotB->startup() && fatMountSimple("SDB1", slotB);
   // CHECK WHICH SD CARDSLOT(S) ARE DETECTED
   // SD CARDSLOT A
   if (ret <= 0) {
      DrawImage(TEX_SDOUT, 25+(0.50*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(0.60*116)+10,190, "SD CARD A");
      WriteFont(25+(0.60*116)+10,215, "No SD Card");
//      slotA->shutdown();
   }
   else {
//      fatInitDefault ();
      fatMountSimple("SDA1", slotA);
      DrawImage(TEX_SDIN, 25+(0.50*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(0.60*116)+10,190, "SD CARD A");
      WriteFont(25+(0.60*116)+10,215, "Searching...");
   }


   // SD CARDSLOT B
   if (ret1 <= 0) {
      DrawImage(TEX_SDOUT, 25+(3*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(3.10*116)+10,190, "SD CARD B");
      WriteFont(25+(3.10*116)+10,215, "No SD Card");
//      slotB->shutdown();
   }
   else {
//      fatInitDefault ();
      fatMountSimple("SDB1", slotB);
      DrawImage(TEX_SDIN, 25+(3*116), 125, 175, 200, 0, 0.0f, 1.0f, 0.0f, 1.0f);
      WriteFont(25+(3.10*116)+10,190, "SD CARD B");
      WriteFont(25+(3.10*116)+10,215, "Searching...");
   }


   // SEARCH SD CARDSLOT A
   if (ret > 0){
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
               WriteFont(25+(0.60*116)+10,265, "Not Found");
            }
            else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish();RunDOL();}
         }
         else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish();RunDOL(); }
      }
      else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish(); RunDOL();}
   }

   // SEARCH SD CARDSLOT B
   if (ret1 > 0){
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
               WriteFont(25+(3.10*116)+10,265, "Not Found"); 
            }
            else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish(); RunDOL();}
         }
         else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish(); RunDOL();}
      }
      else { sprintf(Found,"Found %s",RunEmu); WriteFont(25+(0*116)+10,425, Found); DrawFrameFinish(); RunDOL();}
   }
   
   // DOL NOT FOUND ON SD SLOT A/B...
   // SAFETY CHECK TO PREVENT MOUNTING WIIKEY FUSION
//   if(__wkfSpiReadId() != 0 && __wkfSpiReadId() != 0xFFFFFFFF) {
//      DrawImage(TEX_DVDOUT, 25+(1.25*116), 85, 300, 300, 0, 0.0f, 1.0f, 0.0f, 1.0f);
//      WriteFont(25+(1.70*116)+10,125, "WKF DETECTED");
//      WriteFont(25+(1.60*116)+10,300, "Not ISO9660 Disc");
//      DrawFrameFinish();
//      sleep(5);
//   }
//   else {
   // DOL NOT FOUND ON SD SLOT A/B...
   // SEARCH DVD
   DVD_ISO9660(EmuName);
   //}

   return;
}


// ENTER CONFIRMATION MENU
void DrawConfirm(x,y)
{
   // REFRESH BACKDROP TO HIDE MAIN MENU BUTTONS
   DrawFrameStart();
   DrawMenuLogos();
   DrawMenuSelector(x,y);

   if (curMenu==0){     // DRAW CONFIRMATION BOX 0
      DrawEmptyBox(25,85, vmode->fbWidth-30, 395, COLOR_BLACK);
      if (x == 25 && y == 85) {
         DrawImage(TEX_LOGONES,  25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "fceugx-gc.dol"); }	
      else if (x == 170 && y == 85) { 
         DrawImage(TEX_LOGOSNES, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "snes9xgx-gc.dol"); }
      else if (x == 315 && y == 85) {
         DrawImage(TEX_LOGON64,  25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "cube64.dol"); }
      else if (x == 460 && y == 85) {
         DrawImage(TEX_LOGOGBA,  25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
         sprintf(EmuName, "vbagx_gc.dol"); }	

      else if (x == 25 && y == 185) {
         DrawImage(TEX_LOGOGENESIS, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
         sprintf(EmuName, "genplus_cube.dol"); }
      else if (x == 170 && y == 185) {
         DrawImage(TEX_LOGOTGX,     25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "hugo_cube.dol");}
      else if (x == 315 && y == 185) {
         DrawImage(TEX_LOGONGCD,    25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "neocdredux.dol"); }
      else if (x == 460 && y == 185) {
         DrawImage(TEX_LOGONGP,     25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "neopopgc.dol"); }

      else if (x == 25 && y == 285) {
         DrawImage(TEX_LOGOPSX,     25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "cubeSX.dol"); }
      else if (x == 170 && y == 285) {
         DrawImage(TEX_LOGOVECTREX, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "vecxgc.dol"); }
      else if (x == 315 && y == 285) {
         DrawImage(TEX_LOGOCOLECO,  25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "collisionGC.dol"); }
      else if (x == 460 && y == 285) { GCM(); return; }
//	else if (x == 460 && y == 285) { return; }
      else { WriteFont(25+(2.25*116)+10,175, "unknown error"); return;}
   }

   if (curMenu==1){    // DRAW CONFIRMATION BOX 1
      DrawEmptyBox(25,85, vmode->fbWidth-30, 395, COLOR_BLACK);
      if (x == 25 && y == 85) {
         DrawImage(TEX_LOGOSCUMMVM, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "scummvm-gc.dol"); }	
      else if (x == 170 && y == 85) { 
         DrawImage(TEX_LOGOCHIP8, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "chip8.dol"); }
      else if (x == 315 && y == 85) {
         DrawImage(TEX_LOGOPONG, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "pong.dol"); }
      else if (x == 460 && y == 85) {
         DrawImage(TEX_LOGOQUAKE, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
         sprintf(EmuName, "quake.dol"); }	

      else if (x == 25 && y == 185) {
         DrawImage(TEX_LOGOFRUIT, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
         sprintf(EmuName, "fruitremover.dol"); }			
      else if (x == 170 && y == 185) {
         DrawImage(TEX_LOGOTETRIS, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "tetris.dol");}
      else if (x == 315 && y == 185) {
         DrawImage(TEX_LOGOAF, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "af.dol"); }
      else if (x == 460 && y == 185) {
         DrawImage(TEX_LOGOBREAK, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "breakout.dol"); }

      else if (x == 25 && y == 285) {
         DrawImage(TEX_LOGOSNOW, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "snowlords.dol"); }
      else if (x == 170 && y == 285) {
         DrawImage(TEX_LOGOMETH, 25+(2.25*116)-57, 125, 185, 145, 0, 0.0f, 1.0f, 0.0f, 1.0f);
         sprintf(EmuName, "methane.dol"); }
      else if (x == 315 && y == 285) { return; }
      else if (x == 460 && y == 285) {
         if (easteregg == 0) {
            DrawMenuLogos();
            DrawImage(TEX_LOGOQBOX, 40+(3.75*116), 300, LOGOQBOX_WIDTH,LOGOQBOX_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
            easteregg = 1;
            return;
         }
         if (easteregg == 1) {
            readme();
            easteregg = 0;
            return; 
         }
      }
      else { WriteFont(25+(2.25*116)+10,175, "unknown error"); return;}
   }

   DrawImage(TEX_BTNB, 25+(1.25*116)-50, 315, BTNB_WIDTH,BTNB_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   WriteFont(25+(1.25*116)+10,325, "CANCEL");
   DrawImage(TEX_BTNA, 25+(3.75*116), 315, BTNA_WIDTH,BTNA_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   WriteFont(25+(3.75*116)-75,325, "LOAD");
   DrawFrameFinish();

   while(1){
      //CONFIRM A = EXECUTE DOL	
      while (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
         FindDOL(EmuName);
         return;
      }
      // CANCEL B = EXIT CONFIRMATION MENU
      while (PAD_ButtonsDown(0) & PAD_BUTTON_B) { // do nothing, just return
         return;
      }
   }
}

// MAIN LOOP
void main_loop()
{
   //MAIN MENU
   VIDEO_WaitVSync();
   doBackdrop();
   DrawMenuSelector(x,y);
   DrawFrameFinish(); 

   while(1){

      // SWAP MENU
      if (PAD_ButtonsDown(0) & PAD_TRIGGER_R) {
         // if (curMenu == 0) { curMenu = 1; return; }
         // if (curMenu == 1) { curMenu = 0; return; }
         curMenu ^= 1; return;
      }

      //EXIT
      if (PAD_ButtonsDown(0) & PAD_BUTTON_START) {
         exit(1); }

      //EXIT TO SWISS
      if (PAD_ButtonsDown(0) & PAD_TRIGGER_Z) {
         sprintf(EmuName, "swiss.dol");
         FindDOL(EmuName);
      }

      //MENU SELECTOR MENU
      if ((PAD_ButtonsDown(0) & PAD_BUTTON_LEFT)) {
         if (x <= 460 && x > 25) { x = x - 145; } doBackdrop(); DrawMenuSelector(x,y); DrawFrameFinish(); }
      if ((PAD_ButtonsDown(0) & PAD_BUTTON_RIGHT)) {
         if (x >= 25 && x < 460) { x = x + 145; } doBackdrop(); DrawMenuSelector(x,y); DrawFrameFinish(); }
      if ((PAD_ButtonsDown(0) & PAD_BUTTON_UP)) {
         if (y > 85 && y <= 285) { y = y - 100; } doBackdrop(); DrawMenuSelector(x,y); DrawFrameFinish(); }
      if ((PAD_ButtonsDown(0) & PAD_BUTTON_DOWN)) {
         if (y < 285 && y >= 85) { y = y + 100; } doBackdrop(); DrawMenuSelector(x,y); DrawFrameFinish(); }

      //ENTER CONFIRMATION MENU 0 OR MENU 1
      if (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
         DrawConfirm(x,y);
         return;
      }
   }
}

/* Initialise Video, PAD, Font */
void* Initialise (void)
{
   int retPAD = 0;
  
   VIDEO_Init ();
   PAD_Init ();
   while(retPAD <= 0) { retPAD = PAD_ScanPads(); usleep(100); }
 
    __SYS_ReadROM(IPLInfo,256,0);                  // Read IPL tag
 
   // Wii has no IPL tags for "PAL" so let libOGC figure out the video mode
   if(!is_gamecube()) {
      vmode = VIDEO_GetPreferredMode(NULL);        //Last mode used
   }
   else {
      // Gamecube, determine based on IPL
      // If Trigger L detected during bootup, force 480i safemode
      // for Digital Component cable for SDTV compatibility.
      if(VIDEO_HaveComponentCable() && !(PAD_ButtonsDown(0) & PAD_TRIGGER_L)) {
         if((strstr(IPLInfo,"PAL")!=NULL)) {
            vmode = &TVPal576ProgScale;            //Progressive 576p
         }
         else {
            vmode = &TVNtsc480Prog;                //Progressive 480p
         }
      }
      else {
         //try to use the IPL region
         if(strstr(IPLInfo,"PAL")!=NULL) {
            vmode = &TVPal576IntDfScale;           //PAL
         }
         else if(strstr(IPLInfo,"NTSC")!=NULL) {
            vmode = &TVNtsc480IntDf;               //NTSC
         }
         else {
            vmode = VIDEO_GetPreferredMode(NULL);  //Last mode used
         }
      }
   }

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
   dvd_motor_off();

   while(1) {
     main_loop();
   }
   return 0;
}
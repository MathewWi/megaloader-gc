/* -----------------------------------------------------------
      FrameBufferMagic.h - Framebuffer routines with GX
	      - by emu_kidid & sepp256

      Version 1.0 11/11/2009
        - Initial Code
   ----------------------------------------------------------- */
   
#ifndef FRAMEBUFFERMAGIC_H
#define FRAMEBUFFERMAGIC_H

#include <gccore.h>

#include "dvd.h"
#include "IPLFontWrite.h"
#include "main.h"
#include "wkf.h"

#define _VERSION "v1.1"

#define B_NOSELECT 0
#define B_SELECTED 1

#define BUTTON_COLOUR_INNER 0x2088207C
#define BUTTON_COLOUR_OUTER COLOR_SILVER

#define PROGRESS_BOX_WIDTH  600
#define PROGRESS_BOX_HEIGHT 125

enum TextureId
{
	TEX_BACKDROP=0,
	TEX_SDIN,
	TEX_SDOUT,
	TEX_DVDIN,
	TEX_DVDOUT,	
	TEX_BANNER,
	TEX_BTNNOHILIGHT,
	TEX_BTNHILIGHT,
	TEX_BTNA,
	TEX_BTNB,
	TEX_BTNZ,
	TEX_LOGOBORDER,
//	TEX_LOGOATARI,
//	TEX_LOGOCOLECO,
	TEX_LOGOGBA,
	TEX_LOGOGENESIS,
	TEX_LOGON64,
	TEX_LOGONES,
	TEX_LOGONGCD,
	TEX_LOGONGP,
	TEX_LOGOPSX,
	TEX_LOGOSNES,
	TEX_LOGOTGX,
	TEX_LOGOVECTREX,
	TEX_LOGOSCUMMVM,
	TEX_LOGOCHIP8,
	TEX_LOGOPONG,
	TEX_LOGOQUAKE,
	TEX_LOGOFRUIT,
	TEX_LOGOTETRIS,
	TEX_LOGOAF,
	TEX_LOGOBREAK,
	TEX_LOGOSNOW,
	TEX_LOGOMETH,
	TEX_LOGOGC,
	TEX_LOGOQBOX
	
};

extern GXRModeObj *vmode;
extern u32 *xfb[2];
extern int whichfb;
extern void doBackdrop();

void init_textures();
void DrawImage(int textureId, int x, int y, int width, int height, int depth, float s1, float s2, float t1, float t2);
void DrawFrameStart();
void DrawFrameFinish();
void DrawMessageBox(int type, char *message);
void DrawRawFont(int x, int y, char *message);
void DrawSelectableButton(int x1, int y1, int x2, int y2, char *message, int mode, u32 color);
void DrawEmptyBox(int x1, int y1, int x2, int y2, int color);

void DrawMenuSelector(int x, int y);
void DrawMenuLogos0();
void DrawMenuLogos1();
void DrawDevices();
void DrawMenuButtons();
void DrawConfirm0(int x, int y);
void DrawConfirm1(int x, int y);

#endif
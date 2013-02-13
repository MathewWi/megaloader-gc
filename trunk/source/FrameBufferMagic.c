/* -----------------------------------------------------------
      FrameBufferMagic.c - Framebuffer routines with GX
	      - by emu_kidid & sepp256

      Version 1.0 11/11/2009
        - Initial Code
   ----------------------------------------------------------- */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gccore.h>
#include <ogc/exi.h>

#include "FrameBufferMagic.h"

#include "backdrop_tpl.h"
#include "backdrop.h"
#include "sdin_tpl.h"
#include "sdin.h"
#include "sdout_tpl.h"
#include "sdout.h"
#include "dvdin_tpl.h"
#include "dvdin.h"
#include "dvdout_tpl.h"
#include "dvdout.h"

#include "boxinner_tpl.h"
#include "boxinner.h"
#include "boxouter_tpl.h"
#include "boxouter.h"
#include "btna_tpl.h"
#include "btna.h"
#include "btnb_tpl.h"
#include "btnb.h"
#include "btnz_tpl.h"
#include "btnz.h"
#include "logoborder_tpl.h"
#include "logoborder.h"

//#include "logoatari_tpl.h"
//#include "logoatari.h"
#include "logocoleco_tpl.h"
#include "logocoleco.h"
#include "logogba_tpl.h"
#include "logogba.h"
#include "logogenesis_tpl.h"
#include "logogenesis.h"
#include "logon64_tpl.h"
#include "logon64.h"
#include "logones_tpl.h"
#include "logones.h"
#include "logongcd_tpl.h"
#include "logongcd.h"
#include "logongp_tpl.h"
#include "logongp.h"
#include "logopsx_tpl.h"
#include "logopsx.h"
#include "logosnes_tpl.h"
#include "logosnes.h"
#include "logotgx_tpl.h"
#include "logotgx.h"
#include "logovectrex_tpl.h"
#include "logovectrex.h"

#include "logoscummvm_tpl.h"
#include "logoscummvm.h"
#include "logochip8_tpl.h"
#include "logochip8.h"
#include "logopong_tpl.h"
#include "logopong.h"
#include "logoquake_tpl.h"
#include "logoquake.h"
#include "logofruit_tpl.h"
#include "logofruit.h"
#include "logotetris_tpl.h"
#include "logotetris.h"
#include "logoaf_tpl.h"
#include "logoaf.h"
#include "logobreak_tpl.h"
#include "logobreak.h"
#include "logosnow_tpl.h"
#include "logosnow.h"
#include "logometh_tpl.h"
#include "logometh.h"

#include "logogc_tpl.h"
#include "logogc.h"
#include "logoqbox_tpl.h"
#include "logoqbox.h"

#define GUI_MSGBOX_ALPHA 200
#define BannerSize (96*32*2)   // Banner is 96 cols * 32 lines in RGB5A3 fmt

GXTexObj bannerTexObj;
u8 *bannerData;

TPLFile sdinTPL;
GXTexObj sdinTexObj;
TPLFile sdoutTPL;
GXTexObj sdoutTexObj;
TPLFile dvdinTPL;
GXTexObj dvdinTexObj;
TPLFile dvdoutTPL;
GXTexObj dvdoutTexObj;

TPLFile backdropTPL;
GXTexObj backdropTexObj;
TPLFile boxinnerTPL;
GXTexObj boxinnerTexObj;
TPLFile boxouterTPL;
GXTexObj boxouterTexObj;
TPLFile btnaTPL;
GXTexObj btnaTexObj;
TPLFile btnbTPL;
GXTexObj btnbTexObj;
TPLFile btnzTPL;
GXTexObj btnzTexObj;
TPLFile logoborderTPL;
GXTexObj logoborderTexObj;

//TPLFile logoatariTPL;
//GXTexObj logoatariTexObj;
TPLFile logocolecoTPL;
GXTexObj logocolecoTexObj;
TPLFile logogbaTPL;
GXTexObj logogbaTexObj;
TPLFile logogenesisTPL;
GXTexObj logogenesisTexObj;
TPLFile logon64TPL;
GXTexObj logon64TexObj;
TPLFile logonesTPL;
GXTexObj logonesTexObj;
TPLFile logongcdTPL;
GXTexObj logongcdTexObj;
TPLFile logongpTPL;
GXTexObj logongpTexObj;
TPLFile logopsxTPL;
GXTexObj logopsxTexObj;
TPLFile logosnesTPL;
GXTexObj logosnesTexObj;
TPLFile logotgxTPL;
GXTexObj logotgxTexObj;
TPLFile logovectrexTPL;
GXTexObj logovectrexTexObj;

TPLFile logoscummvmTPL;
GXTexObj logoscummvmTexObj;
TPLFile logochip8TPL;
GXTexObj logochip8TexObj;
TPLFile logopongTPL;
GXTexObj logopongTexObj;
TPLFile logoquakeTPL;
GXTexObj logoquakeTexObj;
TPLFile logofruitTPL;
GXTexObj logofruitTexObj;
TPLFile logotetrisTPL;
GXTexObj logotetrisTexObj;
TPLFile logoafTPL;
GXTexObj logoafTexObj;
TPLFile logobreakTPL;
GXTexObj logobreakTexObj;
TPLFile logosnowTPL;
GXTexObj logosnowTexObj;
TPLFile logomethTPL;
GXTexObj logomethTexObj;

TPLFile logogcTPL;
GXTexObj logogcTexObj;
TPLFile logoqboxTPL;
GXTexObj logoqboxTexObj;


void init_textures() 
{
   bannerData = memalign(32,BannerSize);
   memset(bannerData,0,BannerSize);
   DCFlushRange(bannerData,BannerSize);
   GX_InitTexObj(&bannerTexObj,bannerData,96,32,GX_TF_RGB5A3,GX_CLAMP,GX_CLAMP,GX_FALSE);

   TPL_OpenTPLFromMemory(&sdinTPL, (void *)sdin_tpl, sdin_tpl_size);
   TPL_GetTexture(&sdinTPL,sdin,&sdinTexObj);
   TPL_OpenTPLFromMemory(&sdoutTPL, (void *)sdout_tpl, sdout_tpl_size);
   TPL_GetTexture(&sdoutTPL,sdout,&sdoutTexObj);
   TPL_OpenTPLFromMemory(&dvdinTPL, (void *)dvdin_tpl, dvdin_tpl_size);
   TPL_GetTexture(&dvdinTPL,dvdin,&dvdinTexObj);
   TPL_OpenTPLFromMemory(&dvdoutTPL, (void *)dvdout_tpl, dvdout_tpl_size);
   TPL_GetTexture(&dvdoutTPL,dvdout,&dvdoutTexObj);

   TPL_OpenTPLFromMemory(&backdropTPL, (void *)backdrop_tpl, backdrop_tpl_size);
   TPL_GetTexture(&backdropTPL,backdrop,&backdropTexObj);
   TPL_OpenTPLFromMemory(&boxinnerTPL, (void *)boxinner_tpl, boxinner_tpl_size);
   TPL_GetTexture(&boxinnerTPL,boxinner,&boxinnerTexObj);
   GX_InitTexObjWrapMode(&boxinnerTexObj, GX_CLAMP, GX_CLAMP);
   TPL_OpenTPLFromMemory(&boxouterTPL, (void *)boxouter_tpl, boxouter_tpl_size);
   TPL_GetTexture(&boxouterTPL,boxouter,&boxouterTexObj);
   GX_InitTexObjWrapMode(&boxouterTexObj, GX_CLAMP, GX_CLAMP);

   //buttons
   TPL_OpenTPLFromMemory(&btnaTPL, (void *)btna_tpl, btna_tpl_size);
   TPL_GetTexture(&btnaTPL,btna,&btnaTexObj);
   TPL_OpenTPLFromMemory(&btnbTPL, (void *)btnb_tpl, btnb_tpl_size);
   TPL_GetTexture(&btnbTPL,btnb,&btnbTexObj);
   TPL_OpenTPLFromMemory(&btnzTPL, (void *)btnz_tpl, btnz_tpl_size);
   TPL_GetTexture(&btnzTPL,btnz,&btnzTexObj);
   TPL_OpenTPLFromMemory(&logoborderTPL, (void *)logoborder_tpl, logoborder_tpl_size);
   TPL_GetTexture(&logoborderTPL,logoborder,&logoborderTexObj);	

   // system logos
// TPL_OpenTPLFromMemory(&logoatariTPL, (void *)logoatari_tpl, logoatari_tpl_size);
// TPL_GetTexture(&logoatariTPL,logoatari,&logoatariTexObj);
   TPL_OpenTPLFromMemory(&logocolecoTPL, (void *)logocoleco_tpl, logocoleco_tpl_size);
   TPL_GetTexture(&logocolecoTPL,logocoleco,&logocolecoTexObj);
   TPL_OpenTPLFromMemory(&logogbaTPL, (void *)logogba_tpl, logogba_tpl_size);
   TPL_GetTexture(&logogbaTPL,logogba,&logogbaTexObj);
   TPL_OpenTPLFromMemory(&logogenesisTPL, (void *)logogenesis_tpl, logogenesis_tpl_size);
   TPL_GetTexture(&logogenesisTPL,logogenesis,&logogenesisTexObj);
   TPL_OpenTPLFromMemory(&logon64TPL, (void *)logon64_tpl, logon64_tpl_size);
   TPL_GetTexture(&logon64TPL,logon64,&logon64TexObj);
   TPL_OpenTPLFromMemory(&logonesTPL, (void *)logones_tpl, logones_tpl_size);
   TPL_GetTexture(&logonesTPL,logones,&logonesTexObj);
   TPL_OpenTPLFromMemory(&logongcdTPL, (void *)logongcd_tpl, logongcd_tpl_size);
   TPL_GetTexture(&logongcdTPL,logongcd,&logongcdTexObj);
   TPL_OpenTPLFromMemory(&logongpTPL, (void *)logongp_tpl, logongp_tpl_size);
   TPL_GetTexture(&logongpTPL,logongp,&logongpTexObj);
   TPL_OpenTPLFromMemory(&logopsxTPL, (void *)logopsx_tpl, logopsx_tpl_size);
   TPL_GetTexture(&logopsxTPL,logopsx,&logopsxTexObj);
   TPL_OpenTPLFromMemory(&logosnesTPL, (void *)logosnes_tpl, logosnes_tpl_size);
   TPL_GetTexture(&logosnesTPL,logosnes,&logosnesTexObj);
   TPL_OpenTPLFromMemory(&logotgxTPL, (void *)logotgx_tpl, logotgx_tpl_size);
   TPL_GetTexture(&logotgxTPL,logotgx,&logotgxTexObj);
   TPL_OpenTPLFromMemory(&logovectrexTPL, (void *)logovectrex_tpl, logovectrex_tpl_size);
   TPL_GetTexture(&logovectrexTPL,logovectrex,&logovectrexTexObj);

   TPL_OpenTPLFromMemory(&logoscummvmTPL, (void *)logoscummvm_tpl, logoscummvm_tpl_size);
   TPL_GetTexture(&logoscummvmTPL,logoscummvm,&logoscummvmTexObj);
   TPL_OpenTPLFromMemory(&logochip8TPL, (void *)logochip8_tpl, logochip8_tpl_size);
   TPL_GetTexture(&logochip8TPL,logochip8,&logochip8TexObj);
   TPL_OpenTPLFromMemory(&logopongTPL, (void *)logopong_tpl, logopong_tpl_size);
   TPL_GetTexture(&logopongTPL,logopong,&logopongTexObj);
   TPL_OpenTPLFromMemory(&logoquakeTPL, (void *)logoquake_tpl, logoquake_tpl_size);
   TPL_GetTexture(&logoquakeTPL,logoquake,&logoquakeTexObj);
   TPL_OpenTPLFromMemory(&logofruitTPL, (void *)logofruit_tpl, logofruit_tpl_size);
   TPL_GetTexture(&logofruitTPL,logofruit,&logofruitTexObj);
   TPL_OpenTPLFromMemory(&logotetrisTPL, (void *)logotetris_tpl, logotetris_tpl_size);
   TPL_GetTexture(&logotetrisTPL,logotetris,&logotetrisTexObj);
   TPL_OpenTPLFromMemory(&logoafTPL, (void *)logoaf_tpl, logoaf_tpl_size);
   TPL_GetTexture(&logoafTPL,logoaf,&logoafTexObj);
   TPL_OpenTPLFromMemory(&logobreakTPL, (void *)logobreak_tpl, logobreak_tpl_size);
   TPL_GetTexture(&logobreakTPL,logobreak,&logobreakTexObj);
   TPL_OpenTPLFromMemory(&logosnowTPL, (void *)logosnow_tpl, logosnow_tpl_size);
   TPL_GetTexture(&logosnowTPL,logosnow,&logosnowTexObj);
   TPL_OpenTPLFromMemory(&logomethTPL, (void *)logometh_tpl, logometh_tpl_size);
   TPL_GetTexture(&logomethTPL,logometh,&logomethTexObj);

   TPL_OpenTPLFromMemory(&logogcTPL, (void *)logogc_tpl, logogc_tpl_size);
   TPL_GetTexture(&logogcTPL,logogc,&logogcTexObj);
   TPL_OpenTPLFromMemory(&logoqboxTPL, (void *)logoqbox_tpl, logoqbox_tpl_size);
   TPL_GetTexture(&logoqboxTPL,logoqbox,&logoqboxTexObj);
}

void drawInit()
{
   Mtx44 GXprojection2D;
   Mtx GXmodelView2D;

   // Reset various parameters from gfx plugin
   GX_SetCoPlanar(GX_DISABLE);
   GX_SetClipMode(GX_CLIP_ENABLE);
   GX_SetAlphaCompare(GX_ALWAYS,0,GX_AOP_AND,GX_ALWAYS,0);

   guMtxIdentity(GXmodelView2D);
   GX_LoadTexMtxImm(GXmodelView2D,GX_TEXMTX0,GX_MTX2x4);
   GX_LoadPosMtxImm(GXmodelView2D,GX_PNMTX0);
   guOrtho(GXprojection2D, 0, 479, 0, 639, 0, 700);
   GX_LoadProjectionMtx(GXprojection2D, GX_ORTHOGRAPHIC);

   GX_SetZMode(GX_DISABLE,GX_ALWAYS,GX_TRUE);

   GX_ClearVtxDesc();
   GX_SetVtxDesc(GX_VA_PTNMTXIDX, GX_PNMTX0);
   GX_SetVtxDesc(GX_VA_TEX0MTXIDX, GX_TEXMTX0);
   GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
   GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
   GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
   //set vertex attribute formats here
   GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
   GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
   GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
   GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

   //enable textures
   GX_SetNumChans (1);
   GX_SetNumTexGens (1);
   GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

   GX_SetNumTevStages (1);
   GX_SetTevOrder (GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
   GX_SetTevOp (GX_TEVSTAGE0, GX_PASSCLR);

   //set blend mode
   GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR); //Fix src alpha
   GX_SetColorUpdate(GX_ENABLE);

   //set cull mode
   GX_SetCullMode (GX_CULL_NONE);
}

void drawRect(int x, int y, int width, int height, int depth, GXColor color, float s0, float s1, float t0, float t1)
{
   GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
      GX_Position3f32((float) x,(float) y,(float) depth );
      GX_Color4u8(color.r, color.g, color.b, color.a);
      GX_TexCoord2f32(s0,t0);
      GX_Position3f32((float) (x+width),(float) y,(float) depth );
      GX_Color4u8(color.r, color.g, color.b, color.a);
      GX_TexCoord2f32(s1,t0);
      GX_Position3f32((float) (x+width),(float) (y+height),(float) depth );
      GX_Color4u8(color.r, color.g, color.b, color.a);
      GX_TexCoord2f32(s1,t1);
      GX_Position3f32((float) x,(float) (y+height),(float) depth );
      GX_Color4u8(color.r, color.g, color.b, color.a);
      GX_TexCoord2f32(s0,t1);
   GX_End();
}

void DrawSimpleBox(int x, int y, int width, int height, int depth, GXColor fillColor, GXColor borderColor) 
{
   //Adjust for blank texture border
   x-=4; y-=4; width+=8; height+=8;

   drawInit();
   GX_SetTevOp (GX_TEVSTAGE0, GX_MODULATE);
   GX_InvalidateTexAll();
   GX_LoadTexObj(&boxinnerTexObj, GX_TEXMAP0);

   drawRect(x, y, width/2, height/2, depth, fillColor, 0.0f, ((float)width/32), 0.0f, ((float)height/32));
   drawRect(x+(width/2), y, width/2, height/2, depth, fillColor, ((float)width/32), 0.0f, 0.0f, ((float)height/32));
   drawRect(x, y+(height/2), width/2, height/2, depth, fillColor, 0.0f, ((float)width/32), ((float)height/32), 0.0f);
   drawRect(x+(width/2), y+(height/2), width/2, height/2, depth, fillColor, ((float)width/32), 0.0f, ((float)height/32), 0.0f);

   GX_InvalidateTexAll();
   GX_LoadTexObj(&boxouterTexObj, GX_TEXMAP0);

   drawRect(x, y, width/2, height/2, depth, borderColor, 0.0f, ((float)width/32), 0.0f, ((float)height/32));
   drawRect(x+(width/2), y, width/2, height/2, depth, borderColor, ((float)width/32), 0.0f, 0.0f, ((float)height/32));
   drawRect(x, y+(height/2), width/2, height/2, depth, borderColor, 0.0f, ((float)width/32), ((float)height/32), 0.0f);
   drawRect(x+(width/2), y+(height/2), width/2, height/2, depth, borderColor, ((float)width/32), 0.0f, ((float)height/32), 0.0f);
}

void DrawImage(int textureId, int x, int y, int width, int height, int depth, float s1, float s2, float t1, float t2)
{
   drawInit();
   GX_SetTevOp (GX_TEVSTAGE0, GX_REPLACE);
   GX_InvalidateTexAll();

   switch(textureId)
   {
      case TEX_SDIN:
         GX_LoadTexObj(&sdinTexObj, GX_TEXMAP0);
         break;
      case TEX_SDOUT:
         GX_LoadTexObj(&sdoutTexObj, GX_TEXMAP0);
         break;

      case TEX_DVDIN:
         GX_LoadTexObj(&dvdinTexObj, GX_TEXMAP0);
         break;
      case TEX_DVDOUT:
         GX_LoadTexObj(&dvdoutTexObj, GX_TEXMAP0);
         break;

      case TEX_BACKDROP:
         GX_LoadTexObj(&backdropTexObj, GX_TEXMAP0);
         break;
      case TEX_BANNER:
         GX_LoadTexObj(&bannerTexObj, GX_TEXMAP0);
         break;
      case TEX_BTNA:
         GX_LoadTexObj(&btnaTexObj, GX_TEXMAP0);
         break;
      case TEX_BTNB:
         GX_LoadTexObj(&btnbTexObj, GX_TEXMAP0);
         break;
      case TEX_BTNZ:
         GX_LoadTexObj(&btnzTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOBORDER:
         GX_LoadTexObj(&logoborderTexObj, GX_TEXMAP0);
         break;

//    case TEX_LOGOATARI:
//       GX_LoadTexObj(&logoatariTexObj, GX_TEXMAP0);
//       break;
      case TEX_LOGOCOLECO:
         GX_LoadTexObj(&logocolecoTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOGBA:
         GX_LoadTexObj(&logogbaTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOGENESIS:
         GX_LoadTexObj(&logogenesisTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGON64:
         GX_LoadTexObj(&logon64TexObj, GX_TEXMAP0);
         break;
      case TEX_LOGONES:
         GX_LoadTexObj(&logonesTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGONGCD:
         GX_LoadTexObj(&logongcdTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGONGP:
         GX_LoadTexObj(&logongpTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOPSX:
         GX_LoadTexObj(&logopsxTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOSNES:
         GX_LoadTexObj(&logosnesTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOTGX:
         GX_LoadTexObj(&logotgxTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOVECTREX:
         GX_LoadTexObj(&logovectrexTexObj, GX_TEXMAP0);
         break;

      case TEX_LOGOSCUMMVM:
         GX_LoadTexObj(&logoscummvmTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOCHIP8:
         GX_LoadTexObj(&logochip8TexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOPONG:
         GX_LoadTexObj(&logopongTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOQUAKE:
         GX_LoadTexObj(&logoquakeTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOFRUIT:
         GX_LoadTexObj(&logofruitTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOTETRIS:
         GX_LoadTexObj(&logotetrisTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOAF:
         GX_LoadTexObj(&logoafTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOBREAK:
         GX_LoadTexObj(&logobreakTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOSNOW:
         GX_LoadTexObj(&logosnowTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOMETH:
         GX_LoadTexObj(&logomethTexObj, GX_TEXMAP0);
         break;

      case TEX_LOGOGC:
         GX_LoadTexObj(&logogcTexObj, GX_TEXMAP0);
         break;
      case TEX_LOGOQBOX:
         GX_LoadTexObj(&logoqboxTexObj, GX_TEXMAP0);
         break;
   }

   GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
      GX_Position3f32((float) x,(float) y,(float) depth );
      GX_Color4u8(255, 255, 255, 255);
      GX_TexCoord2f32(s1,t1);
      GX_Position3f32((float) (x+width),(float) y,(float) depth );
      GX_Color4u8(255, 255, 255, 255);
      GX_TexCoord2f32(s2,t1);
      GX_Position3f32((float) (x+width),(float) (y+height),(float) depth );
      GX_Color4u8(255, 255, 255, 255);
      GX_TexCoord2f32(s2,t2);
      GX_Position3f32((float) x,(float) (y+height),(float) depth );
      GX_Color4u8(255, 255, 255, 255);
      GX_TexCoord2f32(s1,t2);
   GX_End();
}

void _DrawBackdrop() 
{
   DrawImage(TEX_BACKDROP, 0, 0, 640, 480, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   char title[1];
   sprintf(title, "MegaLoader %s", _VERSION);
   WriteFont(30,40, title);
}

// Call this when starting a screen
void DrawFrameStart() {
   whichfb ^= 1;
   _DrawBackdrop();
}

// Call this at the end of a screen
void DrawFrameFinish() {
   //Copy EFB->XFB
   GX_SetCopyClear((GXColor){0, 0, 0, 0xFF}, GX_MAX_Z24);
   GX_CopyDisp(xfb[whichfb],GX_TRUE);
   GX_Flush();

   VIDEO_SetNextFramebuffer(xfb[whichfb]);
   VIDEO_Flush();
   VIDEO_WaitVSync();
   if(vmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}

void DrawEmptyBox(int x1, int y1, int x2, int y2, int color) 
{
   int borderSize;
   borderSize = (y2-y1) <= 30 ? 3 : 10;
   x1-=borderSize;x2+=borderSize;y1-=borderSize;y2+=borderSize;

   GXColor fillColor = (GXColor) {35,35,35,GUI_MSGBOX_ALPHA};      //Grey-ish
   GXColor borderColor = (GXColor) {200,200,200,GUI_MSGBOX_ALPHA}; //Silver

   DrawSimpleBox( x1, y1, x2-x1, y2-y1, 0, fillColor, borderColor);
}


void readme()
{
   DrawFrameStart();
   DrawEmptyBox(25,85, vmode->fbWidth-30, 395, COLOR_BLACK);

   char title[1];
   sprintf(title, "MegaLoader %s", _VERSION);
   WriteFont(25+(1.60*116)+10,100, title);
   WriteFont(110,125, "Graphical Interface for Gamecube");
   WriteFont(25+(1.53*116)+10,150, "by Megalomaniac");

   WriteFont(50,200, "Based in part on Swiss opensource software");
   WriteFont(50,225, "by emu_kidid @ WWW.GC-FOREVER.COM");

   WriteFont(50,275, "Concept originally developed as an easy to");
   WriteFont(50,300, "use method for children to load emulators");
   WriteFont(50,325, "and homebrew.");

   WriteFont(110,375, "Enjoy this Gamecube homebrew !!");

   DrawFrameFinish();
   sleep(20);
}



/*
DRAW CONFIRM - NOT USED
   DrawImage(TEX_LOGOATARI, 25+(2.50*116)-57, 147, LOGOATARI_WIDTH,LOGOATARI_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
   sprintf(EmuName, "atari"); }
   DrawImage(TEX_LOGOCOLECO, 25+(2.50*116)-57, 147, LOGOCOLECO_WIDTH,LOGOCOLECO_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
   sprintf(EmuName, "coleco"); }
   sprintf(EmuName, "Please make another selection");
   WriteFont(25+(1*116)+10,175, EmuName);}



FIXED MENU SELECTOR POSITIONS SHOWN BELOW:
   ROW 1
   DrawImage(TEX_LOGOBORDER, 25+(0*116),    85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   ROW 2
   DrawImage(TEX_LOGOBORDER, 25+(0*116),    185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   ROW 3
   DrawImage(TEX_LOGOBORDER, 25+(0*116),    285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
   DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);


NOTE: SEARCH FOR THE FOLLOWING EMULATOR TITLES:
----------------------------------------------
Not Available Yet:
   Atari            = :(
   Intellivision    = :(

Main Window:
----------------------------------------------
   NES              = fceugx-gc.dol
   SNES             = snes9xgx-gc.dol
   N64              = cube64.dol
   GBA              = vbagx_gc.dol

   Genesis/SMS      = genplus_cube.dol
   TGX              = hugo_cube.dol
   NeoGeo CD        = neocdredux.dol
   NeoGeo Portable  = neopopgc.dol

   PSX              = cubeSX.dol
   Vectrex          = vecxgc.dol
   ColecoVision     = collisionGC.dol
   GAMECUBE         = GCM

Second Window:
----------------------------------------------
   ScummVM          = scummvm-gc.dol
   Chip8            = chip8.dol
   Pong             = pong.dol
   Quake            = quake.dol

   Fruit Remover    = fruitremover.dol
   Tetris           = tetris.dol
   Ascii Fighter    = af.dol
   Breakout         = breakout.dol

   Snowlords        = snowlords.dol
   SuperMethaneBros = methane.dol
   MarioBox         = Extra Info
*/
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

#include "dvd.h"
#include "FrameBufferMagic.h"
#include "IPLFontWrite.h"
#include "main.h"
#include "wkf.h"

#include "images/buttons/btns.h"
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

#include "btnnohilight_tpl.h"
#include "btnnohilight.h"
#include "btnhilight_tpl.h"
#include "btnhilight.h"
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
//#include "logocoleco_tpl.h"
//#include "logocoleco.h"

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


int easteregg = 0;
#define GUI_MSGBOX_ALPHA 200

// Banner is 96 cols * 32 lines in RGB5A3 fmt
#define BannerSize (96*32*2)
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
TPLFile btnnohilightTPL;
GXTexObj btnnohilightTexObj;
TPLFile btnhilightTPL;
GXTexObj btnhilightTexObj;

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
//TPLFile logocolecoTPL;
//GXTexObj logocolecoTexObj;

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
	TPL_OpenTPLFromMemory(&btnnohilightTPL, (void *)btnnohilight_tpl, btnnohilight_tpl_size);
	TPL_GetTexture(&btnnohilightTPL,btnnohilight,&btnnohilightTexObj);
	TPL_OpenTPLFromMemory(&btnhilightTPL, (void *)btnhilight_tpl, btnhilight_tpl_size);
	TPL_GetTexture(&btnhilightTPL,btnhilight,&btnhilightTexObj);

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
//	TPL_OpenTPLFromMemory(&logoatariTPL, (void *)logoatari_tpl, logoatari_tpl_size);
//	TPL_GetTexture(&logoatariTPL,logoatari,&logoatariTexObj);
//	TPL_OpenTPLFromMemory(&logocolecoTPL, (void *)logocoleco_tpl, logocoleco_tpl_size);
//	TPL_GetTexture(&logocolecoTPL,logocoleco,&logocolecoTexObj);
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
	case TEX_BTNNOHILIGHT:
		GX_LoadTexObj(&btnnohilightTexObj, GX_TEXMAP0);
		break;
	case TEX_BTNHILIGHT:
		GX_LoadTexObj(&btnhilightTexObj, GX_TEXMAP0);
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

//	case TEX_LOGOATARI:
//		GX_LoadTexObj(&logoatariTexObj, GX_TEXMAP0);
//		break;
//	case TEX_LOGOCOLECO:
//		GX_LoadTexObj(&logocolecoTexObj, GX_TEXMAP0);
//		break;
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
	WriteFont(30,40, "MegaLoader");
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

void DrawMessageBox(int type, char *message) 
{
	int x1 = ((640/2) - (PROGRESS_BOX_WIDTH/2));
	int x2 = ((640/2) + (PROGRESS_BOX_WIDTH/2));
	int y1 = ((480/2) - (PROGRESS_BOX_HEIGHT/2));
	int y2 = ((480/2) + (PROGRESS_BOX_HEIGHT/2));
	int middleY = y2-y1 < 23 ? y1+3 : (y2+y1)/2-12;
	
  	GXColor fillColor = (GXColor) {0,0,0,GUI_MSGBOX_ALPHA}; //black
	GXColor borderColor = (GXColor) {200,200,200,GUI_MSGBOX_ALPHA}; //silver
	
	DrawSimpleBox( x1, y1, x2-x1, y2-y1, 0, fillColor, borderColor); 

	char *tok = strtok(message,"\n");
	while(tok != NULL) {
		WriteFontStyled(640/2, middleY, tok, 1.0f, true, defaultColor);
		tok = strtok(NULL,"\n");
		middleY+=24;
	}
}

void DrawRawFont(int x, int y, char *message) {
  WriteFont(x, y, message);
}

void DrawSelectableButton(int x1, int y1, int x2, int y2, char *message, int mode, u32 color) 
{
	int middleY, borderSize;
	color = (color == -1) ? BUTTON_COLOUR_INNER : color; //never used

	borderSize = (mode==B_SELECTED) ? 6 : 4;
	middleY = (((y2-y1)/2)-12)+y1;

	//determine length of the text ourselves if x2 == -1
	x1 = (x2 == -1) ? x1+2:x1;
	x2 = (x2 == -1) ? GetTextSizeInPixels(message)+x1+(borderSize*2)+6 : x2;

	if(middleY+24 > y2) {
		middleY = y1+3;
	}

	GXColor selectColor = (GXColor) {96,107,164,GUI_MSGBOX_ALPHA}; //bluish
	GXColor noColor = (GXColor) {0,0,0,0}; //black
	GXColor borderColor = (GXColor) {200,200,200,GUI_MSGBOX_ALPHA}; //silver
	
	//Draw Text and backfill (if selected)
	if(mode==B_SELECTED) {
		DrawSimpleBox( x1, y1, x2-x1, y2-y1, 0, selectColor, borderColor);
		WriteFontStyled(x1 + borderSize+3, middleY, message, 1.0f, false, defaultColor);
	}
	else {
		DrawSimpleBox( x1, y1, x2-x1, y2-y1, 0, noColor, borderColor);
		WriteFontStyled(x1 + borderSize+3, middleY, message, 1.0f, false, defaultColor);
	}
}

void DrawEmptyBox(int x1, int y1, int x2, int y2, int color) 
{
	int borderSize;
	borderSize = (y2-y1) <= 30 ? 3 : 10;
	x1-=borderSize;x2+=borderSize;y1-=borderSize;y2+=borderSize;

	GXColor fillColor = (GXColor) {35,35,35,GUI_MSGBOX_ALPHA}; //Grey-ish
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



//###############################################################################

void DrawMenuSelector(x,y)
{
	// DRAW SELECTION BOX BASED ON X/Y POSITION
	DrawImage(TEX_LOGOBORDER, x, y, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);

/*	FIXED MENU SELECTOR POSITIONS SHOWN BELOW:
	DrawImage(TEX_LOGOBORDER, 25+(0*116),    85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 85, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);

	DrawImage(TEX_LOGOBORDER, 25+(0*116),    185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 185, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);

	DrawImage(TEX_LOGOBORDER, 25+(0*116),    285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(1.25*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(2.50*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_LOGOBORDER, 25+(3.75*116), 285, LOGOBORDER_WIDTH,LOGOBORDER_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
*/
}


void DrawMenuLogos0() 
{
	//DRAW ICONS
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
	DrawImage(TEX_LOGOGC,	40+(2.50*116), 300, LOGOGC_WIDTH, LOGOGC_HEIGHT,    0, 0.0f, 1.0f, 0.0f, 1.0f);
//	DrawImage(TEX_Z, 40+(3.75*116), 300, Z_WIDTH, Z_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
}


void DrawMenuLogos1() 
{
	//DRAW ICONS
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
//	DrawImage(TEX_Y, 40+(2.50*116), 300, Y_WIDTH, Y_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	if (easteregg == 1) 
		DrawImage(TEX_LOGOQBOX, 40+(3.75*116), 300, LOGOQBOX_WIDTH,LOGOQBOX_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
}


// MAIN MENU BUTTONS
void DrawMenuButtons() 
{

	// DRAW BUTTONS
	DrawImage(TEX_BTNA, 40+(0*116), 420, BTNA_WIDTH,BTNA_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	DrawImage(TEX_BTNZ, 40+(4*116), 420, BTNZ_WIDTH,BTNZ_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);

	WriteFont(110,430, "SELECT");
	WriteFont(400,430, "SWISS");	
}


/*
NOTE: SEARCH FOR THE FOLLOWING EMULATOR TITLES:
----------------------------------------------
NA:
	Atari			= :(
	ColecoVision	= :(
	Intellivision	= :(

Main Window:
----------------------------------------------
	NES		= fceugx-gc.dol
	SNES		= snes9xgx-gc.dol
	N64		= cube64.dol	
	GBA		= vbagx_gc.dol

	Genesis/SMS	= genplus_cube.dol
	TGX		= hugo_cube.dol
	NeoGeo CD	= neocdredux.dol
	NeoGeo Portable = neopopgc.dol

	PSX		= cubeSX.dol
	Vectrex		= vecxgc.dol
	GAMECUBE	= GCM




Second Window:	
----------------------------------------------
	ScummVM		= scummvm-gc.dol
	Chip8		= chip8.dol
	Pong		= pong.dol
	Quake		= quake.dol

	fruit remover	= fruitremover.dol
	tetris		= tetris.dol
	Ascii Fighter	= af.dol
	Breakout	= breakout.dol

	Snowlords	= snowlords.dol
	SuperMethaneBros= methane.dol
	MarioBox	= Extra Info
*/



void DrawConfirm0(x,y)
{

	char EmuName[10];

	// REFRESH BACKDROP TO HIDE MAIN MENU BUTTONS
	DrawFrameStart();
	DrawMenuLogos0(); 
	DrawMenuSelector(x,y);

//			NOT USED	
//			DrawImage(TEX_LOGOATARI, 25+(2.50*116)-57, 147, LOGOATARI_WIDTH,LOGOATARI_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
//			sprintf(EmuName, "atari"); }
//			DrawImage(TEX_LOGOCOLECO, 25+(2.50*116)-57, 147, LOGOCOLECO_WIDTH,LOGOCOLECO_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f); 
//			sprintf(EmuName, "coleco"); }
//			sprintf(EmuName, "Please make another selection");
//			WriteFont(25+(1*116)+10,175, EmuName);}
	
	
	// DRAW CONFIRMATION BOX 
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
	else if (x == 315 && y == 285) { GCM(); return; }
	else if (x == 460 && y == 285) { return; }
	else { 	WriteFont(25+(2.25*116)+10,175, "unknown error"); return;}

	
	DrawImage(TEX_BTNB, 25+(1.25*116)-50, 315, BTNB_WIDTH,BTNB_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	WriteFont(25+(1.25*116)+10,325, "CANCEL");
	DrawImage(TEX_BTNA, 25+(3.75*116), 315, BTNA_WIDTH,BTNA_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	WriteFont(25+(3.75*116)-75,325, "LOAD");
	
//	WriteFont(275,40, "Confirm");
	DrawFrameFinish();

	while(1){
		//CONFIRM A = EXECUTE DOL	
		while (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
			RunDOL(EmuName);
			return;								//
		}
		// CANCEL B = EXIT CONFIRMATION MENU
		while (PAD_ButtonsDown(0) & PAD_BUTTON_B) { // do nothing, just return
			return;
		}

	}

}

void DrawConfirm1(x,y)
{

	char EmuName[10];

	// REFRESH BACKDROP TO HIDE MAIN MENU BUTTONS
	DrawFrameStart();
	DrawMenuLogos1(); 
	DrawMenuSelector(x,y);

	
	// DRAW CONFIRMATION BOX 
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
				DrawMenuLogos1();
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
	
	
	
	
	
	else { 	WriteFont(25+(2.25*116)+10,175, "unknown error"); return;}

	
	DrawImage(TEX_BTNB, 25+(1.25*116)-50, 315, BTNB_WIDTH,BTNB_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	WriteFont(25+(1.25*116)+10,325, "CANCEL");
	DrawImage(TEX_BTNA, 25+(3.75*116), 315, BTNA_WIDTH,BTNA_HEIGHT, 0, 0.0f, 1.0f, 0.0f, 1.0f);
	WriteFont(25+(3.75*116)-75,325, "LOAD");
	
//	WriteFont(275,40, "Confirm");
	DrawFrameFinish();

	while(1){
		//CONFIRM A = EXECUTE DOL	
		while (PAD_ButtonsDown(0) & PAD_BUTTON_A) {
			RunDOL(EmuName);
			return;								//
		}
		// CANCEL B = EXIT CONFIRMATION MENU
		while (PAD_ButtonsDown(0) & PAD_BUTTON_B) { // do nothing, just return
			return;
		}

	}

}
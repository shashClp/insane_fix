#include <windows.h>
#include <log.h>
#include <misc.h>
#include <mem.h>
#include <guio.h>
#include <efx2d.h>
#include "trash.h"
#include "rt.h"
#include "def.h"
#include "blurs.h"

//****************************************************************************
//* SPRAYS EN EL AGUA                                                        *
//****************************************************************************

#define MAX_INTRO_PALABRAS 6

float afTimeIn[MAX_INTRO_PALABRAS] = {1000,3000,5000,7000,9000,11000};
float afTimeOut[MAX_INTRO_PALABRAS] = {3000,5000,7000,9000,11000,14000};
int   aiPalabrasPos[MAX_INTRO_PALABRAS] = {90,70,100,90,70,30};
char  apPalabrasText[MAX_INTRO_PALABRAS][16] = {
                                                "no water",
                                                "no wind",
	                                              "no sand",
	                                              "no fire",
                                                "just...",
                                                "some sunshine" };

extern SFuente* fntGBL_TypeWriter;

DWORD *img32SPR_Telon;
DWORD *img32SPR_Tmp;
char *img8SPR_Alpha;
char *img8SPR_AlphaB;
TImage *img8SPR_Scroll;
TImage *img32SPR_Scroll;

//////////////////////////////////////////////////////////////////////////////
//
//  1 parte Sprays:
//  -fondo sprays
//  -plasma de campanas deformando el fondo
//  -La distorsion va desapareciendo

void SCENA_PLASMAIN(float iTick)
{
  float iCnt = 240.0f - iTick*240.0f;

  CalculaTablaSenos(iCnt);
  CreaCampanaPlasmaCirc();
  MuevePlasmaCirc();
  RenderPlasmaCircAlpha(img8SPR_Alpha);

	long iFact = (int)(255.0f - iTick*255.0f);

	if(iFact<0) 
    DrawPicOffset(GBL_Bits,img8SPR_Alpha,img32SPR_Telon);
	else {
		DrawPicOffset(img32SPR_Tmp,img8SPR_Alpha,img32SPR_Telon);
		MorphARGB(GBL_Bits,GBL_Bits,img32SPR_Tmp,iFact);
	}
	Margen320x240();
}

void SCENA_PLASMAINSTOP(float iTick)
{
  DrawPicOffset(GBL_Bits,img8SPR_Alpha,img32SPR_Telon);
	Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//  2 parte Sprays:
//  -fondo sprays
//  -doble scroll 'insane' con dist horiz [A + A(azul)]
//  -palabras con dist horiz por debajo [A]
//  -Dist horiz del rsultado [RGB]

void SCENA_SCROLLLEFT(float tick)
{
  int iTick=tick*24000;
  
  long iScrollPos = fmod(iTick/20,1344);
  long iFactor = ((iTick/30) % 15) -7;  	

  memset(img8SPR_Alpha,0,GBL_Size);
  long iHDist = rand()%10;

  ScrollToScreen8(img8SPR_Alpha,img8SPR_Scroll,iScrollPos + iHDist,5);
  HDist8(img8SPR_Alpha,0,201,iFactor);
  FlatternAlphaBlue(GBL_Bits,img32SPR_Telon,img8SPR_Alpha);        
  ScrollToScreen8(img8SPR_Alpha,img8SPR_Scroll,iScrollPos - iHDist,5);    
  HDist8(img8SPR_Alpha,0,201,10);
  for(int i=0;i<MAX_INTRO_PALABRAS;i++)
      if(2*afTimeIn[i]<iTick && 2*afTimeOut[i]>iTick)
          FONT_Print(aiPalabrasPos[i],185,apPalabrasText[i],fntGBL_TypeWriter,img8SPR_Alpha,16);
  long iHDistT = 1+(float)(1.5*((((iTick/2)-1000) % 2000) / 20));
  if(iHDistT>75) iHDistT = 75 + 1 - (iHDistT - 75);

  HDist8Des(img8SPR_Alpha,185,215,iHDistT);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8SPR_Alpha);

  HDist32((DWORD*)GBL_Bits,20,220,iHDistT);
}

//////////////////////////////////////////////////////////////////////////////
//
//  3 parte Sprays:
//  -fondo sprays (scroll hacia rriba (sol) [RGB]
//  -doble scroll 'insane' con dist horiz [A + A(azul)] (terminando)
//  -palabras con dist horiz por debajo [A] (terminando)
//  -Dist Onda resultado progresivo (inc)  [RGB]

void SCENA_SCROLLUP(float tick)
{
  int iTick=tick*10000;

  long iScrollPos = ((iTick+TIME_FIN_SCENA_SCROLLLEFT)/20);
	if(iScrollPos>=1344) iScrollPos = 0;
  long iFactor = (((iTick+TIME_FIN_SCENA_SCROLLLEFT)/30) % 15) -7;  	

  memset(img8SPR_Alpha,0,GBL_Size);              

  long iHDist = rand()%10;
	long iVPos = 180 - (iTick/30);
	if(iVPos<0)	iVPos = 0;

	ScrollToScreen32(img32SPR_Telon,img32SPR_Scroll,0,20,iVPos);
	if(iScrollPos<img8SPR_Scroll->width)
	ScrollToScreen8(img8SPR_Alpha,img8SPR_Scroll,iScrollPos + iHDist,5);        
  HDist8(img8SPR_Alpha,0,184,iFactor);
  FlatternAlphaBlue(GBL_Bits,img32SPR_Telon,img8SPR_Alpha);
  if(iScrollPos<img8SPR_Scroll->width)
	ScrollToScreen8(img8SPR_Alpha,img8SPR_Scroll,iScrollPos - iHDist,5);    
  HDist8(img8SPR_Alpha,0,184,10);
  for(int i=0;i<MAX_INTRO_PALABRAS;i++)
      if(2*afTimeIn[i]<(iTick + TIME_FIN_SCENA_SCROLLLEFT) && 2*afTimeOut[i]>(iTick + TIME_FIN_SCENA_SCROLLLEFT))
          FONT_Print(aiPalabrasPos[i],185,apPalabrasText[i],fntGBL_TypeWriter,img8SPR_Alpha,16);
  long iHDistT = (iTick/50)-70;
	if(iHDistT> -1)	iHDistT = -1;
  
  HDist8(img8SPR_Alpha,185,215,iHDistT);
  FlatternAlpha(img32SPR_Telon,GBL_Bits,img8SPR_Alpha);

  CalculaTablaSenos((180-iVPos)/7.0f);
  DrawWave(img8SPR_Alpha,GBL_Width>>1,-iVPos+150,iTick/10);
  DrawPic(GBL_Bits,img8SPR_Alpha,img32SPR_Telon);
  Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//  4 parte Sprays:
//  -Zoom hacia el sol

void SCENA_ActionInit(float tick)
{
  BLURS_GenerarTunnelWave(160.0f,120.0f,80.0f);
  ScrollToScreen32(img32SPR_Telon,img32SPR_Scroll,0,20,0);
}

void SCENA_SCROLLOUT(float iTick)
{
  BLURS_Render(GBL_Bits);
	Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool SPRAYS_Init()
{
  SEC_START ("Initializing Sprays");

  img32SPR_Telon = (DWORD *) GetMem (GBL_Size4);
  if (!img32SPR_Telon) return false;
  img32SPR_Tmp = (DWORD *) GetMem (GBL_Size4);
  if (!img32SPR_Tmp) return false;

  img8SPR_Alpha = (char *) GetMem (GBL_Size);  
  if (!img8SPR_Alpha) return false;
  img8SPR_AlphaB = (char *) GetMem (GBL_Size);    
  if (!img8SPR_AlphaB) return false;

  img8SPR_Scroll = IMAGE_Load("data\\logscrll.gif",8);  
  if (!img8SPR_Scroll) return false;
  img32SPR_Scroll = IMAGE_Load("data\\spray2.tga",32);
  if (!img32SPR_Scroll) return false;  

  ScrollToScreen32(img32SPR_Telon,img32SPR_Scroll,0,20,180);
	Margen320x240(img32SPR_Telon); 

  for(int i=0;i<400;i++) 
    MuevePlasmaCirc();

  LOG ("Sprays initialized");
  SEC_END();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void SPRAYS_Close()
{
  LOG ("Closing Sprays");

  if (img32SPR_Telon)
  {
    FreeMem (img32SPR_Telon);
    img32SPR_Telon = NULL;
  }
  if (img32SPR_Tmp)
  {
    FreeMem (img32SPR_Tmp);
    img32SPR_Tmp = NULL;
  }
  if (img32SPR_Scroll)
  {
    FreeMem (img32SPR_Scroll);
    img32SPR_Scroll = NULL;
  }
  if (img8SPR_AlphaB)
  {
    FreeMem (img8SPR_AlphaB);
    img8SPR_AlphaB = NULL;
  }
  if (img8SPR_Alpha)
  {
    FreeMem (img8SPR_Alpha);
    img8SPR_Alpha = NULL;
  }
  if (img8SPR_Scroll)
  {
    FreeMem (img8SPR_Scroll);
    img8SPR_Scroll = NULL;
  }
  LOG ("Sprays closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void SPRAYS_Guio()
{
  GUIO_AddFN (SCENA_PLASMAIN,"SCENA_PLASMAIN");
  GUIO_AddFN (SCENA_PLASMAINSTOP,"SCENA_PLASMAINSTOP");
  GUIO_AddFN (SCENA_SCROLLLEFT,"SCENA_SCROLLLEFT");
  GUIO_AddFN (SCENA_SCROLLUP,"SCENA_SCROLLUP");
  GUIO_AddFN (SCENA_SCROLLOUT,"SCENA_SCROLLOUT");
  GUIO_AddFN (SCENA_ActionInit,"SCENA_ActionInit");
}
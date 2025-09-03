#include <windows.h>
#include <image.h>
#include <mem.h>
#include <misc.h>
#include <log.h>
#include <efx2d.h>
#include <defines.h>
#include <guio.h>
#include <halo_light.h>
#include "def.h"
#include "blurs.h"

//****************************************************************************
//* LOGO INICIAL
//****************************************************************************

TImage *LOGOINI_Sprite1;
TImage *LOGOINI_Sprite2;
TImage *LOGOINI_Sprite3;
float LOGOINI_growing;
TImage *img32LOGOINI_Telon;
TImage *img32LOGOINI_Logo;
TImage *img8LOGOINI_Logo;
char *LOGOINI_Alpha;
DWORD *LOGOINI_Buffer;

//////////////////////////////////////////////////////////////////////////////
//
//  Logo In
//  -Fondo logo Anaconda

void LOGOINI_Action1(float tick)
{
  InitClipTabEspejo(20,220);
}

void LOGOINI_SpritesIN(float tick)
{  
float x,y;

  x = 200.0f+80.0f*sin (tick*M_PI);
  y = 140.0f+40.0f*cos (tick*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite1->bits,x,y,tick*32);
  x = 200.0f+80.0f*sin (tick*M_1Div3*2*M_PI);
  y = 140.0f+40.0f*cos (tick*M_1Div3*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite2->bits,x,y,tick*32);
  x = 200.0f+80.0f*sin (tick*M_2Div3*2*M_PI);
  y = 140.0f+40.0f*cos (tick*M_2Div3*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite3->bits,x,y,tick*32);
}

void LOGOINI_Sprites(float tick)
{
float x,y;

  tick+=1;
  x = 200.0f+80.0f*sin (tick*M_PI);
  y = 140.0f+40.0f*cos (tick*2*M_PI);
  PutPartRGB (LOGOINI_Buffer,(DWORD *)LOGOINI_Sprite1->bits,x,y,32+LOGOINI_growing);
  x = 200.0f+80.0f*sin (tick*M_1Div3*2*M_PI);
  y = 140.0f+40.0f*cos (tick*M_1Div3*2*M_PI);
  PutPartRGB (LOGOINI_Buffer,(DWORD *)LOGOINI_Sprite2->bits,x,y,32+LOGOINI_growing);
  x = 200.0f+80.0f*sin (tick*M_2Div3*2*M_PI);
  y = 140.0f+40.0f*cos (tick*M_2Div3*2*M_PI);
  PutPartRGB (LOGOINI_Buffer,(DWORD *)LOGOINI_Sprite3->bits,x,y,32+LOGOINI_growing);
}

void LOGOINI_SpritesOUT(float tick)
{
float x,y;

  float t1 = tick;
  float t = 1+t1;
  tick+=2;
  x = 200.0f*t+80.0f*sin (tick*M_PI);
  y = 140.0f*t+40.0f*cos (tick*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite1->bits,x,y,64*t1+32+LOGOINI_growing);
  x = 200.0f*t+80.0f*sin (tick*M_1Div3*2*M_PI);
  y = 140.0f*t+40.0f*cos (tick*M_1Div3*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite2->bits,x,y,64*t1+32+LOGOINI_growing);
  x = 200.0f*t+80.0f*sin (tick*M_2Div3*2*M_PI);
  y = 140.0f*t+40.0f*cos (tick*M_2Div3*2*M_PI);
  PutPartRGB (GBL_Bits,(DWORD *)LOGOINI_Sprite3->bits,x,y,64*t1+32+LOGOINI_growing);
}

void LOGOINI_SpritesGrow(float tick)
{	
  LOGOINI_growing = 64.0f*sin(tick*M_PI);
}

void LOGOINI_IN(float tick)
{
  float iCnt = 92.0f - (92.0f*tick);
  if(iCnt<0) iCnt=0;
  CalculaTablaSenos(iCnt);
  DrawWave(LOGOINI_Alpha,GBL_Width>>1,1700,(long)(tick*400.0f));
	DrawPic(LOGOINI_Buffer,LOGOINI_Alpha,(DWORD *)img32LOGOINI_Telon->bits);
  MotionBlur32(GBL_Bits,LOGOINI_Buffer);  
  LOGOINI_SpritesIN(tick);
	Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//  Logo Luz
//  -Fondo logo Anaconda
//  -RadialBlur de izq a der

void LOGOINI_Action2(float iTick)
{
}

void LOGOINI_LUZ(float tick)
{
  MotionBlur32(LOGOINI_Buffer,(DWORD *)img32LOGOINI_Logo->bits);
	long iPos = (long)(tick*320.0f);
	LOGOINI_Sprites(tick);  
  HALO_LIGHT_Alpha = (BYTE) (200.0f*sin(tick*M_PI));
  HALO_LIGHT_Render320x240 (iPos,120,GBL_Bits,LOGOINI_Buffer);  
	Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//  Logo Out
//  -Fondo logo Anaconda
//  -La distorsion (Dist Horizontal) aparece

void LOGOINI_OUT(float iTick)
{
  int iCnt = 100-(int) (sin(iTick*M_PI)*80.0f);
  if(iCnt<1) iCnt=1;
  memcpy(LOGOINI_Alpha,img8LOGOINI_Logo->bits,GBL_Size);
//	HDist8(LOGOINI_Alpha,10,190,iCnt);
  MISC_MemSet4(img32LOGOINI_Telon->bits,0,GBL_Size);
  FlatternAlpha(GBL_Bits,img32LOGOINI_Telon->bits,LOGOINI_Alpha);
  LOGOINI_SpritesOUT(iTick);  
}

//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool LOGOINI_Init()
{
  SEC_START ("Initializing LOGOINI");

  img8LOGOINI_Logo = IMAGE_Load("data\\ana_logo.gif",8);  
  if (!img8LOGOINI_Logo) return false;

  img32LOGOINI_Logo = IMAGE_Load("data\\ana_logo.gif",32);  
  if (!img32LOGOINI_Logo) return false;

  img32LOGOINI_Telon = IMAGE_Load("data\\ana_logo.gif",32);
	if (!img32LOGOINI_Telon) return false;

  LOGOINI_Alpha = (char *)GetMem (GBL_Size);
  if (!LOGOINI_Alpha) return false;

  LOGOINI_Buffer = (DWORD *)GetMem (GBL_Size4);
  if (!LOGOINI_Buffer) return false;

  LOGOINI_Sprite1 = IMAGE_Load ("data\\flares1.tga",32);
  if (!LOGOINI_Sprite1) return false;

  LOGOINI_Sprite2 = IMAGE_Load ("data\\flares2.tga",32);
  if (!LOGOINI_Sprite2) return false;

  LOGOINI_Sprite3 = IMAGE_Load ("data\\flares3.tga",32);
  if (!LOGOINI_Sprite3) return false;

  LOGOINI_growing = 0;

  LOG ("LOGOINI initialized!");
  SEC_END();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void LOGOINI_Close()
{
  LOG ("Closing LOGOINI");

  if (img8LOGOINI_Logo)
  {
    FreeMem (img8LOGOINI_Logo);
    img8LOGOINI_Logo = NULL;
  }
  if (img32LOGOINI_Logo)
  {
    FreeMem (img32LOGOINI_Logo);
    img32LOGOINI_Logo = NULL;
  }  
  if (img32LOGOINI_Telon)
  {
    FreeMem (img32LOGOINI_Telon);
    img32LOGOINI_Telon = NULL;
  }
  if (LOGOINI_Buffer)
  {
    FreeMem (LOGOINI_Buffer);
    LOGOINI_Buffer = NULL;
  }
  if (LOGOINI_Alpha)
  {
    FreeMem (LOGOINI_Alpha);
    LOGOINI_Alpha = NULL;
  }
  LOG ("LOGOINI closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//  For Guio

void LOGOINI_Guio()
{
  GUIO_AddFN(LOGOINI_IN,"LOGOINI_IN");
  GUIO_AddFN(LOGOINI_LUZ,"LOGOINI_LUZ");
  GUIO_AddFN(LOGOINI_OUT,"LOGOINI_OUT");
  GUIO_AddFN(LOGOINI_Action1,"LOGOINI_Action1");
  GUIO_AddFN(LOGOINI_Action2,"LOGOINI_Action2"); 
  GUIO_AddFN(LOGOINI_SpritesGrow,"LOGOINI_SpritesGrow");
}
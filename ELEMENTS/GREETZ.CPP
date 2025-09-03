#include <guio.h>
#include <log.h>
#include <image.h>
#include <mem.h>
#include <defines.h>
#include <view.h>
#include <efx2d.h>
#include "blurs.h"
#include "rt.h"

TImage *img32GREETZbk;
TImage *img8GREETZ_Alpha;

const long MAX_GREETZ_EVT = 12;

typedef struct {
	long x,y;
	float ti,to;
	char* texto;
} STextEvent;

STextEvent GREETZ_Events[MAX_GREETZ_EVT] =
{
	20,40,1,3,"MoveData",
	120,90,2,4,"InCogNita",
	20,140,3,5,"V-TeAm",

	120,40,4,6,"dSk!",
	20,90,5,7,"SaVagE",
	120,140,6,8,"TsT",

	20,40,7,9,"RgB",
	120,90,8,10,"¡GenEsiS!",
	20,140,9,11,"CEntOsis",

	120,40,10,12,"fuZZion",
	20,90,11,13,"wIldBiTs",
	120,140,12,14,"WaVeMasTers"

};

extern TImage* img8GBL_Ruido;
extern SFuente* fntGBL_TypeWriter;
DWORD GREETZ_IniTick = -1;

//////////////////////////////////////////////////////////////////////////////
//
//	Greetz, pos eso...

void GREETZ_Action1(float iTick)
{
	BLURS_GenerarTunnelRadialBlurInv(160,120,1.5);
}

void GREETZ_Play(float Tick)
{
	if(GREETZ_IniTick == -1)
		GREETZ_IniTick = GetTickCount();
	DWORD iTick = (float)GetTickCount() - GREETZ_IniTick;
	//Fondo
	OldFreeEsfera((float)iTick/5000.0,M_PIDiv2 + (float)iTick/3500.0,(float)iTick/2000.0);
	VuelcaBufInterp8x8F(GBL_Bits+320*20,(DWORD*)img32GREETZbk->bits);
	BLURS_RenderRadialInv(GBL_Bits,160,120,10);
	//Ruido
    long iFactor = (rand() % 35);
    ScrollToScreen8((char*)img8GREETZ_Alpha->bits,img8GBL_Ruido,iFactor,0);
	//Texto
	for(long i=0;i<MAX_GREETZ_EVT;i++)
	{
	  if((GREETZ_Events[i].ti <= Tick*14) && (GREETZ_Events[i].to >= Tick*14))			
	    FONT_Print(GREETZ_Events[i].x + rand()%2,GREETZ_Events[i].y + rand()%5,
									GREETZ_Events[i].texto,fntGBL_TypeWriter,(char*)img8GREETZ_Alpha->bits,16);

	  if((GREETZ_Events[i].ti <= Tick*14.5) && (GREETZ_Events[i].to >= Tick*14.5))			
	    FONT_Print(GREETZ_Events[i].x + rand()%2,GREETZ_Events[i].y + rand()%5,
									GREETZ_Events[i].texto,fntGBL_TypeWriter,(char*)img8GREETZ_Alpha->bits,16);
	}
	//Fusionar layer alfa con fondo rgb
FlatternAlphaInv(GBL_Bits,GBL_Bits,(char*)(img8GREETZ_Alpha->bits)+321,320*220);
    FlatternAlpha(GBL_Bits,GBL_Bits,img8GREETZ_Alpha->bits);
	Margen320x240();
}


//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool GREETZ_Init()
{	
  SEC_START("Initializing GREETZ!");

	
  img32GREETZbk = IMAGE_Load("data\\wrd1.gif",32);
	if (!img32GREETZbk) return false;
	img8GREETZ_Alpha = IMAGE_Load("data\\new.gif",8);    
	if (!img8GREETZ_Alpha) return false;

	GREETZ_IniTick = -1;
  LOG ("GREETZ initialized!");
	SEC_END();

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void GREETZ_Close()
{
	if (img32GREETZbk)
	{
		FreeMem(img32GREETZbk);
		img32GREETZbk = NULL;
	}
	if (img8GREETZ_Alpha)
	{
		FreeMem(img8GREETZ_Alpha);
		img8GREETZ_Alpha = NULL;
	}

  LOG ("GREETZ closed!");
}


//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void GREETZ_Guio()
{
  GUIO_AddFN(GREETZ_Play,"GREETZ_Play");
  GUIO_AddFN(GREETZ_Action1,"GREETZ_Action1");
}


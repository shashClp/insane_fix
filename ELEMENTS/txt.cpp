#include <windows.h>
#include <image.h>
#include <efx2d.h>
#include <mem.h>
#include <guio.h>
#include <log.h>
#include "rt.h"

//****************************************************************************
//* TEXTOS SUBLIMINALES                                                      *
//****************************************************************************

DWORD *img32TXT2_Telon;
char *img8TXT2_Alpha;
char *img8TXT2_AlphaRuido;

extern TImage *img8GBL_Ruido;
extern SFuente* fntGBL_TypeWriter;
TImage *El[3];

bool CheckFrameRate(long iTick)
{
	static iLastFrame = -1;
    return (iTick/10) != iLastFrame;
}

//////////////////////////////////////////////////////////////////////////////
//
//  1 parte Sprays:

void TXT2_TODO(float iTick)
{    
  long iFactor = (rand() % 35);

  ScrollToScreen8(img8TXT2_AlphaRuido,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,img32TXT2_Telon,img8TXT2_AlphaRuido);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8TXT2_Alpha);    
}

void TXT2_INSANEINTHEBRAIN(float iTick)
//Entre videos de los palos
{
  if(!CheckFrameRate(iTick)) return;

	memset(img8TXT2_Alpha,0,GBL_Size);
	FONT_Print(50,100,"InSaNe iN",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70,130,"tHe bRaIn",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(50-(rand()%2),100-(rand()%3),"InSaNe iN",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70+(rand()%2),130+(rand()%3),"tHe bRaIn",fntGBL_TypeWriter,img8TXT2_Alpha,18);

  long iFactor = (rand() % 35);
  ScrollToScreen8(img8TXT2_AlphaRuido,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,img32TXT2_Telon,img8TXT2_AlphaRuido);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8TXT2_Alpha);  
}

void TXT2_WEREGOINGINSANE(float iTick)
//Entre videos de los palos
{
  if(!CheckFrameRate(iTick)) return;

	memset(img8TXT2_Alpha,0,GBL_Size);
	FONT_Print(50,100,"wE're gOiNg",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70,130,"INsANe!",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(50-(rand()%2),100-(rand()%3),"wE're gOiNg",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70+(rand()%2),130+(rand()%3),"INsANe!",fntGBL_TypeWriter,img8TXT2_Alpha,18);

  long iFactor = (rand() % 35);
  ScrollToScreen8(img8TXT2_AlphaRuido,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,img32TXT2_Telon,img8TXT2_AlphaRuido);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8TXT2_Alpha);  
}

void TXT2_BREAKEMOFFSOME(float iTick)
//Entre videos de los palos
{
  if(!CheckFrameRate(iTick)) return;

	memset(img8TXT2_Alpha,0,GBL_Size);
	FONT_Print(50,100,"bReaK'EM",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70,130,"OfF SomE!",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(50-(rand()%2),100-(rand()%3),"bReaK'EM",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70+(rand()%2),130+(rand()%3),"OfF SomE!",fntGBL_TypeWriter,img8TXT2_Alpha,18);

  long iFactor = (rand() % 35);
  ScrollToScreen8(img8TXT2_AlphaRuido,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,img32TXT2_Telon,img8TXT2_AlphaRuido);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8TXT2_Alpha);  
}

void TXT2_LOVEQUAKERS(float iTick)
//Entre videos de los palos
{
  if(!CheckFrameRate(iTick)) return;

	memset(img8TXT2_Alpha,0,GBL_Size);
	FONT_Print(50,100,"ScEneRs loVe",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70,130,"QuakErks!",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(50-(rand()%2),100-(rand()%3),"ScEneRs loVe",fntGBL_TypeWriter,img8TXT2_Alpha,18);
	FONT_Print(70+(rand()%2),130+(rand()%3),"QuakErks!",fntGBL_TypeWriter,img8TXT2_Alpha,18);

  long iFactor = (rand() % 35);
  ScrollToScreen8(img8TXT2_AlphaRuido,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,img32TXT2_Telon,img8TXT2_AlphaRuido);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8TXT2_Alpha);  
}

void TXT2_ELEM1(float iTick)
{
	FlatternAlphaInv(GBL_Bits+121*320+1,GBL_Bits+121*320+1,(char*)(El[0]->bits)+120*320,80*320);  
	FlatternAlpha(GBL_Bits+120*320,GBL_Bits+120*320,(char*)(El[0]->bits)+120*320,80*320);
}

void TXT2_ELEM2(float iTick)
{
	FlatternAlphaInv(GBL_Bits+121*320+1,GBL_Bits+121*320+1,(char*)(El[1]->bits)+120*320,80*320);  
	FlatternAlpha(GBL_Bits+120*320,GBL_Bits+120*320,(char*)(El[1]->bits)+120*320,80*320);

//	FlatternAlphaInv(GBL_Bits,GBL_Bits,(char*)(El[1]->bits));  
//	FlatternAlpha(GBL_Bits,GBL_Bits,(char*)(El[1]->bits));
}

void TXT2_ELEM3(float iTick)
{
	FlatternAlphaInv(GBL_Bits+121*320+1,GBL_Bits+121*320+1,(char*)(El[2]->bits)+120*320,80*320);  
	FlatternAlpha(GBL_Bits+120*320,GBL_Bits+120*320,(char*)(El[2]->bits)+120*320,80*320);
}

//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool TXT2_Init()
{
  img8TXT2_AlphaRuido = (char *)GetMem (GBL_Size);      
  if (!img8TXT2_AlphaRuido) return false;

  img8TXT2_Alpha = (char *)GetMem (GBL_Size);
  if (!img8TXT2_Alpha) return false;

  img32TXT2_Telon = (DWORD *)GetMem (GBL_Size4);      
  if (!img32TXT2_Telon) return false;
 
  FONT_Print(50,100,"todo:",fntGBL_TypeWriter,img8TXT2_Alpha,25);
  //Recorta32(&img32SPR_Telon,20);
	El[0] = IMAGE_Load("DATA\\elem.gif",8);
	El[1] = IMAGE_Load("DATA\\elem2.gif",8);
	El[2] = IMAGE_Load("DATA\\elem3.gif",8);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void TXT2_Close()
{
  if (img8TXT2_Alpha)
  {
    FreeMem (img8TXT2_Alpha);
    img8TXT2_Alpha = NULL;
  }
  if (img32TXT2_Telon)
  {
    FreeMem (img32TXT2_Telon);
    img32TXT2_Telon = NULL;
  }
  if (img8TXT2_AlphaRuido)
  {
    FreeMem (img8TXT2_AlphaRuido);
    img8TXT2_AlphaRuido = NULL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void TXT2_Guio()
{
  GUIO_AddFN (TXT2_INSANEINTHEBRAIN,"TXT2_INSANEINTHEBRAIN");
  GUIO_AddFN (TXT2_WEREGOINGINSANE,"TXT2_WEREGOINGINSANE");
  GUIO_AddFN (TXT2_BREAKEMOFFSOME,"TXT2_BREAKEMOFFSOME");
  GUIO_AddFN (TXT2_LOVEQUAKERS,"TXT2_LOVEQUAKERS");
	GUIO_AddFN (TXT2_ELEM1,"TXT2_ELEM1");
	GUIO_AddFN (TXT2_ELEM2,"TXT2_ELEM2");
	GUIO_AddFN (TXT2_ELEM3,"TXT2_ELEM3");
  GUIO_AddFN (TXT2_TODO,"TXT2_TODO");
}
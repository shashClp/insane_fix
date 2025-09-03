#include <windows.h>
#include <guio.h>
#include <log.h>
#include <efx2d.h>
#include <misc.h>
#include "trash.h"
#include "cpart.h"

CPartSys1 ESTEL_pt;
CPartSys1 ESTEL_pt2;
char *img8Tmp;
TImage *ESTEL_Logo1;
TImage *ESTEL_Logo2;

bool ESTEL_Init()
{
  SEC_START ("Initializing Estel");
  ESTEL_pt.Init();
  ESTEL_pt.SetMaxPart(300);
  if (!ESTEL_pt.SetBitmapAlpha("data\\part.gif")) return false;

  ESTEL_pt2.Init();
  ESTEL_pt2.SetMaxPart(300);
  if (!ESTEL_pt2.SetBitmapRGB("data\\flar2.tga")) return false;

  img8Tmp = (char *)GetMem (GBL_Size);

  ESTEL_Logo1 = IMAGE_Load ("data\\anafin1.gif",8);
  if (!ESTEL_Logo1) return false;
  ESTEL_Logo2 = IMAGE_Load ("data\\anafin.gif",8);
  if (!ESTEL_Logo2) return false;

  LOG ("Estel initialized!");
  SEC_END();
  return true;
}

void ESTEL_Close()
{
  LOG ("Closing Estel");
  if (img8Tmp)
  {
    FreeMem (img8Tmp);
    img8Tmp = NULL;
  }
  if (ESTEL_Logo1)
  {
    FreeMem (ESTEL_Logo1);
    ESTEL_Logo1 = NULL;
  }
  if (ESTEL_Logo2)
  {
    FreeMem (ESTEL_Logo2);
    ESTEL_Logo2 = NULL;
  }
  LOG ("Estel closed!");
}

void ESTEL_Scene(float tick)
{
  memset (img8Tmp,0,GBL_Size);
	ESTEL_pt.MueveWeird1(tick/100.0f);
	ESTEL_pt.RenderPartA(img8Tmp);
  Alpha2RGB(GBL_Bits,img8Tmp);
}

void ESTEL_SceneRGB(float tick)
{
  MISC_MemSet4 (GBL_Bits,0,GBL_Size);
	ESTEL_pt2.MueveWeird1(0.5f+tick/100.0f);
	ESTEL_pt2.RenderPartRGB(GBL_Bits);
}

void ESTEL_Scene1RGB(float tick)
{
	ESTEL_pt2.MueveWeird1(0.5f+tick/100.0f);
	ESTEL_pt2.RenderPartRGB(GBL_Bits);
}

void ESTEL_Scene1(float tick)
{
  long iFactor = (rand() % 35);
  ScrollToScreen8(img8Tmp,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8Tmp);  
}

void ESTEL_Scene2(float tick)
{
  long iFactor = (rand() % 35);
  ScrollToScreen8(img8Tmp,img8GBL_Ruido,iFactor,0);
  FlatternAlpha(GBL_Bits,GBL_Bits,img8Tmp);  
}

void ESTEL_Logo(float tick)
{
  FlatternAlphaInv(GBL_Bits,GBL_Bits,(char *)ESTEL_Logo1->bits);  
  FlatternAlpha(GBL_Bits,GBL_Bits,(char *)ESTEL_Logo2->bits);  
}

void ESTEL_Guio()
{
  GUIO_AddFN (ESTEL_Scene,"ESTEL_Scene");
  GUIO_AddFN (ESTEL_Scene1,"ESTEL_Scene1");
  GUIO_AddFN (ESTEL_Scene2,"ESTEL_Scene2");
  GUIO_AddFN (ESTEL_SceneRGB,"ESTEL_SceneRGB");
  GUIO_AddFN (ESTEL_Scene1RGB,"ESTEL_Scene1RGB");
  GUIO_AddFN (ESTEL_Logo,"ESTEL_Logo");
}

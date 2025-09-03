#include <windows.h>
#include <image.h>
#include <profile.h>
#include <log.h>
#include <mem.h>
#include <blobs3d.h>
#include <ball.h>
#include <defines.h>
#include <guio.h>
#include <view3d.h>
#include <misc.h>
#include <default.h>
#include <keys.h>
#include <math.h>

TImage *ELITE_Fons;
TImage *ELITE_FonsNou;

bool ELITE_Init()
{
  SEC_START ("Initializing ELITE");

  ELITE_Fons = IMAGE_Load ("data\\elite2.gif",32);
	if (!ELITE_Fons) return false;

  ELITE_FonsNou = IMAGE_Load ("data\\elite1.gif",32);
	if (!ELITE_FonsNou) return false;

  LOG ("ELITE initialized!");
  SEC_END();   
  return true;
}

void ELITE_Close()
{
  LOG ("Closing ELITE");

  if (ELITE_Fons)
  {
    FreeMem (ELITE_Fons);  
    ELITE_Fons = NULL;
  }
  if (ELITE_FonsNou)
  {
    FreeMem (ELITE_FonsNou);  
    ELITE_FonsNou = NULL;
  }

  LOG ("ELITE closed!");
}

void ELITE_Scene1(float tick)
{  
  MISC_MemCpy4 (GBL_Bits,ELITE_Fons->bits,GBL_Size);
}

void ELITE_Scene2(float tick)
{ 
  DWORD j = (tick*240)*GBL_Width;    
  MISC_MemCpy4 (GBL_Bits,((DWORD *)ELITE_FonsNou->bits),j);
}

void ELITE_Scene3(float tick)
{    
  MISC_MemCpy4 (GBL_Bits,ELITE_FonsNou->bits,GBL_Size);
}

void ELITE_Guio()
{
  GUIO_AddFN (ELITE_Scene1,"ELITE_Scene1");  
  GUIO_AddFN (ELITE_Scene2,"ELITE_Scene2");  
  GUIO_AddFN (ELITE_Scene3,"ELITE_Scene3");  
}
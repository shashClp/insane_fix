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
#include "video2.h"

TImage *Cristall1 = NULL;

bool CRISTALL_Init()
{
    
  if ((Cristall1 = IMAGE_Load ("data\\roto1.gif",8))==NULL) return false;
  return true;
}

void CRISTALL_Close()
{

  if (Cristall1!=NULL)
  {
    FreeMem (Cristall1);  
    Cristall1 = NULL;
  }
}

void Cristall_Scene1(float tick)
{  
	if (FrameChanged)
	{
		for (long i=0; i<320*199; i++)
		{
			GBL_Bits[i] = GBL_Bits[i+((byte*)Cristall1->bits)[i]*6];
		}
	}
}

void CRISTALL_Guio()
{
  GUIO_AddFN (Cristall_Scene1,"Cristall_Scene1");  
}


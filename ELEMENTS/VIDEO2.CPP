#include <guio.h>
#include <log.h>
#include <misc.h>
#include <view.h>

#include <cvideo.h>

//****************************************************************************
//* VIDEOS                                                                   *
//****************************************************************************

CVideo cvPelea;
CVideo cvPalo1;
CVideo cvPalo2;
CVideo cvRoto1;
CVideo cvRoto2;
extern bool FrameChanged = false;
//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PELEA (float fTick) 
{
    long pos = fTick*cvPelea.GetTotalFrames();

    if(pos >= cvPelea.GetActFrame())
        cvPelea.RenderRGB(GBL_Bits);
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PALO1 (float fTick) 
{
    long pos = fTick*cvPalo1.GetTotalFrames();
    if(pos >= cvPalo1.GetActFrame())
		{
        cvPalo1.RenderRGBPal(GBL_Bits);
				FrameChanged = true;
		} else FrameChanged = false;
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PALO2 (float fTick)
{
    long pos = fTick*cvPalo2.GetTotalFrames();
    if(pos >= cvPalo2.GetActFrame())
		{
        cvPalo2.RenderRGBPal(GBL_Bits);
				FrameChanged = true;
		} else FrameChanged = false;
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_ROTO1 (float fTick)
{
    long pos = fTick* cvRoto1.GetTotalFrames();
    if(pos >= cvRoto1.GetActFrame())
		{
        cvRoto1.RenderRGB(GBL_Bits);
				FrameChanged = true;
		} else FrameChanged = false;
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_ROTO2( float fTick) 
{
    long pos = fTick* cvRoto2.GetTotalFrames();
    if(pos >= cvRoto2.GetActFrame())
		{
        cvRoto2.RenderRGB(GBL_Bits);
				FrameChanged = true;
		} else FrameChanged = false;
}


//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool VIDEOS_Init() 
{
  SEC_START ("Initializing Videos!");
  if (!cvPelea.Load("data\\pelea.zmc")) return false;
  cvPelea.SetNoise(7);
  cvPelea.SetInterlace(true);

  if (!cvPalo1.Load("data\\palo1.zmc")) return false;
  cvPalo1.SetNoise(7);
  cvPalo1.SetInterlace(true);
 
  if (!cvPalo1.LoadPal("data\\video.col")) return false;
  if (!cvPalo2.Load("data\\palo2.zmc")) return false;
  cvPalo2.SetNoise(7);
  cvPalo2.SetInterlace(true);
  cvPalo2.SetRewind(true);//

  if (!cvPalo2.LoadPal("data\\video.col")) return false;
  if (!cvRoto1.Load("data\\roto1.zmc")) return false;
  cvRoto1.SetNoise(15);
  cvRoto1.SetRewind(true);//

  if (!cvRoto2.Load("data\\roto2.zmc")) return false;
  cvRoto2.SetNoise(7);
  cvRoto2.SetRewind(true);//

  LOG ("Videos Initialized!");

	SEC_END();
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void VIDEOS_Close()
{
		
}


//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void VIDEOS_Guio()
{
	GUIO_AddFN (VIDEOS_PELEA,"VIDEOS_PELEA");
	GUIO_AddFN (VIDEOS_PALO1,"VIDEOS_PALO1");
	GUIO_AddFN (VIDEOS_PALO2,"VIDEOS_PALO2");
	GUIO_AddFN (VIDEOS_ROTO1,"VIDEOS_ROTO1");
	GUIO_AddFN (VIDEOS_ROTO2,"VIDEOS_ROTO2");
}





#include <cvideo.h>

//****************************************************************************
//* VIDEOS                                                                   *
//****************************************************************************

CVideo cvPelea;
CVideo cvPalo1;
CVideo cvPalo2;
CVideo cvRoto1;
CVideo cvRoto2;

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PELEA(long iTick) 
{
    long pos = (iTick*24/1000) % cvPelea.GetTotalFrames();
    if(pos >= cvPelea.GetActFrame())
        cvPelea.RenderRGB(GBL_Bits);
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PALO1(long iTick) 
{
    long pos = (iTick*24/1000) % cvPalo1.GetTotalFrames();
    if(pos >= cvPalo1.GetActFrame())
        cvPalo1.RenderRGBPal(GBL_Bits);
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_PALO2(long iTick) 
{
    long pos = (iTick*24/1000) % cvPalo2.GetTotalFrames();
    if(pos >= cvPalo2.GetActFrame())
        cvPalo2.RenderRGBPal(GBL_Bits);
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_ROTO1(long iTick) 
{
    long pos = (iTick*24/1000) % cvRoto1.GetTotalFrames();
    if(pos >= cvRoto1.GetActFrame())
        cvRoto1.RenderRGB(GBL_Bits);
}

//////////////////////////////////////////////////////////////////////////////
//
//  

void VIDEOS_ROTO2(long iTick) 
{
    long pos = (iTick*24/1000) % cvRoto2.GetTotalFrames();
    if(pos >= cvRoto2.GetActFrame())
        cvRoto2.RenderRGB(GBL_Bits);
}


//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

void VIDEOS_Init()
{
  cvPelea.Load("data\\pelea.zmc");
  cvPelea.SetNoise(7);
  cvPelea.SetInterlace(true);

  cvPalo1.Load("data\\palo1.zmc");
  cvPalo1.SetNoise(7);
  cvPalo1.SetInterlace(true);
  cvPalo1.LoadPal("data\\video.col");
  cvPalo2.Load("data\\palo2.zmc");
  cvPalo2.SetNoise(7);
  cvPalo2.SetInterlace(true);
  cvPalo2.SetRewind(false);
  cvPalo2.LoadPal("data\\video.col");

  cvRoto1.Load("data\\roto1.zmc");
  cvRoto1.SetNoise(15);
  cvRoto1.SetRewind(false);
  cvRoto2.Load("data\\roto2.zmc");
  cvRoto2.SetNoise(7);
  cvRoto2.SetRewind(false);
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void VIDEOS_Close()
{

}

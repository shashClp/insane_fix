#include <windows.h>
#include <defines.h>
#include <rotozoom.h>
#include <log.h>
#include <math.h>
#include <view.h>

effectRotoZoom *effRotoZoom;

extern "C" DWORD *RotoZoom_Source;
extern "C" DWORD *RotoZoom_Dest;
extern "C" DWORD RotoZoom_IncDef;
extern "C" DWORD RotoZoom_Width;
extern "C" DWORD RotoZoom_Pitch;
extern "C" DWORD RotoZoom_Height;
extern "C" WORD RotoZoom_IncX;
extern "C" WORD RotoZoom_IncY;
extern "C" WORD RotoZoom_IncX2;
extern "C" WORD RotoZoom_IncY2;
extern "C" WORD RotoZoom_StartX;
extern "C" WORD RotoZoom_StartY;

extern "C" void MakeRotoZoomer();

effectRotoZoom::effectRotoZoom()
{
  LOG ("Initializing effect RotoZoom");
}

effectRotoZoom::~effectRotoZoom()
{
}

void effectRotoZoom::setDimension (DWORD width,DWORD height)
{
  Width = width;
  Height = height;
}

void effectRotoZoom::makeEffect1 (float sizex,float sizey,float anglex,float angley)
{    
  RotoZoom_IncX = (WORD)(sizex*sin(anglex*M_ToRad));
  RotoZoom_IncY = (WORD)(sizey*cos(angley*M_ToRad));

  RotoZoom_IncX2 = RotoZoom_IncY;
  RotoZoom_IncY2 = -RotoZoom_IncX;

  RotoZoom_StartX = -((int)Width/2)*RotoZoom_IncX;
  RotoZoom_StartY = -((int)Height/2)*RotoZoom_IncY;

  RotoZoom_IncDef = 0;
  RotoZoom_Width = Width;
  RotoZoom_Pitch = Width*4;
  RotoZoom_Height = Height;
  RotoZoom_Dest = DstBuffer;
  RotoZoom_Source = SrcBuffer;
  MakeRotoZoomer();
}

void effectRotoZoom::makeEffect2 (float sizex,float sizey,float anglex,float angley)
{
  RotoZoom_IncX = (WORD)(sizex*sin(anglex*M_ToRad));
  RotoZoom_IncY = (WORD)(sizey*cos(angley*M_ToRad));

  RotoZoom_IncX2 = RotoZoom_IncY;
  RotoZoom_IncY2 = -RotoZoom_IncX;

  RotoZoom_StartX = -((int)Width/2)*RotoZoom_IncX;
  RotoZoom_StartY = -((int)Height/2)*RotoZoom_IncY;

  RotoZoom_IncDef = 0x00010001;
  RotoZoom_Width = Width;
  RotoZoom_Pitch = Width*4;
  RotoZoom_Height = Height;
  RotoZoom_Dest = DstBuffer;
  RotoZoom_Source = SrcBuffer;
  MakeRotoZoomer();
}

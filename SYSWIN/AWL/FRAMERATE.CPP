// AWL - Anaconda Windows Library
// FrameRate

#include <framerate.h>
#include <debug.h>
#include <video.h>
#include <syswin.h>

DWORD FRAMERATE_Frames=300;
LONGLONG FRAMERATE_Frequency=0;
LONGLONG FRAMERATE_Ticks=0;
float FRAMERATE_FPS=0;
bool FRAMERATE_active=false;

void FRAMERATE_Init()
{
  QueryPerformanceFrequency((LARGE_INTEGER *)&FRAMERATE_Frequency);  
  LONGLONG tick;    
  QueryPerformanceCounter((LARGE_INTEGER *)&tick);
  FRAMERATE_Ticks = tick;
}

void FRAMERATE_Paint()
{  
  if (IsIconic(HWnd)) return;
  if (!FRAMERATE_active) return;
  FRAMERATE_Frames++;  
  if (MMX)
  {
    DEBUG (2,1,"fps: %3.2f (MMX)",FRAMERATE_FPS);  
  } else {
    DEBUG (2,1,"fps: %3.2f",FRAMERATE_FPS);  
  }

  LONGLONG tick;    
  QueryPerformanceCounter((LARGE_INTEGER *)&tick);
  
  FRAMERATE_FPS = (float)(((double)FRAMERATE_Frames*FRAMERATE_Frequency)/(tick-FRAMERATE_Ticks));
  if (FRAMERATE_Frames>100)
  {
    FRAMERATE_Frames = 0;
    FRAMERATE_Ticks = tick;
  }
}

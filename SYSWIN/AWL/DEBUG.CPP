// AWL - Anaconda Windows Library
// Multimonitoring Debug Window

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <video.h>
#include <debug.h>
#include <syswin.h>
#include <misc.h>
#include <log.h>

va_list DEBUG_VaList;
char DEBUG_Buffer1[256];

void DEBUG (int x,int y,char *str,...)
{
HDC dc;

  if (IsIconic(HWnd)) return;   
  RECT r;
  GetWindowRect (HWnd,&r);
  x+=r.left;
  y+=r.top;
  if (VIDEO_Windowed)
  {
    x+=3;
    y+=23;
  }

  va_start (DEBUG_VaList,str);
  vsprintf (DEBUG_Buffer1,str,DEBUG_VaList);  
  va_end (DEBUG_VaList);

  hr = VIDEO_Primary->GetDC (&dc);
  if (hr==DDERR_SURFACELOST) return;
  else if (ERRDX("Getting DC in DEBUG")) return;
  SetBkMode(dc,TRANSPARENT);
  SetTextColor (dc,0xffffff);
  TextOut (dc,x+1,y+1,DEBUG_Buffer1,strlen(DEBUG_Buffer1));
  TextOut (dc,x-1,y+1,DEBUG_Buffer1,strlen(DEBUG_Buffer1));
  TextOut (dc,x+1,y-1,DEBUG_Buffer1,strlen(DEBUG_Buffer1));
  TextOut (dc,x-1,y-1,DEBUG_Buffer1,strlen(DEBUG_Buffer1));
  SetTextColor (dc,0x000000);
  TextOut (dc,x,y,DEBUG_Buffer1,strlen(DEBUG_Buffer1));
  hr = VIDEO_Primary->ReleaseDC(dc);     
  if (hr==DDERR_SURFACELOST) return;
  else if (ERRDX("Releasing DC in DEBUG")) return;
}
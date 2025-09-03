// AWL - Anaconda Windows Library
// Direct X Video Initializer

#ifndef AWL_VIDEO
#define AWL_VIDEO

#include <windows.h>
#include <ddraw.h>

#define VIDEO_WINDOW            1
#define VIDEO_MENU              2

extern bool VIDEO_Windowed;
extern bool VIDEO_EraseBackGround;

extern DWORD VIDEO_ScreenWidth;
extern DWORD VIDEO_ScreenHeight;
extern DWORD VIDEO_ScreenDisplay;
extern BYTE VIDEO_ScreenBpp;


void VIDEO_NotifyResize (DWORD width, DWORD height);

void VIDEO_Flip ();
bool VIDEO_Init ();
bool VIDEO_SetUp (DWORD width,DWORD height,BYTE bpp,DWORD flags);
void VIDEO_Close (void);
void VIDEO_Guio ();

#endif

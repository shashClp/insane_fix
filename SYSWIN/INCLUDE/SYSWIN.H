// AWL - Anaconda Windows Library

#ifndef AWL_SYSTEMWINDOWS
#define AWL_SYSTEMWINDOWS

#include <windows.h>

extern HANDLE ThreadID;
extern bool Keys[256];
extern bool LMouse;
extern bool MMouse;
extern bool RMouse;
extern int MouseX;
extern int MouseY;
extern bool Exit;
extern HWND HWnd;
extern HINSTANCE HInst;
extern bool MMX;
extern bool MMXInit;
extern double CPUSpeed;

void SYSWIN_SetPriority (DWORD p);
double SYSWIN_GetCPUSpeed ();

#endif

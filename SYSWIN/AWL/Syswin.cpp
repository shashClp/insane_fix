// AWL - Anaconda Windows Library
// Sys-Win

#include <windows.h>
#include <process.h>
#include <keys.h>
#include <video.h>
#include <log.h>
#include <awm.h>
#include <framerate.h>
#include <profile.h>
#include <elisium.h>
#include <mem.h>
#include <misc.h>

#define VIDEO_DEFAULT_WIDTH         320
#define VIDEO_DEFAULT_HEIGHT        240
#define VIDEO_DEFAULT_BPP           32

HWND HWnd;
HINSTANCE HInst;
HANDLE ThreadID;
bool LMouse;
bool MMouse;
bool RMouse;
int MouseX;
int MouseY;
bool Exit;
bool MMX=true;
bool MMXInit;
double CPUSpeed;

extern void main (int argc, char *argv[]);

void SYSWIN_SetPriority (DWORD p);

double SYSWIN_GetCPUSpeed ()
{
  SEC_START ("Testing CPU speed!");  
  SYSWIN_SetPriority (THREAD_PRIORITY_TIME_CRITICAL);  

  DWORD ti = GetTickCount();
  _int64 tti = MISC_GetTickCount();
  _asm {
    mov ecx,0xffffff
    LoopT:
      xor edx,edx
      mov eax,1
      xor ebx,ebx      
      xor edx,edx
      mov eax,1
      xor edx,edx
      mov eax,1
      xor ebx,ebx      
      xor edx,edx
      dec ecx
    jnz LoopT
  }  
  _int64 ttf = MISC_GetTickCount();
  DWORD tf = GetTickCount();
  SYSWIN_SetPriority (ELISIUM_Priority);

  CPUSpeed = ((double)ttf-tti)/((double)tf-ti);
  LOG ("CPU Speed(Cicle to Tick) = %3.3f Mhz",CPUSpeed/1000.0);
  SEC_END();

  return CPUSpeed;
}

void SYSWIN_SetPriority (DWORD p)
{
  switch (p)
  {
    case 0:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_LOWEST); break;
    case 1:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_BELOW_NORMAL); break;     
    case 2:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_NORMAL); break;
    case 3:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_ABOVE_NORMAL); break;
    case 4:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_HIGHEST); break;
    case 5:
      SetThreadPriority (ThreadID,THREAD_PRIORITY_TIME_CRITICAL); break;      
  }
}

void ThreadFunction (LPVOID arg)
{
  // Conversio de parametres
  arg = NULL;
  int argc = 1;
  char **argv = (char **) GetMem (sizeof(char *)*50);
  char *targ = GetCommandLine()+1;
  argv[0] = targ;
  while (*targ != '"') targ++;
  *targ++ = '\0';
  while (*targ != '\0')
  {
    while ((*targ == ' ') && (*targ != '\0')) targ++;
    if (*targ != '\0') argv[argc++] = targ;
    while ((*targ != ' ') && (*targ != '\0')) targ++;
    if (*targ != '\0') *targ++ = '\0';
  }
  
  if (MMX) MMX=MMXInit;
  
  // Start of Kode
  LOG ("Executing main...");
  main (argc,argv);    
  LOG ("Main finished");  

  // Alliberem memoria
  FreeMem (argv);
  ThreadID = (void *)-1;
  Exit = true;  
}

LRESULT CALLBACK WndProc (HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
  switch (msg)
  {
    case WM_KEYDOWN:
      Keys[wparam] = true;
      break;
    case WM_KEYUP:
      Keys[wparam] = false;
      if (wparam==VK_ESCAPE) { Exit = true; break; }
      if (wparam==VK_F12) { FRAMERATE_active = !FRAMERATE_active; break; }
      if (wparam==VK_F11) { if (MMXInit) MMX = !MMX; break; }
      if (wparam==VK_F9) { if (MUSIC_GetVolume()==0) MUSIC_Volume(MUSIC_MAX_VOLUME); else MUSIC_Volume(0); break; }
      if (wparam==VK_F8) { PROFILE_active = !PROFILE_active; break; }
      break;
    case WM_ERASEBKGND:
      return false;
    case WM_CLOSE:
      Exit=true;
      break;
  }
  return DefWindowProc (hwnd,msg,wparam,lparam);
}

int PASCAL WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
WNDCLASS wc;

  if (!LOG_Init ()) 
  {
    ERR ("Unable to init Log System");
    return false;
  }

  // Deteccio MMX
  MMXInit=true;
  _asm {
    xor edx,edx
    mov eax,1
    cpuid
    test edx,00800000h
    jnz MMX_SI
    mov [MMXInit],0
    MMX_SI:    
  }
  if (MMXInit) LOG ("MMX detected!");

  if (!VIDEO_SetUp (VIDEO_DEFAULT_WIDTH,VIDEO_DEFAULT_HEIGHT,VIDEO_DEFAULT_BPP,VIDEO_MENU)) return false;  

  MMX = ELISIUM_MMX;
  
  if (!hPrevInstance)
  {
    LOG ("Registering window class");
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0; 
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "AnacondaSysWin30"; 
    if (!RegisterClass (&wc)) 
    {
      ERR ("Can't register class");
      return false;
    }
  }

  HInst = hInstance;  
  LOG ("Creating window");
  if (VIDEO_Windowed)
  {
    HWnd = CreateWindow ("AnacondaSysWin30",
                         "SysWin 3.0",
                         WS_SIZEBOX | WS_BORDER | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
                         0,0,320+6,240+23,NULL,NULL,hInstance,NULL);    
  } else {
    HWnd = CreateWindow ("AnacondaSysWin30",
                         "SysWin 3.0",
                         WS_POPUP,
                         0,0,320,240,NULL,NULL,hInstance,NULL);
    ShowCursor (false);
  }
  if (!HWnd) 
  {
    ERR ("Can't create window handle");
    return false;  
  }
  
  ShowWindow (HWnd,SW_NORMAL);  
  UpdateWindow (HWnd);

  LOG ("Begin thread");
  Exit = false;
  LMouse = MMouse = RMouse = false;
  for (int i=0; i<256; i++) Keys[i] = false; 
  ThreadID = (void *)_beginthread (ThreadFunction,0,lpCmdLine);
  if (ThreadID==(void *)-1)
  {
    ERR ("Can't create thread");
    return false;  
  }

  SYSWIN_SetPriority(ELISIUM_Priority);
    
  MSG msg;
  while (!Exit)
  {      
    if (PeekMessage (&msg,HWnd,0,0,PM_REMOVE))
    {
      TranslateMessage (&msg);
      DispatchMessage (&msg);    
    }    
  }

  MemLog();
  LOG ("Application closed");
  LOG_Close ();
  
  return false;
}

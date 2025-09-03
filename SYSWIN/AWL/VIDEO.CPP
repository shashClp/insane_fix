// AWL - Anaconda Windows Library
// Direct X Video Initializer

#include <windows.h>
#include <ddraw.h>
#include <log.h>
#include <syswin.h>
#include <guio.h>
#include <math.h>
#include <dll.h>
#include <elisium.h>
#include <defines.h>
#include <misc.h>
#include <video.h>

bool VIDEO_Windowed;
bool VIDEO_EraseBackGround=true;
DWORD VIDEO_ScreenWidth;
DWORD VIDEO_ScreenHeight;
BYTE VIDEO_ScreenBpp;
DWORD VIDEO_ScreenDriver;
LPDIRECTDRAW4 VIDEO_DD;
LPDIRECTDRAWSURFACE4 VIDEO_Primary;
LPDIRECTDRAWSURFACE4 VIDEO_BackBuffer;
LPDIRECTDRAWCLIPPER VIDEO_Clipper;

typedef struct {
  GUID guid;
  char name[128];
  char desc[128];
} TDDDevices;

typedef struct {  
  DWORD width;
  DWORD height;
  BYTE bpp;
} TDDModes;

DWORD VIDEO_DeviceNum;
TDDDevices VIDEO_DeviceDesc[16];
DWORD VIDEO_ModesNum[16];
TDDModes VIDEO_ModeDesc[16][32];

void VIDEO_Flip()
{
  if (!IsIconic (HWnd))
  {  
    if (VIDEO_Windowed)
    {
      RECT r,rs;
      DDBLTFX ddbltfx;
      ZeroMemory (&ddbltfx,sizeof(DDBLTFX));
      ddbltfx.dwSize = sizeof(DDBLTFX);
      GetWindowRect (HWnd,&rs);
      GetClientRect (HWnd,&r);
      OffsetRect (&r,rs.left+4,rs.top+23);
      hr = VIDEO_Primary->Blt (&r,VIDEO_BackBuffer,NULL,DDBLT_DDFX,&ddbltfx);
      if (hr == DDERR_SURFACELOST)
      {
        VIDEO_BackBuffer->Restore();        
        VIDEO_Primary->Restore();        
      } else
        if (ERRDX ("Video flip!")) return;
    } else {
      hr = VIDEO_Primary->Flip(NULL,DDFLIP_WAIT);
      if (hr == DDERR_SURFACELOST)
        VIDEO_Primary->Restore();
      else
        if (ERRDX ("Video flip!")) return;
    }  
  }
}

bool VIDEO_DefaultSetupVideoMode (DWORD &width,DWORD &height,BYTE &bpp,DWORD &flags,DWORD &driver)
{
  SEC_START ("Default Setup Video Mode Menu!");

  typedef bool (*LPSETUPInit)(DWORD,DWORD,DWORD,BYTE,bool,bool,bool,bool);
  typedef void (*LPSETUPAdd)(const char *);
  typedef void (*LPSETUPAddRes)(DWORD,DWORD,BYTE);
  typedef void (*LPSETUPGetRes)(DWORD &,DWORD &,BYTE &);
  typedef bool (*LPSETUPGetB)(void);
  typedef DWORD (*LPSETUPGetD)(void);
  
  LPSETUPInit SETUP_Init;
  LPSETUPAdd SETUP_AddDriver;
  LPSETUPAddRes SETUP_AddResolution;
  LPSETUPGetRes SETUP_GetResolution;  
  LPSETUPGetD SETUP_GetDriver;
  LPSETUPGetB SETUP_GetFullScreen;
  LPSETUPGetB SETUP_GetSound;
  LPSETUPGetB SETUP_GetLooped;
  LPSETUPGetD SETUP_GetSoundFlags;
  LPSETUPGetD SETUP_GetSoundQuality;
  LPSETUPGetB SETUP_GetMMX;
  LPSETUPGetD SETUP_GetPriority;

  HINSTANCE hDLL = DLL_Init ("elisium");
  if (hDLL==NULL)
  {
    ERR ("Video Initializing DLL menu");
    return false;
  }

  if ((!DLL_Call (hDLL,(void **)&SETUP_Init,"SETUP_Init")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_AddDriver,"SETUP_AddDriver")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetDriver,"SETUP_GetDriver")) ||        
      (!DLL_Call (hDLL,(void **)&SETUP_AddResolution,"SETUP_AddResolution")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetResolution,"SETUP_GetResolution")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetFullScreen,"SETUP_GetFullScreen")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetLooped,"SETUP_GetLooped")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetSound,"SETUP_GetSound")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetSoundQuality,"SETUP_GetSoundQuality")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetSoundFlags,"SETUP_GetSoundFlags")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetMMX,"SETUP_GetMMX")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetPriority,"SETUP_GetPriority")))
  {
      ERR ("Video Initializing, getting functions from DLL");
      return false;
  }

  LOG ("Transfering info to DLL. DeviceNumber %i.",VIDEO_DeviceNum);
  for (DWORD i=0; i<VIDEO_DeviceNum; i++)
  {  
		DWORD k=0;  
    SETUP_AddDriver (VIDEO_DeviceDesc[i].desc);
    for (DWORD j=0; j<VIDEO_ModesNum[i]; j++)
		{			
			if ((VIDEO_ModeDesc[i][j].width==320) && (VIDEO_ModeDesc[i][j].height==240))
			{
				k++;
				SETUP_AddResolution (VIDEO_ModeDesc[i][j].width,VIDEO_ModeDesc[i][j].height,VIDEO_ModeDesc[i][j].bpp);
			}
		}
		if (k==0)
			SETUP_AddResolution (0,0,0);
  }  
  
  LOG ("Calling DLL functions");
  if (!SETUP_Init(driver,width,height,bpp,!TestBit(flags,VIDEO_WINDOW),false,true,MMXInit))
  {
    LOG ("CANCELED Display Mode Selection");      
    return false;
  }

  driver = SETUP_GetDriver();
  SETUP_GetResolution(width,height,bpp);
  if (!SETUP_GetFullScreen()) flags = VIDEO_WINDOW; else flags = 0;
	if (width==0)
	{
		flags = VIDEO_WINDOW;
	}
	if (TestBit (flags,VIDEO_WINDOW))
	{
		width = 320;
		height = 240;
	}
  ELISIUM_Sound = SETUP_GetSound();
  ELISIUM_Looped = SETUP_GetLooped();
  ELISIUM_SoundFlags = SETUP_GetSoundFlags();
  ELISIUM_SoundQuality = SETUP_GetSoundQuality();
  ELISIUM_MMX = SETUP_GetMMX();
  ELISIUM_Priority = SETUP_GetPriority();

  DLL_Close(hDLL);

  SEC_END();

  return true;
}

BOOL WINAPI DDEnumCallbackEx(GUID FAR *lpGUID,LPSTR desc,LPSTR name,LPVOID lpContext,HMONITOR  hm)        
{ 
  TDDDevices *d = &VIDEO_DeviceDesc[VIDEO_DeviceNum];
  if (!lpGUID) ZeroMemory (&d->guid,sizeof(GUID));
  else d->guid = *lpGUID;
  strcpy (d->desc,desc);
  strcpy (d->name,name);
  LOG ("Device[%2i] %s",VIDEO_DeviceNum,name);
  VIDEO_DeviceNum++;
  return DDENUMRET_OK;
}
 
HRESULT WINAPI EnumModesCallback(LPDDSURFACEDESC2 surf,LPVOID lpContext)
{
  DWORD i = *((DWORD *)lpContext);
  TDDModes *m = &VIDEO_ModeDesc[i][VIDEO_ModesNum[i]];
  m->width = surf->dwWidth;
  m->height = surf->dwHeight;
  m->bpp = (BYTE)surf->ddpfPixelFormat.dwRGBBitCount;
  VIDEO_ModesNum[i]++;
  return DDENUMRET_OK;
}
 
bool VIDEO_SetUp (DWORD width,DWORD height,BYTE bpp,DWORD flags)
{
DWORD driver=0;

  SEC_START("Video Init");

  if TestBit(flags,VIDEO_MENU)
  {
    SEC_START("Video devices");
    VIDEO_DeviceNum = 0;
    DirectDrawEnumerateEx (DDEnumCallbackEx,NULL,DDENUM_ATTACHEDSECONDARYDEVICES | DDENUM_DETACHEDSECONDARYDEVICES | DDENUM_NONDISPLAYDEVICES);
    SEC_END();
    if (VIDEO_DeviceNum==0)
    {
      ERR ("Device count=0");
      return false;
    }
     
    LPDIRECTDRAW dd;   
    for (DWORD i=0; i<VIDEO_DeviceNum; i++)
    {
      DirectDrawCreate (&VIDEO_DeviceDesc[i].guid,&dd,NULL);
      dd->QueryInterface (IID_IDirectDraw4,(void **)&VIDEO_DD);
      dd->Release();
      dd = NULL;
      VIDEO_ModesNum[i]=0;
      VIDEO_DD->EnumDisplayModes (0,NULL,&i,EnumModesCallback);
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
    }
    if (!VIDEO_DefaultSetupVideoMode(width,height,bpp,flags,driver)) return false;    
    LOG ("Setup accepted! (Width %i Height %i Bpp %i Flags (%i)",width,height,bpp,flags);    
  } else {
    driver = 0;
  }

  VIDEO_ScreenWidth = width;
  VIDEO_ScreenHeight = height;  
  VIDEO_ScreenBpp = bpp;
  VIDEO_ScreenDriver = driver;
  VIDEO_Windowed = TestBit(flags,VIDEO_WINDOW);

  SEC_END();
  return true;
}

bool VIDEO_Init()
{  
  if (VIDEO_Windowed)   
  {        
    VIDEO_EraseBackGround = true;

    LPDIRECTDRAW dd;
    if (VIDEO_DeviceDesc[VIDEO_ScreenDriver].guid.Data1==0)
      hr = DirectDrawCreate (NULL,&dd,NULL);
    else
      hr = DirectDrawCreate (&VIDEO_DeviceDesc[VIDEO_ScreenDriver].guid,&dd,NULL);
    if (ERRDX ("DirectDrawCreate")) return false;
    hr = dd->QueryInterface (IID_IDirectDraw4,(void **)&VIDEO_DD);
    dd->Release();
    dd = NULL;
    if (ERRDX ("DirectDrawCreate")) return false;

    hr = VIDEO_DD->SetCooperativeLevel (NULL,DDSCL_NORMAL);
    if (ERRDX ("SetCooperativeLevel")) 
    {
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;    
    }
    DDSURFACEDESC2 ddsd;
    ZeroMemory (&ddsd,sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    hr = VIDEO_DD->CreateSurface(&ddsd,&VIDEO_Primary,NULL);
    if (ERRDX ("CreateSurface Primary")) 
    {
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;    
    }

    ZeroMemory (&ddsd,sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.dwWidth = VIDEO_ScreenWidth;
    ddsd.dwHeight = VIDEO_ScreenHeight;   
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    hr = VIDEO_DD->CreateSurface(&ddsd,&VIDEO_BackBuffer,NULL);
    if (ERRDX ("CreateSurface BackBuffer")) 
    {
      VIDEO_Primary->Release();
      VIDEO_Primary = NULL;
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    hr = VIDEO_DD->CreateClipper (0,&VIDEO_Clipper,NULL);
    if (ERRDX ("CreateClipper")) 
    {
      VIDEO_BackBuffer->Release();
      VIDEO_BackBuffer = NULL;
      VIDEO_Primary->Release();
      VIDEO_Primary = NULL;
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    VIDEO_Clipper->SetHWnd (0,HWnd);
    VIDEO_Primary->SetClipper (VIDEO_Clipper);

    RECT r,rs;
    GetWindowRect (HWnd,&rs);
    GetClientRect (HWnd,&r);
    long w = (rs.right-rs.left)-(r.right-r.left);
    long h = (rs.bottom-rs.top)-(r.bottom-r.top);
    SetWindowPos (HWnd,NULL,0,0,VIDEO_ScreenWidth+w,VIDEO_ScreenHeight+h,SWP_NOZORDER | SWP_NOMOVE);    
    VIDEO_EraseBackGround = false;
  } else {
    VIDEO_EraseBackGround = true;

    LPDIRECTDRAW dd;
    hr = DirectDrawCreate (&VIDEO_DeviceDesc[VIDEO_ScreenDriver].guid,&dd,NULL);
    if (ERRDX ("DirectDrawCreate")) return false;
    hr = dd->QueryInterface (IID_IDirectDraw4,(void **)&VIDEO_DD);
    dd->Release();
    dd = NULL;
    if (ERRDX ("DirectDrawCreate")) return false;

    hr = VIDEO_DD->SetCooperativeLevel (HWnd,DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
    if (ERRDX ("SetCooperativeLevel")) 
    {
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    hr = VIDEO_DD->SetDisplayMode (VIDEO_ScreenWidth,VIDEO_ScreenHeight,VIDEO_ScreenBpp,0,0);
    if (ERRDX ("SetDisplayMode")) 
    {
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    DDSURFACEDESC2 ddsd;
    ZeroMemory (&ddsd,sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 2;
    hr = VIDEO_DD->CreateSurface(&ddsd,&VIDEO_Primary,NULL);
    if (ERRDX ("CreateSurface Primary")) 
    {
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    DDSCAPS2 ddscaps;
    ZeroMemory (&ddscaps,sizeof(DDSCAPS2));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    hr = VIDEO_Primary->GetAttachedSurface (&ddscaps,&VIDEO_BackBuffer);
    if (ERRDX ("Getting BackBuffer")) 
    {
      VIDEO_Primary->Release();
      VIDEO_Primary = NULL;
      VIDEO_DD->Release();
      VIDEO_DD = NULL;
      return false;        
    }

    VIDEO_EraseBackGround = false;
  }

  return true;
}

void VIDEO_Close ()
{  
  if (VIDEO_Windowed)
  { 
    VIDEO_Primary->Release();
    VIDEO_Primary=NULL;
    VIDEO_BackBuffer->Release();
    VIDEO_BackBuffer=NULL;
    VIDEO_Clipper->Release();    
    VIDEO_Clipper=NULL;
    VIDEO_DD->Release();
    VIDEO_DD=NULL;
  } else {
    VIDEO_Primary->Release();
    VIDEO_Primary=NULL;
    VIDEO_DD->Release();
    VIDEO_DD=NULL;
  }  
}

RECT VIDEO_RectOriginal;

void VIDEO_SaveWindow (float tick)
{
  if (!IsIconic (HWnd))
		if (VIDEO_Windowed)
			GetWindowRect (HWnd,&VIDEO_RectOriginal);
}

void VIDEO_RestoreWindow (float tick)
{
  if (!IsIconic (HWnd))
		if (VIDEO_Windowed)
			SetWindowPos(HWnd,NULL,VIDEO_RectOriginal.left,VIDEO_RectOriginal.top,
						VIDEO_RectOriginal.right-VIDEO_RectOriginal.left,
						VIDEO_RectOriginal.bottom-VIDEO_RectOriginal.top,SWP_NOZORDER);
}

void VIDEO_Boom (float tick)
{
  if (!IsIconic (HWnd))
	{
		if (VIDEO_Windowed)
		{
			RECT r;
			r.left = VIDEO_RectOriginal.left-32*sin(tick*M_PIx2);
			r.top = VIDEO_RectOriginal.top-32*sin(tick*M_PIx2);
			r.right = VIDEO_RectOriginal.right+32*sin(tick*M_PIx2);
			r.bottom = VIDEO_RectOriginal.bottom+32*sin(tick*M_PIx2);
			SetWindowPos (HWnd,NULL,r.left,r.top,r.right-r.left,r.bottom-r.top,SWP_NOZORDER);
		}	
	}
}

void VIDEO_Guio ()
{
	GUIO_AddFN (VIDEO_Boom,"VIDEO_Boom");
	GUIO_AddFN (VIDEO_SaveWindow,"VIDEO_SaveWindow");
	GUIO_AddFN (VIDEO_RestoreWindow,"VIDEO_RestoreWindow");
}
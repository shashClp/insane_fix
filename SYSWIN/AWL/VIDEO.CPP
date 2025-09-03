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
#include <gl/gl.h>

bool VIDEO_Windowed;
bool VIDEO_EraseBackGround=true;
DWORD VIDEO_ScreenWidth;
DWORD VIDEO_ScreenHeight;
BYTE VIDEO_ScreenBpp;
extern DWORD *GBL_Bits;

HDC hDC;
HGLRC hRC;

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

unsigned int texID = -1;
void VIDEO_NotifyResize (DWORD width, DWORD height)
{
    if (width != 0)
        VIDEO_ScreenWidth  = width;

    if (height != 0)
        VIDEO_ScreenHeight = height;
}

void VIDEO_Flip()
{
    const unsigned int srcWidth = 320;
    const unsigned int srcHeight = 240;

    unsigned int dstWidth  = VIDEO_ScreenWidth;
    unsigned int dstHeight = VIDEO_ScreenHeight;
    unsigned int offsetX   = 0;
    unsigned int offsetY   = 0;

    if (ELISIUM_OriginalRatio)
    {
        // First thing, look for main axis
        float srcRatio = srcWidth / (float)srcHeight;
        float dstRatio = dstWidth / (float)dstHeight;

        // Destination wider than source
        if (dstRatio > srcRatio)
        {
            dstWidth = (unsigned int)floorf( (VIDEO_ScreenHeight/(float)srcHeight)*srcWidth + 0.5f );
            dstWidth = dstWidth > VIDEO_ScreenWidth ? VIDEO_ScreenWidth : dstWidth;
            offsetX  = (VIDEO_ScreenWidth-dstWidth)>>1;
        }
        // Destination taller than source
        else
        {            
            dstHeight = (unsigned int)floorf( (VIDEO_ScreenWidth/(float)srcWidth)*srcHeight + 0.5f );
            dstHeight = dstHeight > VIDEO_ScreenHeight ? VIDEO_ScreenHeight : dstHeight;
            offsetY   = (VIDEO_ScreenHeight-dstHeight)>>1;
        }
    }

    glViewport(offsetX, offsetY, dstWidth, dstHeight);
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glBindTexture	(GL_TEXTURE_2D, texID);
	glTexImage2D	(GL_TEXTURE_2D, 0, 4, 320, 240, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, GBL_Bits);


	glColor3f(1.f, 1.f, 1.0f);
	glBegin (GL_QUADS);
		glTexCoord2f(0.f, 1.f);
		glVertex2f (-1.f,-1.f);

		glTexCoord2f(0.f, 0.f);
		glVertex2f (-1.f, 1.f);

		glTexCoord2f(1.f, 0.f);
		glVertex2f  (1.f, 1.f);

		glTexCoord2f(1.f, 1.f);
		glVertex2f ( 1.f, -1.f);
	glEnd ();
	
	SwapBuffers (hDC);
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
  LPSETUPGetB SETUP_GetOriginalRatio;   // shash: This points to GetSound, resource hack to get stretch support
  LPSETUPGetB SETUP_GetBilinear;        // shash: This points to GetLooped, it's a resource hack to be able to to have a bilinear option
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
      (!DLL_Call (hDLL,(void **)&SETUP_GetBilinear,"SETUP_GetLooped")) ||
      (!DLL_Call (hDLL,(void **)&SETUP_GetOriginalRatio,"SETUP_GetSound")) ||
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
			if (VIDEO_ModeDesc[i][j].bpp == 32)
			{
				k++;
				SETUP_AddResolution (VIDEO_ModeDesc[i][j].width,VIDEO_ModeDesc[i][j].height,VIDEO_ModeDesc[i][j].bpp);
			}
		}
		if (k==0)
			SETUP_AddResolution (1024, 768, 32);
  }

  LOG ("Calling DLL functions");
  if (!SETUP_Init(driver,width,height,bpp,!TestBit(flags,VIDEO_WINDOW),true,true,MMXInit))
  {
    LOG ("CANCELED Display Mode Selection");      
    return false;
  }

  driver = SETUP_GetDriver();
  SETUP_GetResolution(width,height,bpp);
  if (!SETUP_GetFullScreen()) flags = VIDEO_WINDOW; else flags = 0;

  ELISIUM_OriginalRatio = SETUP_GetOriginalRatio();
  ELISIUM_Bilinear = SETUP_GetBilinear();
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
	LPDIRECTDRAW4 VIDEO_DD;
	DWORD driver=0;

  SEC_START("Video Init");

  if (TestBit(flags,VIDEO_MENU))
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
  //VIDEO_ScreenDriver = driver;
  VIDEO_Windowed = TestBit(flags,VIDEO_WINDOW);

  SEC_END();
  return true;
}

// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		VIDEO_ScreenBpp,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		24,                                     // Size of depth buffer
		0,0,
		PFD_MAIN_PLANE,
		0,0,0,0 };

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (SetPixelFormat(hDC, nPixelFormat, &pfd) == FALSE)
	{
        MessageBox(NULL, "Failed to set up pixel format, aborting", "Error", MB_ICONERROR);
        ExitProcess(0);
	}
}

bool VIDEO_Init ()
{  
	hDC = GetDC(HWnd);

	SetDCPixelFormat(hDC);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	
	glMatrixMode	(GL_PROJECTION);
	glLoadIdentity	();

	glMatrixMode	(GL_MODELVIEW);
	glLoadIdentity	();	

	glEnable		(GL_TEXTURE_2D);
	glGenTextures	(1, &texID);
	glBindTexture	(GL_TEXTURE_2D, texID);

    if (ELISIUM_Bilinear)
    {
	    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    }
    else
    {
	    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }

	glClearColor(0.f, 0.f, 0.f, 0.f);

	return true;
}

void VIDEO_Close ()
{  
	ReleaseDC (HWnd, hDC);
	wglMakeCurrent(hDC,NULL);
	wglDeleteContext(hRC);

	ShowCursor(TRUE);
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
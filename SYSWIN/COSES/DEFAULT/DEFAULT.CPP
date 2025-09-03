// Default initializations in a simple program

#define DEFAULT_CAPTION              "InSaNe by Anaconda"
#define DEFAULT_SCROLLTEXT           "Anaconda 2000: Bpm/Cimedia/Elric/Hlod-Wig/Klauz/Nocturnus. You're watching InSaNe Demo...an Anaconda production."
#define DEFAULT_SCROLLTEXT_TIMER     80
#define DEFAULT_SCROLLTEXT_SIZE      24
//#define DEFAULT_ZBUFFER

#include <windows.h>
#include <video.h>
#include <view.h>
#include <view3d.h>
#include <viewz.h>
#include <order.h>
#include <triclip.h>
#include <transbuff.h>
#include <debug.h>
#include <elisium.h>
#include <awm.h>
#include <texture.h>
#include <concat.h>
#include <profile.h>
#include <framerate.h>
#include <syswin.h>

TView *View;
TView3D *View3D;
TViewZ *ViewZ;
TAW3D_TriangleClipper *TC;
TAW3D_BufferZOrder *BZO;
TAW3D_TransformationBuffer *TB;

DWORD DEFAULT_TextTimer=0;
int DEFAULT_TextPos=-DEFAULT_SCROLLTEXT_SIZE;
char *DEFAULT_Text = DEFAULT_SCROLLTEXT;
bool DEFAULT_Minimized = true;

void DEFAULT_Flip()
{  
  if (IsIconic(HWnd)) 
  {
    DEFAULT_Minimized = true;
    // Scroll
    DWORD t = GetTickCount();
    if ((t-DEFAULT_TextTimer)>=DEFAULT_SCROLLTEXT_TIMER)
    {
      DEFAULT_TextTimer=t;      
      if (++DEFAULT_TextPos>(int)strlen(DEFAULT_Text)) 
        DEFAULT_TextPos=-DEFAULT_SCROLLTEXT_SIZE;
    }    
    char Caracters[DEFAULT_SCROLLTEXT_SIZE+1];    
    Caracters[DEFAULT_SCROLLTEXT_SIZE]=0;
    int k = DEFAULT_TextPos;
    for (DWORD i=0; i<DEFAULT_SCROLLTEXT_SIZE; i++,k++)
    {
      if ((k>=0) && (k<(int)strlen(DEFAULT_Text))) 
        Caracters[i]=DEFAULT_Text[k]; else Caracters[i]=' ';      
    }    
    SetWindowText (HWnd,Caracters);
    // Flip
    HICON icon;
    icon = (HICON)GetClassLong (HWnd,GCL_HICON);
    if (icon) DeleteObject (icon);
    icon = VIEW_FlipMinimized(); 
    SetClassLong (HWnd,GCL_HICON,(int)icon);
  } else {
    if (DEFAULT_Minimized) 
    {
      SetWindowText (HWnd,DEFAULT_CAPTION);
      DEFAULT_Minimized = false;
    }
    // Flip
    PBEGIN(0)
    //VIEW_Flip();    
    PEND
  }    

  PBEGIN(1)
  VIDEO_Flip();
  PEND

  PROFILE_End();  
  PROFILE_Paint();    
  FRAMERATE_Paint();
  PROFILE_Begin();
}

bool DEFAULT_Init()
{
  if (!VIDEO_Init()) return false;

  // shash: Hack to force to 320x240
  DWORD VIDEO_ScreenWidth = 320;
  DWORD VIDEO_ScreenHeight = 240;

  if ((View = VIEW_Init (VIDEO_ScreenWidth,VIDEO_ScreenHeight,VIDEO_ScreenWidth/2,VIDEO_ScreenHeight/2,VIDEO_ScreenWidth,NULL))==NULL) return false;
  if ((View3D = VIEW3D_Init (VIDEO_ScreenWidth-2,VIDEO_ScreenHeight-40,(float)VIDEO_ScreenWidth/2,(float)VIDEO_ScreenHeight/2,10.0f,10000.0f,60.0f,1.0f))==NULL) return false;

  TC = new TAW3D_TriangleClipper(NULL,NULL,NULL,NULL);
  BZO = new TAW3D_BufferZOrder (10000,1000000);
  TB = new TAW3D_TransformationBuffer (10000,3);

  VIEW_Set(View);
  VIEW3D_Set(View3D);
#ifdef DEFAULT_ZBUFFER
  if ((ViewZ = VIEWZ_Init (VIDEO_ScreenWidth,VIDEO_ScreenHeight,VIDEO_ScreenWidth,NULL))==NULL) return false;
  VIEWZ_Set(ViewZ);
#endif
  
  MUSIC_Init(ELISIUM_SoundQuality, ELISIUM_SoundFlags);
  CONCAT_Init("insane.adf",true);
  FRAMERATE_Init();
  PROFILE_Init(16);
  TXT_Init(10);

  PROFILE_DefineSection (0,"Flip from Memory",0x7f7f7f);
  PROFILE_DefineSection (1,"Flip from Video",0xffffff);

  return true;
}

void DEFAULT_Close()
{
	MUSIC_Close();  
  TXT_Close();
  PROFILE_Close();  
  CONCAT_Close();

  delete TB;
  delete TC;
  delete BZO;
  FreeMem (View);
  FreeMem (View3D);
#ifdef DEFAULT_ZBUFFER
  FreeMem (ViewZ);  
#endif
  VIDEO_Close();
}

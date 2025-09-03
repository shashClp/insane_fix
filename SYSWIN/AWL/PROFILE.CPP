// AWL - Anaconda Windows Library
// Profile routines

#include <profile.h>
#include <log.h>
#include <mem.h>
#include <video.h>
#include <syswin.h>
#include <misc.h>

TProfileSection *PROFILE_sections;
DWORD PROFILE_seccount=0;
DWORD PROFILE_start;
DWORD PROFILE_end;
DWORD PROFILE_desc;
bool PROFILE_beginned=false;
bool PROFILE_active=false;
DWORD PROFILE_totalticks;
DWORD PROFILE_num;

#ifdef PROFILE_Profiles
inline void PROFILE_GetTicks(DWORD *dest)
{
  _asm {
    rdtsc
    mov edx,dest
    mov [edx],eax
  }
}
#endif

void PROFILE_Init(DWORD sections)
{
#ifdef PROFILE_Profiles
  LOG ("Initializing Profile");
  PROFILE_seccount = sections;
  PROFILE_sections = (TProfileSection *) GetMem (sizeof(TProfileSection)*sections);
  
  PROFILE_active = true;
  PROFILE_Begin();
  for (DWORD i=0; i<10000; i++)
  {     
    PROFILE_BeginSection(0);
    PROFILE_EndSection();
  }
  PROFILE_End();
  PROFILE_active = false;
  PROFILE_beginned=false;
  PROFILE_desc = (PROFILE_sections[0].ticks/10000)+3;
  for (DWORD i=0; i<PROFILE_seccount; i++)
  {
    strcpy (PROFILE_sections[i].name,"Undefined!");
  }
#endif
}

void PROFILE_Close()
{
#ifdef PROFILE_Profiles
  LOG ("Closing Profile");
  if (PROFILE_sections)
  {
    FreeMem(PROFILE_sections);
    PROFILE_sections = NULL;
    PROFILE_seccount = 0;
  }
#endif
}

void PROFILE_Begin()
{
#ifdef PROFILE_Profiles
  if (!PROFILE_active) return;
  for (DWORD i=0; i<PROFILE_seccount; i++)
  {
    PROFILE_sections[i].ticks = 0;
    PROFILE_sections[i].count = 0;
  }
  PROFILE_beginned = true;
  PROFILE_GetTicks(&PROFILE_start);   
#endif
}

void PROFILE_End()
{
#ifdef PROFILE_Profiles
  if (!PROFILE_active) return;
  PROFILE_GetTicks(&PROFILE_end);  
#endif
}

void PROFILE_BeginSection(DWORD num)
{
#ifdef PROFILE_Profiles
  if (!PROFILE_active) return;
  if (num>=PROFILE_seccount) return;  
  PROFILE_num = num;
  PROFILE_GetTicks(&PROFILE_sections[num].start);  
#endif
}

void PROFILE_EndSection()
{  
#ifdef PROFILE_Profiles
  if (!PROFILE_active) return;  
  PROFILE_GetTicks(&PROFILE_sections[PROFILE_num].end);
  PROFILE_sections[PROFILE_num].ticks += PROFILE_sections[PROFILE_num].end-PROFILE_sections[PROFILE_num].start;
  PROFILE_sections[PROFILE_num].count++;
#endif
}

void PROFILE_DefineSection(DWORD num,char *name)
{
#ifdef PROFILE_Profiles
  DWORD r = rand()%256;
  DWORD g = rand()%256;
  DWORD b = rand()%256;
  PROFILE_DefineSection (num,name,(((r<<8)+g)<<8)+b);
#endif
}

void PROFILE_DefineSection(DWORD num,char *name,DWORD color)
{
#ifdef PROFILE_Profiles
  if (num>=PROFILE_seccount) return;
  PROFILE_sections[num].color = color;
  strcpy (PROFILE_sections[num].name,name);
#endif
}

void PROFILE_Paint()
{
#ifdef PROFILE_Profiles
HDC dc;
char s[256];
DWORD i,nl;
DWORD xx,yy,ww;
LOGBRUSH logbrush;
HBRUSH hb,hbold;
HPEN hp,hpold;

  if (IsIconic(HWnd)) return;
  if (!PROFILE_active) return;
  if (!PROFILE_beginned) return;
  PROFILE_beginned = false;
    
  RECT r;
  GetWindowRect (HWnd,&r);  
  xx=r.left;
  yy=r.top;  
  ww=r.right-r.left;
  if (VIDEO_Windowed)
  {
    xx+=3;
    yy+=23;    
    ww-=6;
  }  

  int desc=0;  
  for (i=0; i<PROFILE_seccount; i++)
    desc+=PROFILE_desc*PROFILE_sections[i].count;   
  PROFILE_totalticks = (PROFILE_end-PROFILE_start)-desc;

  float tp1 = 1.0f/(float)PROFILE_totalticks;
  float tp100 = tp1*100.0f;   
  float w = (float)ww*tp1;

  hr = VIDEO_Primary->GetDC(&dc);
  if (hr==DDERR_SURFACELOST) return;
  else if (ERRDX("Getting DC in PROFILE_Paint")) return;
  
  hp = CreatePen (PS_SOLID,1,0);
  hpold = (HPEN)SelectObject (dc,hp);

  logbrush.lbStyle = BS_NULL;
  hb = CreateBrushIndirect (&logbrush);
  hbold = (HBRUSH)SelectObject (dc,hb);
  Rectangle (dc,xx,yy,xx+ww,yy+20);
  SelectObject (dc,hbold);
  DeleteObject (hb);
  
  float tpt = 0;   
  float x = (float)xx;
  float y = (float)yy+30;
  r.top = yy+1;
  r.bottom = yy+19;  
  for (i=0; i<PROFILE_seccount; i++)
  {
    if (PROFILE_sections[i].count!=0)
    {
      float t = (float)(PROFILE_sections[i].ticks-(PROFILE_desc*PROFILE_sections[i].count));
      float tp = ((float)t)/PROFILE_sections[i].count;
      t = tp*w;
      r.left = (DWORD)x;
      r.right = (DWORD)(x+t);
      hb = CreateSolidBrush (PROFILE_sections[i].color);
      FillRect (dc,&r,hb);
      DeleteObject (hb);

      logbrush.lbStyle = BS_SOLID;
      logbrush.lbColor = PROFILE_sections[i].color;
      hb = CreateBrushIndirect (&logbrush);
      hbold = (HBRUSH)SelectObject (dc,hb);
      Rectangle (dc,(int)xx+2,(int)y,(int)xx+18,(int)y+16);
      SelectObject (dc,hbold);
      DeleteObject (hb);

      sprintf (s,"[T:%i6] [%3.2f%%] %s",(int)(tp/1000.0),tp100*tp,PROFILE_sections[i].name);
      nl = strlen(s);

      SetBkMode (dc,TRANSPARENT);
      SetTextColor (dc,0xffffff);      
      TextOut (dc,(int)xx+20,(int)y+1,s,nl);
      TextOut (dc,(int)xx+20,(int)y-1,s,nl);
      TextOut (dc,(int)xx+22,(int)y+1,s,nl);
      TextOut (dc,(int)xx+22,(int)y-1,s,nl);
      SetTextColor (dc,0);
      TextOut (dc,(int)xx+21,(int)y,s,nl);

      tpt+=tp100*tp;
      y+=16;
      x+=t;      
    }
  }  

  sprintf (s,"[Total: %3.2f%%]",tpt);
  nl = strlen(s);

  SetBkMode (dc,TRANSPARENT);
  SetTextColor (dc,0xffffff);      
  TextOut (dc,(int)xx+20,(int)y+1,s,nl);
  TextOut (dc,(int)xx+20,(int)y-1,s,nl);
  TextOut (dc,(int)xx+22,(int)y+1,s,nl);
  TextOut (dc,(int)xx+22,(int)y-1,s,nl);
  SetTextColor (dc,0x7f0000);
  TextOut (dc,(int)xx+21,(int)y,s,nl);

  SelectObject (dc,hpold);
  DeleteObject (hp);
  
  hr = VIDEO_Primary->ReleaseDC(dc);
  if (hr==DDERR_SURFACELOST) return;
  else if (ERRDX("Releasing DC in PROFILE_Paint")) return;
#endif
}

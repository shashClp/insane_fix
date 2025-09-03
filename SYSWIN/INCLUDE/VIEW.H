// AWL - Anaconda Windows Library
// Multiple View library

#ifndef AWL_VIEW
#define AWL_VIEW

#define VIEW_SHAREDMEM          1
#define VIEW_FIRSTGETMEM        2

#include <windows.h>

typedef struct {    
  DWORD flags;
  DWORD width;
  DWORD height;
  DWORD pitch;
  DWORD centerx;
  DWORD centery;  
  DWORD *bits;
} TView;

extern DWORD GBL_Size;
extern DWORD GBL_Size4;
extern DWORD GBL_Width;
extern DWORD GBL_Height;
extern DWORD GBL_Pitch;
extern DWORD GBL_Pitch4;
extern DWORD *GBL_Bits;
extern DWORD GBL_CenterX;
extern DWORD GBL_CenterY;

TView *VIEW_Init (DWORD width=GBL_Width,DWORD height=GBL_Height,DWORD cx=GBL_Width/2,DWORD cy=GBL_Height/2,DWORD pitch=GBL_Pitch,DWORD *bits=NULL);
void FreeMem (TView *view);
void VIEW_Set (TView *view);
//void VIEW_Flip ();
void VIEW_Clear ();
HICON VIEW_FlipMinimized();

#endif
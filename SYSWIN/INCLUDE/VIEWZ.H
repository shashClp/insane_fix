// AW3D - Anaconda 3D Windows Library
// Multiple ZBuffer View library

#ifndef AW3D_VIEWZ
#define AW3D_VIEWZ

#include <windows.h>

#define VIEWZ_SHAREDMEM          1
#define VIEWZ_FIRSTGETMEM        2

typedef struct {    
  DWORD flags;
  DWORD width;
  DWORD height;
  DWORD pitch;
  float *bits;
} TViewZ;

extern float *GBLZ_Bits;
extern DWORD GBLZ_Width;
extern DWORD GBLZ_Height;
extern DWORD GBLZ_Pitch;
extern DWORD GBLZ_Pitch4;

extern TViewZ *VIEWZ_Init (DWORD width,DWORD height,DWORD pitch,float *bits);
extern void FreeMem (TViewZ *view);
extern bool VIEWZ_Set (TViewZ *view);
extern bool VIEWZ_Clear ();   
extern bool VIEWZ_FastClear ();   

#endif
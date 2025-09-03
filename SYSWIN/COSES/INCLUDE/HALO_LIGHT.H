// Anaconda
// by Elric

#ifndef MISC_HALO_LIGHT
#define MISC_HALO_LIGHT

#include <windows.h>
#include <view.h>

extern BYTE HALO_LIGHT_Alpha;

void HALO_LIGHT_Render (DWORD x,DWORD y,TView *dst);
void HALO_LIGHT_RenderReverse (DWORD x,DWORD y,TView *dst);
void HALO_LIGHT_Line (long x1,long y1,long x2,long y2,TView *dst,long a);

void HALO_LIGHT_Render320x240 (DWORD x,DWORD y,DWORD *dst,DWORD *src);

#endif

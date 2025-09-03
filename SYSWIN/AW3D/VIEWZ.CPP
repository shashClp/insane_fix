// AW3D - Anaconda Windows Library
// Multiple ZBuffer View library

#include <windows.h>
#include <math.h>
#include <log.h>
#include <mem.h>
#include <defines.h>
#include <viewz.h>

float *GBLZ_Bits;
DWORD GBLZ_Width;
DWORD GBLZ_Height;
DWORD GBLZ_Pitch;
DWORD GBLZ_Pitch4;

TViewZ *VIEWZ_Init (DWORD width,DWORD height,DWORD pitch,float *bits)                      
{
  SEC_START ("Initializing a Z View!");
  TViewZ *view;  
  
  view = (TViewZ *) GetMem (sizeof(TViewZ));
  if (view==NULL) 
  {
    ERR ("Noth enough memory to make a Z view");
    return NULL;
  }
  if (bits) 
  {
    view->bits = bits;
    view->flags = VIEWZ_SHAREDMEM;
    LOG ("Shared Mem");
  } else {
    view->bits = (float *) GetMem (width*height*4);
    if (view->bits==NULL) 
    {
      ERR ("Not enough memory to get a Z buffer bits");
      return NULL;
    }
    view->flags = VIEWZ_FIRSTGETMEM;
    LOG ("First GetMem");
  }

  view->width = width;
  view->height = height;
  view->pitch = pitch;
  LOG ("Width: %i Height: %i Pitch: %i Pointer: @%p",width,height,pitch,view->bits);
  
  SEC_END ();
  return view;
}

bool VIEWZ_Clear ()
{
  float *v = GBLZ_Bits;
  for (DWORD i=0; i<GBLZ_Height; i++)
  {
    memset (v,M_ZeroFloat2Int,GBLZ_Width*4);
    v+=GBLZ_Pitch;
  }
  return true;
}

bool VIEWZ_FastClear ()
{
  memset (GBLZ_Bits,M_ZeroFloat2Int,GBLZ_Pitch4*GBLZ_Height);
  return true;
}


void FreeMem (TViewZ *view)
{  
  if ((view->flags & VIEWZ_SHAREDMEM)!=0) return;
  LOG ("Freeing Z View");
  FreeMem ((void *)view->bits);  
}

bool VIEWZ_Set (TViewZ *view)
{
  GBLZ_Width = view->width;
  GBLZ_Height = view->height;
  GBLZ_Pitch = view->pitch;
  GBLZ_Pitch4 = GBLZ_Pitch*4;
  GBLZ_Bits = view->bits;
  return true;
}


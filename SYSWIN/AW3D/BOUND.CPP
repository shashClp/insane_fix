// AW3D - Anaconda 3D Library
// Bound

#include <windows.h>
#include <bound.h>

long BOUND_ClipToFrustrum (TBound &a,TFrustrum &f)
{
DWORD clip;
bool AllIn,AllOut;

  clip = 0;
  AllIn = AllOut = true;
  for (DWORD i=0; i<f.count; i++)
  {
    float d = PLA_Distance (f.plane[i],a.org);
    if (d>-a.dist) AllOut=false;
    if (d<a.dist) { AllIn=false; clip |= 1<<i; }
  }
  if (AllIn) return -1;
  if (AllOut) return 0;
  return clip;
}

void BOUND_Set (TBound &a,TVertex &b,float c)
{
  a.org = b;
  a.dist = c;
}

void BOUND_CalcFromVertex (TBound &b,TVertex *v,DWORD count)
{
  TVertex Max,Min;

  VTX_Set(Max,-0x100000,-0x100000,-0x100000);
  VTX_Set(Min,0x100000,0x100000,0x100000);
  for (DWORD i=0; i<count; i++)
  {
    if (v->x<Min.x) Min.x = v->x;
    if (v->y<Min.y) Min.y = v->y;
    if (v->z<Min.z) Min.z = v->z;
    if (v->x>Max.x) Max.x = v->x;
    if (v->y>Max.y) Max.y = v->y;
    if (v->z>Max.z) Max.z = v->z;
    v++;
  }  
  VTX_Between (b.org,Max,Min,0.5f);
  b.dist = VTX_Distance (b.org,Max);
}


// AW3D - Anaconda 3D Library
// Bound

#ifndef AW3D_BOUND
#define AW3D_BOUND

#include <windows.h>
#include <vertex.h>
#include <frustrum.h>

typedef struct {
  TVertex org;
  float dist;
} TBound;

long BOUND_ClipToFrustrum (TBound &a,TFrustrum &f);
void BOUND_Set (TBound &a,TVertex &b,float c);
void BOUND_CalcFromVertex (TBound &b,TVertex *v,DWORD count);

#endif
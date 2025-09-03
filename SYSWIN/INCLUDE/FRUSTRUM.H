// AW3D - Anaconda 3D Windows Library
// Frustrum

#ifndef AW3D_FRUSTRUM
#define AW3D_FRUSTRUM

#include <windows.h>
#include <vertex.h>
#include <plane.h>

typedef struct {
  DWORD count;
  TVertex org;
  TPlane plane[6];
} TFrustrum;

#endif
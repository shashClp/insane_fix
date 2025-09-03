// AW3D - Anaconda Windows Library
// Multiple 3D View library

#ifndef AW3D_VIEW
#define AW3D_VIEW

#include <windows.h>
#include <vertex.h>
#include <matrix.h>
#include <frustrum.h>

typedef struct {
  float factorx;
  float factory;
  TVertex cono[8];
  float fov;  
  float planedist;
  float width;
  float height;
  float centerx;
  float centery;  
  float farz;
  float nearz;  
} TView3D;

extern float GBL3D_FactorX;
extern float GBL3D_FactorY;
extern float GBL3D_CenterX;
extern float GBL3D_CenterY;
extern float GBL3D_FarZ;
extern float GBL3D_NearZ;
extern TVertex GBL3D_Cono[8];


extern TView3D *VIEW3D_Init (DWORD width,DWORD height,float centerx,float centery,
                             float nz,float fz,float fov,float aspect);
extern TView3D *VIEW3D_Init (DWORD width,DWORD height,
                             float nz,float fz,float fov,float aspect);
extern void FreeMem (TView3D *view);
extern bool VIEW3D_Set (TView3D *view);
extern void VIEW3D_Project (TVertex &out,TVertex &in);
extern void VIEW3D_MakeFrustrum (TFrustrum &fru,TMatrix &a);
extern void VIEW3D_MakeFrustrumCamInv (TFrustrum &fru,TMatrix &imat);
extern void VIEW3D_ReInit (TView3D *v,DWORD width,DWORD height,
                           float centerx,float centery,
                           float nz,float fz,float fov,
                           float aspect);

#endif
// AW3D - Anaconda Windows Library
// Multiple 3D View library

#include <windows.h>
#include <math.h>
#include <log.h>
#include <mem.h>
#include <defines.h>
#include <view3d.h>
#include <frustrum.h>

float GBL3D_FactorX;
float GBL3D_FactorY;
float GBL3D_CenterX;
float GBL3D_CenterY;
float GBL3D_FarZ;
float GBL3D_NearZ;
TVertex GBL3D_Cono[8];

void VIEW3D_MakeFrustrum (TFrustrum &fru,TMatrix &a)
{
TMatrix imat;
TVertex vrot[8];

  MTX_Inverse (imat,a);
  for (DWORD i=0; i<8; i++) 
    MTX_Mul (vrot[i],GBL3D_Cono[i],imat);
  
  fru.count = 6;
  fru.org = vrot[0];
  PLA_CalcABCD (fru.plane[0],vrot[0],vrot[2],vrot[1]);
  PLA_CalcABCD (fru.plane[1],vrot[0],vrot[3],vrot[2]);
  PLA_CalcABCD (fru.plane[2],vrot[0],vrot[4],vrot[3]);
  PLA_CalcABCD (fru.plane[3],vrot[0],vrot[1],vrot[4]);

  PLA_CalcABCD (fru.plane[4],vrot[1],vrot[3],vrot[2]);
  PLA_CalcABCD (fru.plane[5],vrot[5],vrot[6],vrot[7]);  
}

void VIEW3D_MakeFrustrumCamInv (TFrustrum &fru,TMatrix &imat)
{
TVertex vrot[8];

  for (DWORD i=0; i<8; i++) MTX_Mul (vrot[i],GBL3D_Cono[i],imat);
  
  fru.count = 6;
  fru.org = vrot[0];
  PLA_CalcABCD (fru.plane[0],vrot[0],vrot[2],vrot[1]);
  PLA_CalcABCD (fru.plane[1],vrot[0],vrot[3],vrot[2]);
  PLA_CalcABCD (fru.plane[2],vrot[0],vrot[4],vrot[3]);
  PLA_CalcABCD (fru.plane[3],vrot[0],vrot[1],vrot[4]);

  PLA_CalcABCD (fru.plane[4],vrot[1],vrot[3],vrot[2]);
  PLA_CalcABCD (fru.plane[5],vrot[5],vrot[6],vrot[7]);  
}

void VIEW3D_Project (TVertex &out,TVertex &in)
{
  out.z = 1.0f/in.z;
  out.x = (in.x*GBL3D_FactorX*out.z)+GBL3D_CenterX;
  out.y = (-in.y*GBL3D_FactorY*out.z)+GBL3D_CenterY;
}

TView3D *VIEW3D_Init (DWORD width,DWORD height,
                      float nz,float fz,float fov,float aspect)
{
  return VIEW3D_Init (width,height,0.5f*(float)width,0.5f*(float)height,nz,fz,fov,aspect);
}

TView3D *VIEW3D_Init (DWORD width,DWORD height,float centerx,float centery,
                      float nz,float fz,float fov,float aspect)
{
  SEC_START ("Initializing a 3D View!");
	float angle = fov*M_ToRad*0.5f;

  TView3D *v = (TView3D *)GetMem (sizeof(TView3D));

  centerx -=0.5f;
  centery -=0.5f;
  v->width = (float)width;
  v->height = (float)height;
  v->fov = fov;  
  v->centerx = centerx;
  v->centery = centery;
  v->nearz = nz;
  v->farz = fz;

  LOG ("Width: %f Height: %f FOV: %f",v->width,v->height,v->fov);
	float x,y;
  float r = v->height/v->width; //(v->height/(v->width*aspect));
  float d = (float)tan(angle);
  v->factorx = v->width/(d*2.0f);
  v->factory = v->factorx*aspect;

  LOG ("Factor X: %f",v->factorx);
  LOG ("Aspect: %f",v->factory);

  LOG ("Generating Vertex Frustrum...");
  LOG ("NearZ: %f FarZ: %f",nz,fz); 
  VTX_Set (v->cono[0],0,0,0);  
  
  x = nz*(float)tan(angle);  
  y = (x*r)/aspect;

  VTX_Set (v->cono[1],-x,-y,nz);
  VTX_Set (v->cono[2],-x,+y,nz);
  VTX_Set (v->cono[3],+x,+y,nz);
  VTX_Set (v->cono[4],+x,-y,nz);

  x = fz*(float)tan(angle);  
  y = (x*r)/aspect;

  VTX_Set (v->cono[5],-x,-y,fz);
  VTX_Set (v->cono[6],-x,+y,fz);
  VTX_Set (v->cono[7],+x,+y,fz);

  LOG ("V1: (%f %f %f)",v->cono[0].x,v->cono[0].y,v->cono[0].z);
  LOG ("V2: (%f %f %f)",v->cono[1].x,v->cono[1].y,v->cono[1].z);
  LOG ("V3: (%f %f %f)",v->cono[2].x,v->cono[2].y,v->cono[2].z);
  LOG ("V4: (%f %f %f)",v->cono[3].x,v->cono[3].y,v->cono[3].z);
  LOG ("V5: (%f %f %f)",v->cono[4].x,v->cono[4].y,v->cono[4].z);
  LOG ("V6: (%f %f %f)",v->cono[5].x,v->cono[5].y,v->cono[5].z);
  LOG ("V7: (%f %f %f)",v->cono[6].x,v->cono[6].y,v->cono[6].z);
  LOG ("V8: (%f %f %f)",v->cono[7].x,v->cono[7].y,v->cono[7].z);

  SEC_END ();
  return v;
}

void VIEW3D_ReInit (TView3D *v,DWORD width,DWORD height,
                    float centerx,float centery,
                    float nz,float fz,float fov,
                    float aspect)
{  
	float angle = fov*M_ToRad*0.5f;
  centerx -=0.5f;
  centery -=0.5f;
  v->width = (float)width;
  v->height = (float)height;
  v->fov = fov;  
  v->centerx = centerx;
  v->centery = centery;
  v->nearz = nz;
  v->farz = fz;
	float x,y;
  float r = v->height/v->width; //(v->height/(v->width*aspect));
  float d = (float)tan(angle);
  v->factorx = v->width/(d*2.0f);
  v->factory = v->factorx*aspect;
  VTX_Set (v->cono[0],0,0,0);  
  x = nz*(float)tan(angle);  
  y = (x*r)/aspect;
  VTX_Set (v->cono[1],-x,-y,nz);
  VTX_Set (v->cono[2],-x,+y,nz);
  VTX_Set (v->cono[3],+x,+y,nz);
  VTX_Set (v->cono[4],+x,-y,nz);
  x = fz*(float)tan(angle);  
  y = (x*r)/aspect;
  VTX_Set (v->cono[5],-x,-y,fz);
  VTX_Set (v->cono[6],-x,+y,fz);
  VTX_Set (v->cono[7],+x,+y,fz);
}

void FreeMem (TView3D *view)
{
  LOG ("Freeing View3D");
  if (view)
  {
    FreeMem ((void *)view);    
    view = NULL;
  }
}

bool VIEW3D_Set (TView3D *view)
{  
  GBL3D_FactorX = view->factorx;
  GBL3D_FactorY = view->factory;
  GBL3D_CenterX = view->centerx;
  GBL3D_CenterY = view->centery;
  GBL3D_FarZ = view->farz;
  GBL3D_NearZ = view->nearz;

  GBL3D_Cono[0] = view->cono[0];
  GBL3D_Cono[1] = view->cono[1];
  GBL3D_Cono[2] = view->cono[2];
  GBL3D_Cono[3] = view->cono[3];
  GBL3D_Cono[4] = view->cono[4];
  GBL3D_Cono[5] = view->cono[5];
  GBL3D_Cono[6] = view->cono[6];
  GBL3D_Cono[7] = view->cono[7];

  return true;
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Sphere
#define SystemR_V30_Sphere
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
extern long SphPreCalc;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct {

  TObject_R General;

  TReal Radi;
  TReal Radi2;
  TReal CX;
  TReal CY;
  TReal CZ;
  TReal A;
  TReal B;
  TReal C;
  TReal TMP1;
  TReal RES;
  int   XMax,XMin,YMax,YMin;
  TReal Fact1;
  TReal INCV;
  TReal ICC;
}TSphere_R;

extern TSphere_R *ActiveSphere ;


extern void TSphere_R_Init  ( TReal P1 , TReal P2 , TReal P3 , TReal P7 , byte P8 , byte P9 );
extern void TSphere_R_GetInfo ( RayInfo &P1, TVertex &CH );
extern void TSphere_R_GetNormal (TVertex  &Pos,TVertex &Norm,TReal &ValNorm,long Inv);
extern void TSphere_R_SetValuesIntersection (TVertex  &Pos);
extern long TSphere_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1);
extern long TSphere_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1);
extern long TSphere_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir);
extern void DestroySphereDefault(TObject_R*M);
extern TObject_R *CreateSphereDefault();
#endif
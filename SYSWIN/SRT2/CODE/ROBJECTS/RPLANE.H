//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Plane
#define SystemR_V30_Plane
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct {

  TObject_R General;
  TVertex   Normal;
  TReal     D,TMP;
  rgbcol    *Texture2;
  word V2,U2,W2;
}TPlane_R;

extern TPlane_R *ActivePlane;


extern void TPlane_R_Init ( TReal P1,TReal P2,TReal P3,TReal P7,TReal P8,TReal P9,byte P10,byte P11 );
extern void TPlane_R_GetInfo ( RayInfo &P1 , TVertex &CH2 );
extern void TPlane_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv);
extern void TPlane_R_SetValuesIntersection (TVertex  &Pos);
extern long TPlane_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1);
extern long TPlane_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1);
extern long TPlane_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir);
extern void DestroyPlaneDefault(TObject_R*M);
extern TObject_R *CreatePlaneDefault();
#endif

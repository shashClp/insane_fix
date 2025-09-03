//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Blob
#define SystemR_V30_Blob
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#define BLOB_QUALITY 0.01
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
#include "rsphere.h"
typedef struct {

  TObject_R General;
  TSphere_R  *Pl1[20];
  TReal      Dens[20];

  TVertex CamPos;
  long    NumComponents;
  long    NumComponents2;
  TReal Fact1;

}TBlob_R;


extern TBlob_R *ActiveBlob;


extern void TBlob_R_Init ( byte P1,byte P2);
extern void TBlob_R_AddComponent ( TReal X1,TReal Y1,TReal Z1,TReal Dens);
extern void TBlob_R_SetTextScale (TReal P1);
extern void TBlob_R_SetRGBColor (byte P1,byte P2,byte P3);
extern void TBlob_R_SetTexture (rgbcol *P1);
extern void TBlob_R_GetInfo ( RayInfo &P1 , TVertex &CH );
extern void TBlob_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv);
extern void TBlob_R_SetValuesIntersection (TVertex  &Pos);
extern long TBlob_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1);
extern long TBlob_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1);
extern long TBlob_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir3);
extern void DestroyBlobDefault(TObject_R*M);
extern TObject_R *CreateBlobDefault();

#endif

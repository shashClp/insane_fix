//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Ellipse
#define SystemR_V30_Ellipse
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct {
  TObject_R General;
  TReal Fact1;
  TReal CX;
  TReal CY;
  TReal CZ;
  TReal C;
  TReal A_R;
  TReal B_R;
  TReal C_R;
  TReal TMP1;
  TReal RES;
  TVertex CPosition;
  long Rot;
}TEllipse_R;

extern TEllipse_R *ActiveEllipse;
extern word NoInvers;


extern void TEllipse_R_Init (TReal P1,TReal P2,TReal P3,TReal P4,TReal P5,TReal P6,byte P8, byte P9);
extern void TEllipse_R_GetInfo ( RayInfo &P1 , TVertex &CH );
extern void TEllipse_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv);
extern void TEllipse_R_SetValuesIntersection (TVertex  &Pos);
extern long TEllipse_R_GetPrimaryIntersection (TVertex &Dir,TReal &T0,TReal &T1);
extern long TEllipse_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1);
extern long TEllipse_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir);
extern void DestroyEllipseDefault(TObject_R*M);
extern TObject_R *CreateEllipseDefault();

#endif

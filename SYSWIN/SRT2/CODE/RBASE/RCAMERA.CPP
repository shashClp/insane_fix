//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
#include "rcamera.h"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TCamera_R *ActiveCamera;

void TCamera_R_Init (TVertex P1,TVertex P2,TVertex P3,TReal const &P4,TReal const &P5)
{
  RVTX_Copy ( ActiveCamera->Position , P1) ;
  RVTX_Copy ( ActiveCamera->LookAt , P2);
  RVTX_Copy ( ActiveCamera->Normal , P3);

  ActiveCamera->PlaneDistance = P4;
  ActiveCamera->AngleView     =   P5*R_PIx2/360.0;
  ActiveCamera->T = ActiveCamera->PlaneDistance*tan(ActiveCamera->AngleView/2);
}

void TCamera_R_Actualize (int PantX,int PantY){

  TVertex hV0,hV1,hV2;

  TReal Aspect = (float)(PantY)/(float)(PantX);

  TVertex N;
  RVTX_Copy (N,ActiveCamera->LookAt);
  RVTX_Sub  (N , ActiveCamera->Position);
  RVTX_Normalize (N);

  TVertex U ;
  TVertex V ;

  RVTX_CrossProduct( U, N , ActiveCamera->Normal );
  RVTX_CrossProduct( V, U , N );

  RVTX_Normalize (U);

  N.x *= ActiveCamera->PlaneDistance;
  N.y *= ActiveCamera->PlaneDistance;
  N.z *= ActiveCamera->PlaneDistance;

  RVTX_Normalize (V);

  RVTX_Set ( V, V.x*Aspect,V.y*Aspect,V.z*Aspect);

  TReal T = ActiveCamera->T;

  RVTX_Set (hV0 ,(+U.x+V.x)*T + N.x,(+U.y+V.y)*T + N.y,(U.z+V.z)*T + N.z);
  RVTX_Set (hV1 ,(-U.x+V.x)*T + N.x,(-U.y+V.y)*T + N.y,(-U.z+V.z)*T + N.z);
  RVTX_Set (hV2 ,(+U.x-V.x)*T + N.x,(+U.y-V.y)*T + N.y,(U.z-V.z)*T + N.z);

  RVTX_Normalize (hV0);
  RVTX_Normalize (hV1);
  RVTX_Normalize (hV2);

  TReal InvPantX = 1.0/PantX;
  TReal InvPantY = 1.0/PantY;
  ActiveCamera->dX0 = (hV1.x-hV0.x)*InvPantX;
  ActiveCamera->dX1 = (hV2.x-hV0.x)*InvPantY;
  ActiveCamera->dY0 = (hV1.y-hV0.y)*InvPantX;
  ActiveCamera->dY1 = (hV2.y-hV0.y)*InvPantY;
  ActiveCamera->dZ0 = (hV1.z-hV0.z)*InvPantX;
  ActiveCamera->dZ1 = (hV2.z-hV0.z)*InvPantY;

  ActiveCamera->dX = hV0.x;
  ActiveCamera->dY = hV0.y;
  ActiveCamera->dZ = hV0.z;
}


void TCamera_R_ChangeFOV (TReal P1)
{
  ActiveCamera->AngleView     = P1*R_PIx2/360;
  ActiveCamera->T = ActiveCamera->PlaneDistance*tan(ActiveCamera->AngleView/2);
}

TCamera_R *CreateCameraDefault()
{
  TCamera_R *TMP = (TCamera_R *) R_GetMem (sizeof(TCamera_R));
  TMP->TCamera_R_Actualize = &TCamera_R_Actualize;
  TMP->TCamera_R_ChangeFOV = &TCamera_R_ChangeFOV;
  return TMP;
}

void DestroyCameraDefault(TCamera_R *M)
{
  R_FreeMem (M);
}

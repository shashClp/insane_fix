//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Camera
#define SystemR_V30_Camera
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct{
  TVertex   Position;
  TVertex   LookAt;
  TVertex   Normal;
  TReal       PlaneDistance;
  TReal       T;
  TReal       AngleView;
  TReal       dX0,dX1,dY0,dY1,dZ0,dZ1,dX,dY,dZ;
  void (*TCamera_R_Actualize) ( int , int );
  void (*TCamera_R_ChangeFOV) ( TReal );
}TCamera_R;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
extern TCamera_R *ActiveCamera;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
extern void TCamera_R_Init (TVertex P1,TVertex P2,TVertex P3,TReal const &P4,TReal const &P5);
extern void TCamera_R_Actualize (int PantX,int PantY);
extern void TCamera_R_ChangeFOV (TReal P1);
extern TCamera_R *CreateCameraDefault();
extern void DestroyCameraDefault(TCamera_R *M);
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#endif

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Light
#define SystemR_V30_Light
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef struct{
public:
  rgbcol    Color;
  TVertex   Position;
  float  (*TLightPoint_R_Diffuse) ( TVertex  &P1,TVertex  &P2 );
  float  (*TLightPoint_R_Phong)   ( TVertex  &P1,TVertex  &P2,TReal &P_I,TReal &P_S);

}TLightPoint_R;

extern TLightPoint_R *ActiveLight;



extern void TLightPoint_R_Init ( TReal P1 ,TReal P2 ,TReal P3,byte R = 255,byte G = 255,byte B = 255 );
extern void TLightPoint_R_Diffuse ( TVertex  &P1,TVertex  &P2 ,TReal &Val);
extern float TLightPoint_R_Phong (TVertex  &P1,TVertex  &P2,TReal &P_I,TReal &P_S);
extern TLightPoint_R *CreateLightDefault();
extern void DestroyLightDefault(TLightPoint_R *M);
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#endif

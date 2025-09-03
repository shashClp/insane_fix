//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Cylinder
#define SystemR_V30_Cylinder
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct {

  TObject_R General;
  byte      Tipus;
  TReal     Radi;
  TReal     Radi2;
  TReal     CX;
  TReal     CY;
  TReal     CZ;
  TReal     C;
  TReal     TMP1;
  TReal     RES;


}TCylinder_R;

TCylinder_R *ActiveCylinder;



TCylinder_R_Init ( TReal P1,TReal P2,TReal P3,TReal P7,byte P8, byte P9, byte T)

{

  ActiveCylinder->Radi  = P7*P7;
  ActiveCylinder->Tipus = T;
  ActiveCylinder->Radi2 = P7;
  RVTX_Set (ActiveCylinder->General.Position, P1,P2,P3 );
  ActiveCylinder->General.FLAGS = ( R_RECIVE_SHADOW*P8 | R_MAKE_SHADOW*P9 | R_VISIBLE );

}


void TCylinder_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{

  word u,v;

  long BTS = (1<<ActiveCylinder->General.Pigment.BitsDefinition);

  if (ActiveCylinder->General.Pigment.TexMap != NULL ) {

    switch(ActiveCylinder->Tipus){
     case 0 :
        v = ((word)((-CH.x)*ActiveCylinder->General.Pigment.TextScale+ (BTS>>1)));
        u = ((word)(64+128.0*ActiveCylinder->General.Pigment.TextScale*(atan(CH.z/CH.y)*InvR_PI )+ (BTS>>1)));
      break;
     case 1 :
        v = ((word)((-CH.y)*ActiveCylinder->General.Pigment.TextScale+ (BTS>>1)));
        u = ((word)(64+128.0*ActiveCylinder->General.Pigment.TextScale*(atan(CH.x/CH.z)*InvR_PI )+ (BTS>>1)));
      break;
     case 2  :
        v = ((word)((-CH.z)*ActiveCylinder->General.Pigment.TextScale+ (BTS>>1)));
        u = ((word)(64+128.0*ActiveCylinder->General.Pigment.TextScale*(atan(CH.x/CH.y)*InvR_PI )+ (BTS>>1)));
      break;
    }
    u = u & (BTS-1);v = v & (BTS-1);
    P1.Color.ColRGB = ActiveCylinder->General.Pigment.TexMap[(word)((u<<ActiveCylinder->General.Pigment.BitsDefinition)+v)].ColRGB;
  } else
  {
    P1.Color.ColRGB = ActiveCylinder->General.Pigment.Color.ColRGB;
  }
  if (IsReflectT)
    {
      ActiveCylinder->General.FLAGS = ActiveCylinder->General.FLAGS & Not(R_REFLECT) ;
      if (P1.Color.B.A)
      {
        ActiveCylinder->General.REFL_INTS = P1.Color.B.A;
        ActiveCylinder->General.FLAGS = ActiveCylinder->General.FLAGS | R_REFLECT ;
      }
    }
}

void TCylinder_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{

  ValNorm = ActiveCylinder->Radi2;

  switch(ActiveCylinder->Tipus){
  case 0 :
    if IsBumpped{}
    else{
      RVTX_Set(Norm,0,Pos.y-ActiveCylinder->General.Position.y,Pos.z-ActiveCylinder->General.Position.z);
    }
    break;
  case 1  :
    if IsBumpped{}
    else{
      RVTX_Set(Norm,Pos.x-ActiveCylinder->General.Position.x,0,Pos.z-ActiveCylinder->General.Position.z);
    }
    break;
  case 2 :
    if IsBumpped{}
    else{
      RVTX_Set(Norm,Pos.x-ActiveCylinder->General.Position.x,Pos.y-ActiveCylinder->General.Position.y,0);
    }
    break;
  }

  RVTX_Normalize (Norm);

  if (Inv == 1) {
    Norm.x = -Norm.x;
    Norm.y = -Norm.y;
    Norm.z = -Norm.z;
  }
}

void TCylinder_R_SetValuesIntersection (TVertex  &Pos)
{
  ActiveCylinder->CZ = Pos.z-ActiveCylinder->General.Position.z;
  ActiveCylinder->CY = Pos.y-ActiveCylinder->General.Position.y;
  ActiveCylinder->CX = Pos.x-ActiveCylinder->General.Position.x;

  ActiveCylinder->TMP1 = ActiveCylinder->General.Position.z*ActiveCylinder->General.Position.z+
         ActiveCylinder->General.Position.y*ActiveCylinder->General.Position.y+
         ActiveCylinder->General.Position.x*ActiveCylinder->General.Position.x;

  ActiveCylinder->C =  Pos.z*Pos.z+Pos.y*Pos.y+Pos.x*Pos.x;

  switch (ActiveCylinder->Tipus){
    case 0:
        ActiveCylinder->TMP1 -= ActiveCylinder->General.Position.x*ActiveCylinder->General.Position.x;
        ActiveCylinder->C +=  ActiveCylinder->TMP1  - Pos.x*Pos.x-(2*(Pos.z*ActiveCylinder->General.Position.z+Pos.y*ActiveCylinder->General.Position.y) +ActiveCylinder->Radi);
    break;
    case 1:
        ActiveCylinder->TMP1 -= ActiveCylinder->General.Position.y*ActiveCylinder->General.Position.y;
        ActiveCylinder->C +=  ActiveCylinder->TMP1 - Pos.y*Pos.y-(2*(Pos.z*ActiveCylinder->General.Position.z+Pos.x*ActiveCylinder->General.Position.x)+ActiveCylinder->Radi);
    break;
    case 2:
        ActiveCylinder->TMP1 -= ActiveCylinder->General.Position.z*ActiveCylinder->General.Position.z;
        ActiveCylinder->C +=  ActiveCylinder->TMP1 - Pos.z*Pos.z-(2*(Pos.x*ActiveCylinder->General.Position.x+Pos.y*ActiveCylinder->General.Position.y)+ActiveCylinder->Radi);
    break;
 }
}

long TCylinder_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{


  if (!IsVisible) return 0;
  TReal A,B;

  switch (ActiveCylinder->Tipus){
    case 0:
      A = Dir.z*Dir.z+Dir.y*Dir.y;
      B = (Dir.z*ActiveCylinder->CZ+Dir.y*ActiveCylinder->CY);
      break;
    case 1:
      A = Dir.x*Dir.x+Dir.z*Dir.z;
      B = (Dir.x*ActiveCylinder->CX+Dir.z*ActiveCylinder->CZ);
      break;
    case 2:
      A = Dir.y*Dir.y+Dir.x*Dir.x;
      B = (Dir.y*ActiveCylinder->CY+Dir.x*ActiveCylinder->CX);
      break;
  }
  TReal RES = B*B-A*ActiveCylinder->C;

  TReal T,T2;

  if (RES>0) {
    RES = sqrt(RES);
    A =1/A;
    T = (-B+RES)*A;
    T2 = (-B-RES)*A;

    if ( T < T2) {
      T0 = T;
      T1 = T2;
    }  else {
      T0 = T2;
      T1 = T;
    }
    if ( T1<0 ) return 0;
    if (T0 < 0) {
        T2 = T0;
        T = T1;

        T1 = T2;
        T0 = T;

    }
    return 1;
  }
  return 0;
}

long TCylinder_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  if (!IsVisible) return 0;

  TReal CZ,CX, CY, C,A,B;
  switch (ActiveCylinder->Tipus){
    case 0:
      CZ = Pos.z-ActiveCylinder->General.Position.z;
      CY = Pos.y-ActiveCylinder->General.Position.y;
      C = ActiveCylinder->TMP1 + Pos.z*Pos.z+Pos.y*Pos.y-(2*(Pos.z*ActiveCylinder->General.Position.z+Pos.y*ActiveCylinder->General.Position.y)+ActiveCylinder->Radi);
      A = Dir.z*Dir.z+Dir.y*Dir.y;
      B = (Dir.z*CZ+Dir.y*CY);
      break;
    case 1:
      CX = Pos.x-ActiveCylinder->General.Position.x;
      CZ = Pos.z-ActiveCylinder->General.Position.z;
      C = ActiveCylinder->TMP1 + Pos.x*Pos.x+Pos.z*Pos.z-(2*(Pos.x*ActiveCylinder->General.Position.x+Pos.z*ActiveCylinder->General.Position.z)+ActiveCylinder->Radi);
      A = Dir.x*Dir.x+Dir.z*Dir.z;
      B = (Dir.x*CX+Dir.z*CZ);
      break;
    case 2:
      CX = Pos.x-ActiveCylinder->General.Position.x;
      CY = Pos.y-ActiveCylinder->General.Position.y;
      C = ActiveCylinder->TMP1 + Pos.x*Pos.x+Pos.y*Pos.y-(2*(Pos.x*ActiveCylinder->General.Position.x+Pos.y*ActiveCylinder->General.Position.y)+ActiveCylinder->Radi);
      A = Dir.x*Dir.x+Dir.y*Dir.y;
      B = (Dir.x*CX+Dir.y*CY);
      break;
  }

  TReal RES = B*B;
  RES -= A*C;
  TReal T,T2;
  if (RES>0) {
    RES = sqrt(RES);
    A =1/A;
    T = (-B+RES)*A;
    T2 = (-B-RES)*A;
    if ( T < T2) {
      T0 = T;
      T1 = T2;
    }  else {
      T0 = T2;
      T1 = T;
    }
    if ( (T < 0) && (T2<0)) return 0;
    return 1;
  }
  return 0;
}


long TCylinder_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{
  TReal SCZ;
  TReal SCY;
  TReal SCX;
  TReal SA ;
  TReal SB ;
  TReal SC ;
  if (!IsOpac) {return 0;}

  switch (ActiveCylinder->Tipus){
    case 0:
      SCZ = Pos.z-ActiveCylinder->General.Position.z;
      SCY = Pos.y-ActiveCylinder->General.Position.y;
      SA = Dir.z*Dir.z+Dir.y*Dir.y;
      SB = (Dir.z*SCZ+Dir.y*SCY);
      SC = ActiveCylinder->TMP1 + Pos.z*Pos.z+Pos.y*Pos.y-(2*(Pos.z*ActiveCylinder->General.Position.z+Pos.y*ActiveCylinder->General.Position.y)+ActiveCylinder->Radi);
      break;
    case 1:
      SCX = Pos.x-ActiveCylinder->General.Position.x;
      SCZ = Pos.z-ActiveCylinder->General.Position.z;
      SA = Dir.x*Dir.x+Dir.z*Dir.z;
      SB = (Dir.x*SCX+Dir.z*SCZ);
      SC = ActiveCylinder->TMP1 + Pos.x*Pos.x+Pos.z*Pos.z-(2*(Pos.x*ActiveCylinder->General.Position.x+Pos.z*ActiveCylinder->General.Position.z)+ActiveCylinder->Radi);
      break;
    case 2:
      SCX = Pos.x-ActiveCylinder->General.Position.x;
      SCY = Pos.y-ActiveCylinder->General.Position.y;
      SA = Dir.x*Dir.x+Dir.y*Dir.y;
      SB = (Dir.x*SCX+Dir.y*SCY);
      SC = ActiveCylinder->TMP1 + Pos.x*Pos.x+Pos.y*Pos.y-(2*(Pos.x*ActiveCylinder->General.Position.x+Pos.y*ActiveCylinder->General.Position.y)+ActiveCylinder->Radi);
      break;
  }
  TReal SRES = SB*SB;
  SRES -= SA*SC;
  TReal T;
  if (SRES>0) {
    SRES = sqrt(SRES);
    SA =1/SA;
    T = (-SB+SRES)*SA;
    if ((T>0) && (T<1)) return 1;
  }
  return 0;
}
void DestroyCylinderDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Cylinder of Memory");
#endif
  R_FreeMem (M);
}

TObject_R *CreateCylinderDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Cylinder in Memory");
#endif
  TCylinder_R *TMP = (TCylinder_R *) R_GetMem (sizeof(TCylinder_R));

  TMP->General.GetColorInfo             = &TCylinder_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TCylinder_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TCylinder_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TCylinder_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TCylinder_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TCylinder_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_CYLINDER;
  TMP->General.Invers = 0;
  TMP->General.Pigment.TxtMode = 0;
  TMP->General.Pigment.TextScale = 10;
  TMP->General.Pigment.InvTextScale = 1.0 / TMP->General.Pigment.TextScale ;
  TMP->General.Pigment.TexMap = NULL;
  TMP->General.PNG = 0;
  TMP->General.Value1 = 0;

#ifdef SYR_PROCEDURAL_TEXTURES
  TMP->General.Pigment.Texture = NULL;
#endif
  return (TObject_R *)TMP;
}

#endif

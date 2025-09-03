//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
#include "rplane.h"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

TPlane_R *ActivePlane;


void TPlane_R_Init ( TReal P1,TReal P2,TReal P3,TReal P7,TReal P8,TReal P9,byte P10,byte P11 )
{


  ActivePlane->General.Position.x = P1;
  ActivePlane->General.Position.y = P2;
  ActivePlane->General.Position.z = P3;

  ActivePlane->Normal.x = P7;
  ActivePlane->Normal.y = P8;
  ActivePlane->Normal.z = P9;

  RVTX_Normalize ( ActivePlane->Normal );

  ActivePlane->D = ActivePlane->Normal.x*ActivePlane->General.Position.x
                  +ActivePlane->Normal.y*ActivePlane->General.Position.y
                  +ActivePlane->Normal.z*ActivePlane->General.Position.z;

  ActivePlane->General.FLAGS = ( R_RECIVE_SHADOW*P10 | R_MAKE_SHADOW*P11 | R_VISIBLE );


}

void TPlane_R_GetInfo ( RayInfo &P1 , TVertex &CH2 )
{
  word u,v;
  word u2,v2;
  rgbcol *TmpTexture;
  long BTS = (1<<ActivePlane->General.Pigment.BitsDefinition);



  if (ActivePlane->General.Pigment.TexMap != NULL ) {
    TVertex CH = CH2;
    RVTX_Sub (CH,ActivePlane->General.Position);

    switch (ActivePlane->General.Pigment.TxtMode){
      case 0:
        u = ((word)(CH.z*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        v = ((word)(CH.x*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        u2 = ActivePlane->W2;
        v2 = ActivePlane->V2;
        break;
      case 1:
        u = ((word)(CH.y*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        v = ((word)(CH.x*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        u2 = ActivePlane->U2;
        v2 = ActivePlane->V2;
        break;
      case 2:
        u = ((word)(CH.y*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        v = ((word)(CH.z*ActivePlane->General.Pigment.TextScale + (BTS>>1) )) ;
        u2 = ActivePlane->U2;
        v2 = ActivePlane->W2;
        break;
    }

    TmpTexture = ActivePlane->General.Pigment.TexMap;

    if (ActivePlane->General.FLAGS & R_CENTRAL_TEXT)
      //if ( ((u-u2)<BTS) && ((v-v2)<BTS) )
      if ( ((u)<BTS) && (v<BTS) )
        TmpTexture = ActivePlane->Texture2;

    u = u & (BTS-1);v = v & (BTS-1);
    P1.Color.ColRGB = TmpTexture[(u<<ActivePlane->General.Pigment.BitsDefinition)+v].ColRGB;
  } else
  {
    P1.Color.ColRGB = ActivePlane->General.Pigment.Color.ColRGB;
  }

  if (IsReflectT)
  {
    ActivePlane->General.FLAGS = ActivePlane->General.FLAGS & Not(R_REFLECT) ;
    if (P1.Color.B.A)
    {
      ActivePlane->General.REFL_INTS = P1.Color.B.A;
      ActivePlane->General.FLAGS = ActivePlane->General.FLAGS | R_REFLECT ;
    }
  }

  if (ActivePlane->General.FLAGS & R_PORTAL_TEXT)
  {
    if (P1.Color.B.A != 0) R_ReturnStatus = R_PORTAL_TEXT;

  }

}

void TPlane_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
   Norm.x = ActivePlane->Normal.x;
   Norm.y = ActivePlane->Normal.y;
   Norm.z = ActivePlane->Normal.z;
   ValNorm = 1;
   if IsBumpped{}
}

void TPlane_R_SetValuesIntersection (TVertex  &Pos)
{
 if (!IsVisible) return ;
 ActivePlane->TMP = (ActivePlane->D-(ActivePlane->Normal.x*Pos.x
                                    +ActivePlane->Normal.y*Pos.y
                                    +ActivePlane->Normal.z*Pos.z));
 ActivePlane->General.Value1 += 0.1;

 if (ActivePlane->General.FLAGS & R_CENTRAL_TEXT)
 {
    TVertex CH = ActivePlane->General.Position;
    ActivePlane->V2 = ((word)(CH.x*ActivePlane->General.Pigment.TextScale )) ;
    ActivePlane->U2 = ((word)(CH.y*ActivePlane->General.Pigment.TextScale )) ;
    ActivePlane->W2 = ((word)(CH.z*ActivePlane->General.Pigment.TextScale )) ;
  }

}

long TPlane_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{
  if (!IsVisible) return 0;
  TReal RES,T;

  RES = ActivePlane->Normal.x*Dir.x +
        ActivePlane->Normal.y*Dir.y +
        ActivePlane->Normal.z*Dir.z;

  if (RES != 0) {
    T = ActivePlane->TMP / RES;
    if (T>0)
    {
      T0 = T;
      return 1;
    }
  }
  return 0;
}

long TPlane_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{

  if (!IsVisible) return 0;
  TReal RES,T;
  RES = ActivePlane->Normal.x*Dir.x +
        ActivePlane->Normal.y*Dir.y +
        ActivePlane->Normal.z*Dir.z ;

  if (RES != 0) {
  T = (ActivePlane->D-(ActivePlane->Normal.x*Pos.x+
                               ActivePlane->Normal.y*Pos.y+
                               ActivePlane->Normal.z*Pos.z))/(RES);
  if (T> 0){
      T0 = T;
      return 1;
    }
  }
  return 0;
}

long TPlane_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{

  if (!IsOpac) {return 0;}
  TReal RES,T;
  RES = ActivePlane->Normal.x*Dir.x +
        ActivePlane->Normal.y*Dir.y +
        ActivePlane->Normal.z*Dir.z ;

  if (RES != 0) {
    T = (ActivePlane->D-(ActivePlane->Normal.x*Pos.x+
                         ActivePlane->Normal.y*Pos.y+
                         ActivePlane->Normal.z*Pos.z  )) / (RES);
    if ((T>0) && (T<1)) return 1;
  }
  return 0;
}
void DestroyPlaneDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Plane of Memory");
#endif

  R_FreeMem (M);
}

TObject_R *CreatePlaneDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Plane in Memory");
#endif

  TPlane_R *TMP = (TPlane_R *) R_GetMem (sizeof(TPlane_R));

  TMP->General.GetColorInfo             = &TPlane_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TPlane_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TPlane_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TPlane_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TPlane_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TPlane_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_PLANE;
  TMP->General.Invers = 0;
  TMP->General.Pigment.TxtMode = 0;
  TMP->General.Pigment.TextScale = 10;
  TMP->General.Pigment.InvTextScale = 1.0 / TMP->General.Pigment.TextScale ;
  TMP->General.Pigment.TexMap = NULL;
  TMP->General.PNG = 0;
  TMP->General.Value1 = 0;
  TMP->General.Pigment.BitsDefinition = 7;
#ifdef SYR_PROCEDURAL_TEXTURES
  TMP->General.Pigment.Texture = NULL;
#endif
  return (TObject_R *)TMP;
}

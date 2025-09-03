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
#include "rsphere.h"
long SphPreCalc;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TSphere_R *ActiveSphere ;


void TSphere_R_Init  ( TReal P1 , TReal P2 , TReal P3 , TReal P7 , byte P8 , byte P9 )
{

  ActiveSphere->General.Position.x = P1;
  ActiveSphere->General.Position.y = P2;
  ActiveSphere->General.Position.z = P3;
  ActiveSphere->Radi        =       P7*P7;
  ActiveSphere->Radi2       =       P7;
  ActiveSphere->General.FLAGS = ( R_RECIVE_SHADOW*P8 | R_MAKE_SHADOW*P9 | R_VISIBLE );

}


void TSphere_R_GetInfo ( RayInfo &P1, TVertex &CH )
{
  word u,v;
  TVertex RPos;

  if ( ActiveSphere->General.Pigment.TexMap != NULL){

    RVTX_Copy (RPos,CH);
    RVTX_Sub ( RPos , ActiveSphere->General.Position );

    u = ((word)(ActiveSphere->Fact1*(asin(RPos.y/ActiveSphere->Radi2))));
    v = ((word)(ActiveSphere->Fact1*(atan(RPos.x/RPos.z))));
    u = (u) & 127;
    v = (v) & 127;

    P1.Color.ColRGB = ActiveSphere->General.Pigment.TexMap[u*128+v].ColRGB;

  } else {
    P1.Color.ColRGB = ActiveSphere->General.Pigment.Color.ColRGB;
  }
/*
    if (IsReflectT)
  {
    ActiveSphere->General.FLAGS = ActiveSphere->General.FLAGS & Not(R_REFLECT) ;
    if (P1.Color.B.A)
    {
      ActiveSphere->General.REFL_INTS = P1.Color.B.A;
      ActiveSphere->General.FLAGS = ActiveSphere->General.FLAGS | R_REFLECT ;
    }
  }
  if (IsTransparentT)
  {
    ActiveSphere->General.FLAGS = ActiveSphere->General.FLAGS & Not(R_TRANSPARENT) ;
    if (P1.Color.B.A)
    {
      ActiveSphere->General.IOR_INTS = P1.Color.B.A;
      ActiveSphere->General.FLAGS = ActiveSphere->General.FLAGS | R_TRANSPARENT ;
    }
  }
*/
}
void TSphere_R_GetNormal (TVertex  &Pos,TVertex &Norm,TReal &ValNorm,long Inv)
{

  TReal SN = 0;

  if IsBumpped {
  }else {
    RVTX_Copy (Norm,Pos);
    RVTX_Sub (Norm,ActiveSphere->General.Position);
    RVTX_Normalize (Norm);
  }
  if ( (Inv == 1) || (ActiveSphere->General.Invers )) {
    Norm.x = -Norm.x;
    Norm.y = -Norm.y;
    Norm.z = -Norm.z;
  }

}
void TSphere_R_SetValuesIntersection (TVertex  &Pos)
{
  if (!IsVisible) return ;

#ifdef DEBUG_RENDER
    cprintf ("Sphere Precalculatin \r\n");
#endif

  ActiveSphere->CX = Pos.x-ActiveSphere->General.Position.x;
  ActiveSphere->CY = Pos.y-ActiveSphere->General.Position.y;
  ActiveSphere->CZ = Pos.z-ActiveSphere->General.Position.z;

  ActiveSphere->TMP1 =   ActiveSphere->General.Position.x*ActiveSphere->General.Position.x
                        +ActiveSphere->General.Position.y*ActiveSphere->General.Position.y
                        +ActiveSphere->General.Position.z*ActiveSphere->General.Position.z;

  ActiveSphere->C = ActiveSphere->TMP1 + Pos.x*Pos.x+Pos.y*Pos.y+Pos.z*Pos.z-
              (2*(Pos.x*ActiveSphere->General.Position.x
                 +Pos.y*ActiveSphere->General.Position.y
                 +Pos.z*ActiveSphere->General.Position.z)+ActiveSphere->Radi);


  ActiveSphere->Fact1 = R_TextureSize*InvR_PI*ActiveSphere->General.Pigment.TextScale;
  SphPreCalc = 0;

}


long TSphere_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{
  if (!IsVisible) return 0;

  if (!SphPreCalc)
  {
    CompletDir = Dir.x*Dir.x+Dir.y*Dir.y+Dir.z*Dir.z;
    InvCompletDir = 1.0 / CompletDir;
    SphPreCalc = 1;
  }

#ifdef DEBUG_RENDER
    cprintf ("Sphere calculatin \r\n");
#endif

  TReal B = (Dir.x*ActiveSphere->CX+Dir.y*ActiveSphere->CY+Dir.z*ActiveSphere->CZ);
  TReal RES = B*B - CompletDir*ActiveSphere->C;
  TReal T,T2;

  if (RES>0)
  {
    RES = sqrt(RES);
    T = (-B+RES)*InvCompletDir;
    T2 = (-B-RES)*InvCompletDir;

    if (T2 < T)  { T0 = T; T = T2; T2 = T0;}

    ActiveSphere->General.Invers = 0;

    if ( T <= 0.0001 )
    {
        if ( T2 > 0.0001 )
        {
            T0 = T2;
            T1 = T;
            ActiveSphere->General.Invers = 1;
            return 1;
        } else return 0;

    } else
    {
        T0 = T;
        T1 = T2;
        return 1;

    }
  }
  return 0;
}

long TSphere_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{

  if (!IsVisible) return 0;

//  TReal CX = Pos.x-ActiveSphere->General.Position.x;
//  TReal CY = Pos.y-ActiveSphere->General.Position.y;
//  TReal CZ = Pos.z-ActiveSphere->General.Position.z;
//  TReal TMP1 = ActiveSphere->General.Position.x*ActiveSphere->General.Position.x
//              +ActiveSphere->General.Position.y*ActiveSphere->General.Position.y
//              +ActiveSphere->General.Position.z*ActiveSphere->General.Position.z;
  TReal C = ActiveSphere->TMP1 + Pos.x*Pos.x+Pos.y*Pos.y+Pos.z*Pos.z-
              (2*(Pos.x*ActiveSphere->General.Position.x
                  +Pos.y*ActiveSphere->General.Position.y
                  +Pos.z*ActiveSphere->General.Position.z)+ActiveSphere->Radi);
  TReal A = Dir.x*Dir.x+Dir.y*Dir.y+Dir.z*Dir.z;
  TReal B = (Dir.x*(Pos.x-ActiveSphere->General.Position.x)+
             Dir.y*(Pos.y-ActiveSphere->General.Position.y)+
             Dir.z*(Pos.z-ActiveSphere->General.Position.z));

  TReal RES = B*B-A*C;
  TReal T,T2;

  if (RES>0) {
    RES = sqrt(RES);
    T = (-B+RES)/A;
    T2 = (-B-RES)/A;

    if (T2 < T)  { T0 = T; T = T2; T2 = T0;}

    ActiveSphere->General.Invers = 0;

    if ( T <= 0.0001 )
    {
        if ( T2 > 0.0001 )
        {
            T0 = T2;
            T1 = T;
            ActiveSphere->General.Invers = 1;
            return 1;
        } else return 0;

    } else
    {
        T0 = T;
        T1 = T2;
        return 1;

    }
  }
  return 0;
}

long TSphere_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{

  if (!IsOpac) {return 0;}

  if (!SphPreCalc)
  {
    CompletPos = Pos.x*Pos.x + Pos.y*Pos.y + Pos.z*Pos.z;
    CompletDir = Dir.x*Dir.x+Dir.y*Dir.y+Dir.z*Dir.z;
    InvCompletDir = 1.0 / CompletDir;
    SphPreCalc = 1;
  }

  TReal SB = (Dir.x*(Pos.x-ActiveSphere->General.Position.x)+
              Dir.y*(Pos.y-ActiveSphere->General.Position.y)+
              Dir.z*(Pos.z-ActiveSphere->General.Position.z));
  TReal SRES = SB*SB - CompletDir*(ActiveSphere->TMP1 + CompletPos   -
              (2*(Pos.x*ActiveSphere->General.Position.x+
                  Pos.y*ActiveSphere->General.Position.y+
                  Pos.z*ActiveSphere->General.Position.z)+ActiveSphere->Radi));
  TReal T;
  if (SRES>0) {
    T = (-SB+sqrt(SRES))*InvCompletDir;
    if ((T>0) && (T<1)) return 1;

  }
  return 0;
}

void DestroySphereDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Sphere of Memory at [%p]",M);
#endif

  R_FreeMem (M);
}

TObject_R *CreateSphereDefault()
{
  TSphere_R *TMP = NULL;

  if ( ( TMP = (TSphere_R *) R_GetMem (sizeof(TSphere_R)))==NULL)
  {
#ifdef R_LOG_DEF
    R_ERR ("Can't alloc memory for sphere");
#endif
    RAY_EXIT ();
  }

#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Sphere in Memory at [%p]",TMP);
#endif


  TMP->General.GetColorInfo             = &TSphere_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TSphere_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TSphere_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TSphere_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TSphere_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TSphere_R_SetValuesIntersection;

  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;


  TMP->General.Type = R_SPHERE;
  TMP->General.Invers = 0;
  TMP->General.Pigment.TxtMode = 0;
  TMP->General.Pigment.TextScale = 1;
  TMP->General.Pigment.InvTextScale = 1.0 / TMP->General.Pigment.TextScale ;
  TMP->General.Pigment.TexMap = NULL;
  TMP->General.PNG = 0;
  TMP->General.Value1 = 0;
  TMP->General.IOR = 2;
#ifdef SYR_PROCEDURAL_TEXTURES
  TMP->General.Pigment.Texture = NULL;
#endif

  return (TObject_R *)TMP;
}


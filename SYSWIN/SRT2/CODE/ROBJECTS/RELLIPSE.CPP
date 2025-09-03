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
#include "rellipse.h"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TEllipse_R *ActiveEllipse;
word NoInvers;


void TEllipse_R_Init (TReal P1,TReal P2,TReal P3,TReal P4,TReal P5,TReal P6,byte P8, byte P9)
{
#ifdef R_LOG_DEF
  R_LOG ("    Position: <%3.3f,%3.3f,%3.3f>",P1,P2,P3);
  R_LOG ("    Radius  : <%3.3f,%3.3f,%3.3f>",P4,P5,P6);
#endif

  ActiveEllipse->A_R = 1.0/(P4*P4);
  ActiveEllipse->B_R = 1.0/(P5*P5);
  ActiveEllipse->C_R = 1.0/(P6*P6);

  RVTX_Set (ActiveEllipse->General.Position,P1,P2,P3);

  ActiveEllipse->General.FLAGS = ( R_RECIVE_SHADOW*P8 | R_MAKE_SHADOW*P9 | R_VISIBLE );

}

void TEllipse_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{

  word u,v;
  TVertex RPos;
  long BTS = (1<<ActiveEllipse->General.Pigment.BitsDefinition);
  if (ActiveEllipse->General.Pigment.TexMap != NULL )
  {
    RVTX_Copy (RPos,CH);
    RVTX_Sub ( RPos , ActiveEllipse->General.Position );
    //u = ((word)(ActiveEllipse->Fact1*(atan(RPos.x/RPos.y)) )) ;
    u = ((word)(ActiveEllipse->Fact1*(atan(RPos.y/RPos.z)) + (BTS>>1))) ;
    v = ((word)(ActiveEllipse->Fact1*(atan(RPos.x/RPos.z)) + (BTS>>1))) ;

    u = u & (BTS-1);v = v & (BTS-1);

    P1.Color.ColRGB = ActiveEllipse->General.Pigment.TexMap[((u<<ActiveEllipse->General.Pigment.BitsDefinition)+(v))].ColRGB;
  } else {
    P1.Color.ColRGB = ActiveEllipse->General.Pigment.Color.ColRGB;
  }
/*
    if (IsReflectT)
  {
    ActiveEllipse->General.FLAGS = ActiveEllipse->General.FLAGS & Not(R_REFLECT) ;
    if (P1.Color.B.A)
    {
      ActiveEllipse->General.REFL_INTS = P1.Color.B.A;
      ActiveEllipse->General.FLAGS = ActiveEllipse->General.FLAGS | R_REFLECT ;
    }
  }
  P1.Color.B.R = P1.Color.B.A;
  P1.Color.B.G = 255-P1.Color.B.A;
  P1.Color.B.B = 0;
*/
  if (IsTransparentT)
  {
    ActiveEllipse->General.FLAGS = ActiveEllipse->General.FLAGS & Not(R_TRANSPARENT) ;
    ActiveEllipse->General.IOR_INTS = 0 ;
    if (P1.Color.B.A)
    {

      ActiveEllipse->General.IOR_INTS = P1.Color.B.A;
      P1.Color.ColRGB = 0;
//      ActiveEllipse->General.IOR_INTS = 255 ;
      ActiveEllipse->General.FLAGS = ActiveEllipse->General.FLAGS | R_TRANSPARENT ;
    }
  }
}

void TEllipse_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
  TReal  SN = 0;
  if (IsBumpped)  {
  SN = sin ((Pos.y-ActiveEllipse->General.Position.y)*0.5 * R_PIx2 )*2;
  RVTX_Set ( Norm, SN + (Pos.x-ActiveEllipse->General.Position.x),
                  SN + (Pos.y-ActiveEllipse->General.Position.y),
                  SN + (Pos.z-ActiveEllipse->General.Position.z));
  } else
  {
  RVTX_Set ( Norm, ActiveEllipse->A_R*(Pos.x-ActiveEllipse->General.Position.x),
                  ActiveEllipse->B_R*(Pos.y-ActiveEllipse->General.Position.y),
                  SN + ActiveEllipse->C_R*(Pos.z-ActiveEllipse->General.Position.z));
  }
  RVTX_Normalize ( Norm );
  ValNorm = 0;
  if ( (Inv == 1) || (ActiveEllipse->General.Invers )) {
    Norm.x = -Norm.x;
    Norm.y = -Norm.y;
    Norm.z = -Norm.z;
  }
}

void TEllipse_R_SetValuesIntersection (TVertex  &Pos)
{

  ActiveEllipse->CX = (Pos.x-ActiveEllipse->General.Position.x)*ActiveEllipse->A_R;
  ActiveEllipse->CY = (Pos.y-ActiveEllipse->General.Position.y)*ActiveEllipse->B_R;
  ActiveEllipse->CZ = (Pos.z-ActiveEllipse->General.Position.z)*ActiveEllipse->C_R;
  ActiveEllipse->TMP1 = ActiveEllipse->General.Position.x*ActiveEllipse->General.Position.x*ActiveEllipse->A_R+
                        ActiveEllipse->General.Position.y*ActiveEllipse->General.Position.y*ActiveEllipse->B_R+
                        ActiveEllipse->General.Position.z*ActiveEllipse->General.Position.z*ActiveEllipse->C_R  ;

  ActiveEllipse->C = ActiveEllipse->TMP1 + Pos.x*Pos.x*ActiveEllipse->A_R+
                                           Pos.y*Pos.y*ActiveEllipse->B_R+
                                           Pos.z*Pos.z*ActiveEllipse->C_R-
              (2*(Pos.x*ActiveEllipse->General.Position.x*ActiveEllipse->A_R+
                  Pos.y*ActiveEllipse->General.Position.y*ActiveEllipse->B_R+
                  Pos.z*ActiveEllipse->General.Position.z*ActiveEllipse->C_R)+1);

  ActiveEllipse->Fact1 = 128.0*InvR_PI*ActiveEllipse->General.Pigment.TextScale;
  ActiveEllipse->CPosition.x = ActiveEllipse->General.Position.x*ActiveEllipse->A_R;
  ActiveEllipse->CPosition.y = ActiveEllipse->General.Position.y*ActiveEllipse->B_R;
  ActiveEllipse->CPosition.z = ActiveEllipse->General.Position.z*ActiveEllipse->C_R;
}

long TEllipse_R_GetPrimaryIntersection (TVertex &Dir,TReal &T0,TReal &T1)
{

  if (!IsVisible) return 0;

  TReal B = (Dir.x*ActiveEllipse->CX+
             Dir.y*ActiveEllipse->CY+
             Dir.z*ActiveEllipse->CZ);

  TReal A = (Dir.x*Dir.x*ActiveEllipse->A_R
            +Dir.y*Dir.y*ActiveEllipse->B_R
            +Dir.z*Dir.z*ActiveEllipse->C_R);
  TReal RES = B*B - A*ActiveEllipse->C;
  TReal T,T2;

  if (RES>0) {
    RES = sqrt(RES);
    A = 1.0/A;
    T = (-B+RES)*A;
    T2 = (-B-RES)*A;

    if (T2 < T)  { T0 = T; T = T2; T2 = T0;}

    ActiveEllipse->General.Invers = 0;
      if ( T <= 0.0001 )
      {
          if ( T2 > 0.0001 )
          {
              T0 = T2;
              T1 = T;
              ActiveEllipse->General.Invers = 1;
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

long TEllipse_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  if (!IsVisible) return 0;

  TReal C  = (
                ActiveEllipse->TMP1
            ) + Pos.x*Pos.x*ActiveEllipse->A_R+Pos.y*Pos.y*ActiveEllipse->B_R+Pos.z*Pos.z*ActiveEllipse->C_R-
            (2*(Pos.x*ActiveEllipse->CPosition.x+
                Pos.y*ActiveEllipse->CPosition.y+
                Pos.z*ActiveEllipse->CPosition.z)+1);
  TReal DX,DY,DZ;
  DX = Dir.x*ActiveEllipse->A_R;
  DY = Dir.y*ActiveEllipse->B_R;
  DZ = Dir.z*ActiveEllipse->C_R;

  /*
  TReal B = (Dir.x*(Pos.x-ActiveEllipse->General.Position.x)*ActiveEllipse->A_R+
             Dir.y*(Pos.y-ActiveEllipse->General.Position.y)*ActiveEllipse->B_R+
             Dir.z*(Pos.z-ActiveEllipse->General.Position.z)*ActiveEllipse->C_R);
  TReal A = (Dir.x*Dir.x*ActiveEllipse->A_R+
             Dir.y*Dir.y*ActiveEllipse->B_R+
             Dir.z*Dir.z*ActiveEllipse->C_R);
  */
  TReal B = (DX*(Pos.x-ActiveEllipse->General.Position.x)+
             DY*(Pos.y-ActiveEllipse->General.Position.y)+
             DZ*(Pos.z-ActiveEllipse->General.Position.z));
  TReal A = (Dir.x*DX+
             Dir.y*DY+
             Dir.z*DZ);

  TReal RES = B*B - A*C;

  TReal T,T2;
  if (RES>0) {
    RES = sqrt(RES);
    A = 1/A;
    T = (-B+RES)*A;
    T2 = (-B-RES)*A;

    if (T2 < T)  { T0 = T; T = T2; T2 = T0;}

    if ( T <= 0.0001 )
    {
        if ( T2 > 0.0001 )
        {
            T0 = T2;
            T1 = T;
            ActiveEllipse->General.Invers = 1;
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
long TEllipse_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{
  if (!IsOpac) {return 0;}

  TReal C  = (
                ActiveEllipse->TMP1

            ) + Pos.x*Pos.x*ActiveEllipse->A_R+Pos.y*Pos.y*ActiveEllipse->B_R+Pos.z*Pos.z*ActiveEllipse->C_R-
            (2*(Pos.x*ActiveEllipse->CPosition.x+
                Pos.y*ActiveEllipse->CPosition.y+
                Pos.z*ActiveEllipse->CPosition.z)+1);

  TReal DX = Dir.x*ActiveEllipse->A_R;
  TReal DY = Dir.y*ActiveEllipse->B_R;
  TReal DZ = Dir.z*ActiveEllipse->C_R;
  /*
  TReal B = (Dir.x*(Pos.x-ActiveEllipse->General.Position.x)*ActiveEllipse->A_R+
             Dir.y*(Pos.y-ActiveEllipse->General.Position.y)*ActiveEllipse->B_R+
             Dir.z*(Pos.z-ActiveEllipse->General.Position.z)*ActiveEllipse->C_R);
  TReal A = (Dir.x*Dir.x*ActiveEllipse->A_R+
             Dir.y*Dir.y*ActiveEllipse->B_R+
             Dir.z*Dir.z*ActiveEllipse->C_R);
  */

  TReal B = (DX*(Pos.x-ActiveEllipse->General.Position.x)+
             DY*(Pos.y-ActiveEllipse->General.Position.y)+
             DZ*(Pos.z-ActiveEllipse->General.Position.z));
  TReal A = (Dir.x*DX+
             Dir.y*DY+
             Dir.z*DZ);

  TReal RES = sqr(B) - A*C;
  TReal T;

  if (RES>0) {
    T = (-B+sqrt(RES))/A ;
    if ((T>0) && (T<1))  return 1;

  }
  return 0;
}
void DestroyEllipseDefault(TObject_R*M)
{

#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Ellipse of Memory at [%p]",M);
#endif
  R_FreeMem (M);

}

TObject_R *CreateEllipseDefault()
{

  TEllipse_R *TMP = NULL;

  if ( ( TMP = (TEllipse_R *) R_GetMem (sizeof(TEllipse_R)))==NULL)
  {

#ifdef R_LOG_DEF
    R_ERR ("Can't alloc memory for ellipse");
#endif
    RAY_EXIT ();
  }

#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Ellipse in Memory at [%p]",TMP);
#endif


  TMP->General.GetColorInfo             = &TEllipse_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TEllipse_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TEllipse_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TEllipse_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TEllipse_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TEllipse_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_ELLIPSE;
  TMP->General.Invers = 0;
  TMP->General.Pigment.TxtMode = 0;
  TMP->General.Pigment.TextScale = 1;
  TMP->General.Pigment.InvTextScale = 1.0 / TMP->General.Pigment.TextScale ;
  TMP->General.Pigment.TexMap = NULL;
  TMP->General.PNG = 0;
  TMP->General.Value1 = 0;
  TMP->Rot = 0;
#ifdef SYR_PROCEDURAL_TEXTURES
  TMP->General.Pigment.Texture = NULL;
#endif
  return (TObject_R *)TMP;
}


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_CSG
#define SystemR_V30_CSG
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
#ifdef PlaneOK
#include "..\ROBJECTS\RPLANE.H"
#endif

#ifdef SphereOK
#include "..\ROBJECTS\RSPHERE.H"
#endif

#ifdef EllipseOK
#include "..\ROBJECTS\RELLIPSE.H"
#endif

#ifdef CylinderOK
#include "..\ROBJECTS\RCYLINDE.H"
#endif

#ifdef CubeOK
#include "..\ROBJECTS\RCUBE.H"
#endif

#ifdef BlobOK
#include "..\ROBJECTS\RBLOB.H"
#endif

#ifdef SorOK
#include "..\ROBJECTS\RSOR.H"
#endif

typedef struct {

  TObject_R General;
  TObject_R *Ob1;
  TObject_R *Ob2;
  byte Invers;

}TCSG_R;

TCSG_R *ActiveCSG;




TCSG_R_Init (TObject_R *O1,TObject_R *O2, byte P1,byte P2 )
{
  ActiveCSG->General.FLAGS = ( R_RECIVE_SHADOW*P1 | R_MAKE_SHADOW*P2 | R_VISIBLE );
  ActiveCSG->Ob1 = O1;
  ActiveCSG->Ob2 = O2;
}


void TCSG_Active ( TObject_R *Ob )
{
  ActiveObject = Ob;
  switch (ActiveObject->Type)
  {
#ifdef PlaneOK
    case R_PLANE:
        ActivePlane = (TPlane_R*) ActiveObject;
        break;
#endif
#ifdef SphereOK
    case R_SPHERE:
        ActiveSphere = (TSphere_R*) ActiveObject;
        break;
#endif
#ifdef CylinderOK
    case R_CYLINDER:
        ActiveCylinder= (TCylinder_R*) ActiveObject;
        break;
#endif
#ifdef EllipseOK
    case R_ELLIPSE:
        ActiveEllipse= (TEllipse_R*) ActiveObject;
        break;
#endif
#ifdef CubeOK
    case R_CUBE:
        ActiveCube= (TCube_R*) ActiveObject;
        break;
#endif
#ifdef CSGOK
    case R_CSG:
        ActiveCSG= (TCSG_R*) ActiveObject;
        break;
#endif

  };

}
void TCSG_R_SetTexture (rgbcol *P1)
{
  TObject_R *Push = ActiveObject;
  TCSG_Active ( ActiveCSG->Ob1 );
  ActiveObject->SetTexture ( P1 );
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
}
void TCSG_R_SetRGBColor (byte P1,byte P2,byte P3)
{
  TObject_R *Push = ActiveObject;
  TCSG_Active ( ActiveCSG->Ob1 );
  ActiveObject->SetRGBColor (P1,P2,P3);
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
}

void TCSG_R_SetProperty ( dword P)
{
  ActiveObject->FLAGS = ActiveObject->FLAGS | P;
  TObject_R *Push = ActiveObject;
  TCSG_Active ( ActiveCSG->Ob1 );
  ActiveObject->SetProperty ( P );
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
}

void TCSG_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{
  TObject_R *Push = ActiveObject;
  TCSG_Active ( ActiveCSG->Ob1 );
  ActiveObject->GetColorInfo ( P1, CH );
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
  if (IsReflectT)
  {
    ActiveCSG->General.FLAGS = ActiveCSG->General.FLAGS & Not(R_REFLECT) ;
    if (P1.Color.B.A)
    {
      ActiveCSG->General.REFL_INTS = P1.Color.B.A;
      ActiveCSG->General.FLAGS = ActiveCSG->General.FLAGS | R_REFLECT ;
    }
  }
  if (IsTransparentT)
  {
    ActiveCSG->General.FLAGS = ActiveCSG->General.FLAGS & Not(R_TRANSPARENT) ;
    if (P1.Color.B.A)
    {
      ActiveCSG->General.IOR_INTS = P1.Color.B.A;
      ActiveCSG->General.FLAGS = ActiveCSG->General.FLAGS | R_TRANSPARENT ;
    }
  }

}

void TCSG_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{

  TObject_R *Push = ActiveObject;
  long Invers = ActiveCSG->Invers;

  if (Inv) {
    if (Invers)
        TCSG_Active ( ActiveCSG->Ob1 );
      else
        TCSG_Active ( ActiveCSG->Ob2 );
  }
    if (Invers)
        TCSG_Active ( ActiveCSG->Ob2 );
      else
        TCSG_Active ( ActiveCSG->Ob1 );

  ActiveObject->GetNormal( Pos , Norm,  ValNorm , Invers );
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
}

void TCSG_R_SetValuesIntersection (TVertex  &Pos)
{
  TObject_R *Push = ActiveObject;

  TCSG_Active ( ((TCSG_R*)Push)->Ob1 );
  ActiveObject->SetValuesIntersection (Pos);

  TCSG_Active ( ((TCSG_R*)Push)->Ob2 );
  ActiveObject->SetValuesIntersection (Pos);

  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
}

long TCSGMinus_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{

  if (!IsVisible) return 0;

  TObject_R *Push = ActiveObject;
  TCSG_R *PushCSG = ActiveCSG;

  TReal T0_1,T1_1,T0_2,T1_2;
  int CInter1 = 0;
  int CInter2 = 0;
  int Ret = 0;

  TCSG_Active ( PushCSG->Ob1 );
  CInter1 = ActiveObject->GetPrimaryIntersection (Dir,T0_1,T1_1);

  if (CInter1)
  {
    TCSG_Active ( PushCSG->Ob2 );

    CInter2 = ActiveObject->GetPrimaryIntersection (Dir,T0_2,T1_2);
  }

  if  ( (CInter1) &&  (!CInter2)  )
  {

    PushCSG->Invers = 0;
    T0 = T0_1;
    T1 = T1_1;
    Ret = 1;
  } else
   if  ( (CInter1) &&  (CInter2)  )
   {
     if ( (T0_1 < T0_2) || (T0_1 > T1_2)   )
      {
        PushCSG->Invers = 0;
        T0 = T0_1;
        T1 = T0_2;
        Ret = 1;
      } else
       if ( (T1_2 > T0_1) && (T1_2 < T1_1) )
        {
          PushCSG->Invers = 1;
          T0 = T1_2;
          T1 = T1_1;
          Ret = 1;
        }
   }
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
  return Ret;
}

long TCSGMinus_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  if (!IsVisible) return 0;

  TObject_R *Push = ActiveObject;
  TCSG_R *PushCSG = ActiveCSG;

  TReal T0_1,T1_1,T0_2,T1_2;
  int CInter1 = 0;
  int CInter2 = 0;

  TCSG_Active ( PushCSG->Ob1 );

  CInter1 = ActiveObject->GetSecondaryIntersection (Pos,Dir,T0_1,T1_1);

  if (CInter1)
  {
    TCSG_Active ( PushCSG->Ob2 );

    CInter2 = ActiveObject->GetSecondaryIntersection (Pos,Dir,T0_2,T1_2);

    if  ( (CInter1) &&  (!CInter2)  )
    {

      PushCSG->Invers = 0;
      T0 = T0_1;
      T1 = T1_1;
      ActiveObject = Push ;
      ActiveCSG = (TCSG_R*)ActiveObject;
      return 1;

    } else
    {
      if  ( (CInter1) &&  (CInter2)  )
      {
        if ( (T0_1 < T0_2) || (T0_1 > T1_2)   )
        {
          PushCSG->Invers = 0;
          T0 = T0_1;
          T1 = T0_2;

          ActiveObject = Push ;
          ActiveCSG = (TCSG_R*)ActiveObject;
          return 1;
        } else
        {
          if ( (T1_2 > T0_1) && (T1_2 < T1_1) )
          {
            PushCSG->Invers = 1;
            T0 = T1_2;
            T1 = T1_1;
            ActiveObject = Push ;
            ActiveCSG = (TCSG_R*)ActiveObject;
            return 1;
          }
        }
      }
    }
  }
  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
  return 0;
}

long TCSGMinus_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{
  if (!IsOpac) {return 0;}

  TObject_R *Push = ActiveObject;
  TCSG_R *PushCSG = ActiveCSG;

  TReal T0_1,T1_1,T0_2,T1_2;
  int CInter1 = 0;
  int CInter2 = 0;
  int Ret = 0;

  TCSG_Active ( PushCSG->Ob1 );

  CInter1 = ActiveObject->GetSecondaryIntersection (Pos,Dir,T0_1,T1_1);

  if (CInter1)
  {
    TCSG_Active ( PushCSG->Ob2 );
    CInter2 = ActiveObject->GetSecondaryIntersection (Pos,Dir,T0_2,T1_2);
  }

  if  ( (CInter1) &&  (!CInter2)  )
    if ((T0_1>0) && (T0_1<1)) Ret = 1;
  if  ( (CInter1) &&  (CInter2)  ) {
    if ( (T0_1 < T0_2) || (T0_1 > T1_2) ) if ((T0_1>0) && (T0_1<1)) Ret = 1;
    if ( (T1_2 > T0_1) && (T1_2 < T1_1) ) if ((T1_2>0) && (T1_2<1)) Ret = 1;
  }

  ActiveObject = Push ;
  ActiveCSG = (TCSG_R*)ActiveObject;
  return Ret;
}

void DestroyMinusDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc CSG of Memory");
#endif
  R_FreeMem (M);
}

TObject_R *CreateMinusDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc CSG in Memory");
#endif
  TCSG_R *TMP = (TCSG_R *) R_GetMem (sizeof(TCSG_R));

  TMP->General.GetColorInfo             = &TCSG_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TCSGMinus_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TCSGMinus_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TCSGMinus_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TCSG_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TCSG_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TCSG_R_SetRGBColor;
  TMP->General.SetTexture               = &TCSG_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TCSG_R_SetProperty;
  TMP->General.Type = R_CSG;
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

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Cube
#define SystemR_V30_Cube
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
#include "rplane.h"
#include "rsphere.h"

typedef struct {

  TObject_R General;
  TPlane_R *Pl1;
  TPlane_R *Pl2;
  TPlane_R *Pl3;
  TPlane_R *Pl4;

  TPlane_R *Pl5;
  TPlane_R *Pl6;

  TSphere_R *Sph;

  TVertex CamPos;
  long    OCheck;
  TVertex Vert1;
  TVertex Vert2;

}TCube_R;

TCube_R *ActiveCube;


void TCube_R_Init ( TReal X1,TReal Y1,TReal Z1,TReal X2,TReal Y2,TReal Z2,byte P1,byte P2)
{

  TObject_R *Push = ActiveObject;

  ActiveCube->Vert1.x = X1;
  ActiveCube->Vert1.y = Y1;
  ActiveCube->Vert1.z = Z1;
  ActiveCube->Vert2.x = X2;
  ActiveCube->Vert2.y = Y2;
  ActiveCube->Vert2.z = Z2;

  ActiveCube->Pl1 = (TPlane_R  *) CreatePlaneDefault();
  ActiveCube->Pl2 = (TPlane_R  *) CreatePlaneDefault();
  ActiveCube->Pl3 = (TPlane_R  *) CreatePlaneDefault();
  ActiveCube->Pl4 = (TPlane_R  *) CreatePlaneDefault();
  ActiveCube->Pl5 = (TPlane_R  *) CreatePlaneDefault();
  ActiveCube->Pl6 = (TPlane_R  *) CreatePlaneDefault();

  ActivePlane   = ActiveCube->Pl1 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl1;
  TPlane_R_Init ( X1,Y1,Z1,0,0,-1,P1,P2);

  ActivePlane   = ActiveCube->Pl2 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl2;
  TPlane_R_Init ( X2,Y2,Z2,0,0,1,P1,P2);

  ActivePlane   = ActiveCube->Pl3 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl3;
  TPlane_R_Init ( X1,Y1,Z1,-1,0,0,P1,P2);

  ActivePlane   = ActiveCube->Pl4 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl4;
  TPlane_R_Init ( X2,Y2,Z2,1,0,0,P1,P2);

  ActivePlane   = ActiveCube->Pl5 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl5;
  TPlane_R_Init ( X1,Y1,Z1,0,-1,0,P1,P2);

  ActivePlane   = ActiveCube->Pl6 ;
  ActiveObject  = (TObject_R*)ActiveCube->Pl6;
  TPlane_R_Init ( X2,Y2,Z2,0,1,0,P1,P2);

  TVertex Dist;
  RVTX_Set ( Dist,X2-X1,Y2-Y1,Z2-Z1 );

  TReal Mod = sqrt(Dist.x*Dist.x + Dist.y*Dist.y + Dist.z*Dist.z)/2.0;
  RVTX_Normalize ( Dist );

  Dist.x *= Mod;
  Dist.y *= Mod;
  Dist.z *= Mod;

  ActiveCube->Sph =(TSphere_R  *) CreateSphereDefault();

  ActiveSphere = ActiveCube->Sph ;
  ActiveObject = (TObject_R*)ActiveCube->Sph ;
  TSphere_R_Init ( X1 + Dist.x,Y1 + Dist.y,Z1 + Dist.z,Mod,P1,P2);


  ActiveCube->General.FLAGS = ( R_RECIVE_SHADOW*P1 | R_MAKE_SHADOW*P2 | R_VISIBLE );


  ActivePlane = ActiveCube->Pl1 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl1 ;
  ActiveCube->Pl1->General.SetTextureMapping (1);
  ActivePlane = ActiveCube->Pl2 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl2 ;
  ActiveCube->Pl2->General.SetTextureMapping (2);
  ActivePlane = ActiveCube->Pl3 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl3 ;
  ActiveCube->Pl3->General.SetTextureMapping (1);
  ActivePlane = ActiveCube->Pl4 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl4 ;
  ActiveCube->Pl4->General.SetTextureMapping (2);

  ActiveObject = Push;

}

void TCube_R_SetTextScale (TReal P1)
{
  TObject_R *Push = ActiveObject;

  ActivePlane = ActiveCube->Pl1 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl1 ;
  ActiveCube->Pl1->General.Pigment.TextScale = P1;
  ActiveCube->Pl1->General.Pigment.InvTextScale = 1.0 / P1;

  ActivePlane = ActiveCube->Pl2 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl2 ;
  ActiveCube->Pl2->General.Pigment.TextScale = P1;
  ActiveCube->Pl2->General.Pigment.InvTextScale = 1.0 / P1;

  ActivePlane = ActiveCube->Pl3 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl3 ;
  ActiveCube->Pl3->General.Pigment.TextScale = P1;
  ActiveCube->Pl3->General.Pigment.InvTextScale = 1.0 / P1;

  ActivePlane = ActiveCube->Pl4 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl4 ;
  ActiveCube->Pl4->General.Pigment.TextScale = P1;
  ActiveCube->Pl4->General.Pigment.InvTextScale = 1.0 / P1;

  ActivePlane = ActiveCube->Pl5 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl5 ;
  ActiveCube->Pl5->General.Pigment.TextScale = P1;
  ActiveCube->Pl5->General.Pigment.InvTextScale = 1.0 / P1;

  ActivePlane = ActiveCube->Pl6 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl6 ;
  ActiveCube->Pl6->General.Pigment.TextScale = P1;
  ActiveCube->Pl6->General.Pigment.InvTextScale = 1.0 / P1;

  ActiveObject = Push ;
}

void TCube_R_SetRGBColor (byte P1,byte P2,byte P3)
{
  TObject_R *Push = ActiveObject;

  ActivePlane = ActiveCube->Pl1 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl1 ;
  ActiveCube->Pl1->General.SetRGBColor (P1,P2,P3);

  ActivePlane = ActiveCube->Pl2 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl2 ;
  ActiveCube->Pl2->General.SetRGBColor (P1,P2,P3);

  ActivePlane = ActiveCube->Pl3 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl3 ;
  ActiveCube->Pl3->General.SetRGBColor (P1,P2,P3);

  ActivePlane = ActiveCube->Pl4 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl4 ;
  ActiveCube->Pl4->General.SetRGBColor (P1,P2,P3);

  ActivePlane = ActiveCube->Pl5 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl5 ;
  ActiveCube->Pl5->General.SetRGBColor (P1,P2,P3);

  ActivePlane = ActiveCube->Pl6 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl6 ;
  ActiveCube->Pl6->General.SetRGBColor (P1,P2,P3);

  ActiveObject = Push ;

}

void TCube_R_SetTexture (rgbcol *P1)
{
  TObject_R *Push = ActiveObject;

  ActivePlane = ActiveCube->Pl1 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl1 ;
  ActiveCube->Pl1->General.SetTexture (P1);
  ActivePlane = ActiveCube->Pl2 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl2 ;
  ActiveCube->Pl2->General.SetTexture (P1);
  ActivePlane = ActiveCube->Pl3 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl3 ;
  ActiveCube->Pl3->General.SetTexture (P1);
  ActivePlane = ActiveCube->Pl4 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl4 ;
  ActiveCube->Pl4->General.SetTexture (P1);
  ActivePlane = ActiveCube->Pl5 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl5 ;
  ActiveCube->Pl5->General.SetTexture (P1);
  ActivePlane = ActiveCube->Pl6 ;
  ActiveObject = (TObject_R*)ActiveCube->Pl6 ;
  ActiveCube->Pl6->General.SetTexture (P1);

  ActiveObject = Push ;

}

void TCube_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{
  TObject_R *Push = ActiveObject;

  switch (ActiveCube->OCheck)
  {
    case 0:
        ActivePlane = ActiveCube->Pl1 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl1 ;
        ActiveCube->Pl1->General.GetColorInfo(P1,CH); break;
    case 1:
        ActivePlane = ActiveCube->Pl2 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl2 ;
        ActiveCube->Pl2->General.GetColorInfo(P1,CH); break;
    case 2:
        ActivePlane = ActiveCube->Pl3 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl3 ;
        ActiveCube->Pl3->General.GetColorInfo(P1,CH); break;
    case 3:
        ActivePlane = ActiveCube->Pl4 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl4 ;
        ActiveCube->Pl4->General.GetColorInfo(P1,CH); break;
    case 4:
        ActivePlane = ActiveCube->Pl5 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl5 ;
        ActiveCube->Pl5->General.GetColorInfo(P1,CH); break;
    case 5:
        ActivePlane = ActiveCube->Pl6 ;
        ActiveObject = (TObject_R*)ActiveCube->Pl6 ;
        ActiveCube->Pl6->General.GetColorInfo(P1,CH); break;
  }
  if (IsReflectT)
  {
    ActiveCube->General.FLAGS = ActiveCube->General.FLAGS & Not(R_REFLECT) ;
    if (P1.Color.B.A)
    {
      ActiveCube->General.REFL_INTS = P1.Color.B.A;
      ActiveCube->General.FLAGS = ActiveCube->General.FLAGS | R_REFLECT ;
    }
  }
  ActiveObject = Push ;
}

void TCube_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
  TObject_R *Push = ActiveObject;
  switch (ActiveCube->OCheck)
  {
    case 0:
        ActivePlane = ActiveCube->Pl1 ;ActiveCube->Pl1->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
    case 1:
        ActivePlane = ActiveCube->Pl2 ;ActiveCube->Pl2->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
    case 2:
        ActivePlane = ActiveCube->Pl3 ;ActiveCube->Pl3->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
    case 3:
        ActivePlane = ActiveCube->Pl4 ;ActiveCube->Pl4->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
    case 4:
        ActivePlane = ActiveCube->Pl5 ;ActiveCube->Pl5->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
    case 5:
        ActivePlane = ActiveCube->Pl6 ;ActiveCube->Pl6->General.GetNormal (Pos,Norm,ValNorm,ActiveCube->General.Invers);break;
  }
  ActiveObject = Push ;
}

void TCube_R_SetValuesIntersection (TVertex  &Pos)
{
  TObject_R *Push = ActiveObject;
  ActiveSphere = ActiveCube->Sph ;ActiveCube->Sph->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl1 ;ActiveCube->Pl1->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl2 ;ActiveCube->Pl2->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl3 ;ActiveCube->Pl3->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl4 ;ActiveCube->Pl4->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl5 ;ActiveCube->Pl5->General.SetValuesIntersection (Pos);
  ActivePlane = ActiveCube->Pl6 ;ActiveCube->Pl6->General.SetValuesIntersection (Pos);
  RVTX_Copy (ActiveCube->CamPos,Pos);
  ActiveObject = Push ;
}
long TCube_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;


  if (!IsVisible) return 0;

  TReal T0_1,T1_1,T0_2,T1_2;
  TReal TMin = 20000, TMax = -2000;
  int FF = 0,A1=0,A2=0,A3=0,A4=0,A5=0,A6=0;

  TReal P1 = 20000,P2 = 20000,P3 = 20000,P4 = 20000,P5 = 20000,P6 = 20000;
  TVertex Check;

  ActiveSphere = ActiveCube->Sph;

  if (ActiveCube->Sph->General.GetPrimaryIntersection (Dir,T0_1,T1_1))
  {
    ActivePlane = ActiveCube->Pl1 ;A1 = ActiveCube->Pl1->General.GetPrimaryIntersection (Dir,P1,T1_2);
    ActivePlane = ActiveCube->Pl2 ;A2 = ActiveCube->Pl2->General.GetPrimaryIntersection (Dir,P2,T1_2);
    ActivePlane = ActiveCube->Pl3 ;A3 = ActiveCube->Pl3->General.GetPrimaryIntersection (Dir,P3,T1_2);
    ActivePlane = ActiveCube->Pl4 ;A4 = ActiveCube->Pl4->General.GetPrimaryIntersection (Dir,P4,T1_2);
    ActivePlane = ActiveCube->Pl5 ;A5 = ActiveCube->Pl5->General.GetPrimaryIntersection (Dir,P5,T1_2);
    ActivePlane = ActiveCube->Pl6 ;A6 = ActiveCube->Pl6->General.GetPrimaryIntersection (Dir,P6,T1_2);

    if ((A1) &&  (P1<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P1*Dir.x,ActiveCube->CamPos.y + P1*Dir.y,ActiveCube->CamPos.z + P1*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
        TMin = P1;
        FF = 1;
        ActiveCube->OCheck = 0;
      }
    }
    if ((A2) &&  (P2<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P2*Dir.x,ActiveCube->CamPos.y + P2*Dir.y,ActiveCube->CamPos.z + P2*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
        TMin = P2;
        FF = 1;
        ActiveCube->OCheck = 1;
      }
    }
    if ((A3) &&  (P3<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P3*Dir.x,ActiveCube->CamPos.y + P3*Dir.y,ActiveCube->CamPos.z + P3*Dir.z);
      if ( (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P3;
        FF = 1;
        ActiveCube->OCheck = 2;
      }
    }
    if ((A4) &&  (P4<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P4*Dir.x,ActiveCube->CamPos.y + P4*Dir.y,ActiveCube->CamPos.z + P4*Dir.z);
      if (
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P4;
        FF = 1;
        ActiveCube->OCheck = 3;
      }
    }

    if ((A5) &&  (P5<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P5*Dir.x,ActiveCube->CamPos.y + P5*Dir.y,ActiveCube->CamPos.z + P5*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P5;
        FF = 1;
        ActiveCube->OCheck = 4;
      }
    }if ((A6) &&  (P6<TMin) )
    {
      RVTX_Set (Check,ActiveCube->CamPos.x + P6*Dir.x,ActiveCube->CamPos.y + P6*Dir.y,ActiveCube->CamPos.z + P6*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P6;
        FF = 1;
        ActiveCube->OCheck = 5;
      }
    }


    if (FF){
        T0 = TMin;
        T1 = TMax;
        ActiveObject = Push ;
        return 1;
      }



  }

  ActiveObject = Push ;
  return 0;

}
long TCube_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;
  if (!IsVisible) return 0;

  TReal T0_1,T1_1,T0_2,T1_2;
  TReal TMin = 2000, TMax = -2000;

  int FF = 0,A1,A2,A3,A4,A5,A6;

  TReal P1 = 20000,P2 = 20000,P3 = 20000,P4 = 20000,P5 = 20000,P6 = 20000;
  TVertex Check;

  ActiveSphere = ActiveCube->Sph;


  if (ActiveCube->Sph->General.GetSecondaryIntersection (Pos,Dir,T0_1,T1_1))
  {
    ActivePlane = ActiveCube->Pl1 ;A1 = ActiveCube->Pl1->General.GetSecondaryIntersection (Pos,Dir,P1,T1_2);
    ActivePlane = ActiveCube->Pl2 ;A2 = ActiveCube->Pl2->General.GetSecondaryIntersection (Pos,Dir,P2,T1_2);
    ActivePlane = ActiveCube->Pl3 ;A3 = ActiveCube->Pl3->General.GetSecondaryIntersection (Pos,Dir,P3,T1_2);
    ActivePlane = ActiveCube->Pl4 ;A4 = ActiveCube->Pl4->General.GetSecondaryIntersection (Pos,Dir,P4,T1_2);
    ActivePlane = ActiveCube->Pl5 ;A5 = ActiveCube->Pl5->General.GetSecondaryIntersection (Pos,Dir,P5,T1_2);
    ActivePlane = ActiveCube->Pl6 ;A6 = ActiveCube->Pl6->General.GetSecondaryIntersection (Pos,Dir,P6,T1_2);

    if (A1 && (P1 < TMin))
    {
      RVTX_Set (Check,Pos.x + P1*Dir.x,Pos.y + P1*Dir.y,Pos.z + P1*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
        TMin = P1;
        FF = 1;
        ActiveCube->OCheck = 0;
      }
    }
    if (A2 && (P2 < TMin))
    {
      RVTX_Set (Check,Pos.x + P2*Dir.x,Pos.y + P2*Dir.y,Pos.z + P2*Dir.z);
      if (
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x)
         )
      {
        TMin = P2;
        FF = 1;
        ActiveCube->OCheck = 1;
      }
    }
    if (A3 && (P3 < TMin))
    {
      RVTX_Set (Check,Pos.x + P3*Dir.x,Pos.y + P3*Dir.y,Pos.z + P3*Dir.z);
      if ( (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
        TMin = P3;
        FF = 1;
        ActiveCube->OCheck = 2;
      }
    }
    if (A4 && (P4 < TMin))
    {
      RVTX_Set (Check,Pos.x + P4*Dir.x,Pos.y + P4*Dir.y,Pos.z + P4*Dir.z);
      if (
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P4;
        FF = 1;
        ActiveCube->OCheck = 3;
      }
    }
    if (A5 && (P5 < TMin))
    {
      RVTX_Set (Check,Pos.x + P5*Dir.x,Pos.y + P5*Dir.y,Pos.z + P5*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P5;
        FF = 1;
        ActiveCube->OCheck = 4;
      }
    }
    if (A6 && (P6 < TMin))
    {
      RVTX_Set (Check,Pos.x + P6*Dir.x,Pos.y + P6*Dir.y,Pos.z + P6*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        TMin = P6;
        FF = 1;
        ActiveCube->OCheck = 5;
      }
    }
    if (FF){
        T0 = TMin;
        T1 = TMax;
        ActiveObject = Push ;
        return 1;
      }
  }
  ActiveObject = Push ;
  return 0;

}


long TCube_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir)
{
  TObject_R *Push = ActiveObject;
  if (!IsOpac) {return 0;}

  TReal T0_1,T1_1,T0_2,T1_2;
  TReal TMin = 2000, TMax = -2000;

  int A1,A2,A3,A4,A5,A6;

  TReal P1 = 20000,P2 = 20000,P3 = 20000,P4 = 20000,P5 = 20000,P6 = 20000;
  TVertex Check;

  ActiveSphere = ActiveCube->Sph;

  if (ActiveCube->Sph->General.GetSecondaryIntersection (Pos,Dir,T0_1,T1_1))
  {
    ActivePlane = ActiveCube->Pl1 ;A1 = ActiveCube->Pl1->General.GetSecondaryIntersection (Pos,Dir,P1,T1_2);
    ActivePlane = ActiveCube->Pl2 ;A2 = ActiveCube->Pl2->General.GetSecondaryIntersection (Pos,Dir,P2,T1_2);
    ActivePlane = ActiveCube->Pl3 ;A3 = ActiveCube->Pl3->General.GetSecondaryIntersection (Pos,Dir,P3,T1_2);
    ActivePlane = ActiveCube->Pl4 ;A4 = ActiveCube->Pl4->General.GetSecondaryIntersection (Pos,Dir,P4,T1_2);
    ActivePlane = ActiveCube->Pl5 ;A5 = ActiveCube->Pl5->General.GetSecondaryIntersection (Pos,Dir,P5,T1_2);
    ActivePlane = ActiveCube->Pl6 ;A6 = ActiveCube->Pl6->General.GetSecondaryIntersection (Pos,Dir,P6,T1_2);

    if ((A1>0) && (A1<1))
    {
      RVTX_Set(Check,Pos.x + P1*Dir.x,Pos.y + P1*Dir.y,Pos.z + P1*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
          ActiveObject = Push ;
          return 1;
      }
    }
    if ((A2>0) && (A2<1))
    {
      RVTX_Set(Check,Pos.x + P2*Dir.x,Pos.y + P2*Dir.y,Pos.z + P2*Dir.z);
      if (
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x)
         )
      {
          ActiveObject = Push ;
          return 1;
      }
    }
    if ((A3>0) && (A3<1))
    {
      RVTX_Set(Check,Pos.x + P3*Dir.x,Pos.y + P3*Dir.y,Pos.z + P3*Dir.z);
      if ( (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z) &&
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y)
         )
      {
          ActiveObject = Push ;
          return 1;
      }
    }
    if ((A4>0) && (A4<1))
    {
      RVTX_Set(Check,Pos.x + P4*Dir.x,Pos.y + P4*Dir.y,Pos.z + P4*Dir.z);
      if (
           (Check.y >= ActiveCube->Vert1.y) && (Check.y <= ActiveCube->Vert2.y) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
          ActiveObject = Push ;
          return 1;
      }
    }
    if ((A5>0) && (A5<1))
    {
      RVTX_Set(Check,Pos.x + P5*Dir.x,Pos.y + P5*Dir.y,Pos.z + P5*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        ActiveObject = Push ;
        return 1;
      }
    }
    if ((A6>0) && (A6<1))
    {
      RVTX_Set(Check,Pos.x + P6*Dir.x,Pos.y + P6*Dir.y,Pos.z + P6*Dir.z);
      if ( (Check.x >= ActiveCube->Vert1.x) && (Check.x <= ActiveCube->Vert2.x) &&
           (Check.z >= ActiveCube->Vert1.z) && (Check.z <= ActiveCube->Vert2.z)
         )
      {
        ActiveObject = Push ;
        return 1;
      }
    }
  }
  ActiveObject = Push ;
  return 0;
}
void DestroyCubeDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Cube of Memory");
#endif

  R_FreeMem (M);
}

TObject_R *CreateCubeDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Cube in Memory");
#endif

  TCube_R *TMP = (TCube_R *) R_GetMem (sizeof(TCube_R));

  TMP->General.GetColorInfo             = &TCube_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TCube_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TCube_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TCube_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TCube_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TCube_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TCube_R_SetRGBColor;
  TMP->General.SetTexture               = &TCube_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TCube_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_CUBE;
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

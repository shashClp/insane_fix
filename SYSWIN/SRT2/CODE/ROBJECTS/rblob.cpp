//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "rblob.h"

TBlob_R *ActiveBlob;


void TBlob_R_Init ( byte P1,byte P2)
{

  TObject_R *Push = ActiveObject;

  ActiveBlob->NumComponents = 0;


  ActiveBlob->General.FLAGS = ( R_RECIVE_SHADOW*P1 | R_MAKE_SHADOW*P2 | R_VISIBLE );

  ActiveObject = Push ;
}

void TBlob_R_AddComponent ( TReal X1,TReal Y1,TReal Z1,TReal Dens)
{

  TObject_R *Push = ActiveObject;

  if (ActiveBlob->NumComponents < 20 )
  {
    ActiveBlob->Pl1[ActiveBlob->NumComponents] =(TSphere_R  *) CreateSphereDefault();
    ActiveSphere = ActiveBlob->Pl1[ActiveBlob->NumComponents]  ;
    ActiveObject = (TObject_R*)ActiveBlob->Pl1[ActiveBlob->NumComponents]  ;
    TSphere_R_Init ( X1 ,Y1 , Z1,1,1,1);
    ActiveBlob->Dens[ActiveBlob->NumComponents] = Dens;
    ActiveBlob->NumComponents ++;
  }
  ActiveBlob->NumComponents2 = ActiveBlob->NumComponents*ActiveBlob->NumComponents;
  ActiveObject = Push ;
}

void TBlob_R_SetTextScale (TReal P1)
{
  TObject_R *Push = ActiveObject;

  ActiveObject = Push ;
}

void TBlob_R_SetRGBColor (byte P1,byte P2,byte P3)
{
  TObject_R *Push = ActiveObject;


  ActiveObject = Push ;

}

void TBlob_R_SetTexture (rgbcol *P1)
{
  TObject_R *Push = ActiveObject;

  ActiveObject = Push ;

}

void TBlob_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{
  TObject_R *Push = ActiveObject;
  word u,v;
  TVertex RPos;

  if (ActiveBlob->General.Pigment.TexMap != NULL )
  {
    RVTX_Copy (RPos,CH);
    RVTX_Sub ( RPos , ActiveBlob->General.Position );

    u = ((word)(ActiveBlob->Fact1*(atan(RPos.y/RPos.z)) )) ;
    v = ((word)(ActiveBlob->Fact1*(atan(RPos.x/RPos.z)) )) ;

    u = (u) & 127;
    v = (v) & 127;

    P1.Color.ColRGB = ActiveBlob->General.Pigment.TexMap[((u)*128+(v))].ColRGB;
  } else {
    P1.Color.ColRGB = ActiveBlob->General.Pigment.Color.ColRGB;
  }

  ActiveObject = Push ;
}

void TBlob_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
  TObject_R *Push = ActiveObject;

  TVertex Norm1;
  RVTX_Set (Norm,0,0,0);
  float  Dist1 ;
  for (long i = 0; i < ActiveBlob->NumComponents; i++)
  {
    ActiveSphere = ActiveBlob->Pl1[i] ;ActiveBlob->Pl1[1]->General.GetNormal (Pos,Norm1,ValNorm,ActiveBlob->General.Invers);
    //Dist1 = RVTX_Distance (Pos,ActiveBlob->Pl1[i]->General.Position);
    Dist1 = ActiveBlob->Dens[i]/sqr(RVTX_Distance (Pos,ActiveBlob->Pl1[i]->General.Position));
    Norm.x  += Norm1.x*Dist1 ;
    Norm.y  += Norm1.y*Dist1 ;
    Norm.z  += Norm1.z*Dist1 ;
  }
  RVTX_Normalize (Norm);
  ActiveObject = Push ;
}

void TBlob_R_SetValuesIntersection (TVertex  &Pos)
{
  TObject_R *Push = ActiveObject;

  for (long i = 0; i < ActiveBlob->NumComponents; i++)
  {
    ActiveSphere = ActiveBlob->Pl1[i] ;ActiveBlob->Pl1[i]->General.SetValuesIntersection (Pos);

  }
  RVTX_Copy (ActiveBlob->CamPos,Pos);
  ActiveBlob->Fact1 = 128.0*InvR_PI*ActiveBlob->General.Pigment.TextScale;
  ActiveObject = Push ;
}
long TBlob_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;


  if (!IsVisible) return 0;

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;
  TVertex Check;
  float Dist1;

  for (long i = 0; i < ActiveBlob->NumComponents; i++)
  {

    ActiveSphere = ActiveBlob->Pl1[i];

    A1 = TSphere_R_GetPrimaryIntersection (Dir,T0_1,T1_1);
    if (A1)
    {
        FF = 1;
        if (T0_1 < TMin) TMin = T0_1;
        if (T1_1 > TMax) TMax = T1_1;
    }
  }



  if (FF)
  {
    for (TMin0 = TMin; TMin0 < TMax; TMin0 += BLOB_QUALITY)
    {
        RVTX_Set (Check,ActiveBlob->CamPos.x + TMin0*Dir.x,ActiveBlob->CamPos.y + TMin0*Dir.y,ActiveBlob->CamPos.z + TMin0*Dir.z);
        Dist1 = 0;
        for (long i = 0; i < ActiveBlob->NumComponents; i++)
        {
  //         Dist1 += ActiveBlob->Dens[i] / (RVTX_Distance (Check,ActiveBlob->Pl1[i]->General.Position));
             Dist1 += ActiveBlob->Dens[i] / (RVTX_SemiDistance (Check,ActiveBlob->Pl1[i]->General.Position));

    }
        if (Dist1>ActiveBlob->NumComponents) {
            T0 = TMin0;
            T1 = TMax ;
            ActiveObject = Push ;
            return 1;
        }
    }
  }
  ActiveObject = Push ;
  return 0;

}
long TBlob_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;
  if (!IsVisible) return 0;

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;

  TReal P1 = 20000,P2 = 20000,P3 = 20000,P4 = 20000,P5 = 20000,P6 = 20000;
  TVertex Check;
  float Dist1;
  float Dist2 = 0;

  for (long i = 0; i < ActiveBlob->NumComponents; i++)
  {

    ActiveSphere = ActiveBlob->Pl1[i];

    A1 = TSphere_R_GetSecondaryIntersection (Pos,Dir,T0_1,T1_1);
    if (A1)
    {
        FF = 1;
        if (T0_1 < TMin) TMin = T0_1;
        if (T1_1 > TMax) TMax = T1_1;
    }
  }


  if (FF)
  {

    for (TMin0 = TMin; TMin0 < TMax; TMin0 +=0.1)
    {
        RVTX_Set (Check,Pos.x + TMin0*Dir.x,Pos.y + TMin0*Dir.y,Pos.z + TMin0*Dir.z);
        Dist1 = 0;
        for (long i = 0; i < ActiveBlob->NumComponents; i++)
        {
         Dist1 += ActiveBlob->Dens[i] / (RVTX_Distance (Check,ActiveBlob->Pl1[i]->General.Position));
        }
        if (Dist1>ActiveBlob->NumComponents) {
            T0 = TMin0;
            T1 = TMax ;
            ActiveObject = Push ;
            return 1;
        }
    }
  }

  ActiveObject = Push ;
  return 0;

}


long TBlob_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir3)
{
  TObject_R *Push = ActiveObject;
  if (!IsOpac) {return 0;}

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;

  TReal P1 = 20000,P2 = 20000,P3 = 20000,P4 = 20000,P5 = 20000,P6 = 20000;
  TVertex Check;
  TVertex Dir2;

  float Dist1;

  float Dist2 = 0;
  RVTX_Copy (Dir2,Dir3);
  RVTX_Normalize (Dir2);
  for (long i = 0; i < ActiveBlob->NumComponents; i++)
  {

    ActiveSphere = ActiveBlob->Pl1[i];

    A1 = TSphere_R_GetSecondaryIntersection (Pos,Dir2,T0_1,T1_1);
    if (A1)
    {
        FF = 1;
        if (T0_1 < TMin) TMin = T0_1;
        if (T1_1 > TMax) TMax = T1_1;
    }
  }


  if (FF)
  {

    for (TMin0 = TMin; TMin0 < TMax; TMin0 +=0.4)
    {
        RVTX_Set (Check,Pos.x + TMin0*Dir2.x,Pos.y + TMin0*Dir2.y,Pos.z + TMin0*Dir2.z);
        Dist1 = 0;
        for (long i = 0; i < ActiveBlob->NumComponents; i++)
        {
         Dist1 += ActiveBlob->Dens[i] / (RVTX_Distance (Check,ActiveBlob->Pl1[i]->General.Position));
        }
        if (Dist1>ActiveBlob->NumComponents) {
            ActiveObject = Push ;
            return 1;
        }
    }
  }

  ActiveObject = Push ;
  return 0;
}

void DestroyBlobDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Blob of Memory : ");
#endif
  for (long i =0; i < ((TBlob_R *)M)->NumComponents; i++)
  {
#ifdef R_LOG_DEF
    R_LOG ("---- DeAlloc ComponentBlob of Memory");
#endif
    DestroySphereDefault((TObject_R *)((TBlob_R *)M)->Pl1[i]);
  }
  R_FreeMem (M);
}

TObject_R *CreateBlobDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Blob in Memory");
#endif

  TBlob_R *TMP = (TBlob_R *) R_GetMem (sizeof(TBlob_R));

  TMP->General.GetColorInfo             = &TBlob_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TBlob_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TBlob_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TBlob_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TBlob_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TBlob_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_BLOB;
  TMP->General.Invers = 0;
  TMP->General.Pigment.TxtMode = 0;
  TMP->General.Pigment.TextScale = 10;
  TMP->General.Pigment.InvTextScale = 1.0 / TMP->General.Pigment.TextScale ;
  TMP->General.Pigment.TexMap = NULL;
  TMP->General.PNG = 0;
  TMP->General.Value1 = 0;
  TMP->Fact1 = 128.0*InvR_PI*TMP->General.Pigment.TextScale;
#ifdef SYR_PROCEDURAL_TEXTURES
  TMP->General.Pigment.Texture = NULL;
#endif
  return (TObject_R *)TMP;
}


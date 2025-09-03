//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Sor
#define SystemR_V30_Sor
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "robject.h"
#include "rsphere.h"
#define NumMaxPoints 200
#define SOR_QUALITY 1.0

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef struct {

  TObject_R General;
  TVertex Punts[NumMaxPoints];
  TVertex Normals[NumMaxPoints];
  long    NumPunts;
  long    PuntCH;
  TSphere_R  *Bounder;
  TVertex CamPos;
  TReal Fact1;
}TSor_R;


TSor_R *ActiveSor;


TSor_R_Init (TReal X,TReal Y,TReal Z, byte P1,byte P2)
{

  TObject_R *Push = ActiveObject;
  RVTX_Set (ActiveObject->Position,X,Y,Z);
  ActiveSor->Bounder =(TSphere_R  *) CreateSphereDefault();
  ActiveSor->NumPunts = 0;
  ActiveSor->General.FLAGS = ( R_RECIVE_SHADOW*P1 | R_MAKE_SHADOW*P2 | R_VISIBLE );
  ActiveObject = Push ;
}
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TVertex Bezier (float U,int N,TVertex *PC);


TSor_R_SetPoints ( TVertex *Points, long NumPoints )
{

  TObject_R *Push = ActiveObject;
  TReal Distance = 0;
  TReal Distance1 = 0;
  TReal Delta1 = 1.0  / (TReal)NumMaxPoints  ;
  TReal Delta2 = 0  ;
  TVertex Punt;
  TVertex Punt0;
  TVertex Punt1;

  Punt0.x = 0;
  Punt0.y = ActiveSor->General.Position.y;
  Punt0.z = 0;
/*
  for (long i = 0;i<NumMaxPoints; i++)
  {
    Punt = Bezier (Delta2,NumPoints,Points);
    Delta2 += Delta1;
    ActiveSor->Punts[i].z = 0;
    ActiveSor->Punts[i].x = Punt.x;
    ActiveSor->Punts[i].y = ActiveSor->General.Position.y + Punt.y;
    Distance1 = RVTX_Distance (Punt0,ActiveSor->Punts[i]);
    if (Distance1 > Distance) Distance=Distance1;
  }
*/

  for (long i = 0;i<NumMaxPoints; i++)
  {
    Punt.x = 3+sin(i*R_PI/50.0)*1 + cos(i*R_PI/250.0)*2;
    Punt.y = (-1+i/20.0);
    ActiveSor->Punts[i].z = 0;
    ActiveSor->Punts[i].x = Punt.x;
    ActiveSor->Punts[i].y = ActiveSor->General.Position.y + Punt.y;
    Distance1 = RVTX_Distance (Punt0,ActiveSor->Punts[i]);
    if (Distance1 > Distance) Distance=Distance1;
  }

  Punt0.x = 0;
  Punt0.y = 0;
  Punt0.z = -100;
  for (i = 0;i<NumMaxPoints-1; i++)
  {
    RVTX_Copy(Punt,ActiveSor->Punts[i+1]);
    RVTX_Sub(Punt,ActiveSor->Punts[i]);
    RVTX_CrossProduct (ActiveSor->Normals[i],Punt0,Punt);
  }

  ActiveSphere = ActiveSor->Bounder;
  ActiveObject = (TObject_R*)ActiveSor->Bounder;

  TSphere_R_Init ( ActiveSor->General.Position.x,ActiveSor->General.Position.y,ActiveSor->General.Position.z,Distance,1,1);

  ActiveObject = Push ;

}
TReal TSor_GetRadi ( TVertex Point )
{
//  return (0.6+sin(Point.y*R_PIx2)*0.5);

  for (long i = 0; (i < NumMaxPoints-1) ; i++)
  {
    if ( (Point.y >= ActiveSor->Punts[i].y  ) &&
         (Point.y <= ActiveSor->Punts[i+1].y  ) )
       {
       ActiveSor->PuntCH =  i;
        return ActiveSor->Punts[i].x;
       }
  }
  return 0;
}
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
void TSor_R_SetTextScale (TReal P1)
{
  TObject_R *Push = ActiveObject;

  ActiveObject = Push ;
}

void TSor_R_SetRGBColor (byte P1,byte P2,byte P3)
{
  TObject_R *Push = ActiveObject;


  ActiveObject = Push ;

}

void TSor_R_SetTexture (rgbcol *P1)
{
  TObject_R *Push = ActiveObject;

  ActiveObject = Push ;

}

void TSor_R_GetInfo ( RayInfo &P1 , TVertex &CH )
{
  TObject_R *Push = ActiveObject;
  word u,v;
  TVertex RPos;

  //P1.Color.ColRGB = ActiveObject->Pigment.Color.ColRGB;
  if (ActiveSor->General.Pigment.TexMap != NULL )
  {
    RVTX_Copy (RPos,CH);
    RVTX_Sub ( RPos , ActiveSor->General.Position );
    u = ((word)(ActiveSor->Fact1*(atan(RPos.y/RPos.z)) )) & 127;
    v = ((word)(ActiveSor->Fact1*(atan(RPos.x/RPos.z)) )) & 127;
    P1.Color.ColRGB = ActiveSor->General.Pigment.TexMap[((u)*128+(v))].ColRGB;
  } else {
    P1.Color.ColRGB = ActiveSor->General.Pigment.Color.ColRGB;
  }

  ActiveObject = Push ;
}

void TSor_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
  TObject_R *Push = ActiveObject;

    Norm.x = Pos.x - ActiveSor->General.Position.x;
    Norm.z = Pos.z - ActiveSor->General.Position.z;
    Norm.y = ActiveSor->Normals[ActiveSor->PuntCH].y;
    RVTX_Normalize (Norm);
    ActiveObject = Push ;
}

void TSor_R_SetValuesIntersection (TVertex  &Pos)
{
  TObject_R *Push = ActiveObject;
  ActiveSphere = ActiveSor->Bounder ;ActiveSor->Bounder->General.SetValuesIntersection (Pos);
  RVTX_Copy (ActiveSor->CamPos,Pos);
  ActiveSor->Fact1 = 128.0*InvR_PI*ActiveSor->General.Pigment.TextScale;
  ActiveObject = Push ;
}

long TSor_R_GetPrimaryIntersection (TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;


  if (!IsVisible) return 0;

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;
  TVertex Check;
  float Dist1;

  ActiveSphere = ActiveSor->Bounder;
  A1 = TSphere_R_GetPrimaryIntersection (Dir,T0_1,T1_1);

  if (A1)
  {

    for (TMin0 = T0_1; TMin0 < T1_1; TMin0 += SOR_QUALITY)
    {
        RVTX_Set (Check,ActiveSor->CamPos.x + TMin0*Dir.x,ActiveSor->CamPos.y + TMin0*Dir.y,ActiveSor->CamPos.z + TMin0*Dir.z);

         if (RVTX_DistanceXZ(Check,ActiveSor->General.Position)< TSor_GetRadi ( Check ) )
        {
            T0 = TMin0;
            T1 = T1_1 ;
            ActiveObject = Push ;
            return 1;
        }
    }
  }
  ActiveObject = Push ;
  return 0;

}
long TSor_R_GetSecondaryIntersection (TVertex  &Pos,TVertex  &Dir,TReal &T0,TReal &T1)
{
  TObject_R *Push = ActiveObject;
  if (!IsVisible) return 0;

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;
  TVertex Check;
  float Dist1;

  ActiveSphere = ActiveSor->Bounder;
  A1 = TSphere_R_GetSecondaryIntersection (Pos,Dir,T0_1,T1_1);

  if (A1)
  {

    for (TMin0 = T0_1; TMin0 < T1_1; TMin0 += SOR_QUALITY)
    {
        RVTX_Set (Check,Pos.x + TMin0*Dir.x,Pos.y + TMin0*Dir.y,Pos.z + TMin0*Dir.z);

         if (RVTX_DistanceXZ(Check,ActiveSor->General.Position)< TSor_GetRadi ( Check ) )
//        if (fabs(Check.x-ActiveSor->General.Position.x)< TSor_GetRadi ( Check ) )
        {
            T0 = TMin0;
            T1 = T1_1 ;
            ActiveObject = Push ;
            return 1;
        }
    }
  }


  ActiveObject = Push ;
  return 0;

}


long TSor_R_GetShadowIntersection (TVertex  &Pos,TVertex  &Dir2)
{
  TObject_R *Push = ActiveObject;
  if (!IsOpac) {return 0;}

  TReal T0_1,T1_1;
  TReal TMin0 = 20000,TMin = 20000, TMax = -2000;
  int FF = 0,A1;
  TVertex Check;
  TVertex Dir;
  float Dist1;
  RVTX_Copy (Dir,Dir2);
  RVTX_Normalize (Dir);
  ActiveSphere = ActiveSor->Bounder;
  A1 = TSphere_R_GetSecondaryIntersection (Pos,Dir,T0_1,T1_1);

  if (A1)
  {
//    ActiveObject = Push ;
//    return 1;
    for (TMin0 = T0_1; TMin0 < T1_1; TMin0 +=0.2)
    {
        RVTX_Set (Check,Pos.x + TMin0*Dir.x,Pos.y + TMin0*Dir.y,Pos.z + TMin0*Dir.z);

         if (RVTX_DistanceXZ(Check,ActiveSor->General.Position)< TSor_GetRadi ( Check ) )
//        if (fabs(Check.x-ActiveSor->General.Position.x)< TSor_GetRadi ( Check ) )
        {
            ActiveObject = Push ;
            return 1;
        }
    }
  }
  ActiveObject = Push ;
  return 0;

  ActiveObject = Push ;
  return 0;
}
void DestroySorDefault(TObject_R*M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc SOR of Memory");
#endif

  R_FreeMem (M);
}

TObject_R *CreateSorDefault()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc SOR in Memory");
#endif

  TSor_R *TMP = (TSor_R *) R_GetMem (sizeof(TSor_R));

  TMP->General.GetColorInfo             = &TSor_R_GetInfo;
  TMP->General.GetPrimaryIntersection   = &TSor_R_GetPrimaryIntersection;
  TMP->General.GetSecondaryIntersection = &TSor_R_GetSecondaryIntersection;
  TMP->General.GetShadowIntersection    = &TSor_R_GetShadowIntersection;
  TMP->General.GetNormal                = &TSor_R_GetNormal;
  TMP->General.SetValuesIntersection    = &TSor_R_SetValuesIntersection;
  TMP->General.SetRGBColor              = &TObject_R_SetRGBColor;
  TMP->General.SetTexture               = &TObject_R_SetTexture;
  TMP->General.SetTextureMapping        = &TObject_R_SetTextureMapping;
  TMP->General.SetTextScale             = &TObject_R_SetTextScale;
  TMP->General.SetBump                  = &TObject_R_SetBump;
  TMP->General.SetProperty              = &TObject_R_SetProperty;
  TMP->General.Type = R_SOR;
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




float Comb (int N,int I)
{
int Cmt;
float Tmp=1;

    for (Cmt=I+1;Cmt<=N;Cmt++) {Tmp=Tmp*((float)Cmt);}
    for (Cmt=1;Cmt<=(N-I);Cmt++) {Tmp=Tmp /((float)Cmt);}
    return (Tmp);
}

float Blend (int I,int N,float U)
{
int Cmt;float V;

    V=Comb(N,I);
    for(Cmt=1;Cmt<=I;Cmt++) {V=V*U;}
    for(Cmt=1;Cmt<=(N-I);Cmt++) {V=V*(1-U);}
    return (V);
}

TVertex Bezier (float U,int N,TVertex *PC)
{
int Cmt;float B;
TVertex *Tmp,Tmp2;

  Tmp=PC;
  Tmp2.x=Tmp2.y=Tmp2.z=0;

  for (Cmt=0;Cmt<=N;Cmt++)
      {
      B=Blend (Cmt,N,U);
      Tmp2.x+=(Tmp->x)*B;Tmp2.y+=(Tmp->y)*B;Tmp2.z+=(Tmp->z)*B;
      Tmp++;
      }
  return (Tmp2);
}

#endif

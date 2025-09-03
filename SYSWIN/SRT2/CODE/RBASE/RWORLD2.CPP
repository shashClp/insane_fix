//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include <time.h>
#include "..\RDEFINES\RDEFINES.H"
#include "RSCENE.H"
#include "RSCENE.CPP"

#include "..\REFECTS\RLIGHT.H"
#include "RCAMERA.H"
#include "RWORLD.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef SYR_PROCEDURAL_TEXTURES
#include "..\REFECTS\RTEXT.CPP"
#endif

#define NumMaxObjectes 20000
#define NumMaxLight 10
#define NumMaxWorldList 10
#define ERRORCODE 6565                  // Cal controlar... si es vol.

long R_RAY_DIM = 1;

long R_ScreenX = 640;
long R_ScreenY = 480;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

TWorld_R    *ActiveWorld;
long        IDActiveObject;
long        IDActiveLight;
TWorld_R    *WorldList[NumMaxWorldList];
long        IndexWorldList = 0;


long        RPrimRay = 0;
long        RShadRay = 0;
long        RSecRay = 0;
long        RHitPrimRay = 0;
long        RHitSecRay = 0;
long        RTestInt1 = 0;
long        RTestInt2 = 0;
long        RTestInt3 = 0;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef SYR_PROCEDURAL_TEXTURES
#include "..\REFECTS\RTEXT.H"
#endif

#include "RSET.H"
#include "RGET.H"
#include "RBOUNDER.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

long RBorderV ;
long RBorderH ;
float R_FAR_CLIP = 10000.0;

long  R_FOG = 0;
float R_FOG_DISTANCE_FACTOR= 2;
float R_FOG_R;
float R_FOG_B;
float R_FOG_G;
long R_BACK_COLOR;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

inline TReal    RAngle ( TVertex &P1, TVertex &P2 );
inline long     TWorld_IsShadowed (TVertex &Check,TVertex  &Rest,long &ID);
inline void     TWorld_CalcLight (rgbcol &LightIntentsPNG,RayInfo &RayPtr,long &ID,TVertex &Dir,TVertex &Norm,TVertex &Check,long Fact);
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline TReal RAngle ( TVertex &P1, TVertex &P2 )
{
  return (P1.x*P2.x + P1.y*P2.y + P1.z*P2.z);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline long TWorld_IsShadowed (TVertex &Check,TVertex  &Rest,long &ID)
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  RShadRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  BYTE Tmp;

  if (!(ActiveWorld->Flags & RW_SHADOWS)) return 0;

  #ifdef SphereOK
    CompletPos = Check.x*Check.x + Check.y*Check.y + Check.z*Check.z;
    CompletDir = Rest.x*Rest.x + Rest.y*Rest.y + Rest.z*Rest.z ;
    InvCompletDir = 1.0f / CompletDir;
  #endif

  for (register long Cmt = 1 ; (Cmt <= ActiveWorld->Num_Of_Obj); Cmt++)
  {
    if( (ID != ActiveWorld->ShadowList[Cmt])  )
    {

      Ray_Active_Object_Internal ( ActiveWorld->ShadowList[Cmt] );
      if (Ray_GetShadowIntersection (Check,Rest))
      {
        Tmp = ActiveWorld->ShadowList[1];
        ActiveWorld->ShadowList[1]     = ActiveWorld->ShadowList[Cmt];
        ActiveWorld->ShadowList[Cmt]   = Tmp;
        return 1;
      }
    }
  }
  return 0;
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void TWorld_CalcLight (rgbcol &LightIntentsPNG,RayInfo &RayPtr,long &ID,TVertex &Dir,TVertex &Norm,TVertex &Check,long Fact)
{
  TVertex Rest;
  long Shadow = 0;
  word R ;
  word G ;
  word B ;

  word PR = 0;
  word PG = 0;
  word PB = 0;
  TReal T2 = 0;
  TReal AN;
  TVertex Dir2;

  if (ActiveObject->FLAGS & R_NO_LIGHT)
  {
    R = 255;
    G = 255;
    B = 255;
  } else
  {

    R = ActiveWorld->AMBIENT; G = ActiveWorld->AMBIENT; B = ActiveWorld->AMBIENT;
    TReal AmbientFActor = (1-(float)ActiveWorld->AMBIENT/256.0);

    for (register long TTL = 0; TTL < ActiveWorld->Num_Of_Lights; TTL++)
    {
      Ray_Active_Light ( TTL );

      RVTX_Copy ( Rest , ActiveLight->Position);
      RVTX_Sub  ( Rest , Check );

      Shadow = TWorld_IsShadowed (Check,Rest,ID) ;

      Ray_Active_Object_Internal ( ID );

      if ((!Shadow))
      {
        RayPtr.LigthIntens = Ray_GetDiffuseLight (Check,Norm);
        R += AmbientFActor*RayPtr.LigthIntens*ActiveLight->Color.B.R;
        G += AmbientFActor*RayPtr.LigthIntens*ActiveLight->Color.B.G;
        B += AmbientFActor*RayPtr.LigthIntens*ActiveLight->Color.B.B;
      } else RayPtr.Shadowed = 1;

      if ((Ray_GetObjectPhong () > 0) && (RayPtr.LigthIntens > 0) && (!Shadow))
      {
        if (!T2)
        {
          T2 = -2.0*RAngle (Dir,Norm);
          RVTX_Set (Dir2,Norm.x*T2 + Dir.x,Norm.y*T2 + Dir.y,Norm.z*T2 + Dir.z);
          RVTX_Normalize (Dir2);
        }
        AN = Ray_GetPhongLight( Check , Dir2 , Ray_GetObjectPhong (),Ray_GetObjectPhongSize ());

        PR += AN*ActiveLight->Color.B.R;
        PG += AN*ActiveLight->Color.B.G;
        PB += AN*ActiveLight->Color.B.B;
      }
    }
  }
  if ( R > 255 ) R = 255;
  if ( G > 255 ) G = 255;
  if ( B > 255 ) B = 255;
  if ( PR > 255 ) PR = 255;
  if ( PG > 255 ) PG = 255;
  if ( PB > 255 ) PB = 255;

  RayPtr.Color.B.R = (Fact*RayPtr.Color.B.R*R)/(256*256);
  RayPtr.Color.B.G = (Fact*RayPtr.Color.B.G*G)/(256*256);
  RayPtr.Color.B.B = (Fact*RayPtr.Color.B.B*B)/(256*256);

  LightIntentsPNG.B.R = PR;
  LightIntentsPNG.B.G = PG;
  LightIntentsPNG.B.B = PB;
}

void TWorld_Reflect ( long &ID,TVertex &Dir,TVertex &Norm,TVertex &Check,RayInfo &RayPtr,int Deep,TReal const &Index,long const &Fact)

{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  #ifdef DEBUG_RENDER_REFLECT
      cprintf ("Enter reflect for ID[%d] \r\n",ID);
  #endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  RSecRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

  TReal   T2 = RAngle (Dir,Norm)*Index;

  TReal   ValNorm2;
  TVertex Check2;
  TVertex Norm2;

  long    Object_ID2;
  byte    Hit = 0;
  byte		Shadow = 0;
  byte    Shadow2 = 0;
  byte    Inv = 0;

  rgbcol TMP_RGB;

  rgbcol LightIntentsPNG;
  LightIntentsPNG.ColRGB = 0;
  //LightIntentsPNG.B.A = 0;


  TMP_RGB.ColRGB = RayPtr.Color.ColRGB;

  TReal   T0 = 10000;
  TReal   T1 = 10000;
  TReal   TMin = 10000;

  TVertex Dir2;
  TVertex Dir3;

  RVTX_Set (Dir2,Norm.x*T2 + Dir.x,Norm.y*T2 + Dir.y,Norm.z*T2 + Dir.z);


  #ifdef SphereOK

    CompletDir = Dir2.x*Dir2.x + Dir2.y*Dir2.y + Dir2.z*Dir2.z ;
    InvCompletDir = 1 /CompletDir;
  #endif

  for (register long Cmt  = 1 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++)
  {
//    if ( !((ActiveObject->Type == R_BLOB ) && (Cmt==ID)) )
    if ( Cmt!=ID )


    {
      Ray_Active_Object_Internal ( Cmt );
      #ifdef DEBUG_RENDER_REFLECT
          cprintf ("Activating ID[%d] \r\n",Cmt);
      #endif
      if ( (Index < 0.0) && (ActiveObject->FLAGS & R_VAMPIRE)); else
      if (Ray_GetSecondaryIntersection (Check,Dir2,T0,T1) )
      {

        if ((T0< TMin) && (T0>0.0001) ){
            Object_ID2 = Cmt;
            TMin = T0;
            Hit = 1;
        }
      }
    }
  }

  if (Hit)
  {
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
    RHitSecRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

    RVTX_Set (Check2,Check.x + TMin*Dir2.x,Check.y + TMin*Dir2.y,Check.z + TMin*Dir2.z);

    Ray_Active_Object_Internal ( Object_ID2 );

    Inv = ActiveObject->Invers;

    //ActiveObject->GetColorInfo (RayPtr,Check2);
    Ray_GetColorInfo ( RayPtr,Check2);
    ActiveObject->GetNormal(Check2,Norm2,ValNorm2,Inv);

/*
    if (IsTransparentT)
    {
      if (ActiveObject->IOR_INTS == 255)
      {
        RayPtr.Color.B.R = TMP_RGB.B.R;
        RayPtr.Color.B.G = TMP_RGB.B.G;
        RayPtr.Color.B.B = TMP_RGB.B.B;

    //    TWorld_CalcLight  (LightIntentsPNG,RayPtr,Object_ID2,Dir2,Norm2,Check2,Fact);

        if (Object_ID2 != ID )  goto NoPigment;
      }
    }
*/


    TWorld_CalcLight  (LightIntentsPNG,RayPtr,Object_ID2,Dir2,Norm2,Check2,Fact);


    RayPtr.Color.B.R += (TMP_RGB.B.R*(255-Fact))/256;
    RayPtr.Color.B.G += (TMP_RGB.B.G*(255-Fact))/256;
    RayPtr.Color.B.B += (TMP_RGB.B.B*(255-Fact))/256;

NoPigment:

    if ( Deep > 1 )
    {
      long Trans = IsTransparent;
      long Refl  = IsReflect;
      TObject_R *Push2 = ActiveObject;
      if ( Trans && (ActiveWorld->Flags & RW_REFLECT) )
        TWorld_Reflect ( Object_ID2,Dir2,Norm2,Check2,RayPtr,Deep-1,Push2->IOR,255-Push2->IOR_INTS/2.0);
      if ( Refl && (ActiveWorld->Flags & RW_REFRACT) )
        TWorld_Reflect ( Object_ID2,Dir2,Norm2,Check2,RayPtr,Deep-1,-2.0,Push2->REFL_INTS);
    }
        if (LightIntentsPNG.ColRGB)
        {
          RayPtr.Color.B.R = RayPtr.Color.B.R*(255-LightIntentsPNG.B.R)/256 + LightIntentsPNG.B.R;
          RayPtr.Color.B.G = RayPtr.Color.B.G*(255-LightIntentsPNG.B.G)/256 + LightIntentsPNG.B.G;
          RayPtr.Color.B.B = RayPtr.Color.B.B*(255-LightIntentsPNG.B.B)/256 + LightIntentsPNG.B.B;
        }
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  #ifdef DEBUG_RENDER_REFLECT
      cprintf ("Leave reflect for ID[%d] \r\n",ID);
  #endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
}

void TWorld_Init ( TVertex Pos)
{
    if (ActiveWorld->Precalculated) return;

    for (register long Cmt = 1 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++)
    {
        Ray_Active_Object_Internal ( Cmt );
        Ray_PrecalcIntersection ( Pos );
    }
    for (Cmt = 0 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++) {ActiveWorld->ShadowList[Cmt] = Cmt;}
    ActiveWorld->Precalculated = 1;
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void TWorld_GetFrame ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST)
{

  TReal dX,dY,dZ,tdX,tdY,tdZ;                   // Camera Interpolation
  TReal dX0,dY0,dZ0,dX1,dY1,dZ1;                // Camera Interpolation
  TReal T0,T1,T2,TMin,TMax;                     // Distance Vars.
  TReal ValNorm;

  TVertex   Check,Dir,Dir2,Norm,Rest,Pos;

  long  Hit,Object_ID,Shad,Inv;
  long  PosBounder,IncBounder1,IncBounder2,ValBounder;
  long  NumCh,NumNCh = 0;

  rgbcol LightIntentsPNG;
  RayInfo RayPtr;

  dword *dst;
  dword Incy;

  clock_t Stime, Etime;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Precalculem Escena
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  Stime = clock ();
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Screen Values
  TReal Y0 = (R_ScreenY/2)-SY/2;
  TReal X0 = (R_ScreenX/2)-SX/2;
  dword iY0 = (dword) (Y0);
  dword iX0 = (dword) (X0);

  RBorderV = iY0 ;
  RBorderH = iX0 ;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Camera interpolation Values

  dX = Panasonic->dX;
  dY = Panasonic->dY;
  dZ = Panasonic->dZ;
  dX0 = Panasonic->dX0;
  dY0 = Panasonic->dY0;
  dZ0 = Panasonic->dZ0;
  dX1 = Panasonic->dX1;
  dY1 = Panasonic->dY1;
  dZ1 = Panasonic->dZ1;

  dX0 *= GX; dY0 *= GX; dZ0 *= GX;
  dX1 *= GY; dY1 *= GY; dZ1 *= GY;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Initialize defaults values for frame

  RVTX_Copy (Pos,Panasonic->Position) ;

  ActiveWorld->Precalculated = 0;

  TWorld_Init ( Pos );

  LightIntentsPNG.B.A = 0;

  T0 = 10000;
  T1 = 10000;
  TMin  = 1000;
  Hit   = 0;
  Object_ID = 0;

//±±±±±±±±±±±  Bounder
  dst = (dword *)(DST+(R_ScreenX*iY0)+iX0);

  long DespOrigin = (R_ScreenX*iY0)+iX0;

  Incy = R_ScreenX*GY;

  Check_Bounder(1+SX/GX,1+SY/GY);

  PosBounder = 0;
  IncBounder1 = 0;
  IncBounder2 = 1+SX/GX;
  ValBounder = 0;

//±±±±±±±±±±±  Result vars.
  NumCh     = 0;
  NumNCh    = 0;
  RPrimRay  = 0;
  RShadRay  = 0;
  RSecRay   = 0;
  RHitPrimRay   = 0;
  RHitSecRay    = 0;
  RTestInt1 = 0;
  RTestInt2 = 0;
  RTestInt3 = 0;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Calc Frame
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

  for (register long y=iY0; y<(SY+iY0); y += GY)
  {

    tdX = dX;
    tdY = dY;
    tdZ = dZ;

    for (register long x=iX0; x<(SX+iX0); x += GX)
    {
      Dir.x = tdX;
      Dir.y = tdY;
      Dir.z = tdZ;
      TMin = T0 = T1 = 10000.0;
      Hit = 0;
      TMax = -100000;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
      RPrimRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

      #ifdef SphereOK

        CompletDir = Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z;
        InvCompletDir = 1.0 / CompletDir;
      #endif


      if (ActiveWorld->Flags & RW_PREDICTION_ANIM)
      {
        ValBounder = BounderCheck[PosBounder+IncBounder1];

        if (ValBounder)
        {
          if (ValBounder != 255)
          {
            Ray_Active_Object_Internal ( ValBounder );
            if (Ray_GetPrimaryIntersection ( Dir, T0, T1 ))
            {
              if ( (T0< TMin)  )
              {
                Object_ID = ValBounder;
                TMin = T0;
                Hit = 1;
              }
              NumCh ++;
            }
          }
          goto TestingHit;
        } else goto NoBounderEnabled;
      } else
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
NoBounderEnabled:
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
      {
        for (register long Cmt = 1 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++)
        {
          Ray_Active_Object_Internal ( Cmt );
          if (Ray_GetPrimaryIntersection ( Dir, T0, T1 ))
          {

            if ( (T0< TMin) )
            {
              Object_ID = Cmt;
              TMin = T0;
              Hit = 1;
            }
          }
        }
        NumNCh ++;
      }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TestingHit:
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
      if (Hit && ( TMin < R_FAR_CLIP )   )
      {
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
        RHitPrimRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
        RayPtr.Color.ColRGB =  0;
        RVTX_Set (Check,Pos.x + TMin*Dir.x,Pos.y + TMin*Dir.y,Pos.z + TMin*Dir.z);

        Ray_Active_Object_Internal ( Object_ID );

        Inv = ActiveObject->Invers;

        RayPtr.Desp = DespOrigin + x;

        //  R_ReturnStatus = 0;
        Ray_GetColorInfo ( RayPtr,Check);

        if (ActiveWorld->Flags & RW_PREDICTION_ANIM) *(Bounder+PosBounder+IncBounder1) = Object_ID;

        ActiveObject->GetNormal(Check,Norm,ValNorm,Inv);

        TWorld_CalcLight (LightIntentsPNG,RayPtr,Object_ID,Dir,Norm,Check,255);

        if ( Deep > 0 )
        {
          long Trans = IsTransparent;
          long Refl  = IsReflect;
          TObject_R *Push2 = ActiveObject;

          if ( Trans  && (ActiveWorld->Flags & RW_REFRACT))
            TWorld_Reflect ( Object_ID,Dir,Norm,Check,RayPtr,Deep,Push2->IOR,Push2->IOR_INTS);
          if ( Refl  && (ActiveWorld->Flags & RW_REFLECT))
            TWorld_Reflect ( Object_ID,Dir,Norm,Check,RayPtr,Deep,-2.0,Push2->REFL_INTS);
        }

        if (LightIntentsPNG.ColRGB)
        {
          RayPtr.Color.B.R = RayPtr.Color.B.R*(255-LightIntentsPNG.B.R)/256 + LightIntentsPNG.B.R;
          RayPtr.Color.B.G = RayPtr.Color.B.G*(255-LightIntentsPNG.B.G)/256 + LightIntentsPNG.B.G;
          RayPtr.Color.B.B = RayPtr.Color.B.B*(255-LightIntentsPNG.B.B)/256 + LightIntentsPNG.B.B;

        }

        if (R_FOG)
        {
          TMin *= R_FOG_DISTANCE_FACTOR;
          if (TMin>255) TMin = 255;

          dword FogIntens = (dword) (TMin);

          RayPtr.Color.B.R = RayPtr.Color.B.R*(255-FogIntens)/256 + FogIntens*R_FOG_R;
          RayPtr.Color.B.B = RayPtr.Color.B.B*(255-FogIntens)/256 + FogIntens*R_FOG_B;
          RayPtr.Color.B.G = RayPtr.Color.B.G*(255-FogIntens)/256 + FogIntens*R_FOG_G;
        }

        *(dst+x) = RayPtr.Color.ColRGB;

      } else
      {
        *(dst+x) = R_BACK_COLOR;
        if (ActiveWorld->Flags & RW_PREDICTION_ANIM) *(Bounder+PosBounder+IncBounder1) = 255;
      }
      tdX+=dX0; tdY+=dY0; tdZ+=dZ0;
      IncBounder1++;
    }
    dst +=Incy;
    DespOrigin +=Incy;

    PosBounder +=IncBounder2;
    IncBounder1 = 0;
    dX+=dX1; dY+=dY1; dZ+=dZ1;
  }
  Etime = clock ();
#ifdef R_LOG_DEF_DEF
  if ( ActiveWorld->Flags & RW_RESULTS )
  {
    R_LOG ("System-R: Get frame");
    R_LOG ("  Camera:");
    R_LOG ("    Position : <%3.3f,%3.3f,%3.3f>",Panasonic->Position.x,Panasonic->Position.y,Panasonic->Position.z);
    R_LOG ("    Target   : <%3.3f,%3.3f,%3.3f>",Panasonic->LookAt.x,Panasonic->LookAt.y,Panasonic->LookAt.z);
    R_LOG ("  Object:");
    R_LOG ("    Number   : %d",ActiveWorld->Num_Of_Obj);
    R_LOG ("  Light :");
    R_LOG ("    Number   : %d",ActiveWorld->Num_Of_Lights);
    R_LOG ("  Result     :");
    R_LOG ("    Primary Ray       : %d",RPrimRay);
    R_LOG ("    Secondary Ray     : %d",RSecRay);
    R_LOG ("    Shading Ray       : %d",RShadRay);
    R_LOG ("    Hit Primary Ray   : %3.3f%%",RHitPrimRay*100.0/(TReal)RPrimRay);
    if (RSecRay == 0) RSecRay = 1;
    R_LOG ("    Hit Secondary Ray : %3.3f%%",RHitSecRay*100.0/(TReal)RSecRay);
    R_LOG ("    Test Function 1   : %d",RTestInt1 );
    R_LOG ("    Test Function 2   : %d",RTestInt2);
    R_LOG ("    Test Function 3   : %d",RTestInt3);
    if ( ActiveWorld->Flags & RW_PREDICTION_ANIM)
    {
      R_LOG ("    Prediction hit    : %3.3f%%",NumCh*100.0/(TReal)(NumNCh+NumCh));
    }
    R_LOG ("    Time              : %3.3fs",(TReal)( Etime - Stime )/CLOCKS_PER_SEC);
    R_LOG ("    Fps               : %3.3f",1.0/((TReal)( Etime - Stime )/CLOCKS_PER_SEC));
  }
#endif
}



//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TWorld_R *CreateWorldDefault()
{
  TWorld_R *TMP = (TWorld_R *) R_GetMem (sizeof(TWorld_R));
  TMP->GetFrame    =    &TWorld_GetFrame;
  TMP->Flags       = 0;
  return TMP;
}

void DestroyWorld(TWorld_R *M)
{
  R_FreeMem (M);
}


//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²         ADD Functions
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
long Ray_ADD_Object ( TObject_R *Obj )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  if ( Obj==NULL )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't add Null object");
#endif
    return ERRORCODE;
  }
  if ( ActiveWorld->Num_Of_Obj==NumMaxObjectes )
  {
#ifdef R_LOG_DEF
  R_ERR ("SystemR can't add object [%p]",Obj);
#endif
    return ERRORCODE;
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld->Objects[ActiveWorld->Num_Of_Obj++] = Obj;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF
  R_LOG ("Adding Object [%p] width ID[%3d]",Obj,ActiveWorld->Num_Of_Obj-1);
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  return (ActiveWorld->Num_Of_Obj);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Del_Object ( long NObj )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  for (long i = NObj-1; i < ActiveWorld->Num_Of_Obj; i++ )
    ActiveWorld->Objects[i] = ActiveWorld->Objects[i+1];
  ActiveWorld->Num_Of_Obj--;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF
  R_LOG ("Deleting Object [%d]",NObj);
#endif
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
long Ray_ADD_Light ( TLightPoint_R *Ll )
{
  if ( Ll==NULL )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't add Null light");
#endif
    return ERRORCODE;
  }
  if ( ActiveWorld->Num_Of_Lights==NumMaxLight )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't add light [%p]",Ll);
#endif
    return ERRORCODE;
  }

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld->Lights[ActiveWorld->Num_Of_Lights++] = Ll;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF
  R_LOG ("Adding Light [%p] width ID[%3d]",Ll,ActiveWorld->Num_Of_Lights-1);
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  return (ActiveWorld->Num_Of_Lights-1);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²         Active Functions
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Active_Object_Internal ( long ID )
{

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF_DEBUG
    R_LOG ("Activating Object ID[%3d]  ",ID);
#endif

  ActiveObject = ActiveWorld->Objects[ID-1];
  IDActiveObject = ID-1;

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

#ifdef BlobOK
    case R_BLOB:
        ActiveBlob= (TBlob_R*) ActiveObject;
        break;
#endif
#ifdef SorOK
    case R_SOR:
        ActiveSor= (TSor_R*) ActiveObject;
        break;
#endif

#ifdef HFieldOK
    case R_HFIELD:
        ActiveHField= (THField_R*) ActiveObject;
        break;
#endif
  };
}

void Ray_Active_Object ( long ID )
{


  switch ((ActiveWorld->Objects[ID-1])->Type)
  {
#ifdef PlaneOK
    case R_PLANE:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating Plane [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef SphereOK
    case R_SPHERE:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating Sphere [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef CylinderOK
    case R_CYLINDER:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating Cylinder [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef EllipseOK
    case R_ELLIPSE:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating Ellipse [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef CubeOK
    case R_CUBE:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating Cube [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef CSGOK
    case R_CSG:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating CSG [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif

#ifdef BlobOK
    case R_BLOB:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating BLOB [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
#ifdef SorOK
    case R_SOR:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating SOR [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif

#ifdef HFieldOK
    case R_HFIELD:
        #ifdef R_LOG_DEF_DEF
        R_LOG ("Activating HFIELD [%p]",ActiveWorld->Objects[ID-1]);
        #endif
        break;
#endif
  };
  Ray_Active_Object_Internal ( ID );
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Active_Light ( long ID )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  if  (ActiveWorld->Num_Of_Lights<ID )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't active an inexisten light");
#endif
    RAY_EXIT();
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveLight = ActiveWorld->Lights[ID];
  IDActiveLight = ID;
}


//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Active_World ( TWorld_R *ID )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  if  (ID == NULL )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't active an Null world");
#endif
    RAY_EXIT();
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld = ID;
#ifdef R_LOG_DEF_DEF
  R_LOG ("Activating World ID[%p]",ID);
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Active_Camera ( TCamera_R *ID )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  if  (ID == NULL )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't active an Null camera");
#endif
    RAY_EXIT();
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveCamera = ID;
#ifdef R_LOG_DEF_DEF
  R_LOG ("Activating Camera ID[%p]",ID);
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_Initialize_World ( TWorld_R *ID )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  if  (ID == NULL )
  {
#ifdef R_LOG_DEF
    R_ERR ("SystemR can't initialize an Null world");
#endif
    RAY_EXIT();
  }
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ID->Num_Of_Obj     = 0;
  ID->Num_Of_Lights  = 0;
  ID->AMBIENT = 0;
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_PrecalcIntersection ( TVertex &Pos )
{
  ActiveObject->SetValuesIntersection(Pos);
}


//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void RW_Enable ( dword P )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld->Flags = ActiveWorld->Flags | P ;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF
  switch (P)
  {
    case RW_PREDICTION_ANIM    :LOG ("Enable PREDICTION_ANIM"); break;
    case RW_SHADOWS            :LOG ("Enable RW_SHADOWS"); break;
    case RW_REFLECT            :LOG ("Enable RW_REFLECT"); break;
    case RW_REFRACT            :LOG ("Enable RW_REFRACT"); break;
    case RW_RESULTS            :LOG ("Enable RW_RESULTS"); break;
  }
#endif

}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void RW_Disable ( dword P )
{
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld->Flags = ActiveWorld->Flags & Not(P) ;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef R_LOG_DEF_DEF
  switch (P)
  {
    case RW_PREDICTION_ANIM    :LOG ("Disable PREDICTION_ANIM"); break;
    case RW_SHADOWS            :LOG ("Disable RW_SHADOWS"); break;
    case RW_REFLECT            :LOG ("Disable RW_REFLECT"); break;
    case RW_REFRACT            :LOG ("Disable RW_REFRACT"); break;
    case RW_RESULTS            :LOG ("Disable RW_RESULTS"); break;
  }
#endif
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Ray_GetColorInfo ( RayInfo &RayPtr,TVertex &CH){

#ifndef SYR_PROCEDURAL_TEXTURES
      ActiveObject->GetColorInfo (RayPtr,CH);
#else

  ProceduralTexture *TXT = ActiveObject->Pigment.Texture;

  if (TXT == NULL)
  {
     ActiveObject->GetColorInfo (RayPtr,CH);
  }
    else
  {
    byte FIRT_LEVEL = 1;
    RealColor Col,P1;
    P1.RGBA[0]  = P1.RGBA[1]  = P1.RGBA[2]  = P1.RGBA[3]  = 0;
    Col.RGBA[0] = Col.RGBA[1] = Col.RGBA[2] = Col.RGBA[3] = 0;
    float LastAlfa = 0;

    while (TXT != NULL ) {

      GetTextureColor(TXT,CH,ActiveObject->Pigment.TextScale,ActiveObject->Pigment.InvTextScale,Col);

      if (FIRT_LEVEL) { LastAlfa = Col.RGBA[3] ; Col.RGBA[3] = 1 ; FIRT_LEVEL = 0; }
      P1.RGBA[0] = Col.RGBA[0]*Col.RGBA[3] + P1.RGBA[0]*(1-Col.RGBA[3]);
      P1.RGBA[1] = Col.RGBA[1]*Col.RGBA[3] + P1.RGBA[1]*(1-Col.RGBA[3]);
      P1.RGBA[2] = Col.RGBA[2]*Col.RGBA[3] + P1.RGBA[2]*(1-Col.RGBA[3]);
      TXT = TXT->Next;
    }

    RayPtr.Color.B.R =(byte) (P1.RGBA[0]*255.0);
    RayPtr.Color.B.G =(byte) (P1.RGBA[1]*255.0);
    RayPtr.Color.B.B =(byte) (P1.RGBA[2]*255.0);
    RayPtr.Color.B.A =(byte) (LastAlfa*255.0);
/*
    RayPtr.Color.B.R =(byte) (55.0);
    RayPtr.Color.B.G =(byte) (125.0);
    RayPtr.Color.B.B =(byte) (255.0);
*/
  }

  if (IsTransparentT)
  {
    ActiveObject->FLAGS = ActiveObject->FLAGS & Not(R_TRANSPARENT) ;

    if (RayPtr.Color.B.A)
    {
      ActiveObject->IOR_INTS = RayPtr.Color.B.A;
      ActiveObject->FLAGS = ActiveObject->FLAGS | R_TRANSPARENT ;
    }
  }

#endif
}



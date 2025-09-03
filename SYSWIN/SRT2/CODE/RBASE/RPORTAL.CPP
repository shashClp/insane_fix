//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_PortalWorld
#define SystemR_V30_PortalWorld

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
#include "RSCENE.H"
#include "..\REFECTS\RLIGHT.H"
#include "RCAMERA.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

TWorld_R    *WorldList[NumMaxWorldList];
long        IndexWorldList = 0;


void            TWorld_GetPortalFrame ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST);
inline void     PushWorld (TWorld_R *P);
inline void     PopWorld ();

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void TWorld_GetPortalFrame ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST)
{

  TReal dX,dY,dZ,tdX,tdY,tdZ;                   // Camera Interpolation
  TReal dX0,dY0,dZ0,dX1,dY1,dZ1;                // Camera Interpolation
  TReal T0,T1,T2,TMin,TMax;                     // Distance Vars.
  TReal ValNorm;

  TVertex   Check,Dir,Dir2,Norm,Rest,Pos;

  long  Hit,Object_ID,Shad,Inv;
  long  PosBounder,IncBounder1,IncBounder2,ValBounder;
  long  PortalFound;
  long  NumCh,NumNCh = 0;

  rgbcol LightIntentsPNG;
  RayInfo RayPtr;

  dword *dst;
  dword Incy;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Precalculem Escena
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

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
  Incy = R_ScreenX*GY;

  Check_Bounder(1+SX/GX,1+SY/GY);

  PosBounder = 0;
  IncBounder1 = 0;
  IncBounder2 = 1+SX/GX;
  ValBounder = 0;
  PortalFound = 0;

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
      PortalFound = 0;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
      RPrimRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

      #ifdef SphereOK

        CompletDir = Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z;
        InvCompletDir = 1.0 / CompletDir;
      #endif


    if (ActiveWorld->Flags & RW_PREDICTION_ANIM)
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

     } else

PortalBucle:

     {
      for (register long Cmt = 1 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++)
      {
        Ray_Active_Object_Internal ( Cmt );
        if (Ray_GetPrimaryIntersection ( Dir, T0, T1 ))
        {

          if (PortalFound) {
            if (T0 < TMax )
             if (T1 < TMax ) T0 = TMin;
             else {
               T0 = T1;
               ActiveObject->Invers = 1;
             }
          }
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

      if (Hit && ( TMin < R_FAR_CLIP )   )
      {
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
        RHitPrimRay++;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
        RayPtr.Color.ColRGB =  0;
        RVTX_Set (Check,Pos.x + TMin*Dir.x,Pos.y + TMin*Dir.y,Pos.z + TMin*Dir.z);

        Ray_Active_Object_Internal ( Object_ID );

        Inv = ActiveObject->Invers;

        R_ReturnStatus = 0;

        //ActiveObject->GetColorInfo (RayPtr,Check);
        Ray_GetColorInfo ( RayPtr,Check);

        if (ActiveWorld->Flags & RW_PREDICTION_ANIM)
         if (!PortalFound) *(Bounder+PosBounder+IncBounder1) = Object_ID;

        if ( R_ReturnStatus == R_PORTAL_TEXT)
        {

            PortalFound = 1;
            PushWorld ((TWorld_R *)ActiveObject->PortalToWorld1);
            TWorld_Init ( Pos );
            #ifdef DEBUG_RENDER
                cprintf ("Activing Portal\r\n");
            #endif
            TMax = TMin;
            TMin = T0 = T1 = 10000.0;
            Hit = 0;

            goto PortalBucle;
        }


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
/*
        if (  Deep > 0 )
        {
          if ( IsTransparent  )
          {
            TWorld_Reflect (Object_ID,Dir,Norm,Check,RayPtr,Deep,ActiveObject->IOR,ActiveObject->IOR_INTS);
          }

          if ( IsReflect  )
          {
            TWorld_Reflect (Object_ID,Dir,Norm,Check,RayPtr,Deep,-2.0,ActiveObject->REFL_INTS);
          }
        }
 */
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
      if (PortalFound) PopWorld ();
    }
    dst +=Incy;
    PosBounder +=IncBounder2;
    IncBounder1 = 0;
    dX+=dX1; dY+=dY1; dZ+=dZ1;
  }

#ifdef R_LOG_DEF
  if ( ActiveWorld->Flags & RW_RESULTS )
  {
    R_LOG ("System-R: Get Portal frame");
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
    R_LOG ("    Test Function 1  : %d",RTestInt1 );
    R_LOG ("    Test Function 2  : %d",RTestInt2);
    R_LOG ("    Test Function 3  : %d",RTestInt3);
    if ( ActiveWorld->Flags & RW_PREDICTION_ANIM)
    R_LOG ("    Prediction hit    : %3.3f%%",NumCh*100.0/(TReal)(NumNCh+NumCh));
  }
#endif
}



//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline void PushWorld (TWorld_R *P)
{
#ifdef R_LOG_DEF
  R_LOG ("Push World[%p]",P);
#endif
  if (IndexWorldList < NumMaxWorldList)
  {
    WorldList[IndexWorldList++] = ActiveWorld;
    ActiveWorld = P;
  }
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline void PopWorld ()
{

  if (IndexWorldList > 0)
  {
    ActiveWorld = WorldList[IndexWorldList-1];
#ifdef R_LOG_DEF
    R_LOG ("Pop World[%p]",ActiveWorld);
#endif
    IndexWorldList--;
  }
}

#endif







































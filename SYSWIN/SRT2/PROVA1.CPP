//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#define VGA256
#define PlaneOK
//#define SphereOK
#define EllipseOK
//#define CSGOK
//#define BlobOK
//#define SorOK
//#define CylinderOK
//#define EllipseOK
//#define CubeOK

#define NDEBUG
//#define DEBUG
//#define DEBUG_RENDER
//#define DEBUG_RENDER_REFLECT
#define RayDim 3
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#include "..\Rbase\RWorld.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void Sortir ()
{
  exit(0);
}
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void main(int num , char *aw[])
{

  RAY_EXIT = &Sortir;
  TVertex P,L,N;

  InitScreen();
  InitContext();

  TObject_R *Plane1,*Plane2,*Plane3,*Plane4,*Plane5,*Plane6;
  TObject_R *Sphere1,*Sphere2,*Sphere3,*Sphere4;
  TObject_R *Cyl1,*Cyl2,*Cyl3,*Cyl4;
  TObject_R *Ell1,*Ell2,*Ell3,*Ell4;
  TObject_R *Cub1,*Cub2,*Cub3,*Cub4;
  TObject_R *b1,*b2,*b3,*b4;
  TObject_R *CSG1,*CSG2,*CSG3,*CSG4;

  TWorld_R  *World1;
  TLightPoint_R *Llum1,*Llum2,*Llum3;
  TCamera_R   *Camera1;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  rgbcol *Dst = (rgbcol*) GetMem (R_ScreenX*R_ScreenX*sizeof(rgbcol));
  FillMem (Dst,0,R_ScreenX*R_ScreenX);
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

  World1  = CreateWorldDefault();

  Ray_Initialize_World ( World1 );
  Ray_Active_World ( World1 );


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

  Camera1 = CreateCameraDefault();
  VTX_Set (P,0,-3,-5);
  VTX_Set (L,0,0,0);
  VTX_Set (N,0,1,0);
  Ray_Active_Camera ( Camera1 );
  TCamera_R_Init (P,L,N,256,70);
  TCamera_R_Actualize (R_ScreenX,R_ScreenY);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

  Llum1   = CreateLightDefault();
  long ID_Llum1  = Ray_ADD_Light  ( Llum1 );
  Ray_Active_Light ( ID_Llum1 );
  TLightPoint_R_Init ( -20 ,27 ,-20);

  Llum2   = CreateLightDefault();
  long ID_Llum2  = Ray_ADD_Light  ( Llum2 );
  Ray_Active_Light ( ID_Llum2 );
  TLightPoint_R_Init ( 20 ,27 ,-20);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

  Ell1  = CreateEllipseDefault();
  Ray_Active_Object ( Ray_ADD_Object (Ell1));
  TEllipse_R_Init  ( 0,0,0,6,7,6,1,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Set_Phong ( 1, 100 );
  Ray_Enable ( R_REFLECT );
  Ray_Set_ReflectInts ( 100 );

  Ell2  = CreateEllipseDefault();
  /*
  Ray_Active_Object ( Ray_ADD_Object (Ell2));
  TEllipse_R_Init  ( 5-rand()%10,0,5-rand()%10,2,7,2,1,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Set_Phong ( 1, 100 );
  Ray_Enable ( R_REFLECT );
  Ray_Set_ReflectInts ( 100 );
  */
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  Plane1  = CreatePlaneDefault();
/*
  Ray_Active_Object ( Ray_ADD_Object (Plane1));
  TPlane_R_Init  (0,-6,0,0,1,0,0,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Enable ( R_REFLECT );
  Ray_Set_ReflectInts ( 100 );
*/
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

  World1->AMBIENT = 40;
  TReal a = 0;

  RW_Enable ( RW_PREDICTION_ANIM )                                         ;
  RW_Enable ( RW_RESULTS )                                         ;
  RW_Enable ( RW_SHADOWS )                                         ;
  RW_Enable ( RW_REFLECT )                                         ;
  Bounder_Init(R_ScreenX,R_ScreenY,1,1);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//  while ( a < 50)
  while ( !kbhit() )
  {
    a += 1;
    ActiveCamera->Position.x = sin (a*R_PIx2/200)*24;
    ActiveCamera->Position.z = cos (a*R_PIx2/200)*24;
    ActiveCamera->Position.y = 0;

    TCamera_R_Actualize (R_ScreenX,R_ScreenY);

    Llum1->Position.x = cos (a*R_PIx2/120)*14;
    Llum1->Position.z = cos (a*R_PIx2/130)*19;
    Llum1->Position.y = sin (a*R_PIx2/160)*13;

    TWorld_GetFrame ( Camera1,R_ScreenX,R_ScreenY-60, 6 , RayDim,RayDim-1,Dst);
    Pintar(Dst,VAR_VIDEO_VGA);

  }


  RW_Disable ( RW_PREDICTION_ANIM );

  TWorld_GetFrame ( Camera1,R_ScreenX,R_ScreenY, 6 ,1,1,Dst);
  CreateTGA((rgbcol *)Dst,"out.tga");

  DestroyEllipseDefault(Ell1);
  DestroyEllipseDefault(Ell2);
  DestroyPlaneDefault(Plane1);

  CloseScreen();
  CloseContext();
}


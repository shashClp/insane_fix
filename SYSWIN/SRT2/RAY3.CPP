//==============================================================================
#define R_ENABLE_FILES_DEPENDENCIES

//#define VGA256
//==============================================================================
#define DEBUG_PREDICTION
//==============================================================================
#define PlaneOK
#define SphereOK
#define EllipseOK
#define CylinderOK

#define NSP2 0
#define NSP  NSP2*3

#define R_ScreenXView R_ScreenX
#define R_ScreenYView R_ScreenY-70

#define R_RAY_DIM 1
#define RayDimX R_RAY_DIM
#define RayDimY R_RAY_DIM


#define SYR_PROCEDURAL_TEXTURES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <Systemr.v30\RDefines\extra.h>
#include <Systemr.v30\Rbase\RWorld.h>

//#include <Systemr.v30\RDefines\RDefines.h>
//#include <Systemr.v30\Rbase\RWorld.h>
#include <Systemr.v30\Refects\inter.h>

ProceduralTexture *ActiveFog;
TReal ActiveFogScale = 10.0;
TReal ActiveFogInvScale = 1.0/ActiveFogScale;
long FogDeep = 10;
TReal FogJump = 1;
void LoadTGA(rgbcol **Pantalla,char *namef);
//void TWorld_CalcRealisticFog ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST);
//void TWorld_GetFrameFog ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST);

void main( int argc, char *argv[])
{

  InitScreen();
  InitContext();
  


  TWorld_R  *World1;
  TLightPoint_R *Llum1;
  TLightPoint_R *Llum2;
  long i = 0;
  TCamera_R   *Camera1;
  MakeObject (R_SPHERE,NSP+7);
  MakeObject (R_PLANE,2);
  MakeObject (R_ELLIPSE,3);
  MakeObject (R_CYLINDER,4);

  World1  = CreateWorldDefault();
  Llum1   = CreateLightDefault();
  Llum2   = CreateLightDefault();
  Camera1 = CreateCameraDefault();


  Ray_Initialize_World ( World1 );
  Ray_Active_World ( World1 );


  rgbcol *Textr;  
  rgbcol *Textr2;  
  LoadTGA(&Textr,"col.tga");
  LoadTGA(&Textr2,"marble21.tga");

  /*
  Ray_Active_Object ( Ray_ADD_Object ( OPlanes[0] ) );
  TPlane_R_Init (0,-10,0,0,1,0,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_Texture ( (rgbcol *)Textr );
  OPlanes[0]->Pigment.BitsDefinition = 8;
  Ray_Set_TextScale ( 6 );
 // Ray_Enable (R_REFLECT);
//  Ray_Set_ReflectInts (50);
  

  Ray_Active_Object ( Ray_ADD_Object ( OCylinders[0] ) );
  TCylinder_R_Init (10,0,10,2,1,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_Texture ( (rgbcol *)Textr2 );
  Ray_Set_TextScale ( 20 );
  Ray_Set_Phong ( 1, 100 ) ;

  Ray_Active_Object ( Ray_ADD_Object ( OCylinders[1] ) );
  TCylinder_R_Init (10,0,-10,2,1,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 20 );
  Ray_Set_Texture ( (rgbcol *)Textr2 );
  Ray_Set_Phong ( 1, 100 ) ;

  Ray_Active_Object ( Ray_ADD_Object ( OCylinders[2] ) );
  TCylinder_R_Init (-10,0,-10,2,1,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 20 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Set_Texture ( (rgbcol *)Textr2 );

  Ray_Active_Object ( Ray_ADD_Object ( OCylinders[3] ) );
  TCylinder_R_Init (-10,0,10,2,1,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 20 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Set_Texture ( (rgbcol *)Textr2 );

 */ 
  Ray_Active_Object ( Ray_ADD_Object ( OEllipses[0] ) );
  TEllipse_R_Init (0,0,0,8,8,8,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 2 );
//  Ray_Set_Phong ( 1, 100 ) ;
 // Ray_Enable (R_REFLECT);
 // Ray_Set_ReflectInts (127);
 // Ray_Enable (R_TRANSPARENT);
 // Ray_Set_IORValues ( 0.4 , 150 );
  //Ray_Set_Texture ( (rgbcol *)Textr2 );

  OCylinders[0]->Pigment.BitsDefinition = 8;
  OCylinders[1]->Pigment.BitsDefinition = 8;
  OCylinders[2]->Pigment.BitsDefinition = 8;
  OCylinders[3]->Pigment.BitsDefinition = 8;
  OEllipses[0]->Pigment.BitsDefinition = 8;
  /*
  Ray_Active_Object ( Ray_ADD_Object ( OEllipses[0] ) );
  TEllipse_R_Init (0,0,0,2,10,2,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 5 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);

  Ray_Active_Object ( Ray_ADD_Object ( OEllipses[1] ) );
  TEllipse_R_Init (0,0,0,10,2,2,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 5 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);

  Ray_Active_Object ( Ray_ADD_Object ( OEllipses[2] ) );
  TEllipse_R_Init (0,0,0,2,2,10,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 5 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);
  */

/*
  Ray_Active_Object ( Ray_ADD_Object ( OEllipses[0] ) );
  TEllipse_R_Init (0,20,0,5,20,5,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 1 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);
*/
/*
  Ray_Active_Object ( Ray_ADD_Object ( OSpheres[1] ) );
  TSphere_R_Init (8,0,8,2,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 1 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);


  Ray_Active_Object ( Ray_ADD_Object ( OSpheres[2] ) );
  TSphere_R_Init (8,0,-8,0.6,0,0);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);


  Ray_Active_Object ( Ray_ADD_Object ( OSpheres[3] ) );
  TSphere_R_Init (-8,0,-8,0.6,0,0);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
*/
  /*
  Ray_Active_Object ( Ray_ADD_Object ( OSpheres[4] ) );
  TSphere_R_Init (-8,0,8,2,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 1 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);

  Ray_Active_Object ( Ray_ADD_Object ( OSpheres[5] ) );
  TSphere_R_Init (-8,4,8,1.5,1,1);
  Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
  Ray_Set_TextScale ( 1 );
  Ray_Set_Phong ( 1, 100 ) ;
  Ray_Enable (R_REFLECT);
  Ray_Set_ReflectInts (50);
*/
 srand((int)time(NULL));
 float  Val[NSP+10][3];

 for ( i = 0; i < NSP2; i++)
  {
    Ray_Active_Object ( Ray_ADD_Object ( OSpheres[i] ) );
//    TSphere_R_Init ((float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(50+rand()%160)/100.0,1,1);
    TSphere_R_Init (sin((TReal)i*16.0/(TReal)NSP)*10.0,i/2.0,cos((TReal)i*16.0/(TReal)NSP)*10.0,2+(rand()%100)/50.0,1,1);
    Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
    Ray_Set_TextScale ( 1 );
    Ray_Set_Phong ( 1, 100 ) ;
 //  Ray_Enable (R_REFLECT);
    Ray_Set_ReflectInts (50);
    Ray_Set_Texture ( (rgbcol *)Textr );
  }

 /*
 for ( i = 0; i < NSP2; i++)
  {
    Ray_Active_Object ( Ray_ADD_Object ( OSpheres[i+NSP2] ) );
//    TSphere_R_Init ((float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(50+rand()%160)/100.0,1,1);
    TSphere_R_Init (sin((TReal)i*16.0/(TReal)NSP)*10.0,cos((TReal)i*16.0/(TReal)NSP)*10.0,i/2.0,2,1,1);
    Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
    Ray_Set_TextScale ( 1 );
    Ray_Set_Phong ( 1, 100 ) ;
 //  Ray_Enable (R_REFLECT);
    Ray_Set_ReflectInts (50);
    Ray_Set_Texture ( (rgbcol *)Textr );

  }
 for ( i = 0; i < NSP2; i++)
  {
    Ray_Active_Object ( Ray_ADD_Object ( OSpheres[i+NSP2*2] ) );
//    TSphere_R_Init ((float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(-800+rand()%1600)/100.0,(float)(50+rand()%160)/100.0,1,1);
    TSphere_R_Init (i/2.0,sin((TReal)i*16.0/(TReal)NSP)*10.0,cos((TReal)i*16.0/(TReal)NSP)*10.0,2,1,1);
    Ray_Set_Color ( 150+rand()%100,150+rand()%100,150+rand()%100);
    Ray_Set_TextScale ( 1 );
    Ray_Set_Phong ( 1, 100 ) ;
 //  Ray_Enable (R_REFLECT);
    Ray_Set_ReflectInts (50);
    Ray_Set_Texture ( (rgbcol *)Textr );
  }
*/

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  long *Dst  = (long *) GetMem (R_ScreenX*R_ScreenX*sizeof(long ));
  long *Dst3  = (long *) GetMem (R_ScreenX*R_ScreenX*sizeof(long ));
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  TVertex P,L,N;
  VTX_Set (P,40,40,40);
  VTX_Set (L,0,0,0);
  VTX_Set (N,0,1,0);
  Ray_Active_Camera ( Camera1 );
  TCamera_R_Init (P,L,N,256,90);
  TCamera_R_Actualize (R_ScreenX,R_ScreenY);
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  ActiveWorld->AMBIENT = 40;
//  RCheck_Bounder_Range = 4;
 // RW_Enable ( RW_PREDICTION_ANIM )                                         ;

  RW_Enable ( RW_SHADOWS )                                         ;
  RW_Enable ( RW_REFLECT )                                         ;
  RW_Enable ( RW_RESULTS )                                         ;
  RW_Enable ( RW_REFRACT )                                         ;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  Bounder_Init(R_ScreenX,R_ScreenY,R_RAY_DIM,R_RAY_DIM);
  float a;

  ProceduralTexture Prova3 = {
    2,5,
    {
     {0    ,0,0,0,0.7},
     {0.3  ,1,0.7,0.5,0.3},
     {0.5  ,1,0.7,0.5,0},
     {0.7  ,1,0.7,0.5,0.3},
     {1    ,0,0,0,0.7}
    },
    0.7,
      {0.5,1.3,2,
      {1.5,1.60,1.82}

    }
  };

  ProceduralTexture Prova4 = {
    2,5,
    {
     {0    ,0,0,0,0.7},
     {0.3  ,1,0.4,0.3,0.3},
     {0.5  ,1,0.7,0.7,0},
     {0.7  ,1,0.4,0.3,0.3},
     {1    ,0,0,0,0.7}
    },
    0.27,
      {0.5,1.3,6,
      {1.5,2.60,2.82}

    }
  };

  ProceduralTexture Prova32 = {
    2,3,
    {
     {0    ,0,0,0,0.7},
     {0.5  ,0.5,0.3,0.3,0.7},
     {1    ,0,0,0,0.7}
    },
    0.17,
      {0.5,1.3,5,
      {1.5,1.60,1.92}

    }
  };
  ProceduralTexture Prova42 = {
    2,3,
    {
     {0    ,1,0,0,0.7},
     {0.5  ,0.8,0.5,0.7,0.7},
     {1    ,1,0,0,0.7}
    },
    0.27,
      {0.5,1.3,5,
      {1.0,1.90,2.01}

    }
  };

  ProceduralTexture Prova5 = {
    0,0,
    {{0    ,0,0,0,0.7}},
    30,
      {0.5,2.3,6, {0,0,0}

    }
  };

  Prova5.CNext1 = &Prova32;
  Prova5.CNext2 = &Prova42;

  //OSpheres[0]->Pigment.Texture = &Prova3;
  //OPlanes[0]->Pigment.Texture = &Prova4;
  //OPlanes[0]->Pigment.Texture = &Prova5;
  InitTextureTable();
  /*
  OEllipses[0]->Pigment.Texture = &Prova3;
  OEllipses[1]->Pigment.Texture = &Prova3;
  OEllipses[2]->Pigment.Texture = &Prova3;
  OCylinders[0]->Pigment.Texture = &Prova3;
  OCylinders[1]->Pigment.Texture = &Prova3;
  OCylinders[2]->Pigment.Texture = &Prova3;
  OCylinders[3]->Pigment.Texture = &Prova3;
  for ( i = 0; i < (NSP); i++)
  {
    OSpheres[i]->Pigment.Texture = &Prova3;
   Ray_Set_Texture ( (rgbcol *)Textr2 );
  }

*/  

  R_FOG = 0;
  R_FOG_DISTANCE_FACTOR= 2;
  R_FOG_R = 0;
  R_FOG_G = 0;
  R_FOG_B = 0;
  R_BACK_COLOR = (((((dword)(R_FOG_R*255))<<8)+ (dword)(R_FOG_G*255) )<<8)+(dword) (R_FOG_B*255);


  ProceduralTexture Fog = {
    1,8,
    {
     {0    ,1,1,1,1},
     {0.2  ,1,1,1,0.95},
     {0.5  ,1,1,1,0.95},
     {0.8  ,1,1,1,0.95},
     {1    ,1,1,1,1}
    },
    2.0,
      {0.5,1.3,6,
      {60,60,60}

    }
  };



  ActiveFog = &Fog;
  ActiveFogScale = 400;
  ActiveFogInvScale = 1.0/ActiveFogScale;
  char NameF[50];
  long FF = 0;

  long ID_Llum1  = Ray_ADD_Light  ( Llum1 );
//  long ID_Llum2  = Ray_ADD_Light  ( Llum2 );

  Ray_Active_Light ( ID_Llum1 );
  TLightPoint_R_Init ( 0 ,0 ,0 );

/*
  Ray_Active_Light ( ID_Llum2 );
  TLightPoint_R_Init ( 0 ,37 ,0);
*/
  while ( !EsperaEsc )
  {
    a += 2;

    ActiveCamera->Position.x = sin (a/43)*28;
    ActiveCamera->Position.y = 10 + sin (a/23)*8;
    ActiveCamera->Position.z = cos (a/43)*28;
    TCamera_R_Actualize (R_ScreenXView, R_ScreenYView);


    Llum1->Position.x = sin (a/34)*118;
    Llum1->Position.z=  cos (a/31)*125;

    Llum2->Position.x = cos (a/42)*148;
    Llum2->Position.z=  sin (a/45)*168;

    ActiveWorld->Precalculated = 0;
/*
 for ( i = 6; i < NSP+6; i++)
  {
    OSpheres[i]->Position.x = sin ((a+Val[i][0])/35)*Val[i][1];
    OSpheres[i]->Position.y = sin ((a+Val[i][0])/25)*Val[i][2];
    OSpheres[i]->Position.z = cos ((a+Val[i][0])/35)*Val[i][1];
  }
*/


    /*
    OSpheres[1]->Position.y = cos (a*R_PIx2/300)*5;
    OSpheres[2]->Position.y = sin (a*R_PIx2/260)*5;
    OSpheres[3]->Position.y = cos ((a+200)*R_PIx2/250)*5;
    OSpheres[4]->Position.y = sin ((a+230)*R_PIx2/330)*5;
    OSpheres[5]->Position.x = sin (a*3*R_PIx2/330)*6;
    OSpheres[5]->Position.z = cos (a*3*R_PIx2/330)*6;
    */

    TWorld_GetFrame ( Camera1, R_ScreenXView, R_ScreenYView, 6 , RayDimX,RayDimY,(rgbcol*)Dst);

//    TWorld_GetFrameFog ( Camera1, R_ScreenXView, R_ScreenYView, 2 , RayDimX,RayDimY,(rgbcol*)Dst);
//    TWorld_CalcRealisticFog ( Camera1, R_ScreenXView, R_ScreenYView, 7 , RayDimX,RayDimY,(rgbcol*)Dst);

    ToScreen ((dword *)Dst);


  /*

  //
  //  real-3D
  //
    ActiveCamera->Position.x = -1;
    TCamera_R_Actualize (R_ScreenXView, R_ScreenYView);
    ActiveWorld->Precalculated = 0;
    TWorld_GetFrame ( Camera1, R_ScreenXView, R_ScreenYView, 2 , RayDimX,RayDimY,(rgbcol*)Dst);
    Interpola ((dword *)Dst,(dword *)Dst3)  ;

    for (long i= 0; i<R_ScreenSize; i++)
    {
        ((rgbcol*)(Dst2+i))->B.B = ((rgbcol*)(Dst3+i))->B.B;
        ((rgbcol*)(Dst2+i))->B.G = (((rgbcol*)(Dst2+i))->B.G+((rgbcol*)(Dst3+i))->B.G)>>1;
    }
*/

    //FF ++;

/*
    AW3D_BeginScene ();
    int l,k;
    for (l=RBorderV; l<R_ScreenY; l += RayDimY)
    {
      long D1 = l*R_ScreenX;
      for (k=RBorderH; k<R_ScreenX; k += RayDimX)
      {  Dst[D1+k] = rand()%0xffffff;
    }}
    for (l=RBorderV; l<R_ScreenY; l += RayDimY)
    {
      long D1 = l*R_ScreenX;
      long D2 = (l+RayDimY)*R_ScreenX;

      for (k=RBorderH; k<R_ScreenX; k += RayDimX)
      {

       src_v2[0].sx = k;
       src_v2[0].sy = l;
       src_v2[0].color = Dst[D1+k];

       src_v2[1].sx = k+RayDimX;
       src_v2[1].sy = l;
       src_v2[1].color = Dst[D1+k+RayDimX];

       src_v2[2].sx = k;
       src_v2[2].sy = l+RayDimY;
       src_v2[2].color = Dst[D2+k];

       AW3D_D3DDevice->DrawPrimitive (D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,(LPVOID)src_v2,3,0);

       src_v[1].sx = k+RayDimX;
       src_v[1].sy = l;
       src_v[1].color = Dst[D1+k+RayDimX];

       src_v[2].sx = k+RayDimX;
       src_v[2].sy = l+RayDimY;
       src_v[2].color = Dst[D2+k+RayDimX];

       src_v[0].sx = k;
       src_v[0].sy = l+RayDimY;
       src_v[0].color = Dst[D2+k];

       AW3D_D3DDevice->DrawPrimitive (D3DPT_TRIANGLELIST,D3DFVF_TLVERTEX,(LPVOID)src_v,3,0);


      }
    }

    //AW3D_D3DDevice->DrawPrimitive (D3DPT_TRIANGLELIST,D3DFVF_XYZ | D3DFVF_DIFFUSE,(LPVOID)vec,3,0);
    AW3D_EndScene ();

*/

  /*
  dword *DST = (dword *) VIDEO_Lock(Pitch);

   for (l=0; l<R_ScreenY; l += RayDimY)
    {
      for (int k=0; k<R_ScreenX; k += RayDimX)
      {
        DST[l*R_ScreenX+k] = 0x00000000;
      }
    }

  VIDEO_Unlock();
 */

  }

  

  CloseScreen();
  CloseContext();
}


/*
void TWorld_CalcRealisticFog ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST)
{

  TReal dX0,dX1,dY0,dY1,dZ0,dZ1,dX,dY,dZ;
  TVertex Check,Dir;
  TVertex Rest,Pos;

  VTX_Copy (Pos,Panasonic->Position) ;
  dX0 = Panasonic->dX0;
  dX1 = Panasonic->dX1;
  dY0 = Panasonic->dY0;
  dY1 = Panasonic->dY1;
  dZ0 = Panasonic->dZ0;
  dZ1 = Panasonic->dZ1;
  dX = Panasonic->dX;
  dY = Panasonic->dY;
  dZ = Panasonic->dZ;

  TReal Y0 = (R_ScreenY/2)-SY/2;
  TReal X0 = (R_ScreenX/2)-SX/2;
  dword iY0 = Y0;
  dword iX0 = X0;

  TReal tdX = dX;
  TReal tdY = dY;
  TReal tdZ = dZ;
  TReal TMin = 0.0;
  TReal TMin2 = 0.0;
  dX0 *= GX; dY0 *= GX; dZ0 *= GX;
  dX1 *= GY; dY1 *= GY; dZ1 *= GY;

  RayInfo RayPtr;


  dword *dst = (dword *)(DST+(R_ScreenX*iY0)+iX0);
  dword Incy = R_ScreenX*GY;

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

      TMin = 0.0;

      RayPtr.Color.ColRGB =  0;


      byte FIRT_LEVEL = 1;
      RealColor Col,P1;
      P1.RGBA[0]  = P1.RGBA[1]  = P1.RGBA[2]  = P1.RGBA[3]  = 0;
      Col.RGBA[0] = Col.RGBA[1] = Col.RGBA[2] = Col.RGBA[3] = 0;
      float LastAlfa = 0;

      for (long NumInt = 0; NumInt < Deep; NumInt++)
      {
        TMin += NumInt*2;
        VTX_Set (Check,Pos.x + TMin*Dir.x,Pos.y + TMin*Dir.y,Pos.z + TMin*Dir.z);
        ProceduralTexture *TXT = ActiveFog;
        while (TXT != NULL )
        {
            GetTextureColor(TXT,Check,ActiveFogScale,ActiveFogInvScale,Col);
            if (FIRT_LEVEL) { LastAlfa = Col.RGBA[3] ; Col.RGBA[3] = 1 ; FIRT_LEVEL = 0; }
            P1.RGBA[0] = Col.RGBA[0]*Col.RGBA[3] + P1.RGBA[0]*(1-Col.RGBA[3]);
            P1.RGBA[1] = Col.RGBA[1]*Col.RGBA[3] + P1.RGBA[1]*(1-Col.RGBA[3]);
            P1.RGBA[2] = Col.RGBA[2]*Col.RGBA[3] + P1.RGBA[2]*(1-Col.RGBA[3]);
            TXT = TXT->Next;
        }
      }

        RayPtr.Color.B.R =(byte) (P1.RGBA[0]*255.0);
        RayPtr.Color.B.G =(byte) (P1.RGBA[1]*255.0);
        RayPtr.Color.B.B =(byte) (P1.RGBA[2]*255.0);
        RayPtr.Color.B.A =(byte) (LastAlfa*255.0);

        //*(dst+x) = RayPtr.Color.ColRGB;

        ((rgbcol*)(dst+x))->B.R = (byte)((P1.RGBA[0]*(1-Col.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.R/256.0)*(Col.RGBA[3]))*255.0);
        ((rgbcol*)(dst+x))->B.G = (byte)((P1.RGBA[1]*(1-Col.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.G/256.0)*(Col.RGBA[3]))*255.0);
        ((rgbcol*)(dst+x))->B.B = (byte)((P1.RGBA[2]*(1-Col.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.B/256.0)*(Col.RGBA[3]))*255.0);

        //P1.RGBA[0] = Col.RGBA[0]*Col.RGBA[3] + P1.RGBA[0]*(1-Col.RGBA[3]);
        //P1.RGBA[1] = Col.RGBA[1]*Col.RGBA[3] + P1.RGBA[1]*(1-Col.RGBA[3]);
        //P1.RGBA[2] = Col.RGBA[2]*Col.RGBA[3] + P1.RGBA[2]*(1-Col.RGBA[3]);

      tdX+=dX0; tdY+=dY0; tdZ+=dZ0;
    }
    dst +=Incy;
    dX+=dX1; dY+=dY1; dZ+=dZ1;
  }

}


void TWorld_GetFrameFog ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST)
{

  TReal dX0,dX1,dY0,dY1,dZ0,dZ1,dX,dY,dZ;
  TVertex Check,Dir,Norm;
  TVertex Rest,Pos;
  ActiveWorld->Precalculated = 0;
  VTX_Copy (Pos,Panasonic->Position) ;

  dX0 = Panasonic->dX0;
  dX1 = Panasonic->dX1;
  dY0 = Panasonic->dY0;
  dY1 = Panasonic->dY1;
  dZ0 = Panasonic->dZ0;
  dZ1 = Panasonic->dZ1;
  dX = Panasonic->dX;
  dY = Panasonic->dY;
  dZ = Panasonic->dZ;

  TReal T0 = 10000,T1 = 10000, TMin = 1000,TMax;

  TReal TMin2;

  long  Hit = 0;

  long  Object_ID = 0;

  TReal Y0 = (R_ScreenY/2)-SY/2;
  TReal X0 = (R_ScreenX/2)-SX/2;
  dword iY0 = (dword) (Y0);
  dword iX0 = (dword) (X0);

  RBorderV = iY0 ;
  RBorderH = iX0 ;

  TReal ValNorm;
  word Shad;

  TReal tdX = dX;
  TReal tdY = dY;
  TReal tdZ = dZ;
  dX0 *= GX; dY0 *= GX; dZ0 *= GX;
  dX1 *= GY; dY1 *= GY; dZ1 *= GY;

  rgbcol LightIntentsPNG;
  LightIntentsPNG.B.A = 0;

  RayInfo RayPtr;


  long Inv;

  TReal T2 ;
  TVertex Dir2;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Precalculem Escena
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
  TWorld_Init ( Pos );
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±  Calculem Frame
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef DEBUG_RENDER
    cprintf ("Redering scene\r\n");
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

  dword *dst = (dword *)(DST+(R_ScreenX*iY0)+iX0);
  dword Incy = R_ScreenX*GY;

  Check_Bounder(1+SX/GX,1+SY/GY);

  long PosBounder = 0;
  long IncBounder1 = 0;
  long IncBounder2 = 1+SX/GX;
  long ValBounder = 0;


  long NumCh = 0;
  long NumNCh = 0;
  long PortalFound ;

  for (register long y=iY0; y<(SY+iY0); y += GY)
  {

    tdX = dX;
    tdY = dY;
    tdZ = dZ;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef DEBUG_RENDER
    cprintf ("Rendering Line[%d]\r\n",y);
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

    for (register long x=iX0; x<(SX+iX0); x += GX)
    {
      Dir.x = tdX;
      Dir.y = tdY;
      Dir.z = tdZ;
      TMin = T0 = T1 = 10000.0;
      Hit = 0;
      TMax = -100000;
      PortalFound = 0;


      #ifdef SphereOK

        CompletDir = Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z;
        InvCompletDir = 1.0 / CompletDir;
      #endif


    if (ActiveWorld->Flags & RW_PREDICTION_ANIM)
        ValBounder = BounderCheck[PosBounder+IncBounder1];

    #ifdef DEBUG_RENDER
        cprintf ("Bounder ID[%d]\r\n",ValBounder);
    #endif
     if (ValBounder)
     {
        if (ValBounder != 255)
        {
            Ray_Active_Object ( ValBounder );
            if (Ray_GetPrimaryIntersection ( Dir, T0, T1 ))
            {
                if ( (T0< TMin)  )
                {
                Object_ID = ValBounder;
                TMin = T0;
                Hit = 1;
        //±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±#define DEBUG_PREDICTION±±±±±
                    #ifdef DEBUG_RENDER
                        cprintf ("Hit ID[%d] at %dx%d\r\n",Object_ID,x,y);
                    #endif
        //±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
                }
             #ifdef DEBUG_PREDICTION
               NumCh ++;
             #endif
            }
        }

     } else

PortalBucle:

     {
      for (register long Cmt = 1 ; Cmt <= ActiveWorld->Num_Of_Obj; Cmt++)
      {
        Ray_Active_Object ( Cmt );
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
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
              #ifdef DEBUG_RENDER
                  cprintf ("Hit ID[%d] at %dx%d\r\n",Cmt,x,y);
              #endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
          }
        }
      }
      #ifdef DEBUG_PREDICTION
      NumNCh ++;
      #endif
     }


      if (Hit && ( TMin < R_FAR_CLIP )   )
      {
        RayPtr.Color.ColRGB =  0;
        VTX_Set (Check,Pos.x + TMin*Dir.x,Pos.y + TMin*Dir.y,Pos.z + TMin*Dir.z);

        Ray_Active_Object ( Object_ID );

        Inv = ActiveObject->Invers;

        R_ReturnStatus = 0;

        //ActiveObject->GetColorInfo (RayPtr,Check);
        Ray_GetColorInfo ( RayPtr,Check);

        if (ActiveWorld->Flags & RW_PREDICTION_ANIM)
         if (!PortalFound) *(Bounder+PosBounder+IncBounder1) = Object_ID;

        if ( R_ReturnStatus == R_PORTAL_TEXT)
        {

            PortalFound = 1;
//            PushWorld ((TWorld_R *)ActiveObject->PortalToWorld1);
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
          if ( Trans  && (ActiveWorld->Flags & RW_REFLECT))
            TWorld_Reflect ( Object_ID,Dir,Norm,Check,RayPtr,Deep,Push2->IOR,Push2->IOR_INTS);
          if ( Refl  && (ActiveWorld->Flags & RW_REFRACT))
            TWorld_Reflect ( Object_ID,Dir,Norm,Check,RayPtr,Deep,-2.0,Push2->REFL_INTS);
        }
        if (LightIntentsPNG.ColRGB)
        {
#ifdef DEF_NO_MMX
          RayPtr.Color.B.R = RayPtr.Color.B.R*(255-LightIntentsPNG.B.R)/256 + LightIntentsPNG.B.R;
          RayPtr.Color.B.G = RayPtr.Color.B.G*(255-LightIntentsPNG.B.G)/256 + LightIntentsPNG.B.G;
          RayPtr.Color.B.B = RayPtr.Color.B.B*(255-LightIntentsPNG.B.B)/256 + LightIntentsPNG.B.B;

#else
#endif
        }
        if (R_FOG)
        {
        TMin *= R_FOG_DISTANCE_FACTOR;
        if (TMin>255) TMin = 255;

        dword FogIntens = (dword) (TMin);

#ifdef DEF_NO_MMX
        RayPtr.Color.B.R = RayPtr.Color.B.R*(255-FogIntens)/256 + FogIntens*R_FOG_R;
        RayPtr.Color.B.B = RayPtr.Color.B.B*(255-FogIntens)/256 + FogIntens*R_FOG_B;
        RayPtr.Color.B.G = RayPtr.Color.B.G*(255-FogIntens)/256 + FogIntens*R_FOG_G;
#else
        FogIntens += FogIntens << 8;
//        RAY_CalcularLlum (&RayPtr.Color,(rgbcol *)&FogIntens);
#endif
        }

        *(dst+x) = RayPtr.Color.ColRGB;


      } else
      {
        TMin = 1000.0;
        *(dst+x) = R_BACK_COLOR;
        if (ActiveWorld->Flags & RW_PREDICTION_ANIM) *(Bounder+PosBounder+IncBounder1) = 255;
      }


      RayPtr.Color.ColRGB =  0;

      TMin2 = 0.0;

      byte FIRT_LEVEL = 1;
      RealColor Col,P1;
      P1.RGBA[0]  = P1.RGBA[1]  = P1.RGBA[2]  = 0;
      P1.RGBA[3]  = 1;
      Col.RGBA[0] = Col.RGBA[1] = Col.RGBA[2] = Col.RGBA[3] = 0;
      float LastAlfa = 0;

      for (long NumInt = 0; (NumInt < FogDeep) && (TMin2<TMin); NumInt++)
      {
        TMin2 += NumInt*FogJump;
        VTX_Set (Check,Pos.x + TMin2*Dir.x,Pos.y + TMin2*Dir.y,Pos.z + TMin2*Dir.z);
        ProceduralTexture *TXT = ActiveFog;
        while (TXT != NULL )
        {
            GetTextureColor(TXT,Check,ActiveFogScale,ActiveFogInvScale,Col);
            P1.RGBA[3] = P1.RGBA[3]*Col.RGBA[3];

            if (FIRT_LEVEL)
            {
               LastAlfa = Col.RGBA[3] ; Col.RGBA[3] = 1 ; FIRT_LEVEL = 0;
            }

            P1.RGBA[0] = Col.RGBA[0]*Col.RGBA[3] + P1.RGBA[0]*(1-Col.RGBA[3]);
            P1.RGBA[1] = Col.RGBA[1]*Col.RGBA[3] + P1.RGBA[1]*(1-Col.RGBA[3]);
            P1.RGBA[2] = Col.RGBA[2]*Col.RGBA[3] + P1.RGBA[2]*(1-Col.RGBA[3]);

            TXT = TXT->Next;
        }
      }

      if (NumInt)
      {
        ((rgbcol*)(dst+x))->B.R = (byte)((P1.RGBA[0]*(1-P1.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.R/256.0)*(P1.RGBA[3]))*255.0);
        ((rgbcol*)(dst+x))->B.G = (byte)((P1.RGBA[1]*(1-P1.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.G/256.0)*(P1.RGBA[3]))*255.0);
        ((rgbcol*)(dst+x))->B.B = (byte)((P1.RGBA[2]*(1-P1.RGBA[3]) + ((float)((rgbcol*)(dst+x))->B.B/256.0)*(P1.RGBA[3]))*255.0);
      }
     //  LOG ("%f at %d %d width %d %f",TMin,x,y,NumInt,P1.RGBA[3]);


      tdX+=dX0; tdY+=dY0; tdZ+=dZ0;
      IncBounder1++;
//      if (PortalFound) PopWorld ();
    }
    dst +=Incy;
    PosBounder +=IncBounder2;
    IncBounder1 = 0;
    dX+=dX1; dY+=dY1; dZ+=dZ1;
  }

  #ifdef DEBUG_PREDICTION
    LOG ("RAY_ANIM_PREDICTION-->C[%d]N[%d]\n",NumCh,NumNCh);
  #endif
}
*/

void LoadTGA(rgbcol **Pantalla,char *namef)
    {
      char DummyC;
      char DummyC2;
      word DummyW;
      word WidthX;
      word WidthY;

      FILE *OUT_TGA = fopen (namef,"rb");
      DummyC = fgetc(OUT_TGA);  /* Byte 0  - Length of Image ID field */
      DummyC = fgetc(OUT_TGA);  /* Byte 1  - Color map type (0 is no color map) */
      DummyC = fgetc(OUT_TGA);  /* Byte 2  - Type of targa file */
     // if (DummyC != 2) exit(0);
      DummyC = fgetc(OUT_TGA);  /* Byte 3  - Index of first color map entry LSB */
      DummyC = fgetc(OUT_TGA);  /* Byte 4  - Index of first color map entry MSB */
      DummyC = fgetc(OUT_TGA);  /* Byte 5  - Color map length LSB */
      DummyC = fgetc(OUT_TGA);  /* Byte 6  - Color map legth MSB */
      DummyC = fgetc(OUT_TGA);  /* Byte 7  - Color map size */
      DummyC = fgetc(OUT_TGA);  /* Byte 8  - First Column % 8 */
      DummyC = fgetc(OUT_TGA);  /* Byte 9  - First Column / 8 */
      DummyC = fgetc(OUT_TGA);  /* Byte 10 - First Line % 8 */
      DummyC = fgetc(OUT_TGA);  /* Byte 11 - First Line / 8 */
      DummyC2 = fgetc(OUT_TGA);
      DummyC = fgetc(OUT_TGA);
      WidthX = (DummyC<<8) + DummyC2 ;
      
      DummyC2 = fgetc(OUT_TGA);
      DummyC = fgetc(OUT_TGA);

      WidthY = (DummyC<<8) + DummyC2 ;
      

      LOG ("%s",namef);
      LOG ("%d %d",WidthX, WidthY);
      DummyC = fgetc(OUT_TGA);  /* Byte 11 - First Line / 8 */
      LOG ("%d",DummyC);

      if (DummyC != 32) exit(0);
      
      DummyC = fgetc(OUT_TGA);  /* Byte 11 - First Line / 8 */
      LOG ("%d",DummyC);
      //if (DummyC != 0x28) exit(0);
      
      (*Pantalla) = (rgbcol*)malloc (256*256*sizeof(rgbcol));
      fread ((*Pantalla),256*256,sizeof(dword),OUT_TGA);
      LOG ("%s",namef);


      /*
      for (long i = 0; i < R_ScreenX*R_ScreenY; i++)
      {
        Dest.B.R = Pantalla[i].B.R;
        Dest.B.G = Pantalla[i].B.G;
        Dest.B.B = Pantalla[i].B.B;
        Dest.B.A = Pantalla[i].B.A;
        fwrite (&Dest,sizeof(rgbcol),1,OUT_TGA);
      }
      
      */

      fclose (OUT_TGA);
    }

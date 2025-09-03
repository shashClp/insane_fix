#include <windows.h>
#include <image.h>
#include <profile.h>
#include <log.h>
#include <mem.h>
#include <blobs3d.h>
#include <ball.h>
#include <efx2d.h>
#include <defines.h>
#include <guio.h>
#include <view3d.h>
#include <misc.h>
#include <rgb.h>
#include <default.h>
#include <object3dpaint.h>
#include <keys.h>
#include <math.h>
#include <view3d.h>

TMatrix BLOBS_fin;
TVertex insaneBLOBS_Formes[5][20];
float insaneBLOBS_lia;
float insaneBLOBS_lfa;
float insaneBLOBS_sia;
float insaneBLOBS_sfa;
float insaneBLOBS_ria;
float insaneBLOBS_rfa;
DWORD insaneBLOBS_ia;
DWORD insaneBLOBS_fa;
TBall insaneBLOBS_Forma[20];
float insaneBLOBS_cameramov;
float insaneBLOBS_cameramovchange;
DWORD insaneBLOBS_paintertype;
TTexture *insaneBLOBS_txt;
TTexture *insaneBLOBS_txt2;
TImage *insaneBLOBS_flare;
TView3D *BLOBS_view;
float BLOBSscalex,BLOBSscaley,BLOBSscalez;

bool insaneBLOBS_Init()
{
DWORD i,j;

  SEC_START ("Initializing BLOBS");

  BLOBS_view = VIEW3D_Init (GBL_Width-2,GBL_Height-40,GBL_Width/2,GBL_Height/2,10,10000,70,1);
  if (!BLOBS_view) return false;
  
  if (!BLOBS3D_Init (24,24,24,10000,10000)) return false;  
  VTX_Set (BLOBS3D_object.bound.org,0,0,0);
  BLOBS3D_object.bound.dist=30*15/2;
  BLOBS3D_limit = 25.0f;

  TVertex min,max;
  VTX_Set (min,-30.0f,-30.0f,-30.0f);
  VTX_Set (max,+30.0f,+30.0f,+30.0f);
  BALL_Init (min,max);  

  // Set 1
  VTX_Set (insaneBLOBS_Formes[0][ 0],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 1],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 2],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 3],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 4],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 5],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 6],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 7],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 8],  8,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][ 9], 16,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][10],-16,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][11], -8,  0,  0);
  VTX_Set (insaneBLOBS_Formes[0][12],  0,  8,  0);
  VTX_Set (insaneBLOBS_Formes[0][13],  0, 16,  0);
  VTX_Set (insaneBLOBS_Formes[0][14],  0,-16,  0);
  VTX_Set (insaneBLOBS_Formes[0][15],  0, -8,  0);
  VTX_Set (insaneBLOBS_Formes[0][16],  0,  0,  8);
  VTX_Set (insaneBLOBS_Formes[0][17],  0,  0, 16);
  VTX_Set (insaneBLOBS_Formes[0][18],  0,  0,-16);
  VTX_Set (insaneBLOBS_Formes[0][19],  0,  0, -8);  

  // Set 2
  VTX_Set (insaneBLOBS_Formes[1][ 0],-16, 16,-16);
  VTX_Set (insaneBLOBS_Formes[1][ 1],  0, 16,-16);
  VTX_Set (insaneBLOBS_Formes[1][ 2], 16, 16,-16);
  VTX_Set (insaneBLOBS_Formes[1][ 3], 16, 16,  0);
  VTX_Set (insaneBLOBS_Formes[1][ 4], 16, 16, 16);
  VTX_Set (insaneBLOBS_Formes[1][ 5],  0, 16, 16);
  VTX_Set (insaneBLOBS_Formes[1][ 6],-16, 16, 16);
  VTX_Set (insaneBLOBS_Formes[1][ 7],-16, 16,  0);
  VTX_Set (insaneBLOBS_Formes[1][ 8],-16,-16,-16);
  VTX_Set (insaneBLOBS_Formes[1][ 9],  0,-16,-16);
  VTX_Set (insaneBLOBS_Formes[1][10], 16,-16,-16);
  VTX_Set (insaneBLOBS_Formes[1][11], 16,-16,  0);
  VTX_Set (insaneBLOBS_Formes[1][12], 16,-16, 16);
  VTX_Set (insaneBLOBS_Formes[1][13],  0,-16, 16);
  VTX_Set (insaneBLOBS_Formes[1][14],-16,-16, 16);
  VTX_Set (insaneBLOBS_Formes[1][15],-16,-16,  0);
  VTX_Set (insaneBLOBS_Formes[1][16],-16,  0,-16);
  VTX_Set (insaneBLOBS_Formes[1][17],-16,  0, 16);
  VTX_Set (insaneBLOBS_Formes[1][18], 16,  0,-16);
  VTX_Set (insaneBLOBS_Formes[1][19], 16,  0, 16);  
  
  // Set 3
  for (i=0; i<20; i++)
  {
    VTX_Set (insaneBLOBS_Formes[2][i],
        16.0f*(float)sin((float)i*M_PIx2/20.0f),
        16.0f*(float)cos((float)i*M_PIx2/20.0f),0);
  }

  // Set 4
  for (i=0; i<10; i++)
  {
    VTX_Set (insaneBLOBS_Formes[3][i],
        16.0f*(float)sin((float)i*M_PIx2/10.0f),
        16.0f*(float)cos((float)i*M_PIx2/10.0f),0);    
  }
  VTX_Set (insaneBLOBS_Formes[3][10],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][11],  0,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][12],  5,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][13], 10,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][14],-10,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][15], -5,  0,  0);
  VTX_Set (insaneBLOBS_Formes[3][16],  0,  5,  0);
  VTX_Set (insaneBLOBS_Formes[3][17],  0, 10,  0);
  VTX_Set (insaneBLOBS_Formes[3][18],  0,-10,  0);
  VTX_Set (insaneBLOBS_Formes[3][19],  0, -5,  0);

  // Set 5
  for (i=0; i<20; i++)
  {
    float z = (float)i-10.0f;
    VTX_Set (insaneBLOBS_Formes[4][i],
        (10.0f-fabs(z))*(float)sin((float)i*2*M_PIx2/20.0f),
        (10.0f-fabs(z))*(float)cos((float)i*2*M_PIx2/20.0f),
        z*1.6f);
  }

  insaneBLOBS_cameramovchange = 0;
  insaneBLOBS_paintertype = 0;

  insaneBLOBS_txt = TXT_AddTexture ("data\\blob2.tga"); 
  if (!insaneBLOBS_txt) return false;

  insaneBLOBS_txt2 = TXT_AddTexture ("data\\bloblgt1.tga"); 
  if (!insaneBLOBS_txt2) return false;

  insaneBLOBS_flare = IMAGE_Load ("data\\flar2.tga",32);
  if (!insaneBLOBS_flare) return false;

  LOG ("BLOBS initialized!");
  SEC_END();   
  return true;
}

void insaneBLOBS_Close()
{
  LOG ("Closing BLOBS");

  if (BLOBS_view)
  {
    FreeMem (BLOBS_view);
    BLOBS_view = NULL;
  }
  if (insaneBLOBS_flare)
  {
    FreeMem (insaneBLOBS_flare);
    insaneBLOBS_flare = NULL;
  }
  TXT_DelTexture (insaneBLOBS_txt);
  TXT_DelTexture (insaneBLOBS_txt2);
  BLOBS3D_Close ();

  LOG ("BLOBS closed!");
}

void insaneBLOBS_ScaleLayer1(float tick)
{
  BLOBSscalex=15.0f*sin(tick*M_PI);
}

void insaneBLOBS_ScaleLayer2(float tick)
{
  BLOBSscaley=15.0f*sin(tick*M_PI);
}

void insaneBLOBS_ScaleLayer3(float tick)
{
  BLOBSscalez=15.0f*sin(tick*M_PI);
}

void insaneBLOBS_Action1(float tick)
{
  insaneBLOBS_cameramov = 0;
  insaneBLOBS_ia=0;  
  insaneBLOBS_fa=0;
  insaneBLOBS_lia=255;
  insaneBLOBS_sia=40;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=30;
  insaneBLOBS_sfa=50;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_Action2(float tick)
{
  insaneBLOBS_cameramov = 1;
  insaneBLOBS_ia=0;  
  insaneBLOBS_fa=1;

  insaneBLOBS_lia=30;
  insaneBLOBS_sia=50;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=30;
  insaneBLOBS_sfa=30;
  insaneBLOBS_rfa=20;
}

void insaneBLOBS_Action3(float tick)
{
  insaneBLOBS_cameramov = 2;
  insaneBLOBS_ia=1;  
  insaneBLOBS_fa=2;
  insaneBLOBS_lia=30;
  insaneBLOBS_sia=30;
  insaneBLOBS_ria=20;

  insaneBLOBS_lfa=50;
  insaneBLOBS_sfa=40;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_Action4(float tick)
{
  insaneBLOBS_cameramov = 3;
  insaneBLOBS_ia=2;  
  insaneBLOBS_fa=3;
  insaneBLOBS_lia=40;
  insaneBLOBS_sia=40;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=50;
  insaneBLOBS_sfa=50;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_Action5(float tick)
{
  insaneBLOBS_cameramov = 4;
  insaneBLOBS_ia=3;  
  insaneBLOBS_fa=4;

  insaneBLOBS_lia=50;
  insaneBLOBS_sia=50;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=50;
  insaneBLOBS_sfa=50;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_Action6(float tick)
{
  insaneBLOBS_cameramov = 5;
  insaneBLOBS_ia=4;  
  insaneBLOBS_fa=4;
  insaneBLOBS_lia=50;
  insaneBLOBS_sia=50;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=255;
  insaneBLOBS_sfa=10;
  insaneBLOBS_rfa=20;
}

void insaneBLOBS_Action7(float tick)
{
  insaneBLOBS_cameramov = 5;
  insaneBLOBS_ia=4;  
  insaneBLOBS_fa=0;
  insaneBLOBS_lia=50;
  insaneBLOBS_sia=50;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=255;
  insaneBLOBS_sfa=40;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_Action8(float tick)
{
  insaneBLOBS_cameramov = 5;
  insaneBLOBS_ia=0;  
  insaneBLOBS_fa=0;
  insaneBLOBS_lia=255;
  insaneBLOBS_sia=40;
  insaneBLOBS_ria=10;

  insaneBLOBS_lfa=512;
  insaneBLOBS_sfa=40;
  insaneBLOBS_rfa=10;
}

void insaneBLOBS_ActionType1(float tick)
{
  insaneBLOBS_paintertype=0;
}

void insaneBLOBS_ActionType2(float tick)
{
  insaneBLOBS_paintertype=1;
}

void insaneBLOBS_ActionType3(float tick)
{
  insaneBLOBS_paintertype=2;
}

void insaneBLOBS_ActionType4(float tick)
{
  insaneBLOBS_paintertype=3;
}

void insaneBLOBS_ActionType5(float tick)
{
  insaneBLOBS_paintertype=4;
}

void insaneBLOBS_ActionType6(float tick)
{
  insaneBLOBS_paintertype=5;
}

void insaneBLOBS_Layer1(float tick)
{
  insaneBLOBS_cameramovchange = sin(tick*M_PI)*M_PIx2;
}

void insaneBLOBS_Scene1(float tick)
{
TMatrix mat,obj,ifin;

  VIEW3D_Set (BLOBS_view);

  float j = (tick*2.0f)-1.0f;
  if (j<0) j=0;
  if (j>1) j=1;

  float x,y,z,s,r;
  for (DWORD i=0; i<20; i++)
  {
    s = insaneBLOBS_sia+(insaneBLOBS_sfa-insaneBLOBS_sia)*j;
    r = insaneBLOBS_ria+(insaneBLOBS_rfa-insaneBLOBS_ria)*j;
    BALL_Set (insaneBLOBS_Forma[i],r,s);

    TVertex *ini = &insaneBLOBS_Formes[insaneBLOBS_ia][i];
    TVertex *fin = &insaneBLOBS_Formes[insaneBLOBS_fa][i];
    x=ini->x+(fin->x-ini->x)*j;
    y=ini->y+(fin->y-ini->y)*j;
    z=ini->z+(fin->z-ini->z)*j;
    BALL_Set (insaneBLOBS_Forma[i],x,y,z);
  }

  BLOBS3D_limit = insaneBLOBS_lia+(insaneBLOBS_lfa-insaneBLOBS_lia)*j;

  float t = tick+insaneBLOBS_cameramov;
  MTX_LookAt (mat,300.0f+120.0f*sin(t*M_PI*3+insaneBLOBS_cameramovchange),
                  250.0f+100.0f*cos(t*M_PI*2+M_PIx2+insaneBLOBS_cameramovchange),
                  200.0f+120.0f*sin(t*M_PI*3+insaneBLOBS_cameramovchange),0,0,0,t*90);  

  MTX_Rotate (obj,-t*360,-t*180,t*150);
  MTX_Mul (BLOBS_fin,mat,obj);
  MTX_Inverse (ifin,obj);
  
  TVertex v;
  VTX_Set (v,1,1,1);  
  MTX_Mul (OBJ3DPAINT_Llum,v,ifin);
  VTX_Normalize(OBJ3DPAINT_Llum);

  VIEW3D_MakeFrustrum (OBJ3DPAINT_TC->frustrum,BLOBS_fin);

  MISC_MemSetFloat (BLOBS3D_data,0.0f,BLOBS3D_ijkDim);
  BALL_Put (insaneBLOBS_Forma,20);
  BLOBS3D_pofv = OBJ3DPAINT_TC->frustrum.org;  

  VTX_Set (BLOBS3D_scale,15.0f+BLOBSscalex,15.0f+BLOBSscaley,15.0f+BLOBSscalez);
  
  OBJ3DPAINT_BZO->Prepare();
  switch (insaneBLOBS_paintertype)
  {
    case 0:
      RGB_Set (OBJ3DPAINT_color,0,120,250);
      BLOBS3D_MakeMarch ();
      OBJ3DPAINT_OrderRep_Flat_Light (BLOBS_fin,BLOBS3D_object);
      break;
    case 1:
      BLOBS3D_MakeMarch ();
      RGB_Set (OBJ3DPAINT_color,250,180,0);
      OBJ3DPAINT_OrderRep_Flat_Border_Light (BLOBS_fin,BLOBS3D_object);      
      break;
    case 2:
      RGB_Set (OBJ3DPAINT_color,0,120,250);
      BLOBS3D_MakeMarch ();
      OBJ3DPAINT_OrderRep_Flat_Border_Light (BLOBS_fin,BLOBS3D_object);      
      break;
    case 3:
      BLOBS3D_txt = insaneBLOBS_txt;
      BLOBS3D_MakeMarch_Texture ();
      OBJ3DPAINT_OrderRep_PI_MAP (BLOBS_fin,BLOBS3D_object);      
      break;
    case 4:
      RGB_Set (OBJ3DPAINT_color,0,120,250);
      BLOBS3D_txt = insaneBLOBS_txt;
      BLOBS3D_MakeMarch_Texture ();
      OBJ3DPAINT_OrderRep_PI_MAP_Flat_Light (BLOBS_fin,BLOBS3D_object); 
      break;
    case 5:
      BLOBS3D_txt = insaneBLOBS_txt;
      BLOBS3D_MakeMarch_Texture ();
      OBJ3DPAINT_txt2 = insaneBLOBS_txt2;
      OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Environemnt (BLOBS_fin,BLOBS3D_object); 
      break;
  }
  OBJ3DPAINT_BZO->Execute();  

  VIEW3D_Set (View3D);
}

void insaneBLOBS_StarBlobs1(float tick)
{
  float t = 24*sin(tick*M_PI);
  for (DWORD i=0; i<20; i++)
  {
    TVertex v,vr,vp;
    v.x = (insaneBLOBS_Forma[i].x-BLOBS3D_iDim2)*BLOBS3D_scale.x;
    v.y = (insaneBLOBS_Forma[i].y-BLOBS3D_jDim2)*BLOBS3D_scale.y;
    v.z = (insaneBLOBS_Forma[i].z-BLOBS3D_kDim2)*BLOBS3D_scale.z;    
    MTX_Mul (vr,v,BLOBS_fin);
    VIEW3D_Project (vp,vr);
    PutPartRGB (GBL_Bits,(DWORD *)insaneBLOBS_flare->bits,vp.x,vp.y,t);
  }    
}

void insaneBLOBS_StarBlobs2(float tick)
{  
  for (DWORD i=0; i<20; i++)
  {
    TVertex v,vr,vp;
    v.x = (insaneBLOBS_Forma[i].x-BLOBS3D_iDim2)*BLOBS3D_scale.x;
    v.y = (insaneBLOBS_Forma[i].y-BLOBS3D_jDim2)*BLOBS3D_scale.y;
    v.z = (insaneBLOBS_Forma[i].z-BLOBS3D_kDim2)*BLOBS3D_scale.z;    
    MTX_Mul (vr,v,BLOBS_fin);
    VIEW3D_Project (vp,vr);
    PutPartRGB (GBL_Bits,(DWORD *)insaneBLOBS_flare->bits,vp.x,vp.y,24);
  }
}

void insaneBLOBS_Guio()
{
  GUIO_AddFN (insaneBLOBS_Scene1,"insaneBLOBS_Scene1");
  GUIO_AddFN (insaneBLOBS_Action1,"insaneBLOBS_Action1");
  GUIO_AddFN (insaneBLOBS_Action2,"insaneBLOBS_Action2");
  GUIO_AddFN (insaneBLOBS_Action3,"insaneBLOBS_Action3");
  GUIO_AddFN (insaneBLOBS_Action4,"insaneBLOBS_Action4");
  GUIO_AddFN (insaneBLOBS_Action5,"insaneBLOBS_Action5");
  GUIO_AddFN (insaneBLOBS_Action6,"insaneBLOBS_Action6");   
  GUIO_AddFN (insaneBLOBS_Action7,"insaneBLOBS_Action7");   
  GUIO_AddFN (insaneBLOBS_Action8,"insaneBLOBS_Action8");   
  GUIO_AddFN (insaneBLOBS_Layer1,"insaneBLOBS_Layer1");  
  GUIO_AddFN (insaneBLOBS_ActionType1,"insaneBLOBS_ActionType1");
  GUIO_AddFN (insaneBLOBS_ActionType2,"insaneBLOBS_ActionType2");
  GUIO_AddFN (insaneBLOBS_ActionType3,"insaneBLOBS_ActionType3");
  GUIO_AddFN (insaneBLOBS_ActionType4,"insaneBLOBS_ActionType4");
  GUIO_AddFN (insaneBLOBS_ActionType5,"insaneBLOBS_ActionType5");
  GUIO_AddFN (insaneBLOBS_ActionType6,"insaneBLOBS_ActionType6");
  GUIO_AddFN (insaneBLOBS_StarBlobs1,"insaneBLOBS_StarBlobs1");
  GUIO_AddFN (insaneBLOBS_StarBlobs2,"insaneBLOBS_StarBlobs2");
  GUIO_AddFN (insaneBLOBS_ScaleLayer1,"insaneBLOBS_ScaleLayer1");
  GUIO_AddFN (insaneBLOBS_ScaleLayer2,"insaneBLOBS_ScaleLayer2");
  GUIO_AddFN (insaneBLOBS_ScaleLayer3,"insaneBLOBS_ScaleLayer3");
}
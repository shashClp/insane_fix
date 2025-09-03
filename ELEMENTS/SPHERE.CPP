#include <windows.h>
#include <guio.h>
#include <math.h>
#include <vertex.h>
#include <matrix.h>
#include <view3d.h>
#include <view.h>
#include <log.h>
#include <misc.h>
#include <efx2d.h>
#include <default.h>

#define SPHERE_MAX_POINTS 200

TVertex SPHERE_Points[SPHERE_MAX_POINTS];
float SPHERE_Angles[SPHERE_MAX_POINTS];
float SPHERE_Rot[SPHERE_MAX_POINTS];
TImage *SPHERE_flares;
TView3D *SPHERE_view3D;
float SPHERE_fov;
float SPHERE_flaregrow;

bool SPHERE_Init()
{
  SEC_START ("Initializing SPHERE");

  for (DWORD i=0; i<SPHERE_MAX_POINTS; i++)
  {
    TVertex vp;
    TVertex vr;
    TMatrix cam;

    float x = 100.0f+rand()%200;
    float y = 100.0f+rand()%200;
    float z = 100.0f+rand()%200;

    VTX_Set (vp,x,y,z);
    MTX_Rotate (cam,rand()%360,rand()%360,rand()%360);
    MTX_Mul (vr,vp,cam);
    SPHERE_Points[i] = vr;
    SPHERE_Angles[i] = rand()%360;
    SPHERE_Rot[i] = (rand()%10)-5;
  }

  SPHERE_flares = IMAGE_Load ("data\\flar1.tga",32);
  if (!SPHERE_flares) return false;

  SPHERE_view3D = VIEW3D_Init (GBL_Width,GBL_Height,GBL_Width/2,GBL_Height/2,10,10000,60,1);

  SPHERE_fov = 0;
  SPHERE_flaregrow = 0;

  LOG ("SPHERE initialized!");
  SEC_END();
  return true;
}

void SPHERE_Close()
{
  LOG ("Closing SPHERE");

  if (SPHERE_flares)
  {
    FreeMem (SPHERE_flares);
    SPHERE_flares = NULL;
  }
  if (SPHERE_view3D)
  {
    FreeMem (SPHERE_view3D);
    SPHERE_view3D = NULL;
  }
  LOG ("SPHERE closed!");
}

void SPHERE_Layer1(float tick)
{
  SPHERE_fov = 100.0f*sin(tick*M_PI);
}

void SPHERE_Layer3(float tick)
{
  SPHERE_fov = 120.0f*(1.0f-tick);
}

void SPHERE_Layer2(float tick)
{
  SPHERE_flaregrow = 64.0f*sin(tick*M_PI);
}

void SPHERE_Scene1(float tick)
{
    //-----------------------------------------------------------------------------------
    // shash: Framerate fix
    static float last_tick = tick;
    float tick_delta = tick - last_tick;
    last_tick = tick;
    //-----------------------------------------------------------------------------------

TMatrix cam;

  MISC_MemSet4 (GBL_Bits,0,GBL_Size);
  VIEW3D_ReInit (SPHERE_view3D,GBL_Width,GBL_Height,GBL_Width/2,GBL_Height/2,10,10000,80.0f+SPHERE_fov,1);
  VIEW3D_Set (SPHERE_view3D);
  MTX_Rotate (cam,tick*400,tick*600,tick*800);
  for (DWORD i=0; i<SPHERE_MAX_POINTS; i++)
  {
    TVertex vr,vp;
    MTX_Mul (vr,SPHERE_Points[i],cam);
    if (vr.z>0)
    {
      VIEW3D_Project (vp,vr);    
      float px = vp.x+100.0f*sin(SPHERE_Angles[i]*M_PIx2/360);
      float py = vp.y+60.0f*cos(SPHERE_Angles[i]*M_PIx2/360);
      SPHERE_Angles[i]+=SPHERE_Rot[i] * tick_delta*1000.f;
      PutPartRGB (GBL_Bits,(DWORD *)SPHERE_flares->bits,px,py,(long)(SPHERE_flaregrow+rand()%10+(32.0f*(1.0f-vp.z))));
    }
  }  
  VIEW3D_Set (View3D);
}

void SPHERE_Guio()
{
  GUIO_AddFN (SPHERE_Scene1,"SPHERE_Scene1");
  GUIO_AddFN (SPHERE_Layer1,"SPHERE_Layer1");
  GUIO_AddFN (SPHERE_Layer2,"SPHERE_Layer2");
  GUIO_AddFN (SPHERE_Layer3,"SPHERE_Layer3");
}

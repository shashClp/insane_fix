#include <awl.h>
#include <awm.h>
#include <aw3d.h>
#include <object3dpaint.h>
#include <texture.h>
#include <default.h>
#include <matrix.h>
#include <math.h>
#include "fades.h"
#include "efx2d.h"

#define SURFACE_DETAIL 20
#define CURVE_DETAIL 20

TObject *KLZ_tmp;
TMatrix KLAUZ_cam;
TMatrix KLAUZ_fin;
TTexture *BEZIER_environment;
TTexture *BEZIER_environment2;
TBezierSurface s1;
TBezierCurve c1,c2,c3,c4,c5;	
TBezierSurface s2;
TBezierCurve c21,c22,c23,c24,c25;	
TImage *BEZIER_fons;
TView3D *BEZIER_View3D;

bool KLAUZ_Init()
{
	SEC_START("Initializing KLAUZ!");

  OBJ3DPAINT_BZO = BZO;
  OBJ3DPAINT_TC = TC;

	BEZIER_Init();
	
	BEZIER_CurveInit(c1,4,BEZIER_NORMAL);
	BEZIER_CurveAddPoint(c1,-1050.0f,1050.0f,30.0f);
	BEZIER_CurveAddPoint(c1,-1050.0f,200.0f,30.0f);
	BEZIER_CurveAddPoint(c1,-1050.0f,-200.0f,30.0f);
	BEZIER_CurveAddPoint(c1,-1050.0f,-1050.0f,30.0f);

	BEZIER_CurveInit(c2,4,BEZIER_NORMAL);
	BEZIER_CurveAddPoint(c2,-250.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c2,-250.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c2,-250.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c2,-250.0f,-1050.0f,-10.0f);

	BEZIER_CurveInit(c3,4,BEZIER_NORMAL);
	BEZIER_CurveAddPoint(c3,0.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c3,0.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c3,0.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c3,0.0f,-1050.0f,-10.0f);

	BEZIER_CurveInit(c4,4,BEZIER_NORMAL);
	BEZIER_CurveAddPoint(c4,250.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c4,250.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c4,250.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c4,250.0f,-1050.0f,-10.0f);

	BEZIER_CurveInit(c5,4,BEZIER_NORMAL);
	BEZIER_CurveAddPoint(c5,1050.0f,1050.0f,30.0f);
	BEZIER_CurveAddPoint(c5,1050.0f,200.0f,30.0f);
	BEZIER_CurveAddPoint(c5,1050.0f,-200.0f,30.0f);
	BEZIER_CurveAddPoint(c5,1050.0f,-1050.0f,30.0f);

	BEZIER_SurfaceInit(s1,5);
	BEZIER_SurfaceAddCurve(s1,c1);
	BEZIER_SurfaceAddCurve(s1,c2);
	BEZIER_SurfaceAddCurve(s1,c3);
	BEZIER_SurfaceAddCurve(s1,c4);
	BEZIER_SurfaceAddCurve(s1,c5);

	if (!BEZIER_SurfaceInit3DObject(s1, SURFACE_DETAIL, CURVE_DETAIL, "data\\blob4.tga")) return false;	

	
	BEZIER_CurveInit(c21,6,BEZIER_NORMAL);
  BEZIER_CurveAddPoint(c21,-1050.0f,30.0f,-1000.0f);
	BEZIER_CurveAddPoint(c21,-1050.0f,1050.0f,30.0f);
	BEZIER_CurveAddPoint(c21,-1050.0f,200.0f,30.0f);
	BEZIER_CurveAddPoint(c21,-1050.0f,-200.0f,30.0f);
	BEZIER_CurveAddPoint(c21,-1050.0f,-1050.0f,30.0f);
  BEZIER_CurveAddPoint(c21,-1050.0f,30.0f,-1000.0f);

	BEZIER_CurveInit(c22,6,BEZIER_NORMAL);
  BEZIER_CurveAddPoint(c22,-1050.0f,30.0f,-1000.0f);
	BEZIER_CurveAddPoint(c22,-250.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c22,-250.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c22,-250.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c22,-250.0f,-1050.0f,-10.0f);
  BEZIER_CurveAddPoint(c22,-1050.0f,30.0f,-1000.0f);

	BEZIER_CurveInit(c23,6,BEZIER_NORMAL);
  BEZIER_CurveAddPoint(c23,-1050.0f,30.0f,-1000.0f);
	BEZIER_CurveAddPoint(c23,0.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c23,0.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c23,0.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c23,0.0f,-1050.0f,-10.0f);
  BEZIER_CurveAddPoint(c23,-1050.0f,30.0f,-1000.0f);

	BEZIER_CurveInit(c24,6,BEZIER_NORMAL);
  BEZIER_CurveAddPoint(c24,-1050.0f,30.0f,-1000.0f);
	BEZIER_CurveAddPoint(c24,250.0f,1050.0f,-10.0f);
	BEZIER_CurveAddPoint(c24,250.0f,200.0f,-10.0f);
	BEZIER_CurveAddPoint(c24,250.0f,-200.0f,-10.0f);
	BEZIER_CurveAddPoint(c24,250.0f,-1050.0f,-10.0f);
  BEZIER_CurveAddPoint(c24,-1050.0f,30.0f,-1000.0f);

	BEZIER_CurveInit(c25,6,BEZIER_NORMAL);
  BEZIER_CurveAddPoint(c25,-1050.0f,30.0f,-1000.0f);
	BEZIER_CurveAddPoint(c25,1050.0f,1050.0f,30.0f);
	BEZIER_CurveAddPoint(c25,1050.0f,200.0f,30.0f);
	BEZIER_CurveAddPoint(c25,1050.0f,-200.0f,30.0f);
	BEZIER_CurveAddPoint(c25,1050.0f,-1050.0f,30.0f);
  BEZIER_CurveAddPoint(c25,-1050.0f,30.0f,-1000.0f);

	BEZIER_SurfaceInit(s2,5);
	BEZIER_SurfaceAddCurve(s2,c21);
	BEZIER_SurfaceAddCurve(s2,c22);
	BEZIER_SurfaceAddCurve(s2,c23);
	BEZIER_SurfaceAddCurve(s2,c24);
	BEZIER_SurfaceAddCurve(s2,c25);

	if (!BEZIER_SurfaceInit3DObject(s2, SURFACE_DETAIL, CURVE_DETAIL, "data\\text5.tga")) return false;	

  BEZIER_environment = TXT_AddTexture ("data\\bloblgt2.tga");
  BEZIER_environment2 = TXT_AddTexture ("data\\bloblgt1.tga");

  BEZIER_fons = IMAGE_Load ("data\\abel.tga",32);
  IMAGE_FlipV (BEZIER_fons);

  BEZIER_View3D = VIEW3D_Init (GBL_Width-2,GBL_Height,GBL_Width/2,GBL_Height/2,10,10000,50,1);

	LOG("KLAUZ initialized!");

	SEC_END();

	return true;
}

void KLAUZ_Play(float fTick)
{
  MISC_MemSet4 (GBL_Bits,0,GBL_Size);

	float a = fTick*35.0f;
	float b = fTick*26.0f;
	float c = fTick*24.0f;
	float d = fTick*30.0f;

	TVertex v;
	BEZIER_SurfaceGetPoint(v,s1,1,1);
	v.x=-350.0f*(float)cos(d);
	v.z=2750.0f*(float)sin(a);
	BEZIER_SurfaceSetPoint(s1,1,1,v);
	BEZIER_SurfaceGetPoint(v,s1,1,2);
	v.x=650.0f*(float)cos(c);
	v.y=250.0f*(float)sin(a);
	v.z=-2000.0f*(float)sin(b);
	BEZIER_SurfaceSetPoint(s1,1,2,v);

	BEZIER_SurfaceGetPoint(v,s1,2,1);
	v.x=750.0f*(float)sin(d);
	v.y=-250.0f*(float)sin(b);
	v.z=4610.0f*(float)cos(c);
	BEZIER_SurfaceSetPoint(s1,2,1,v);
	BEZIER_SurfaceGetPoint(v,s1,2,2);
	v.y=450.0f*(float)cos(c);
	v.z=-3110.0f*(float)sin(d);
	BEZIER_SurfaceSetPoint(s1,2,2,v);

	BEZIER_SurfaceGetPoint(v,s1,3,1);
	v.x=350.0f*(float)sin(a);
	v.z=-4750.0f*(float)sin(c);
	BEZIER_SurfaceSetPoint(s1,3,1,v);
	BEZIER_SurfaceGetPoint(v,s1,3,2);
	v.x=-250.0f*(float)cos(a);
	v.z=2750.0f*(float)sin(b);
	BEZIER_SurfaceSetPoint(s1,3,2,v);

	MTX_LookAt (KLAUZ_cam,0.0,0.0,1500.0,0.0,0.0,0.0,fTick*360);	

  VTX_Set (OBJ3DPAINT_Llum,0.0,0.0,1400.0);
  VTX_Normalize (OBJ3DPAINT_Llum);

  OBJ3DPAINT_BZO->Prepare();
		
  VIEW3D_MakeFrustrum (OBJ3DPAINT_TC->frustrum,KLAUZ_cam);

  OBJ3DPAINT_txt2 = BEZIER_environment2;
	OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Environemnt(KLAUZ_cam,*BEZIER_Surface2Object(s1,SURFACE_DETAIL,CURVE_DETAIL));

  OBJ3DPAINT_BZO->Execute();
}

void KLAUZ_Play2(float fTick)
{
  MISC_MemSet4 (GBL_Bits,0,GBL_Width*20);
  MISC_MemCpy4 (GBL_Bits+GBL_Width*20,((DWORD *)BEZIER_fons->bits)+GBL_Width*20,GBL_Width*200);
  MISC_MemSet4 (GBL_Bits+GBL_Width*220,0,GBL_Width*20);  

	OBJ3DPAINT_color=0;

	float a = fTick*35.0f;
	float b = fTick*26.0f;
	float c = fTick*24.0f;
	float d = fTick*30.0f;

	TVertex v;
	BEZIER_SurfaceGetPoint(v,s2,1,2);
	v.x=-350.0f*(float)cos(d);
	v.z=2750.0f*(float)sin(a);
	BEZIER_SurfaceSetPoint(s2,1,2,v);
	BEZIER_SurfaceGetPoint(v,s2,1,3);
	v.x=650.0f*(float)cos(c);
	v.y=250.0f*(float)sin(a);
	v.z=-2000.0f*(float)sin(b);
	BEZIER_SurfaceSetPoint(s2,1,3,v);

	BEZIER_SurfaceGetPoint(v,s2,2,2);
	v.x=750.0f*(float)sin(d);
	v.y=-250.0f*(float)sin(b);
	v.z=4610.0f*(float)cos(c);
	BEZIER_SurfaceSetPoint(s2,2,2,v);
	BEZIER_SurfaceGetPoint(v,s2,2,3);
	v.y=450.0f*(float)cos(c);
	v.z=-3110.0f*(float)sin(d);
	BEZIER_SurfaceSetPoint(s2,2,3,v);

	BEZIER_SurfaceGetPoint(v,s2,3,2);
	v.x=350.0f*(float)sin(a);
	v.z=-4750.0f*(float)sin(c);
	BEZIER_SurfaceSetPoint(s2,3,2,v);
	BEZIER_SurfaceGetPoint(v,s2,3,3);
	v.x=-250.0f*(float)cos(a);
	v.z=2750.0f*(float)sin(b);
	BEZIER_SurfaceSetPoint(s2,3,3,v);

  TMatrix obj;
  VTX_Set (OBJ3DPAINT_Llum,0.0,0.0,1400.0);
  VTX_Normalize (OBJ3DPAINT_Llum);
  KLZ_tmp = BEZIER_Surface2Object(s2,SURFACE_DETAIL,CURVE_DETAIL);

	MTX_LookAt (KLAUZ_cam,1000.0,1000.0,1000.0,200.0,100.0,50.0,fTick*720);
  MTX_Rotate (obj,0,0,0);
  MTX_Mul (KLAUZ_fin,KLAUZ_cam,obj);

  VIEW3D_MakeFrustrum (OBJ3DPAINT_TC->frustrum,KLAUZ_fin);
	OBJ3DPAINT_Rep_Flat50(KLAUZ_fin,*KLZ_tmp);

	MTX_LookAt (KLAUZ_cam,1000.0,1000.0,1000.0,0.0,0.0,0.0,fTick*720);
  MTX_Rotate (obj,0,0,0);
  MTX_Mul (KLAUZ_fin,KLAUZ_cam,obj);
}

void KLAUZ_Play3(float fTick)
{
  VIEW3D_Set (BEZIER_View3D);

  VIEW3D_MakeFrustrum (OBJ3DPAINT_TC->frustrum,KLAUZ_fin);
  OBJ3DPAINT_BZO->Prepare();		
  OBJ3DPAINT_txt2 = BEZIER_environment;
	OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Environemnt(KLAUZ_fin,*KLZ_tmp);
  OBJ3DPAINT_BZO->Execute();

  VIEW3D_Set (View3D);
}

void KLAUZ_Play4(float fTick)
{
  VIEW3D_Set (BEZIER_View3D);

  VIEW3D_MakeFrustrum (OBJ3DPAINT_TC->frustrum,KLAUZ_fin);

  RGB_Set (OBJ3DPAINT_color,255,255,255);  
  OBJ3DPAINT_Rep_Flat_Border_Light (KLAUZ_fin,*KLZ_tmp);

  VIEW3D_Set (View3D);
}

void KLAUZ_Close()
{ 
	BEZIER_SurfaceClose(s1);
	BEZIER_Close();

  if (BEZIER_environment)
  {
    TXT_DelTexture (BEZIER_environment);
    BEZIER_environment = NULL;
  }

	LOG("KLAUZ closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//	void FadeAInverso(DWORD* dst,DWORD* src,float pos)
//	pos = [0..1]; 0 = sin efecto; 1 = Inverso

void FadeAInversoMMX(DWORD *src,float pos)
{
	if(pos>1) pos = 1;
	if(pos<0) pos = 0;
	long iPos = pos*255.0f;

  DWORD *d=src;
  for (DWORD i=0; i<GBL_Size; i++)
  {
    DWORD col = d[i];
    DWORD r = Get_R (col);
    DWORD g = Get_G (col);
    DWORD b = Get_B (col);
//    r = r+(((255-r)-g)*iPos)/256;
//    g = g+(((255-g)-b)*iPos)/256;
//    b = b+(((255-b)-r)*iPos)/256;
    r = r+(((255-r)-r)*iPos)/256;
    g = g+(((255-g)-g)*iPos)/256;
    b = b+(((255-b)-b)*iPos)/256;
    RGB_Set (d[i],g,b,r);
  }
  /*
  static _int64 factor = (((iPos<<8)+iPos)<<8)+iPos;
  static _int64 negatiu = 0x00ff00ff00ff00ff;

	_asm //fade xor
	{
		pushad
		mov esi,[GBL_Size]
		mov edi,[src]
    pxor mm0,mm0
    movd mm3,factor
    punpcklbw mm3,mm0
	 bucAB:
		movd mm1,[edi]
    punpcklbw mm1,mm0
    movq mm2,negatiu
    psubw mm2,mm1

//a+((b-a)*d)>>8

    psubw mm2,mm1
    pmullw mm2,mm3
    psrlw mm2,8
    paddw mm2,mm1
    packuswb mm2,mm0

    movd [edi],mm2
		add edi,4
		dec esi
		jnz bucAB
    emms
		popad
	}
  */
}


void KLAUZ_Layer1(float tick)
{
  FadeAInversoMMX (GBL_Bits,sin(tick*M_PI));
}

void KLAUZ_Guio()
{
  GUIO_AddFN(KLAUZ_Play,"KLAUZ_Play");
  GUIO_AddFN(KLAUZ_Play2,"KLAUZ_Play2");
  GUIO_AddFN(KLAUZ_Play3,"KLAUZ_Play3");
	GUIO_AddFN(KLAUZ_Play4,"KLAUZ_Play4");
  GUIO_AddFN(KLAUZ_Layer1,"KLAUZ_Layer1");
}



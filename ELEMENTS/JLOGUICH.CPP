#include <guio.h>
#include <log.h>
#include <misc.h>
#include <view.h>
#include <mem.h>
#include <efx2d.h>
#define ComplexNoise

#define Value_0         0.2
#define Value_1         0.05
#include <image.h>
#include "..\syswin\srt2\includes\srt301.h"
#include "..\syswin\srt2\code\refects\inter.h"
#define NamberSferes 15

ProceduralTexture ProvaR = {
    2,5,
    {
     {0    ,0,0,0,0.7},
     {0.25  ,1,0,1,0},
     {0.5  ,1,1,1,0},
     {0.75  ,1,1,0,0},
     {1    ,0,0,0,0.7}
    },
    0.7,
      {0.5,1.3,6,
      {1.5,1.60,1.82}

    }
  };

 ProceduralTexture Marbre1 = {
    2,3,
    {
     {0    ,1,0.5,0.5,0},
     {0.5  ,1,1,1,0},
     {1    ,1,0.5,0.5,0}
    },
    1,
      {0.5,1.3,5,
      {1.5,1.60,1.82}
    }
  };

	ProceduralTexture Prova3 = {
		2,3,
		{
		{0.0     ,0.4,0.1,0.1,0.0},
		{0.5     ,0.8,0.8,0.8,1.0},
		{1.0     ,0.4,0.1,0.1,0.0}
		},

		0.5,
		{0.5,1.3,2,
			{1.25,1.20,1.82}
		}
	};

	ProceduralTexture Prova13 = {
		2,3,
		{
		{0.0     ,0.9,0.1,0.1,0.0},
		{0.5     ,0.8,0.8,0.8,1.0},
		{1.0     ,0.9,0.1,0.1,0.0}
		},

		0.5,
		{0.5,1.3,3,
			{1.25,1.20,1.82}
		}
	};

	ProceduralTexture Prova14 = {
		2,3,
		{
			{0.0     ,0.3,0.2,0.8,0.0},
			{0.5     ,0.45,0.56,0.23,1.0},
			{1.0     ,0.3,0.2,0.8,0.0}
		},

		1.6,
		{0.5,1.5,2,
			{1.25,1.20,1.82}
		}
	};
void NardenNar(){};

typedef struct {

  TObject_R *Sphere[NamberSferes];
  TObject_R *Ell1,*Ell2,*Ell3,*Ell4;
  TObject_R *Plane1;
  TObject_R *Plane2;
  TWorld_R  *World1;
  TLightPoint_R *Llum1;
  TCamera_R   *Camera1;
	float ValorChorraQueNecesito;
	rgbcol *PeazoBuffer;
	float V0;
}ECPMC;

ECPMC I_ECPMC;
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void TEllipse2_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv);
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

bool ECPMC_Init1 ()
{

  RAY_EXIT = &NardenNar;
	I_ECPMC.World1  = CreateWorldDefault();
  Ray_Initialize_World ( I_ECPMC.World1 );
  Ray_Active_World ( I_ECPMC.World1 );

  I_ECPMC.Camera1 = CreateCameraDefault();
	TVertex P,L,N;

  RVTX_Set (P,0,-3,-5);
  RVTX_Set (L,0,0,0);
  RVTX_Set (N,0,1,0);
  Ray_Active_Camera ( I_ECPMC.Camera1 );
  TCamera_R_Init (P,L,N,256,70);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  I_ECPMC.Llum1   = CreateLightDefault();
  Ray_Active_Light ( Ray_ADD_Light  ( I_ECPMC.Llum1 ) );
  TLightPoint_R_Init ( -20 ,27 ,-20);
  
  
  I_ECPMC.Ell1 = CreateEllipseDefault ();
  Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Ell1));
  TEllipse_R_Init  ( 0,0,0,2,6,2,1,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Set_Phong ( 1, 80 );
//  Ray_Enable (R_TRANSPARENT);

  I_ECPMC.Ell2 = CreateEllipseDefault ();
  Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Ell2));
  TEllipse_R_Init  ( 0,0,0,6,2,2,1,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Set_Phong ( 1, 80 );
  //Ray_Enable (R_TRANSPARENT);

  I_ECPMC.Ell3 = CreateEllipseDefault ();
  Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Ell3));
  TEllipse_R_Init  ( 0,0,0,2,2,6,1,1);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Set_Phong ( 1, 80 );
  //Ray_Enable (R_TRANSPARENT);

  RW_Enable ( RW_RESULTS ) ;
  RW_Enable ( RW_SHADOWS ) ;
  RW_Enable ( RW_REFLECT ) ;
  RW_Enable ( RW_REFRACT );
  Bounder_Init(R_ScreenX,R_ScreenY,1,1);
  InitTextureTable();

  I_ECPMC.World1->AMBIENT = 120; 

  I_ECPMC.Ell1->Pigment.Texture = &Prova3;
  I_ECPMC.Ell2->Pigment.Texture = &Prova3;
  I_ECPMC.Ell3->Pigment.Texture = &Prova3;
  I_ECPMC.Ell1->GetNormal = &TEllipse2_R_GetNormal;
  I_ECPMC.Ell2->GetNormal = &TEllipse2_R_GetNormal;
  I_ECPMC.Ell3->GetNormal = &TEllipse2_R_GetNormal;

  return true;
}

void ECPMC_Runeando1(float a)
{
	float b;
		MISC_MemSet4 (GBL_Bits,0,GBL_Size);
  	I_ECPMC.Camera1->Position.x = sin (a*R_PIx2*2)*10;
		I_ECPMC.Camera1->Position.z = cos (a*R_PIx2*2)*10;
		I_ECPMC.Camera1->Position.y = 3+cos (a*R_PIx2*2);

		TCamera_R_Actualize (100,100);

		I_ECPMC.Llum1->Position.x = cos (a*R_PIx2*2)*19;
		I_ECPMC.Llum1->Position.z = sin (a*R_PIx2*2)*19;
		I_ECPMC.Llum1->Position.y = 3;

		b = 2.2 + sin (a*R_PIx2*3)*2;	  ((TEllipse_R*)I_ECPMC.Ell1)->A_R = 1.0/(b*b);
		b = 2.2 + cos (a*R_PIx2*3.2)*2;		((TEllipse_R*)I_ECPMC.Ell1)->B_R = 1.0/(b*b);
		b = 2.2 + sin (a*R_PIx2*4.2)*2;		((TEllipse_R*)I_ECPMC.Ell1)->C_R = 1.0/(b*b);

		b = 2.2 + cos (a*R_PIx2*2.3)*2;	  ((TEllipse_R*)I_ECPMC.Ell2)->A_R = 1.0/(b*b);
		b = 2.2 + sin (a*R_PIx2*4.2)*2;		((TEllipse_R*)I_ECPMC.Ell2)->B_R = 1.0/(b*b);
		b = 2.2 + sin (a*R_PIx2*3.4)*2;		((TEllipse_R*)I_ECPMC.Ell2)->C_R = 1.0/(b*b);

		b = 2.2 + cos (a*R_PIx2*2.2)*2;	  ((TEllipse_R*)I_ECPMC.Ell3)->A_R = 1.0/(b*b);
		b = 2.2 + cos (a*R_PIx2*4.5)*2;		((TEllipse_R*)I_ECPMC.Ell3)->B_R = 1.0/(b*b);
		b = 2.2 + sin (a*R_PIx2*4.2)*2;		((TEllipse_R*)I_ECPMC.Ell3)->C_R = 1.0/(b*b);
    Prova3.Turbulence.Factor.x =0.8+sin (a*R_PIx2*2)*0.6;
    Prova3.Turbulence.Factor.y =0.8+cos (a*R_PIx2*4)*0.6;
    Prova3.Turbulence.Factor.z =0.8+sin (a*R_PIx2*5)*0.6;

    TWorld_GetFrame ( I_ECPMC.Camera1,100,100, 1 , 1,1,(rgbcol*)GBL_Bits);

}


void TEllipse2_R_GetNormal ( TVertex  &Pos , TVertex &Norm , TReal &ValNorm , long Inv)
{
  TReal  SN = 0;
  TReal  CN = 0;
  TReal  CN2 = 0;
  /*
  SN	= sin ((Pos.y-ActiveEllipse->General.Position.y)*1.5 * R_PIx2 );
  CN	= cos ((Pos.z-ActiveEllipse->General.Position.z)*1.5 * R_PIx2 );
  CN2	= cos ((Pos.x-ActiveEllipse->General.Position.x)*1.5 * R_PIx2 );
  */
  DefTurb Turb;

  Turb.Factor.x = 1.25;
  Turb.Factor.x = 1.85;
  Turb.Factor.x = 1.55;

  Turb.Lambda  = 0.5;
  Turb.Octaves  = 1.3;
  Turb.Omega  = 2.0;

  Turbulence(Pos,&Turb);

  CN2 = CN = SN = Turbulence(Pos,&Turb)*1.0;
  
  RVTX_Set ( Norm, SN + (Pos.x-ActiveEllipse->General.Position.x),
                  CN + (Pos.y-ActiveEllipse->General.Position.y),
                  SN*CN2 + (Pos.z-ActiveEllipse->General.Position.z));
  RVTX_Normalize ( Norm );
  ValNorm = 0;
  if ( (Inv == 1) || (ActiveEllipse->General.Invers )) {
    Norm.x = -Norm.x;
    Norm.y = -Norm.y;
    Norm.z = -Norm.z;
  }
}



//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//			Bolitas tortilleras girando 
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北


bool ECPMC_Init2 ()
{
  RAY_EXIT = &NardenNar;
	I_ECPMC.World1  = CreateWorldDefault();
  Ray_Initialize_World ( I_ECPMC.World1 );
  Ray_Active_World ( I_ECPMC.World1 );

  I_ECPMC.Camera1 = CreateCameraDefault();
	TVertex P,L,N;

  RVTX_Set (P,0,-3,-5);
  RVTX_Set (L,0,0,0);
  RVTX_Set (N,0,1,0);
  Ray_Active_Camera ( I_ECPMC.Camera1 );
  TCamera_R_Init (P,L,N,256,70);
  TCamera_R_Actualize (R_ScreenX,R_ScreenY);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  I_ECPMC.Llum1   = CreateLightDefault();
  Ray_Active_Light ( Ray_ADD_Light  ( I_ECPMC.Llum1 ) );
  TLightPoint_R_Init ( -20 ,27 ,-20);
  
  for (long i = 0; i <NamberSferes ; i++)
	{
		I_ECPMC.Sphere[i] = CreateSphereDefault ();
		Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Sphere[i]));
		TSphere_R_Init  ( sin(i*2*3.1415/(float)NamberSferes)*5,cos(i*2*3.1415/(float)NamberSferes)*5,cos(i*8*3.1415/(float)NamberSferes)*3,1,0,0);
		Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
		Ray_Set_Phong ( 1, 80 );
	}

  RW_Enable ( RW_RESULTS ) ;
  RW_Enable ( RW_SHADOWS ) ;
  RW_Enable ( RW_REFLECT ) ;
  RW_Enable ( RW_REFRACT );
  Bounder_Init(R_ScreenX,R_ScreenY,1,1);
  InitTextureTable();

  I_ECPMC.World1->AMBIENT = 120; 

  I_ECPMC.Camera1->Position.x = sin (Value_0*R_PIx2)*15;
	I_ECPMC.Camera1->Position.z = 500.0;
	I_ECPMC.Camera1->Position.y = 10+cos (Value_0*R_PIx2)*10;
	I_ECPMC.ValorChorraQueNecesito = (500.0 - cos (Value_0*R_PIx2)*15)/Value_0;

	return true;
}


void ECPMC_Runeando2(float a)
{

	float b;
	if (a<=Value_0)
	{
		I_ECPMC.Camera1->Position.z = 500.0-I_ECPMC.ValorChorraQueNecesito*a;
	} else
		{
			I_ECPMC.Camera1->Position.x = sin (a*R_PIx2)*15;
			I_ECPMC.Camera1->Position.z = cos (a*R_PIx2)*15;
			I_ECPMC.Camera1->Position.y = 10+cos (a*R_PIx2)*10;
		}
	TCamera_R_Actualize (R_ScreenX,R_ScreenY);
	I_ECPMC.Llum1->Position.x = cos (a*R_PIx2/120)*19;
	I_ECPMC.Llum1->Position.z = sin (a*R_PIx2/120)*19;
	I_ECPMC.Llum1->Position.y = 3;

	TWorld_GetFrame ( I_ECPMC.Camera1,R_ScreenX,R_ScreenY, 1 , 2,2,(rgbcol*)(GBL_Bits));
	R_RAY_DIM	= 2;
	Interpola (GBL_Bits,GBL_Bits);

}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
bool ECPMC_Init4 ()
{

  RAY_EXIT = &NardenNar;
	I_ECPMC.World1  = CreateWorldDefault();
  Ray_Initialize_World ( I_ECPMC.World1 );
  Ray_Active_World ( I_ECPMC.World1 );

  I_ECPMC.Camera1 = CreateCameraDefault();
	TVertex P,L,N;

  RVTX_Set (P,0,-3,-5);
  RVTX_Set (L,0,0,0);
  RVTX_Set (N,0,1,0);
  Ray_Active_Camera ( I_ECPMC.Camera1 );
  TCamera_R_Init (P,L,N,256,70);
  TCamera_R_Actualize (R_ScreenX,R_ScreenY);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  I_ECPMC.Llum1   = CreateLightDefault();
  Ray_Active_Light ( Ray_ADD_Light  ( I_ECPMC.Llum1 ) );
  TLightPoint_R_Init ( 0 ,0 ,0);
  
  I_ECPMC.Ell1 = CreateEllipseDefault ();
  Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Ell1));
  TEllipse_R_Init  ( 0,0,0,20,60,20,1,1);
	Ray_Set_TextScale ( 20 );
  Ray_Enable (R_NO_LIGHT);

  Bounder_Init(R_ScreenX,R_ScreenY,1,1);
  InitTextureTable();

  I_ECPMC.World1->AMBIENT = 10; 

  //I_ECPMC.Plane1->Pigment.Texture = &Prova13;
  //I_ECPMC.Plane2->Pigment.Texture = &Prova14;
	I_ECPMC.Ell1->Pigment.Texture = &Prova14;
	I_ECPMC.V0 = 1;
  return true;
}

void ECPMC_Runeando4(float a)
{
		float b;
  	I_ECPMC.Camera1->Position.x = sin (a*R_PIx2*6*I_ECPMC.V0)*0.2;
		I_ECPMC.Camera1->Position.z = cos (a*R_PIx2*4*I_ECPMC.V0)*0.6;
		I_ECPMC.Camera1->Position.y = cos (a*R_PIx2*5*I_ECPMC.V0)*0.5;

		TCamera_R_Actualize (R_ScreenX,R_ScreenY);

		b = 14.2 + sin (a*R_PIx2*3)*6;	  ((TEllipse_R*)I_ECPMC.Ell1)->A_R = 1.0/(b*b);
		b = 14.2 + cos (a*R_PIx2*3.2)*6;		((TEllipse_R*)I_ECPMC.Ell1)->B_R = 1.0/(b*b);
		b = 13.2 + sin (a*R_PIx2*4.2)*6;		((TEllipse_R*)I_ECPMC.Ell1)->C_R = 1.0/(b*b);

    Prova14.Turbulence.Factor.x =2.8+cos (a*R_PIx2*4.4)*1.2;
    Prova14.Turbulence.Factor.y =2.8+cos (a*R_PIx2*6.)*1.2;
    Prova14.Turbulence.Factor.z =2.8+sin (a*R_PIx2*3.4)*1.2;
		R_RAY_DIM	= 4;
		MISC_MemSet4 (GBL_Bits,0,GBL_Size);
		FallasValue = a*40;
		FallasValue  = FallasValue % 3;
    TWorld_GetFrame ( I_ECPMC.Camera1,R_ScreenX-3,R_ScreenY-20, 1 , R_RAY_DIM,R_RAY_DIM,(rgbcol*)GBL_Bits);
		Interpola(GBL_Bits,GBL_Bits);

		_asm{
			mov	edi,[GBL_Bits]
			add	edi,(320*21*4)-12
			xor	eax,eax
			//dec	eax //para debug
			mov	edx,200
			buc:
			 stosd
			 stosd
			 stosd
			 add	edi,(320-3)*4
			 dec	edx
			jnz	buc
		}
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
bool ECPMC_Init5 ()
{

  RAY_EXIT = &NardenNar;
	I_ECPMC.World1  = CreateWorldDefault();
  Ray_Initialize_World ( I_ECPMC.World1 );
  Ray_Active_World ( I_ECPMC.World1 );

  I_ECPMC.Camera1 = CreateCameraDefault();
	TVertex P,L,N;

  RVTX_Set (P,0,-3,-5);
  RVTX_Set (L,0,0,0);
  RVTX_Set (N,0,1,0);
  Ray_Active_Camera ( I_ECPMC.Camera1 );
  TCamera_R_Init (P,L,N,256,70);
  TCamera_R_Actualize (GBL_Width,GBL_Height);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
  I_ECPMC.Llum1   = CreateLightDefault();
  Ray_Active_Light ( Ray_ADD_Light  ( I_ECPMC.Llum1 ) );
  TLightPoint_R_Init ( -1 ,1 ,-1);
  
	I_ECPMC.Plane2 = CreatePlaneDefault ();
  Ray_Active_Object ( Ray_ADD_Object (I_ECPMC.Plane2));
  TPlane_R_Init  ( 0,2,0,0,1,0,0,0);
  Ray_Set_Color (rand()%255,rand()%255,rand()%255 );
  Ray_Enable (R_NO_LIGHT);
  Ray_Set_TextScale ( 10 );

  Bounder_Init(GBL_Width,GBL_Height,1,1);
  InitTextureTable();

  I_ECPMC.World1->AMBIENT = 120; 

  I_ECPMC.Plane2->Pigment.Texture = &Prova13;
	I_ECPMC.V0 = 1;
  return true;
}

void ECPMC_Runeando5(float a)
{
		float b;
  	I_ECPMC.Camera1->Position.x = sin (a*R_PIx2*6)*2;
		I_ECPMC.Camera1->Position.z = cos (a*R_PIx2*4)*2;
		I_ECPMC.Camera1->Position.y = 4+cos (a*R_PIx2*5);

		TCamera_R_Actualize (GBL_Width,GBL_Height);
 
    Prova13.Turbulence.Factor.x =1.8+cos (a*R_PIx2*4.4)*1.2;
    Prova13.Turbulence.Factor.y =1.8+cos (a*R_PIx2*6.)*1.2;
    Prova13.Turbulence.Factor.z =1.8+sin (a*R_PIx2*3.4)*1.2;
		R_RAY_DIM	= 8;

    TWorld_GetFrame ( I_ECPMC.Camera1,GBL_Width,GBL_Height, 1 , R_RAY_DIM,R_RAY_DIM,(rgbcol*)GBL_Bits);
		Interpola (GBL_Bits,GBL_Bits);

}


bool ECPMC_Init(){ return true;}
void ECPMC_Prepare1 (float Tick)
{
		ECPMC_Init1 ();
}
void ECPMC_Prepare2 (float Tick)
{
		ECPMC_Init2 ();
}
void ECPMC_Prepare4 (float Tick)
{
		ECPMC_Init4 ();
}
void ECPMC_Prepare5 (float Tick)
{
		ECPMC_Init5 ();
}

void ECPMC_VTo3 (float Tick)
{
		I_ECPMC.V0 = 3;
}

void ECPMC_VTo1 (float Tick)
{
		I_ECPMC.V0 = 1;
}


void ECPMC_Guio()
{

	GUIO_AddFN (ECPMC_Runeando1,"ECPMC_Runeando1");
	GUIO_AddFN (ECPMC_Runeando2,"ECPMC_Runeando2");
	GUIO_AddFN (ECPMC_Runeando4,"ECPMC_Runeando4");
	GUIO_AddFN (ECPMC_Runeando5,"ECPMC_Runeando5");

	GUIO_AddFN (ECPMC_Prepare1,"ECPMC_Prepare1");
	GUIO_AddFN (ECPMC_Prepare2,"ECPMC_Prepare2");
	GUIO_AddFN (ECPMC_Prepare4,"ECPMC_Prepare4");
	GUIO_AddFN (ECPMC_Prepare5,"ECPMC_Prepare5");

	GUIO_AddFN (ECPMC_VTo3,"ECPMC_VTo3");
	GUIO_AddFN (ECPMC_VTo1,"ECPMC_VTo1");

}

void ECPMC_Close(){;}

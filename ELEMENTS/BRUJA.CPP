#include <guio.h>
#include <log.h>
#include <misc.h>
#include "3d.h"
#include "cpart.h"
#include "blurs.h"

const long MAX_BRUJAPART = 100;
CPartSys1 BRUJA_ps1;
CPartSys1 BRUJA_ps2;

TImage *img32Bruja;
TImage *img32TextRtz;
float BRUJA_dist;


void BRUJA_Action1(float iTick)
{
	memset(GBL_Bits,0,GBL_Size);
}

//////////////////////////////////////////////////////////////////////////////
//
// BRUJA 1
// -2 systemas de particulas + fondo

void BRUJA_Layer1(float iTick)
{	
	BRUJA_dist = 127.0f*sin(iTick*M_PI);
	HDist32(GBL_Bits,20,220,127-BRUJA_dist);
}

void BRUJA_Layer2(float iTick)
{	
	BRUJA_dist = 127.0f*sin(iTick*M_PI);
	HDist32(GBL_Bits,20,220,BRUJA_dist);
}

void BRUJA_IN(float iTick)
{
	//Fondo
	MISC_MemCpy4(GBL_Bits,img32Bruja->bits,GBL_Size);
	
	float fFact = 100.0 - iTick*100.0f;

	BLURS_GenerarTunnelRadialBlur (160.0,100.0,fFact);
	BLURS_RenderRadialMIX(GBL_Bits,160.0,100.0,00);	
	
	Margen320x240();		
}

//////////////////////////////////////////////////////////////////////////////
//
// BRUJA 2

void BRUJA_PS(float iTick)
{
	//Fondo
	MISC_MemCpy4(GBL_Bits,img32Bruja->bits,GBL_Size);

	float fDist1 = iTick*1.3f;
	if(fDist1>1.0)
		fDist1 = 1.0;
	float fDist2 = 1.0 - fDist1;

	CMatrix3x3 mRot(true);

	CVector3 vPos(-150.0,0.0,fDist1*8.0f);
	BRUJA_ps2.MueveBruja(0.01f-(iTick*0.01f));
	BRUJA_ps2.Posiciona(CVector3(0,0,0),mRot,vPos);
	BRUJA_ps2.RenderPartAdnRGB(GBL_Bits);

	vPos.set(135.0,0.0,fDist2*8.0f);
	BRUJA_ps2.MueveBruja(iTick*0.01f);
	BRUJA_ps2.Posiciona(CVector3(0,0,0),mRot,vPos);
	BRUJA_ps2.RenderPartAdnRGB(GBL_Bits);

	Margen320x240();		

	float fFact = iTick*400.0f;
	int iFact = -(int)fFact;
	if(iFact>-127)
		HDist32(GBL_Bits,20,220,iFact);
}


//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool BRUJA_Init()
{
  SEC_START("Initializing BRUJA!");

	BRUJA_ps1.Init();
	BRUJA_ps1.SetBitmapRGB("DATA\\PARTRA.GIF");
	BRUJA_ps2.Init();
	BRUJA_ps2.SetBitmapRGB("DATA\\PARTR.GIF");
	BRUJA_ps2.SetMaxPart(60);
	
	img32Bruja = IMAGE_Load("data\\bruja.tga",32);
	IMAGE_FlipV (img32Bruja);

	img32TextRtz = IMAGE_Load("data\\txt1.gif",32);  

	BRUJA_dist = 0;

  LOG ("BRUJA initialized!");
	SEC_END();

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void BRUJA_Close()
{
	FreeMem(img32Bruja);

  LOG ("BRUJA closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void BRUJA_Guio()
{
  GUIO_AddFN(BRUJA_IN,"BRUJA_IN");
  GUIO_AddFN(BRUJA_PS,"BRUJA_PS");
  GUIO_AddFN(BRUJA_Action1,"BRUJA_Action1");
	GUIO_AddFN(BRUJA_Layer1,"BRUJA_Layer1");
  GUIO_AddFN(BRUJA_Layer2,"BRUJA_Layer2");
}




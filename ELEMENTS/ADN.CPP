#include <guio.h>
#include <log.h>
#include <misc.h>
#include "3d.h"
#include "blurs.h"
#include "rt.h"
#include "cpart.h"
#include "fades.h"

#define ADN_Desfase 3.14159/2

CPartSys1 ADN_ps1;
CPartSys1 ADN_ps2;
TImage *img32ADNbk;
DWORD *pImgLogo;
DWORD ADN_InitTickCount;

//////////////////////////////////////////////////////////////////////////////
//
// Cimedia says: Creando base ;)
//

void HDIST_Action(float iTick)
{
	HDist32(GBL_Bits,20,220,sin(iTick*fPi)*80.0f);
}

void HDISTOUT_Action(float iTick)
{
	HDist32(GBL_Bits,20,220,80.0f*iTick);
}

void HDISTIOUT_Action(float iTick)
{
	HDist32(GBL_Bits,20,220,-80.0f*iTick);
}

void FADE_ABlanco(float iTick)
{
	FadeABlanco(GBL_Bits,1.0f-iTick);
}
     
void FADE_ABlanco320x240(float iTick)
{
	FadeABlanco320x240(GBL_Bits,1.0f-iTick);
}
     
void FADE_DesdeBlanco320x240(float iTick)
{
	FadeABlanco320x240(GBL_Bits,iTick);
}

void FADE_ANegro(float iTick)
{
	FadeANegro(GBL_Bits,1.0f-iTick);
}

void FADE_AInverso(float iTick)
{
	FadeAInverso(GBL_Bits,1.0f-iTick);
}

void FADE_DesdeBlanco(float iTick)
{
	FadeABlanco(GBL_Bits,iTick);
}

void FADE_DesdeNegro(float iTick)
{
	FadeANegro(GBL_Bits,iTick);
}

void FADE_DesdeInverso(float iTick)
{
	FadeAInverso(GBL_Bits,iTick);
}


//////////////////////////////////////////////////////////////////////////////
//
// ADN In
// -Empieza a aparecer las particulas

void ADN_Action1(float iTick)
{
  MISC_MemCpy4(pImgLogo,GBL_Bits,GBL_Size);
  BLURS_GenerarTunnelRadialBlur (160.0f,120.0f,2);
	ADN_InitTickCount = (DWORD) GetTickCount();
}

void ADN_IN(float iTick)
{
	long iCntPartADN = (long) (iTick*60.0f);

	DWORD uTick = (DWORD) GetTickCount()-ADN_InitTickCount;

	float t = (float)uTick/1000000.0f + 0.3f;
	float a = (float)uTick/1000.0f;
	MISC_MemSet4(GBL_Bits,0,GBL_Size);

	long iFact = 255 - (iTick*300);
	if(iFact<0) iFact=0;

	MorphARGB(GBL_Bits,pImgLogo,GBL_Bits,iFact);
	
	ADN_ps1.MueveADN(t);
	ADN_ps1.RenderPartAdnRGB(GBL_Bits,iCntPartADN);

	Margen320x240();	
}


//////////////////////////////////////////////////////////////////////////////
//
// ADN 2
// -2 systemas de particulas

void ADN_2(float iTick)
{
	MISC_MemSet4 (GBL_Bits,0,GBL_Size);

	DWORD uTick = (DWORD) GetTickCount()-ADN_InitTickCount;

	float t = (float)uTick/1000000.0f + 0.3f;
	float a = (float)uTick/1000.0f;
	ADN_ps1.MueveADN(t);
	ADN_ps1.RenderPartAdnRGB(GBL_Bits);	
	for (int i=0;i<8;i++)
	{
		CMatrix3x3 mRot(true);
		mRot.SetRotacion(0.0f,-0.3f*cos(t*200.0f),0.5f+0.5f*cos(t*200.0f));
		float fSin = (float) sin((a+ADN_Desfase)+i*0.3f)+0.5f;
		CVector3 vPos(0,fSin*350.0f,0);
		ADN_ps2.MueveRdm(t);
		ADN_ps2.Posiciona(CVector3(0,0,0),mRot,vPos);
		ADN_ps2.RenderPartRGB(GBL_Bits);
	}
	Margen320x240();		
}


//////////////////////////////////////////////////////////////////////////////
//
// ADN 2
// -2 systemas de particulas + fondo

void ADN_2F(float iTick)
{
  iTick+=2.0f;

	//Fondo  
	OldFreeEsfera(iTick,fPi/4,fPi/4);
	VuelcaBufInterp8x8F(GBL_Bits+320*20,(DWORD *)img32ADNbk->bits);
	BLURS_RenderRadialADD(GBL_Bits,160,120,110);

	DWORD uTick = (DWORD) GetTickCount()-ADN_InitTickCount;

	float t = (float)uTick/1000000.0f + 0.3f;
	float a = (float)uTick/1000.0f;
	ADN_ps1.MueveADN(t);
	ADN_ps1.RenderPartAdnRGB(GBL_Bits);	
	for(long i=0;i<8;i++)
	{
		CMatrix3x3 mRot(true);
		mRot.SetRotacion(0.0,-0.3*cos(t*200.0),0.5+0.5*cos(t*200.0));
		float fSin = sin((a+ADN_Desfase)+i*0.3)+0.5;
		CVector3 vPos(0,fSin*350,0);
		ADN_ps2.MueveRdm(t);
		ADN_ps2.Posiciona(CVector3(0,0,0),mRot,vPos);
		ADN_ps2.RenderPartRGB(GBL_Bits);
	}
	Margen320x240();		
}

//////////////////////////////////////////////////////////////////////////////
//
// ADN Out
// -1 systema de particulas + fondo con fade hacia la siguiente escena

void ADN_OUT(float iTick)
{
  iTick+=3.0f;

	//Fondo
	OldFreeEsfera(iTick,fPi/2 + iTick,fPi/2 + iTick);
	VuelcaBufInterp8x8F(GBL_Bits+320*20,(DWORD *)img32ADNbk->bits);
	BLURS_RenderRadialADD(GBL_Bits,160,120,110);

	DWORD uTick = (DWORD) GetTickCount()-ADN_InitTickCount;

	float t = (float)uTick/1000000.0f + 0.3f;
	float a = (float)uTick/1000.0f;
	ADN_ps1.MueveADN(t);
	ADN_ps1.RenderPartAdnRGB(GBL_Bits);	

  Margen320x240();		
}


//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool ADN_Init()
{
  SEC_START ("Initializing ADN");  

	ADN_ps1.Init();
	ADN_ps2.Init();

	if (!ADN_ps1.SetBitmapRGB("DATA\\PARTRA.GIF")) return false;
	if (!ADN_ps2.SetBitmapRGB("DATA\\PARTR.GIF")) return false;

	ADN_ps2.SetMaxPart(20);

	img32ADNbk = IMAGE_Load("data\\prova1.tga",32); 
  if (!img32ADNbk) return false;

  pImgLogo = (DWORD *)GetMem(GBL_Size4);

  LOG ("ADN initialized!");
  SEC_END();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void ADN_Close()
{
  LOG ("Closing ADN");

  ADN_ps1.Close();
  ADN_ps2.Close();

  if (pImgLogo)
  {
	  FreeMem(pImgLogo);
    pImgLogo = NULL;
  }
  if (img32ADNbk)
  {
    FreeMem(img32ADNbk);
    img32ADNbk = NULL;
  }

  LOG ("ADN closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void ADN_Guio()
{
  GUIO_AddFN(HDIST_Action,"HDIST_Action");
  GUIO_AddFN(HDISTOUT_Action,"HDISTOUT_Action");
  GUIO_AddFN(HDISTIOUT_Action,"HDISTIOUT_Action");
  GUIO_AddFN(FADE_ABlanco,"FADE_ABlanco");
  GUIO_AddFN(FADE_ANegro,"FADE_ANegro");
  GUIO_AddFN(FADE_AInverso,"FADE_AInverso");
  GUIO_AddFN(FADE_DesdeBlanco,"FADE_DesdeBlanco");
  GUIO_AddFN(FADE_DesdeNegro,"FADE_DesdeNegro");
  GUIO_AddFN(FADE_DesdeInverso,"FADE_DesdeInverso");
  GUIO_AddFN(ADN_IN,"ADN_IN");
  GUIO_AddFN(ADN_2,"ADN_2");
  GUIO_AddFN(ADN_2F,"ADN_2F");
  GUIO_AddFN(ADN_OUT,"ADN_OUT");
  GUIO_AddFN(ADN_Action1,"ADN_Action1");
  GUIO_AddFN(FADE_ABlanco320x240,"FADE_ABlancoAll");
  GUIO_AddFN(FADE_DesdeBlanco320x240,"FADE_DesdeBlancoAll");
}

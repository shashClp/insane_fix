#include <guio.h>
#include <image.h>
#include <log.h>
#include <view.h>
#include <mem.h>
#include <misc.h>
#include "efx2d.h"

TImage *img32CREEDZbk;

TImage *img8Bpm;
TImage *img8Cimedia;
TImage *img8Hlod;
TImage *img8Elric;
TImage *img8Klauz;
TImage *img8Nocturnus;
TImage *img8Anaconda;
TImage *img8Elements;
TImage *img8Tmp;
TImage *img8Tmp2;

TImage *img8BpmPattern;
TImage *img8CimediaCode;
TImage *img8HlodCode;
TImage *img8ElricCode;
TImage *img8KlauzCode;
TImage *img32NoctFace;
TImage *img8MtxCode1;
TImage *img8MtxCode2;
TImage *img8MtxCode3;
TImage *CREEDZ_who;

extern TImage* img32Tmp;
extern TImage* img8GBL_Ruido;
extern SFuente* fntGBL_TypeWriter;

//////////////////////////////////////////////////////////////////////////////
//
//	Mostrar nombre, con ruido

void CREEDZ_ActionBpm(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Bpm;
}

void CREEDZ_ActionCimedia(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Cimedia;
}

void CREEDZ_ActionElric(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Elric;
}

void CREEDZ_ActionHlod(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Hlod;
}

void CREEDZ_ActionKlauz(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Klauz;
}

void CREEDZ_ActionNocturnus(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Nocturnus;
}

void CREEDZ_ActionAnaconda(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Anaconda;
}

void CREEDZ_ActionElements(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	CREEDZ_who=img8Elements;
}

void CREEDZ_Play(float iTick)
{
	MISC_MemSet4(GBL_Bits,0,GBL_Size);
	memcpy(img8Tmp2->bits,CREEDZ_who->bits,GBL_Size);
	char iFDist = -(rand()%127);
	HDist8((char*)img8Tmp2->bits,20,220,iFDist);

	//Fondo Ruido
    long iFactor = (rand() % 35);
    ScrollToScreen8((char*)img8Tmp->bits,img8GBL_Ruido,iFactor,0);
		void* pScr = img8Tmp->bits;
		_asm{
			mov	edi,[pScr]
			add	edi,(320*21)-36
			xor	eax,eax
			//dec	eax //para debug
			mov	edx,200
			buc:
			 stosd
			 stosd
			 stosd
			 stosd
			 stosd
			 stosd
			 stosd
			 stosd
			 stosd
			 add	edi,320-36
			 dec	edx
			jnz	buc
		}
	MergeAlphaADD(img8Tmp->bits,img8Tmp->bits,img8Tmp2->bits,GBL_Size);
	//Fusionar layer alfa con fondo rgb
    FlatternAlpha(GBL_Bits,GBL_Bits,img8Tmp->bits);
	Margen320x240();
}

//////////////////////////////////////////////////////////////////////////////
//
//	Partes

/*void CREEDZ_Cimedia(long iTick)
{
	//Fondo
	OldFreeEsfera((float)iTick/10000.0,fPi/2 + (float)iTick/10000.0,(float)iTick/10000.0);
	VuelcaBufInterp8x8F(GBL_Bits+320*20,(DWORD*)img32CREEDZbk->bits);
	BLUR_RenderRadialInv(GBL_Bits,160,120,10);    
}*/

//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool CREEDZ_Init()
{	
	SEC_START("Initializing CREEDZ!");

  img32CREEDZbk = IMAGE_Load("data\\wrd1.gif",32); 
	if (!img32CREEDZbk) return false;

	img8Tmp = IMAGE_Load("data\\new.gif",8);
	if (!img8Tmp) return false;
	img8Tmp2 = IMAGE_Load("data\\new.gif",8);
	if (!img8Tmp2) return false;

	img8Bpm = IMAGE_Load("data\\bpm.gif",8);
	if (!img8Bpm) return false;
	img8Cimedia = IMAGE_Load("data\\cimedia.gif",8);
	if (!img8Cimedia) return false;
	img8Hlod = IMAGE_Load("data\\hlodwig.gif",8);
	if (!img8Hlod) return false;
	img8Klauz = IMAGE_Load("data\\klauz.gif",8);
	if (!img8Klauz) return false;
	img8Elric = IMAGE_Load("data\\elric.gif",8);
	if (!img8Elric) return false;
	img8Nocturnus = IMAGE_Load("data\\nocturn.gif",8);
	if (!img8Nocturnus) return false;
	img8Anaconda = IMAGE_Load("data\\anaconda.gif",8);
	if (!img8Anaconda) return false;
	img8Elements = IMAGE_Load("data\\elements.gif",8);
	if (!img8Elements) return false;

	img8BpmPattern = IMAGE_Load("data\\bpmTrack.gif",8);
	if (!img8BpmPattern) return false;
	img8CimediaCode = IMAGE_Load("data\\cimecode.gif",8);
	if (!img8CimediaCode) return false;
	img8ElricCode = IMAGE_Load("data\\elricode.gif",8);
	if (!img8ElricCode) return false;
	img8HlodCode = IMAGE_Load("data\\hlodcode.gif",8);
	if (!img8HlodCode) return false;
	img8KlauzCode = IMAGE_Load("data\\klauzcod.gif",8);
	if (!img8KlauzCode) return false;
	img32NoctFace = IMAGE_Load("data\\noctface.gif",32);
	if (!img32NoctFace) return false;

	img8MtxCode1 = IMAGE_Load("data\\matrix1.gif",8);
	if (!img8MtxCode1) return false;
	img8MtxCode2 = IMAGE_Load("data\\matrix2.gif",8);
	if (!img8MtxCode2) return false;
	img8MtxCode3 = IMAGE_Load("data\\matrix3.gif",8);
	if (!img8MtxCode3) return false;

	LOG("CREEDZ initialized!");
	SEC_END();

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void CREEDZ_Close()
{
	FreeMem(img32CREEDZbk);
	FreeMem(img8Tmp);
	FreeMem(img8Tmp2);
	FreeMem(img8Bpm);
	FreeMem(img8Cimedia);
	FreeMem(img8Hlod);
	FreeMem(img8Klauz);
	FreeMem(img8Elric);
	FreeMem(img8Nocturnus);
	FreeMem(img8Anaconda);
	FreeMem(img8Elements);
	FreeMem(img8BpmPattern);
	FreeMem(img8CimediaCode);
	FreeMem(img8HlodCode);
	FreeMem(img8ElricCode);
	FreeMem(img8KlauzCode);
	FreeMem(img32NoctFace);
	FreeMem(img8MtxCode1);
	FreeMem(img8MtxCode2);
	FreeMem(img8MtxCode3);

  LOG ("CREEDZ closed!");
}

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void CREEDZ_Guio()
{
  GUIO_AddFN(CREEDZ_Play,"CREEDZ_Play");
	GUIO_AddFN(CREEDZ_ActionBpm,"CREEDZ_ActionBpm");
	GUIO_AddFN(CREEDZ_ActionCimedia,"CREEDZ_ActionCimedia");
	GUIO_AddFN(CREEDZ_ActionElric,"CREEDZ_ActionElric");
	GUIO_AddFN(CREEDZ_ActionHlod,"CREEDZ_ActionHlod");
	GUIO_AddFN(CREEDZ_ActionKlauz,"CREEDZ_ActionKlauz");
	GUIO_AddFN(CREEDZ_ActionNocturnus,"CREEDZ_ActionNocturnus");
	GUIO_AddFN(CREEDZ_ActionAnaconda,"CREEDZ_ActionAnaconda");
	GUIO_AddFN(CREEDZ_ActionElements,"CREEDZ_ActionElements");
}


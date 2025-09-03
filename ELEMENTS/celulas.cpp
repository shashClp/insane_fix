//////////////////////////////////////////////////////////////////////
//
// Superposion de N patrones de 256x256x8
//
// CiMEDIA [5/99]
//

#include <windows.h>
#include <math.h>
#include <view.h>
#include <log.h>
#include <guio.h>
#include <misc.h>
#include <mem.h>
#include "efx2d.h"

#define pi 3.1415f 

#define MaxCirc 10

int XX[MaxCirc]={0};
int YY[MaxCirc]={0};
int deltaX[MaxCirc]={0};
int deltaY[MaxCirc]={0};

//TImage img32Tmp;

extern unsigned char campana[256][256];
extern long ZMRand();

//-- Funciones aux ---------------------------------------------------

void CircPlasma(DWORD *Dest);
void Ini_CircPlasma();
void Suaviza_Mascara(int iPases);  
void CELULA_Move();


extern TImage* img8Tmp;
extern TImage* img32ADNbk;
char* pBuf8;


extern TImage *img8BpmPattern;
extern TImage *img8CimediaCode;
extern TImage *img8HlodCode;
extern TImage *img8ElricCode;
extern TImage *img8KlauzCode;
extern TImage *img32NoctFace;
extern TImage *img8MtxCode1;
extern TImage *img8MtxCode2;
extern TImage *img8MtxCode3;


TImage* img32Tmp;
TImage* CELULA_who;
int CELULA_color;
float CELULA_RTZI_iFirstTick, CELULA_RTZI_iLastTick;
char* CELULA_RTZI_img;

extern TImage *img8Chip1;
extern TImage *img8Chip1Txt;
extern TImage *img8Chip2;
extern TImage *img8Chip3;
extern TImage *img8Chip4;
extern TImage *img8Chip5;
extern TImage *img8Chip6;
extern TImage *img8Chip7;

//////////////////////////////////////////////////////////////////////
//
// void CELULA_RTZ(long iTick)

void CELULA_RTZ(float iTick)
{
  RotoZoom_Texel(iTick,1.0+sin(iTick)*0.1,
	  160.0+sin(iTick)*20.0,120.0,
	  GBL_Bits,(DWORD*)img32ADNbk->bits);
  CELULA_Move();
  CircPlasma(GBL_Bits);
};

//////////////////////////////////////////////////////////////////////
//
// void CELULA_RTZ(long iTick)

void CELULA_RTZI_Action1(float iTick)
{
	Ini_CircPlasma();
	Suaviza_Mascara(4);

	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.6f;
	CELULA_RTZI_img = (char*)img8Chip1->bits;
}

void CELULA_RTZI_Action2(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip1Txt->bits;
}

void CELULA_RTZI_Action3(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip2->bits;
}

void CELULA_RTZI_Action4(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip3->bits;
}

void CELULA_RTZI_Action5(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip4->bits;
}

void CELULA_RTZI_Action6(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip5->bits;
}

void CELULA_RTZI_Action7(float iTick)
{
	CELULA_RTZI_iFirstTick = 0.4f;
	CELULA_RTZI_iLastTick = 0.7f;
	CELULA_RTZI_img = (char*)img8Chip6->bits;
}

void CELULA_RTZI(float iTick)
{
  CELULA_RTZ(iTick);

	long iFact = 0;

	if(iTick <= CELULA_RTZI_iFirstTick)
		iFact = (iTick/CELULA_RTZI_iFirstTick)*127;

	if(iTick >= CELULA_RTZI_iLastTick)
		iFact = -((1.0f-iTick)/CELULA_RTZI_iLastTick)*127;

	memcpy(img8Tmp->bits,CELULA_RTZI_img,GBL_Size);
	HDist8Des((char*)img8Tmp->bits,20,220,iFact);
	FlatternAlphaInv(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits+322,GBL_Width*(GBL_Height-2));
	FlatternAlpha(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);
}

//////////////////////////////////////////////////////////////////////
//
// void CELULA_Play(long iTick)

void CELULA_ActionBpm(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8BpmPattern;
	CELULA_color=1;
}

void CELULA_ActionCimedia(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8CimediaCode;
	CELULA_color=1;
}

void CELULA_ActionElric(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8ElricCode;
	CELULA_color=0;
}

void CELULA_ActionHlod(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8HlodCode;
	CELULA_color=1;
}

void CELULA_ActionKlauz(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8KlauzCode;
	CELULA_color=0;
}

void CELULA_ActionNocturnus(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img32NoctFace;
	CELULA_color=2;
}

void CELULA_ActionAnaconda(float iTick)
{
	MISC_MemCpy4(img32Tmp->bits,GBL_Bits,GBL_Size);
	CELULA_who = img8MtxCode1;
	CELULA_color=1;
}

void CELULA_Play(float iTick)
{
  CELULA_Move();
  CircPlasma((DWORD*)img32Tmp->bits);

  RotoZoomA(0.0,0.8,155.0,128.0-iTick*80.0,(char *)img8Tmp->bits,(char *)CELULA_who->bits);

  MISC_MemCpy4(GBL_Bits,img32Tmp->bits,GBL_Size);

  switch(CELULA_color)
  {
	case 0: FlatternAlphaInv(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);
			break;
	case 1: FlatternAlphaInv(GBL_Bits,GBL_Bits,((BYTE*)img8Tmp->bits)+321,GBL_Width*(GBL_Height-2));
					FlatternAlphaInvRed(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);
			break;
	case 2: FlatternAlphaInv(GBL_Bits,GBL_Bits,((BYTE*)img8Tmp->bits)+321,GBL_Width*(GBL_Height-2));
					FlatternAlphaInvGreen(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);
			break;
	case 3: FlatternAlphaInvBlue(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);
			break;
  }
};

void CELULA_PlayMtx(float iTick)
{

	CELULA_Move();
  CircPlasma((DWORD*)img32Tmp->bits);
	MISC_MemCpy4(GBL_Bits,img32Tmp->bits,GBL_Size);

	RotoZoomA_Texel(0.0,0.8,155.0,128.0-iTick*(-120.0),(char *)img8Tmp->bits,(char *)CELULA_who->bits);
	MISC_MemCpy4(pBuf8,(char *)img8Tmp->bits,GBL_Size>>2);

	RotoZoomA_Texel(0.0,0.8,155.0,128.0-iTick*(-80.0),(char *)img8Tmp->bits,(char *)img8MtxCode2->bits);
	MergeAlphaADD(pBuf8,pBuf8,(char *)img8Tmp->bits,GBL_Size);

	RotoZoomA_Texel(0.0,0.8,155.0,128.0-iTick*60.0,(char *)img8Tmp->bits,(char *)img8MtxCode3->bits);
	MergeAlphaADD((char *)img8Tmp->bits,(char *)img8Tmp->bits,pBuf8,GBL_Size);
	//FlatternAlphaInv(GBL_Bits,GBL_Bits,((BYTE*)img8Tmp->bits)+321,GBL_Width*(GBL_Height-2));
	FlatternAlphaInv(GBL_Bits,GBL_Bits,(BYTE*)img8Tmp->bits);

	Margen320x240();

}

void CELULA_PlayNoct(float iTick)
{
	MorphARGB(GBL_Bits,(DWORD*)img32Tmp->bits,(DWORD*)img32NoctFace->bits,255-(DWORD)(iTick*255.0f));
}


//////////////////////////////////////////////////////////////////////
//
// void CELULA_Init()

bool CELULA_Init()
{
	SEC_START("Initializing CELULA!");

  Ini_CircPlasma();
  Suaviza_Mascara(4);
  for(int i=0;i<100;i++)
	CELULA_Move();
  
	img32Tmp = IMAGE_Load("data\\new.gif",32);
	if (!img32Tmp) return false;

	pBuf8 = (char*)GetMem(GBL_Size);

	LOG("CELULA initialized!");

	SEC_END();
	return true;
};

//////////////////////////////////////////////////////////////////////
//
// void CELULA_Close()

void CELULA_Close()
{
	FreeMem(img32Tmp);
	FreeMem(pBuf8);

	LOG("CELULA closed!");
};

//////////////////////////////////////////////////////////////////////
//
// void CELULA_Move()

void CELULA_Move()
{
	__asm{
	  pushad
      //--varia centros y deltas X--
      mov  edx,MaxCirc
      IncDeltasX:
       call ZMRand
       and  eax,0x00000707
       sub  al,ah
       movsx eax,al
       mov  ebx,DWORD Ptr deltaX[edx*4-4]
       add  ebx,eax
       cmp  ebx,10
       jng  incX_OK1
        mov ebx,10
       incX_OK1:
       cmp  ebx,-10
       jnl  incX_OK2
        mov ebx,-10
       incX_OK2:
       mov  DWORD Ptr deltaX[edx*4-4],ebx
       add  ebx,DWORD Ptr XX[edx*4-4]
       cmp  ebx,319
       jng  XX_OK1
        mov ebx,319
       XX_OK1:
       cmp  ebx,0
       jnl  XX_OK2
        xor ebx,ebx
       XX_OK2:
       mov  DWORD Ptr XX[edx*4-4],ebx
      dec   edx
      jnz   IncDeltasX

      //--varia centros y deltas Y--
      mov  edx,MaxCirc
      IncDeltasY:
       call ZMRand
       and  eax,0x00000707
       sub  al,ah
       movsx eax,al
       mov  ebx,DWORD Ptr deltaY[edx*4-4]
       add  ebx,eax
       cmp  ebx,10
       jng  incY_OK1
        mov ebx,10
       incY_OK1:
       cmp  ebx,-10
       jnl  incY_OK2
        mov ebx,-10
       incY_OK2:
       mov  DWORD Ptr deltaY[edx*4-4],ebx
       add  ebx,DWORD Ptr YY[edx*4-4]
       cmp  ebx,199
       jng  YY_OK1
        mov ebx,199
       YY_OK1:
       cmp  ebx,0
       jnl  YY_OK2
        xor ebx,ebx
       YY_OK2:
       mov  DWORD Ptr YY[edx*4-4],ebx
      dec   edx
      jnz   IncDeltasY

      popad

	};
};


//////////////////////////////////////////////////////////////////////////////
//
//  Guio

//////////////////////////////////////////////////////////////////////////////
//
//  Guio

void CELULA_Guio()
{
  GUIO_AddFN(CELULA_Play,"CELULA_Play");
	GUIO_AddFN(CELULA_PlayMtx,"CELULA_PlayMtx");
	GUIO_AddFN(CELULA_PlayNoct,"CELULA_PlayNoct");
	GUIO_AddFN(CELULA_ActionBpm,"CELULA_ActionBpm");
	GUIO_AddFN(CELULA_ActionCimedia,"CELULA_ActionCimedia");
	GUIO_AddFN(CELULA_ActionElric,"CELULA_ActionElric");
	GUIO_AddFN(CELULA_ActionHlod,"CELULA_ActionHlod");
	GUIO_AddFN(CELULA_ActionKlauz,"CELULA_ActionKlauz");
	GUIO_AddFN(CELULA_ActionNocturnus,"CELULA_ActionNocturnus");
	GUIO_AddFN(CELULA_ActionAnaconda,"CELULA_ActionAnaconda");
  GUIO_AddFN(CELULA_RTZI,"CELULA_RTZI");
	GUIO_AddFN(CELULA_RTZI_Action1,"CELULA_RTZI_Action1");
	GUIO_AddFN(CELULA_RTZI_Action2,"CELULA_RTZI_Action2");
	GUIO_AddFN(CELULA_RTZI_Action3,"CELULA_RTZI_Action3");
	GUIO_AddFN(CELULA_RTZI_Action4,"CELULA_RTZI_Action4");
	GUIO_AddFN(CELULA_RTZI_Action5,"CELULA_RTZI_Action5");
	GUIO_AddFN(CELULA_RTZI_Action6,"CELULA_RTZI_Action6");
	GUIO_AddFN(CELULA_RTZI_Action7,"CELULA_RTZI_Action7");
}


//////////////////////////////////////////////////////////////////////
//
// Aux Stuff

//-- void CircPlasma(DWORD *Dest) ------------------------------------

void CircPlasma(DWORD *Dest)
{
	__asm{
      pushad
      //--borra el fondo--
      xor   eax,eax
      mov   ecx,320*240
      mov   edi,[Dest]
      //rep   stosd
      mov   edi,[Dest]
	  
      //--superpone MaxCirc circulos--
      mov   edx,MaxCirc-1            //edx:circulos por poner DEBUG!!! -1!!!
     Otro_Circ:
      mov   ebx,DWORD Ptr YY[edx*4-4]
      lea   ebx,[ebx*4+ebx]
      shl   ebx,6
      add   ebx,DWORD Ptr XX[edx*4-4]//ebx:esquina sup-izq. del circulo en Dest
      mov   esi,offset campana       //esi:campana
      xor   ch,ch                    // ch:contador y (256)
      bucY:
       xor  cl,cl                    // cl:contador x (256)
       bucX:
        mov al,[esi]
        inc esi
        add [edi+ebx*4+320*18*4+0],al
		jnc ok1
		mov [edi+ebx*4+320*18*4+0],0x0ff
		ok1:
		shr al,1
		add [edi+ebx*4+320*18*4+2],al
		jnc ok2
		mov [edi+ebx*4+320*18*4+2],0x0ff
		ok2:
		shr al,1
		add [edi+ebx*4+320*18*4+1],al
		jnc ok3
		mov [edi+ebx*4+320*18*4+1],0x0ff
		ok3:
		add bx,1
        dec cl
        jnz bucX
       add  bx,320-256 
       dec  ch
       jnz  bucY
      dec   edx
      jnz   Otro_Circ

	  //--borra linia inf--
      xor   eax,eax
      mov   ecx,256
      mov   edi,[Dest]
	  add   edi,(204+18)*320*4
      rep   stosd
      mov   edi,[Dest]

	  popad	  

	};
};

//-- void Ini_CircPlasma() -------------------------------------------

void Ini_CircPlasma()
{
  float seno[256];

  for(int i=0;i<256;i++)
   seno[i]=(sin(pi*i/256)*130)+128;
  for(i=0;i<256;i++)
   for(int ii=0;ii<256;ii++)
    campana[ii][i]=(int)(-floor(seno[i]*seno[ii]))>>8;

  for(i=0;i<256;i++)
   for(int ii=0;ii<256;ii++)
    if(campana[i][ii]==0)
     campana[i][ii]=255;
	else if (campana[i][ii]<0)
	 campana[i][ii]=0;
    else
	 campana[i][ii]=256 / campana[i][ii];
};

//-- void Suaviza_Mascara(int iPases) --------------------------------

void Suaviza_Mascara(int iPases)
{
  for(int n=1;n<=iPases;n++)
   for(int i=1;i<255;i++)
    for(int ii=1;ii<255;ii++)
     campana[i][ii]=((unsigned char)campana[i-1][ii-1]+
					 (unsigned char)campana[i+1][ii-1]+
					 (unsigned char)campana[i-1][ii+1]+
					 (unsigned char)campana[i+1][ii+1])/3.8;
};


//-- eof -------------------------------------------------------------
/*
----------------------------------------------------------------------
--                                                                  --
--   CVideo class by CiMEDIA                                        --
--                                                                  --
--           a.k.a. Cesar Vellido [3/98]                            --
--           (C port [5/99])                                        --
--           (C++ port [8/99])                                      --

       -->>>   FE[byte]=igual que E pero en 8 bits    <<<--

----------------------------------------------------------------------
*/

#include "cvideo.h"
#include <file.h>
#include <mem.h>
#include <log.h>
#include <misc.h>

//#define NULL 0

  unsigned char CVideo::CODEC[64]= 
    {0, 1, 2, 2, 3, 3, 3, 3,   4, 4, 4, 4, 4, 4, 4, 4,
     5, 5, 5, 5, 5, 5, 5, 5,   6, 6, 6, 6, 6, 6, 6, 6,
     7, 7, 7, 7, 7, 7, 7, 7,   8, 8, 8, 8, 8, 8, 8, 8,
     9, 9, 9, 9, 9, 9, 9, 9,  10,10,10,10,11,11,12,13};

  unsigned char CVideo::DECODEC[16] =
    {0,+1,+2,+4,+8,+16,+24,+32,+40,+48,+56,+60,+62,+63,0 ,0 };

//////////////////////////////////////////////////////////////////////
//
// CVideo();

CVideo::CVideo()
{
    m_bLoaded = false;
    m_iNoiseLevel = 3; //muy poco ruido
    m_iInterlace = 1; //no entrelazado
		m_bRewind = true;

    m_pBuffer = (char *)GetMem(320*250);
    m_pBuffer += 320; //para blur
    m_pBufferRGB = (long *)GetMem(320*240<<2);

    m_pMovie = NULL;
    m_pMoviePtr = NULL;
};

//////////////////////////////////////////////////////////////////////
//
// ~CVideo();

CVideo::~CVideo()
{
  m_pBuffer-=320;
  FreeMem(m_pBuffer);
  FreeMem(m_pBufferRGB);
  if (m_bLoaded)
   FreeMem(m_pMovie);
};

//////////////////////////////////////////////////////////////////////
//
// void Load(char *filename);

bool CVideo::Load(char *filename)
{
	m_bLoaded = LoadCompMovie(filename);
  memset(m_pBuffer,0,320*240); 
	memset(m_pBufferRGB,0,320*240<<2);

  return m_bLoaded;
};

//////////////////////////////////////////////////////////////////////
//
// void Load(char *filename);

bool CVideo::LoadPal(char *filename)
{
  FILE* fPal;
	FILE_Open(&fPal,filename,"rb");	
	FILE_Read(&m_aPal[0],1,768,fPal);	

	char* pPal = (char*)&m_aPal[0];
	pPal += 768;
	for(int i=255;i>=0;i--)
	{
		pPal -= 3;
		m_aPal[i] = (pPal[0]<<18) + (pPal[1]<<10) + (pPal[2]<<2);
	}
	FILE_Close(fPal);
	return true;
}

//////////////////////////////////////////////////////////////////////
//
// char* RenderAlpha();

char* CVideo::RenderAlpha()
{
    DecodeFrame();
	return m_pBuffer;
}

//////////////////////////////////////////////////////////////////////
//
// void RenderRGB(unsigned long* dstRGB);

void CVideo::RenderRGB(unsigned long* dstRGB)
{
	char* pScrnBW = DecodeFrame();
    long NOISE = (m_iNoiseLevel<<8)+m_iNoiseLevel;
    
	__asm{
		pushad
		lea   esi,pScrnBW
		mov   esi,[esi]
		lea   edi,dstRGB
		mov   edi,[edi]
        add   edi,20*320*4
		xor   ebx,ebx

		buc:
		 mov   cx,[esi+ebx]

         shl   ecx,2
         and   ecx,((63<<8)+63)<<2

         call  ZMRand
         //and   eax,(INOISE<<8)+INOISE*3 //-->TV efx!
         and   eax,NOISE
         add   eax,ecx

		 mov   dl,ah
		 mov   ah,al
		 mov   dh,dl
		 shl   eax,8
		 shl   edx,8
		 mov   al,ah
		 mov   dl,dh
		 
         mov   [edi+ebx*4],eax
		 mov   [edi+ebx*4+4],edx
		 add   ebx,2
		 cmp   ebx,320*200
		 jne   buc

		popad
	};
};

//////////////////////////////////////////////////////////////////////
//
// void RenderRGBBlue(unsigned long* dstRGB);

void CVideo::RenderRGBPal(unsigned long* dstRGB)
{
	static bool bPalLocal = false;
	static long aPal[256];
	if(!bPalLocal)
	{
		memcpy(aPal,m_aPal,sizeof(m_aPal));
		bPalLocal = true;
	}

	char* pScrnBW = DecodeFrame();
    long NOISE = (m_iNoiseLevel<<8)+m_iNoiseLevel;
    
	__asm{
		pushad
		lea   esi,pScrnBW
		mov   esi,[esi]
		lea   edi,dstRGB
		mov   edi,[edi]
        add   edi,20*320*4
		xor   ebx,ebx

		buc:
		 mov   cx,[esi+ebx]

         shl   ecx,2
         and   ecx,((63<<8)+63)<<2

         call  ZMRand
         and   eax,NOISE
         add   eax,ecx
		 mov   edx,eax
		 shr   eax,8	
		 and   edx,0FFh
		 and   eax,0FFh

		 mov   edx,aPal[edx*4]
		 mov   eax,aPal[eax*4]

         mov   [edi+ebx*4],edx
		 mov   [edi+ebx*4+4],eax
		 add   ebx,2
		 cmp   ebx,320*200
		 jne   buc

		popad
	};
};


//-- Funciones aux ---------------------------------------------------

//-- long ZMRand() ---------------------------------------------------

static long Sem1 = 0x01234;
static long Sem2 = 0x09876;

long ZMRand()
{
  _asm {
	  push  edx
	  mov   eax,[Sem1]
	  mov   edx,[Sem2]
	  add   edx,eax
	  mul   edx
	  mov   [Sem2],eax
	  xor   edx,eax
	  add   edx,0x0E46C
	  mov   [Sem1],edx
	  mov   eax,edx
	  pop   edx
	};
};

/* ----------------------------------------------------------- */

void CVideo::Decode(char *ActFrame,                //-- Compressed Frame to decode
            tRFrame LastFrame,             //-- Last decomp. Frame in raw format
            tRFrame RDest)                 //-- Decompressed Frame in raw format
{
  char *OrigPtr = ActFrame;
  if((*OrigPtr)!=-1)
  {
     //Compressed frame
     for(DWORD j=0;j<COM_Y;j++)
      for(DWORD i=0;i<COM_X;i++)
      {
         unsigned char an = *(OrigPtr++);
         //Nibble 1
         RDest[j][i<<1] = (LastFrame[j][i<<1] + DECODEC[an & 0x00F]) & 63;
         //Nibble 2
         RDest[j][(i<<1)+1] = (LastFrame[j][(i<<1)+1] + DECODEC[an>>4]) & 63;
       }
  }
  else
  {
     //Uncompressed 6bit frame
     OrigPtr++;
     unsigned char *DestPtr = (unsigned char *)RDest;
     for(int j=0;j<RAW_Y*RAW_X/4;j++)
     {
          unsigned char b1 = *(OrigPtr++);
          unsigned char b2 = *(OrigPtr++);
          unsigned char b3 = *(OrigPtr++);
          *(DestPtr++) = b1>>2;
          *(DestPtr++) = ((b1<<4)+(b2>>4)) & 63;
          *(DestPtr++) = ((b2<<2)+(b3>>6)) & 63;
          *(DestPtr++) = b3 & 63;     
     }
  }
};

/* ----------------------------------------------------------- */

void CVideo::RFrame2Video(tRFrame Orig,char* Dest)
{
  char Last,Act;
  char* DestPtr = Dest;

  //Horiz.Interpolation
  for(int j=0;j<RAW_Y;j++)
  {
     Act = Orig[j][0];
     *DestPtr = Act;
     DestPtr+=2;
     for(int i=1;i<RAW_X;i++)
     {
        Last = Act;
        Act = Orig[j][i];
        *(DestPtr-1) = (Last+Act)>>1;
        *DestPtr = Act;
        DestPtr+=2;
      }
     DestPtr+=(RAW_X*2*3);
  };

  //Vert.Interpolation 1
  DestPtr = (Dest+(RAW_X*4));
  for(int j=0;j<RAW_Y;j++)
  {
     for(int i=0;i<(RAW_X*2);i++)
       *(DestPtr++) = (((*(DestPtr-320*2)+*(DestPtr+320*2))<<1)+ 
                       (*(DestPtr-320*2-2)+*(DestPtr+320*2-2)+
                       (*(DestPtr-320*2+2)+*(DestPtr+320*2+2))))>>3;
     DestPtr+=(RAW_X*2)*3;
  };

  //Vert.Interpolation 2
  DestPtr = (Dest+(RAW_X*2));
  for(int j=0;j<(RAW_Y*2);j++)
  {
     for(int i=0;i<(RAW_X*2);i++)
      *(DestPtr++) = (*(DestPtr-(RAW_X*2))+*(DestPtr+(RAW_X*2))) >> m_iInterlace;
     DestPtr+=(RAW_X*2);
  };
};

//    -----------------------------------------------------------

#define IDISK_CACHE 2048
 
#define GetBuf( n )                  \
{                                    \
  if ((PtrBuf & 1)==0)               \
   n = (RdBuf[PtrBuf>>1] & 0x0F);    \
  else                               \
   n = (RdBuf[PtrBuf>>1] >>4);       \
  if (++PtrBuf==IDISK_CACHE*2)       \
  {                                  \
    PtrBuf=0;                        \
    FILE_Read(&RdBuf,IDISK_CACHE,1,fZMV);\
  };                                 \
}


#define WrNibble( n )                \
{                                    \
 if (bLastWritedOdd)                 \
 {(*DestPtr)+=(n <<4);               \
  DestPtr++;                         \
 }else                               \
  (*DestPtr)=n;                      \
 bLastWritedOdd=!bLastWritedOdd;     \
}

bool CVideo::LoadCompMovie(char *fname)
{
  unsigned char RdBuf[IDISK_CACHE];
  unsigned char data,iCount;
  bool bLastWritedOdd = false;
  int w;
  
  FILE *fZMV;
  FILE_Open(&fZMV,fname,"rb");
  m_iTotalFrames = 0;
  FILE_Read(&m_iTotalFrames,1,2,fZMV);
  m_pMovie = (char *)GetMem(2001+m_iTotalFrames*sizeof(tCFrame));
  unsigned char *DestPtr = (unsigned char *)m_pMovie;
  long PtrBuf = 0;
  FILE_Read(&RdBuf,IDISK_CACHE,1,fZMV);

  int Nleft = m_iTotalFrames*sizeof(tRFrame);
  do
  {
    GetBuf( data );
    switch (data)
    {
     case 0x0E:
          GetBuf( iCount );
          Nleft-=iCount;
          for (w=0;w<iCount;w++)
           WrNibble( 0 );        
          break;

     case 0x0F:
          GetBuf( data );
          if (data!=0x0F)
          {
            GetBuf( iCount ); 
            Nleft-=iCount;
            for (w=0;w<iCount;w++) 
             WrNibble( data );
          }
          else
          {
            WrNibble( 0x0F );
            WrNibble( 0x0F );
            Nleft-=RAW_X*RAW_Y;
            for (w=0;w<DEC_X*DEC_Y*2;w++)
            {
             GetBuf( data );
             WrNibble( data );
            }
          };
          break;

     default:
          WrNibble( data );
          Nleft--;
          break;           
    };
  } while (Nleft!=0);
  FILE_Close(fZMV);

  memset(tempRFrame,0,sizeof(tRFrame));
  m_pMoviePtr = m_pMovie;
  m_iActFrame=0;
  return true;
};

//     -----------------------------------------------------------

char* CVideo::DecodeFrame()
{
  if(m_iActFrame != -1)
  {
	Decode(m_pMoviePtr,tempRFrame,tempRFrame);
	RFrame2Video(tempRFrame,m_pBuffer);
  }
  else
	return m_pBuffer; //video finalizado sin rewind

  if((m_iActFrame) >= m_iTotalFrames-1)
  {
	if(m_bRewind)
	{
	  m_iActFrame = 0;
	  m_pMoviePtr = m_pMovie;
	  MISC_MemSet4(tempRFrame,0,sizeof(tRFrame)/4);
	} 
	else
	  m_iActFrame = -1;
  }
  else 
  {
	if (m_iActFrame++ == 0)
	  m_pMoviePtr += 6001;
	else
	  m_pMoviePtr += 4000;
  }
 
  return m_pBuffer;
};



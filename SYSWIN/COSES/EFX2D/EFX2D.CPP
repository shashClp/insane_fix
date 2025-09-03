#include <efx2d.h>
#include <string.h>
#include <image.h>
#include <file.h>
#include <mem.h>
#include <math.h>
#include <log.h>


//////////////////////////////////////////////////////////////////////////////
//
//  void GetBM8
//  Pilla un BitMap de un layer alpha (sin optimizadar)
void GetBM8(int x1,int y1,int x2,int y2,char* buffer,char* src, int src_width)
{
	long iH = x2-x1+1;
	long iV = y2-y1+1;

	*((long*)buffer) = iH;
    buffer += 4;
	*((long*)buffer) = iV;
    buffer += 4;

	for(int j=y1;j<y1+iV;j++)
	  for(int i=x1;i<x1+iH;i++)
		  *(buffer++) = *((char*)src + j*src_width + i);
}

//////////////////////////////////////////////////////////////////////////////
//
//	void PutBM8(int x1,int y1,char* buffer, void* dst = GBL_Bits,bool alpha = true,long width = GBL_Width)
//  Pone un BitMap en un layer alpha (sin optimizadar)
void PutBM8(int x1,int y1,char* buffer, void* dst,bool alpha,long width)
{
	long iH = *((long*)buffer);	
  buffer += 4;

	long iV = *((long*)buffer); 
  buffer += 4;

  DWORD yr=0,yi=y1,yf=y1+iV;
  if (yi<0) { yi=0; yr=(0-y1); }
  if (yf>GBL_Height) 
	 yf=GBL_Height;
  char *dest=(char *)dst+((yi)*width)+x1;
  buffer+=yr*iH;
  
	if(alpha)  
  {
	  for(int j=yi;j<yf;j++)
    {
	    for(int i=x1;i<x1+iH;i++)
      {
        if (*((BYTE *)dest) < *((BYTE *)buffer)) *dest = *buffer;
        dest++;
        buffer++;
      }
      dest+=width-iH;
    }
  }
}


//////////////////////////////////////////////////////////////////////////////
//
//  Rutinas de Fuentes
//  ------------------
//  SFuente* FONT_Load(char* sFileName);
//  void FONT_Close(SFuente* fnt);
//  void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,bool alpha=true);

SFuente* FONT_Load(char* sFileName)
{	
  SFuente* fnt = (SFuente*)GetMem(sizeof(SFuente));
  
  TImage *imgTmp = IMAGE_Load(sFileName,8);
  char* pAct = (char*)imgTmp->bits; //Borra maya de delimitacion de caracteres (color 1)
  for(int ij=0; ij<(int)imgTmp->height;ij++) 
    for(int ii=0;ii<(int)imgTmp->width;ii++)
        if(*pAct==1)
            *pAct = 0;

  fnt->iH = imgTmp->width / 26;
  fnt->iV = imgTmp->height / 4;

  LOG ("Cargando fuente %s (%i,%i)",sFileName,fnt->iH,fnt->iV);
  
  for(int i=0;i<10;i++) //10 digitos
  {	    
		int iX=i*(fnt->iH);
		int iY=0;
        fnt->pBuf[i] = (char*)GetMem(8+fnt->iH*fnt->iV);
		GetBM8(iX,iY,iX+fnt->iH-1,iY+fnt->iV-1,fnt->pBuf[i],(char*)imgTmp->bits,imgTmp->width);
  }

  for(int j=0;j<3;j++) //3 linias de 26 caracteres
    for(int i=0;i<26;i++)
	{
	  int iX=i*(fnt->iH);
	  int iY=(1+j)*(fnt->iV);
	  fnt->pBuf[10+j*26+i] = (char*)GetMem(8+fnt->iH*fnt->iV);
	  GetBM8(iX,iY,iX+fnt->iH-1,iY+fnt->iV-1,fnt->pBuf[10+i+j*26],(char*)imgTmp->bits,imgTmp->width);
	}
  

  FreeMem (imgTmp);

  return fnt;
}

void FONT_Close(SFuente* fnt)
{
  for(int i=0;i<26*3+10;i++)
	  FreeMem(fnt->pBuf[i]);
  FreeMem(fnt);
}

char FONT_Translate(char c)
{

  if(c>=47 && c<=56) //digitos
    return c-47;
  if(c>=65 && c<=90) //mayusculas 
    return c-65+10;
  if(c>=97 && c<=122) //minusculas
    return c-97+10+26;
  switch(c)
  {
        case '.':return 62;
        case ',':return 63;
        case '(':return 66;
        case ')':return 67;
        case '{':return 68;
        case '}':return 69;
        case '?':return 70;
        case ':':return 71;
        case '@':return 72;
        case '<':return 73;
        case '>':return 74;
        case '+':return 75;
        case '-':return 76;
        case '!':return 77;
        case '"':return 78;
        //case '''':return 79;
        default:return 80;
  };


}

//	void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,int espacio = 0,bool alpha=true)
void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,int espacio,bool alpha)
{
  if(espacio==0)
      espacio=fnt->iH;
  if(alpha)
    for(long i=0;pTxt[i]!=0;i++)
      PutBM8(x+i*espacio,y,(char*)fnt->pBuf[FONT_Translate(pTxt[i])],(char*)dst);
}


#define Y_MIN 20
#define Y_MAX 220
#define X_MIN 0
#define X_MAX 320
#define SCR_X 320
#define SCR_Y 240

//////////////////////////////////////////////////////////////////////
//
// void RotoZoom(float fAlpha,float fZoom,float cpX,float cpY,bool clip,DWORD* dst,DWORD* src);

void RotoZoom(float fAlpha,float fZoom,float cpX,float cpY,bool clip,DWORD* dst,DWORD* src)
//TODO: optimizar como el de alpha! 
// -bug: trabajar con 7 bits, en vez de 8!
// -calcular eficientemente t11,t12,t21,t22
{  
  float ang = fAlpha;  
  float zoom = fZoom;

  unsigned long pixel;

  unsigned long* BitMap = src;
  unsigned long* pDest = dst + (SCR_X*(SCR_Y-(Y_MAX-Y_MIN))>>1);
       
  __int32 x1 = (__int32)floor((128-zoom*(cpX*cos(ang)-cpY*sin(ang)))*65536);
  __int32 y1 = (__int32)floor((128-zoom*(cpY*cos(ang)+cpX*sin(ang)))*65536);
  __int32 deltaX = (__int32)floor(cos(ang)*zoom*65536);
  __int32 deltaY = (__int32)floor(sin(ang)*zoom*65536);  

  bool bNoClip = !clip;

  for(__int32 yc=Y_MIN;yc<Y_MAX;yc++)
  {        
      __int32 x2 = x1;
      __int32 y2 = y1;
      for(__int32 xc=X_MIN;xc<X_MAX;xc++)
      {
          if(bNoClip || (x2>=0 && x2<=255<<16 && y2>=0 && y2 <=255<<16))
          {
              unsigned long t11 = *(BitMap +((((y2 >> 8)& 0x0FF00)+(x2 >> 16))& 0x0FFFF));
              unsigned long t12 = *(BitMap +((((y2 >> 8)& 0x0FF00)+((x2 >> 16)+1))& 0x0FFFF));
              unsigned long t21 = *(BitMap +(((((y2 >> 8)+256)& 0x0FF00)+(x2 >> 16))& 0x0FFFF));
              unsigned long t22 = *(BitMap +(((((y2 >> 8)+256)& 0x0FF00)+((x2 >> 16)+1))& 0x0FFFF));
              unsigned long a_X = x2 & 0x0FFFF;
              unsigned long a_Y = y2 & 0x0FFFF;

              __asm{
                  pushad
                  xor   eax,eax
                  mov   [pixel],eax

                  //R
                  mov   ecx,[a_X] //factor X
                  mov   al,BYTE ptr[t12+2] //cargar incr
                  mov   bl,BYTE ptr[t22+2]
                  sub   al,BYTE ptr[t11+2]
                  sub   bl,BYTE ptr[t21+2]
                  sal   eax,24 //extender signo
                  sal   ebx,24
                  sar   eax,24
                  sar   ebx,24
                  imul  ecx
                  xchg  eax,ecx
                  sar   ecx,16 //parte entera
                  imul  ebx
                  add   cl,BYTE ptr[t11+2]                  
                  sar   eax,16 //parte entera
                  mov   ebx,[a_Y] //factor Y
                  add   al,BYTE ptr[t21+2]
                  sub   al,cl
                  sal   eax,24 //extender signo
                  sal   ecx,24
                  sar   eax,24
                  sar   ecx,24
                  imul  ebx
                  sar   eax,16 //parte entera
                  add   eax,ecx                  
                  mov   BYTE ptr[pixel+2],al

                  //G
                  mov   ecx,[a_X] //factor X
                  mov   al,BYTE ptr[t12+1] //cargar incr
                  mov   bl,BYTE ptr[t22+1]
                  sub   al,BYTE ptr[t11+1]
                  sub   bl,BYTE ptr[t21+1]
                  sal   eax,24 //extender signo
                  sal   ebx,24
                  sar   eax,24
                  sar   ebx,24
                  imul  ecx
                  xchg  eax,ecx
                  sar   ecx,16 //parte entera
                  imul  ebx
                  add   cl,BYTE ptr[t11+1]
                  sar   eax,16 //parte entera
                  mov   ebx,[a_Y] //factor Y
                  add   al,BYTE ptr[t21+1]
                  sub   al,cl
                  sal   eax,24 //extender signo
                  sal   ecx,24
                  sar   eax,24
                  sar   ecx,24
                  imul  ebx
                  sar   eax,16 //parte entera
                  add   eax,ecx                  
                  mov   BYTE ptr[pixel+1],al

                  //B  
                  mov   ecx,[a_X] //factor X                  
                  mov   al,BYTE ptr[t12] //cargar incr
                  mov   bl,BYTE ptr[t22]
                  sub   al,BYTE ptr[t11]
                  sub   bl,BYTE ptr[t21]
                  sal   eax,24 //extender signo
                  sal   ebx,24
                  sar   eax,24
                  sar   ebx,24
                  imul  ecx
                  xchg  eax,ecx
                  sar   ecx,16 //parte entera
                  imul  ebx
                  add   cl,BYTE ptr[t11]
                  sar   eax,16 //parte entera
                  mov   ebx,[a_Y] //factor Y
                  add   al,BYTE ptr[t21]
                  sub   al,cl
                  sal   eax,24 //extender signo
                  sal   ecx,24
                  sar   eax,24
                  sar   ecx,24
                  imul   ebx
                  sar   eax,16 //parte entera
                  add   eax,ecx
                  mov   BYTE ptr[pixel],al
                  popad
              };
              *(pDest++) =  pixel;     
          }
          else
          {
              pDest++;
          };
          x2+=deltaX;
          y2+=deltaY;                           
      };
      x1-= deltaY;
      y1+= deltaX;
  }

};

//////////////////////////////////////////////////////////////////////
//
// void RotoZoom_Texel(float fAlpha,float fZoom,float cenX,float cenY,DWORD* dst,DWORD* src);

void RotoZoom_Texel(float fAlpha,float fZoom,float cpX,float cpY,DWORD* dst,DWORD* src)
{  
  float ang = fAlpha;  
  float zoom = fZoom;

  unsigned long* BitMap = src;
  unsigned long* pDest = dst + (SCR_X*(SCR_Y-(Y_MAX-Y_MIN))>>1);
       
  __int32 x1 = (__int32)floor((128-zoom*(cpX*cos(ang)-cpY*sin(ang)))*65536);
  __int32 y1 = (__int32)floor((128-zoom*(cpY*cos(ang)+cpX*sin(ang)))*65536);
  __int32 deltaX = (__int32)floor(cos(ang)*zoom*65536);
  __int32 deltaY = (__int32)floor(sin(ang)*zoom*65536);  
       
  for(__int32 yc=Y_MIN;yc<Y_MAX;yc++)
  {        
          __int32 x2 = x1;
          __int32 y2 = y1;
          for(__int32 xc=X_MIN;xc<X_MAX;xc++)
          {
              *(pDest++) = *(BitMap +((((y2 >> 8)& 0x0FF00)+(x2 >> 16))& 0x0FFFF));
              x2+=deltaX;
              y2+=deltaY;              
          }
          x1-= deltaY;
          y1+= deltaX;
  }

};

//////////////////////////////////////////////////////////////////////
//
// void RotoZoomA(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src);

void RotoZoomA(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src)
{  
  float ang = fAlpha;  
  float zoom = fZoom;

  unsigned long pixel;

  char* BitMap = src;
  char* pDest = dst + (SCR_X*(SCR_Y-(Y_MAX-Y_MIN))>>1);
       
  __int32 x1 = (__int32)floor((128-zoom*(cpX*cos(ang)-cpY*sin(ang)))*65536);
  __int32 y1 = (__int32)floor((128-zoom*(cpY*cos(ang)+cpX*sin(ang)))*65536);
  __int32 deltaX = (__int32)floor(cos(ang)*zoom*65536);
  __int32 deltaY = (__int32)floor(sin(ang)*zoom*65536);  
       
  for(__int32 yc=Y_MIN;yc<Y_MAX;yc++)
  {        
          __int32 x2 = x1;
          __int32 y2 = y1;
          for(__int32 xc=X_MIN;xc<X_MAX;xc++)
          {
							unsigned char t11;// = *(BitMap +((((y2 >> 8)& 0x0FF00)+(x2 >> 16))& 0x0FFFF))>>1;
              unsigned char t12;// = *(BitMap +((((y2 >> 8)& 0x0FF00)+((x2 >> 16)+1))& 0x0FFFF))>>1;
              unsigned char t21;// = *(BitMap +(((((y2 >> 8)+256)& 0x0FF00)+(x2 >> 16))& 0x0FFFF))>>1;
              unsigned char t22;// = *(BitMap +(((((y2 >> 8)+256)& 0x0FF00)+((x2 >> 16)+1))& 0x0FFFF))>>1;
              unsigned long a_X = x2 & 0x0FFFF;
              unsigned long a_Y = y2 & 0x0FFFF;

							__asm{
                  //pushad
                  //xor   eax,eax
                  //mov   [pixel],eax
									xor		edx,edx
									mov		dx,WORD Ptr[x2+2]
									add		dh,BYTE Ptr[y2+2]
									add		edx,[BitMap]

									mov		ax,[edx]
									ror		eax,16
									mov		ax,[edx+256]
									shr		eax,1
									and		eax,0x7f7f7f7f	//eax: 12/11/22/21
									mov		[t21],al
									mov		ebx,eax
									shr		eax,16
									mov		[t11],al
									sub		bh,bl	//	bh = 22 - 21
									sub		ah,al	//	ah = 12 - 11

                  mov   ecx,[a_X] //factor X
                  sal   eax,16 //extender signo
                  sal   ebx,16
                  sar   eax,24
                  sar   ebx,24
                  imul  ecx
                  xchg  eax,ecx
                  sar   ecx,16 //parte entera
                  imul  ebx
                  add   cl,BYTE ptr[t11]                  
                  sar   eax,16 //parte entera
                  mov   ebx,[a_Y] //factor Y
                  add   al,BYTE ptr[t21]
                  sub   al,cl
                  sal   eax,24 //extender signo
                  sal   ecx,24
                  sar   eax,24
                  sar   ecx,24
                  imul  ebx
                  sar   eax,16 //parte entera
                  add   eax,ecx                  
	                mov   BYTE ptr[pixel],al             
                  //popad
              };
              *(pDest++) =  (BYTE) (pixel<<1); 

              //*(pDest++) = *(BitMap +((((y2 >> 8)& 0x0FF00)+(x2 >> 16))& 0x0FFFF));
              x2+=deltaX;
              y2+=deltaY;              
          }
          x1-= deltaY;
          y1+= deltaX;
  }

};

//////////////////////////////////////////////////////////////////////
//
// void RotoZoomA_Texel(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src);

void RotoZoomA_Texel(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src)
{  
  float ang = fAlpha;  
  float zoom = fZoom;

  char* BitMap = src;
  char* pDest = dst + (SCR_X*(SCR_Y-(Y_MAX-Y_MIN))>>1);
       
  __int32 x1 = (__int32)floor((128-zoom*(cpX*cos(ang)-cpY*sin(ang)))*65536);
  __int32 y1 = (__int32)floor((128-zoom*(cpY*cos(ang)+cpX*sin(ang)))*65536);
  __int32 deltaX = (__int32)floor(cos(ang)*zoom*65536);
  __int32 deltaY = (__int32)floor(sin(ang)*zoom*65536);  
       
  for(__int32 yc=Y_MIN;yc<Y_MAX;yc++)
  {        
          __int32 x2 = x1;
          __int32 y2 = y1;
          for(__int32 xc=X_MIN;xc<X_MAX;xc++)
          {
              *(pDest++) = *(BitMap +((((y2 >> 8)& 0x0FF00)+(x2 >> 16))& 0x0FFFF));
              x2+=deltaX;
              y2+=deltaY;              
          }
          x1-= deltaY;
          y1+= deltaX;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
//   Transparencia al %indicado entre 2 layers de alpha

void Morph8(char* dst,char* src1,char* src2,DWORD fact)
// a*x + (1-a)*y --> a*(x-y) + y
{
  _asm{
      pushad

      mov ebx,GBL_Size
      mov ecx, fact //solo cl [0..127]
      shr ebx,2
	  mov edi,[dst]
	  neg ebx
	  mov esi,[src1]
      mov ebp,[src2]
	  sub edi,ebx
	  sub esi,ebx
      sub ebp,ebx
     buc:
	  mov al,[esi+ebx]
      mov ch,[ebp+ebx]
      shr al,1
      shr ch,1
      sub al,ch
      imul cl
      add ah,ch      
      shl ah,2
      mov [edi+ebx],ah		
	  inc ebx
	  jnz buc

      popad
   };
}


//////////////////////////////////////////////////////////////////////////////
//
//   Transparencia al %indicado entre 2 layers ARGB

void MorphARGB(DWORD* dst,DWORD* src1,DWORD* src2,DWORD fact)
// a*x + (1-a)*y --> a*(x-y) + y
{
  _asm{
      pushad

      mov ebx,GBL_Size4
      mov edx, fact //solo dl [0..255]
	  mov edi,[dst]
	  neg ebx
	  mov esi,[src1]
      mov ebp,[src2]
	  sub edi,ebx
	  sub esi,ebx
      sub ebp,ebx
	  
     buc:
	 xor eax,eax
	 xor ecx,ecx
	  mov al,[esi+ebx]
      mov cl,[ebp+ebx]      
      sub eax,ecx
	 mov ch,dl //guardamos dl
      imul edx
	 mov edx,ecx
	 shr edx,8
      add ah,cl
      mov [edi+ebx],ah		
	  inc ebx
	  jnz buc

      popad
   };
}

//////////////////////////////////////////////////////////////////////////////
//
//  Motion Blur al 50%

void MotionBlur32(DWORD* dst,DWORD* src)
{
	_asm{
		pushad

		mov ebx,GBL_Size4
		mov edi,[dst]
		neg ebx
		mov esi,[src]
		sub edi,ebx
		sub esi,ebx

     buc:
		mov eax,[edi+ebx]
		mov edx,[esi+ebx]
		and eax,0x0FEFEFEFE
		and edx,0x0FEFEFEFE
		shr eax,1
		shr edx,1
		add eax,edx
		mov [edi+ebx],eax
		add ebx,4
		jnz buc

		popad
	}
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlpha
//  Superpone un layer alpha en un layer RGB (alpha -> blanco)

void FlatternAlpha(void* dst,void* src,void* alpha,int iSize)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  _asm{
    pushad
    mov eax,[iSize]    
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[iSize]
    neg ebx

    mov eax,[iSize]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
   not cl
    mul cl
   not cl
    add cl,ah
    jnc ok1
     mov cl,0x0FF
    ok1:

    mov eax,edx         //G
   not ch
    mul ch
   not ch
    add ch,ah
    jnc ok2
     mov ch,0x0FF
    ok2:
    ror ecx,16

    mov eax,edx         //R
   not cl
    mul cl
   not cl
    add cl,ah
    jnc ok3
     mov cl,0x0FF
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlphaBlue
//  Superpone un layer alpha en un layer RGB (alpha -> cyan)
void FlatternAlphaBlue(void* dst,void* src,void* alpha)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  DWORD cnt = GBL_Size;
  _asm{
    pushad
    mov eax,[cnt]    
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[cnt]
    neg ebx

    mov eax,[cnt]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
   not cl
    mul cl
   not cl
    add cl,ah
    jnc ok1
     mov cl,0x0FF
    ok1:    

    mov eax,edx         //G
   not ch
    mul ch
   not ch
shr ah,1
    add ch,ah
    jnc ok2
     mov ch,0x0FF
    ok2:
    ror ecx,16

    mov eax,edx         //R
   not cl
    mul cl
   not cl
shr ah,2
    add cl,ah
    jnc ok3
     mov cl,0x0FF
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlphaInv
//  Elimina de un layer RGB la componente alpha (alpha -> negro)

void FlatternAlphaInv(void* dst,void* src,void* alpha,int iSize /*=GBL_Width*GBL_Height*/)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  _asm{
    pushad
    mov eax,[iSize]
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[iSize]
    neg ebx

    mov eax,[iSize]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
//   not cl
    mul cl
//   not cl
    sub cl,ah
    jnc ok1
     mov cl,0x000
    ok1:

    mov eax,edx         //G
//   not ch
    mul ch
//   not ch
    sub ch,ah
    jnc ok2
     mov ch,0x000
    ok2:
    ror ecx,16

    mov eax,edx         //R
//   not cl
    mul cl
//   not cl
    sub cl,ah
    jnc ok3
     mov cl,0x000
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlphaInvGreen
//  Elimina de un layer RGB la componente alpha (alpha -> negro)

void FlatternAlphaInvGreen(void* dst,void* src,void* alpha,int iSize /*=GBL_Width*GBL_Height*/)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  _asm{
    pushad
    mov eax,[iSize]
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[iSize]
    neg ebx

    mov eax,[iSize]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
//   not cl
    mul cl
//   not cl
    sub cl,ah
    jnc ok1
     mov cl,0x000
    ok1:

    mov eax,edx         //G
   not ch
    mul ch
   not ch
    ADD ch,ah
    jnc ok2
     mov ch,0x000
    ok2:
    ror ecx,16

    mov eax,edx         //R
//   not cl
    mul cl
//   not cl
    sub cl,ah
    jnc ok3
     mov cl,0x000
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlphaInvBlue
//  Elimina de un layer RGB la componente alpha (alpha -> negro)

void FlatternAlphaInvBlue(void* dst,void* src,void* alpha,int iSize /*=GBL_Width*GBL_Height*/)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  _asm{
    pushad
    mov eax,[iSize]
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[iSize]
    neg ebx

    mov eax,[iSize]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
   not cl
    mul cl
   not cl
    sub cl,ah
    jnc ok1
     mov cl,0x000
    ok1:

    mov eax,edx         //G
//   not ch
    mul ch
//   not ch
    sub ch,ah
    jnc ok2
     mov ch,0x000
    ok2:
    ror ecx,16

    mov eax,edx         //R
//   not cl
    mul cl
//   not cl
    ADD cl,ah
    jnc ok3
     mov cl,0x000
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void FlatternAlphaInvRed
//  Elimina de un layer RGB la componente alpha (alpha -> negro)

void FlatternAlphaInvRed(void* dst,void* src,void* alpha,int iSize /*=GBL_Width*GBL_Height*/)
//dst y src en 32bits ARGB; alpha en 8 bits A
{
  _asm{
    pushad
    mov eax,[iSize]    
    mov esi,[src]
    shl eax,2
    mov edi,[dst]
    add esi,eax
    add edi,eax    
    
    mov ebx,[iSize]
    neg ebx

    mov eax,[iSize]
    mov ebp,[alpha]
    add ebp,eax

   buc:
    mov ecx,[esi+ebx*4] //ARGB
    mov dl,[ebp+ebx]    //Alpha [dl]

    mov eax,edx         //B
//   not cl
    mul cl
//   not cl
    sub cl,ah
    jnc ok1
     mov cl,0x000
    ok1:

    mov eax,edx         //G
//   not ch
    mul ch
//   not ch
    sub ch,ah
    jnc ok2
     mov ch,0x000
    ok2:
    ror ecx,16

    mov eax,edx         //R
   not cl
    mul cl
   not cl
    ADD cl,ah
    jnc ok3
     mov cl,0x000
    ok3:
    rol ecx,16
 
    mov [edi+ebx*4],ecx
    inc ebx
    jnz buc

    popad
  }
};

//////////////////////////////////////////////////////////////////////////////
//
//  void MergeAlphaADD(void* dst,void* src1,void* src2,int size);
//  Superpone un layer alpha en otro layer alpha 

void MergeAlphaADD(void* dst,void* src1,void* src2,int iSize /*=GBL_Width*GBL_Height*/)
{
	__asm{
		pushad
		mov		ebx,[iSize]
		mov		edi,[dst]
		neg		ebx
		mov		esi,[src1]
		mov		ebp,[src2]
		sub		edi,ebx
		sub		esi,ebx
		sub		ebp,ebx

		buc:
		 mov	al,[esi+ebx]
		 mov	ah,[ebp+ebx]
		 add	al,ah
		 jnc	sOk
		  mov	al,255
		 sOk:
		 mov	[edi+ebx],al
		 inc	ebx
		jnz		buc
		popad
	}
}

//////////////////////////////////////////////////////////////////////////////
//
//  void MergeAlphaMIX(void* dst,void* src1,void* src2,int size);
//  mezcla un layer alpha con otro layer alpha (alpha -> blanco)

void MergeAlphaMIX(void* dst,void* src1,void* src2,int iSize)
{
__asm{
		pushad
		mov		ebx,[iSize]
		mov		edi,[dst]
		neg		ebx
		mov		esi,[src1]
		mov		ebp,[src2]
		sub		edi,ebx
		sub		esi,ebx
		sub		ebp,ebx
		xor		eax,eax
		xor		ecx,ecx
		buc:
		 mov	al,[esi+ebx]
		 mov	cl,[ebp+ebx]
		 add	eax,ecx
		 shr	eax,1
		 mov	[edi+ebx],al
		 inc	ebx
		jnz		buc
		popad
	}
}

//////////////////////////////////////////////////////////////////////////////
//
//  void MergeAlphaMIX(void* dst,void* src1,void* src2,int size);
//  mezcla un layer alpha con otro layer alpha (alpha -> blanco)

void MergeAlphaMAX(void* dst,void* src1,void* src2,int iSize)
{
__asm{
		pushad
		mov		ebx,[iSize]
		mov		edi,[dst]
		neg		ebx
		mov		esi,[src1]
		mov		ebp,[src2]
		sub		edi,ebx
		sub		esi,ebx
		sub		ebp,ebx
		xor		eax,eax
		xor		ecx,ecx
		buc:
		 mov	al,[esi+ebx]
		 mov	cl,[ebp+ebx]
		 cmp	al,cl
		 jg		alG
		 mov	[edi+ebx],cl
		 inc	ebx
		 jnz	buc
		alG:
		 mov	[edi+ebx],al
		 inc	ebx
		 jnz	buc
		popad
	}
}


//////////////////////////////////////////////////////////////////////////////
//
//  void Alpha2RGB(void* dst,void* src);
//  convierte un layer alpha en un layer RGB (alpha -> blanco)
//	dst en 32bits ARGB; src en 8 bits A

void Alpha2RGB(void* dstRGB,void* srcA,int iSize)
{
	__asm{
		pushad
		mov   esi,[srcA]
		mov   edi,[dstRGB]
		mov	  ecx,[iSize]

		xor   ebx,ebx
		buc:
		 mov   ax,[esi+ebx]
//         shl   eax,2								//  \ Alpha mod 64
//         and   eax,((63<<8)+63)<<2	//	/
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
		 cmp   ebx,ecx
		 jne   buc

		popad
	};
}

//////////////////////////////////////////////////////////////////////////////
//
//  void Alpha2RGBPal(void* dst,void* src);
//  convierte un layer alpha en un layer RGB (alpha -> indice en la paleta 256c)
//	dst en 32bits ARGB; src en 8 bits A

void Alpha2RGBPal(void* dstRGB,void* srcA,void* pal,int iSize)
{
	__asm{
		pushad
		lea   esi,[srcA]
		lea   edi,[dstRGB]
		mov	  ecx,[pal]
		mov	  ecx,[ecx]
		xor   ebx,ebx

		buc:
		 mov   ax,[esi+ebx]
//         shl   eax,2								//  \ Alpha mod 64
//         and   eax,((63<<8)+63)<<2	//	/
		 mov   edx,eax
		 shr   eax,8	
		 and   edx,0FFh
		 and   eax,0FFh

		 mov   edx,[ecx+edx*4]
		 mov   eax,[ecx+eax*4]

         mov   [edi+ebx*4],edx
		 mov   [edi+ebx*4+4],eax
		 add   ebx,2
		 cmp   ebx,[iSize]
		 jne   buc
		popad
	};
}

//	void LoadPal(char *filename);
//	Carga la Paleta para Alpha2RGBPal

bool LoadPal(UINT aPal[256],char *filename)
{
  FILE *fPal;
	FILE_Open(&fPal,filename,"rb");
	FILE_Read(&aPal[0],1,768,fPal);	

	char* pPal = (char*)&aPal[0];
	pPal += 768;
	for(int i=255;i>=0;i--)
	{
		pPal -= 3;
		aPal[i] = (pPal[0]<<18) + (pPal[1]<<10) + (pPal[2]<<2);
	}
	FILE_Close(fPal);
	return true;
}

//////////////////////////////////////////////////////////////////////////////
//
//  void HDistAlpha
//  Distorsion Horizontal a lo 303 fast para layers alpha (resultado muy cutre!)

void HDistAlpha(void* dst,void* src,DWORD factor)
//dst y src en 8 bits A
{
  DWORD cnt =  ((GBL_Size>>2)-factor*8);

  _asm{
    pushad

    mov edx,[factor]
    adc edx,0

    shl edx,0

    mov esi,[src]
    mov edi,[dst]
    add edi,edx
    shr edx,0
    
    mov ebp,[cnt]        
    
   buc:
    xor ebx,ebx

    mov eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]    

    add esi,4
   
    //blur anterior
    mov ebx,[edi]
    and eax,0x0FEFEFEFE
    and ebx,0x0FEFEFEFE
    shr eax,1
    shr ebx,1
    add eax,ebx


   #define _HQ_BLUR
    
   #ifdef _HQ_BLUR
    //blur lateral
    mov ebx,eax

    mov ecx,[edi-1]
    and eax,0x0FF
    and ecx,0x0FF
    add eax,ecx
    shr ebx,8
    shr eax,1
    mov [edi],al

    mov eax,ebx
    mov ecx,[edi]
    and eax,0x0FF
    and ecx,0x0FF
    add eax,ecx
    shr ebx,8
    shr eax,1
    mov [edi+1],al

    mov eax,ebx
    mov ecx,[edi+1]
    and eax,0x0FF
    and ecx,0x0FF
    add eax,ecx
    shr ebx,8
    shr eax,1
    mov [edi+2],al

    mov eax,ebx
    mov ecx,[edi+2]
    and eax,0x0FF
    and ecx,0x0FF
    add eax,ecx
    shr ebx,8
    shr eax,1
    mov [edi+3],al

    add edi,4

   #else
    stosd
   #endif //_HQ_BLUR
    
    dec ebp
    jnz buc

    popad
    
  };

};


//////////////////////////////////////////////////////////////////////////////
//
//  void HDistAlpha1
//  Distorsion Horizontal a lo 303 fast para layers alpha (resultado muy cutre!)

void HDistAlpha1(void* dst,void* src,DWORD factor)
//dst y src en 8 bits A
//4 muestras por pixel
//src no puede pasar del indice 64 (=256/4) en cada pixel
{
  DWORD cnt =  ((GBL_Size>>2)-GBL_Width*factor*4);

  _asm{
    pushad

    mov edx,[factor]
	mov eax,GBL_Width
	mul edx
	mov edx,eax
    shl edx,2

    mov esi,[src]
    mov edi,[dst]
    add edi,edx
    shr edx,1
    
    mov ebp,[cnt]        
    
   buc:
    xor ebx,ebx

    mov eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]
    add ebx,edx
    add eax,[esi+ebx]    

    add esi,4
    stosd

    dec ebp
    jnz buc

    popad
  };

};


//////////////////////////////////////////////////////////////////////////////
//
//  void PutPart
//  Dibuja un particula alpha (TImage Bubble) de 64x64 de 8 bits en un layer alpha
//  (escalable y con clipping (muy guarro))
//  la comparacion entre layers puede ser MAX(x,y) o SUMA_ACOTADA(x,y) segun codigo

//TImage imgBubble;


void PutPart(char* dst,long px,long py,long t,char* PartBitMap)
{
  DWORD x8,y8,y;
  DWORD Xini,Xlong;

  if ((px>320-20) || (py>240-20) || (px+t<0+20) ||(py+t<0+20) || (t>100))
      return;

  px-=t>>1;
  py-=t>>1;

  _asm{
	  pushad
	  //--*Clip Total*--
	  mov 	eax,[px]
	  cmp 	eax,320-20
	  jge 	exit
	  add 	eax,[t]
	  cmp 	eax,0
	  jl		exit
	  mov 	eax,[py]
	  cmp 	eax,240-20
	  jge 	exit
	  add 	eax,[t]
	  cmp 	eax,0
	  jl		exit

	  //--*Calc. Delta*--
	  mov 	eax,64*256
	  xor 	dx,dx
	  div 	WORD Ptr[t]
	  mov 	ecx,eax 		 //--ecx:delta

	  //--*Clip Parcial Y*--
	  //--ClipY1
	  mov 	eax,[py]
	  mov 	[y8],0
	  mov 	ebx,[t] 			//--ebx:#y
	  sub 	eax,0
	  jge 	ClippedY1
	   add	ebx,eax
	   jz 	exit
	   mov	[py],0
	   xor	edx,edx
	   imul ecx
	   sub	[y8],eax //eax negativo
	  ClippedY1:
	  //--ClipY2
	  mov 	eax,[py]
	  add 	eax,[t]
	  sub 	eax,240-20
	  jl		ClippedY2
	   sub	ebx,eax
	  ClippedY2:

	  //--Inic.
	  mov 	edi,[py]
	  mov 	edx,ecx 			//edx:delta
	  lea 	edi,[edi+edi*4]
	  shl 	edi,6
	  add 	edi,[px]
	  add 	edi,[dst]
	  mov 	esi,[PartBitMap]

	  mov 	eax,[t]
	  mov 	[Xlong],eax
	  mov 	[Xini],0

   //align 4
   YBuc:
	  mov 	eax,[Xini]
	  mov 	[x8],eax
	  mov 	eax,[y8]
	  shr 	eax,8
	  mov 	ecx,[Xlong] 		//ecx:#x
	  shl 	eax,6
	  mov 	[y],eax
   //align 4
	  XBuc:
	   mov	eax,[x8]
	   shr	eax,8
	   add	eax,[y]
	   add	[x8],edx
       //buc para max
       mov	al,[esi+eax]
       cmp	al,[edi]
       jb    no_max
        mov	[edi],al
       no_max:
       //buc para suma acotada
//	   mov	al,[esi+eax]
//	   add	al,[edi]	   
//     jnc  Ok
//		  mov al,254
//	   Ok:
//	   mov	[edi],al
	   inc	edi
	   dec	ecx
	   jnz	XBuc
	  sub 	edi,[t]
	  add 	[y8],edx
	  add 	edi,320
	  dec 	ebx
	  jnz 	YBuc

	  exit:
	  popad
	  };
};


//////////////////////////////////////////////////////////////////////////////
//
//  void ScrollToScreen8
//  Dibuja la parte visible de un layer alpha que puede ser mas grande que
//  que el layer alpha sobre el cual se renderiza (clipping 100%)

void ScrollToScreen8(char* dst,TImage* scroll,long xpos,long ypos)
{
    if(xpos<0 || xpos>=scroll->width)
        return;

    long    src = (long)scroll->bits;
    long    src_width = scroll->width;
    long    posv = (ypos*GBL_Width);
    long    iter = min(GBL_Height-ypos,scroll->height);
    long    pixels_en_linea = min(GBL_Width,scroll->width-xpos);
    long    pixels_en_linea_div4 = pixels_en_linea >>2;
    long    pixels_en_linea_mod4 = pixels_en_linea & 0x03;
    long    margen_dst = GBL_Width - pixels_en_linea;
    long    margen_src = src_width - pixels_en_linea;

    _asm
    {
        pushad
        mov     edi,[dst]
        mov     esi,[src]
        add     edi,[posv]
        add     esi,[xpos]
        mov     ebx,[iter]
       bucY:
        //movs de 32 bits
        mov     ecx,[pixels_en_linea_div4]
        rep     movsd                
        //movs hasta ajustar la linea
        mov     ecx,[pixels_en_linea_mod4]
        rep     movsb
        add     esi,[margen_src]
        add     edi,[margen_dst]

        dec     ebx
        jnz     bucY
        popad
    }
}

//////////////////////////////////////////////////////////////////////////////
//
//  void ScrollToScreen32
//  Dibuja la parte visible de un layer RGB que puede ser mas grande que
//  que el layer aRGB sobre el cual se renderiza (clipping 100%)

void ScrollToScreen32(DWORD* dst,TImage* scroll,long xpos,long ypos,long ystart)
{
	if(xpos<0 || xpos>=scroll->width) return;

  long src = (long)scroll->bits + (ystart*scroll->width<<2);
  long src_width = scroll->width;
  long posv = (ypos*GBL_Width)<<2;
  long iter = min(GBL_Height-ypos,scroll->height);
  long pixels_en_linea = min(GBL_Width,scroll->width-xpos);
  long margen_dst = (GBL_Width - pixels_en_linea)<<2;
  long margen_src = (src_width - pixels_en_linea)<<2;

  _asm
  {
      pushad
      mov     edi,[dst]
      mov     esi,[src]
      add     edi,[posv]
      add     esi,[xpos]
      mov     ebx,[iter]
     bucY:
      //movs de 32 bits
      mov     ecx,[pixels_en_linea]
      rep     movsd                
      add     esi,[margen_src]
      add     edi,[margen_dst]

      dec     ebx
      jnz     bucY
      popad
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//  void Recorta32(DWORD* dst, long margen);
//	Anyade los margenes superior e inferior (simulando formato 16:9)

void Recorta32(TImage* img_dst, long margen)
{
	long pixels_negros= img_dst->width * margen;
	long dst = (long)img_dst->bits;
	long dst2 = (long)img_dst->bits + (((img_dst->height*img_dst->width) - pixels_negros)<<2);

	_asm
	{
		pushad
		xor		eax,eax
		mov		edi,[dst]
		mov		ecx,[pixels_negros]
		rep		stosd
		mov		edi,[dst2]
		mov		ecx,[pixels_negros]
		rep		stosd
		popad
	}
}

//////////////////////////////////////////////////////////////////////////////
//
//  void HDist8;
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina el blanco
// Factor<0 -> se difumina el negro

void HDist8(char* dst,int lin_ini,int lin_fin,char factor)
{
    if(factor==0) return;

    dst += GBL_Width*lin_ini;

    if(factor<0)
        //Difuminar Negro
        _asm
        {
            pushad
            neg     [factor]
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            NbucY:

                //hacia la drecha
                mov     bl,255
                mov     ecx,[GBL_Width]
    
               NbucX1: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado1
                 mov    al,bl
                Nnormalizado1:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor1
                 mov    bl,255
                Nnormalizado_factor1:

                dec     ecx
                jnz     NbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_Width]                
                    
               NbucX2: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado2
                 mov    al,bl
                Nnormalizado2:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor2
                 mov    bl,255
                Nnormalizado_factor2:

                dec     ecx
                jnz     NbucX2

                mov     eax,[GBL_Width]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz NbucY
            popad        
        }
    else
        //Difuminar Blanco
        _asm
        {
            pushad                        
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            BbucY:

                //hacia la drecha
                xor     bl,bl
                mov     ecx,[GBL_Width]
    
               BbucX1: 
                mov     al,[edi]
                cmp     al,bl
                ja      Bnormalizado1
                 mov    al,bl
                Bnormalizado1:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor1
                 xor    bl,bl
                Bnormalizado_factor1:

                dec     ecx
                jnz     BbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_Width]                
                    
               BbucX2: 
                mov     al,[edi]
                cmp     al,bl
                ja      Bnormalizado2
                 mov    al,bl
                Bnormalizado2:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor2
                 xor    bl,bl
                Bnormalizado_factor2:

                dec     ecx
                jnz     BbucX2

                mov     eax,[GBL_Width]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz BbucY
            popad        
        }
}

//////////////////////////////////////////////////////////////////////////////
//
//  void HDist8Des;
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina el blanco
// Factor<0 -> se difumina el negro
// Un factor positivo bajo (mas distorsion) hace que se desbanezca mas

void HDist8Des(char* dst,int lin_ini,int lin_fin,char factor)
{
    if(factor==0)
        return;

    dst += GBL_Width*lin_ini;

    char factor_des = (34 - factor<<1); //solo se utiliza con factores positivos (difusion de intensidad)
    if(factor_des < 0)
        factor_des = 0;

    if(factor<0)
        //Difuminar Negro
        _asm
        {
            pushad
            neg     [factor]
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            NbucY:

                //hacia la drecha
                mov     bl,255
                mov     ecx,[GBL_Width]
    
               NbucX1: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado1
                 mov    al,bl
                Nnormalizado1:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor1
                 mov    bl,255
                Nnormalizado_factor1:

                dec     ecx
                jnz     NbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_Width]                
                  
               NbucX2: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado2
                 mov    al,bl
                Nnormalizado2:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor2
                 mov    bl,255
                Nnormalizado_factor2:

                dec     ecx
                jnz     NbucX2

                mov     eax,[GBL_Width]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz NbucY
            popad        
        }
    else
        //Difuminar Blanco
        _asm
        {
            pushad                        
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            BbucY:

                //hacia la drecha
                xor     bl,bl
                mov     ecx,[GBL_Width]
    
               BbucX1: 
                mov     al,[edi]
                sub     al,[factor_des]
                jnc     Bnormalizada_entrada1
                 xor    al,al
                Bnormalizada_entrada1:

                cmp     al,bl
                ja      Bnormalizado1
                 mov    al,bl
                Bnormalizado1:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor1
                 xor    bl,bl
                Bnormalizado_factor1:

                dec     ecx
                jnz     BbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_Width]                
                    
               BbucX2: 
                mov     al,[edi]
                sub     al,[factor_des]
                jnc     Bnormalizada_entrada2
                 xor    al,al
                Bnormalizada_entrada2:

                cmp     al,bl
                ja      Bnormalizado2
                 mov    al,bl
                Bnormalizado2:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor2
                 xor    bl,bl
                Bnormalizado_factor2:

                dec     ecx
                jnz     BbucX2

                mov     eax,[GBL_Width]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz BbucY
            popad        
        }
}

//////////////////////////////////////////////////////////////////////////////
//
//  void HDist32;
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina la maxima intensidad
// Factor<0 -> se difumina la minima intensidad

void HDist32(DWORD* dst,int lin_ini,int lin_fin,char factor)
{
    if(factor==0)
        return;

    dst += GBL_Width*lin_ini;

    char swR;
    char swG;
    char swB;

    if(factor<0)
        //Difuminar Negro
        _asm
        {
            pushad
            neg     [factor]
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            AbucY:

                //hacia la derecha
                mov     bl,255
                mov     ecx,[GBL_Width]
                mov     [swR],bl
                mov     [swG],bl
                mov     [swB],bl 
                    
               AbucX1: 
                //R
                mov     bl,[swR]
                mov     al,[edi+0]
                cmp     al,bl
                jb      AnormalizadoR1
                 mov    al,bl
                AnormalizadoR1:
                mov			[edi+0],al

                add     al,[factor]
                jnc     Anormalizado_factorR1
                 mov    al,255
                Anormalizado_factorR1:
                mov     [swR],al

                //G
                mov     bl,[swG]
                mov     al,[edi+1]
                cmp     al,bl
                jb      AnormalizadoG1
                 mov    al,bl
                AnormalizadoG1:
                mov			[edi+1],al

                add     al,[factor]
                jnc     Anormalizado_factorG1
                 mov    al,255
                Anormalizado_factorG1:
                mov     [swG],al

                //B
                mov     bl,[swB]
                mov     al,[edi+2]
                cmp     al,bl
                jb      AnormalizadoB1
                 mov    al,bl
                AnormalizadoB1:
                mov			[edi+2],al

                add     al,[factor]
                jnc     Anormalizado_factorB1
                 mov    al,255
                Anormalizado_factorB1:
                mov     [swB],al

                //A
                add			edi,4	//saltamos A

                dec     ecx
                jnz     AbucX1

                //hacia la izquierda                
                mov     bl,255
                SUB     edi,4
                mov     [swR],bl
                mov     [swG],bl
                mov     [swB],bl
                mov     ecx,[GBL_Width]                
                    
               AbucX2: 
                //R
                mov     bl,[swR]
                mov     al,[edi+0]
                cmp     al,bl
                jb      AnormalizadoR2
                 mov    al,bl
                AnormalizadoR2:
                mov			[edi+0],al

                add     al,[factor]
                jnc     Anormalizado_factorR2
                 mov    al,255
                Anormalizado_factorR2:
                mov     [swR],al

                //G
                mov     bl,[swG]
                mov     al,[edi+1]
                cmp     al,bl
                jb      AnormalizadoG2
                 mov    al,bl
                AnormalizadoG2:
                mov			[edi+1],al

                add     al,[factor]
                jnc     Anormalizado_factorG2
                 mov    al,255
                Anormalizado_factorG2:
                mov     [swG],al

                //B
                mov     bl,[swB]
                mov     al,[edi+2]
                cmp     al,bl
                jb      AnormalizadoB2
                 mov    al,bl
                AnormalizadoB2:
                mov			[edi+2],al

                add     al,[factor]
                jnc     Anormalizado_factorB2
                 mov    al,255
                Anormalizado_factorB2:
                mov     [swB],al

                //A
                sub			edi,4	//saltamos A

                dec     ecx
                jnz     AbucX2

                mov     eax,[GBL_Width]
                lea     edi,[edi + eax*4 + 4]

            dec edx
            jnz AbucY
            popad        
        }
    else
        //Difuminar Blanco
        _asm
        {
            pushad                        
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            BbucY:

                //hacia la derecha
                xor     bl,bl
                mov     [swR],bl
                mov     [swG],bl
                mov     [swB],bl
                mov     ecx,[GBL_Width]

								ALIGN	16
               BbucX1: 
                //R
                mov     bl,[swR]
                mov     al,[edi+0]
                cmp     al,bl
                ja      BnormalizadoR1
                 mov    al,bl
                BnormalizadoR1:
                mov			[edi+0],al

                sub     al,[factor]
                jnc     Bnormalizado_factorR1
                 xor    al,al
                Bnormalizado_factorR1:
                mov     [swR],al

                //G
                mov     bl,[swG]
                mov     al,[edi+1]
                cmp     al,bl
                ja      BnormalizadoG1
                 mov    al,bl
                BnormalizadoG1:
                mov			[edi+1],al

                sub     al,[factor]
                jnc     Bnormalizado_factorG1
                 xor    al,al
                Bnormalizado_factorG1:
                mov     [swG],al

                //B
                mov     bl,[swB]
                mov     al,[edi+2]
                cmp     al,bl
                ja      BnormalizadoB1
                 mov    al,bl
                BnormalizadoB1:
                mov			[edi+2],al

                sub     al,[factor]
                jnc     Bnormalizado_factorB1
                 xor    al,al
                Bnormalizado_factorB1:
                mov     [swB],al

                //A
                add			edi,4	//saltamos el A

                dec     ecx
                jnz     BbucX1

                //hacia la izquierda
                xor     bl,bl
                SUB     edi,4
                mov     [swR],bl
                mov     [swG],bl
                mov     [swB],bl
                mov     ecx,[GBL_Width]                
                
								ALIGN	16
               BbucX2: 
                //R
                mov     bl,[swR]
                mov     al,[edi+0]
                cmp     al,bl
                ja      BnormalizadoR2
                 mov    al,bl
                BnormalizadoR2:
                mov			[edi+0],al

                sub     al,[factor]
                jnc     Bnormalizado_factorR2
                 xor    al,al
                Bnormalizado_factorR2:
                mov     [swR],al

                //G
                mov     bl,[swG]
                mov     al,[edi+1]
                cmp     al,bl
                ja      BnormalizadoG2
                 mov    al,bl
                BnormalizadoG2:
                mov			[edi+1],al

                sub     al,[factor]
                jnc     Bnormalizado_factorG2
                 xor    al,al
                Bnormalizado_factorG2:
                mov     [swG],al

                //B
                mov     bl,[swB]
                mov     al,[edi+2]
                cmp     al,bl
                ja      BnormalizadoB2
                 mov    al,bl
                BnormalizadoB2:
                mov			[edi+2],al

                sub     al,[factor]
                jnc     Bnormalizado_factorB2
                 xor    al,al
                Bnormalizado_factorB2:
                mov     [swB],al

                //A
                sub			edi,4	//saltamos el A

                dec     ecx
                jnz     BbucX2

                mov     eax,[GBL_Width]
                lea     edi,[edi + eax*4 + 4]

            dec edx
            jnz BbucY
            popad        
        }
}

void Margen320x240(DWORD *dst)
{
	_asm //Margenes
	{
		pushad
		mov edi,[dst]
		mov ecx,320*20
		xor eax,eax
		rep stosd
		add edi,320*200*4
		mov ecx,320*20
		rep stosd
		popad 
	}
	for (DWORD i=0; i<GBL_Height; i++)
	{
		dst[0] = 0;
		dst[GBL_Width-1] = 0;
		dst+=GBL_Width;
	}
}





DWORD* _Dest = NULL;
DWORD* _Bitmap = NULL;

int _ClipX1 = 1;
int _ClipX2 = 319;
int _ClipY1 = 1;
int _ClipY2 = 199;
int _AnchoPant = 320;


int dudx,dvdx;
int X1,deltaX1,X2,deltaX2; 
int U1,deltaU1,V1,deltaV1;

int array1[12],array2[12];
int seccion1,seccion2;
int altura_seccion1,altura_seccion2;
DWORD* DestPtr,*DestPtrClip;

int skip,temp,temp2,longest,tam,altura;

//	int LeftSection();

int LeftSection()
{
__asm{
   push  ecx
   push  edi
   push  esi
   mov   eax,[seccion1]
   mov   esi,DWORD Ptr array1[eax*4]   //-- esi:=vvt1=array1[seccion1]
   mov   edi,DWORD Ptr array1[eax*4-4] //-- edi:=vvt2=array1[seccion1-1]
   mov   ecx,[edi+4]
   sub   ecx,[esi+4]         //-- ecx:=Result=vvt2^.y-vvt1^.y;
   jz    L_DivOkl
      mov  eax,[edi]
      sub  eax,[esi]
      shl  eax,16
      cdq
      idiv ecx
      mov  [deltaX1],eax       //-- DeltaX1:=((vvt2^.x-vvt1^.x) shl 16) div Result;
      mov  eax,[esi]
      shl  eax,16
      mov  [X1],eax            //-- X1:=(vvt1^.x+1) shl 16;

      mov  eax,[edi+8]
      sub  eax,[esi+8]
      shl  eax,8
      cdq
      idiv ecx
      mov  [deltaU1],eax       //-- DeltaU1:=((vvt2^.u-vvt1^.u) shl 8) div Result;
      mov  eax,[esi+8]
      shl  eax,8
      mov  [U1],eax            //-- U1:=(vvt1^.u) shl 8;

      mov  eax,[edi+12]
      sub  eax,[esi+12]
      shl  eax,8
      cdq
      idiv ecx
      mov  [deltaV1],eax       //-- DeltaV1:=((vvt2^.v-vvt1^.v) shl 8) div Result;
      mov  eax,[esi+12]
      shl  eax,8
      mov  [V1],eax            //-- V1:=(vvt1^.v) shl 8;
   L_DivOkl:
   mov   [altura_seccion1],ecx
   mov   eax,ecx
   pop   esi
   pop   edi
   pop   ecx
	}
}


//	int RightSection();

int RightSection()
{
__asm{
   push  ecx
   push  edi
   push  esi
   mov   eax,[seccion2]
   mov   esi,DWORD Ptr array2[eax*4]   //-- esi:=vvt1=array2[seccion2]
   mov   edi,DWORD Ptr array2[eax*4-4] //-- edi:=vvt2=array2[seccion2-1]
   mov   ecx,[edi+4]
   sub   ecx,[esi+4]         //-- ecx:=Result=vvt2^.y-vvt1^.y;
   jz    L_DivOkr
      mov  eax,[edi]
      sub  eax,[esi]
      shl  eax,16
      cdq
      idiv ecx
      mov  [deltaX2],eax       //-- DeltaX2:=((vvt2^.x-vvt1^.x) shl 16) div Result;
      mov  eax,[esi]
      inc  eax
      shl  eax,16
      mov  [X2],eax            //-- X2:=(vvt1^.x+1) shl 16;
   L_DivOkr:
   mov   [altura_seccion2],ecx
   mov   eax,ecx
   pop   esi
   pop   edi
   pop   ecx
	}
}


//	void SetScanLine();

void SetScanLine()
{
__asm{
   mov   [tam],eax
   mov   ecx,eax
   mov   edi,[X1]
   sar   edi,16
   mov   esi,[dudx]
   mov   ebp,[dvdx]
   shl   esi,16
   shl   ebp,16
   mov   [temp],ebp
   mov   [temp2],esi
   mov   eax,[U1]
   mov   edx,[V1]
   rol   eax,16
   rol   edx,16
   xor   ebx,ebx
   mov   bl,al
   mov   bh,dl
   mov   dl,BYTE ptr [dudx+2]
   mov   dh,BYTE ptr [dvdx+2]
   shl   edi,2
   add   edi,[DestPtr]
   mov   ebp,[_Bitmap]
	}
}


//void CalcularGradient();

void CalcularGradient()
{
__asm{
   mov  eax,[ecx+ebp]
   sub  eax,[esi+ebp]
   imul DWORD Ptr [temp]
   mov  ebx,[esi+ebp]
   sub  ebx,[edi+ebp]
   shl  ebx,16
   add  eax,ebx
   mov  edx,eax
   shl  eax,8
   sar  edx,24
   idiv DWORD Ptr [longest]
	}
}


//	void SetNewLine();

void SetNewLine()
//   local L_Return
{
__asm{

   mov  eax,[_AnchoPant]
   shl  eax,2
   add  [DestPtr],eax
   dec  [altura_seccion1]
   jg   L_Alt1Ok
   dec  [seccion1]
   jle  L_Return
   call LeftSection
   cmp  eax,1
   jge  L_Alt1Done
   jmp  L_Return
L_Alt1Ok:
   mov  eax,[deltaX1]
   mov  ebx,[deltaU1]
   mov  edx,[deltaV1]
   add  [X1],eax
   add  [U1],ebx
   add  [V1],edx
L_Alt1Done:
   dec  [altura_seccion2]
   jg   L_Alt2Ok
   dec  [seccion2]
   jle  L_Return
   call RightSection
   cmp  eax,1
   jge  L_Alt2Done
   jmp  L_Return
L_Alt2Ok:
   mov  eax,[deltaX2]
   add  [X2],eax
L_Alt2Done:
   ret
L_Return:
   xor ecx,ecx
	}
}


//	void TexTri(DWORD* Dest,VerTex* vv1,VerTex* vv2,VerTex* vv3,DWORD* Bitmap);

void TexTri(DWORD* Dest,VerTex* vv1,VerTex* vv2,VerTex* vv3,DWORD* Bitmap)
{
__asm{
   push ebp

   //-- Cargar y Ordenar Vectores a Puntos--
   mov   eax,[esi+4]  //--vv1^.y<=vv2^.y
   cmp   eax,[edi+4]
   jle   L_OrdOk1
   xchg esi,edi
L_OrdOk1:
   mov   eax,[edi+4]  //--vv2^.y<=vv3^.y
   cmp   eax,[ecx+4]
   jle   L_OrdOk2
   xchg edi,ecx
L_OrdOk2:
   mov   eax,[esi+4]  //--vv1^.y<=vv2^.y
   cmp   eax,[edi+4]
   jle   L_OrdOk3
   xchg esi,edi
L_OrdOk3:
   mov   eax,[_ClipY1]
   mov   ebx,[_ClipY2]
   cmp   [ecx+4],eax
   jl    L_TexSalir
   cmp   [esi+4],ebx
   jg    L_TexSalir

   //--Ini.variables--
   mov   eax,[edi+4]
   mov   ebx,[ecx+4]
   sub   eax,[esi+4]
   shl   eax,16
   cdq
   sub   ebx,[esi+4]
   jz    L_TexSalir     //-- if altura=0 then goto TexSalir;
   mov   [altura],ebx  //-- altura:=vv3^.y-vv1^.y;
   idiv  ebx
   mov   ebx,[ecx]
   mov   [temp],eax    //-- temp:=((vv2^.y-vv1^.y) shl 16) div altura;
   sub   ebx,[esi]
   imul  ebx
   mov   ebx,[esi]
   sub   ebx,[edi]
   shl   ebx,16
   add   eax,ebx
   jz    L_TexSalir     //-- if longest=0 then goto TexSalir;
   mov   [longest],eax //-- longest:=temp*(vv3^.x-vv1^.x)+((vv1^.x-vv2^.x) shl 16);
   jns   L_LongestPos

   //--Longest negativo--
   mov   DWORD Ptr [array2+0],ecx  //-- array2[0]:=vv3;
   mov   DWORD Ptr [array2+4],edi  //-- array2[1]:=vv2;
   mov   DWORD Ptr [array2+8],esi  //-- array2[2]:=vv1;
   mov   DWORD Ptr [seccion2],2
   mov   DWORD Ptr [array1+0],ecx  //-- array1[0]:=vv3;
   mov   DWORD Ptr [array1+4],esi  //-- array1[1]:=vv1;
   mov   DWORD Ptr [seccion1],1
   cmp   [longest],-1000h  //-- evitar Div 0
   jb    L_LongestOkNeg
   mov   [longest],-1000h
L_LongestOkNeg:
   call  LeftSection
   cmp   eax,0
   jle   L_TexSalir
   call  RightSection
   cmp   eax,0
   jg    L_IniDone
   dec   [seccion2]
   call  RightSection
   cmp   eax,0
   jle   L_TexSalir
   jmp   L_IniDone

L_LongestPos:
   //--Longest positivo--
   mov   DWORD Ptr [array1+0],ecx  //-- array1[0]:=vv3;
   mov   DWORD Ptr [array1+4],edi  //-- array1[1]:=vv2;
   mov   DWORD Ptr [array1+8],esi  //-- array1[2]:=vv1;
   mov   DWORD Ptr [seccion1],2
   mov   DWORD Ptr [array2+0],ecx  //-- array2[0]:=vv3;
   mov   DWORD Ptr [array2+4],esi  //-- array2[1]:=vv1;
   mov   DWORD Ptr [seccion2],1
   cmp   [longest],1000h //-- evitar Div 0
   jg    L_LongestOkPos
   mov   [longest],1000h
L_LongestOkPos:
   call  RightSection
   cmp   eax,0
   jle   L_TexSalir
   call  LeftSection
   cmp   eax,0
   jg    L_IniDone
   dec   [seccion1]
   call  LeftSection
   cmp   eax,0
   jle   L_TexSalir

L_IniDone:
   //--Calcula los gradientes de u y v--
   mov ebp,8
   call CalcularGradient
   mov  [dudx],eax
   mov ebp,12
   call CalcularGradient
   mov  [dvdx],eax

   //--calcula punteros de destino y Clipping--
   mov   eax,[esi+4]
   mov   ebx,[_ClipY1]
   imul  [_AnchoPant]
   imul  ebx,[_AnchoPant]
   shl   eax,2
   shl   ebx,2
   add   eax,[_Dest]
   add   ebx,[_Dest]
   mov   [DestPtr],eax
   mov   [DestPtrClip],ebx
   //-- DWORD(DestPtr):=DWORD(Dest)+(vv1^.y*AnchoPant);
   //-- DWORD(DestPtrClip):=DWORD(Dest)+(ClipY1*AnchoPant);

   mov   edx,[ecx+4]
   sub   edx,[_ClipY2]
   jle   L_ClipY2Done
   sub  [altura],edx
L_ClipY2Done:
   //-- if vv3^.y>ClipY2 then Altura-:=(vv3^.y-ClipY2); -- Clipping Y2
   mov   edx,ecx
   mov   ecx,[altura]
   mov   eax,[_ClipX1]
   cmp   ecx,0
   jle   L_TexSalir
   mov   ebx,[_ClipX2]
   cmp   [esi],eax
   jl    L_OutterClipX
   cmp   [esi],ebx
   jg    L_OutterClipX
   cmp   [edi],eax
   jl    L_OutterClipX
   cmp   [edi],ebx
   jg    L_OutterClipX
   cmp   [edx],eax
   jl    L_OutterClipX
   cmp   [edx],ebx
   jg    L_OutterClipX

   //--Dibuja el Triangulo con Cliping Vertical
L_Outter:
   mov   ebx,[DestPtrClip]
   mov   eax,[X2]
   cmp   ebx,[DestPtr]  ;-- Clipping Y1
   ja    L_Done
   and   eax,0ffff0000h
   sub   eax,[X1]
   sar   eax,16
   jle   L_Done

      //--Dibuja el ScanLine--
      push  ecx
      call SetScanLine
      align 4
      L_inner:
         mov   esi,[ebp+ebx*4]
         add   edx,[temp]
         adc   bh,dh
         add   eax,[temp2]
         adc   bl,dl
         mov   [edi],esi
         add   edi,4
         dec   ecx
         jnz   L_inner
      pop   ecx

   L_Done:
   call  SetNewLine
   dec   ecx
   jns   L_Outter
   jmp   L_TexSalir

   //--Dibuja el Triangulo con Cliping Vertical y Horizontal
L_OutterClipX:
   mov   eax,[X2]
   mov   ebx,[DestPtrClip]
   and   eax,0ffff0000h
   sub   eax,[X1]
   sar   eax,16
   jle   L_DoneClipX
   cmp   ebx,[DestPtr]  //-- Clipping Y1
   ja    L_DoneClipX
   mov   ebx,[_ClipX1]
   mov   edx,[X1]
   mov   [skip],ebx
   sar   edx,16
   mov   ebx,[X1]
   add   edx,eax
   sar   ebx,16
   sub   edx,[_ClipX2]
   jle   L_ClipedX2
   dec   edx
   sub   eax,edx		//-- Clipping X2
   jng   L_DoneClipX
L_ClipedX2:
   sub   [skip],ebx     //-- Clipping X1
   jle   L_ClipedX1
   mov   ebx,[skip]
   sub   eax,ebx
   jng  L_DoneClipX
L_ClipedX1:
   cmp   eax,0
   jle   L_DoneClipX

      //--Dibuja ScanLine--
      push  ecx
      call  SetScanLine
      mov   ecx,[skip]
      cmp   ecx,0
      jle   L_skipped
      lea   edi,[edi+ecx*4]
      align 4
      L_skipper:
         add edx,[temp]
         adc bh,dh
         add eax,esi
         adc bl,dl
         dec ecx
         jnz L_skipper
      L_skipped:
      mov   ecx,[tam]
      align 4
      L_InnerClipX:
         mov   esi,[ebp+ebx*4]
         add   edx,[temp]
         adc   bh,dh
         add   eax,[temp2]
         adc   bl,dl
         mov   [edi],esi
         add   edi,4
         dec   ecx
         jnz   L_InnerClipX
      pop   ecx

L_DoneClipX:
   call  SetNewLine
   dec   ecx
   jns   L_OutterClipX

L_TexSalir:
   pop ebp
	}
}

void PutPartA(char* dest,char* PartBitMapA,long px,long py,long t)
{
	long x8,y8,y;
	long Xini,Xlong;

	px -= t>>1;
	py -= t>>1;

	if(t<4) t=4;
	if(t>63) t=63;
	if(px<0 || py <0 || px+t>=320 || py+t>=240)
		return;

	_asm
	{
	  pushad
	  //--*Clip Total*--
	  mov   eax,[px]    
	  cmp   eax,320
	  jge   exit
	  add   eax,[t]
	  cmp   eax,0
	  jl    exit
	  mov   eax,[py]
	  cmp   eax,240
	  jge   exit
	  add   eax,[t]
	  cmp   eax,0
	  jl    exit

	  //--*Calc. Delta*--
	  mov   eax,64*256
	  xor   dx,dx
	  div   WORD Ptr[t]
	  mov   ecx,eax      //ecx:delta

	  //--*Clip Parcial Y*--
	  //ClipY1
	  mov   eax,[py]
	  mov   [y8],0
	  mov   ebx,[t]       //ebx:#y
	  cmp   eax,0
	  jge   ClippedY1
	   add  ebx,eax
	   mov  [py],0
	   xor  edx,edx
	   imul ecx
	   sub  [y8],eax //eax negativo
	  ClippedY1:
	  //ClipY2
	  mov   eax,[py]
	  add   eax,[t]
	  sub   eax,240
	  jl    ClippedY2
	   sub  ebx,eax	
	  ClippedY2:

	  //Inic.
	  mov   edi,[py]
	  mov   edx,ecx       //edx:delta
	  lea   edi,[edi+edi*4]
	  shl   edi,6
	  add   edi,[px]
	  add   edi,[dest]
	  mov   esi,[PartBitMapA]

	  mov   eax,[t]
	  mov   [Xlong],eax
	  mov   [Xini],0

	 //align 4
	 YBuc:
	  mov   eax,[Xini]
	  mov   [x8],eax
	  mov   eax,[y8]
	  shr   eax,8
	  mov   ecx,[Xlong]     //ecx:#x
	  shl   eax,6
	  mov   [y],eax
	 //align 4
	  XBuc:
	   mov  eax,[x8]
	   shr  eax,8
	   add  eax,[y]
	   add  [x8],edx
	   mov  al,[esi+eax]
	   add  al,[edi]
	   jnc Ok
		mov al,255
	   Ok:
	   mov  [edi],al
	   inc  edi
	   dec  ecx
	   jnz  XBuc
	  sub   edi,[t]
	  add   [y8],edx
	  add   edi,320
	  dec   ebx
	  jnz   YBuc

	  exit:
	  popad
	}
}

void PutPartRGB(DWORD* dest,DWORD* PartBitMapRGB,long px,long py,long t)
{
	long x8,x,y8,y;
	long Xini,Xlong,Xlong4;

	if(t<3) t=3;
	if(t>200) t=200;

	px -= t>>1;
	py -= t>>1;

	_asm
	{
	  pushad
	  //--*Clip Total*--
	  mov   eax,[px]    
	  cmp   eax,320
	  jge		salir
	  add   eax,[t]

	  cmp   eax,0
	  jle		salir
	  mov   eax,[py]
	  cmp   eax,240
	  jge		salir
	  add   eax,[t]
	  cmp   eax,0
	  jle		salir

	  //--*Calc. Delta*--
	  mov   eax,64*256
	  xor   dx,dx
	  div   WORD Ptr[t]
	  mov   ecx,eax      //ecx:delta (se necesita en clipping)

		//--*Clip Parcial X*--
		xor		eax,eax
		mov	  ebx,[t]
		mov		[Xini],eax
		mov		[x8],eax
		mov		[Xlong],ebx
		//ClipX2
		sub   eax,[px]
		jl		ClippedX1
		 sub	[Xlong],eax
		 xor  edx,edx
		 add  [Xini],eax 
	   imul ecx
shr	eax,8
	   add  [x8],eax 
		 align 16
	  ClippedX1:
	  //ClipX2
	  mov		eax,320
		sub		eax,[Xlong]
		sub		eax,[px]
		jg		ClippedX2
		 add	[Xlong],eax
		 align 16
	  ClippedX2:

		mov	eax,[Xlong]
		shl	eax,2
		mov	[Xlong4],eax


	  //--*Clip Parcial Y*--
	  //ClipY1
	  mov   eax,[py]
	  mov   [y8],0
	  mov   ebx,[t]       //ebx:#y
	  cmp   eax,0
	  jge   ClippedY1
	   add  ebx,eax
	   mov  [py],0
	   xor  edx,edx
	   imul ecx
	   sub  [y8],eax //eax negativo
		 align 16
	  ClippedY1:
	  //ClipY2
	  mov   eax,[py]
	  add   eax,[t]
	  sub   eax,240
	  jl    ClippedY2
	   sub  ebx,eax
		 align 16
	  ClippedY2:

	  //Inic.
	  mov   edi,[py]
	  mov   edx,ecx       //edx:delta
	  lea   edi,[edi+edi*4]
	  shl   edi,6
	  add   edi,[px]
	  shl	edi,2
	  add   edi,[dest]
	  mov   esi,[PartBitMapRGB]

		mov		eax,[x8]
//shr	eax,8
		mov		ecx,[Xini]
		lea		esi,[esi+eax*4]
		lea		edi,[edi+ecx*4]
		
	  align 4
	 YBuc: 
		mov   eax,[x8]
//shr		eax,8
		mov   [x],eax
	  mov   eax,[y8]
		shr   eax,2
	  mov   ecx,[Xlong]     //ecx:#x
		and		eax,0x0ffffffc0
	  mov   [y],eax
	   align 16
	  XBuc:
	   mov  eax,[x]
	   shr  eax,8
	   add  eax,[y]
	   add  [x],edx

//shr	eax,8
	   mov  eax,[esi+eax*4] 
   		//R
	   add  al,[edi]
	   jnc OkB
		mov al,255
	   OkB:
		//G
	   add  ah,[edi+1]
	   jnc OkG
		mov ah,255
	   OkG:
		//R
	   rol	eax,16
	   add  al,[edi+2]
	   jnc OkR
		mov al,255
	   OkR:	   
	   rol	eax,16
	   mov  [edi],eax
	   add  edi,4
	   dec  ecx
	   jnz  XBuc
	  sub   edi,[Xlong4] //[t]
	  add   [y8],edx
	  add   edi,320*4
	  dec   ebx
	  jnz   YBuc

	 salir:
	  popad
	}
}


/*

void PutPartRGB(DWORD* dest,DWORD* PartBitMapRGB,long px,long py,long t)
{
	long x8,y8,y;
	long Xini,Xlong;

	px -= t>>1;
	py -= t>>1;

	if(t<4) t=4;
	if(t>128) t=128;
	if(px<0 || py <0 || px+t>=320 || py+t>=240)
		return;

	_asm
	{
	  pushad
	  //--*Clip Total*--
	  mov   eax,[px]    
	  cmp   eax,320
	  jge   exit
	  add   eax,[t]

	  cmp   eax,0
	  jl    exit
	  mov   eax,[py]
	  cmp   eax,240
	  jge   exit
	  add   eax,[t]
	  cmp   eax,0
	  jl    exit

	  //--*Calc. Delta*--
	  mov   eax,64*256
	  xor   dx,dx
	  div   WORD Ptr[t]
	  mov   ecx,eax      //ecx:delta

		//--*Clip Parcial X*--


	  //--*Clip Parcial Y*--
	  //ClipY1
	  mov   eax,[py]
	  mov   [y8],0
	  mov   ebx,[t]       //ebx:#y
	  cmp   eax,0
	  jge   ClippedY1
	   add  ebx,eax
	   mov  [py],0
	   xor  edx,edx
	   imul ecx
	   sub  [y8],eax //eax negativo
	  ClippedY1:
	  //ClipY2
	  mov   eax,[py]
	  add   eax,[t]
	  sub   eax,240
	  jl    ClippedY2
	   sub  ebx,eax	
	  ClippedY2:

	  //Inic.
	  mov   edi,[py]
	  mov   edx,ecx       //edx:delta
	  lea   edi,[edi+edi*4]
	  shl   edi,6
	  add   edi,[px]
	  shl	edi,2
	  add   edi,[dest]
	  mov   esi,[PartBitMapRGB]

	  shl	[t],2
	  mov   eax,[t]	  
	  mov   [Xlong],eax
	  shr	[Xlong],2
	  mov   [Xini],0

	 //align 4
	 YBuc:
	  mov   eax,[Xini]
	  mov   [x8],eax
	  mov   eax,[y8]
	  shr   eax,8
	  mov   ecx,[Xlong]     //ecx:#x
	  shl   eax,6
	  mov   [y],eax
	 //align 4
	  XBuc:
	   mov  eax,[x8]
	   shr  eax,8
	   add  eax,[y]
	   add  [x8],edx

	   mov  eax,[esi+eax*4] 
   		//R
	   add  al,[edi]
	   jnc OkB
		mov al,255
	   OkB:
		//G
	   add  ah,[edi+1]
	   jnc OkG
		mov ah,255
	   OkG:
		//R
	   rol	eax,16
	   add  al,[edi+2]
	   jnc OkR
		mov al,255
	   OkR:	   
	   rol	eax,16
	   mov  [edi],eax
	   add  edi,4
	   dec  ecx
	   jnz  XBuc
	  sub   edi,[t]
	  add   [y8],edx
	  add   edi,320*4
	  dec   ebx
	  jnz   YBuc

	  exit:
	  popad
	}
}
*/

void PutPartRGBNoClip(DWORD* dest,DWORD* PartBitMapRGB,long px,long py,long t)
{
	long x8,y8,y;
	long Xini,Xlong;

	px -= t>>1;
	py -= t>>1;

	if(t<4) t=4;
	if(t>128) t=128;
	if(/*px<0 ||*/ py <0 || px+t>=320 || py+t>=240)
		return;

	_asm
	{
	  pushad
	  //--*Clip Total*--
	  mov   eax,[px]    
	  cmp   eax,320
	  jge   exit
	  add   eax,[t]
	  cmp   eax,0
	  jl    exit
	  mov   eax,[py]
	  cmp   eax,240
	  jge   exit
	  add   eax,[t]
	  cmp   eax,0
	  jl    exit

	  //--*Calc. Delta*--
	  mov   eax,64*256
	  xor   dx,dx
	  div   WORD Ptr[t]
	  mov   ecx,eax      //ecx:delta

		//--*Clip Parcial X*--


	  //--*Clip Parcial Y*--
	  //ClipY1
	  mov   eax,[py]
	  mov   [y8],0
	  mov   ebx,[t]       //ebx:#y
	  cmp   eax,0
	  jge   ClippedY1
	   add  ebx,eax
	   mov  [py],0
	   xor  edx,edx
	   imul ecx
	   sub  [y8],eax //eax negativo
	  ClippedY1:
	  //ClipY2
	  mov   eax,[py]
	  add   eax,[t]
	  sub   eax,240
	  jl    ClippedY2
	   sub  ebx,eax	
	  ClippedY2:

	  //Inic.
	  mov   edi,[py]
	  mov   edx,ecx       //edx:delta
	  lea   edi,[edi+edi*4]
	  shl   edi,6
	  add   edi,[px]
	  shl	edi,2
	  add   edi,[dest]
	  mov   esi,[PartBitMapRGB]

	  shl	[t],2
	  mov   eax,[t]	  
	  mov   [Xlong],eax
	  shr	[Xlong],2
	  mov   [Xini],0

	 //align 4
	 YBuc:
	  mov   eax,[Xini]
	  mov   [x8],eax
	  mov   eax,[y8]
	  shr   eax,8
	  mov   ecx,[Xlong]     //ecx:#x
	  shl   eax,6
	  mov   [y],eax
	 //align 4
	  XBuc:
	   mov  eax,[x8]
	   shr  eax,8
	   add  eax,[y]
	   add  [x8],edx

	   mov  eax,[esi+eax*4] 
   		//R
	   add  al,[edi]
	   jnc OkB
		mov al,255
	   OkB:
		//G
	   add  ah,[edi+1]
	   jnc OkG
		mov ah,255
	   OkG:
		//R
	   rol	eax,16
	   add  al,[edi+2]
	   jnc OkR
		mov al,255
	   OkR:	   
	   rol	eax,16
	   mov  [edi],eax
	   add  edi,4
	   dec  ecx
	   jnz  XBuc
	  sub   edi,[t]
	  add   [y8],edx
	  add   edi,320*4
	  dec   ebx
	  jnz   YBuc

	  exit:
	  popad
	}
}

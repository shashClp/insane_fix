#include <math.h>
#include <windows.h>
#include <view.h>
#include <mem.h>
#include <rgb.h>
#include "blurs.h"

DWORD *BLURS_Inc;
DWORD *BLURS_IncRadial;
DWORD *BLURS_IncRadialI;
float BLURS_Angle;
float BLURS_Angle2;
DWORD BLURS_Pitch;

void BLURS_GenerarTunnel (float cenX,float cenY,float dist)
{
DWORD i,j;
DWORD *Inc;

  Inc = BLURS_Inc;
  for (j=1; j<GBL_Height-1; j++)
  {
    for (i=1; i<GBL_Width-1; i++)
    {
      float a = ((float)i)-cenX;
      float b = ((float)j)-cenY;
      
      float mod = (float)sqrt((a*a)+(b*b));
      if (mod < 0.01) 
		  mod = (float)0.01;
      float x = a/mod;
      float y = b/mod;
      x -= (float)(a/dist);
      y -= (float)(b/dist);

      Inc[0] = (DWORD)(256.0*(x+i));
      Inc[1] = (DWORD)(256.0*(y+j));

      if (Inc[0] >= GBL_Width*256) Inc[0] = (GBL_Width-1)*256;
      if (Inc[1] >= GBL_Height*256) Inc[1] = (GBL_Height-1)*256;
      if (Inc[0] < 0) Inc[0] = 0;
      if (Inc[1] < 0) Inc[1] = 0;

      Inc+=2;
    }
  }
}

void BLURS_GenerarTunnelWave(float cenX,float cenY,float dist,float fase)
{
DWORD i,j;
DWORD *Inc;

  Inc = BLURS_Inc;
  for (j=1; j<GBL_Height-1; j++)
  {
    for (i=1; i<GBL_Width-1; i++)
    {
      float a = ((float)i)-cenX;
      float b = ((float)j)-cenY;
      a+=0.5;
      b+=0.5;
      
      float mod = (float)sqrt((a*a)+(b*b));
      if (mod < 0.01) 
		  mod = (float)0.01;
      float x = a/mod * cos((b/5)+fase);
      float y = b/mod * sin((a/5)+fase);
      x -= (float)(a/dist);
      y -= (float)(b/dist);

      Inc[0] = (DWORD)(256.0*(x+i));
      Inc[1] = (DWORD)(256.0*(y+j));

      if (Inc[0] >= GBL_Width*256) Inc[0] = (GBL_Width-1)*256;
      if (Inc[1] >= GBL_Height*256) Inc[1] = (GBL_Height-1)*256;
      if (Inc[0] < 0) Inc[0] = 0;
      if (Inc[1] < 0) Inc[1] = 0;

      Inc+=2;
    }
  }
}

void BLURS_GenerarTunnelRadialBlur (float cenX,float cenY,float dist)
{
  DWORD *Inc = BLURS_IncRadial;
  for (long j=1; j<(int)GBL_Height-1; j++)
  {
    for (long i=1; i<(int)GBL_Width-1; i++)
    {
      float a = ((float)i)-cenX;
      float b = ((float)j)-cenY;
      
      float mod = (float)sqrt((a*a)+(b*b));
      if (mod < 0.0001) 
		  mod = (float)0.0001;
      float x = -dist*a/mod;
      float y = -dist*b/mod;

      Inc[0] = (DWORD)(256.0*(x+i));
      Inc[1] = (DWORD)(256.0*(y+j));

      if (Inc[0] >= GBL_Width*256) Inc[0] = (GBL_Width-1)*256;
      if (Inc[1] >= GBL_Height*256) Inc[1] = (GBL_Height-1)*256;
      if (Inc[0] < 0) Inc[0] = 0;
      if (Inc[1] < 0) Inc[1] = 0;

      Inc+=2;
    }
  }
}

void BLURS_GenerarTunnelRadialBlurInv (float cenX,float cenY,float dist)
{
  DWORD *Inc = BLURS_IncRadialI;
  for (long j=1; j<(int)GBL_Height-1; j++)
  {
    for (long i=1; i<(int)GBL_Width-1; i++)
    {
      float a = ((float)i)-cenX;
      float b = ((float)j)-cenY;
      
      float mod = (float)sqrt((a*a)+(b*b));
      if (mod < 0.0001) 
		  mod = (float)0.0001;
      float x = +dist*a/mod;
      float y = +dist*b/mod;

      Inc[0] = (DWORD)(256.0*(x+i));
      Inc[1] = (DWORD)(256.0*(y+j));

      if (Inc[0] >= GBL_Width*256) Inc[0] = (GBL_Width-1)*256;
      if (Inc[1] >= GBL_Height*256) Inc[1] = (GBL_Height-1)*256;
      if (Inc[0] < 0) Inc[0] = 0;
      if (Inc[1] < 0) Inc[1] = 0;

      Inc+=2;
    }
  }
}

/////////////////////////////////////////////////////////////////////
//	void BLURS_RenderCentro(DWORD* imgIn,int x,int y);

void BLURS_RenderCentro(DWORD* imgIn,float x,float y,float factor)
{
  DWORD i,j;
  DWORD *In,*Inxy,*pOut;
  DWORD *Inc;
  DWORD pixel;

  BLURS_Inc = (DWORD *) GetMem ((320-2)*(240-2+1)*2*4);
  BLURS_Pitch = 320*4;
  BLURS_GenerarTunnel(x,y,factor);

  Inc = BLURS_Inc;  
  In=imgIn;
  pOut=GBL_Bits;
  pOut+=321;

  for (j=0; j<GBL_Height-2; j++)
  {
    for (i=0; i<GBL_Width-2; i++)
    {
      unsigned long a_X = *Inc++;
      unsigned long a_Y = *Inc++;
	  Inxy = In+((GBL_Width*(a_Y >> 8))+(a_X >> 8));	  
	  unsigned long t11 = *(Inxy);
      unsigned long t12 = *(Inxy + 1);
      unsigned long t21 = *(Inxy + 320);
      unsigned long t22 = *(Inxy + 321);
	  a_X &= 255;
	  a_Y &= 255;

	  __asm{
          pushad
          xor   eax,eax	
          mov   [pixel],eax

          //R
          movzx ecx,byte ptr[t11+2]
          movzx eax,byte ptr[t12+2] 
		  movzx edx,byte ptr[t21+2]
		  sub   eax,ecx
		  movzx ebx,byte ptr[t22+2]
		  mov   ecx,[a_X] //factor X
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11+2]
          add   ecx,edx	//ecx = t1	 
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21+2]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx //eax = t2
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx            		 
          mov   byte ptr[pixel+2],al

          //G
          movzx ecx,byte ptr[t11+1]
          movzx eax,byte ptr[t12+1] 
		  movzx edx,byte ptr[t21+1]
		  sub   eax,ecx
		  movzx ebx,byte ptr[t22+1]		  
		  mov   ecx,[a_X] //factor X          
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11+1]
          add   ecx,edx
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21+1]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx                  
          mov   byte ptr[pixel+1],al

          //B  
          movzx ecx,byte ptr[t11]
          movzx eax,byte ptr[t12] 
		  movzx edx,byte ptr[t21]
		  sub   eax,ecx		  
		  movzx ebx,byte ptr[t22]
		  mov   ecx,[a_X] //factor X          
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11]
          add   ecx,edx
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx                  
          mov   byte ptr[pixel],al

          popad
      };
	  *(pOut++)=pixel;	
    }
    pOut+=2;
  }
}

/////////////////////////////////////////////////////////////////////
//	void BLURS_Render(DWORD* imgIn);

void BLURS_Render(DWORD* imgIn)
{
  DWORD i,j;
  DWORD *In,*Inxy,*pOut;
  DWORD *Inc;
  DWORD pixel;

  Inc = BLURS_Inc;  
  In=imgIn;
  pOut=GBL_Bits;
  pOut+=321;

  for (j=0; j<GBL_Height-2; j++)
  {
    for (i=0; i<GBL_Width-2; i++)
    {
      unsigned long a_X = *Inc++;
      unsigned long a_Y = *Inc++;
	  Inxy = In+((GBL_Width*(a_Y >> 8))+(a_X >> 8));	  
	  unsigned long t11 = *(Inxy);
      unsigned long t12 = *(Inxy + 1);
      unsigned long t21 = *(Inxy + 320);
      unsigned long t22 = *(Inxy + 321);
	  a_X &= 255;
	  a_Y &= 255;

	  __asm{
          pushad
          xor   eax,eax	
          mov   [pixel],eax

          //R
          movzx ecx,byte ptr[t11+2]
          movzx eax,byte ptr[t12+2] 
		  movzx edx,byte ptr[t21+2]
		  sub   eax,ecx
		  movzx ebx,byte ptr[t22+2]
		  mov   ecx,[a_X] //factor X
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11+2]
          add   ecx,edx	//ecx = t1	 
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21+2]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx //eax = t2
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx            		 
          mov   byte ptr[pixel+2],al

          //G
          movzx ecx,byte ptr[t11+1]
          movzx eax,byte ptr[t12+1] 
		  movzx edx,byte ptr[t21+1]
		  sub   eax,ecx
		  movzx ebx,byte ptr[t22+1]		  
		  mov   ecx,[a_X] //factor X          
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11+1]
          add   ecx,edx
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21+1]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx                  
          mov   byte ptr[pixel+1],al

          //B  
          movzx ecx,byte ptr[t11]
          movzx eax,byte ptr[t12] 
		  movzx edx,byte ptr[t21]
		  sub   eax,ecx		  
		  movzx ebx,byte ptr[t22]
		  mov   ecx,[a_X] //factor X          
		  sub   ebx,edx  
		  
          imul  ecx
          xchg  eax,ecx
          sar   ecx,8 //parte entera
          imul  ebx
		  movzx	edx,byte ptr[t11]
          add   ecx,edx
          sar   eax,8 //parte entera
		  movzx edx,byte ptr[t21]
          mov   ebx,[a_Y] //factor Y          
		  add	eax,edx
          sub   eax,ecx          
          imul  ebx
          sar   eax,8 //parte entera
          add   eax,ecx                  
          mov   byte ptr[pixel],al

          popad
      };
	  *(pOut++)=pixel;	
    }
    pOut+=2;
  }
}

/////////////////////////////////////////////////////////////////////
//	void BLURS_RenderMMX(DWORD* imgIn);

void BLURS_RenderMMX(DWORD* imgIn)
{
DWORD i,j,x,y;
DWORD *In,*Inxy,*pOut;
DWORD *Inc;

 Inc = BLURS_Inc;  
  In=imgIn;
  pOut=GBL_Bits;

  _asm pxor mm5,mm5;

  for (j=0; j<GBL_Height-2; j++)
  {
    for (i=0; i<GBL_Width-2; i++)
    {
      x = *Inc++;
      y = *Inc++;
      //Inxy = In+(/*(BLURS_View->pitch*(y >> 8))+*/(x >> 8));
	  Inxy = In+((320*(y >> 8))+(x >> 8));
	  //Inxy = In+((x >> 8));
    
      _asm {        
        mov esi,Inxy
        mov edi,BLURS_Pitch
        mov eax,x
        mov ecx,y
        //and eax,0xff
        //and ecx,0xff
        mov ah,al
        mov ch,cl
        shl eax,8
        shl ecx,8
        mov al,ah
        mov cl,ch        

        mov ebx,eax
        mov edx,ecx
        xor eax,-1
        xor ecx,-1                
        
        movd       mm1,[esi-4]
        punpcklbw  mm1,mm5
        movd       mm6,eax
        punpcklbw  mm6,mm5
        pmullw     mm1,mm6

        movd       mm2,[esi+4]
        punpcklbw  mm2,mm5
        movd       mm6,ebx
        punpcklbw  mm6,mm5
        pmullw     mm2,mm6

        movd       mm3,[esi-320*4]
        punpcklbw  mm3,mm5
        movd       mm6,ecx
        punpcklbw  mm6,mm5
        pmullw     mm3,mm6

        movd       mm4,[esi+320*4]
        punpcklbw  mm4,mm5
        movd       mm6,edx
        punpcklbw  mm6,mm5
        pmullw     mm4,mm6

        movd       mm0,[esi]
        punpcklbw  mm0,mm5
        psrlw      mm1,8
        psrlw      mm2,8
        psrlw      mm3,8
        psrlw      mm4,8        
        paddw      mm1,mm2
        paddw      mm3,mm4
        paddw      mm0,mm0
        paddw      mm1,mm3

        paddw      mm0,mm1
        psrlw      mm0,2
        mov        edi,pOut
        packuswb   mm0,mm5        
        add        pOut,4
        movd       [edi],mm0                        
      }      
    }
    pOut+=2;
  }

  _asm emms;
}

/////////////////////////////////////////////////////////////////////
//
//	void BLURS_CalcOffs(DWORD* pixel,DWORD* t11,DWORD* t12,DWORD* t21,DWORD* t22,DWORD* a_X,DWORD* a_Y);
//	usado por void BLURS_RenderRadial(DWORD* imgIn);

DWORD BLURS_CalcOffs(DWORD* In,DWORD a_X,DWORD a_Y)
{
	DWORD pixel;
	DWORD* Inxy = In+((GBL_Width*(a_Y >> 8))+(a_X >> 8));
	unsigned long t11 = *(Inxy);
  unsigned long t12 = *(Inxy + 1);
  unsigned long t21 = *(Inxy + 320);
  unsigned long t22 = *(Inxy + 321);
	a_X &= 255;
	a_Y &= 255;
	__asm{
      pushad
      xor   eax,eax	
      mov   [pixel],eax

      //R
      movzx ecx,byte ptr[t11+2]
      movzx eax,byte ptr[t12+2] 
	  movzx edx,byte ptr[t21+2]
	  sub   eax,ecx
	  movzx ebx,byte ptr[t22+2]
	  mov   ecx,[a_X] //factor X
	  sub   ebx,edx  
	  
      imul  ecx
      xchg  eax,ecx
      sar   ecx,8 //parte entera
      imul  ebx
	  movzx	edx,byte ptr[t11+2]
      add   ecx,edx	//ecx = t1	 
      sar   eax,8 //parte entera
	  movzx edx,byte ptr[t21+2]
      mov   ebx,[a_Y] //factor Y          
	  add	eax,edx //eax = t2
      sub   eax,ecx          
      imul  ebx
      sar   eax,8 //parte entera
      add   eax,ecx            		 
      mov   byte ptr[pixel+2],al

      //G
      movzx ecx,byte ptr[t11+1]
      movzx eax,byte ptr[t12+1] 
	  movzx edx,byte ptr[t21+1]
	  sub   eax,ecx
	  movzx ebx,byte ptr[t22+1]		  
	  mov   ecx,[a_X] //factor X          
	  sub   ebx,edx  
	  
      imul  ecx
      xchg  eax,ecx
      sar   ecx,8 //parte entera
      imul  ebx
	  movzx	edx,byte ptr[t11+1]
      add   ecx,edx
      sar   eax,8 //parte entera
	  movzx edx,byte ptr[t21+1]
      mov   ebx,[a_Y] //factor Y          
	  add	eax,edx
      sub   eax,ecx          
      imul  ebx
      sar   eax,8 //parte entera
      add   eax,ecx                  
      mov   byte ptr[pixel+1],al

      //B  
      movzx ecx,byte ptr[t11]
      movzx eax,byte ptr[t12] 
	  movzx edx,byte ptr[t21]
	  sub   eax,ecx		  
	  movzx ebx,byte ptr[t22]
	  mov   ecx,[a_X] //factor X          
	  sub   ebx,edx  
	  
      imul  ecx
      xchg  eax,ecx
      sar   ecx,8 //parte entera
      imul  ebx
	  movzx	edx,byte ptr[t11]
      add   ecx,edx
      sar   eax,8 //parte entera
	  movzx edx,byte ptr[t21]
      mov   ebx,[a_Y] //factor Y          
	  add	eax,edx
      sub   eax,ecx          
      imul  ebx
      sar   eax,8 //parte entera
      add   eax,ecx                  
      mov   byte ptr[pixel],al

      popad
  };
  return pixel;
}

/////////////////////////////////////////////////////////////////////
//	void BLURS_RenderRadial(DWORD* imgIn,long fact);

void BLURS_RenderRadialADD(DWORD* imgIn,long cenX,long cenY,long fact)
{
  long j;

  //2o cradrante
  DWORD *pInc2 = BLURS_IncRadial + (((GBL_Width-2)*cenY - (GBL_Width-cenX-1))<<1);
  DWORD *pOut2 = GBL_Bits + GBL_Width*(cenY+1) - (GBL_Width-cenX);
  for (j=20;j<cenY+1;j++)
  {
    for (long i=1; i<cenX+1; i++)
    {
      unsigned long iOffsX = *pInc2--;
      unsigned long iOffsY = *pInc2--;
	    DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	    *(pOut2--) = RGB_Add(RGB_Dec(pixel,fact),*pOut2);
    }
    pOut2 -= GBL_Width - cenX;
	  pInc2 -= (GBL_Width-cenX-2)<<1;
  }
  
  //1er cuadrante
  DWORD *pInc1 = BLURS_IncRadial + (((GBL_Width-2)*cenY - (GBL_Width-cenX-2))<<1);
  DWORD *pOut1 = GBL_Bits + GBL_Width*(cenY+1) - (GBL_Width-cenX-1);
  for (j=20;j<cenY+1;j++)
  {
    for (long i=cenX+1; i<GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc1++;
      unsigned long iOffsY = *pInc1++;
	    DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	    *(pOut1++) = RGB_Add(RGB_Dec(pixel,fact),*pOut1);
    }
    pOut1 += -GBL_Width -(GBL_Width - cenX -1) +1;
	  pInc1 += (-(GBL_Width-2) -(GBL_Width - cenX -1) +1)<<1;
  }

  //3er cuadrante
  DWORD *pInc3 = BLURS_IncRadial + (((GBL_Width-2)*cenY +cenX-1)<<1);  
  DWORD *pOut3 = GBL_Bits + GBL_Width*(cenY+1) +cenX;
  for (j=cenY+1;j<GBL_Height-20;j++)
  {
    for (long i=1; i<cenX+1; i++)
    {
      unsigned long iOffsX = *pInc3--;
      unsigned long iOffsY = *pInc3--;
	    DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	    *(pOut3--) = RGB_Add(RGB_Dec(pixel,fact),*pOut3);
    }
    pOut3 += GBL_Width + cenX;
	  pInc3 += (GBL_Width + cenX -2)<<1;
  }
  
  //4o cuadrante
  DWORD *pInc4 = BLURS_IncRadial + (((GBL_Width-2)*cenY +cenX)<<1);  
  DWORD *pOut4 = GBL_Bits + GBL_Width*(cenY+1) +cenX+1;
  for (j=cenY+1;j<GBL_Height-20;j++)
  {
    for (long i=cenX+1; i<GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc4++;
      unsigned long iOffsY = *pInc4++;
	    DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	    *(pOut4++) = RGB_Add(RGB_Dec(pixel,fact),*pOut4);
    }
    pOut4 += cenX +2;
	  pInc4 += cenX<<1;
  }
}

void BLURS_RenderRadialMIX(DWORD* imgIn,long cenX,long cenY,long fact)
{
  long j;

  //2o cradrante
  DWORD *pInc2 = BLURS_IncRadial + (((GBL_Width-2)*cenY - (GBL_Width-cenX-1))<<1);  
  DWORD *pOut2 = GBL_Bits + GBL_Width*(cenY+1) - (GBL_Width-cenX);
  for (j=20;j<cenY+1;j++)
  {
    for (long i=1; i<cenX+1; i++)
    {
      unsigned long iOffsX = *pInc2--;
      unsigned long iOffsY = *pInc2--;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut2--) = RGB_Mix(RGB_Dec(pixel,fact),*pOut2);
    }
    pOut2 -= GBL_Width - cenX;
	pInc2 -= (GBL_Width-cenX-2)<<1;
  }
  
  //1er cuadrante
  DWORD *pInc1 = BLURS_IncRadial + (((GBL_Width-2)*cenY - (GBL_Width-cenX-2))<<1);
  DWORD *pOut1 = GBL_Bits + GBL_Width*(cenY+1) - (GBL_Width-cenX-1);
  for (j=20;j<cenY+1;j++)
  {
    for (long i=cenX+1; i<GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc1++;
      unsigned long iOffsY = *pInc1++;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut1++) = RGB_Mix(RGB_Dec(pixel,fact),*pOut1);
    }
    pOut1 += -GBL_Width -(GBL_Width - cenX -1) +1;
	pInc1 += (-(GBL_Width-2) -(GBL_Width - cenX -1) +1)<<1;
  }

  //3er cuadrante
  DWORD *pInc3 = BLURS_IncRadial + (((GBL_Width-2)*cenY +cenX-1)<<1);  
  DWORD *pOut3 = GBL_Bits + GBL_Width*(cenY+1) +cenX;
  for (j=cenY+1;j<GBL_Height-20;j++)
  {
    for (long i=1; i<cenX+1; i++)
    {
      unsigned long iOffsX = *pInc3--;
      unsigned long iOffsY = *pInc3--;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut3--) = RGB_Mix(RGB_Dec(pixel,fact),*pOut3);
    }
    pOut3 += GBL_Width + cenX;
	pInc3 += (GBL_Width + cenX -2)<<1;
  }
  
  //4o cuadrante
  DWORD *pInc4 = BLURS_IncRadial + (((GBL_Width-2)*cenY +cenX)<<1);  
  DWORD *pOut4 = GBL_Bits + GBL_Width*(cenY+1) +cenX+1;
  for (j=cenY+1;j<GBL_Height-20;j++)
  {
    for (long i=cenX+1; i<GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc4++;
      unsigned long iOffsY = *pInc4++;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut4++) = RGB_Mix(RGB_Dec(pixel,fact),*pOut4);
    }
    pOut4 += cenX +2;
	pInc4 += cenX<<1;
  }
}


/////////////////////////////////////////////////////////////////////
//	void BLURS_RenderRadialInv(DWORD* imgIn,long fact);

void BLURS_RenderRadialInv(DWORD* imgIn,long cenX,long cenY,long fact)
{
  long j;

  //2o cradrante
  DWORD *pInc2 = BLURS_IncRadialI + (((GBL_Width-2)*20)<<1);  
  DWORD *pOut2 = GBL_Bits + GBL_Width*21 +1;
  for (j=20;j<cenY+1;j++)
  {
    for (long i=1; i<cenX+1; i++)
    {
      unsigned long iOffsX = *pInc2++;
      unsigned long iOffsY = *pInc2++;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut2++) = RGB_Add(RGB_Dec(pixel,fact),*pOut2);
    }
    pOut2 += GBL_Width - cenX;
	pInc2 += (GBL_Width-cenX-2)<<1;
  }
  
  //1er cuadrante
  DWORD *pInc1= BLURS_IncRadialI + (((GBL_Width-2)*20 - 1)<<1);  
  DWORD *pOut1 = GBL_Bits + GBL_Width*21 - 2;
  for (j=20;j<cenY+1;j++)
  {
    for (long i=cenX+1; i<GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc1--;
      unsigned long iOffsY = *pInc1--;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut1--) = RGB_Add(RGB_Dec(pixel,fact),*pOut1);
    }
    pOut1 -= -GBL_Width -(GBL_Width - cenX -1) +1;
	pInc1 -= (-(GBL_Width-2) -(GBL_Width - cenX -1) +1)<<1;
  }

  //3er cuadrante
  DWORD *pInc3 = BLURS_IncRadialI + (((GBL_Width-2)*(GBL_Height-21))<<1);  
  DWORD *pOut3 = GBL_Bits + GBL_Width*(GBL_Height-20) +1;
  for (j=cenY+1;j<(int)GBL_Height-20;j++)
  {
    for (long i=1; i<(int)cenX+1; i++)
    {
      unsigned long iOffsX = *pInc3++;
      unsigned long iOffsY = *pInc3++;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut3++) = RGB_Add(RGB_Dec(pixel,fact),*pOut3);
    }
    pOut3 -= GBL_Width + cenX;
	pInc3 -= (GBL_Width + cenX -2)<<1;
  }
  
  //4o cuadrante
  DWORD *pInc4 = BLURS_IncRadialI + (((GBL_Width-2)*(GBL_Height-21) - 1)<<1);  
  DWORD *pOut4 = GBL_Bits + GBL_Width*(GBL_Height-20) - 2;
  for (j=cenY+1;j<(int)GBL_Height-20;j++)
  {
    for (long i=cenX+1; i<(int)GBL_Width-1; i++)
    {
      unsigned long iOffsX = *pInc4--;
      unsigned long iOffsY = *pInc4--;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut4--) = RGB_Add(RGB_Dec(pixel,fact),*pOut4);
    }
    pOut4 -= cenX +2;
	pInc4 -= cenX<<1;
  }
}

/////////////////////////////////////////////////////////////////////
//	void BLURS_Render2(DWORD* imgIn);
//  Siempre centrado :(

void BLURS_RenderRadial2(DWORD* imgIn,long fact)
{
  long j;
  
  DWORD *pInc1 = BLURS_Inc + ((((GBL_Width-2)*(GBL_Height-2)>>1) -((GBL_Width-2)>>1) +01)<<1);  
  DWORD *pOut1 = GBL_Bits + GBL_Width*(GBL_Height>>1) -(GBL_Width>>1) +01; //+1???
  DWORD *pInc2 = BLURS_Inc + ((((GBL_Width-2)*(GBL_Height-2)>>1) -((GBL_Width-2)>>1))<<1);  
  DWORD *pOut2 = GBL_Bits + GBL_Width*(GBL_Height>>1) -(GBL_Width>>1);
  DWORD *pInc3 = BLURS_Inc + ((((GBL_Width-2)*(GBL_Height-2)>>1) +((GBL_Width-2)>>1))<<1);  
  DWORD *pOut3 = GBL_Bits + GBL_Width*(GBL_Height>>1) +(GBL_Width>>1);
  DWORD *pInc4 = BLURS_Inc + ((((GBL_Width-2)*(GBL_Height-2)>>1) +((GBL_Width-2)>>1) +01)<<1);  
  DWORD *pOut4 = GBL_Bits + GBL_Width*(GBL_Height>>1) +(GBL_Width>>1) +01;
  
  for (j=20;j<((int)GBL_Height>>1);j++)
  {
    for (long i=1; i<(int)GBL_Width>>1; i++)
    {
      unsigned long iOffsX = *pInc2--;
      unsigned long iOffsY = *pInc2--;
	  DWORD pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut2--) = RGB_Add(RGB_Dec(pixel,fact),*pOut2);

	  iOffsX = *pInc1++;
      iOffsY = *pInc1++;
	  pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut1++) = RGB_Add(RGB_Dec(pixel,fact),*pOut1);

	  iOffsX = *pInc3--;
      iOffsY = *pInc3--;
	  pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut3--) = RGB_Add(RGB_Dec(pixel,fact),*pOut3);

	  iOffsX = *pInc4++;
      iOffsY = *pInc4++;
	  pixel = BLURS_CalcOffs(imgIn,iOffsX,iOffsY);
	  *(pOut4++) = RGB_Add(RGB_Dec(pixel,fact),*pOut4);
    }
	pOut1 -= (3*GBL_Width>>1) -1;
	pInc1 -= 3*(GBL_Width - 2);

    pOut2 -= 2 + GBL_Width>>1;
	pInc2 -= GBL_Width - 2;

	pOut3 += (3*GBL_Width>>1) -1;
	pInc3 += 3*(GBL_Width - 2);

    pOut4 += (GBL_Width>>1) +1;
	pInc4 += GBL_Width - 2;
  }
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

bool BLURS_Init()
{
  BLURS_Inc = (DWORD *) GetMem ((320-2)*(240-2+1)*2*4);
  if (!BLURS_Inc) return false;

  BLURS_IncRadial = (DWORD *) GetMem ((320-2)*(240-2+1)*2*4);
  if (!BLURS_IncRadial) return false;

  BLURS_IncRadialI = (DWORD *) GetMem ((320-2)*(240-2+1)*2*4);
  if (!BLURS_IncRadial) return false;

  BLURS_Pitch = 320*4;
  BLURS_GenerarTunnel(160.0f,130.0f,10.0f);
//  BLURS_GenerarTunnelWave(160.0f,130.0f,30.0f);

  return true;
}

void BLURS_Close()
{  
  FreeMem (BLURS_Inc);
  BLURS_Inc = NULL;
}


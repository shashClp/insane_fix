#include <math.h>
#include <defines.h>
#include "3d.h"
#include "rt.h"

SElemBufInterp BufInterp8x8[26][64];                

long ii,jj,uu,vv,u1,v1,u2,v2,du1,dv1,du2,dv2;

void VuelcaBufInterp8x8F(DWORD* dest,DWORD* textura)
{  
  __asm
  {
	mov     esi,[textura]  //-->esi:=@Textura (no hace falta alineamiento a 64k)
	mov     edi,[dest]     //-->edi:=@Destino
	push    ebp
	mov     [jj],0
	align   16
   BucJ:
	mov    [ii],0
	align  16
   BucI:
	mov   ebx,[jj]
	mov   eax,[ii]
	shl   ebx,3+6
	shl   eax,3
	add   ebx,eax

	mov   eax,DWORD Ptr BufInterp8x8[ebx+0]
	mov   ecx,DWORD Ptr BufInterp8x8[ebx+4]
	mov   [u1],eax  //--u1:=BufInterp8x8[j,i].x;
	mov   [v1],ecx  //--v1:=BufInterp8x8[j,i].y;
	mov   eax,DWORD Ptr BufInterp8x8[ebx+8]
	mov   ecx,DWORD Ptr BufInterp8x8[ebx+12]
	and   eax,0x00ffffff
	mov   [v2],ecx  //--v2:=BufInterp8x8[j,i+1].y;
	mov   [u2],eax  //--u2:=BufInterp8x8[j,i+1].x;

	add   ebx,64*2*4
	mov   eax,DWORD Ptr BufInterp8x8[ebx+0]
	and   eax,0x00ffffff
	mov   ecx,DWORD Ptr BufInterp8x8[ebx+4]
	sub   eax,[u1]
	sub   ecx,[v1]
	shl   eax,13
	shl   ecx,13
	mov   [du1],eax //--du1:=((BufInterp8x8[j+1,i].x-u1) shl 13);
	mov   [dv1],ecx //--dv1:=((BufInterp8x8[j+1,i].y-v1) shl 13);
	mov   eax,DWORD Ptr BufInterp8x8[ebx+8]
	and   eax,0x00ffffff
	mov   ecx,DWORD Ptr BufInterp8x8[ebx+12]
	sub   eax,[u2]
	sub   ecx,[v2]
	shl   eax,13
	shl   ecx,13
	mov   [du2],eax //--du2:=((BufInterp8x8[j+1,i+1].x-u2) shl 13);
	mov   [dv2],ecx //--dv2:=((BufInterp8x8[j+1,i+1].y-v2) shl 13);

	shl   [u1],16
	shl   [v1],16
	shl   [u2],16
	shl   [v2],16

	mov   ebp,8          //-->ebp:=#Scanlines por pintar
	align 4
   BucY:
	mov   eax,[u1]
	mov   ebx,[v1]
	mov   [uu],eax
	mov   [vv],ebx
	//Calcula du y dv
	mov   ebx,[u2]
	mov   edx,[v2]
	sub   ebx,[u1]
	sub   edx,[v1]
	shr   ebx,3     //--ebx:=du FP:16.16
	shr   edx,3     //--edx:=dv FP:16.16

	 xor   eax,eax

	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  ecx,[esi+eax*4]
	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  [edi],ecx
	 mov  ecx,[esi+eax*4]
	 mov  [edi+4],ecx

	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  ecx,[esi+eax*4]
	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  [edi+8],ecx
	 mov  ecx,[esi+eax*4]
	 mov  [edi+12],ecx

	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  ecx,[esi+eax*4]
	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  [edi+16],ecx
	 mov  ecx,[esi+eax*4]
	 mov  [edi+20],ecx

	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  ecx,[esi+eax*4]
	 mov  ah,Byte Ptr [uu+2]
	 add  [uu],ebx
	 mov  al,Byte Ptr [vv+2]
	 add  [vv],edx
	 mov  [edi+24],ecx
	 mov  ecx,[esi+eax*4]
	 mov  [edi+28],ecx


	add   edi,320*4
	mov   eax,[du1]
	mov   ebx,[dv1]
	add   [u1],eax
	add   [v1],ebx
	mov   eax,[du2]
	mov   ebx,[dv2]
	add   [u2],eax
	add   [v2],ebx
	dec   ebp
	jnz   BucY
	sub   edi,(320*4*8)-8*4
	inc    [ii]
	cmp    [ii],320 shr 3
	jne    BucI
	add     edi,(320*4*7)
	inc     [jj]
	cmp     [jj],200 shr 3
	jne     BucJ
	pop     ebp
  }
}


#define  Radio (512*2)
#define  RadioAlCubo (Radio*Radio)
#define  zz (Radio/4)

CVector3 vCamara(0,0,0);

void OldFreeEsfera(float x,float y,float z)
{
  float a,b,c;
  float delta,t,tt;
  CVector3 vDireccion1,vDireccion,vInterseccion;
  CMatrix3x3 mRotacion(true);
  mRotacion.SetRotacion(x,y,z);

  float cx = cos(x);
  float sx = sin(x);
  float cz = cos(z);
  float sz = sin(z);
  vDireccion1.m_z = zz;
  for(long yy=0;yy<=200;yy+=8)
  {
   for(long xx=0;xx<=320;xx+=8)
   {
      //Vector Direccion
      vDireccion1.m_x = (xx-160);
      vDireccion1.m_y = (yy-100);      
      //Direction *= RotationMatrix;
      vDireccion = mRotacion * vDireccion1;
      if(abs(vDireccion.m_y)<0.005)
		  vDireccion.m_y = 0.005;

      //Distancia hasta la interseccion
      a = sqr(vDireccion.m_x) + sqr(vDireccion.m_y) + sqr(vDireccion.m_z);
      if(a==0) a=1;
      b = 2*(vCamara.m_x*vDireccion.m_x + vCamara.m_y*vDireccion.m_y + vCamara.m_z*vDireccion.m_z);
      c = sqr(vCamara.m_x) + sqr(vCamara.m_y) + sqr(vCamara.m_z) - RadioAlCubo;
      delta = sqr(b)-4*a*c;
      t = (-b-sqrt(delta))/(2*a);
      //Punto de Interseccion
      vInterseccion.m_x = vCamara.m_x + t*vDireccion.m_x;
      if(vInterseccion.m_x==0)
		vInterseccion.m_x = 1;
      vInterseccion.m_y = vCamara.m_y + t*vDireccion.m_y;
      vInterseccion.m_z = vCamara.m_z + t*vDireccion.m_z;
      //Mapeo del punto
      long u = ceil(/*sin*/(vInterseccion.m_z/sqrt(sqr(vInterseccion.m_x)+sqr(vInterseccion.m_y)+sqr(vInterseccion.m_z)))*Radio/2);
      long v = ceil(abs(atan(vInterseccion.m_y/vInterseccion.m_x)*(Radio/2)/fPi)); //atan2!!!!
      BufInterp8x8[yy>>3][xx>>3].x = u;
      BufInterp8x8[yy>>3][xx>>3].y = v;    
	  }
  }
}


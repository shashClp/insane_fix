// AWL - Anaconda Windows Library
// RGB

#ifndef AWL_RGB
#define AWL_RGB

#include <windows.h>

#define Get_A(a)  (((BYTE *)&a)[3])
#define Get_R(a)  (((BYTE *)&a)[2])
#define Get_G(a)  (((BYTE *)&a)[1])
#define Get_B(a)  (((BYTE *)&a)[0])

#define Set_A(a,b)  ((((BYTE *)&a)[3])=(BYTE)b)
#define Set_R(a,b)  ((((BYTE *)&a)[2])=(BYTE)b)
#define Set_G(a,b)  ((((BYTE *)&a)[1])=(BYTE)b)
#define Set_B(a,b)  ((((BYTE *)&a)[0])=(BYTE)b)

void RGB_Set (DWORD &a,DWORD r,DWORD g,DWORD b);

#define RGB_Mix(a,b) (((a & 0x0FEFEFE) + (b & 0x0FEFEFE))>>1)

inline DWORD RGB_MixBlack(DWORD a,DWORD c)
{
  long r = (Get_R(a)*c)/256;
  long g = (Get_G(a)*c)/256;
  long b = (Get_B(a)*c)/256;
  return (((r<<8)+g)<<8)+c;
}

inline DWORD RGB_Dec(DWORD a,DWORD x)
// x = [0..127] (cada componente se decrementa en x)
{
	_asm
	{
		push	ebx
		mov		eax,[a]
		mov		ebx,[x]
		sub		al,bl
		jnc		rOk
		 mov	al,0
		rOk:
		sub		ah,bl
		jnc		gOk
		 mov	ah,0
		gOk:
		ror		eax,16
		sub		al,bl
		jnc		bOk
		 mov	al,0
		bOk:
		pop		ebx
		ror		eax,16
	}
}

inline DWORD RGB_Add(DWORD a,DWORD b)
{
	_asm
	{
		push	ebx
		mov		eax,[a]
		mov		ebx,[b]
		add		al,bl
		jnc		rOk
		 mov	al,255
		rOk:
		add		ah,bh
		jnc		gOk
		 mov	ah,255
		gOk:
		ror		eax,16
		ror		ebx,16
		add		al,bl
		jnc		bOk
		 mov	al,255
		bOk:
		pop		ebx
		ror		eax,16
	}
}

#endif

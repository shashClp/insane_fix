#include <windows.h>
#include <view.h>

long FADE_Tipo = 0;
long FADE_Pos = 0; //0->inicial, 256->final
long FADE_Velocidad = 0;

#define FADE_DESDE_NEGRO		2
#define FADE_A_BLANCO				4
#define FADE_DESDE_BLANCO		8
#define FADE_A_INVERTIDO		16
#define FADE_DESDE_INVERTIDO	32

//////////////////////////////////////////////////////////////////////////////
//
//	void FadeANegro(DWORD* dst,DWORD* src,float pos)
//	pos = [0..1]; 0 = sin efecto; 1 = Negro Total

void FadeANegro(DWORD* src,float pos)
{
	if(pos>1) pos = 1;
	if(pos<0) pos = 0;
	long iPos = pos*255.0f;

	_asm
	{
		pushad
		mov esi,[GBL_Size]
		sub esi,320*20*2
		mov edi,[src]
		add edi,320*20*4
	   bucAN:
		mov ebx,[edi]
		mov eax,[iPos]
		mul bl
		mov bl,ah
		mov eax,[iPos]
		mul bh
		mov bh,ah
		rol ebx,16
		mov eax,[iPos]
		mul bl
		mov bl,ah
		ror ebx,16
		mov [edi],ebx
		add edi,4
		dec esi
		jnz bucAN
		popad
	};	
}


//////////////////////////////////////////////////////////////////////////////
//
//	void FadeABlanco(DWORD* dst,DWORD* src,float pos)
//	pos = [0..1]; 0 = sin efecto; 1 = Blanco Total

void FadeABlanco(DWORD* src,float pos)
{
	if(pos>1) pos = 1;
	if(pos<0) pos = 0;
	long iPos = pos*255.0f;

	_asm
	{
		pushad
		mov esi,[GBL_Size]
		sub esi,320*20*2
		mov edi,[src]
		add edi,320*20*4
	   bucAB:
		mov ebx,[edi]
		not ebx
		mov eax,[iPos]
		mul bl
		mov bl,ah
		mov eax,[iPos]
		mul bh
		mov bh,ah
		rol ebx,16
		mov eax,[iPos]
		mul bl
		mov bl,ah
		ror ebx,16					
		add [edi],ebx
		add edi,4
		dec esi
		jnz bucAB
		popad
	};
}


//////////////////////////////////////////////////////////////////////////////
//
//	void FadeAInverso(DWORD* dst,DWORD* src,float pos)
//	pos = [0..1]; 0 = sin efecto; 1 = Inverso

void FadeAInverso(DWORD* src,float pos)
{
	if(pos>1) pos = 1;
	if(pos<0) pos = 0;
	long iPos = pos*255.0f;

//TODO:

	_asm //fade blanco
	{
		pushad
		mov esi,[GBL_Size]
		sub esi,320*20*2
		mov edi,[src]
		add edi,320*20*4
	   bucAB:
		mov ebx,[edi]
		not ebx
		mov eax,[iPos]
		mul bl
		mov bl,ah
		mov eax,[iPos]
		mul bh
		mov bh,ah
		rol ebx,16
		mov eax,[iPos]
		mul bl
		mov bl,ah
		ror ebx,16					
		add [edi],ebx
		add edi,4
		dec esi
		jnz bucAB
		popad
	};
}


void FadeABlanco320x240(DWORD* src,float pos)
{
	if(pos>1) pos = 1;
	if(pos<0) pos = 0;
	long iPos = pos*255.0f;

	_asm
	{
		pushad
		mov esi,[GBL_Size]		
		mov edi,[src]		
	   bucAB:
		mov ebx,[edi]
		not ebx
		mov eax,[iPos]
		mul bl
		mov bl,ah
		mov eax,[iPos]
		mul bh
		mov bh,ah
		rol ebx,16
		mov eax,[iPos]
		mul bl
		mov bl,ah
		ror ebx,16					
		add [edi],ebx
		add edi,4
		dec esi
		jnz bucAB
		popad
	};
}

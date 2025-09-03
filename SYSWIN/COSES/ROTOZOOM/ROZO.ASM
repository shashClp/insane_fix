section .bss

global _RotoZoom_Source
global _RotoZoom_Dest
global _RotoZoom_IncDef
global _RotoZoom_Width
global _RotoZoom_Pitch
global _RotoZoom_Height
global _RotoZoom_Inc
global _RotoZoom_IncX
global _RotoZoom_IncY
global _RotoZoom_Inc2
global _RotoZoom_IncX2
global _RotoZoom_IncY2
global _RotoZoom_Start
global _RotoZoom_StartX
global _RotoZoom_StartY

_RotoZoom_Source resd 1
_RotoZoom_Dest   resd 1
_RotoZoom_IncDef resd 1
_RotoZoom_Width  resd 1
_RotoZoom_Pitch  resd 1
_RotoZoom_Height resd 1
_RotoZoom_Inc:
_RotoZoom_IncX   resw 1
_RotoZoom_IncY   resw 1
_RotoZoom_Inc2:
_RotoZoom_IncX2  resw 1
_RotoZoom_IncY2  resw 1
_RotoZoom_Start:
_RotoZoom_StartX resw 1
_RotoZoom_StartY resw 1

section .text

global _MakeRotoZoomer

_MakeRotoZoomer:
   push ebp
   mov edi,[_RotoZoom_Dest]
   mov ebx,[_RotoZoom_Inc]
   mov ebp,[_RotoZoom_Start]

   mov esi,[_RotoZoom_Height]
loopY:
      push esi
      mov esi,[_RotoZoom_Source]
      mov edx,ebp
      mov ecx,[_RotoZoom_Width]
   loopX:
         add edx,ebx
         shld eax,edx,8
         mov ah,dh
         and eax,0xffff
auto:    mov eax,[eax*4+esi]
         dec ecx
         mov [edi+ecx*4],eax
      jnz loopX
      add edi,[_RotoZoom_Pitch]
      add ebx,[_RotoZoom_IncDef]
      add ebp,[_RotoZoom_Inc2]
      pop esi
      dec esi
   jnz loopY

   pop ebp
   ret
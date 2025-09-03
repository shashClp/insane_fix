// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <painter.h>
#include <tri.h>

void TRI_Function_ScanLine_Flat50()
{
  __asm
  {
    cmp TRI_width,0
    jle EqFinal
    
    mov ebx,TRI_color
    and ebx,0xfefefeff
    mov ecx,TRI_width
    mov edi,TRI_dst

    shr ecx,1
    jnc buc
      mov eax,[edi]
      and eax,0xfefefeff      
      add eax,ebx
      shr eax,1
      mov [edi],eax
      add edi,4
    buc:
      cmp ecx,0
      je EqFinal

      align 16
    buc2:      
      mov eax,[edi]
      mov edx,[edi+4]
      and eax,0xfefefeff      
      and edx,0xfefefeff      
      add eax,ebx
      add edx,ebx
      shr eax,1
      shr edx,1
      stosd
      mov eax,edx
      stosd
      dec ecx
    jnz buc2

  EqFinal:
  }
}

void TRI_CFunction_ScanLine_Flat50 ()
{
  DWORD col = TRI_color & 0xfefefeff;
  while (TRI_width-->0) 
  {
    *TRI_dst = ((*TRI_dst & 0xfefefeff)+col)/2;
    TRI_dst++;
  }
}

void _fastcall TRI_FLAT50_Paint (TPTRI_FLAT *src)
{
  TRI_color = src->col;
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;
  TRI_SetUp = NULL;
  TRI_SlopeValues = NULL;  
  TRI_ConstantSlope = NULL;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_Flat50;
#else
  TRI_ScanLine = TRI_Function_ScanLine_Flat50;
#endif
  TRI_Draw();  
}

// AW3D - Anaconda 3D Windows Library
// Triangle Flat

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>

void TRI_MMXFunction_ScanLine_Flat()
{
  __asm
  {
    align 16
    cmp TRI_width,0
    jle EqFinal
    
    mov eax,TRI_color
    mov edi,TRI_dst
    
    mov ecx,TRI_width
    shr ecx,1
    jnc NoCarry    
    stosd

  NoCarry:
    jz EqFinal
    movd mm0,TRI_color
    movd mm1,TRI_color
    psllq mm0,32
    por mm0,mm1

    align 16
  LoopX:
    movq [edi],mm0
    add edi,8
    dec ecx
    jnz LoopX   
    emms     
  EqFinal:        
  }  
}

void TRI_Function_ScanLine_Flat()
{
  __asm
  {
    align 16
    cmp TRI_width,0
    jle EqFinal
    
    mov eax,TRI_color
    mov ecx,TRI_width
    mov edi,TRI_dst
    
    rep stosd

  EqFinal:
  }
}

void TRI_CFunction_ScanLine_Flat ()
{
  while (TRI_width-->0) *TRI_dst++ = TRI_color;
}

void _fastcall TRI_FLAT_Paint (TPTRI_FLAT *src)
{
  TRI_color = src->col;
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;
  TRI_SetUp = NULL;
  TRI_SlopeValues = NULL;  
  TRI_ConstantSlope = NULL;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_Flat;  
#else
  if (MMX)
  {
    TRI_ScanLine = TRI_MMXFunction_ScanLine_Flat;
  } else {
    TRI_ScanLine = TRI_Function_ScanLine_Flat;
  }
#endif
  TRI_Draw();  
}

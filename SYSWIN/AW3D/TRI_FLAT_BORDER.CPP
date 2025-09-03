// AW3D - Anaconda 3D Windows Library
// Triangle Flat Border

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>

void TRI_Function_ScanLine_Flat_Border()
{
  __asm
  {
    align 16
    cmp TRI_width,0
    jle EqFinal
    
    mov edi,TRI_dst    
    mov eax,TRI_color
    mov ecx,TRI_width
    mov [edi],eax
    mov [edi+ecx*4-4],eax

  EqFinal:
  }
}

void TRI_CFunction_ScanLine_Flat_Border ()
{
  if (TRI_width>0) 
  {
    TRI_dst[0] = TRI_color;    
    TRI_dst[TRI_width-1] = TRI_color;
  }
}

void _fastcall TRI_FLAT_BORDER_Paint (TPTRI_FLAT *src)
{
  TRI_color = src->col;
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;
  TRI_SetUp = NULL;
  TRI_SlopeValues = NULL;  
  TRI_ConstantSlope = NULL;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_Flat_Border;  
#else
  TRI_ScanLine = TRI_Function_ScanLine_Flat_Border;
#endif
  TRI_Draw();  
}

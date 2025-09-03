// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include "tri.inc"

void TRI_Function_SlopeValues_PI_MAP (TTRI_Edge &edge)
{
  edge.du = edge.valS2->v1-edge.valS1->v1;
  edge.dv = edge.valS2->v2-edge.valS1->v2;  
  edge.su = edge.valS1->v1+(edge.du*TRI_subpixel);
  edge.sv = edge.valS1->v2+(edge.dv*TRI_subpixel);  

  edge.du*= TRI_idy;
  edge.dv*= TRI_idy;  
}

void TRI_Function_ConstantSlope_PI_MAP ()
{
  TRI_du = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dv = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_duSHL16 = (DWORD)(TRI_du*TRI_Mul16);
  TRI_dvSHL16 = (DWORD)(TRI_dv*TRI_Mul16);
}

void TRI_Function_ScanLine_PI_MAP ()
{
  __asm 
  {
    cmp TRI_width,0
    jle EqFinal

    PreStep2(tSU,TRI_du,TRI_uu,tSV,TRI_dv,TRI_vv,TRI_Mul16)

    push ebp

    xor ebx,ebx
    mov ecx,TRI_width
    mov esi,TRI_texture_source
    mov edi,TRI_dst
    mov ebp,TRI_duSHL16
    mov edx,TRI_dvSHL16

    align 16
    EqLoop:
      mov bh,BYTE PTR TRI_uu[2]
      mov bl,BYTE PTR TRI_vv[2]
      add TRI_uu,ebp
      and ebx,TRI_MaskTex
      add TRI_vv,edx
      mov eax,[esi+ebx*4]
      add edi,4
      dec ecx
      mov [edi-4],eax
      jnz EqLoop

    pop ebp

  EqFinal:
    AddDelta2(tSU,tDU,tSV,tDV)
  }
}

void TRI_CFunction_ScanLine_PI_MAP ()
{
  float deltaX = TRI_xL-TRI_Left.sx;
  DWORD uu = (DWORD)((TRI_Left.su+deltaX*TRI_du)*65536.0);
  DWORD vv = (DWORD)((TRI_Left.sv+deltaX*TRI_dv)*65536.0);
  while (TRI_width-->0)
  {
    *TRI_dst++ = TRI_texture_source[(((((BYTE *)&uu)[2])<<8)+(((BYTE *)&vv)[2])) & TRI_MaskTex];
    uu+= TRI_duSHL16; vv+= TRI_dvSHL16;
  }
  TRI_Left.su += TRI_Left.du;
  TRI_Left.sv += TRI_Left.dv;  
}

void _fastcall TRI_PI_MAP_Paint (TPTRI_PI_MAP *src)
{
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;

  TRI_val[0] = (TTRI_Info *) &src->u1;
  TRI_val[1] = (TTRI_Info *) &src->u2;
  TRI_val[2] = (TTRI_Info *) &src->u3;

  TRI_texture_source = src->txt->bits;
  TRI_SetUpMask (src->txt->w,src->txt->h);

  TRI_SetUp = NULL;
  TRI_SlopeValues = TRI_Function_SlopeValues_PI_MAP;  
  TRI_ConstantSlope = TRI_Function_ConstantSlope_PI_MAP;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_PI_MAP;
#else
  TRI_ScanLine = TRI_Function_ScanLine_PI_MAP;
#endif
  TRI_Draw();  
}

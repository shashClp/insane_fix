// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>
#include "tri.inc"

void TRI_Function_SlopeValues_RGB (TTRI_Edge &edge)
{
  edge.dr = edge.valS2->v1-edge.valS1->v1;
  edge.dg = edge.valS2->v2-edge.valS1->v2;
  edge.db = edge.valS2->v3-edge.valS1->v3;
  edge.sr = edge.valS1->v1+(edge.dr*TRI_subpixel);
  edge.sg = edge.valS1->v2+(edge.dg*TRI_subpixel);
  edge.sb = edge.valS1->v3+(edge.db*TRI_subpixel);

  edge.dr*= TRI_idy;
  edge.dg*= TRI_idy;
  edge.db*= TRI_idy;
}

void TRI_Function_ConstantSlope_RGB ()
{
  TRI_dr = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dg = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_db = ((TRI_val[0]->v3-TRI_val[2]->v3)*TRI_y1y2 -
            (TRI_val[1]->v3-TRI_val[2]->v3)*TRI_y0y2) * TRI_daemon;
  TRI_drSHL16 = (DWORD) (TRI_dr*TRI_Mul16);
  TRI_dgSHL16 = (DWORD) (TRI_dg*TRI_Mul16);
  TRI_dbSHL16 = (DWORD) (TRI_db*TRI_Mul16);
}

void TRI_Function_ConstantSlope8b_RGB ()
{
  TRI_dr = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dg = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_db = ((TRI_val[0]->v3-TRI_val[2]->v3)*TRI_y1y2 -
            (TRI_val[1]->v3-TRI_val[2]->v3)*TRI_y0y2) * TRI_daemon;
  TRI_drSHL8 = (DWORD) (TRI_dr*TRI_Mul8);
  TRI_dgSHL8 = (DWORD) (TRI_dg*TRI_Mul8);
  TRI_dbSHL8 = (DWORD) (TRI_db*TRI_Mul8);
}

void TRI_Function_ScanLine_RGB()
{
  __asm 
  {
    cmp TRI_width,0
    jle EqFinal

    PreStep3(tSR,TRI_dr,TRI_rr,tSG,TRI_dg,TRI_gg,tSB,TRI_db,TRI_bb,TRI_Mul16)
    
    push ebp
    
    mov ecx,TRI_width
    mov edi,TRI_dst
    mov ebp,TRI_drSHL16
    mov edx,TRI_dgSHL16
    mov ebx,TRI_dbSHL16
    mov esi,TRI_rr

    align 16
    EqLoop:
      mov eax,esi
      add esi,ebp
      mov ah,BYTE PTR TRI_gg[2]
      add TRI_gg,edx
      mov al,BYTE PTR TRI_bb[2]      
      add TRI_bb,ebx
      add edi,4
      dec ecx
      mov [edi-4],eax
      jnz EqLoop

    pop ebp

  EqFinal:
    AddDelta3(tSR,tDR,tSG,tDG,tSB,tDB)
  }
}

void TRI_MMXFunction_ScanLine_RGB()
{
  __asm 
  {
    cmp TRI_width,0
    jle EqFinal

    PreStep3(tSR,TRI_dr,TRI_rr,tSG,TRI_dg,TRI_gg,tSB,TRI_db,TRI_bb,TRI_Mul8)
    
    push ebp

    mov ecx,TRI_width
    mov edi,TRI_dst

    // Ready mmx
    and TRI_drSHL8,0xffff
    and TRI_rr,0xffff
    and TRI_dgSHL8,0xffff
    and TRI_gg,0xffff
    and TRI_dbSHL8,0xffff
    and TRI_bb,0xffff
    movd mm0,TRI_drSHL8
    movd mm3,TRI_rr    
    movd mm4,TRI_gg
    movd mm1,TRI_dgSHL8
    movd mm5,TRI_bb
    movd mm2,TRI_dbSHL8
    psllq mm3,32
    psllq mm0,32
    psllq mm4,16
    psllq mm1,16
    por mm3,mm4
    por mm0,mm1
    por mm3,mm5    
    por mm0,mm2
    pxor mm6,mm6

    align 16
    EqLoop:
      movq mm7,mm3
      add edi,4
      psrlw mm7,7
      dec ecx
      packuswb mm7,mm6
      paddw mm3,mm0
      movd [edi-4],mm7      
      jnz EqLoop

    pop ebp

    emms

  EqFinal:
    AddDelta3(tSR,tDR,tSG,tDG,tSB,tDB)
  }
}

void TRI_CFunction_ScanLine_RGB ()
{
  float deltaX = TRI_xL-TRI_Left.sx;
  DWORD rr = (DWORD)((TRI_Left.sr+deltaX*TRI_dr)*65536.0);
  DWORD gg = (DWORD)((TRI_Left.sg+deltaX*TRI_dg)*65536.0);
  DWORD bb = (DWORD)((TRI_Left.sb+deltaX*TRI_db)*65536.0);
  while (TRI_width-->0)
  {    
    *TRI_dst++ = ((((((BYTE *)&rr)[2])<<8)+(((BYTE *)&gg)[2]))<<8)+((BYTE *)&bb)[2];
    rr+= TRI_drSHL16; 
    gg+= TRI_dgSHL16; 
    bb+= TRI_dbSHL16;
  }
  TRI_Left.sr += TRI_Left.dr;
  TRI_Left.sg += TRI_Left.dg;
  TRI_Left.sb += TRI_Left.db;
}

void _fastcall TRI_RGB_Paint (TPTRI_RGB *src)
{
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;

  TRI_val[0] = (TTRI_Info *) &src->r1;
  TRI_val[1] = (TTRI_Info *) &src->r2;
  TRI_val[2] = (TTRI_Info *) &src->r3;

  TRI_SetUp = NULL;
  TRI_SlopeValues = TRI_Function_SlopeValues_RGB;  
#ifdef _DEBUG
  TRI_ConstantSlope = TRI_Function_ConstantSlope_RGB;
  TRI_ScanLine = TRI_CFunction_ScanLine_RGB;
#else
  if (MMX)
  {
    TRI_ConstantSlope = TRI_Function_ConstantSlope8b_RGB;
    TRI_ScanLine = TRI_MMXFunction_ScanLine_RGB;  
  } else {
    TRI_ConstantSlope = TRI_Function_ConstantSlope_RGB;
    TRI_ScanLine = TRI_Function_ScanLine_RGB;  
  }
#endif
  TRI_Draw();  
}

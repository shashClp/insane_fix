// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>
#include <rgb.h>
#include "tri.inc"

void TRI_Function_SlopeValues_PI_MAP_RGB (TTRI_Edge &edge)
{
  edge.du = edge.valS2->v1-edge.valS1->v1;
  edge.dv = edge.valS2->v2-edge.valS1->v2;  
  edge.su = edge.valS1->v1+(edge.du*TRI_subpixel);
  edge.sv = edge.valS1->v2+(edge.dv*TRI_subpixel);  

  edge.du*= TRI_idy;
  edge.dv*= TRI_idy;  

  edge.dr = edge.valS2->v3-edge.valS1->v3;
  edge.dg = edge.valS2->v4-edge.valS1->v4;
  edge.db = edge.valS2->v5-edge.valS1->v5;
  edge.sr = edge.valS1->v3+(edge.dr*TRI_subpixel);
  edge.sg = edge.valS1->v4+(edge.dg*TRI_subpixel);
  edge.sb = edge.valS1->v5+(edge.db*TRI_subpixel);

  edge.dr*= TRI_idy;
  edge.dg*= TRI_idy;
  edge.db*= TRI_idy;
}

void TRI_Function_ConstantSlope_PI_MAP_RGB ()
{
  TRI_du = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dv = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_duSHL16 = (DWORD)(TRI_du*TRI_Mul16);
  TRI_dvSHL16 = (DWORD)(TRI_dv*TRI_Mul16);

  TRI_dr = ((TRI_val[0]->v3-TRI_val[2]->v3)*TRI_y1y2 -
            (TRI_val[1]->v3-TRI_val[2]->v3)*TRI_y0y2) * TRI_daemon;
  TRI_dg = ((TRI_val[0]->v4-TRI_val[2]->v4)*TRI_y1y2 -
            (TRI_val[1]->v4-TRI_val[2]->v4)*TRI_y0y2) * TRI_daemon;
  TRI_db = ((TRI_val[0]->v5-TRI_val[2]->v5)*TRI_y1y2 -
            (TRI_val[1]->v5-TRI_val[2]->v5)*TRI_y0y2) * TRI_daemon;
  TRI_drSHL16 = (DWORD) (TRI_dr*TRI_Mul16);
  TRI_dgSHL16 = (DWORD) (TRI_dg*TRI_Mul16);
  TRI_dbSHL16 = (DWORD) (TRI_db*TRI_Mul16);
}

void TRI_Function_ConstantSlope8b_PI_MAP_RGB ()
{
  TRI_du = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dv = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_duSHL16 = (DWORD)(TRI_du*TRI_Mul16);
  TRI_dvSHL16 = (DWORD)(TRI_dv*TRI_Mul16);

  TRI_dr = ((TRI_val[0]->v3-TRI_val[2]->v3)*TRI_y1y2 -
            (TRI_val[1]->v3-TRI_val[2]->v3)*TRI_y0y2) * TRI_daemon;
  TRI_dg = ((TRI_val[0]->v4-TRI_val[2]->v4)*TRI_y1y2 -
            (TRI_val[1]->v4-TRI_val[2]->v4)*TRI_y0y2) * TRI_daemon;
  TRI_db = ((TRI_val[0]->v5-TRI_val[2]->v5)*TRI_y1y2 -
            (TRI_val[1]->v5-TRI_val[2]->v5)*TRI_y0y2) * TRI_daemon;
  TRI_drSHL8 = (DWORD) (TRI_dr*TRI_Mul8);
  TRI_dgSHL8 = (DWORD) (TRI_dg*TRI_Mul8);
  TRI_dbSHL8 = (DWORD) (TRI_db*TRI_Mul8);
}

void TRI_MMXFunction_ScanLine_PI_MAP_RGB ()
{
  __asm 
  {
    cmp TRI_width,0
    jle EqFinal

    PreStep2(tSU,TRI_du,TRI_uu,tSV,TRI_dv,TRI_vv,TRI_Mul16)
    PreStep3(tSR,TRI_dr,TRI_rr,tSG,TRI_dg,TRI_gg,tSB,TRI_db,TRI_bb,TRI_Mul8)

    push ebp

    xor ebx,ebx
    mov ecx,TRI_width
    mov esi,TRI_texture_source
    mov edi,TRI_dst
    mov ebp,TRI_duSHL16
    mov edx,TRI_dvSHL16

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
      mov bh,BYTE PTR TRI_uu[2]
      movq mm7,mm3
      mov bl,BYTE PTR TRI_vv[2]
      psrlw mm7,7
      and ebx,TRI_MaskTex
      add TRI_uu,ebp
      paddw mm3,mm0
      add TRI_vv,edx

      movd mm5,[esi+ebx*4]
      punpcklbw mm5,mm6

      pmullw mm5,mm7
      psrlw mm5,7
      packuswb mm5,mm6

      movd [edi],mm5
      add edi,4
      dec ecx
      jnz EqLoop

    pop ebp

    emms

  EqFinal:
    AddDelta3(tSR,tDR,tSG,tDG,tSB,tDB)
    AddDelta2(tSU,tDU,tSV,tDV)
  }
}

void TRI_CFunction_ScanLine_PI_MAP_RGB ()
{
  float deltaX = TRI_xL-TRI_Left.sx;
  DWORD uu = (DWORD)((TRI_Left.su+deltaX*TRI_du)*65536.0);
  DWORD vv = (DWORD)((TRI_Left.sv+deltaX*TRI_dv)*65536.0);
  DWORD rr = (DWORD)((TRI_Left.sr+deltaX*TRI_dr)*65536.0);
  DWORD gg = (DWORD)((TRI_Left.sg+deltaX*TRI_dg)*65536.0);
  DWORD bb = (DWORD)((TRI_Left.sb+deltaX*TRI_db)*65536.0);
  while (TRI_width-->0)
  {
    DWORD col1 = ((((((BYTE *)&rr)[2])<<8)+(((BYTE *)&gg)[2]))<<8)+((BYTE *)&bb)[2];
    DWORD col2 = TRI_texture_source[(((((BYTE *)&uu)[2])<<8)+(((BYTE *)&vv)[2])) & TRI_MaskTex];
    *TRI_dst++ = RGB_Add(col1,col2);
    rr+= TRI_drSHL16; gg+= TRI_dgSHL16; bb+= TRI_dbSHL16;
    uu+= TRI_duSHL16; vv+= TRI_dvSHL16;
  }
  TRI_Left.su += TRI_Left.du;
  TRI_Left.sv += TRI_Left.dv;  
  TRI_Left.sr += TRI_Left.dr;
  TRI_Left.sg += TRI_Left.dg;
  TRI_Left.sb += TRI_Left.db;
}

void _fastcall TRI_PI_MAP_RGB_Paint (TPTRI_PI_MAP_RGB *src)
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
  TRI_SlopeValues = TRI_Function_SlopeValues_PI_MAP_RGB;  
#ifdef _DEBUG
  TRI_ConstantSlope = TRI_Function_ConstantSlope_PI_MAP_RGB;
  TRI_ScanLine = TRI_CFunction_ScanLine_PI_MAP_RGB;
#else
  if (MMX)
  {
    TRI_ConstantSlope = TRI_Function_ConstantSlope8b_PI_MAP_RGB;
    TRI_ScanLine = TRI_MMXFunction_ScanLine_PI_MAP_RGB;
  } else {
    TRI_ConstantSlope = TRI_Function_ConstantSlope_PI_MAP_RGB;
    TRI_ScanLine = TRI_CFunction_ScanLine_PI_MAP_RGB;
  }
#endif
  TRI_Draw();  
}

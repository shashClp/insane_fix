// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>
#include "tri.inc"

void TRI_Function_SlopeValues_PI_MAP_DOUBLE (TTRI_Edge &edge)
{
  edge.du = edge.valS2->v1-edge.valS1->v1;
  edge.dv = edge.valS2->v2-edge.valS1->v2;  
  edge.su = edge.valS1->v1+(edge.du*TRI_subpixel);
  edge.sv = edge.valS1->v2+(edge.dv*TRI_subpixel);  

  edge.du*= TRI_idy;
  edge.dv*= TRI_idy;  

  edge.dl1 = edge.valS2->v3-edge.valS1->v3;
  edge.dl2 = edge.valS2->v4-edge.valS1->v4;  
  edge.sl1 = edge.valS1->v3+(edge.dl1*TRI_subpixel);
  edge.sl2 = edge.valS1->v4+(edge.dl2*TRI_subpixel);  

  edge.dl1*= TRI_idy;
  edge.dl2*= TRI_idy;  
}

void TRI_Function_ConstantSlope_PI_MAP_DOUBLE ()
{
  TRI_du = ((TRI_val[0]->v1-TRI_val[2]->v1)*TRI_y1y2 -
            (TRI_val[1]->v1-TRI_val[2]->v1)*TRI_y0y2) * TRI_daemon;
  TRI_dv = ((TRI_val[0]->v2-TRI_val[2]->v2)*TRI_y1y2 -
            (TRI_val[1]->v2-TRI_val[2]->v2)*TRI_y0y2) * TRI_daemon;
  TRI_duSHL16 = (DWORD)(TRI_du*TRI_Mul16);
  TRI_dvSHL16 = (DWORD)(TRI_dv*TRI_Mul16);

  TRI_dl1 = ((TRI_val[0]->v3-TRI_val[2]->v3)*TRI_y1y2 -
             (TRI_val[1]->v3-TRI_val[2]->v3)*TRI_y0y2) * TRI_daemon;
  TRI_dl2 = ((TRI_val[0]->v4-TRI_val[2]->v4)*TRI_y1y2 -
             (TRI_val[1]->v4-TRI_val[2]->v4)*TRI_y0y2) * TRI_daemon;
  TRI_dl1SHL16 = (DWORD)(TRI_dl1*TRI_Mul16);
  TRI_dl2SHL16 = (DWORD)(TRI_dl2*TRI_Mul16);
}

void TRI_MMXFunction_ScanLine_PI_MAP_DOUBLE ()
{
  __asm 
  {
    cmp TRI_width,0
    jle EqFinal

    PreStep2(tSU,TRI_du,TRI_uu,tSV,TRI_dv,TRI_vv,TRI_Mul16)
    PreStep2(tSL1,TRI_dl1,TRI_ll1,tSL2,TRI_dl2,TRI_ll2,TRI_Mul16)

    push ebp

    mov edi,TRI_dst    
    mov esi,TRI_texture_source
    mov eax,TRI_texture_source2
    mov ebp,TRI_duSHL16
    mov edx,TRI_dvSHL16
    mov ebx,TRI_dl1SHL16
    mov ecx,TRI_dl2SHL16

    align 16
    EqLoop:
      movd mm0,ebx
      mov bh,BYTE PTR TRI_uu[2]
      mov bl,BYTE PTR TRI_vv[2]
      and ebx,TRI_MaskTex

      movd mm7,[esi+ebx*4]

      mov bh,BYTE PTR TRI_ll1[2]
      mov bl,BYTE PTR TRI_ll2[2]      

      movd mm6,[eax+ebx*4]

      paddusb mm7,mm6

      movd [edi],mm7
      add edi,4

      movd ebx,mm0
      add TRI_uu,ebp
      add TRI_vv,edx
      add TRI_ll1,ebx
      add TRI_ll2,ecx
      dec DWORD PTR [TRI_width]
      jnz EqLoop

    pop ebp

    emms

  EqFinal:
    AddDelta2(tSL1,tDL1,tSL2,tDL2)
    AddDelta2(tSU,tDU,tSV,tDV)
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

void TRI_CFunction_ScanLine_PI_MAP_DOUBLE ()
{
  float deltaX = TRI_xL-TRI_Left.sx;
  DWORD uu = (DWORD)((TRI_Left.su+deltaX*TRI_du)*65536.0);
  DWORD vv = (DWORD)((TRI_Left.sv+deltaX*TRI_dv)*65536.0);
  DWORD ll1 = (DWORD)((TRI_Left.sl1+deltaX*TRI_dl1)*65536.0);
  DWORD ll2 = (DWORD)((TRI_Left.sl2+deltaX*TRI_dl2)*65536.0);
  while (TRI_width-->0)
  {
    DWORD col1 = TRI_texture_source[(((((BYTE *)&uu)[2])<<8)+(((BYTE *)&vv)[2])) & TRI_MaskTex];
    DWORD col2 = TRI_texture_source2[(((((BYTE *)&ll1)[2])<<8)+(((BYTE *)&ll2)[2])) & TRI_MaskTex];
    *TRI_dst++ = RGB_Add (col1,col2);
    uu+= TRI_duSHL16; vv+= TRI_dvSHL16;
    ll1+= TRI_dl1SHL16; ll2+= TRI_dl2SHL16;
  }
  TRI_Left.su += TRI_Left.du;
  TRI_Left.sv += TRI_Left.dv;  
  TRI_Left.sl1 += TRI_Left.dl1;
  TRI_Left.sl2 += TRI_Left.dl2;  
}

void _fastcall TRI_PI_MAP_DOUBLE_Paint (TPTRI_PI_MAP_DOUBLE *src)
{
  TRI_vtx[0] = (TVertex *) &src->x1;
  TRI_vtx[1] = (TVertex *) &src->x2;
  TRI_vtx[2] = (TVertex *) &src->x3;

  TRI_val[0] = (TTRI_Info *) &src->u1;
  TRI_val[1] = (TTRI_Info *) &src->u2;
  TRI_val[2] = (TTRI_Info *) &src->u3;

  TRI_texture_source = src->txt1->bits;
  TRI_texture_source2 = src->txt2->bits;
  TRI_SetUpMask (src->txt1->w,src->txt1->h);

  TRI_SetUp = NULL;
  TRI_SlopeValues = TRI_Function_SlopeValues_PI_MAP_DOUBLE;  
  TRI_ConstantSlope = TRI_Function_ConstantSlope_PI_MAP_DOUBLE;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_PI_MAP_DOUBLE;
#else  
  if (MMX)
  {
    TRI_ScanLine = TRI_MMXFunction_ScanLine_PI_MAP_DOUBLE;
  } else {
    TRI_ScanLine = TRI_CFunction_ScanLine_PI_MAP_DOUBLE;
  }
#endif
  TRI_Draw();  
}

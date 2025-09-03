#ifndef R_INTERPOLA
#define R_INTERPOLA

void Interpola (unsigned long *Scene,unsigned long *Dst)
{
  __int64 Inv = (256 / R_RAY_DIM);
  Inv +=(((Inv<<16)+Inv)<<16);
  dword IncV = (R_ScreenX-(R_RAY_DIM))*4;
  dword R1 = R_RAY_DIM*4;
  dword R2 = R_RAY_DIM*R_ScreenX;

  _asm pxor mm0,mm0

  for (long j = RBorderV; j < R_ScreenY-RBorderV-1; j += R_RAY_DIM)
  {
    for (long i = RBorderH; i < R_ScreenX; i += R_RAY_DIM)
    {
      dword *D = Dst+(j*R_ScreenX)+i;
      dword *S1 = Scene+(j*R_ScreenX)+i;
      dword *S2 = S1+R2;

      _asm {
        movq mm5,Inv
        mov edi,S1
        mov esi,S2
        mov ebx,R1

        movd mm1,[edi]
        movd mm2,[edi+ebx]
        movd mm3,[esi]
        movd mm4,[esi+ebx]
        punpcklbw mm1,mm0
        punpcklbw mm2,mm0
        punpcklbw mm3,mm0
        punpcklbw mm4,mm0

        psubw mm3,mm1
        psubw mm4,mm2
        pmullw mm3,mm5
        pmullw mm4,mm5

        psllw mm1,8
        psllw mm2,8

        mov edi,D
        mov ebx,IncV
        mov edx,R_RAY_DIM
//        dec edx
        LoopY:
          movq mm5,Inv
          movq mm6,mm1
          movq mm7,mm2

          psrlw mm6,8
          psrlw mm7,8
          psubw mm7,mm6
          pmullw mm7,mm5
          movq mm6,mm1

          mov ecx,R_RAY_DIM
          LoopX:
            movq mm5,mm6
            paddw mm6,mm7
            psrlw mm5,8
            add edi,4
            packuswb mm5,mm0
            dec ecx
            movd [edi-4],mm5
            jnz LoopX

          paddw mm1,mm3
          paddw mm2,mm4
          add edi,ebx
          dec edx
          jnz LoopY
      }
    }
  }
  _asm emms
}
#endif

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
#include "rlight.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±


TLightPoint_R *ActiveLight;


void TLightPoint_R_Init ( TReal P1 ,TReal P2 ,TReal P3,byte R ,byte G ,byte B )
{
    RVTX_Set( ActiveLight->Position,P1,P2,P3 );

    ActiveLight->Color.B.R = R;
    ActiveLight->Color.B.G = G;
    ActiveLight->Color.B.B = B;

}

/*
float TLightPoint_R_Diffuse ( TVertex  &P1,TVertex  &P2 ,TReal &Val);
#pragma aux TLightPoint_R_Diffuse = \
"   mov ecx , dword ptr ActiveLight"\
"     fld dword ptr [ecx+4]     "\
"     fsub  dword ptr [edx]     "\
"     fst st(1)                 "\
"     fmul  dword ptr [eax]     "\
"     fld dword ptr [ecx+8]     "\
"     fsub  dword ptr [edx+4]   "\
"     fst st(3)                 "\
"     fmul  dword ptr [eax+4]   "\
"     fadd                      "\
"     fld dword ptr [ecx+12]    "\
"     fsub dword ptr  [edx+8]   "\
"     fst st(4)                 "\
"     fmul dword ptr  [eax+8]   "\
"     fadd                      "\
"     fst st(4)                 "\
"     fldz                      "\
"     fcompp                    "\
"     fnstsw    ax              "\
"     sahf                      "\
"     jae       @Surt           "\
"         fld st                "\
"         fmul                  "\
"         fld st(1)             "\
"         fmulp st(2),st        "\
"         fadd                  "\
"         fld st(1)             "\
"         fmulp st(2),st        "\
"         fadd                  "\
"         fsqrt                 "\
"         fdiv                  "\
"         fstp dword ptr [edi]  "\
"         jmp @Surt2            "\
" @Surt:                        "\
"         xor eax,eax           "\
"         mov dword ptr [edi],eax "\
" @Surt2:                       "\
parm [edx][eax][edi] modify [ecx eax 8087] ;
*/

void TLightPoint_R_Diffuse ( TVertex  &P1,TVertex  &P2 ,TReal &Val)
{

  register TReal LLumX2,LLumY2,LLumZ2,Value;
  register TReal LLumX,LLumY,LLumZ;
  register TReal Factor;
  LLumX = ActiveLight->Position.x - P1.x;
  LLumY = ActiveLight->Position.y - P1.y;
  LLumZ = ActiveLight->Position.z - P1.z;
  Value = P2.x*LLumX+P2.y*LLumY+P2.z*LLumZ;
  Val = 0;
  if (Value > 0 )
  {
    LLumY2 = LLumY*LLumY;
    LLumZ2 = LLumZ*LLumZ;
    LLumX2 = LLumX*LLumX;
    Val = Value /(sqrt(LLumX2+LLumY2+LLumZ2) );
  }
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
float TLightPoint_R_Phong (TVertex  &P1,TVertex  &P2,TReal &P_I,TReal &P_S)
{

  TReal LLumX2,LLumY2,LLumZ2,Value;
  TReal LLumX,LLumY,LLumZ;

  LLumX = ActiveLight->Position.x - P1.x;
  LLumX2 = LLumX*LLumX;

  LLumY = ActiveLight->Position.y - P1.y;
  LLumY2 = LLumY*LLumY;

  LLumZ = ActiveLight->Position.z - P1.z;
  LLumZ2 = LLumZ*LLumZ;

  Value = P2.x*LLumX+P2.y*LLumY+P2.z*LLumZ;

  if (Value > 0 )
  {
    LLumZ = sqrt(LLumX2+LLumY2+LLumZ2);
    Value =  Value /(LLumZ);
    return (P_I*pow(Value,P_S));
  }
  return (0);
}
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TLightPoint_R *CreateLightDefault()
{

  TLightPoint_R *TMP = NULL;

  if ( ( TMP = (TLightPoint_R *) R_GetMem (sizeof(TLightPoint_R)))==NULL)
  {
#ifdef R_LOG_DEF
    R_ERR ("Can't alloc memory for light");
#endif
    RAY_EXIT ();
  }
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Light in Memory at [%p]",TMP);
#endif
  //TMP->TLightPoint_R_Diffuse    = &TLightPoint_R_Diffuse;
  TMP->TLightPoint_R_Phong      = &TLightPoint_R_Phong;
  TMP->Color.B.R = 1;
  TMP->Color.B.G = 1;
  TMP->Color.B.B = 1;
  return TMP;
}

void DestroyLightDefault(TLightPoint_R *M)
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Light from Memory at [%p]",M);
#endif
  R_FreeMem (M);
}
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

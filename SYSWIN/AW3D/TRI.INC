#define tDX    28
#define tDZ    32
#define tDU    36
#define tDV    40
#define tDA    44
#define tDR    48
#define tDG    52
#define tDB    56
#define tDL1   60
#define tDL2   64
#define tDB1   68
#define tDB2   72

#define tSX    76
#define tSZ    80
#define tSU    84
#define tSV    88
#define tSA    92
#define tSR    96
#define tSG   100
#define tSB   104
#define tSL1  108
#define tSL2  112
#define tSB1  116
#define tSB2  120

#define PreStep1(S1,D1,L1,t) __asm \
{ \
  __asm fild TRI_xL \
  __asm fsub DWORD PTR TRI_Left[tSX] \
  __asm fld D1 \
  __asm fmul st,st(1) \
  __asm fadd DWORD PTR TRI_Left[S1] \
  __asm fmul t \
  __asm fistp L1 \
}

#define PreStep2(S1,D1,L1,S2,D2,L2,t) __asm \
{ \
  __asm fild TRI_xL \
  __asm fsub DWORD PTR TRI_Left[tSX] \
  __asm fld D1 \
  __asm fmul st,st(1) \
  __asm fxch \
  __asm fmul D2 \
  __asm fxch \
  __asm fadd DWORD PTR TRI_Left[S1] \
  __asm fxch \
  __asm fadd DWORD PTR TRI_Left[S2] \
  __asm fxch \
  __asm fmul t \
  __asm fxch \
  __asm fmul t \
  __asm fxch \
  __asm fistp L1 \
  __asm fistp L2 \
}

#define PreStep3(S1,D1,L1,S2,D2,L2,S3,D3,L3,t) __asm \
{ \
  __asm fild TRI_xL \
  __asm fsub DWORD PTR TRI_Left[tSX] \
  __asm fld D3 \
  __asm fmul st,st(1) \
  __asm fxch \
  __asm fld D1 \
  __asm fmul st,st(1) \
  __asm fxch \
  __asm fmul D2 \
  __asm fxch \
  __asm fadd DWORD PTR TRI_Left[S1] \
  __asm fxch \
  __asm fadd DWORD PTR TRI_Left[S2] \
  __asm fxch st(2) \
  __asm fadd DWORD PTR TRI_Left[S3] \
  __asm fxch \
  __asm fmul t \
  __asm fxch st(2) \
  __asm fmul t \
  __asm fxch \
  __asm fmul t \
  __asm fxch st(2) \
  __asm fistp L1 \
  __asm fistp L2 \
  __asm fistp L3 \
}

#define AddDelta1(S1,D1) __asm \
{ \
  __asm fld  TRI_Left[S1] \
  __asm fadd DWORD PTR TRI_Left[D1] \
  __asm fstp TRI_Left[S1] \
}

#define AddDelta2(S1,D1,S2,D2) __asm \
{ \
  __asm fld  TRI_Left[S1] \
  __asm fadd DWORD PTR TRI_Left[D1] \
  __asm fld  TRI_Left[S2] \
  __asm fadd DWORD PTR TRI_Left[D2] \
  __asm fxch \
  __asm fstp TRI_Left[S1] \
  __asm fstp TRI_Left[S2] \
}

#define AddDelta3(S1,D1,S2,D2,S3,D3) __asm \
{ \
  __asm fld  TRI_Left[S3] \
  __asm fadd DWORD PTR TRI_Left[D3] \
  __asm fld  TRI_Left[S1] \
  __asm fadd DWORD PTR TRI_Left[D1] \
  __asm fld  TRI_Left[S2] \
  __asm fadd DWORD PTR TRI_Left[D2] \
  __asm fxch \
  __asm fstp TRI_Left[S1] \
  __asm fstp TRI_Left[S2] \
  __asm fstp TRI_Left[S3] \
}

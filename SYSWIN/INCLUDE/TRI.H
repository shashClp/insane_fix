// AW3D - Anaconda 3D Windows Library
// Basic Triangle

#ifndef AW3D_TRIANGLE
#define AW3D_TRIANGLE

#include <windows.h>
#include <vertex.h>

// Standard Types

typedef struct {
  float v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
} TTRI_Info;

typedef struct {
  TVertex *vtxS1,*vtxS2;
  TTRI_Info *valS1,*valS2;
  TTRI_Info *pcvalS1,*pcvalS2;
  float dy;
  float dx,dz,du,dv,da,dr,dg,db,dl1,dl2,db1,db2;
  float sx,sz,su,sv,sa,sr,sg,sb,sl1,sl2,sb1,sb2;
} TTRI_Edge;


extern DWORD TRI_uu;
extern DWORD TRI_vv;
extern DWORD TRI_aa;
extern DWORD TRI_rr;
extern DWORD TRI_gg;
extern DWORD TRI_bb;
extern DWORD TRI_ll1;
extern DWORD TRI_ll2;
extern float TRI_Mul8;
extern float TRI_Mul16;

extern DWORD TRI_color;
extern DWORD TRI_color2;
extern DWORD *TRI_texture_source;
extern DWORD *TRI_texture_source2;

extern TVertex *TRI_vtx[3];
extern TTRI_Info *TRI_val[3];
extern void (*TRI_ScanLine)();
extern void (*TRI_ConstantSlope)();
extern void (*TRI_SetUp)();
extern void (*TRI_SlopeValues)(TTRI_Edge &edge);

extern long TRI_xL;
extern long TRI_width;
extern DWORD *TRI_dst;
extern TTRI_Edge TRI_Left;
extern TTRI_Edge TRI_Right;
extern float TRI_subpixel;
extern float TRI_daemon;
extern float TRI_y0y2;
extern float TRI_y1y2;
extern float TRI_idy;

extern long TRI_start_y;
extern long TRI_fin_y;
extern bool TRI_calcvalues;
extern DWORD *TRI_view;
extern float *TRI_zview;
extern TTRI_Info *TRI_pcval[3];
extern TTRI_Info TRI_pcvalues[64];

extern float TRI_du;
extern float TRI_dv;
extern float TRI_dz;
extern float TRI_dr;
extern float TRI_dg;
extern float TRI_db;
extern float TRI_da;
extern float TRI_dl1;
extern float TRI_dl2;
extern float TRI_db1;
extern float TRI_db2;

extern float TRI_duTex;
extern float TRI_dvTex;
extern float TRI_dzTex;
extern float TRI_drTex;
extern float TRI_dgTex;
extern float TRI_dbTex;
extern float TRI_daTex;
extern float TRI_dl1Tex;
extern float TRI_dl2Tex;
extern float TRI_db1Tex;
extern float TRI_db2Tex;

extern long TRI_duSHL8;
extern long TRI_dvSHL8;
extern long TRI_drSHL8;
extern long TRI_dgSHL8;
extern long TRI_dbSHL8;
extern long TRI_daSHL8;
extern long TRI_dl1SHL8;
extern long TRI_dl2SHL8;
extern long TRI_db1SHL8;
extern long TRI_db2SHL8;

extern long TRI_duSHL16;
extern long TRI_dvSHL16;
extern long TRI_drSHL16;
extern long TRI_dgSHL16;
extern long TRI_dbSHL16;
extern long TRI_daSHL16;
extern long TRI_dl1SHL16;
extern long TRI_dl2SHL16;
extern long TRI_db1SHL16;
extern long TRI_db2SHL16;

extern DWORD TRI_MaskTex;
extern DWORD TRI_DivTex;
extern DWORD TRI_ModTex;
extern DWORD TRI_PixTex;
extern float TRI_InvTex;

// Functions
void TRI_Draw ();
void TRI_SetUpTexel (DWORD tex);
void TRI_SetUpMask (DWORD w,DWORD h);

#endif
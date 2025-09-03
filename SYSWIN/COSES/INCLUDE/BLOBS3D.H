// Marching Cubes Blobs 
// By Elric

#ifndef AW_BLOBS3D
#define AW_BLOBS3D

#include <vertex.h>
#include <plane.h>
#include <face.h>
#include <bound.h>
#include <matrix.h>
#include <a3dtypes.h>

#define BLOBS3D_EMPTY_EDGE     -1

#define BLOBS3D_V1  (float)0.5f+0
#define BLOBS3D_V2  (float)0.5f+1
#define BLOBS3D_V3  (float)0.5f-1

const DWORD BLOBS3D_to[12] = { 1,2,3,0,5,6,7,4,4,5,6,7 };
const TVertex BLOBS3D_pfr[12] = {
    {BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V1},{BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V1},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V1},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V1},
    {BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V2},{BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V2},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V2},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V2},
    {BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V1},{BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V1},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V1},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V1} };
const TVertex BLOBS3D_pto[12] = {
    {BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V1},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V1},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V1},{BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V1},
    {BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V2},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V2},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V2},{BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V2},
    {BLOBS3D_V1,BLOBS3D_V1,BLOBS3D_V2},{BLOBS3D_V2,BLOBS3D_V1,BLOBS3D_V2},{BLOBS3D_V2,BLOBS3D_V3,BLOBS3D_V2},{BLOBS3D_V1,BLOBS3D_V3,BLOBS3D_V2} };

extern TObject BLOBS3D_object;
extern TVertex BLOBS3D_scale;
extern float BLOBS3D_limit;
extern float *BLOBS3D_data;
extern TTexture *BLOBS3D_txt;
extern TVertex BLOBS3D_pofv;
extern DWORD BLOBS3D_iDim;
extern DWORD BLOBS3D_jDim;
extern DWORD BLOBS3D_kDim;
extern DWORD BLOBS3D_ijDim;
extern DWORD BLOBS3D_ijkDim;

extern DWORD BLOBS3D_iDim2;
extern DWORD BLOBS3D_jDim2;
extern DWORD BLOBS3D_kDim2;
extern DWORD BLOBS3D_iDim12;
extern DWORD BLOBS3D_ijDim12;
extern long *BLOBS3D_edges;
extern float *BLOBS3D_bot;
extern float *BLOBS3D_top;

extern WORD BLOBS3D_edgetable[256];
extern char BLOBS3D_tritable[256][16];

///////////////////////////////////////////////////////////////////////////////

void BLOBS3D_MakeMarch ();
void BLOBS3D_MakeMarch_Texture ();

bool BLOBS3D_Init (DWORD i,DWORD j,DWORD k,DWORD vnum,DWORD fnum);
void BLOBS3D_Close ();

/*
void BLOBS3D_PaintFlat (TMatrix &m);
void BLOBS3D_PaintGouraud (TMatrix &m);
void BLOBS3D_PaintFlatBorder (TMatrix &m);
void BLOBS3D_PaintPI_MAP_Flat (TMatrix &m);
void BLOBS3D_PaintPI_MAP_Gouraud (TMatrix &m);
void BLOBS3D_PaintPI_MAP_DOUBLE_Environment (TMatrix &m);

void BLOBS3D_CalcLightDiffuse (TVertex &P,float &Val);
void BLOBS3D_CalcLightNormal (TVertex &P,float &Val);
*/

#endif
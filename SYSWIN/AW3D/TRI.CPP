// AW3D - Anaconda 3D Windows Library
// Basic Triangle

#include <windows.h>
#include <math.h>
#include <view.h>
#include <viewz.h>
#include <tri.h>

DWORD TRI_uu;
DWORD TRI_vv;
DWORD TRI_aa;
DWORD TRI_rr;
DWORD TRI_gg;
DWORD TRI_bb;
DWORD TRI_ll1;
DWORD TRI_ll2;

float TRI_Mul8 = 128.0f;
float TRI_Mul16 = 65536.0f;
DWORD TRI_MaskTex;
DWORD TRI_DivTex;
DWORD TRI_ModTex;
DWORD TRI_PixTex;
float TRI_InvTex;

DWORD TRI_color;
DWORD TRI_color2;
DWORD *TRI_texture_source;
DWORD *TRI_texture_source2;

TVertex *TRI_vtx[3];
TTRI_Info *TRI_val[3];
void (*TRI_ScanLine)();
void (*TRI_ConstantSlope)();
void (*TRI_SetUp)();
void (*TRI_SlopeValues)(TTRI_Edge &edge);

long TRI_xL;
long TRI_width;
DWORD *TRI_dst;
TTRI_Edge TRI_Left;
TTRI_Edge TRI_Right;
float TRI_subpixel;
float TRI_daemon;
float TRI_y0y2;
float TRI_y1y2;
float TRI_idy;

long TRI_start_y;
long TRI_fin_y;
bool TRI_calcvalues;
DWORD *TRI_view;
float *TRI_zview;
TTRI_Info *TRI_pcval[3];
TTRI_Info TRI_pcvalues[64];

float TRI_du;
float TRI_dv;
float TRI_dz;
float TRI_dr;
float TRI_dg;
float TRI_db;
float TRI_da;
float TRI_dl1;
float TRI_dl2;
float TRI_db1;
float TRI_db2;

float TRI_duTex;
float TRI_dvTex;
float TRI_dzTex;
float TRI_drTex;
float TRI_dgTex;
float TRI_dbTex;
float TRI_daTex;
float TRI_dl1Tex;
float TRI_dl2Tex;
float TRI_db1Tex;
float TRI_db2Tex;

long TRI_duSHL8;
long TRI_dvSHL8;
long TRI_drSHL8;
long TRI_dgSHL8;
long TRI_dbSHL8;
long TRI_daSHL8;
long TRI_dl1SHL8;
long TRI_dl2SHL8;
long TRI_db1SHL8;
long TRI_db2SHL8;

long TRI_duSHL16;
long TRI_dvSHL16;
long TRI_drSHL16;
long TRI_dgSHL16;
long TRI_dbSHL16;
long TRI_daSHL16;
long TRI_dl1SHL16;
long TRI_dl2SHL16;
long TRI_db1SHL16;
long TRI_db2SHL16;

inline void TRI_CalculateEdge (TTRI_Edge &edge,bool values)
{
  edge.dy = edge.vtxS2->y - edge.vtxS1->y;
  if (edge.dy > 0.0)
  {
    edge.sx = edge.vtxS1->x;
    TRI_idy = 1.0f / edge.dy;
    edge.dx = (edge.vtxS2->x - edge.sx) * TRI_idy;
    TRI_subpixel = TRI_start_y-edge.vtxS1->y;
    edge.sx += edge.dx * TRI_subpixel;
    if (values)
    {
      TRI_subpixel *= TRI_idy;
      TRI_SlopeValues(edge);
    }
  }
}

inline void TRI_Slope ()
{  
  while (TRI_start_y < TRI_fin_y)
  {
    TRI_xL = (long)ceil(TRI_Left.sx);
    TRI_dst = TRI_view+TRI_xL;
    TRI_width = (long)ceil(TRI_Right.sx)-TRI_xL;
    TRI_ScanLine();
    TRI_view += GBL_Pitch;
    TRI_Right.sx += TRI_Right.dx;
    TRI_Left.sx += TRI_Left.dx;
    TRI_start_y++;
  }
}

void TRI_Draw ()
{
bool dir;
float min_y,max_y;
DWORD min,mid,max,maxl,maxr;

  min = max = 0; maxr = 1; maxl = 2;
  min_y = max_y = TRI_vtx[0]->y;
  if (TRI_vtx[1]->y < max_y) { max_y = TRI_vtx[1]->y; max = 1; maxr = 2; maxl = 0; }
  if (TRI_vtx[2]->y < max_y) { max_y = TRI_vtx[2]->y; max = 2; maxr = 0; maxl = 1; }
  if (TRI_vtx[1]->y > min_y) { min_y = TRI_vtx[1]->y; min = 1; }
  if (TRI_vtx[2]->y > min_y) { min_y = TRI_vtx[2]->y; min = 2; }

  if (TRI_ConstantSlope)
  {
    TRI_y0y2 = TRI_vtx[0]->y-TRI_vtx[2]->y;
    TRI_y1y2 = TRI_vtx[1]->y-TRI_vtx[2]->y;
    TRI_daemon = (TRI_vtx[0]->x - TRI_vtx[2]->x) * TRI_y1y2 -
                 (TRI_vtx[1]->x - TRI_vtx[2]->x) * TRI_y0y2;
    TRI_daemon = 1.0f / TRI_daemon;
    TRI_ConstantSlope();
  }

  TRI_start_y = (long)ceil(TRI_vtx[max]->y);   
  TRI_view = GBL_Bits+TRI_start_y*GBL_Pitch;
  TRI_zview = GBLZ_Bits+TRI_start_y*GBLZ_Pitch;

  if (TRI_SetUp) TRI_SetUp();

  TRI_Left.vtxS1 = TRI_vtx[max];
  TRI_Left.vtxS2 = TRI_vtx[maxl];
  TRI_Left.valS1 = TRI_val[max];
  TRI_Left.valS2 = TRI_val[maxl];
  TRI_Left.pcvalS1 = TRI_pcval[max];
  TRI_Left.pcvalS2 = TRI_pcval[maxl];
  TRI_CalculateEdge (TRI_Left,TRI_SlopeValues!=NULL);
  TRI_Right.vtxS1 = TRI_vtx[max];
  TRI_Right.vtxS2 = TRI_vtx[maxr];
  TRI_Right.valS1 = TRI_val[max];
  TRI_Right.valS2 = TRI_val[maxr];
  TRI_Right.pcvalS1 = TRI_pcval[max];
  TRI_Right.pcvalS2 = TRI_pcval[maxr];
  TRI_CalculateEdge (TRI_Right,FALSE);
  dir = TRI_vtx[maxl]->y > TRI_vtx[maxr]->y;
  if (dir) mid = maxr; else mid = maxl;
  TRI_fin_y = (long)ceil(TRI_vtx[mid]->y);
  TRI_Slope ();
  TRI_fin_y = (long)ceil(TRI_vtx[min]->y);
  if (TRI_start_y < TRI_fin_y)
  {
    if (dir)
    {
      TRI_Right.vtxS1 = TRI_vtx[mid];
      TRI_Right.vtxS2 = TRI_vtx[min];
      TRI_Right.valS1 = TRI_val[mid];
      TRI_Right.valS2 = TRI_val[min];
      TRI_Right.pcvalS1 = TRI_pcval[mid];
      TRI_Right.pcvalS2 = TRI_pcval[min];
      TRI_CalculateEdge (TRI_Right,FALSE);
    } else {
      TRI_Left.vtxS1 = TRI_vtx[mid];
      TRI_Left.vtxS2 = TRI_vtx[min];
      TRI_Left.valS1 = TRI_val[mid];
      TRI_Left.valS2 = TRI_val[min];
      TRI_Left.pcvalS1 = TRI_pcval[mid];
      TRI_Left.pcvalS2 = TRI_pcval[min];
      TRI_CalculateEdge (TRI_Left,TRI_SlopeValues!=NULL);
    }
    TRI_Slope ();
  }
}

void TRI_SetUpTexel (DWORD tex)
{
  // Valors valids 4 8 16 32 64
  switch (tex)
  {
    case 4:
      TRI_DivTex = 2;
      TRI_ModTex = 3;
      break;
    case 8:
      TRI_DivTex = 3;
      TRI_ModTex = 7;
      break;
    case 16:
      TRI_DivTex = 4;
      TRI_ModTex = 15;
      break;
    case 32:
      TRI_DivTex = 5;
      TRI_ModTex = 31;
      break;
    case 64:
      TRI_DivTex = 6;
      TRI_ModTex = 63;
      break;
  }
  TRI_PixTex = tex;
  TRI_InvTex = 1.0f/tex;
}

void TRI_SetUpMask (DWORD w,DWORD h)
{
  switch (w)
  {
    case 256: TRI_MaskTex = 0xff; break;
    case 128: TRI_MaskTex = 0x7f; break;
    case 64:  TRI_MaskTex = 0x3f; break;
    case 32:  TRI_MaskTex = 0x1f; break;
    case 16:  TRI_MaskTex = 0x0f; break;
    case 8:   TRI_MaskTex = 0x07; break;
    case 4:   TRI_MaskTex = 0x03; break;
    case 2:   TRI_MaskTex = 0x01; break;
  }
  switch (h)
  {
    case 256: TRI_MaskTex |= 0xff00; break;
    case 128: TRI_MaskTex |= 0x7f00; break;
    case 64:  TRI_MaskTex |= 0x3f00; break;
    case 32:  TRI_MaskTex |= 0x1f00; break;
    case 16:  TRI_MaskTex |= 0x0f00; break;
    case 8:   TRI_MaskTex |= 0x0700; break;
    case 4:   TRI_MaskTex |= 0x0300; break;
    case 2:   TRI_MaskTex |= 0x0100; break;
  }
}


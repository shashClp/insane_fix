// AW3D - Anaconda 3D Windows Library
// Painters

#include <windows.h>
#include <view.h>
#include <paintertypes.h>
#include <tri.h>
#include <syswin.h>

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
  TRI_drSHL16 = (DWORD) (TRI_dr*65536.0);
  TRI_dgSHL16 = (DWORD) (TRI_dg*65536.0);
  TRI_dbSHL16 = (DWORD) (TRI_db*65536.0);
}

void TRI_Function_ScanLine_RGB()
{
}

void TRI_MMXFunction_ScanLine_RGB()
{
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
  TRI_ConstantSlope = TRI_Function_ConstantSlope_RGB;
#ifdef _DEBUG
  TRI_ScanLine = TRI_CFunction_ScanLine_RGB;
#else
  if (MMX)
  {
    TRI_ScanLine = TRI_MMXFunction_ScanLine_RGB;  
  } else {
    TRI_ScanLine = TRI_Function_ScanLine_RGB;  
  }
#endif
  TRI_Draw();  
}

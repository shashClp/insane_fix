// Default initializations in a simple program

#ifndef AW_OBJ3DPAINT 
#define AW_OBJ3DPAINT

#include <matrix.h>
#include <order.h>
#include <triclip.h>
#include <a3dtypes.h>

extern TVertex OBJ3DPAINT_Llum;
extern TAW3D_BufferZOrder *OBJ3DPAINT_BZO;
extern TAW3D_TriangleClipper *OBJ3DPAINT_TC;

extern float OBJ3DPAINT_despX;
extern float OBJ3DPAINT_despY;
extern TTexture *OBJ3DPAINT_txt2;
extern DWORD OBJ3DPAINT_color;

//////////////////////////////////////////////////////////////////////////////////////

inline void OBJ3DPAINT_CalcLightSphere (TVertex &P,float &Val)
{
  TVertex NP = P;
  VTX_Normalize(NP);
  Val = VTX_DotProduct (NP,OBJ3DPAINT_Llum);
  Val *=0.5f;
  Val +=0.5f;
  Val*=Val*Val;
}

inline void OBJ3DPAINT_CalcLightNormal (TVertex &P,float &Val)
{  
  Val = VTX_DotProduct (P,OBJ3DPAINT_Llum);
  Val *=0.5f;
  Val +=0.5f;
  Val*=Val*Val;
}

//////////////////////////////////////////////////////////////////////////////////////

void OBJ3DPAINT_Flat50 (TMatrix &m,TObject &obj);
void OBJ3DPAINT_Rep_Flat50 (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Flat_Border (TMatrix &m,TObject &obj);
void OBJ3DPAINT_Rep_Flat_Border (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Flat_Border_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_Rep_Flat_Border_Light (TMatrix &m,TObject &obj);

//////////////////////////////////////////////////////////////////////////////////////

void OBJ3DPAINT_Order_Flat_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_Flat_Light (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_Flat50 (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_Flat50 (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_Flat_Border (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_Flat_Border (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_Flat_Border_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_Flat_Border_Light (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_Gouraud_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_Gouraud_Light (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_PI_MAP (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_PI_MAP (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_PI_MAP_Flat_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_PI_MAP_Flat_Light (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_PI_MAP_Gouraud_Light (TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_PI_MAP_Gouraud_Light (TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_PI_MAP_DOUBLE_Environemnt(TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Environemnt(TMatrix &m,TObject &obj);

void OBJ3DPAINT_Order_PI_MAP_DOUBLE_Desplacement(TMatrix &m,TObject &obj);
void OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Desplacement(TMatrix &m,TObject &obj);

#endif
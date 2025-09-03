#include <windows.h>
#include <a3dtypes.h>
#include <object3dpaint.h>
#include <view3D.h>
#include <plane.h>
#include <bound.h>
#include <painter.h>
#include <rgb.h>

TVertex OBJ3DPAINT_Llum;
TAW3D_BufferZOrder *OBJ3DPAINT_BZO;
TAW3D_TriangleClipper *OBJ3DPAINT_TC;

float OBJ3DPAINT_despX;
float OBJ3DPAINT_despY;
TTexture *OBJ3DPAINT_txt2;
DWORD OBJ3DPAINT_color;


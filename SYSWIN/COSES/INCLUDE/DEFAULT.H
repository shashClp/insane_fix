// Default initializations in a simple program

#ifndef AW_DEFAULT 
#define AW_DEFAULT 

#include <view.h>
#include <view3d.h>
#include <viewz.h>
#include <transbuff.h>
#include <order.h>
#include <triclip.h>

extern TView *View;
extern TView3D *View3D;
extern TViewZ *ViewZ;
extern TAW3D_TriangleClipper *TC;
extern TAW3D_BufferZOrder *BZO;
extern TAW3D_TransformationBuffer *TB;

bool DEFAULT_Init();
void DEFAULT_Close();
void DEFAULT_Flip();

#endif
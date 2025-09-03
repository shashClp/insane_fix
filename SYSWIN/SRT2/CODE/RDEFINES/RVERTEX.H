#ifndef RLIBRARY_VERTEX
#define RLIBRARY_VERTEX

#include <stdio.h>

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
typedef struct {
  float x;
  float y;
  float z;
} TVertex;

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#define RVTX_Size    sizeof(TVertex)
#define RVTX_SizeDW  sizeof(TVertex)/4

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
extern void RVTX_Set (TVertex &a,float x,float y,float z);
extern void RVTX_Copy (TVertex &a,TVertex const &b);
extern void RVTX_Add (TVertex &a,TVertex &b);
extern void RVTX_Sub (TVertex &a,TVertex &b);
extern void RVTX_Mul (TVertex &a,TVertex &b);
extern void RVTX_Div (TVertex &a,TVertex &b);
extern void RVTX_AddValue (TVertex &a,float b);
extern void RVTX_SubValue (TVertex &a,float b);
extern void RVTX_MulValue (TVertex &a,float b);
extern void RVTX_DivValue (TVertex &a,float b);
extern void RVTX_Normalize (TVertex &a);
extern void RVTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c);
extern void RVTX_Print (TVertex &a);
extern void RVTX_Neg (TVertex &a);
extern float RVTX_DotProduct(TVertex &a,TVertex &b);
extern float RVTX_Distance (TVertex &a,TVertex &b);
extern float RVTX_SemiDistance (TVertex &a,TVertex &b);
extern float RVTX_DistanceXY (TVertex &a,TVertex &b);
extern float RVTX_DistanceXZ (TVertex &a,TVertex &b);
extern float RVTX_DistanceYZ (TVertex &a,TVertex &b);
extern float RVTX_SemiDistanceXY (TVertex &a,TVertex &b);
extern float RVTX_SemiDistanceXZ (TVertex &a,TVertex &b);
extern float RVTX_SemiDistanceYZ (TVertex &a,TVertex &b);
extern float RVTX_Length (TVertex &a);

#endif

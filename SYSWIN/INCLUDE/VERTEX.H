// AW3D - Anaconda 3D Windows Library
// Vertex

#ifndef AW3D_VERTEX
#define AW3D_VERTEX

typedef struct {
  float x,y,z;
} TVertex;

void VTX_Between (TVertex &a,TVertex &b,TVertex &c,float d);
void VTX_Set (TVertex &a,float x,float y,float z);
void VTX_Copy (TVertex &a,TVertex const &b);
void VTX_Add (TVertex &a,TVertex &b);
void VTX_Sub (TVertex &a,TVertex &b);
void VTX_Sub (TVertex &a,TVertex &b,TVertex &c);
void VTX_Mul (TVertex &a,TVertex &b);
void VTX_Div (TVertex &a,TVertex &b);
void VTX_AddValue (TVertex &a,float b);
void VTX_SubValue (TVertex &a,float b);
void VTX_MulValue (TVertex &a,float b);
void VTX_DivValue (TVertex &a,float b);
void VTX_Normalize (TVertex &a);
void VTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c);
void VTX_Neg (TVertex &a);
float VTX_DotProduct(TVertex &a,TVertex &b);
float VTX_Distance (TVertex &a,TVertex &b);
float VTX_SemiDistance (TVertex &a,TVertex &b);
float VTX_DistanceXY (TVertex &a,TVertex &b);
float VTX_DistanceXZ (TVertex &a,TVertex &b);
float VTX_DistanceYZ (TVertex &a,TVertex &b);
float VTX_SemiDistanceXY (TVertex &a,TVertex &b);
float VTX_SemiDistanceXZ (TVertex &a,TVertex &b);
float VTX_SemiDistanceYZ (TVertex &a,TVertex &b);
float VTX_Length (TVertex &a);
#endif
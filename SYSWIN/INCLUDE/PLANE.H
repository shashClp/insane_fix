// AW3D - Anaconda 3D Windows Library
// Plane

#ifndef AW3D_PLANE
#define AW3D_PLANE

#include <vertex.h>

typedef struct {
  float A,B,C,D;
} TPlane;

float PLA_DotProduct(TPlane &a,TVertex &b);
void PLA_Set (TPlane &a,float A,float B,float C,float D);
void PLA_CalcABCD (TPlane &a,TVertex &v1,TVertex &v2,TVertex &v3);
float PLA_Distance (TPlane &a,TVertex &b);
float PLA_CalcD (TPlane &a,TVertex &v);

#endif
#include "rvertex.h"
#include <math.h>
extern double sqr (double x);
// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
 void RVTX_Copy (TVertex &a,TVertex const &b)
{
  a.x = b.x;
  a.y = b.y;
  a.z = b.z;
}

float RVTX_Length (TVertex &a)
{
  return sqrt(sqr(a.x)+sqr(a.y)+sqr(a.z));
}

float RVTX_Distance (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

 void RVTX_Set (TVertex &e,float a,float b,float c)
{
  e.x = a;
  e.y = b;
  e.z = c;
}

 void RVTX_Add (TVertex &a,TVertex &b)
{
  a.x+= b.x;
  a.y+= b.y;
  a.z+= b.z;
}

 void RVTX_Sub (TVertex &a,TVertex &b)
{
  a.x-= b.x;
  a.y-= b.y;
  a.z-= b.z;
}

 void RVTX_Mul (TVertex &a,TVertex &b)
{
  a.x*= b.x;
  a.y*= b.y;
  a.z*= b.z;
}

 void RVTX_Div (TVertex &a,TVertex &b)
{
  a.x/= b.x;
  a.y/= b.y;
  a.z/= b.z;
}

 void RVTX_AddValue (TVertex &a,float b)
{
  a.x+= b;
  a.y+= b;
  a.z+= b;
}

 void RVTX_SubValue (TVertex &a,float b)
{
  a.x-= b;
  a.y-= b;
  a.z-= b;
}

 void RVTX_MulValue (TVertex &a,float b)
{
  a.x*= b;
  a.y*= b;
  a.z*= b;
}

 void RVTX_DivValue (TVertex &a,float b)
{
  float c = (b!=0) ? 1.0/b : 0.00001;
  RVTX_MulValue (a,c);
}

 void RVTX_Neg (TVertex &a)
{
  a.x = -a.x;
  a.y = -a.y;
  a.z = -a.z;
}

 float RVTX_DotProduct(TVertex &a,TVertex &b)
{
  return (a.x * b.x +
          a.y * b.y +
          a.z * b.z);
}

 float RVTX_SemiDistance (TVertex &a,TVertex &b)
{
  return  (sqr(b.x-a.x) +
              sqr(b.y-a.y) +
              sqr(b.z-a.z));
}
 float RVTX_DistanceXY (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.y-a.y));
}
 float RVTX_DistanceXZ (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.z-a.z));
}
 float RVTX_DistanceYZ (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

 float RVTX_SemiDistanceXY (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) +
              sqr(b.y-a.y));
}
 float RVTX_SemiDistanceXZ (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) +
              sqr(b.z-a.z));
}
 float RVTX_SemiDistanceYZ (TVertex &a,TVertex &b)
{
  return (sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

 void RVTX_Normalize (TVertex &a)
{
  RVTX_DivValue (a,RVTX_Length(a));
}

 void RVTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c)
{
  a.x = b.y * c.z - b.z * c.y;
  a.y = b.z * c.x - b.x * c.z;
  a.z = b.x * c.y - b.y * c.x;
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
 void RVTX_Print (TVertex &a)
{
#ifdef DEBUG_INFO
  printf ("VERTEX - ( X: %f Y: %f Z: %f )\n\r",a.x,a.y,a.z);
#endif
}


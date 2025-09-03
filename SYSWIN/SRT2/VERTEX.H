/*北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Library of Vertex
    v1.0 by Elric
    February 1999
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Comentaris:
      - Les operacions que no retornen valors sempre es realitzen sobre
      el primer operant que es passa a la funcio per referencia.
      - Es fa servir la llibreria tinymath per les funcions matematiques
      comunes.

    19/03/99: Primera implementacio.
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北*/


#ifndef LIBRARY_VERTEX
#define LIBRARY_VERTEX

#include <stdio.h>
#include "tinymath.h"

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
typedef struct {
  float x;
  float y;
  float z;
} TVertex;

typedef TVertex *LPVertex;

#define VTX_Size    sizeof(TVertex)
#define VTX_SizeDW  sizeof(TVertex)/4

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
inline void VTX_Set (TVertex &a,float x,float y,float z);
inline void VTX_Copy (TVertex &a,TVertex const &b);
inline void VTX_Add (TVertex &a,TVertex &b);
inline void VTX_Sub (TVertex &a,TVertex &b);
inline void VTX_Mul (TVertex &a,TVertex &b);
inline void VTX_Div (TVertex &a,TVertex &b);
inline void VTX_AddValue (TVertex &a,float b);
inline void VTX_SubValue (TVertex &a,float b);
inline void VTX_MulValue (TVertex &a,float b);
inline void VTX_DivValue (TVertex &a,float b);
inline void VTX_Normalize (TVertex &a);
inline void VTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c);
inline void VTX_Print (TVertex &a);
inline void VTX_Neg (TVertex &a);
inline float VTX_DotProduct(TVertex &a,TVertex &b);
inline float VTX_Distance (TVertex &a,TVertex &b);
inline float VTX_SemiDistance (TVertex &a,TVertex &b);
inline float VTX_DistanceXY (TVertex &a,TVertex &b);
inline float VTX_DistanceXZ (TVertex &a,TVertex &b);
inline float VTX_DistanceYZ (TVertex &a,TVertex &b);
inline float VTX_SemiDistanceXY (TVertex &a,TVertex &b);
inline float VTX_SemiDistanceXZ (TVertex &a,TVertex &b);
inline float VTX_SemiDistanceYZ (TVertex &a,TVertex &b);
inline float VTX_Length (TVertex &a);

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
inline void VTX_Set (TVertex &a,float x,float y,float z)
{
  a.x = x;
  a.y = y;
  a.z = z;
}

inline void VTX_Copy (TVertex &a,TVertex const &b)
{
  a.x = b.x;
  a.y = b.y;
  a.z = b.z;
}

inline void VTX_Add (TVertex &a,TVertex &b)
{
  a.x+= b.x;
  a.y+= b.y;
  a.z+= b.z;
}

inline void VTX_Sub (TVertex &a,TVertex &b)
{
  a.x-= b.x;
  a.y-= b.y;
  a.z-= b.z;
}

inline void VTX_Mul (TVertex &a,TVertex &b)
{
  a.x*= b.x;
  a.y*= b.y;
  a.z*= b.z;
}

inline void VTX_Div (TVertex &a,TVertex &b)
{
  a.x/= b.x;
  a.y/= b.y;
  a.z/= b.z;
}

inline void VTX_AddValue (TVertex &a,float b)
{
  a.x+= b;
  a.y+= b;
  a.z+= b;
}

inline void VTX_SubValue (TVertex &a,float b)
{
  a.x-= b;
  a.y-= b;
  a.z-= b;
}

inline void VTX_MulValue (TVertex &a,float b)
{
  a.x*= b;
  a.y*= b;
  a.z*= b;
}

inline void VTX_DivValue (TVertex &a,float b)
{
  float c = (b!=0) ? 1.0/b : 0.00001;
  VTX_MulValue (a,c);
}

inline void VTX_Neg (TVertex &a)
{
  a.x = -a.x;
  a.y = -a.y;
  a.z = -a.z;
}

inline float VTX_DotProduct(TVertex &a,TVertex &b)
{
  return (a.x * b.x +
          a.y * b.y +
          a.z * b.z);
}

inline float VTX_Distance (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.y-a.y) +
              sqr(b.z-a.z));
}
inline float VTX_SemiDistance (TVertex &a,TVertex &b)
{
  return  (sqr(b.x-a.x) +
              sqr(b.y-a.y) +
              sqr(b.z-a.z));
}
inline float VTX_DistanceXY (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.y-a.y));
}
inline float VTX_DistanceXZ (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.x-a.x) +
              sqr(b.z-a.z));
}
inline float VTX_DistanceYZ (TVertex &a,TVertex &b)
{
  return sqrt(sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

inline float VTX_SemiDistanceXY (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) +
              sqr(b.y-a.y));
}
inline float VTX_SemiDistanceXZ (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) +
              sqr(b.z-a.z));
}
inline float VTX_SemiDistanceYZ (TVertex &a,TVertex &b)
{
  return (sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

inline float VTX_Length (TVertex &a)
{
  return sqrt(sqr(a.x)+sqr(a.y)+sqr(a.z));
}

inline void VTX_Normalize (TVertex &a)
{
  VTX_DivValue (a,VTX_Length(a));
}

inline void VTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c)
{
  a.x = b.y * c.z - b.z * c.y;
  a.y = b.z * c.x - b.x * c.z;
  a.z = b.x * c.y - b.y * c.x;
}

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
inline void VTX_Print (TVertex &a)
{
#ifdef DEBUG_INFO
  printf ("VERTEX - ( X: %f Y: %f Z: %f )\n\r",a.x,a.y,a.z);
#endif
}

#endif

// AW3D - Anaconda 3D Windows Library
// Vertex

#include <windows.h>
#include <vertex.h>
#include <math.h>
#include <defines.h>

void VTX_Normalize (TVertex &a)
{
  float len = 1.0f / VTX_Length(a);
  a.x *=len;
  a.y *=len;
  a.z *=len;  
}


float VTX_DotProduct(TVertex &a,TVertex &b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}

float VTX_Length (TVertex &a)
{
  return (float) sqrt (a.x*a.x+a.y*a.y+a.z*a.z);
}

void VTX_Set (TVertex &a,float x,float y,float z)
{
  a.x = x;
  a.y = y;
  a.z = z;
}

float VTX_Distance (TVertex &a,TVertex &b)
{
  return (float) sqrt((b.x-a.x)*(b.x-a.x) +
                      (b.y-a.y)*(b.y-a.y) +
                      (b.z-a.z)*(b.z-a.z));
}

void VTX_Between (TVertex &a,TVertex &b,TVertex &c,float d)
{
  a.x = b.x+(c.x-b.x)*d;
  a.y = b.y+(c.y-b.y)*d;
  a.z = b.z+(c.z-b.z)*d;
}

 void VTX_Copy (TVertex &a,TVertex const &b)
{
  a.x = b.x;
  a.y = b.y;
  a.z = b.z;
}

void VTX_Add (TVertex &a,TVertex &b)
{
  a.x+= b.x;
  a.y+= b.y;
  a.z+= b.z;
}

void VTX_Sub (TVertex &a,TVertex &b)
{
  a.x-= b.x;
  a.y-= b.y;
  a.z-= b.z;
}

void VTX_Sub (TVertex &a,TVertex &b,TVertex &c)
{
  a.x = b.x-c.x;
  a.y = b.y-c.y;
  a.z = b.z-c.z;
}

void VTX_Mul (TVertex &a,TVertex &b)
{
  a.x*= b.x;
  a.y*= b.y;
  a.z*= b.z;
}

void VTX_Div (TVertex &a,TVertex &b)
{
  a.x/= b.x;
  a.y/= b.y;
  a.z/= b.z;
}

void VTX_AddValue (TVertex &a,float b)
{
  a.x+= b;
  a.y+= b;
  a.z+= b;
}

void VTX_SubValue (TVertex &a,float b)
{
  a.x-= b;
  a.y-= b;
  a.z-= b;
}

void VTX_MulValue (TVertex &a,float b)
{
  a.x*= b;
  a.y*= b;
  a.z*= b;
}

void VTX_DivValue (TVertex &a,float b)
{
  float c = (b!=0) ? 1.0f/b : 0.00001f;
  VTX_MulValue (a,c);
}

void VTX_Neg (TVertex &a)
{
  a.x = -a.x;
  a.y = -a.y;
  a.z = -a.z;
}


float VTX_SemiDistance (TVertex &a,TVertex &b)
{
  return  (sqr(b.x-a.x) +
              sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

float VTX_DistanceXY (TVertex &a,TVertex &b)
{
  return (float)sqrt(sqr(b.x-a.x) + sqr(b.y-a.y));
}

float VTX_DistanceXZ (TVertex &a,TVertex &b)
{
  return (float)sqrt(sqr(b.x-a.x) + sqr(b.z-a.z));
}

float VTX_DistanceYZ (TVertex &a,TVertex &b)
{
  return (float)sqrt(sqr(b.y-a.y) + sqr(b.z-a.z));
}

float VTX_SemiDistanceXY (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) + sqr(b.y-a.y));
}

float VTX_SemiDistanceXZ (TVertex &a,TVertex &b)
{
  return (sqr(b.x-a.x) +
              sqr(b.z-a.z));
}

float VTX_SemiDistanceYZ (TVertex &a,TVertex &b)
{
  return (sqr(b.y-a.y) +
              sqr(b.z-a.z));
}

void VTX_CrossProduct (TVertex &a,TVertex &b,TVertex &c)
{
  a.x = b.y * c.z - b.z * c.y;
  a.y = b.z * c.x - b.x * c.z;
  a.z = b.x * c.y - b.y * c.x;
}

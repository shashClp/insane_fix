// AW3D - Anaconda 3D Windows Library
// Plane

#include <windows.h>
#include <math.h>
#include <vertex.h>
#include <plane.h>

float PLA_Distance (TPlane &a,TVertex &b)
{
  return a.A * b.x + a.B * b.y + a.C * b.z - a.D;
}

float PLA_DotProduct(TPlane &a,TVertex &b)
{
  return (a.A * b.x + a.B * b.y + a.C * b.z);
}

void PLA_Set (TPlane &a,float A,float B,float C,float D)
{
  a.A = A;
  a.B = B;
  a.C = C;
  a.D = D;
}

void PLA_CalcABCD (TPlane &a,TVertex &v1,TVertex &v2,TVertex &v3)
{
  float rx1 = v2.x - v1.x;
  float ry1 = v2.y - v1.y;
  float rz1 = v2.z - v1.z;
  float rx2 = v3.x - v1.x;
  float ry2 = v3.y - v1.y;
  float rz2 = v3.z - v1.z;
  a.A = ry1 * rz2 - ry2 * rz1;
  a.B = rz1 * rx2 - rz2 * rx1;
  a.C = rx1 * ry2 - rx2 * ry1;
  float ilen = 1.0f/(float)sqrt(a.A*a.A+a.B*a.B+a.C*a.C);
  a.A *= ilen;
  a.B *= ilen;
  a.C *= ilen;
  a.D = a.A * v1.x + a.B * v1.y + a.C * v1.z;
}

float PLA_CalcD (TPlane &a,TVertex &v)
{
  return a.A * v.x + a.B * v.y + a.C * v.z;
}
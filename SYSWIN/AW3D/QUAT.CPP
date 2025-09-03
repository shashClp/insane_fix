// AW3D - Anaconda 3D Windows Library
// Quaternion

#include <windows.h>
#include <math.h>
#include <defines.h>
#include <quat.h>

void QUAT_Set(TQuat &a, float w, float x, float y, float z)
{ a.w = w; a.x = x; a.y = y; a.z = z; }

void QUAT_Add(TQuat &a, TQuat &b)
{ a.w += b.w; a.x += b.x; a.y += b.y; a.z += b.z; }

void QUAT_Add(TQuat &a, TQuat &b, TQuat &c)
{ a.w = b.w + c.w; a.x = b.x + c.x; a.y = b.y + c.y; a.z = b.z + c.z; }

void QUAT_Sub(TQuat &a, TQuat &b)
{ a.w -= b.w; a.x -= b.x; a.y -= b.y; a.z -= b.z; }

void QUAT_Sub(TQuat &a, TQuat &b, TQuat &c)
{ a.w = b.w - c.w; a.x = b.x - c.x; a.y = b.y - c.y; a.z = b.z - c.z; }

void QUAT_Mul(TQuat &a, float k)
{ a.w *= k; a.x *= k; a.y *= k; a.z *= k; }

void QUAT_Mul(TQuat &a, TQuat &b)
{
  TQuat temp;

  temp.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
  temp.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
  temp.y = a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z;
  temp.z = a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x;

  a = temp;
}

void QUAT_Mul(TQuat &r, TQuat &a, TQuat &b)
{
  TQuat res = a;

  QUAT_Mul(res,b);
  r = res;
}

void QUAT_Div(TQuat &a, TQuat &b)
{
  TQuat qaux, t, s;

  qaux = b;
  qaux.x = -qaux.x;
  qaux.y = -qaux.y;
  qaux.z = -qaux.z;
  QUAT_Mul(t,a,qaux);
  QUAT_Mul(s,qaux,qaux);
  a.w = t.w/s.w;
  a.x = t.x/s.w;
  a.y = t.y/s.w;
  a.z = t.z/s.w;
}

void QUAT_Neg(TQuat &a)
{ QUAT_Normalize(a); a.x = -a.x; a.y = -a.y; a.z = -a.z; }

void QUAT_Identity(TQuat &a)
{ QUAT_Set(a, 1, 0, 0, 0); }

void QUAT_Square(TQuat &a)
{
  float s = 2*a.w;

  a.w = (a.w*a.w - a.x*a.x - a.y*a.y - a.z*a.z);
  a.x *= s;
  a.y *= s;
  a.z *= s;
}

void QUAT_Sqrt(TQuat &a)
{
  float len, m;
  float A, B;
  TQuat r;

  len = (float)sqrt (a.w*a.w + a.x*a.x + a.y*a.y);
  if (len != 0.0f) len = 1.0f / len;
  else len = 1.0f;

  r.w = a.w * len;
  r.x = a.x * len;
  r.y = a.z * len;
  r.z = 0.0f;

  m = 1.0f / (float)sqrt (r.w*r.w + r.x*r.x);
  A = (float)sqrt ((1.0f + r.y) * 0.5f);
  B = (float)sqrt ((1.0f - r.y) * 0.5f);

  float sqrtlen = (float)sqrt(len);
  a.w = sqrtlen * B * r.w * m;
  a.x = sqrtlen * B * r.x * m;
  a.y = sqrtlen * A;
}

void QUAT_Normalize(TQuat &a)
{
  float myLength = QUAT_Length(a) ? 1.0f / QUAT_Length(a) : 0.00001f;
  a.w *= myLength;
  a.x *= myLength;
  a.y *= myLength;
  a.z *= myLength;
}

void QUAT_Inverse(TQuat &a)
{
  float mag;

  mag = a.w*a.w + a.x*a.x + a.y*a.y + a.z*a.z;
  if (mag != 0.0f) mag = 1.0f / mag;
  else mag = 1.0f;

  a.w *=  mag;
  a.x *= -mag;
  a.y *= -mag;
  a.z *= -mag;
}

void QUAT_Exp(TQuat &a)
{
  float len, len1;

  len = (float)sqrt (a.x*a.x + a.y*a.y + a.z*a.z);
  if (len > 0.0) len1 = (float)sin(len) / len;
  else len1 = 1.0f;

  a.w = (float)cos (len);
  a.x *= len1;
  a.y *= len1;
  a.z *= len1;
}

void QUAT_Log(TQuat &a)
{
  float len;

  len = (float)sqrt (a.x*a.x + a.y*a.y + a.z*a.z);
  if (a.w != 0.0) len = (float)atan (len / a.w);
  else len = M_PIDiv2;

  a.w = 0.0f;
  a.x *= len;
  a.y *= len;
  a.z *= len;
}

void QUAT_LnDif(TQuat &a, TQuat &b)
{
  TQuat inv, dif;
  float len, len1, s;

  inv = a;
  QUAT_Inverse(inv);
  QUAT_Mul(dif,inv,b);

  len = (float)sqrt (dif.x*dif.x + dif.y*dif.y + dif.z*dif.z);
  s = QUAT_DotProduct(a,b);

  if (s != 0.0f) len1 = (float)atan (len / s);
  else len1 = M_PIDiv2;

  if (len != 0.0f) len1 /= len;

  a.w = 0.0f;
  a.x = dif.x * len1;
  a.y = dif.y * len1;
  a.z = dif.z * len1;
}

void QUAT_QuatFromAngle(TQuat &q, float a, float x, float y, float z)
{
  float s, omega;

  omega = a / 2.0f;
  s = (float)sin (omega);

  q.w = (float)cos(omega);
  q.x = x * s;
  q.y = y * s;
  q.z = z * s;
}

void QUAT_QuatToAngle (TQuat q, float &ang, float &x, float &y, float &z)
{
  float s, omega;

  QUAT_Normalize(q);
  omega = (float)acos (q.w);
  ang = omega * 2.0f;
  s = (float)sin (omega);
  if (fabs (s) > QUAT_EPSILON) {
    s = 1.0f / s;
    x = q.x * s;
    y = q.y * s;
    z = q.z * s;
  } else {
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
  }
}

void QUAT_Slerp(TQuat &a, TQuat &b, float t)
{
  float k1, k2;
  float angle, cosa, isina;

  cosa = QUAT_DotUnitProduct(a,b);
  if ((1.0f - fabs(cosa)) < QUAT_EPSILON) {
    k1 = 1.0f - t;
    k2 = t;
  } else {
    angle = (float)acos (cosa);
    isina = 1.0f / (float)sin (angle);
    k1 = (float)sin((1 - t)*angle) * isina;
    k2 = (float)sin(t*angle) * isina;
  }
	a.w = k1*a.w + k2*b.w;
  a.x = k1*a.x + k2*b.x;
  a.y = k1*a.y + k2*b.y;
  a.z = k1*a.z + k2*b.z;
}

float QUAT_Length(TQuat &a)
{ return (float)sqrt(a.w*a.w + a.x*a.x + a.y*a.y + a.z*a.z); }

float QUAT_DotProduct(TQuat &a, TQuat &b)
{
  float Len = 1.0f / (QUAT_Length(a) * QUAT_Length(b));

  return (QUAT_DotUnitProduct(a,b)*Len);
}

float QUAT_DotUnitProduct(TQuat &a, TQuat &b)
{ return (float) (a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z); }

void QUAT_RotationMatrix(TMatrix &m, TQuat &a)
{
  float a2, b2, c2, d2, ab, ac, ad, bc, bd, cd;

  a2 = a.w*a.w; b2 = a.x*a.x; c2 = a.y*a.y; d2 = a.z*a.z;
  ab = a.w*a.x; ac = a.w*a.y; ad = a.w*a.z;
  bc = a.x*a.y; bd = a.x*a.z;
  cd = a.y*a.z;

  m[0][0] = a2+b2-c2-d2;
  m[0][1] = 2*(bc-ad);
  m[0][2] = 2*(bd+ac);
  m[0][3] = 0.0;
  m[1][0] = 2*(bc+ad);
  m[1][1] = a2-b2+c2-d2;
  m[1][2] = 2*(cd-ab);
  m[1][3] = 0.0;
  m[2][0] = 2*(bd-ac);
  m[2][1] = 2*(cd+ab);
  m[2][2] = a2-b2-c2+d2;
  m[2][3] = 0.0;
}


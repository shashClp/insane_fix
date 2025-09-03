////////////////////////////////////////////////////////////////////////////
//
//  QUAT.H v.0.1. - By Klauz / Anaconda. April 1999.
//
//  Library for quaternion.
//
////////////////////////////////////////////////////////////////////////////

#ifndef LIBRARY_QUAT
#define LIBRARY_QUAT

#include "matrix.h"

#define CQUAT_EPSILON 1.0e-6

class CQuat
{
public:
float w,x,y,z;

  inline CQuat ( void ) { };
  inline CQuat ( float W, float X, float Y, float Z );
  inline CQuat &operator=( CQuat const &Q );

  inline void SetElements( float W, float X, float Y, float Z );

  inline void Add ( CQuat const &Q );
  inline void Sub ( CQuat const &Q );
  inline void Mul ( CQuat const &Q );
  inline void Div ( CQuat const &Q );
  inline void Neg ( void );
  inline void Identity( void );
  inline void Square ( void );
  inline void Sqrt ( void );
  inline void Normalize( void );
  inline void Inverse( void );
  inline void Exp( void );
  inline void Log( void );
  inline void LnDif( CQuat const &Q );
  inline void QuatFromAngle ( float a, float X, float Y, float Z );
  inline void SphereInterpol (CQuat const &Q, float spin, float alpha);
  inline float Length ( void ) const;
  inline float DotProduct( CQuat const &Q );
  inline float DotUnitProduct( CQuat const &Q );

  inline TMatrix RotationMatrix ( void );
  inline TMatrix InvRotationMatrix ( void );

  inline friend CQuat operator+( CQuat const &Q1, CQuat const &Q2 );
  inline friend CQuat operator-( CQuat const &Q1, CQuat const &Q2 );
  inline friend CQuat operator-( CQuat const &Q );
  inline friend CQuat operator*( CQuat const &Q1, CQuat const &Q2 );
  inline friend CQuat operator*( CQuat const &Q, float Value );
  inline friend CQuat operator*( float Value, CQuat const &Q);
  inline friend CQuat operator/( CQuat const &Q1, CQuat const &Q2 );
  inline friend CQuat Square( CQuat const &Q );
  inline friend CQuat Sqrt( CQuat const &Q );
  inline friend CQuat Normalize( CQuat const &Q );
};

/////////////////////////////////////////////////////////////////////////////

inline CQuat::CQuat ( float W, float X, float Y, float Z )
{ w = W; x = X; y = Y; z = Z; }

inline CQuat &CQuat::operator=( CQuat const &Q )
{ w = Q.w; x = Q.x; y = Q.y; z = Q.z; return *this; }

inline void CQuat::SetElements( float W, float X, float Y, float Z )
{ w = W; x = X; y = Y; z = Z; }

inline void CQuat::Add ( CQuat const &Q )
{ w += Q.w; x += Q.x; y += Q.y; z += Q.z; }

inline void CQuat::Sub ( CQuat const &Q )
{ w += Q.w; x -= Q.x; y -= Q.y; z -= Q.z; }

inline void CQuat::Mul ( CQuat const &Q )
{
CQuat temp;

  temp.w = w*Q.w - x*Q.x - y*Q.y - z*Q.z;
  temp.x = w*Q.x + x*Q.w + y*Q.z - z*Q.y;
  temp.y = w*Q.y + y*Q.w + z*Q.x - x*Q.z;
  temp.z = w*Q.z + z*Q.w + x*Q.y - y*Q.x;

  *this = temp;
}

inline void CQuat::Div ( CQuat const &Q )
{
CQuat qaux, t, s;

  qaux = Q;
  qaux.x = -qaux.x;
  qaux.y = -qaux.y;
  qaux.z = -qaux.z;
  t = *this*qaux;
  s = qaux*qaux;

  w = t.w/s.w;
  x = t.x/s.w;
  y = t.y/s.w;
  z = t.z/s.w;
}

inline void CQuat::Neg ( void )
{ Normalize(); x = -x; y = -y; z = -z; }

inline void CQuat::Identity( void )
{ w = 1.0; x = y = z = 0; }

inline void CQuat::Square ( void )
{
float  s = 2*w;

  w = (w*w - x*x - y*y - z*z);
  x *= s;
  y *= s;
  z *= s;
}

inline void CQuat::Sqrt ( void )
{
float len, m;
float A, B;
CQuat r, temp;

  len = sqrt (w*w + x*x + y*y);
  if (len != 0.0) len = 1.0 / len;
  else len = 1.0;

  r.w = w * len;
  r.x = x * len;
  r.y = z * len;
  r.z = 0.0;

  m = 1.0 / sqrt (r.w*r.w + r.x*r.x);
  A = sqrt ((1.0 + r.y) * 0.5);
  B = sqrt ((1.0 - r.y) * 0.5);

  w = sqrt (len) * B * r.w * m;
  x = sqrt (len) * B * r.x * m;
  y = sqrt (len) * A;
}

inline void CQuat::Normalize( void )
{
  float MyLength = Length() ? 1 / Length() : 0.00001;
  w *= MyLength;
  x *= MyLength;
  y *= MyLength;
  z *= MyLength;
}

inline void CQuat::Inverse( void )
{
float mag;

  mag = w*w + x*x + y*y + z*z;
  if (mag != 1.0) mag = 1.0 / mag;
  else mag = 1.0;

  w *=  mag;
  x *= -mag;
  y *= -mag;
  z *= -mag;
}

inline void CQuat::Exp( void )
{
float len, len1;

  len = sqrt (x*x + y*y + z*z);
  if (len > 0.0) len1 = sin(len) / len;
  else len1 = 1.0;

  w = cos (len);
  x *= len1;
  y *= len1;
  z *= len1;
}

inline void CQuat::Log( void )
{
float len;

  len = sqrt (x*x + y*y + z*z);
  if (w != 0.0) len = atan (len / w);
  else len = M_PIDiv2;

  w = 0.0;
  x *= len;
  y *= len;
  z *= len;
}

inline void CQuat::LnDif( CQuat const &Q )
{
CQuat inv, dif;
float len, len1, s;

  inv = *this;
  inv.Inverse();
  dif = inv * Q;

  len = sqrt (dif.x*dif.x + dif.y*dif.y + dif.z*dif.z);
  s = DotProduct(Q);

  if (s != 0.0) len1 = atan (len / s);
  else len1 = M_PIDiv2;

  if (len != 0.0) len1 /= len;

  w = 0.0;
  x = dif.x * len1;
  y = dif.y * len1;
  z = dif.z * len1;
}

inline void CQuat::QuatFromAngle ( float a, float X, float Y, float Z )
{
float s, omega;

  omega = a / 2.0;
  s = sin (omega);

  w = cos(omega);
  x = X * s;
  y = Y * s;
  z = Z * s;
}

inline void CQuat::SphereInterpol (CQuat const &Q, float spin, float alpha)
{
float k1, k2;
float angle, anglespin;
float sina, cosa;

  cosa = DotUnitProduct(Q);
  if (1.0 - fabs (cosa) < CQUAT_EPSILON)
  {
    k1 = 1.0 - alpha;
    k2 = alpha;
  }
  else
  {
    angle = acos (cosa);
    sina = sin (angle);
    anglespin = angle + spin*M_PIDiv2;
    k1 = sin (angle - alpha*anglespin) / sina;
    k2 = sin (alpha*anglespin) / sina;
  }
  x = k1*x + k2*Q.x;
  y = k1*y + k2*Q.y;
  z = k1*z + k2*Q.z;
  w = k1*w + k2*Q.w;
}

inline float CQuat::Length( void ) const
{ return sqrt(w*w + x*x + y*y + z*z); }

inline float CQuat::DotProduct( CQuat const &Q )
{
float Len = 1.0 / (Length() * Q.Length());

return (DotUnitProduct(Q)*Len);
}

inline float CQuat::DotUnitProduct( CQuat const &Q )
{ return (float) (w*Q.w + x*Q.x + y*Q.y + z*Q.z); }

inline TMatrix CQuat::RotationMatrix( void )
{
TMatrix M;
float x2, y2, z2, wx, wy, wz, xx, xy, xz, yy, yz, zz;

  x2 = x + x;  y2 = y + y;  z2 = z + z;
  wx = w * x2; wy = w * y2; wz = w * z2;
  xx = x * x2; xy = x * y2; xz = x * z2;
  yy = y * y2; yz = y * z2; zz = z * z2;

  M.Matrix[0][0] = 1.0 - (yy + zz);
  M.Matrix[0][1] = xy + wz;
  M.Matrix[0][2] = xz - wy;
  M.Matrix[0][3] = 0.0;
  M.Matrix[1][0] = xy - wz;
  M.Matrix[1][1] = 1.0 - (xx + zz);
  M.Matrix[1][2] = yz + wx;
  M.Matrix[1][3] = 0.0;
  M.Matrix[2][0] = xz + wy;
  M.Matrix[2][1] = yz - wx;
  M.Matrix[2][2] = 1.0 - (xx + yy);
  M.Matrix[2][3] = 0.0;

  return M;
}

inline TMatrix CQuat::InvRotationMatrix( void )
{
TMatrix M;
float x2, y2, z2, wx, wy, wz, xx, xy, xz, yy, yz, zz;

  x2 = x + x;  y2 = y + y;  z2 = z + z;
  wx = w * x2; wy = w * y2; wz = w * z2;
  xx = x * x2; xy = x * y2; xz = x * z2;
  yy = y * y2; yz = y * z2; zz = z * z2;

  M.Matrix[0][0] = 1.0 - (yy + zz);
  M.Matrix[0][1] = xy - wz;
  M.Matrix[0][2] = xz + wy;
  M.Matrix[0][3] = 0.0;
  M.Matrix[1][0] = xy + wz;
  M.Matrix[1][1] = 1.0 - (xx + zz);
  M.Matrix[1][2] = yz - wx;
  M.Matrix[1][3] = 0.0;
  M.Matrix[2][0] = xz - wy;
  M.Matrix[2][1] = yz + wx;
  M.Matrix[2][2] = 1.0 - (xx + yy);
  M.Matrix[2][3] = 0.0;

  return M;
}

////////////////////////////////////////////////////////////////////////////

inline CQuat operator+( CQuat const &Q1,CQuat const &Q2 )
{ return CQuat (Q1.w+Q2.w, Q1.x+Q2.x, Q1.y+Q2.y, Q1.z+Q2.z); }

inline CQuat operator-( CQuat const &Q1, CQuat const &Q2 )
{ return CQuat (Q1.w-Q2.w, Q1.x-Q2.x, Q1.y-Q2.y, Q1.z-Q2.z); }

inline CQuat operator-( CQuat const &Q )
{
CQuat qaux = Q;

  qaux.Normalize();
  return CQuat (qaux.w,-qaux.x,-qaux.y,-qaux.z);
}

inline CQuat operator*( CQuat const &Q1,CQuat const &Q2 )
{
CQuat qaux = Q1;

  qaux.Mul(Q2);
  return qaux;
}

inline CQuat operator*( CQuat const &Q, float Value )
{ return CQuat (Q.w*Value, Q.x*Value, Q.y*Value, Q.z*Value); }

inline CQuat operator*( float Value, CQuat const &Q )
{ return CQuat (Q.w*Value, Q.x*Value, Q.y*Value, Q.z*Value); }

inline CQuat operator/( CQuat const &Q1,CQuat const &Q2 )
{
CQuat qaux = Q1;

  qaux.Div(Q2);
  return qaux;
}

inline CQuat Square( CQuat const &Q )
{
CQuat qaux = Q;

  qaux.Square();
  return qaux;
}

inline CQuat Sqrt( CQuat const &Q )
{
CQuat qaux = Q;

  qaux.Sqrt();
  return qaux;
}

inline CQuat Normalize( CQuat const &Q )
{
CQuat qaux = Q;

  qaux.Normalize();
  return qaux;
}

#endif

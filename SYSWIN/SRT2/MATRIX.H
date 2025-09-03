/*北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Library of Matrix
    v1.0 by Elric & Klauz
    February 1999
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Comentaris:
      - Les operacions que no retornen valors sempre es realitzen sobre
      el primer operant que es passa a la funcio per referencia.
      - Es fa servir la llibreria tinymath per les funcions matematiques
      comunes.

    19/03/99: Primera implementacio.
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北*/

#ifndef LIBRARY_MATRIX
#define LIBRARY_MATRIX

#include "library.h"
#include "mem.h"
#include "vertex.h"
#include "plane.h"

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

TMatrix MTX_IdentityMatrix = { 1,0,0,0, 0,1,0,0, 0,0,1,0 };

inline void MTX_Identity (TMatrix &a);
inline void MTX_Copy (TMatrix &a,TMatrix &b);
inline void MTX_Rotation (TMatrix &a,float x,float y,float z);
inline void MTX_Mul (TVertex &a,TVertex &b,TMatrix &c);
inline void MTX_Mul (TPlane &a,TPlane &b,TMatrix &c);
inline void MTX_Mul (TMatrix &a,TMatrix &b,TMatrix &c);
inline void MTX_Mul (TMatrix &a,TMatrix &b,float c);
void MTX_Inverse (TMatrix &b,TMatrix &a);

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void MTX_Inverse (TMatrix &b, TMatrix &a)
{
  // Calcul d'una matriu inversa. Matriu 3x4
  float Det;
  TMatrix c;

  // Calculem el determinant de la matriu A.
  Det = a.Matrix[0][0]*a.Matrix[1][1]*a.Matrix[2][2]+
        a.Matrix[0][1]*a.Matrix[1][2]*a.Matrix[2][0]+
        a.Matrix[1][0]*a.Matrix[2][1]*a.Matrix[0][2]-
        a.Matrix[0][2]*a.Matrix[1][1]*a.Matrix[2][0]-
        a.Matrix[1][0]*a.Matrix[0][1]*a.Matrix[2][2]-
        a.Matrix[2][1]*a.Matrix[1][2]*a.Matrix[0][0];

  if (Det == 0.0)
  {
    // matematicament hi haura error (divisio per 0)
    return;
  }

  // Calculem la matriu cofactor transposada.
  c.Matrix[0][0] =  (a.Matrix[1][1]*a.Matrix[2][2]-a.Matrix[2][1]*a.Matrix[1][2]);
  c.Matrix[1][0] = -(a.Matrix[1][0]*a.Matrix[2][2]-a.Matrix[2][0]*a.Matrix[1][2]);
  c.Matrix[2][0] =  (a.Matrix[1][0]*a.Matrix[2][1]-a.Matrix[2][0]*a.Matrix[1][1]);

  c.Matrix[0][1] = -(a.Matrix[0][1]*a.Matrix[2][2]-a.Matrix[2][1]*a.Matrix[0][2]);
  c.Matrix[1][1] =  (a.Matrix[0][0]*a.Matrix[2][2]-a.Matrix[2][0]*a.Matrix[0][2]);
  c.Matrix[2][1] = -(a.Matrix[0][0]*a.Matrix[2][1]-a.Matrix[2][0]*a.Matrix[0][1]);

  c.Matrix[0][2] =  (a.Matrix[0][1]*a.Matrix[1][2]-a.Matrix[1][1]*a.Matrix[0][2]);
  c.Matrix[1][2] = -(a.Matrix[0][0]*a.Matrix[1][2]-a.Matrix[1][0]*a.Matrix[0][2]);
  c.Matrix[2][2] =  (a.Matrix[0][0]*a.Matrix[1][1]-a.Matrix[1][0]*a.Matrix[0][1]);

  c.Matrix[0][3] = -(a.Matrix[0][1]*a.Matrix[1][2]*a.Matrix[2][3]+
                     a.Matrix[0][2]*a.Matrix[1][3]*a.Matrix[2][1]+
                     a.Matrix[1][1]*a.Matrix[2][2]*a.Matrix[0][3]-
                     a.Matrix[0][3]*a.Matrix[1][2]*a.Matrix[2][1]-
                     a.Matrix[1][1]*a.Matrix[0][2]*a.Matrix[2][3]-
                     a.Matrix[2][2]*a.Matrix[1][3]*a.Matrix[0][1]);

  c.Matrix[1][3] = a.Matrix[0][0]*a.Matrix[1][2]*a.Matrix[2][3]+
                   a.Matrix[0][2]*a.Matrix[1][3]*a.Matrix[2][0]+
                   a.Matrix[1][0]*a.Matrix[2][2]*a.Matrix[0][3]-
                   a.Matrix[0][3]*a.Matrix[1][2]*a.Matrix[2][0]-
                   a.Matrix[1][0]*a.Matrix[0][2]*a.Matrix[2][3]-
                   a.Matrix[2][2]*a.Matrix[1][3]*a.Matrix[0][0];

  c.Matrix[2][3] = -(a.Matrix[0][0]*a.Matrix[1][1]*a.Matrix[2][3]+
                     a.Matrix[0][1]*a.Matrix[1][3]*a.Matrix[2][0]+
                     a.Matrix[1][0]*a.Matrix[2][1]*a.Matrix[0][3]-
                     a.Matrix[0][3]*a.Matrix[1][1]*a.Matrix[2][0]-
                     a.Matrix[1][0]*a.Matrix[0][1]*a.Matrix[2][3]-
                     a.Matrix[2][1]*a.Matrix[1][3]*a.Matrix[0][0]);

  for(long i=0; i<3; i++)
  {
    for(long j=0; j<4; j++)
    {
      b.Matrix[i][j] = c.Matrix[i][j]/Det;
    }
  }
}

inline void MTX_Identity (TMatrix &a)
{
  CopyMem (&MTX_IdentityMatrix,&a,sizeof(TMatrix)/4);
}

inline void MTX_Copy (TMatrix &a,TMatrix &b)
{
  CopyMem (&b,&a,sizeof(TMatrix)/4);
}

inline void MTX_Rotation (TMatrix &a,float x,float y,float z)
{
  x = x * M_ToRad;
  y = y * M_ToRad;
  z = z * M_ToRad;
  float sinx = sin (x);
  float siny = sin (y);
  float sinz = sin (z);
  float cosx = cos (x);
  float cosy = cos (y);
  float cosz = cos (z);

  a.Matrix[0][0] = cosz * cosy;
  a.Matrix[0][1] =-sinz * cosx + cosz * siny * sinx;
  a.Matrix[0][2] =-sinz *-sinx + cosz * siny * cosx;
  a.Matrix[0][3] = 0;
  a.Matrix[1][0] = sinz * cosy;
  a.Matrix[1][1] = cosz * cosx + sinz * siny * sinx;
  a.Matrix[1][2] = cosz *-sinx + sinz * siny * cosx;
  a.Matrix[1][3] = 0;
  a.Matrix[2][0] = -siny;
  a.Matrix[2][1] = cosy * sinx;
  a.Matrix[2][2] = cosy * cosx;
  a.Matrix[2][3] = 0;
}

inline void MTX_Mul (TPlane &a,TPlane &b,TMatrix &c)
{
  a.A = b.A * c.Matrix[0][0]+
        b.B * c.Matrix[0][1]+
        b.C * c.Matrix[0][2];
  a.B = b.A * c.Matrix[1][0]+
        b.B * c.Matrix[1][1]+
        b.C * c.Matrix[1][2];
  a.C = b.A * c.Matrix[2][0]+
        b.B * c.Matrix[2][1]+
        b.C * c.Matrix[2][2];
}

inline void MTX_Mul (TVertex &a,TVertex &b,TMatrix &c)
{
  a.x = b.x * c.Matrix[0][0]+
        b.y * c.Matrix[0][1]+
        b.z * c.Matrix[0][2]+c.Matrix[0][3];
  a.y = b.x * c.Matrix[1][0]+
        b.y * c.Matrix[1][1]+
        b.z * c.Matrix[1][2]+c.Matrix[1][3];
  a.z = b.x * c.Matrix[2][0]+
        b.y * c.Matrix[2][1]+
        b.z * c.Matrix[2][2]+c.Matrix[2][3];
}

inline void MTX_Mul (TMatrix &a,TMatrix &b,float c)
{
  a.Matrix[0][0]=b.Matrix[0][0]*c;
  a.Matrix[0][1]=b.Matrix[0][0]*c;
  a.Matrix[0][2]=b.Matrix[0][0]*c;
  a.Matrix[1][0]=b.Matrix[1][0]*c;
  a.Matrix[1][1]=b.Matrix[1][0]*c;
  a.Matrix[1][2]=b.Matrix[1][0]*c;
  a.Matrix[2][0]=b.Matrix[2][0]*c;
  a.Matrix[2][1]=b.Matrix[2][0]*c;
  a.Matrix[2][2]=b.Matrix[2][0]*c;
}

inline void MTX_Mul (TMatrix &a,TMatrix &b,TMatrix &c)
{
  a.Matrix[0][0]=b.Matrix[0][0]*c.Matrix[0][0]+b.Matrix[0][1]*c.Matrix[1][0]+b.Matrix[0][2]*c.Matrix[2][0];
  a.Matrix[0][1]=b.Matrix[0][0]*c.Matrix[0][1]+b.Matrix[0][1]*c.Matrix[1][1]+b.Matrix[0][2]*c.Matrix[2][1];
  a.Matrix[0][2]=b.Matrix[0][0]*c.Matrix[0][2]+b.Matrix[0][1]*c.Matrix[1][2]+b.Matrix[0][2]*c.Matrix[2][2];
  a.Matrix[0][3]=b.Matrix[0][0]*c.Matrix[0][3]+b.Matrix[0][1]*c.Matrix[1][3]+b.Matrix[0][2]*c.Matrix[2][3]+b.Matrix[0][3];
  a.Matrix[1][0]=b.Matrix[1][0]*c.Matrix[0][0]+b.Matrix[1][1]*c.Matrix[1][0]+b.Matrix[1][2]*c.Matrix[2][0];
  a.Matrix[1][1]=b.Matrix[1][0]*c.Matrix[0][1]+b.Matrix[1][1]*c.Matrix[1][1]+b.Matrix[1][2]*c.Matrix[2][1];
  a.Matrix[1][2]=b.Matrix[1][0]*c.Matrix[0][2]+b.Matrix[1][1]*c.Matrix[1][2]+b.Matrix[1][2]*c.Matrix[2][2];
  a.Matrix[1][3]=b.Matrix[1][0]*c.Matrix[0][3]+b.Matrix[1][1]*c.Matrix[1][3]+b.Matrix[1][2]*c.Matrix[2][3]+b.Matrix[1][3];
  a.Matrix[2][0]=b.Matrix[2][0]*c.Matrix[0][0]+b.Matrix[2][1]*c.Matrix[1][0]+b.Matrix[2][2]*c.Matrix[2][0];
  a.Matrix[2][1]=b.Matrix[2][0]*c.Matrix[0][1]+b.Matrix[2][1]*c.Matrix[1][1]+b.Matrix[2][2]*c.Matrix[2][1];
  a.Matrix[2][2]=b.Matrix[2][0]*c.Matrix[0][2]+b.Matrix[2][1]*c.Matrix[1][2]+b.Matrix[2][2]*c.Matrix[2][2];
  a.Matrix[2][3]=b.Matrix[2][0]*c.Matrix[0][3]+b.Matrix[2][1]*c.Matrix[1][3]+b.Matrix[2][2]*c.Matrix[2][3]+b.Matrix[2][3];
}

#endif
